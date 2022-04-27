#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "coroutine.h"

#pragma GCC diagnostic ignored "-Wint-conversion"
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

volatile int a = 0;

void* test_coroutine(void* v)
{
    uintptr_t s = v;
    a = s;
    v = pdco_yield(1);
    a = ((uintptr_t)v) * s * s;
    v = pdco_yield(2);
    a = ((uintptr_t)v) * s * s * s;
    return (uintptr_t)3;
}

#if defined(PLAYDATE) || defined(TARGET_PLAYDATE)
#include "pd_api.h"
#define printfln playdate->system->logToConsole

int eventHandler(PlaydateAPI* playdate, PDSystemEvent event, uint32_t arg)
{
    if (event != kEventInitLua) return 0;
#else
#define printfln(fmt, ...) printf(fmt "\n", __VA_ARGS__)
int main()
{
#endif
    uintptr_t rv = 0;
    for (size_t i = 0; i < 3; ++i)
    {
        uintptr_t powerbase = (i == 2) ? 10 : i + 1;
        int co;
        switch(co = pdco_run(test_coroutine, 0, powerbase, &rv))
        {
        case -1:
            printfln("%s", "an error occurred launching the coroutine.");
            break;
        case 0:
            printfln("%s", "coroutine run and complete");
            break;
        default:
            printfln("coroutine launched, id %d", co);
            break;
        }
        
        printfln(" -> the value of a is %d", a);
        printfln(" -> the value of rv is %d", (int)rv);
        
        while(co > 0)
        {
            printfln(" -> %s", "resuming coroutine");
            co = pdco_resume(co, 5, &rv);
            printfln(" -> the value of a is %d", a);
            printfln(" -> the value of rv is %d", (int)rv);
        }
    }
    
	return 0;
}
