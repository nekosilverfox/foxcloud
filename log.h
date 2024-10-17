#ifndef LOG_H
#define LOG_H

/**
 * @brief 用于打印显示数据（日志）
 */
#include <string>

void LogInit();
void Serialize(const char* fmt, ...);


/* __FILE__ 是一个预定义的宏，用于获取当前源文件的文件名
 * strrchr 函数是一个字符串操作函数，用于在字符串中查找指定字符的最后一次出现的位置
 * 因此，strrchr(__FILE__, '/') 的作用是在当前源文件的文件路径中找到最后一个斜杠('/')的位置，并返回该位置之后的部分。这通常用于提取源文件的文件名而去除路径信息。
 * 举例来说，如果 __FILE__ 的值是 "/path/to/myfile.cpp"，那么 strrchr(__FILE__, '/') 将返回指向字符串 "/myfile.cpp" 的指针。这可以帮助在程序中获取当前源文件的文件名而不包含路径。
*/
#define __FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1) : __FILE__)

/* 这是一个用于创建日志信息前缀的宏定义，其中包含了函数名和传入的格式字符串
 * __FUNCTION__ 是C++的一个预定义宏，用于获取当前函数的名称
 */
#define makePrefix(fmt) std::string(__FUNCTION__).append("() - ").append(fmt).c_str()

/*
 * ##__VA_ARGS__：这是一个用于处理可变参数的预处理器技巧，它将传递给宏的可变参数与前面的 fmt 连接在一起。## 是可变参数前面的连接符，它在可变参数为空时起到去除前面的逗号的作用。
 */
#define LogDebug(fmt, ...)	Serialize(std::string("[Debug] ").append(makePrefix(fmt)).c_str(), ##__VA_ARGS__)
#define LogInfo(fmt, ...)	Serialize(std::string("[INFO] ").append(makePrefix(fmt)).c_str(), ##__VA_ARGS__)
#define LogNotice(fmt, ...)	Serialize(std::string("[Notice] ").append(makePrefix(fmt)).c_str(), ##__VA_ARGS__)
#define LogError(fmt, ...)	Serialize(std::string("[Error] ").append(makePrefix(fmt)).c_str(), ##__VA_ARGS__)


#endif // LOG_H
