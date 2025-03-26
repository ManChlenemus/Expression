#include <iostream>
#include <complex>
#include <map>
#include <memory>
#include <string>
#include "headers/Expression.h"
#include "headers/Tokenator.h"
#include "headers/Parser.h"

int main(int argc, char* argv[]) {
    /*std::map<std::string, double> params;
    std::string par = "y";
    std::string str = "y * sin(x + y)";
    auto tokens = tokenize(str);
    Parser<double> parser(tokens);
    auto expr = parser.parse();
    auto diffExpr = expr->diff(par);
    diffExpr = optimize(diffExpr);
    std::cout << diffExpr->to_string() << std::endl;
    return 0;*/


     if (argc < 2) {
         std::cerr << "Usage: differentiator --eval <expression> [variable=value ...]" << std::endl;
         std::cerr << "       differentiator --diff <expression> --by <variable>" << std::endl;
         return 1;
     }
     std::string mode = argv[1];  // Режим работы (--eval или --diff)
     std::string expression = argv[2];  // Выражение

     if (mode == "--eval") {
         std::map<std::string, std::complex<double>> params;

         for (int i = 3; i < argc; i++) {
             std::string arg = argv[i];
             size_t eqPos = arg.find('=');
             if (eqPos == std::string::npos) {
                 std::cerr << "Invalid argument: " << arg << std::endl;
                 return 1;
             }
             std::string var = arg.substr(0, eqPos);

             std::string val_expr = arg.substr(eqPos + 1);
             auto val_tokens = tokenize(val_expr);
             Parser<std::complex<double>> val_expr_parser(val_tokens);
             auto expr = val_expr_parser.parse();
             std::map<std::string, std::complex<double>> result;
             std::complex<double> value = expr->eval(result);

             params[var] = value;
         }

         auto tokens = tokenize(expression);
         Parser<std::complex<double>> parser(tokens);
         auto expr = parser.parse();
         std::cout << expr->eval(params) << std::endl;

     } else if (mode == "--diff") {
         if (argc < 5 || std::string(argv[3]) != "--by") {
             std::cerr << "Usage: differentiator --diff <expression> --by <variable>" << std::endl;
             return 1;
         }

         std::string diffVar = argv[4];  // Переменная, по которой дифференцируем
         auto tokens = tokenize(expression);
         Parser<std::complex<double>> parser(tokens);
         auto expr = parser.parse();
         auto diffExpr = expr->diff(diffVar);
         optimize(diffExpr);
         std::cout << diffExpr->to_string() << std::endl;
     } else {
         std::cerr << "Unknown mode: " << mode << std::endl;
         return 1;
     }
     return 0;
}