#define PDCO_FORCE_COMPILE

#if defined(__arm__) || defined(__thumb__)
    #define SETSP(x) asm volatile(\
        "mov sp, %0" \
        : : "r" (x) \
    );
    #define COROUTINE_INC "coroutine.arm.inc"
#elif defined (__i386__)
    #define SETSP(x) asm volatile(\
        "movl %0, %%esp \n movl %0, %%ebp" \
        : : "r" (x) \
    );
    #define COROUTINE_INC "coroutine.i386.inc"
#elif defined (__x86_64__)
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
#include <setjmp.h>
#include <stdlib.h>
#include <string.h>

//#include "coroutine.h"
typedef void*(*pdco_fn_t)(void*);

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
static inline int stack_direction()
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
    pdco_fn_t fn;
    jmp_buf jbcaller;
    jmp_buf jbalt;
    size_t stacksize;
    void* stack;
    void* stackstart;
    void* tx;
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
    while (tmp = *ll)
    {
        if (tmp->id == id)
        {
            *ll = tmp->next;
            //free(tmp);
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

__attribute__((noinline))
int pdco_run(pdco_fn_t fn, size_t stacksize, void* udata, void** value)
{
    register coroutine_t* nc = (coroutine_t*)malloc(sizeof(coroutine_t));
    if (!nc) return -1;
    
    if (stacksize == 0) stacksize = 1 << 10;
    
    nc->stack = malloc(stacksize);
    if (!nc->stack)
    {
        free(nc);
        return -1;
    }
    
    nc->stackstart = getstackstart(nc);
    nc->status = 1; // running
    nc->yields_to = pdco_active;
    nc->stacksize = stacksize;
    nc->next = pdco_first;
    nc->id = next_coroutine_id++;
    nc->fn = fn;
    nc->tx = udata;
    if (setjmp(nc->jbalt) != 0)
    {
        // ---- inside coroutine ----
        
        // set stack pointer
        SETSP(nc->stackstart);
        
        // call coroutine
        pdco_active->tx = pdco_active->fn(pdco_active->tx);
        
        // final yield
        pdco_active->status = 0;
        longjmp(pdco_active->jbcaller, 1);
    }
    
    // ---- outside coroutine ----
    
    pdco_active = nc->yields_to;
    pdco_first = nc;
    
    if (setjmp(nc->jbcaller) == 0)
    {
        // ---- resume coroutine ----
        pdco_active = nc;
        longjmp(nc->jbalt, 1);
    }
    else
    {
        // ---- coroutine yielded / exited ----
        pdco_active = nc->yields_to;
        if (value) *value = nc->tx;
        return cleanup_if_complete(nc)
            ? nc->id
            : 0;
    }
}

void* pdco_yield(void* udata)
{
    pdco_active->tx = udata;
    
    if (setjmp(pdco_active->jbalt) == 0)
    {
        longjmp(pdco_active->jbcaller, 1);
    }
    else
    {
        return pdco_active->tx;
    }
}

int pdco_resume(int co, void* udata, void** retval)
{
    register coroutine_t* nc = getco(co);
    if (!nc) return -1;
    if (nc == pdco_active) return -1;
    nc->yields_to = pdco_active;
    nc->tx = udata;
    if (setjmp(nc->jbcaller) == 0)
    {
        pdco_active = nc;
        longjmp(nc->jbalt, 1);
    }
    else
    {
        pdco_active = nc->yields_to;
        if (retval) *retval = nc->tx;
        return cleanup_if_complete(nc)
            ? nc->id
            : 0;
    }
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