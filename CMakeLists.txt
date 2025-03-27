cmake_minimum_required(VERSION 3.20)
project(Expression)

set(CMAKE_CXX_STANDARD 20)

include_directories(headers)
add_library(TokenLib STATIC realization/Tokenator.cpp)
target_include_directories(TokenLib PUBLIC headers)

# Подключение тестов
Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.8.0
)

FetchContent_MakeAvailable(Catch2)

# Считыватель с консоли
add_executable(differentiator main.cpp)
target_link_libraries(differentiator PRIVATE TokenLib)
target_include_directories(differentiator PUBLIC headers)

# Сами тесты
add_executable(tests_ tests/tests.cpp)
target_link_libraries(tests_ PRIVATE TokenLib Catch2::Catch2WithMain)
target_include_directories(tests_ PUBLIC headers)

enable_testing()
add_test(NAME ExpressionTests COMMAND tests_)