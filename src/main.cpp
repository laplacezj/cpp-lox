#include<fstream>
#include<iostream>
#include<string>
#include"Lox.hpp"


int main(int args, char* argv[])
{
    lox myLox;

    if (args > 2)
    {
        std::cout<<"usage Lox[script]"<<std::endl;
        exit(1);
    }
    else if (args == 2)
    {
        myLox.runFile(argv[1]);
    }
    else
    {
        myLox.runPrompt();
    }
    
    
}