#include <iostream>
#include <stdarg.h>

// 不同类型，直接输出
template<class T>
std::ostream &print(std::ostream &os, const T &t)
{
    return os << t << ", ";
}

template<class T, class ...Args>
std::ostream &print(std::ostream &os, const T &t, const Args &...rest)
{
    os << t << ", ";
    return print(os, rest...);
}

int main(int argc, char **argv)
{
    print(std::cout, 1, 2, "abc", '1', 1.1);
    return 0;
}
