cmake_minimum_required(VERSION 3.29)
project(Expression)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

set(SOURCES
        main.cpp
        realization/Tokenator.cpp
        tests/test_diff.cpp
)
set(HEADERS
        headers/Expression.h
        headers/Parser.h
        headers/Tokenator.h
        tests/test_diff.cpp
)
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE headers)

enable_testing()
add_subdirectory(tests)