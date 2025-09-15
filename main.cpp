#include <iostream>

#ifdef ENABLE_FILE
#include "file/w_w_file.h"
#endif

#ifdef ENABLE_FORK
#include "fork/w_w_fork.h"
#endif

#ifdef ENABLE_PIPE
#include "pipe/w_w_pipe.h"
#endif

int main(int argN, const char* args[])
{
    std::cout << "Welcome to POSIX API explorer\n";
    #ifdef ENABLE_FILE
        fileChecking(argN, args);
    #endif

    #ifdef ENABLE_FORK
        forkChecking();
    #endif

    #ifdef ENABLE_PIPE
        pipeRdrctChecking(argN, args);
    #endif

    return 0;
}