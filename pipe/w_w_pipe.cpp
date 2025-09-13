#include <iostream>
#include <thread>
#include <chrono>

#include <unistd.h>
#include <sys/wait.h>

int forkChecking()
{
    pid_t pid = ::fork();
    if (pid == -1)
    {
        std::cerr << "Not successfully created child process\n";
    }

    //Далее мы можем выполнять разную логику для parent и child процесса:
    if (pid == 0) //child logic
    {
        size_t i = 2;
        while (i--)
        {
            std::cout << "child process: " << getpid() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        return 0;
    }
    else //pid > 0 is parent logic
    {
        // kill(pid, SIGINT); //u can kill child process using signall, just for test case
        std::this_thread::sleep_for(std::chrono::milliseconds(4000));
        int status(0);
        ::waitpid(pid, &status, 0);
        std::cout << "parent process " << getpid() << " has got child status: \n";
        if (WIFEXITED(status))
        {
            std::cout << "\treturn code is " << WEXITSTATUS(status) << std::endl;
        }
        else if (WIFSIGNALED(status))
        {
            std::cout << "\tbroken by signal: " << WTERMSIG(status) << std::endl;
        }
    }
    return 0;
}