#ifndef TOKENATOR_H
#define TOKENATOR_H
#include <vector>
#include <string>

enum TokenType {
    NUMBER,     // Число (действительная часть)
    COMPLEX,    // Комплексное число (только мнимая часть)
    VARIABLE,   // Переменная (1 буква)
    OPERATOR,   // Оператор (+, -, *, /, ^)
    FUNCTION,   // Функция (sin, cos, ln, exp)
    LEFT_PAREN, // Левая скобка
    RIGHT_PAREN, // Правая скобка
    START // для унарного минуса
};

struct Token {
    TokenType type;
    std::string value;

    Token(const TokenType type, const std::string &value) noexcept
        : type(type), value(value) {}
};

std::vector<Token> tokenize(const std::string& str);
void printTokens(std::vector<Token> &tokens);
#endif //TOKENATOR_H
