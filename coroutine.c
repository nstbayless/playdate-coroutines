#if defined(__arm__) || defined(__thumb__)
    #define SETSP(x) asm volatile(\
        "mov sp, %0" \
        : : "r" (x) \
    );
#endif

#include "pd_api.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "coroutine.h"

#if __has_include(<ucontext.h>)
    #define USE_UCONTEXT
    #include <ucontext.h>
#else
    #define USE_SETJMP
    #include <setjmp.h>
#endif

#pragma GCC optimize("O1")

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
    pdco_handle_t id;
    
    // next in linked list
    struct coroutine_t* next;
    
    int status; // if this is set to 0, means marked as dirty and should be cleaned up.
    
    // on main thread, these are all left unset.
    pdco_handle_t creator;
    pdco_fn_t fn;
    size_t stacksize;
    size_t txsize;
    void* ud;
    void* stackstart;
    void* stack; // stack followed by user-defined data
    void* stack_ptr;
    
    #ifdef USE_SETJMP
    jmp_buf jbalt;
    #else // USE_UCONTEXT
    ucontext_t ucalt;
    #endif
} coroutine_t;

static coroutine_t co_main = {
    .id = PDCO_MAIN_ID,
    .status = 1,
    .ud = NULL
};

// linked list
static coroutine_t* pdco_first = NULL;

// Current coroutine
// It is the caller's responsibility to maintain this.
static coroutine_t* volatile pdco_active = &co_main;
static coroutine_t* volatile pdco_prev = NULL;
static int next_coroutine_id = PDCO_MAIN_ID + 1;

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
    // !! FIXME !!
    return co->stack + co->stacksize / 2;
    #endif
    
    return (stack_direction() == 1)
        ? co->stack + STACKMARGIN
        : (co->stack + co->stacksize - STACKMARGIN);
}

// gets coroutine from handle or NULL if does not exist
static coroutine_t* getco(pdco_handle_t id)
{
    if (id == PDCO_MAIN_ID) return &co_main;
    
    coroutine_t* co = pdco_first;
    while (co && co->id != id)
    {
        co = co->next;
    }
    
    return co;
}

__attribute__((noinline))
static void pdco_guard(void)
{
    cleanup_if_complete(pdco_prev);
    
    register pdco_handle_t resume_next;
    asm("");
    resume_next = pdco_active->fn(pdco_active->creator);
    asm("");
    
    // final yield.
    pdco_active->status = 0; // mark thread as dirty / clean up
    pdco_yield(resume_next); // after resuming, this thread will clean us up.
}

// returns -1 if failure.
__attribute__((noinline))
static int pdco_resume_(coroutine_t* nc)
{
    // cannot resume active (current) coroutine.
    if (!nc) return -1;
    if (nc == pdco_active) return -1;
    pdco_prev = pdco_active;
    pdco_active = nc;
    
    pdco_prev->stack_ptr = __builtin_frame_address(0) + stack_direction()*192; // reasonable-seeming safety margin
    
    #ifdef USE_SETJMP
        asm("");
        if (setjmp(pdco_prev->jbalt) == 0)
        {
            longjmp(nc->jbalt, 1);
        }
    #else
        swapcontext(&pdco_prev->ucalt, &nc->ucalt);
    #endif    
    
    cleanup_if_complete(pdco_prev);
    return 0;
}

void pdco_yield(pdco_handle_t h)
{
    coroutine_t* nc = getco(h);
    if (pdco_resume_(nc) != 0)
    {
        // crash
        pdco_resume_(nc);
    }
}

pdco_handle_t pdco_create(pdco_fn_t fn, size_t stacksize, void* ud)
{
    #if defined(__arm__) || defined(__clang__)
    register
    #endif
    coroutine_t* nc = (coroutine_t*)malloc(sizeof(coroutine_t));
    if (!nc) return -1;
    memset(nc, 0, sizeof(coroutine_t));
    
    #ifdef USE_UCONTEXT
        if (getcontext(&pdco_active->ucalt) < 0)
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
    nc->ud = ud;
    nc->stackstart = getstackstart(nc);
    nc->stacksize = stacksize;
    nc->status = 1; // running
    nc->fn = fn;
    nc->creator = pdco_active->id;
    
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
        nc->ucalt.uc_stack.ss_size = nc->stacksize;
        nc->ucalt.uc_link = NULL; // we take care of return ourselves.
        makecontext(&nc->ucalt, pdco_guard, 0);
        
        // there's a bug for some reason in some implementations of makecontext
        // where fpregs is left NULL.
        if (nc->ucalt.uc_mcontext.fpregs == NULL)
        {
            nc->ucalt.uc_mcontext.fpregs = &nc->ucalt.__fpregs_mem;
        }
    #endif
    
    return nc->id;
}

pdco_handle_t pdco_current(void)
{
    return pdco_active->id;
}

void** pdco_ud(pdco_handle_t h)
{
    coroutine_t* c = pdco_active;
    if (h > 0) c = getco(h);
    return &c->ud;
}

int pdco_exists(pdco_handle_t h)
{
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

int pdco_is_active(pdco_handle_t h)
{
    coroutine_t* c = getco(h);
    if (c != NULL)
    {
        return pdco_active == c;
    }
    else
    {
        return 0;
    }
}

void* pdco_get_stack_ptr(pdco_handle_t h)
{
    coroutine_t* c = getco(h);
    if (c != NULL)
    {
        if (pdco_active == c) return 0;
        
        return c->stack_ptr;
    }
    else
    {
        return 0;
    }
}

#ifdef TARGET_SIMULATOR
void detour_system_operations_to_tcm(PlaydateAPI* pd)
{ }
#else

struct Args
{
    uintptr_t v0, v1, v2, v3;
};

typedef uintptr_t(*fn_t)(uintptr_t a0, uintptr_t a1, uintptr_t a2, uintptr_t a3);

uintptr_t RunFnWithArgs(fn_t fn, struct Args* a)
{
    return fn(a->v0, a->v1, a->v2, a->v3);
}

static __attribute__((naked)) uintptr_t RunWithStack(void* f, void* a, void* b) {
    __asm__ volatile(
        "push {r4, lr}\n"
        "mov  r4, sp\n"        // save current stack pointer
        "mov  sp, r2\n"        // switch to new stack (b)
        "bl   RunFnWithArgs\n" // call RunFnWithArgs(f, a)
        "mov  sp, r4\n"        // restore original stack pointer
        "pop  {r4, lr}\n"
        "bx   lr\n"            // return
    );
}

static void* get_tcm_stack(void)
{
    return pdco_get_stack_ptr(PDCO_MAIN_ID);
}

static int is_stack_in_tcm(void)
{
    uintptr_t s = (uintptr_t)__builtin_frame_address(0);
    return ((s & 0xFF000000) == 0x20000000);
}

#define DEFINE_DETOUR(ns, name) \
void* org__##ns_##name; \
uintptr_t detour__##ns_##name(uintptr_t r0, uintptr_t r1, uintptr_t r2, uintptr_t r3) \
{ \
    struct Args a = {r0, r1, r2, r3}; \
    if (is_stack_in_tcm()) return RunFnWithArgs(org__##ns_##name, &a); \
    return RunWithStack(org__##ns_##name, &a, get_tcm_stack()); \
}

DEFINE_DETOUR(file, geterr)
// TODO: listfiles
DEFINE_DETOUR(file, stat)
DEFINE_DETOUR(file, mkdir)
DEFINE_DETOUR(file, unlink)
DEFINE_DETOUR(file, rename)
DEFINE_DETOUR(file, open)
DEFINE_DETOUR(file, close)
DEFINE_DETOUR(file, read)
DEFINE_DETOUR(file, write)
DEFINE_DETOUR(file, flush)
DEFINE_DETOUR(file, tell)
DEFINE_DETOUR(graphics, loadBitmap)
DEFINE_DETOUR(graphics, loadIntoBitmap)
DEFINE_DETOUR(graphics, loadBitmapTable)
DEFINE_DETOUR(graphics, loadIntoBitmapTable)
DEFINE_DETOUR(graphics, loadFont)

#define APPLY_DETOUR(ns, name) \
    org__##ns_##name = pd->ns->name; \
    *(void**)&pd->ns->name = detour__##ns_##name;

void detour_system_operations_to_tcm(PlaydateAPI* pd)
{
    static bool enabled = false;
    if (enabled) return;
    enabled = true;

    APPLY_DETOUR(file, geterr)
    // TODO: listfiles
    APPLY_DETOUR(file, stat)
    APPLY_DETOUR(file, mkdir)
    APPLY_DETOUR(file, unlink)
    APPLY_DETOUR(file, rename)
    APPLY_DETOUR(file, open)
    APPLY_DETOUR(file, close)
    APPLY_DETOUR(file, read)
    APPLY_DETOUR(file, write)
    APPLY_DETOUR(file, flush)
    APPLY_DETOUR(file, tell)
    APPLY_DETOUR(graphics, loadBitmap)
    APPLY_DETOUR(graphics, loadIntoBitmap)
    APPLY_DETOUR(graphics, loadBitmapTable)
    APPLY_DETOUR(graphics, loadIntoBitmapTable)
    APPLY_DETOUR(graphics, loadFont)
}
#endif