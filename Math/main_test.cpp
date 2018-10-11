#include <iostream>
#include "Matrix.h"

int main()
try
{
    Matrix a{{2, 3, 5, 7
            , 11, 13, 17, 19
            , 23, 29, 31, 37
            , 41, 43, 47 ,53}, 4};
    Matrix vect{{59, 61, 67, 71}, 1};


    std::cout << "Result:" << std::endl;
    std::cout << a*vect << std::endl;
/*
Result:
1133
3930
7830
11954
*/
    return 0;
}
catch (std::invalid_argument &e)
{
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
}
catch (...)
{
    std::cerr << "Unknown exception\n";
    return -1;
}
