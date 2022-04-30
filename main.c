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
void test_coroutine(void)
{
    a = 1;
    pdco_yield();
    a = 1;
    pdco_yield();
    a = 1;
}
#else
void test_coroutine(void)
{
    a = 1;
    pdco_yield();
    a = 2;
    pdco_yield();
    a = 3;
}
#endif

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
        int co;
        switch(co = pdco_run(test_coroutine, 0))
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
        
        #ifdef TEST_AUTOMATED
        assert(a == 1);
        #else
        printfln(" -> the value of a is %d", a);
        #endif
        
        while(co > 0)
        {
            printfln(" -> %s", "resuming coroutine");
            co = pdco_resume(co);
            #ifdef TEST_AUTOMATED
            assert(a == 1);
            #else
            printfln(" -> the value of a is %d", a);
            #endif
        }
    }
    
	return 0;
}
