# 大写的是 Make 内部的关键字
# 使用的编译器
CC=gcc

# 预处理参数（注释去掉，宏替换）
CPPLFAGS=-I
CFLAGS=-Wall

# 需要链接的动态库 .so
LIBS=

# 子目标, 因为是测试,所有需要单独生成很多子目标(生成可执行程序的名字)
# 前面是变量名，后面是子目标的名字
upload_fdfs=./upload

# ALL 是最终目标，也就是最终的可执行程序，其中可以包含多个可执行程序
target=$(upload_fdfs)
ALL:$(target)

# 模式规则（% - 是做匹配，将 .c 生成所有的.o 文件）
%.o:%.c
	$(CC) -c $< -o $@ $(CPPLFAGS) $(CFLAGS)

# 生成每一个子目标, 
# main 程序
$(upload_fdfs):fdfs_api/main.o fdfs_api/fdfs_api.o fdfs_api/log.o
	$(CC) $^ $(LIBS) -o $@

# 项目清除
clean:
	-rm -rf *.o $(target) ./fdfs_api/*.o

# 声明伪文件
.PHONY:clean ALL
