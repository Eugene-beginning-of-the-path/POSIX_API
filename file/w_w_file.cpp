#include <iostream>
#include <cstring>

#include <fcntl.h>    //open()
#include <unistd.h>   //_exit()
#include <sys/stat.h> //S_ISREG макрос 

[[noreturn]] static void die_error(const char* msg)
{
    std::cerr << msg << ':' << std::strerror(errno);
    _exit(1);
}

struct MyStruct
{
    int a = 13;
    const char* str = "GigaChat";
};

int file(int argN, const char* args[])
{
    struct MyStruct ob{.str = "GPT"};
    std::cout << ob.a << '\t' << ob.str << std::endl;

    if (argN != 3)
    {
        std::cerr << "Usage pattern: " << args[0] << " <src> <dst>" << std::endl;
        return 2;
    }

    const char* src = args[1];
    const char* dst = args[2];

    int fd = open(src, O_RDONLY | O_CLOEXEC);
    if (fd == -1) die_error("open(src)");

    struct stat st;
    if (fstat(fd, &st) == -1)
    {
        close(fd);
        die_error("fstat(src)");
    }
    if (!S_ISREG(st.st_mode))
    {
        close(fd);
        std::cerr << "src is not regular file" << std::endl;
        return 2;
    }



    return 0;
}