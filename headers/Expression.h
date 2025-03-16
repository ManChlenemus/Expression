#ifndef EXPRESSION_H
#define EXPRESSION_H
#include <cmath>
#include <iostream>
#include <map>
#include <memory>
#include <string>

enum operation { PLUS, MINUS, MULT, DIV, POW };

enum function { SIN, COS, LN, EXP };

struct operators {
    operation type; // тип
    int priority; // приоритет
    explicit operators(operation type); // конструктор
};

template <typename T>
struct Expression {
    virtual ~Expression() = default;

    virtual std::string to_string() = 0;
    virtual T eval(std::map<std::string, T> &parameters) = 0;
    virtual std::shared_ptr<Expression> diff(std::string &str) = 0;
};

template <typename T>
class ConstantExpression: public Expression<T> {
    T value;
public:
    explicit ConstantExpression(T value) : value(value) {}
    ~ConstantExpression() override = default;
    ConstantExpression(const ConstantExpression<T> &other) = default;
    ConstantExpression(ConstantExpression<T> &&other) = default;
    ConstantExpression &operator=(const ConstantExpression<T> &other) = default;
    ConstantExpression &operator=(ConstantExpression<T> &&other) = default;

    T getValue() { return value; }
    T eval(std::map<std::string, T> &parameters) override {
        return value;
    }
    std::shared_ptr<Expression<T>> diff(std::string &str) override {
        return std::make_shared<ConstantExpression<T>>(ConstantExpression<T>(0));
    }
    std::string to_string() override {
        return std::to_string(value);
    }
};

template <typename T>
class VarExpression: public Expression<T> {
    std::string value;
public:
    explicit VarExpression(std::string &value) : value(value) {}
    ~VarExpression() override = default;
    VarExpression(const VarExpression<T> &other) = default;
    VarExpression(VarExpression<T> &&other) = default;
    VarExpression &operator=(const VarExpression<T> &other) = default;
    VarExpression &operator=(VarExpression<T> &&other) = default;

    T eval(std::map<std::string, T> &parameters) override {
        return parameters[value];
    }
    std::shared_ptr<Expression<T>> diff(std::string &str) override {
        if (str == value) return std::make_shared<ConstantExpression<T>>(ConstantExpression<T>(T(1)));
        return std::make_shared<VarExpression<T>>(T(0));
    }
    std::string to_string() override {
        return value;
    }
};

template <typename T>
class MonoExpression: public Expression<T> {
    std::shared_ptr<Expression<T>> expr;
    function func;
public:
    MonoExpression(const std::shared_ptr<Expression<T>> &expr, function func) {
        this->expr = expr;
        this->func = func;
    }
    ~MonoExpression() override = default;
    MonoExpression(const MonoExpression<T> &other) = default;
    MonoExpression(MonoExpression<T> &&other) = default;
    MonoExpression &operator=(const MonoExpression<T> &other) = default;
    MonoExpression &operator=(MonoExpression<T> &&other) = default;

    T eval(std::map<std::string, T> &parameters) override {
        switch (func) {
            case SIN:
                return std::sin(expr->eval(parameters));
            case COS:
                return std::cos(expr->eval(parameters));
            case LN:
                return std::log(expr->eval(parameters));
            case EXP:
                return std::exp(expr->eval(parameters));
            default:
                std::cout << "Unknown function" << std::endl; // Не может быть (на всякий)
                return -1;
        }
    }
    std::shared_ptr<Expression<T>> diff(std::string &str) override; // реализация ниже
    std::string to_string() override {
        std::string tmp;
        switch (func) {
            case COS: tmp = "cos"; break;
            case SIN: tmp = "sin"; break;
            case LN:  tmp = "ln";  break;
            case EXP: tmp = "exp"; break;
        }
        return tmp + "(" + expr->to_string() + ")";
    }
};

template <typename T>
class BinaryExpression: public Expression<T> {
    std::shared_ptr<Expression<T>> left;
    std::shared_ptr<Expression<T>> right;
    operation op;
public:
    BinaryExpression(const std::shared_ptr<Expression<T>> &left, const std::shared_ptr<Expression<T>> &right, operation op)
    :   left(left), right(right), op(op) {}
    ~BinaryExpression() override = default;
    BinaryExpression(const BinaryExpression<T> &other) = default;
    BinaryExpression(BinaryExpression<T> &&other) = default;
    BinaryExpression &operator=(const BinaryExpression<T> &other) = default;
    BinaryExpression &operator=(BinaryExpression<T> &&other) = default;

    std::shared_ptr<Expression<T>> getLeft() { return left; }
    std::shared_ptr<Expression<T>> getRight() { return right; }
    void setLeft(std::shared_ptr<Expression<T>> left) { this->left = left; }
    void setRight(std::shared_ptr<Expression<T>> right) { this->right = right; }
    operation getOper() { return op; }

    T eval(std::map<std::string, T> &parameters) override {
        switch (op) {
            case PLUS:
                return left->eval(parameters) + right->eval(parameters);
            case MINUS:
                return left->eval(parameters) - right->eval(parameters);
            case MULT:
                return left->eval(parameters) * right->eval(parameters);
            case DIV:
                return left->eval(parameters) / right->eval(parameters);
            case POW:
                return std::pow(left->eval(parameters), right->eval(parameters));
            default: std::cout << "Unknown operation" << std::endl; // Не может быть
                return -1;
        }
    }
    std::shared_ptr<Expression<T>> diff(std::string &str) override;
    std::string to_string() override;
};

template<typename T>
std::shared_ptr<Expression<T> > MonoExpression<T>::diff(std::string &str) {
    auto expr_diff = expr->diff(str);
    switch (func) {
        case SIN:
            return std::make_shared<BinaryExpression<T>>(std::make_shared<MonoExpression<T>>
                (MonoExpression<T>(expr, COS)), expr_diff, MULT);
        case COS:
            return std::make_shared<BinaryExpression<T>>(std::make_shared<BinaryExpression<T>>
                (std::make_shared<ConstantExpression<T>>(ConstantExpression<T>(T(-1))),
                 std::make_shared<MonoExpression<T>>(MonoExpression<T>(expr, SIN))),
                 expr_diff, MULT);
        case LN:
            return std::make_shared<BinaryExpression<T>>(std::make_shared<BinaryExpression<T>>(
                expr_diff, expr, DIV));
        case EXP:
            return std::make_shared<BinaryExpression<T>>(std::make_shared<BinaryExpression<T>>
                (std::make_shared<MonoExpression<T>>((MonoExpression<T>(expr, EXP))),
                expr_diff, MULT));
        default: std::cout << "Unknown operation" << std::endl; // не может быть
            return nullptr;
    }
}


#endif //EXPRESSION_H
