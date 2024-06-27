#pragma once

#include<string>
#include"Token.hpp"
#include<vector>
#include<unordered_map>

namespace Lox
{
    class Scanner
    {

    public:
        Scanner(const std::string& source);
        ~Scanner();
        std::vector<Token> scanTokens();

    private:
        bool isAtEnd() const;
        char advance();
        
        bool match(char expect);
        char peek() const;
        char peekNext() const;

        void addToken(TokenType type, std::any literal);
        void addToken(TokenType type);

        void comment();
        bool isDigit(char c);
        bool isAlpha(char c);

        void string();
        void number();
        void identifier();
        bool isAlphaNumeric(char c);
        

        void scanToken();

           //data
        std::string source;
        std::vector<Token> tokens;
        std::unordered_map<std::string,TokenType> keywords;
         
        int start{0};
        int current{0};
        int line{1};

    };
}









