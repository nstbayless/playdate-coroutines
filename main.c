#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "pd_api.h"
#include "coroutine.h"

#pragma GCC diagnostic ignored "-Wint-conversion"
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

PlaydateAPI* pd;
volatile int a = 0;

__attribute__((noinline))
int subroutine(void)
{
    a = 10;
    yield(PDCO_MAIN_ID);
    return 0;
}

co_thread file_io_test(co_thread caller)
{
    SDFile* file = pd->file->open("test", kFileWrite);
    const char* str = "TEST";
    pd->file->write(file, str, strlen(str));
    pd->file->close(file);
    return caller;
}

co_thread test_coroutine(co_thread caller)
{
    a = 1;
    yield(caller);
    a = 2;
    yield(caller);
    subroutine();
    a = 3;
    return caller;
}

#if defined(PLAYDATE) || defined(TARGET_PLAYDATE)
#define printfln playdate->system->logToConsole

int eventHandler(PlaydateAPI* playdate, PDSystemEvent event, uint32_t arg)
{
    pd = playdate;
    printfln("event %d", event);
    if (event != kEventInitLua) return 0;
#else
#define printfln(fmt, ...) printf(fmt "\n", __VA_ARGS__)
int main()
{
#endif
    printfln("%s", "Running coroutine test...");
    co_thread t = create_thread(test_coroutine, 1024, NULL);
    while (thread_exists(t))
    {
        yield(t);
        printfln(" -> the value of a is %d", a);
    }
    printfln("Completed. The final value of a is %d", a);
    if (a != 3) return 1;
    
    co_thread t2 = create_thread(file_io_test, 4096, NULL);
    yield(t2);
    
    printfln("Complete. %d", 0);
    
	return 0;
}
