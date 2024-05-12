#include<iostream>
#include<string>
#include<fstream>
#include"Lox.hpp"

lox::lox(/* args */)
{
    
}

lox::~lox()
{
}

void lox::run(const std::string& source)
{
    Scanner scanner{source};
    const auto tokens = scanner.scanTokens();
    for(auto token: tokens)
    {
        std::cout<<token.toString()<<std::endl;
    }
}


void lox::runPrompt()
{
    std::cout<<"lox v0.0.1"<<std::endl;

    std::string input;
    while (true)
    {
        std::cout<<">>";
        if(std::getline(std::cin, input))
        {
            run(input);
        }
        else
        {
            std::cout<<std::endl;
            break;
        }
    }
}


void lox::runFile(const std::string& filename)
{
    std::cout<<"Runing file '"<<filename<<"'..."<<std::endl;
    std::ifstream file{filename};
    std::string line;
    std::string source;

    while (std::getline(file, line))
    {
        source += line +"\n";
    }
    
    run(source);
}


void lox::report(int line, const std::string& Where, const std::string& message)
{
    std::cout<<"line: "<<line<<"  "+Where <<message<<std::endl;
}