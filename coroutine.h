#pragma once

#include <ctype.h>

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
    size_t udsize       PDCO_CPPONLY(=0)
);

// crashes if h is invalid or ended
void pdco_resume(pdco_handle_t h);

// gets user-defined local value
// main ud is always null
void* pdco_ud(pdco_handle_t h);

#ifndef PDCO_NO_KEYWORDS
// Define keyword aliases

typedef pdco_handle_t co_thread;

co_thread get_thread(void) __attribute__((alias("pdco_current")));
co_thread create_thread(
        pdco_fn_t fn,
        size_t stacksize    PDCO_CPPONLY(=0),
        void* ud            PDCO_CPPONLY(=NULL),
        size_t udsize       PDCO_CPPONLY(=0)
) __attribute__((alias("pdco_create")));

void resume(co_thread h) __attribute__((alias("pdco_resume")));

#endif

#ifdef __cplusplus
}
#endif