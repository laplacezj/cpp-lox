#pragma once

#include<string>
#include"Token.hpp"
#include<vector>
#include<unordered_map>

class Scanner
{

public:
    Scanner(const std::string& source);
    ~Scanner();
    std::vector<Token> scanTokens();

private:
    //data
    std::string source;
    std::vector<Token> tokens;
    std::unordered_map<std::string,TokenType> keywords;

    int start{0};
    int current{0};
    int line{1};

    bool isAtEnd() const;
    void scanToken();

    char advance();

    void addToken(TokenType type, std::any literal);
    void addToken(TokenType type);
    bool match(char expect);
    char peek();
    char peekNext();
    void string();

    bool isDigit(char c);
    bool isAlpha(char c);

    void identifier();
    bool isAlphaNumeric(char c);
    void number();


};







