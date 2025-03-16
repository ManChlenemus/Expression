cmake_minimum_required(VERSION 3.29)
project(Expression)

set(CMAKE_CXX_STANDARD 20)

add_executable(Expression main.cpp
        headers/Expression.h
        headers/Tokenator.h
        realization/Tokenator.cpp
        realization/Expression.cpp
)
