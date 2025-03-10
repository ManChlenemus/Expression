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
    std::unique_ptr<Token> next = nullptr;

    Token(const TokenType type, const std::string& value) noexcept
        : type(type), value(value) {}
};

std::unique_ptr<Token> tokenize(const std::string& str);
void printTokens(const Token* token);

#endif //TOKENATOR_H
