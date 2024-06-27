#pragma once
#include<string>
#include<any>
#include"TokenType.hpp"

namespace Lox
{
    class Token
    {
    private:
        TokenType type;
        std::string lexeme;
        std::any literal;
        int line;
    public:
        Token(TokenType type, std::string lexeme, int line);
        Token(TokenType type, std::string lexeme, std::any literal, int line);
        ~Token();

        TokenType getType()const{return type;}
        const std::string& getText() const { return lexeme; }
        const std::any& getLiteral() const { return literal; }
        int getLine() const {return line;}
        std::string literalToString() const;
        std::string toString() const;
    };
}



