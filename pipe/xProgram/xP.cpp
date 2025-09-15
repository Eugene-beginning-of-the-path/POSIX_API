#include <iostream>
#include <string>
#include <limits>
#include <unistd.h> //getpid()

/*
    Данная P программа запускается как дочерний процесс и получает данные на обработку
    напрямую из другой программы в stdin через pipe конца read-end, 
    и после импровизированной обработки через *2 откидывает данные через stdout 
    по другому pipe с write-end

    Build: g++ xP.cpp -o P
*/
int main()
{
    int pipeData = 0;
    size_t counterData;

    while (true)
    {
        if (std::cin >> pipeData) //если смогли преобразовать данные к int
        {
            ++counterData;
            std::cout << pipeData*2 << std::endl;
        }
        else if (std::cin.eof()) //если read-end у pipe получил завершение
        {
            break;
        }
        else //если поток ввода перешел в состояние с ошибкой(установил флаг failbit)
        {
            std::cin.clear(); //снимаем флаг ошибки или любое чтение будет игнорироваться
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //пропускаем все символы до \n
        }
    }
    
    std::cerr << "Processed data: " << counterData << std::endl;
    std::cerr << "X process has " << getpid() << std::endl;

    return 0;
}