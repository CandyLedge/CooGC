cmake_minimum_required(VERSION 3.29)
project(CooGC)

set(CMAKE_CXX_STANDARD 17)

add_executable(CooGC main.cpp
        linklist.cpp
        bitmap.cpp)
