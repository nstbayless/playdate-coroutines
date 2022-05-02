#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "pdco.h"

#pragma GCC diagnostic ignored "-Wint-conversion"
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

volatile int a = 0;

__attribute__((noinline))
void subroutine(void)
{
    a = 10;
    pdco_yield(PDCO_MAIN_ID);
}

pdco_handle_t test_coroutine(pdco_handle_t caller)
{
    a = 1;
    pdco_yield(caller);
    a = 2;
    pdco_yield(caller);
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
    pdco_handle_t t = pdco_create(test_coroutine, 1024, NULL);
    while (pdco_exists(t))
    {
        pdco_yield(t);
        printfln(" -> the value of a is %d", a);
    }
    printfln("Completed. The final value of a is %d", a);
	return 0;
}
