# Coroutines, Green Threads, Fibres, etc. for the Panic Playdate

This small library allows C ([and C++](https://github.com/nstbayless/playdate-cpp)) to make use of multiple execution stacks instead of the usual one. This permits the easy implementation of (deep) coroutines, greenlets, lightweight user-space threads and more. It is intended primarily for use on the Panic Playdate, but may also be used in any other C context if `<ucontext.h>` is available, or on ARM if `<setjmp.h>` is available.

Only one such "thread" may run at a time. Each has its own stack. Switching between threads is done manually using pdco_resume. **There is no scheduler** (unless you build one yourself) -- use of this library is intended mostly for applications like Lua-style deep coroutines, but you may use it however you wish.

## Usage

Simply add `pdco.c` and `pdco.h` into your source. You may refer to [`main.c` herein](./main.c) as a simple example. Here is the API:

### `pdco_create`

```C
pdco_handle_t pdco_create(pdco_fn_t fn, size_t stacksize=0, void* ud=NULL);
```

Readies the function `fn` with a stack of size `stacksize` in *bytes* (defaults to 64 *kilobytes* if `stacksize` is 0). `ud` is optional. `fn` **is not run immediately** -- you must call `pdco_yield` to yield execution to it.

`fn` takes as an argument the id of the thread which spawns it, and may either (a) never return, or (b) return a thread id to yield to. Upon `fn`'s return, the stack is free'd. Returning is the only way to free the stack.

### `pdco_current`

```C
pdco_handle_t pdco_current(void);
```

Returns the id of the current thread.

### `pdco_exists`
```C
int pdco_exists(pdco_handle_t)
```

Returns true iff the given thread id points to a thread which exists and has not yet returned.

### `pdco_yield`

```C
void pdco_yield(pdco_handle_t)
```

Pauses execution in the current thread and resumes execution in the given thread. Execution in the current thread will remain paused until such time as another thread explicitly yields control back here, at which point `yield` will return.

### `pdco_ud`

```C
void** pdco_ud(pdco_handle_t)
```

Retrieves a reference to the user-defined `void*` pointer specific to the given thread. **Please carefully notice** that `void**` is returned, *not* `void*`; this allows that the `void* ud` value may be assigned to.

## Missing Functionality

### Stack Usage

Currently, only half of the request stack size is used, because it's difficult to detect which direction the stack is supposed to grow.  Using `ucontext.h` for the simulator adds some difficulty as well. Therefore, the stack pointer is set to start out in the middle of the stack region. Please submit a patch if you can figure out how to improve on this.

### Stack Protection

There is currently no support for ensuring that stacks are malloc'd in a 
page which permits the use of stacks. This is not a problem on Linux, but may be a problem on Windows or Mac. If you are familiar with these concepts, please contribute a PR to allow this library to be used on other operating systems.