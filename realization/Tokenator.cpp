#include "../headers/Tokenator.h"
#include <iostream>

std::string lower(const std::string& str) {
    std::string res(str);
    for (char& c : res) {
        c = static_cast<char>(std::tolower(c));
    }
    return res;
}

std::unique_ptr<Token> tokenize(const std::string& str) {
    const auto root = std::make_unique<Token>(BEGIN,"");
    Token* curr = root.get();
    size_t pos = 0;
    while (pos < str.size()) {
        // Исключения
        while (pos < str.size() && isspace(str[pos])) pos++;
        if (pos >= str.size()) break;

        // Основная часть
        if (str[pos] == '(' || str[pos] == ')') {
            std::string bracket = str[pos] == '(' ? "(" : ")";
            curr->next = std::make_unique<Token>(BRACKET, bracket);
            curr = curr->next.get();
            pos++;
        }
        else if (str[pos] == '+' || str[pos] == '-' || str[pos] == '*'|| str[pos] == '/' || str[pos] == '^') {
            std::string op; op += str[pos];
            curr->next = std::make_unique<Token>(OPERATOR, op);
            curr = curr->next.get();
            pos++;
        }
        else if (isdigit(str[pos])) {
            const size_t start = pos;
            while (pos < str.size() && (isdigit(str[pos]) || str[pos] == '.')) pos++;
            curr->next = std::make_unique<Token>(NUMBER, str.substr(start, pos - start));
            curr = curr->next.get();
        }
        else if (isalpha(str[pos])) {
            const size_t start = pos;
            while (pos < str.size() && isalpha(str[pos])) pos++;
            curr->next = make_unique<Token>(VARIABLE, str.substr(start, pos - start));
            curr = curr->next.get();

            for (std::string fun : {"sin", "cos", "ln", "exp"}) {
                if (fun == lower(curr->value)) {
                    curr->type = FUNCTION;
                    curr->value = lower(curr->value);
                    break;
                }
            }
        }
        else std::cout << "error: unrecognized token type" << std::endl;
    }
    return std::move(root->next);
}

void printTokens(const Token* token) {
    if (token != nullptr) {
        std::cout << token->value << std::endl;
        if (token->next != nullptr) {
            printTokens(token->next.get());
        }
    }
}