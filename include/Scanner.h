#pragma once
#include <string>
#include <vector>
#include <map>
#include "Token.h"
#include "TokenType.h"

class Scanner
{

    static const std::map<std::string, TokenType> keywords;

    std::string_view source;
    std::vector<Token> tokens;
    int start = 0;
    int current = 0;
    int line = 1;

private:
    void scanToken();

    bool isAtEnd() { return current >= source.length(); }
    char advance() { return source.at(current++); }
    void addToken(TokenType type, std::any literal);

    void addToken(TokenType type)
    {
        addToken(type, nullptr);
    }

    bool match(char expected);
    char peek()
    {
        if (isAtEnd())
            return '\0';

        return source.at(current);
    }

    char peekNext()
    {
        if (current + 1 >= source.length())
            return '\0';
        return source.at(current + 1);
    }

    void string();
    bool isDigit(char c) { return c >= '0' && c <= '9'; }

    void number();

    bool isAlpha(char c) { return (c >= 'a' && c <= 'z') ||
                                  (c >= 'A' && c <= 'Z') ||
                                  c == '_'; }
    void identifier();
    bool isAlphaNumeric(char c){return isdigit(c) || isAlpha(c) ;}
public:
    Scanner(std::string_view source);

    std::vector<Token> scanTokens();
};
