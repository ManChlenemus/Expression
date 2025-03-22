#include <iostream>
#include <complex>
#include <map>
#include <memory>
#include "headers/Expression.h" // Подключаем ваш заголовочный файл

int main() {
    // Используем std::complex<double> как тип T
    using Complex = std::complex<double>;

    // Создаем переменные
    std::string var_x = "x";
    std::string var_y = "y";

    // Создаем выражения
    auto x = std::make_shared<VarExpression<Complex>>(var_x);
    auto y = std::make_shared<VarExpression<Complex>>(var_y);

    // Создаем сложное выражение: x + y * (x - y)
    auto expr = std::make_shared<BinaryExpression<Complex>>(
        x,
        std::make_shared<BinaryExpression<Complex>>(
            y,
            std::make_shared<BinaryExpression<Complex>>(x, y, MINUS),
            MULT),
        PLUS);

    // Создаем параметры для вычисления
    std::map<std::string, Complex> parameters = {
        {"x", Complex(3.0, 4.0)}, // x = 3 + 4i
        {"y", Complex(1.0, -2.0)} // y = 1 - 2i
    };

    // Вычисляем значение выражения
    Complex result = expr->eval(parameters);
    std::cout << "Expression: " << expr->to_string() << std::endl;
    std::cout << "Result: " << result << std::endl;

    // Дифференцируем выражение по x
    auto diff_expr = expr->diff(var_x);
    std::cout << "Derivative: " << diff_expr->to_string() << std::endl;

    // Вычисляем значение производной
    Complex diff_result = diff_expr->eval(parameters);
    std::cout << "Derivative Result: " << diff_result << std::endl;

    return 0;
}