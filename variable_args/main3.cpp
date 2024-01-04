#include <iostream>
#include <string>
#include <sstream>

// 利用ostringstream进行输出
std::ostream &print(std::ostream &os)
{
    return os;
}

template<class T, class ...Args>
std::ostream &print(std::ostream &os, const T &t, const Args &...rest)
{
    os << t << ' ';
    return print(os, rest...);
}

template<class ...Args>
std::string format(const Args &...args)
{
    std::ostringstream oss;
    print(oss, args...);
    return oss.str();
}

int main(int argc, char **argv)
{
    std::cout << format(1, 2, "abc", 'a', 1.1) << std::endl;
    return 0;
}