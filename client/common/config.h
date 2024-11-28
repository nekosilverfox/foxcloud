#ifndef CONFIG_H
#define CONFIG_H

/* 正则表达式
 * R 是 C++ 原始字符串字面量（Raw String Literal）的标志
 * 表示字符串中的内容不需要对特殊字符（如 \ 或 "）进行转义。它是从 C++11 开始引入的特性
 */
#define REG_LOGIN       (R"(^[a-zA-Z\d_@#\-\*]{3,16}$)")
#define REG_PASSWD      (R"(^[a-zA-Z\d_@#\-\*]{6,18}$)")
#define REG_PHONE       (R"(^\d{6,10}$)")
#define REG_EMAIL       (R"(^[a-zA-Z\d._%+-]+@[a-zA-Z\d.-]+\.[a-zA-Z]{2,63}$)")
#define REG_IP          (R"(^((25[0-5]|2[0-4]\d|1\d{2}|\d{1,2})\.){3}(25[0-5]|2[0-4]\d|1\d{2}|\d{1,2})$)")
#define REG_PORT        (R"(^([1-9]\d{0,3}|[1-5]\d{4}|6[0-4]\d{3}|65[0-4]\d{2}|655[0-2]\d|6553[0-5])$)")

#define PATH_CONFIG_FILE    ("config/foxcloud-client.json") // 客户端配置文件


#endif // CONFIG_H
