#pragma once
#include<string>
#include<any>
#include"TokenType.hpp"

class Token
{
private:
    TokenType type;
    std::string lexeme;
    std::any literal;
    int line;
public:
    Token(TokenType type, std::string lexeme, std::any literal, int line);
    ~Token();


    const std::string& getText() const { return lexeme; }
    std::string Token::literalToString() const;
    std::string toString() const;


};


