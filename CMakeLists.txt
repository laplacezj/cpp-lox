cmake_minimum_required(VERSION 3.1)

project(cpp-lox 
        VERSION 0.0.1
        LANGUAGES C CXX )

# C++17 is required
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)
