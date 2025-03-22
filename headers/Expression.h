#ifndef EXPRESSION_H
#define EXPRESSION_H
#include <cmath>
#include <complex>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <stdexcept>

enum Operation { PLUS, MINUS, MULT, DIV, POW };
enum Function { SIN, COS, LN, EXP };

struct operators {
    Operation type; // тип
    int priority; // приоритет
    explicit operators(Operation type); // конструктор
};

template <typename T>
struct Expression {
    virtual ~Expression() = default;

    virtual std::string to_string() = 0;
    virtual T eval(std::map<std::string, T> &parameters) = 0;
    virtual std::shared_ptr<Expression<T>> diff(std::string &str) = 0;
};

template <typename T>
class ConstantExpression : public Expression<T> {
    T value;
public:
    explicit ConstantExpression(T value) : value(value) {}
    ~ConstantExpression() override = default;
    ConstantExpression(const ConstantExpression<T> &other) = default;
    ConstantExpression(ConstantExpression<T> &&other) = default;
    ConstantExpression &operator=(const ConstantExpression<T> &other) = default;
    ConstantExpression &operator=(ConstantExpression<T> &&other) = default;

    T eval(std::map<std::string, T> &parameters) override {
        return value;
    }
    std::shared_ptr<Expression<T>> diff(std::string &str) override {
        return std::make_shared<ConstantExpression<T>>(T(0));
    }
    std::string to_string() override {
        if constexpr (std::is_same_v<T, std::complex<double>>) {
            // Специальная обработка для комплексных чисел
            double real = value.real();
            double imag = value.imag();
            if (imag >= 0) {
                return std::to_string(real) + " + " + std::to_string(imag) + "i";
            } else {
                return std::to_string(real) + " - " + std::to_string(-imag) + "i";
            }
        } else {
            return std::to_string(value);
        }
    }
};

template <typename T>
class VarExpression : public Expression<T> {
    std::string value;
public:
    explicit VarExpression(const std::string &value) : value(value) {}
    ~VarExpression() override = default;
    VarExpression(const VarExpression<T> &other) = default;
    VarExpression(VarExpression<T> &&other) = default;
    VarExpression &operator=(const VarExpression<T> &other) = default;
    VarExpression &operator=(VarExpression<T> &&other) = default;

    T eval(std::map<std::string, T> &parameters) override {
        return parameters[value];
    }
    std::shared_ptr<Expression<T>> diff(std::string &str) override {
        if (str == value) return std::make_shared<ConstantExpression<T>>(T(1));
        return std::make_shared<ConstantExpression<T>>(T(0));
    }
    std::string to_string() override {
        return value;
    }
};

template <typename T>
class MonoExpression: public Expression<T> {
    std::shared_ptr<Expression<T>> expr;
    Function func;
public:
    MonoExpression(const std::shared_ptr<Expression<T>> &expr, Function func)
        : expr(expr), func(func) {}
    ~MonoExpression() override = default;
    MonoExpression(const MonoExpression<T> &other) = default;
    MonoExpression(MonoExpression<T> &&other) = default;
    MonoExpression &operator=(const MonoExpression<T> &other) = default;
    MonoExpression &operator=(MonoExpression<T> &&other) = default;

    T eval(std::map<std::string, T> &parameters) override {
        switch (func) {
            case SIN: return std::sin(expr->eval(parameters));
            case COS: return std::cos(expr->eval(parameters));
            case LN: return std::log(expr->eval(parameters));
            case EXP: return std::exp(expr->eval(parameters));
            default: throw std::runtime_error("Unknown function");
        }
    }
    std::shared_ptr<Expression<T>> diff(std::string &str) override; // реализация ниже
    std::string to_string() override {
        switch (func) {
            case SIN: return "sin(" + expr->to_string() + ")";
            case COS: return "cos(" + expr->to_string() + ")";
            case LN: return "ln(" + expr->to_string() + ")";
            case EXP: return "exp(" + expr->to_string() + ")";
            default: return "Unknown function";
        }
    }

};

template <typename T>
class BinaryExpression: public Expression<T> {
    std::shared_ptr<Expression<T>> left;
    std::shared_ptr<Expression<T>> right;
    Operation op;
public:
    BinaryExpression(const std::shared_ptr<Expression<T>> &left,
                     const std::shared_ptr<Expression<T>> &right,
                     Operation op)
        : left(left), right(right), op(op) {}
    ~BinaryExpression() override = default;
    BinaryExpression(const BinaryExpression<T> &other) = default;
    BinaryExpression(BinaryExpression<T> &&other) = default;
    BinaryExpression &operator=(const BinaryExpression<T> &other) = default;
    BinaryExpression &operator=(BinaryExpression<T> &&other) = default;

    /*std::shared_ptr<Expression<T>> getLeft() { return left; }
    std::shared_ptr<Expression<T>> getRight() { return right; }
    void setLeft(std::shared_ptr<Expression<T>> left) { this->left = left; }
    void setRight(std::shared_ptr<Expression<T>> right) { this->right = right; }
    operation getOper() { return op; }*/

    T eval(std::map<std::string, T> &parameters) override {
        switch (op) {
            case PLUS: return left->eval(parameters) + right->eval(parameters);
            case MINUS: return left->eval(parameters) - right->eval(parameters);
            case MULT: return left->eval(parameters) * right->eval(parameters);
            case DIV:
                if (right->eval(parameters) == T(0)) throw std::runtime_error("Division by zero");
                return left->eval(parameters) / right->eval(parameters);
            case POW: return std::pow(left->eval(parameters), right->eval(parameters));
            default: throw std::runtime_error("Unknown operation");

        }
    }
    std::shared_ptr<Expression<T>> diff(std::string &str) override {
        auto left_diff = left->diff(str);
        auto right_diff = right->diff(str);
        switch (op) {
            case PLUS:
                return std::make_shared<BinaryExpression<T>>(left_diff, right_diff, PLUS);
            case MINUS:
                return std::make_shared<BinaryExpression<T>>(left_diff, right_diff, MINUS);
            case MULT: {
                auto left_mult = std::make_shared<BinaryExpression<T>>(left_diff, right, MULT);
                auto right_mult = std::make_shared<BinaryExpression<T>>(left, right_diff, MULT);
                return std::make_shared<BinaryExpression<T>>(left_mult, right_mult, PLUS);
            }
            case DIV: {
                auto numerator = std::make_shared<BinaryExpression<T>>(
                    std::make_shared<BinaryExpression<T>>(left_diff, right, MULT),
                    std::make_shared<BinaryExpression<T>>(left, right_diff, MULT),
                    MINUS);
                auto denominator = std::make_shared<BinaryExpression<T>>(
                    right, std::make_shared<ConstantExpression<T>>(T(2)), POW);
                return std::make_shared<BinaryExpression<T>>(numerator, denominator, DIV);
            }
            case POW: {
                auto term1 = std::make_shared<BinaryExpression<T>>(
                    right_diff, std::make_shared<MonoExpression<T>>(left, LN), MULT);
                auto term2 = std::make_shared<BinaryExpression<T>>(
                    right, std::make_shared<BinaryExpression<T>>(left_diff, left, DIV), MULT);
                return std::make_shared<BinaryExpression<T>>(term1, term2, PLUS);
            }
            default: throw std::runtime_error("Unknown operation");
        }
    }
    std::string to_string() override {
        switch (op) {
            case PLUS: return "(" + left->to_string() + " + " + right->to_string() + ")";
            case MINUS: return "(" + left->to_string() + " - " + right->to_string() + ")";
            case MULT: return "(" + left->to_string() + " * " + right->to_string() + ")";
            case DIV: return "(" + left->to_string() + " / " + right->to_string() + ")";
            case POW: return "(" + left->to_string() + "^" + right->to_string() + ")";
            default: return "Unknown operation";
        }
    }
};

template<typename T>
std::shared_ptr<Expression<T> > MonoExpression<T>::diff(std::string &str) {
    auto expr_diff = expr->diff(str);
    switch (func) {
        case SIN:
            return std::make_shared<BinaryExpression<T>>(
                std::make_shared<MonoExpression<T>>(expr, COS),
                expr_diff, MULT);
        case COS:
            return std::make_shared<BinaryExpression<T>>(
                std::make_shared<BinaryExpression<T>>(
                    std::make_shared<ConstantExpression<T>>(T(-1)),
                    std::make_shared<MonoExpression<T>>(expr, SIN),
                    MULT),
                expr_diff, MULT);
        case LN:
            return std::make_shared<BinaryExpression<T>>(expr_diff, expr, DIV);
        case EXP:
            return std::make_shared<BinaryExpression<T>>(
                std::make_shared<MonoExpression<T>>(expr, EXP),
                expr_diff, MULT);
        default: throw std::runtime_error("Unknown function");
    }
}

#endif //EXPRESSION_H
