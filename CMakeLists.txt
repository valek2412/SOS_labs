cmake_minimum_required(VERSION 3.16)
project(Lab2)

set(CMAKE_CXX_STANDARD 20)

add_executable(Lab2 main.cpp Allocator.cpp Tester.cpp Allocator.h Tester.h)