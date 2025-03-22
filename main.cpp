#include "headers/Expression.h"
#include <iostream>
#include <map>
#include <memory>

int main() {
    std::map<std::string, int> m;
    m["a"] = 1;
    m["b"] = 2;
    std::string aa = "a";
    std::string bb = "b";
    std::shared_ptr<BinaryExpression<int>> ptr = make_shared<BinaryExpression<int>>(std::make_shared<VarExpression<int>>(aa),
        std::make_shared<VarExpression<int>>(bb), PLUS);
    std::cout << ptr->eval(m) << std::endl;

    return 0;
}
