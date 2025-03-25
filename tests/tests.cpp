#include <gtest/gtest.h>
#include "../headers/Expression.h"
#include "../headers/Tokenator.h"
#include "../headers/Parser.h"

TEST(ExpressionTest, ConstantExpression) {
    auto expr = std::make_shared<ConstantExpression<double>>(5.0);
    std::map<std::string, double> params;
    EXPECT_DOUBLE_EQ(expr->eval(params), 5.0);
    EXPECT_EQ(expr->to_string(), "5.000000");
}

// Тест для переменных
TEST(ExpressionTest, VariableExpression) {
    auto expr = std::make_shared<VarExpression<double>>("x");
    std::map<std::string, double> params = {{"x", 2.0}};
    EXPECT_DOUBLE_EQ(expr->eval(params), 2.0);
    EXPECT_EQ(expr->to_string(), "x");
}

// Тест для сложения
TEST(ExpressionTest, Addition) {
    auto left = std::make_shared<ConstantExpression<double>>(2.0);
    auto right = std::make_shared<ConstantExpression<double>>(3.0);
    auto expr = std::make_shared<BinaryExpression<double>>(left, right, PLUS);
    std::map<std::string, double> params;
    EXPECT_DOUBLE_EQ(expr->eval(params), 5.0);
    EXPECT_EQ(expr->to_string(), "(2.000000 + 3.000000)");
}

// Тест для дифференцирования
TEST(ExpressionTest, Differentiation) {
    auto var = std::make_shared<VarExpression<double>>("x");
    auto c = std::make_shared<ConstantExpression<double>>(2.0);
    auto expr = std::make_shared<BinaryExpression<double>>(var, c, MULT); // x * 2
    std::string diffVar = "x";
    auto diffExpr = expr->diff(diffVar);
    std::map<std::string, double> params = {{"x", 1.0}};
    EXPECT_DOUBLE_EQ(diffExpr->eval(params), 2.0); // Производная: 2
}

// Тест для оптимизации (упрощения выражения)
TEST(ExpressionTest, Optimization) {
    auto zero = std::make_shared<ConstantExpression<double>>(0.0);
    auto var = std::make_shared<VarExpression<double>>("x");
    auto expr = std::make_shared<BinaryExpression<double>>(zero, var, PLUS); // 0 + x
    auto optimized = optimize(expr);
    EXPECT_EQ(optimized->to_string(), "x"); // Должно упроститься до x
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}