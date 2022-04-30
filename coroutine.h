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
    void* ud       PDCO_CPPONLY(=NULL)
);

// crashes if h is invalid or ended
void pdco_resume(pdco_handle_t h);

// gets user-defined local value
void** pdco_ud(pdco_handle_t h);

#ifndef PDCO_NO_KEYWORDS
// Define keyword aliases

typedef pdco_handle_t co_thread;

inline co_thread get_thread(void){
    return pdco_current();
}

inline co_thread create_thread(
        pdco_fn_t fn,
        size_t stacksize    PDCO_CPPONLY(=0),
        void* ud       PDCO_CPPONLY(=NULL)
) {
    return pdco_create(fn, stacksize, ud);
}

inline void resume(co_thread thread) {
    return pdco_resume(thread);
};

#endif

#ifdef __cplusplus
}
#endif