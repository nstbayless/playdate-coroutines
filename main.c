#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "coroutine.h"

#pragma GCC diagnostic ignored "-Wint-conversion"
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

volatile int a = 0;

__attribute__((noinline))
int subroutine()
{
    a = 10;
    pdco_resume(PDCO_MAIN_ID);
}

co_thread test_coroutine(co_thread caller)
{
    a = 1;
    pdco_resume(caller);
    a = 2;
    pdco_resume(caller);
    subroutine();
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
    co_thread t = create_thread(test_coroutine, 32, 0);
    while (thread_exists(t))
    {
        resume(t);
        printfln(" -> the value of a is %d", a);
    }
    printfln("Completed. The final value of a is %d", a);
	return 0;
}
