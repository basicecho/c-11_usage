#include <iostream>
#include <stdarg.h>

// 不定参数，类型相同，求和
int sum(int n, ...)
{
    int result = 0;

    va_list args;
    va_start(args, n);
    for(int i = 0; i < n; i++) {
        result += va_arg(args, int);
    }
    va_end(args);

    return result;
}

int main(int argc, char **argv)
{
    std::cout << sum(5, 4, 6, 8, 10, 12) << std::endl;
    return 0;
}