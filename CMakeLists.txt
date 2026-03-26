# 1. 指定 CMake 的最低版本要求
cmake_minimum_required(VERSION 3.10)

# 2. 定义项目名称和版本
project(TcpRedisServer VERSION 1.0)

# 3. 指定 C++ 标准 (对应编译命令里的 -std=c++11)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 4. 寻找系统的多线程库 (对应编译命令里的 -pthread)
find_package(Threads REQUIRED)

# 5. 告诉 CMake 头文件在哪里 ("." 代表当前目录)
include_directories(include third_party test)

# 6. 把所有的源文件整理成一个变量 (注意只放 .cpp 文件)
# set(SOURCES
#     main.cpp
#     TcpRedisServer.cpp
#     Utils.cpp
# )

# (可选) 把头文件也加进来，这样在使用 VS Code 时，左侧资源管理器能更好地显示它们
# set(HEADERS
#     TcpRedisServer.h
#     Utils.h
#     ThreadPool.hpp
# )
aux_source_directory(src SRC)
aux_source_directory(test TEST)
# 7. 生成可执行文件 (名字叫 my_server)
add_executable(my_server ${SRC} ${TEST})

# 8. 链接第三方库 (非常重要！)
# Threads::Threads 代表多线程库
# hiredis 代表 Redis 官方 C 库 (对应编译命令里的 -lhiredis)
target_link_libraries(my_server PRIVATE Threads::Threads)