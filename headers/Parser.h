#ifndef PARSER_H
#define PARSER_H

#include "Expression.h"
#include "Tokenator.h"
#include <stdexcept>

template<typename T>
class Parser {
    std::vector<Token> tokens;
    size_t current = 0;
    size_t cnt_par = 0; // счетчик скобок

    Token watch() const {
        if (current < tokens.size()) {
            return tokens[current];
        }
        throw std::runtime_error("Unexpected end of input");
    }

    Token consume() {
        if (current < tokens.size()) {
            return tokens[current++];
        }
        throw std::runtime_error("Unexpected end of input");
    }

    bool ismatch(TokenType type) { // для проверки существования ')', как парной к '('
        if (current < tokens.size() && tokens[current].type == type) {
            current++;
            return true;
        }
        return false;
    }

    // --- Основные методы парсера ---
    std::shared_ptr<Expression<T> > parseExpression() {
        return parseBinary(0);
    }

    // Парсит и может принимать бин операции
    std::shared_ptr<Expression<T> > parseBinary(int minPriority) {
        auto left = parsePrimary();

        while (true) {
            if (current >= tokens.size()) {
                break;
            }

            auto token = watch();
            if (token.type != OPERATOR) {
                switch (token.type) {
                    case RIGHT_PAREN: {
                        if (!cnt_par) throw std::runtime_error("Extra ')'");
                        break;
                    }
                    default: throw std::runtime_error("Expected operation");
                }
                break; // если скобка ")", но она не лишняя
            }

            operators op(token.value == "+"
                             ? PLUS
                             : token.value == "-"
                                   ? MINUS
                                   : token.value == "*"
                                         ? MULT
                                         : token.value == "/"
                                               ? DIV
                                               : token.value == "^"
                                                     ? POW
                                                     : PLUS);

            if (op.priority < minPriority) {
                break; // приоритет операции меньше минимального => возвращаемся к меньшим приоритетам операции
            }

            consume(); // удовлетворяющая операция => съедаем ее (тк уже записали ее в op)
            auto right = parseBinary(op.priority + 1);
            left = std::make_shared<BinaryExpression<T> >(left, right, op.type);
        }

        return left;
    }

    // Вспомогательный метод парсера, который парсит
    std::shared_ptr<Expression<T> > parsePrimary() {
        if (current >= tokens.size()) {
            throw std::runtime_error("Unexpected end of input in parsePrimary");
        }

        auto token = consume(); // работаем со след токеном
        switch (token.type) {
            case NUMBER: return std::make_shared<ConstantExpression<T> >(std::stod(token.value));
            case COMPLEX: {
                if constexpr (std::is_same_v<T, std::complex<double>>) { // для нормального компила
                    return std::make_shared<ConstantExpression<T> >(std::complex<double>(0, std::stod(token.value)));
                } else {
                    return std::make_shared<ConstantExpression<T> >(std::stod(token.value));
                }
            }
            case VARIABLE: return std::make_shared<VarExpression<T> >(token.value);
            case FUNCTION: {
                auto arg = parsePrimary(); // тк ожидается скобка '(    '
                Function func = token.value == "sin"
                                    ? SIN
                                    : token.value == "cos"
                                          ? COS
                                          : token.value == "ln"
                                                ? LN
                                                : token.value == "exp"
                                                      ? EXP
                                                      : SIN;
                return std::make_shared<MonoExpression<T> >(arg, func);
            }
            case LEFT_PAREN: {
                cnt_par++;
                auto expr = parseExpression();
                if (!ismatch(RIGHT_PAREN)) {
                    throw std::runtime_error("Expected ')'");
                }
                cnt_par--;
                return expr;
            }
            default:
                throw std::runtime_error("Unexpected token: " + token.value);
        }
    }

public:
    explicit Parser(const std::vector<Token> &tokens) : tokens(tokens) {}

    std::shared_ptr<Expression<T> > parse() {
        return parseExpression();
    }
};

#endif // PARSER_H
