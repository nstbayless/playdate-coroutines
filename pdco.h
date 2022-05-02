#pragma once

#include <ctype.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {

#define PDCO_CPPONLY(x) x
#else
#define PDCO_CPPONLY(x)
#endif

#define PDCO_MAIN_ID 1

typedef int pdco_handle_t;
typedef pdco_handle_t(*pdco_fn_t)(pdco_handle_t caller);

// Please note that none of these functions are thread-safe.
// (They are, of course, coroutine-safe.)

pdco_handle_t pdco_current(void);
pdco_handle_t pdco_create(
    pdco_fn_t fn,
    size_t stacksize    PDCO_CPPONLY(=0),
    void* ud            PDCO_CPPONLY(=NULL)
);
int pdco_exists(pdco_handle_t h);

// pauses execution on this thread and resumes execution on h
// crashes if h is invalid or ended 
// returns thread that yields to this thread
pdco_handle_t pdco_yield(pdco_handle_t h);

// gets user-defined local value
void** pdco_ud(pdco_handle_t h);

#ifdef __cplusplus
}
#endif