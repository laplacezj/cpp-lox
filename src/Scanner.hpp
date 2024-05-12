#pragma once

#include<string>
#include"Token.hpp"
#include<vector>

class Scanner
{
private:
    /* data */
public:
    Scanner(const std::string& source);
    ~Scanner();
    std::vector<Token> scanTokens();
};

Scanner::Scanner(const std::string& source)
{
}

Scanner::~Scanner()
{
}


