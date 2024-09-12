#include <cstring>  // std::strerror
#include <fstream>  // readFile
#include <iostream> // std::getline
#include <string>
#include <vector>
#include "Scanner.h"
#include "Error.h"
#include "Parser.h"
#include "AstPrinter.h"
#include "Interpreter.h"

static Interpreter interpreter{};

void run(std::string_view source)
{
    Scanner scanner = {source};
    std::vector<Token> tokens = scanner.scanTokens();

    // for (const Token &token : tokens)
    // {
    //     std::cout << token.toString() << "\n";
    // }

    Parser parser{tokens};

    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    
    if (hadError)
    {
        return;
    }

    //std::cout << AstPrinter{}.print(expression) << "\n";

    interpreter.interpret(statements);
}

void runFile(std::string_view path)
{
    // 打开文件并检查是否成功
    std::ifstream file{std::string(path), std::ios::in | std::ios::binary};

    if (!file)
    {
        std::cerr << "Failed to open file " << path << ": "
                  << std::strerror(errno) << "\n";
        std::exit(74);
    }

    // 读取文件内容到一个 std::string 对象中
    std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    run(contents);

    if (hadError)
    {
        std::exit(64);
    }
    if (hadRuntimeError) std::exit(70);
}

void runPrompt()
{
    for (;;)
    {
        std::cout << ">";
        std::string line;
        if (!std::getline(std::cin, line))
            break;

        run(line);
        hadError = false;
    }
}

int main(int argc, char *argv[])
{
    if (argc > 2)
    {
        std::cout << "Usage: cpp-lox [script]" << std::endl;
        std::exit(64);
    }
    else if (argc == 2)
    {
        runFile(argv[1]);
    }
    else
    {
        runPrompt();
    }
}