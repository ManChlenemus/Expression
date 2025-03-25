cmake_minimum_required(VERSION 3.20)

# Вариант с ручной загрузкой (надежнее)
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(ExpressionTests
        tests.cpp
        ../realization/Tokenator.cpp
)

target_include_directories(ExpressionTests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../headers
)

target_link_libraries(ExpressionTests PRIVATE
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(ExpressionTests)