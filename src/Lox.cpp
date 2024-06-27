#include <iostream>
#include <string>
#include <fstream>
#include "lox/Lox.hpp"

namespace Lox
{
    bool lox::hadError = false;

    lox::lox(/* args */)
    {
    }

    lox::~lox()
    {
    }

    void lox::run(const std::string &source)
    {
        Scanner scanner{source};
        const auto tokens = scanner.scanTokens();
        for (auto token : tokens)
        {
            std::cout << token.toString() << std::endl;
        }
    }

    void lox::runPrompt()
    {
        std::cout << "lox v0.0.1" << std::endl;

        std::string input;
        while (true)
        {
            std::cout << ">>";
            if (std::getline(std::cin, input))
            {
                run(input);
                hadError = true; // 每一行之后进行重置
            }
            else
            {
                std::cout << std::endl;
                break;
            }
        }
    }

    void lox::runFile(const std::string &filename)
    {
        std::cout << "Runing file '" << filename << "'..." << std::endl;
        std::ifstream file{filename};
        std::string line;
        std::string source;

        while (std::getline(file, line))
        {
            source += line + "\n";
        }

        run(source);

        if (hadError == false)
            exit(2);
    }

    void lox::report(int line, const std::string &Where, const std::string &message)
    {
        std::cout << "line: " << line << "  " + Where << message << std::endl;
    }

    void lox::error(int line, const std::string &message)
    {
        report(line, "", message);
        hadError = true;
    }

    void lox::error(Token token, const std::string &message)
    {
        if (token.getType() == TokenType::TOKEN_EOF)
        {
            report(token.getLine(), " at end", message);
        }
        else
        {
            report(token.getLine(), " at '" + token.getText() + "'", message);
        }
        hadError = true;
    }
}

