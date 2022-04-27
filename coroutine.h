#pragma once

#include <ctype.h>

#ifdef __cplusplus
extern "C" {

#define PDCO_CPPONLY(x) x
#else
#define PDCO_CPPONLY(x)
#endif

typedef void*(*pdco_fn_t)(void*);

// Please note that none of these functions are thread-safe.
// (They are, of course, coroutine-safe.)

// call from outside a coroutine -------------------------------------

// starts a new coroutine
// udata and retval are optional. Leave as NULL if not used.
// returns id of coroutine, or 0 if already complete, or -1 if an error occurred.
int pdco_run(
    pdco_fn_t fn,
    size_t stacksize    PDCO_CPPONLY(=0),
    void* udata         PDCO_CPPONLY(=nullptr),
    void** retval       PDCO_CPPONLY(=nullptr)
);

// returns 0 if completed
// udata and retval are optional. Leave as NULL if not used.
// returns -1 if error (e.g. coroutine finished/no such coroutine)
// returns id of the coroutine otherwise.
int pdco_resume(
    int co,
    void* udata         PDCO_CPPONLY(=nullptr),
    void** retval       PDCO_CPPONLY(=nullptr)
); 

// Ends a coroutine prematurely. Not normally needed. Not recommended.
// returns 0 if deleted, -1 if an error occurred, and co if co not found.
int pdco_kill(int co);

// call from within a coroutine. --------------------------------------

// `udata` is returned to the coroutine's calling procedure.
// udata and return value are optional. udata is "yielded"
// (it is accessible from pdco_run and pdco_resume as the retval field).
// it's generally safe to yield a pointer to a stack value here.
void* pdco_yield(void* udata PDCO_CPPONLY(=nullptr));

#ifdef __cplusplus
}
#endif