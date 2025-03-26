#define CATCH_CONFIG_MAIN
#include "../library/catch_amalgamated.hpp"

#include "Expression.h"
#include "Tokenator.h"
#include <Parser.h>

bool diff_double(const std::string &input, const std::string &expected, std::string by) {
    auto tokens = tokenize(input);
    Parser<double> parser(tokens);
    auto expr = parser.parse();
    auto diffExpr = expr->diff(by);
    diffExpr = optimize(diffExpr);
    std::cout << "d/d" << by << "(" << input << ") = " << diffExpr->to_string();
    std::cout << " || " << expected << " (expected)"<< std::endl;
    return diffExpr->to_string() == expected;
}

bool diff_complex(const std::string &input, const std::string &expected, std::string by) {
    auto tokens = tokenize(input);
    Parser<std::complex<double>> parser(tokens);
    auto expr = parser.parse();
    auto diffExpr = expr->diff(by);
    diffExpr = optimize(diffExpr);
    std::cout << "d/d" << by << "(" << input << ") = " << diffExpr->to_string();
    std::cout << " || " << expected << " (expected)"<< std::endl;
    return diffExpr->to_string() == expected;
}


TEST_CASE("Дифференцирование") {
    SECTION("DOUBLE") {
        CHECK(diff_double("x", "1", "x"));
        CHECK(diff_double("3 * x + 5", "3", "x"));
        CHECK(diff_double("x^2 + 4 * x - 7", "((2 * (x^1)) + 4)", "x"));
        CHECK(diff_double("x^5 - 3 * x^3 + 2 * x", "(((5 * (x^4)) - (3 * (3 * (x^2)))) + 2)", "x"));
        CHECK(diff_double("x * sin(x)", "(sin(x) + (x * cos(x)))", "x"));
        CHECK(diff_double("x / (x^2 + 1)", "((((x^2) + 1) - (x * (2 * (x^1)))) / (((x^2) + 1)^2))", "x"));
        CHECK(diff_double("exp(2 * x)", "(exp(2 * x) * 2)", "x"));
        CHECK(diff_double("ln(x^2 + 1)", "((2 * (x^1)) / ((x^2) + 1))", "x"));
        CHECK(diff_double("x^x", "(ln(x) + (x * (1 / x)))", "x"));
        CHECK(diff_double("sin(x^2)", "(cos(x^2) * (2 * (x^1)))", "x"));
        CHECK(diff_double("cos(ln(x))", "((-1 * sin(ln(x))) * (1 / x))", "x"));
        CHECK(diff_double("exp(x) * sin(x)", "((exp(x) * sin(x)) + (exp(x) * cos(x)))", "x"));
        CHECK(diff_double("ln(x) / x^3", "((((1 / x) * (x^3)) - (ln(x) * (3 * (x^2)))) / ((x^3)^2))", "x"));
        CHECK(diff_double("sin(x) * cos(x)", "((cos(x) * cos(x)) + (sin(x) * (-1 * sin(x))))", "x"));
    }
    SECTION("COMPLEX") {
        CHECK(diff_complex("(3+2i)*x + (1-4i)", "(3 + 2i)", "x"));
        CHECK(diff_complex("(1-i)*x + (2+3i)", "(1 - 1i)", "x"));
        CHECK(diff_complex("exp((1+2i)*x)", "(exp((1 + 2i) * x) * (1 + 2i))", "x"));
        CHECK(diff_complex("exp(i*x)", "(exp(1i * x) * 1i)", "x"));
        CHECK(diff_complex("sin((1-i)*x)", "(cos((1 - 1i) * x) * (1 - 1i))", "x"));
        CHECK(diff_complex("cos(2i*x)", "((-1 * sin(2i * x)) * 2i)", "x"));
        CHECK(diff_complex("ln((3+4i)*x)", "((3 + 4i) / ((3 + 4i) * x))", "x"));
        CHECK(diff_complex("ln(x + i)", "(1 / (x + 1i))", "x"));
        CHECK(diff_complex("exp(i*x)*sin(x)", "(((exp(1i * x) * 1i) * sin(x)) + (exp(1i * x) * cos(x)))", "x"));
        CHECK(diff_complex("(x-i)/(x+i)", "(((x + 1i) - (x - 1i)) / ((x + 1i)^2))", "x"));
    }
}

bool eval_double(const std::string &input, const double &expected, const std::map<std::string, double> &params) {
    auto tokens = tokenize(input);
    Parser<double> parser(tokens);
    auto expr = parser.parse();
    auto tmp = params;
    auto evalExpr = expr->eval(tmp);
    std::cout  << input << " = " << evalExpr << " || " << expected << " (expected)"<< std::endl;
    return evalExpr == expected;
}

bool eval_complex(const std::string &input, const std::complex<double> &expected, const std::map<std::string, std::complex<double>> &params) {
    auto tokens = tokenize(input);
    Parser<std::complex<double>> parser(tokens);
    auto expr = parser.parse();
    auto tmp = params;
    auto evalExpr = expr->eval(tmp);
    std::cout  << input << " = " << evalExpr << " || " << expected << " (expected)"<< std::endl;
    return evalExpr == expected;
}

std::complex<double> to_cm(double real, double imag = 0.0f) {
    return {real, imag};
}

TEST_CASE("Подсчет") {
    SECTION("DOUBLE") {
        CHECK(eval_double("x + y", 12, {{"x", 5}, {"y", 7}}));
        CHECK(eval_double("x - y", -2, {{"x", 5}, {"y", 7}}));
        CHECK(eval_double("x * y", 35, {{"x", 5}, {"y", 7}}));
        CHECK(eval_double("x / y", 5.0 / 7.0, {{"x", 5}, {"y", 7}}));
        CHECK(eval_double("x ^ y", std::pow(5, 3), {{"x", 5}, {"y", 3}}));

        CHECK(eval_double("sin(x)", std::sin(0.5), {{"x", 0.5}}));
        CHECK(eval_double("cos(x)", std::cos(1.0), {{"x", 1.0}}));
        CHECK(eval_double("ln(x)", std::log(2.0), {{"x", 2.0}}));
        CHECK(eval_double("exp(x)", std::exp(2.0), {{"x", 2.0}}));

        CHECK(eval_double("(a + b) * (c - d) / e", (6 + 2) * (10 - 4) / 4.0, {{"a", 6}, {"b", 2}, {"c", 10}, {"d", 4}, {"e", 4}}));
        CHECK(eval_double("sin(x) + cos(y)", std::sin(0.5) + std::cos(1.0), {{"x", 0.5}, {"y", 1.0}}));
        CHECK(eval_double("ln(x) / ln(2)", std::log(8) / std::log(2), {{"x", 8}}));
        CHECK(eval_double("exp(x) - 1", std::exp(2) - 1, {{"x", 2}}));
        CHECK(eval_double("(a + b) ^ c", std::pow(1 + 2, 3), {{"a", 1}, {"b", 2}, {"c", 3}}));
    }
    SECTION("COMPLEX") {
        CHECK(eval_complex("x + y - 4i", to_cm(5, 0),  {{"x", to_cm(1, 2)}, {"y", to_cm(4, 2)}}));
        CHECK(eval_complex("2i + x - y", to_cm(-3, 2), {{"x", to_cm(1, 2)}, {"y", to_cm(4, 2)}}));
        CHECK(eval_complex("x * y * i", to_cm(-5, 0), {{"x", to_cm(2, 1)}, {"y", to_cm(1, 2)}}));
        CHECK(eval_complex("x / y - i", to_cm(0, 0), {{"x", to_cm(2, 2)}, {"y", to_cm(2, -2)}}));
        CHECK(eval_complex("x ^ y", std::pow(to_cm(1, 1), to_cm(2, 0)), {{"x", to_cm(1, 1)}, {"y", to_cm(2, 0)}}));

        CHECK(eval_complex("sin(x)", std::sin(to_cm(1, 1)), {{"x", to_cm(1, 1)}}));
        CHECK(eval_complex("cos(x)", std::cos(to_cm(1, 1)), {{"x", to_cm(1, 1)}}));
        CHECK(eval_complex("ln(x)", std::log(to_cm(2, 3)), {{"x", to_cm(2, 3)}}));
        CHECK(eval_complex("exp(x)", std::exp(to_cm(1, 1)), {{"x", to_cm(1, 1)}}));

        CHECK(eval_complex("(a + b) * (c - d) / e",
              ((to_cm(1, 1) + to_cm(2, -1)) * (to_cm(3, 4) - to_cm(1, 2))) / to_cm(2, 0),
              {{"a", to_cm(1, 1)}, {"b", to_cm(2, -1)}, {"c", to_cm(3, 4)}, {"d", to_cm(1, 2)}, {"e", to_cm(2, 0)}}));
        CHECK(eval_complex("ln(-1)", to_cm(0, M_PI), {{"x", to_cm(-1, 0)}})); // Комплексный логарифм
        CHECK(eval_complex("(x + y) ^ (a - b)", std::pow(to_cm(2, 3), to_cm(1, -1)), {{"x", to_cm(1, 1)}, {"y", to_cm(1, 2)}, {"a", to_cm(2, 0)}, {"b", to_cm(1, 1)}}));
    }
}