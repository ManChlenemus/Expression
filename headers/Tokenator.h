#ifndef TOKENATOR_H
#define TOKENATOR_H
#include <memory>
#include <string>

enum TokenType {
    BEGIN,
    OPERATOR,
    NUMBER,
    VARIABLE,
    FUNCTION,
    BRACKET
};

struct Token {
    TokenType type;
    std::string value;
    std::unique_ptr<Token> next;

    Token(TokenType type, std::string value) : type(type), value(value) {
        next = nullptr;
    }
};

std::unique_ptr<Token> tokenize(std::string input);
void printTokens(const Token* token);

#endif //TOKENATOR_H
