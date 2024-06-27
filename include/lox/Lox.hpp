#pragma once
#include<iostream>
#include<string>
#include<fstream>
#include"Scanner.hpp"
#include"Token.hpp"

namespace Lox{
    
    class lox
    {
    private:
    
    public:
    lox(/* args */);
    ~lox();

    void run(const std::string& source);
    void runFile(const std::string& filename);
    void runPrompt();

    static void report(int line, const std::string& Where, const std::string& message);
    static void error(int line, const std::string& message);
    static void error(Token token, const std::string& message);
    static bool hadError;
    };
}



