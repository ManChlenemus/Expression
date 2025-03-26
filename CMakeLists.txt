cmake_minimum_required(VERSION 3.20)
project(Expression)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "-static")
add_executable(differentiator
        main.cpp
        realization/Tokenator.cpp
)

target_include_directories(differentiator PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/headers
)

add_executable(tests
        tests/tests.cpp
        realization/Tokenator.cpp
        library/catch_amalgamated.cpp
)

target_include_directories(tests PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/headers
        ${CMAKE_CURRENT_SOURCE_DIR}/library
)

enable_testing()
add_test(NAME ExpressionTests COMMAND tests)