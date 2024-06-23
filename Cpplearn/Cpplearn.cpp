#include <iostream>
#define log(x) std::cout << x << std::endl
using namespace std;

int main()
{
    char* buffer = new char[8];
    memset(buffer, 245, 8);
    log(*buffer);
    delete[] buffer;
    return 0;
}