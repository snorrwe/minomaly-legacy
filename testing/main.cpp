#include "gtest/gtest.h"
#include <iostream>

/* Exception handling */
#if defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)
#include <execinfo.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void handler(int sig)
{
    void* array[10];
    size_t size;
    // get void*'s for all entries on the stack
    size = backtrace(array, 10);
    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}
#endif

int main(int argc, char* argv[])
{
#if defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)
    signal(SIGSEGV, handler); // install our handler
#endif

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    return 0;
}
