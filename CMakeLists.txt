cmake_minimum_required(VERSION 3.29)
project(CooGC)

set(CMAKE_CXX_STANDARD 17)

add_executable(CooGC
        src/main.cpp
        src/Bitmap.cpp
        src/VPool.cpp
        src/Object.cpp
        head/HPoint.h
        head/SLog.h
)

# 正确添加头文件路径
target_include_directories(CooGC PRIVATE head)
