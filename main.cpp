#include <iostream>

#ifdef ENABLE_FILE
#include "file/w_w_file.h"
#endif

#ifdef ENABLE_PIPE
#include "pipe/w_w_pipe.h"
#endif

int main(int argN, const char* args[])
{
    std::cout << "Welcome to POSIX API explorer\n";
    #ifdef ENABLE_FILE
        file(argN, args);
    #endif

    #ifdef ENABLE_PIPE
        forkChecking();
    #endif

    return 0;
}