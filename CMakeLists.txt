cmake_minimum_required(VERSION 3.13)  # CMake version check
project(Minux)
set(CMAKE_CXX_STANDARD 14)            # Enable c++14 standard

file(GLOB SOURCES
        *.h
        *.cpp
)
# Add executable target with source files listed in SOURCE_FILES variable
add_executable(kernel x32/kernel/kernel.c)