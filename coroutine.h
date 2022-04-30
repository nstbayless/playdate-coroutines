#pragma once

#include <ctype.h>

#ifdef __cplusplus
extern "C" {

#define PDCO_CPPONLY(x) x
#else
#define PDCO_CPPONLY(x)
#endif

typedef void(*pdco_fn_t)(void);

// Please note that none of these functions are thread-safe.
// (They are, of course, coroutine-safe.)

// call from outside a coroutine
// starts a new coroutine
// returns id of coroutine, or 0 if already complete, or -1 if an error occurred.
int pdco_run(
    pdco_fn_t fn,
    size_t stacksize    PDCO_CPPONLY(=0)
);

// call from outside a coroutine.
// returns 0 if completed
// returns -1 if error (e.g. coroutine finished/no such coroutine)
// returns id of the coroutine otherwise.
int pdco_resume(int co); 

// call from outside a coroutine.
// Ends a coroutine prematurely. Not normally needed. Not recommended.
// returns 0 if deleted, -1 if an error occurred, and co if co not found.
int pdco_kill(int co);

// call from within a coroutine.
void pdco_yield(void);

// returns the id of the active coroutine, or 0 if none are active.
int pdco_get_coroutine(void);

// returns a pointer to a per-coroutine user-definable void* pointer.
// if co is 0, returns the void* associated with the current coroutine.
// returns null if no such coroutine.
void** pdco_ud(int co);

#ifdef __cplusplus
}
#endif