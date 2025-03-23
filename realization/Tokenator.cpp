#include "../headers/Tokenator.h"
#include <iostream>

std::string lower(const std::string& str) {
    std::string res(str);
    for (char& c : res) {
        c = static_cast<char>(std::tolower(c));
    }
    return res;
}

std::vector<Token> tokenize(const std::string &input) {
    std::vector<Token> tokens;
    size_t i = 0;

    while (i < input.size()) {
        char c = input[i];

        if (std::isspace(c)) {
            i++;
            continue;
        }

        if (isdigit(c) || c == '.') {
            std::string numStr;
            while (i < input.size() && (std::isdigit(input[i]) || input[i] == '.')) {
                numStr += input[i];
                i++;
            }
            tokens.emplace_back(NUMBER, numStr);
            continue;
        }

        if (c == 'i') {
            if (!tokens.empty() && tokens.back().type == NUMBER) {
                tokens.back().type = COMPLEX;
            } else {
                if (!tokens.empty() && tokens.back().type == RIGHT_PAREN)
                    tokens.emplace_back(OPERATOR, "*");
                tokens.emplace_back(COMPLEX, "1");
            }
            i++;
            continue;
        }

        if (std::isalpha(c)) {
            std::string name;
            while (i < input.size() && std::isalpha(input[i])) {
                name += input[i];
                i++;
            }
            name = lower(name);
            if (name == "sin" || name == "cos" || name == "ln" || name == "exp") {
                tokens.emplace_back(FUNCTION, name);
            } else {
                tokens.emplace_back(VARIABLE, name);
            }
            continue;
        }

        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
            tokens.emplace_back(OPERATOR, std::string(1, c));
            i++;
            continue;
        }

        if (c == '(') {
            tokens.emplace_back(LEFT_PAREN, "(");
            i++;
            continue;
        }
        if (c == ')') {
            tokens.emplace_back(RIGHT_PAREN, ")");
            i++;
            continue;
        }
        throw std::runtime_error("Unknown character: " + std::string(1, c));
    }
    return tokens;
}

void printTokens(std::vector<Token> &tokens) {
    for (const auto &token : tokens) {
        switch (token.type) {
            case NUMBER: std::cout << "NUMBER: " << token.value << std::endl; break;
            case COMPLEX: std::cout << "COMPLEX: " << token.value << std::endl; break;
            case FUNCTION: std::cout << "FUNCTION: " << token.value << std::endl; break;
            case VARIABLE: std::cout << "VARIABLE: " << token.value << std::endl; break;
            case OPERATOR: std::cout << "OPERATOR: " << token.value << std::endl; break;
            case LEFT_PAREN: std::cout << "LEFT_P: (" << std::endl; break;
            case RIGHT_PAREN: std::cout << "RIGHT_P: )" << std::endl; break;
        }
    }
}