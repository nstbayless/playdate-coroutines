#if defined(__arm__) || defined(__thumb__)
    #define SETSP(x) asm volatile(\
        "mov sp, %0" \
        : : "r" (x) \
    );
#endif

#define _GNU_SOURCE

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if !defined(TARGET_PLAYDATE) || defined(NEWLIB_COMPLETE)
#include <assert.h>
#include <stdio.h>
#endif

#include "pdco.h"

#if __has_include(<ucontext.h>)
    #define USE_UCONTEXT
    #include <ucontext.h>
#else
    #define USE_SETJMP
    #include <setjmp.h>
#endif

#pragma GCC optimize("O1")

#ifndef NDEBUG
static const char* canary_context = "";
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

#define CANARYC1 0xC0DEDEAD
#define CANARYC2 0xDEADC0DE
#define CANARYS1 0xDEADDA7E
#define CANARYS2 0xDA7EDEAD

typedef struct coroutine_t
{
    int canary1;
    pdco_handle_t id;
    
    // next in linked list
    struct coroutine_t* next;
    
    int status; // if this is set to 0, means marked as dirty and should be cleaned up.
    
    // on main thread, these are all left unset.
    pdco_handle_t creator;
    int has_started;
    pdco_fn_t fn;
    size_t stacksize;
    size_t txsize;
    void* ud;
    void* stackstart;
    void* stack; // stack followed by user-defined data
    
    #ifdef USE_SETJMP
    jmp_buf jbalt;
    #else // USE_UCONTEXT
    ucontext_t ucalt;
    #endif
    int canary2;
} coroutine_t;

static coroutine_t co_main;

// linked list
static coroutine_t* pdco_first = NULL;

// Current coroutine
// It is the caller's responsibility to maintain this.
static coroutine_t* volatile pdco_active = NULL;
static coroutine_t* volatile pdco_prev = NULL;
static int next_coroutine_id = PDCO_MAIN_ID + 1;

static int is_init = 0;
static void init(void)
{
    if (!is_init)
    {
        co_main.id = PDCO_MAIN_ID;
        co_main.status = 1;
        co_main.ud = NULL;
        pdco_active = &co_main;
        
        is_init = 1;
    }
}

// delete coroutine if it exists and is not main
static void delco(int id)
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
            return;
        }
    }
}

static inline void cleanup_if_complete(coroutine_t* co)
{
    if (co->status == 0) delco(co->id);
}

// safety bytes.
#define STACKMARGIN 48

static inline void* getstackstart(coroutine_t* co)
{
    #ifdef USE_UCONTEXT
    return co->stack + STACKMARGIN;
    #endif
    
    // !! FIXME !!
    // return co->stack + co->stacksize / 2;
    
    return (stack_direction() == 1)
        ? co->stack + STACKMARGIN
        : (co->stack + co->stacksize - STACKMARGIN);
}

// gets coroutine from handle or NULL if does not exist
static coroutine_t* getco(pdco_handle_t id)
{
    if (id <= 0) return NULL;
    if (id == PDCO_MAIN_ID) return &co_main;
    
    coroutine_t* co = pdco_first;
    while (co && co->id != id)
    {
        co = co->next;
    }
    
    return co;
}

#ifndef NDEBUG
static int canaryok(coroutine_t* co)
{
    if (co == &co_main) return 0;
    
    if (!co) return 1;
    if (*(uint32_t*)(co->stack) != CANARYS1) return 4;
    if (*(uint32_t*)(co->stack + co->stacksize - sizeof(uint32_t)) != CANARYS2) return 5;
    if (co->canary1 != CANARYC1) return 2;
    if (co->canary2 != CANARYC2) return 3;
    
    return 0;
}

static void canarycheck(coroutine_t* co)
{
    #if !defined(TARGET_PLAYDATE) || defined(NEWLIB_COMPLETE)
    if (co == &co_main) return;
    
    int error = 0;
    switch (canaryok(co))
    {
    case 0:
        break;
    case 1:
        printf("PDCO ERROR: coroutine is null (id %d) %s\n", co->id, canary_context);
        error = 1;
        break;
    case 2:
        printf("PDCO ERROR: canary C1 died (id %d) %s\n", co->id, canary_context);
        error = 1;
        break;
    case 3:
        printf("PDCO ERROR: canary C2 died (id %d) %s\n", co->id, canary_context);
        error = 1;
        break;
    case 4:
        printf("PDCO ERROR: canary S1 died (id %d) %s\n", co->id, canary_context);
        error = 1;
        break;
    case 5:
        printf("PDCO ERROR: canary S2 died (id %d) %s\n", co->id, canary_context);
        error = 1;
        break;
    default:
        printf("PDCO ERROR: unknown canary error (id %d) %s\n", co->id, canary_context);
        error = 1;
        break;
    }
    
    #if defined(__x86_64__) && defined(USE_UCONTEXT)
    gregset_t greg;
    void* rsp = *(void**)&(co->ucalt.uc_mcontext.gregs[REG_RSP]);
    if (rsp && co->has_started)
    {
        if (rsp < co->stack || rsp >= co->stack + co->stacksize)
        {
            printf(
                "PDCO ERROR: stack %d pointer out of bounds, is %p but stack is %p to %p\n",
                co->id, rsp, co->stack, co->stack + co->stacksize
            );
            if (rsp < co->stack)
            {
                printf(
                    "Note: stack size is 0x%lx bytes, and rsp is below the bottom of the stack by 0x%lx bytes\n",
                    co->stacksize, (uintptr_t)(co->stack - rsp)
                );
            }
            else
            {
                printf(
                    "Note: stack size is 0x%lx bytes, and rsp is above the top of the stack by 0x%lx bytes\n",
                    co->stacksize, (uintptr_t)(rsp - (co->stack + co->stacksize))
                );
            }
            printf("Context: %s\n", canary_context);
            error = 1;
        }
    }
    #endif
    
    assert(!error);
    canary_context = "";
    #endif
}
#endif

__attribute__((noinline))
static void pdco_guard(void)
{
    pdco_active->has_started = 1;
    #ifndef NDEBUG
    canary_context = "guard (check prev)";
    canarycheck(pdco_prev);
    canary_context = "guard (check next)";
    canarycheck(pdco_active);
    #endif
    
    cleanup_if_complete(pdco_prev);
    
    register pdco_handle_t resume_next;
    asm("");
    resume_next = pdco_active->fn(pdco_active->creator);
    asm("");
    
    #ifndef NDEBUG
    canary_context = "guard (end)";
    canarycheck(pdco_active);
    #endif
    
    // final yield.
    pdco_active->status = 0; // mark thread as dirty / clean up
    pdco_yield(resume_next); // after resuming, the next thread will clean us up.
}

static int prev_id;

// returns -1 if failure.
__attribute__((noinline))
static int pdco_resume_(coroutine_t* nc)
{
    // cannot resume active (current) coroutine.
    if (!nc) return -1;
    if (nc == pdco_active) return -1;
    pdco_prev = pdco_active;
    pdco_active = nc;
    
    #ifndef NDEBUG
    canary_context = "yield/resume (pre-swap)";
    canarycheck(pdco_active);
    #endif
    
    #ifdef USE_SETJMP
        asm("");
        if (setjmp(pdco_prev->jbalt) == 0)
        {
            longjmp(nc->jbalt, 1);
        }
    #else
        swapcontext(&pdco_prev->ucalt, &nc->ucalt);
    #endif    
    
    #ifndef NDEBUG
    canary_context = "yield/resume (post-swap)";
    canarycheck(pdco_active);
    #endif
    
    prev_id = pdco_prev->id;
    cleanup_if_complete(pdco_prev);
    return 0;
}

pdco_handle_t pdco_yield(pdco_handle_t h)
{
    coroutine_t* nc = getco(h);
    #ifndef NDEBUG
    canary_context = "yield";
    canarycheck(nc);
    #endif
    if (pdco_resume_(nc) != 0)
    {
        #if !defined(TARGET_PLAYDATE) || defined(NEWLIB_COMPLETE)
        assert(0);
        #endif
        
        // causes crash
        pdco_resume_(nc);
    }
    
    return prev_id;
}

pdco_handle_t pdco_create(pdco_fn_t fn, size_t stacksize, void* ud)
{
    init();
    
    #if defined(__arm__) || defined(__clang__)
    register
    #endif
    coroutine_t* nc = (coroutine_t*)malloc(sizeof(coroutine_t));
    if (!nc) return -1;
    memset(nc, 0, sizeof(coroutine_t));
    nc->canary1 = CANARYC1;
    nc->canary2 = CANARYC2;
    
    #ifdef USE_UCONTEXT
        if (getcontext(&pdco_active->ucalt) < 0)
        {
            free(nc);
            return -1;
        }
    #endif
    
    nc->stack = NULL;
    if (stacksize == 0) stacksize = 1 << 16;
    nc->stacksize = stacksize;
    nc->stack = malloc(stacksize);
    if (!nc->stack)
    {
        free(nc);
        return -1;
    }
    *(uint32_t*)(nc->stack) = CANARYS1;
    *(uint32_t*)(nc->stack + stacksize - sizeof(uint32_t)) = CANARYS2;
    #ifndef NDEBUG
    canary_context = "create (start)";
    canarycheck(nc);
    #endif
    nc->ud = ud;
    nc->stackstart = getstackstart(nc);
    #if !defined(TARGET_PLAYDATE) || defined(NEWLIB_COMPLETE)
    assert(nc->stackstart >= nc->stack);
    assert(nc->stackstart <= nc->stack + nc->stacksize);
    #endif
    nc->status = 1; // running
    nc->fn = fn;
    nc->creator = pdco_active->id;
    nc->has_started = 0;
    
    #ifndef NDEBUG
    canary_context = "create (middle)";
    canarycheck(nc);
    #endif
    
    // assign unique ID
    nc->id = next_coroutine_id++;
    while (getco(nc->id)) nc->id = next_coroutine_id++;
    
    // attach to linked list
    nc->next = pdco_first;
    pdco_first = nc;
    
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
        nc->ucalt.uc_stack.ss_size = nc->stacksize - 2 * STACKMARGIN;
        nc->ucalt.uc_link = NULL; // we take care of return ourselves.
        makecontext(&nc->ucalt, pdco_guard, 0);
        
        // there's a bug for some reason in some implementations of makecontext
        // where fpregs is left NULL.
        if (nc->ucalt.uc_mcontext.fpregs == NULL)
        {
            nc->ucalt.uc_mcontext.fpregs = &nc->ucalt.__fpregs_mem;
        }
    #endif
    
    #ifndef NDEBUG
    canary_context = "create (end)";
    canarycheck(nc);
    #endif
    
    return nc->id;
}

pdco_handle_t pdco_current(void)
{
    init();
    return pdco_active->id;
}

void** pdco_ud(pdco_handle_t h)
{
    init();
    coroutine_t* c = pdco_active;
    if (h > 0) c = getco(h);
    return &c->ud;
}

int pdco_exists(pdco_handle_t h)
{
    init();
    coroutine_t* c = getco(h);
    if (c != NULL)
    {
        return c->status == 1;
    }
    else
    {
        return 0;
    }
}