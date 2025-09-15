#include <iostream>
#include <vector>

#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <sys/wait.h>

typedef int fd_t;

fd_t createFile(const char* path, bool append = false)
{
    int flags = O_WRONLY | O_CREAT | O_CLOEXEC | (append ? O_APPEND : O_TRUNC);
    fd_t fd = ::open(path, flags, 0666/*neet to specify because of O_CREAT*/);
    if (fd == -1)
    {
        std::cerr << "open(" << path << "): " << std::strerror(errno) << std::endl;
    }

    return fd;
}

bool redirect1(const char* path) // prog 2> file.txt redirect std::cerr to file.txt
{
    fd_t fd = createFile(path);
    if (fd == -1)
    {
        return false;
    }

    //буферы stdio копят в себе данные и только после записывают в нужные fd
    //нужно смывать данные из буфера std::cerr, чтобы все накопившееся внутри 
    //записалось куда планировалось до этого:
    fflush(stderr);
    
    fd_t stdcerrFd = ::dup(STDERR_FILENO);
    if (stdcerrFd == -1)
    {
        std::cerr << "dup(STDCERR_FILE): " << std::strerror(errno) << std::endl;
        return false;
    }

    if (::dup2(fd, STDERR_FILENO) == -1) //STDERR_FILENO has default val eq 2
    {
        std::cerr << "not successfully duplicated fd(" << fd << ") to STDCERR_FILE: "
            << std::strerror(errno) << std::endl;
        ::close(fd);
        ::close(stdcerrFd);
        return false;
    }

    //теперь STDCERR_FILE указывает на файл X
    //и fd указывает на файл Х(те же флаги и курсор)
    //значит можно оставить дефолтный поток вывода stdcerr, а второй закрыть
    ::close(fd);

    std::cerr << "Redirect1() is checking its logic implementation\n";

    if (::dup2(stdcerrFd, STDERR_FILENO) == -1) //возвращаем запись буфера stdcerr в его стандартный файл под предыдущим fd=1
    {
        std::cerr << "not successfully restore stderr back: " << std::strerror(errno)
            << std::endl;
        ::close(stdcerrFd);
        return false;
    }
    
    ::close(stdcerrFd); ////теперь STDCERR_FILE указывает на тот же файл, что и stdcerrFd

    return true;
}

bool pipe1()
{
    int p[2];
    if (pipe(p) == -1)
    {
        std::cerr << "pipe(): " << strerror(errno) << std::endl;
        return false;
    }
    
    pid_t ch1 = ::fork();
    if (ch1 == -1)
    {
        std::cerr << "ch1 fork(): " << strerror(errno) << std::endl;
        return false;
    }

    if (ch1 == 0)
    {
        fd_t fd = dup2(p[1], STDOUT_FILENO);
        if (fd == -1)
        {
            std::cerr << "dup2(p[1], out): " << strerror(errno) << std::endl;
            return false;
        }

        ::close(p[0]);
        ::close(p[1]);

        ::execlp("lc", "lc", nullptr);
        _exit(127);
    }

    pid_t ch2 = ::fork();
    if (ch2 == -1)
    {
        std::cerr << "ch2 fork(): " << strerror(errno) << std::endl;
        return false;
    }

    if (ch2 == 0)
    {
        fd_t fd = dup2(p[1], STDOUT_FILENO);
        if (fd == -1)
        {
            std::cerr << "dup2(p[0], in): " << strerror(errno) << std::endl;
            return false;
        }

        ::close(p[0]);
        ::close(p[1]);

        ::execlp("wc", "wc", "-l", nullptr);
        _exit(127);
    }

    ::close(p[0]);
    ::close(p[1]);

    int status;
    ::waitpid(ch1, &status, 0);
    ::waitpid(ch2, &status, 0);

    return true;
}

bool pipe2()
{
    int to_child[2];
    int from_child[2];

    if (::pipe(to_child) == -1 || ::pipe(from_child) == -1)
    {
        std::cerr << "pipe(): " << strerror(errno) << std::endl;
        return false;
    }

    pid_t chP = ::fork();
    if (chP == -1)
    {
        std::cerr << "fork(): " << strerror(errno) << std::endl;
        return false;
    }

    if (chP == 0)
    {
        ::dup2(to_child[0], STDIN_FILENO);
        ::dup2(from_child[1], STDOUT_FILENO);

        ::close(to_child[0]); ::close(to_child[1]);
        ::close(from_child[0]); ::close(from_child[1]);

        ::execlp("./P", "P", nullptr);
        _exit(127);
    }

    ::close(to_child[0]);
    ::close(from_child[1]);

    std::vector<int> pipeData = {2, 5, 16, 64};
    std::string buf;
    for (auto & el : pipeData)
    {
        buf.append(std::to_string(el) + "\n");
    }
    size_t n = buf.size();
    const char* ptrBuf = buf.data();
    while (n)
    {
        ssize_t sentBytes = ::write(to_child[1], ptrBuf, n);
        if (sentBytes > 0)
        {
            ptrBuf += sentBytes;
            n -= sentBytes;
            continue;
        }
        if (sentBytes == -1 && errno == EINTR)
        {
            continue;
        }
        return false;
    }
    
    ::close(to_child[1]);

    

}

void pipeRdrctChecking(int argN, const char* args[])
{
    if (argN == 2)
    {
        redirect1(args[1]);
    }
}