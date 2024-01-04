#include <iostream>
#include <cstdarg>

// 定义一个带有可变参数的宏
#define LOG(format, ...) logHelper(format, ##__VA_ARGS__)

// 辅助函数，使用 va_list 处理可变参数
void logHelper(const char* format, ...) {
    char str[1024];

    va_list args;
    va_start(args, format);
    vsnprintf(str, sizeof(str), format, args);
    va_end(args);

    printf("%s\n", str);
}

int main() {
    LOG("aaa");
    LOG("aaa %d", 1);
    LOG("BBB %s", "world");
    return 0;
}
