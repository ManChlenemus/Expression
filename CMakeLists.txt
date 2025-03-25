cmake_minimum_required(VERSION 3.20)
project(Expression)

set(CMAKE_CXX_STANDARD 20)

# Основной исполняемый файл
add_executable(differentiator
        main.cpp
        realization/Tokenator.cpp
)

target_include_directories(differentiator PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/headers
)

enable_testing()
add_subdirectory(tests)