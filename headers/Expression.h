#ifndef EXPRESSION_H
#define EXPRESSION_H
#include <map>
#include <memory>
#include <string>

enum operation { PLUS, MINUS, MULT, DIV, POW };

enum function { SIN, COS, LN, EXP };

struct operators {
    operation type;
    int priority;
    explicit operators(operation type); // для явного преобразования
};

template <typename T>
class Expression {
public:
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
    T eval(std::map<std::string, T> &parameters) override;
    std::shared_ptr<Expression<T>> diff(std::string &str) override;
    std::string to_string() override;
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

    T eval(std::map<std::string, T> &parameters) override;
    std::shared_ptr<Expression<T>> diff(std::string &str) override;
    std::string to_string() override;
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

    T eval(std::map<std::string, T> &parameters) override;
    std::shared_ptr<Expression<T>> diff(std::string &str) override;
    std::string to_string() override;
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

    T eval(std::map<std::string, T> &parameters) override;
    std::shared_ptr<Expression<T>> diff(std::string &str) override;
    std::string to_string() override;
};




#endif //EXPRESSION_H
