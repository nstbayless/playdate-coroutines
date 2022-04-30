#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "coroutine.h"

#pragma GCC diagnostic ignored "-Wint-conversion"
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

volatile int a = 0;

#if !defined(PLAYDATE) && !defined(TARGET_PLAYDATE)
    //#define TEST_AUTOMATED
#endif

#ifdef TEST_AUTOMATED
#include <assert.h>
#endif

co_thread test_coroutine(co_thread caller)
{
    a = 1;
    pdco_resume(caller);
    a = 2;
    pdco_resume(caller);
    a = 3;
    return caller;
}

#if defined(PLAYDATE) || defined(TARGET_PLAYDATE)
#include "pd_api.h"
#define printfln playdate->system->logToConsole

int eventHandler(PlaydateAPI* playdate, PDSystemEvent event, uint32_t arg)
{
    printfln("event %d", event);
    if (event != kEventInitLua) return 0;
#else
#define printfln(fmt, ...) printf(fmt "\n", __VA_ARGS__)
int main()
{
#endif
    printfln("%s", "Running coroutine test...");
    for (size_t i = 0; i < 3; ++i)
    {
        co_thread * t = create_thread(test_coroutine, 32, 0, 0);
        resume(t);
        #ifdef TEST_AUTOMATED
        assert(a == i);
        #else
        printfln(" -> the value of a is %d", a);
        #endif
    }
    printfln("Completed. The final value of a is %d", a);
	return 0;
}
