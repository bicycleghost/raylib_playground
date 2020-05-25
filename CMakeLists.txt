cmake_minimum_required(VERSION 3.15)
project(RaylibTemplate)

set(CMAKE_C_STANDARD 99)

add_subdirectory(dependencies/raylib)

add_executable(RaylibTemplate source/main.c)
target_link_libraries(RaylibTemplate PRIVATE raylib)