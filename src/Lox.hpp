#include<iostream>
#include<string>
#include<fstream>
#include"Scanner.hpp"

class lox
{
private:
    static void report(int line, const std::string& Where, const std::string& message);
public:
    lox(/* args */);
    ~lox();

    void run(const std::string& source);
    void runFile(const std::string& filename);
    void runPrompt();
    
    static bool hadError;
 
    static void error(int line, const std::string& message);
};

bool lox::hadError = false;

void lox::error(int line, const std::string& message)
{

}
