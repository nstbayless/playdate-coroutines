#if defined(__arm__) || defined(__thumb__)
    #define SETSP(x) asm volatile(\
        "mov sp, %0" \
        : : "r" (x) \
    );
    #define COROUTINE_INC "coroutine.arm.inc"
#elif defined (__i386__)
#define PDCO_FORCE_COMPILE
    #define SETSP(x) asm volatile(\
        "movl %0, %%esp \n movl %0, %%ebp" \
        : : "r" (x) \
    );
    #define COROUTINE_INC "coroutine.i386.inc"
#elif defined (__x86_64__)
    #define PDCO_FORCE_COMPILE
    #define SETSP(x) asm volatile(\
        "movq %0, %%rsp \n movq %0, %%rbp" \
        : : "r" (x) \
    );
    #define COROUTINE_INC "coroutine.x64.inc"
#else
    #error "coroutines not supported on this platform"
#endif

#if !defined(PDCO_FORCE_COMPILE) && __has_include(COROUTINE_INC)
    #define PDCO_USE_PRECOMPILED
#endif

#ifndef PDCO_USE_PRECOMPILED
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "coroutine.h"

#if __has_include(<ucontext.h>)
    #define USE_UCONTEXT
    #include <ucontext.h>
    #include <assert.h>
#else
    #define USE_SETJMP
    #include <setjmp.h>
#endif

#pragma GCC optimize("O1")

#if defined(__arm__) || defined(__thumb__)

#elif defined(__x86_64__)

#else
#pragma error "coroutine.c not supported on this architecture"
#endif

__attribute__((noinline))
static int stack_direction_helper(int* x)
{
    int y;
    return ((uintptr_t)&y < (uintptr_t)&x) ? -1 : 1;
}

// returns 1 if stack grows toward positive, -1 if grows toward negative.
static inline int stack_direction(void)
{
    int x;
    return stack_direction_helper(&x);
}

typedef struct coroutine_t
{
    int id;
    struct coroutine_t* next;
    struct coroutine_t* yields_to;
    int status; // 1 = running
    void* tx;
    pdco_fn_t fn;
    #ifdef USE_SETJMP
    jmp_buf jbcaller;
    jmp_buf jbalt;
    #else // USE_UCONTEXT
    ucontext_t uccaller;
    ucontext_t ucalt;
    #endif
    size_t stacksize;
    void* stack;
    void* stackstart;
} coroutine_t;

static coroutine_t* pdco_first = NULL;

// Current coroutine (NULL if not in a coroutine)
// It is the caller's responsibility to maintain this.
static coroutine_t* volatile pdco_active = NULL;
static int next_coroutine_id = 1;

// delete coroutine if it exists
// returns 1 if deleted.
static int delco(int id)
{
    coroutine_t** ll = &pdco_first;
    coroutine_t* tmp;
    while ((tmp = *ll))
    {
        if (tmp->id == id)
        {
            *ll = tmp->next;
            if (tmp->stack)
            {
                free(tmp->stack);
                tmp->stack = NULL;
            }
            free(tmp);
            return 1;
        }
    }
    
    return 0;
}

static inline int cleanup_if_complete(coroutine_t* co)
{
    if (co->status == 0)
    {
        delco(co->id);
        return 0;
    }
    return 1;
}

// safety bytes.
#define STACKMARGIN 48

static inline void* getstackstart(coroutine_t* co)
{
    // !! FIXME !!
    return co->stack + co->stacksize / 2;
    
    return (stack_direction() == 1)
        ? co->stack + STACKMARGIN
        : (co->stack + co->stacksize - STACKMARGIN);
}

static coroutine_t* getco(int id)
{
    coroutine_t* co = pdco_first;
    while (co && co->id != id)
    {
        co = co->next;
    }
    
    return co;
}

void pdco_yield(void)
{
    #ifdef USE_SETJMP
        asm("");
        if (setjmp(pdco_active->jbalt) == 0)
        {
            longjmp(pdco_active->jbcaller, 1);
        }
    #else // USE_UCONTEXT
        swapcontext(&pdco_active->ucalt, &pdco_active->uccaller);
    #endif
}

__attribute__((noinline))
static void pdco_guard(void)
{
    asm("");
    pdco_active->fn();
    asm("");
    
    // final yield.
    pdco_active->status = 0;
    pdco_yield();
}

// runs after returning/yielding from the swapped-to context.
static int resume_epilogue(void)
{
    coroutine_t* nc = pdco_active;
    pdco_active = nc->yields_to;
    return cleanup_if_complete(nc)
        ? nc->id
        : 0;
}

static int pdco_resume_(coroutine_t* nc)
{
    // cannot resume active (current) coroutine.
    if (!nc) return -1;
    if (nc == pdco_active) return -1;
    nc->yields_to = pdco_active;
    pdco_active = nc;
    
    #ifdef USE_SETJMP
        asm("");
        if (setjmp(nc->jbcaller) == 0)
        {
            longjmp(nc->jbalt, 1);
        }
    #else
        swapcontext(&nc->uccaller, &nc->ucalt);
    #endif    
    
    return resume_epilogue();
}

__attribute__((noinline))
int pdco_run(pdco_fn_t fn, size_t stacksize)
{
    coroutine_t* nc = (coroutine_t*)malloc(sizeof(coroutine_t));
    if (!nc) return -1;
    
    #ifdef USE_UCONTEXT
        memset(&nc->uccaller, 0, sizeof(ucontext_t));
        if (getcontext(&nc->uccaller) < 0)
        {
            free(nc);
            return -1;
        }
    #endif
    
    nc->stack = NULL;
    
    if (stacksize == 0) stacksize = 1 << 10;
    
    nc->stack = malloc(stacksize);
    if (!nc->stack)
    {
        free(nc);
        return -1;
    }
    
    nc->stackstart = getstackstart(nc);
    nc->stacksize = stacksize;
    nc->status = 1; // running
    nc->next = pdco_first;
    pdco_first = nc;
    nc->id = next_coroutine_id++;
    nc->fn = fn;
    
    #ifdef USE_SETJMP
        asm("");
        if (setjmp(nc->jbalt) != 0)
        {
            // ---- inside coroutine ----
            
            // set stack pointer
            SETSP(nc->stackstart);
            
            // call coroutine
            pdco_guard();
        }
    #else // USE_UCONTEXT
        nc->ucalt.uc_stack.ss_sp = nc->stackstart; // or should this be nc->stack?
        nc->ucalt.uc_stack.ss_size = nc->stacksize;
        nc->ucalt.uc_link = NULL; // we take care of return ourselves.
        memset(&nc->ucalt, 0, sizeof(ucontext_t));
        nc->ucalt.uc_mcontext.fpregs = &nc->ucalt.__fpregs_mem;
        
        makecontext(&nc->ucalt, pdco_guard, 0);
        
        // there's a bug for some reason in some implementations of makecontext
        // where fpregs is left NULL.
        if (nc->ucalt.uc_mcontext.fpregs == NULL)
        {
            nc->ucalt.uc_mcontext.fpregs = &nc->ucalt.__fpregs_mem;
        }
    #endif
    return pdco_resume_(nc);
}

int pdco_get_coroutine(void)
{
    if (pdco_active == NULL)
    {
        return 0;
    }
    else
    {
        return pdco_active->id;
    }
}

void** pdco_ud(int co)
{
    coroutine_t* c = pdco_active;
    if (co > 0) c = getco(co);
    if (c)
    {
        return &c->tx;
    }
    else
    {
        return NULL;
    }
}

int pdco_resume(int co)
{
    register coroutine_t* nc = getco(co);
    return pdco_resume_(nc);
}

int pdco_kill(int co)
{
    // can't kill the active coroutine.
    if (pdco_active && co == pdco_active->id)
    {
        return -1;
    }
    
    return delco(co)
        ? 0
        : co;
}

#else
    // precompiled versions
    #include COROUTINE_INC
#endif