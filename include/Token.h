#pragma once
#include "TokenType.h"
#include <any>
#include<string>

class Token
{
private:
    const TokenType type;
    const std::string lexeme;
    const std::any literal;
    const int line;
    

public:
    Token(TokenType type, std::string lexeme, const std::any literal, int line);
    ~Token();

    std::string toString() const;
    

};
