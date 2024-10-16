#pragma once

#include <cassert>
#include <vector>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility> // std::move

#include "Token.h"
#include "Expr.h"
#include "Stmt.h"
#include "Error.h"

class Parser
{
private:
    struct ParseError : public std::runtime_error
    {
        using std::runtime_error::runtime_error;
    };

    void synchronize();

    const std::vector<Token> &tokens;
    int current = 0;

public:
    Parser(const std::vector<Token> &tokens);
    ~Parser();

    std::vector<std::shared_ptr<Stmt>> parse();
    std::shared_ptr<Stmt> statement();
    
private:
    std::shared_ptr<Expr> expression();
    std::shared_ptr<Expr> orExpression();
    std::shared_ptr<Expr> andExpression();
    std::shared_ptr<Expr> assignment();
    std::shared_ptr<Expr> equality();
    std::shared_ptr<Expr> comparison();
    std::shared_ptr<Expr> term();
    std::shared_ptr<Expr> factor();
    std::shared_ptr<Expr> unary();
    std::shared_ptr<Expr> call();
    std::shared_ptr<Expr> finishCall(std::shared_ptr<Expr> callee);
    std::shared_ptr<Expr> primary();

    std::shared_ptr<Stmt> returnStatement();
    std::shared_ptr<Stmt> forStatement();
    std::shared_ptr<Stmt> whileStatement();
    std::vector<std::shared_ptr<Stmt>> block();
    std::shared_ptr<Stmt> printStatement();
    std::shared_ptr<Stmt> ifStatement();
    std::shared_ptr<Stmt> expressionStatement();
    std::shared_ptr<Stmt> declaration();
    std::shared_ptr<Stmt> varDeclaration();
    std::shared_ptr<FunctionStmt> function(std::string kind);
    std::shared_ptr<Stmt> classDeclaration();

    template <class... T>
    bool match(T... type)
    {
        assert((... && std::is_same_v<T, TokenType>));

        if ((... || check(type)))
        {
            advance();
            return true;
        }

        return false;
    }

    Token advance()
    {
        if (!isAtEnd())
            ++current;
        return previous();
    }

    bool isAtEnd()
    {
        return peek().type == END_OF_FILE;
    }

    Token peek()
    {
        return tokens.at(current);
    }

    Token previous()
    {
        return tokens.at(current - 1);
    }

    Token consume(TokenType type, std::string_view message)
    {
        if (check(type))
            return advance();

        throw error(peek(), message);
    }

    bool check(TokenType type)
    {
        if (isAtEnd())
            return false;
        return peek().type == type;
    }

    ParseError error(const Token &token, std::string_view message)
    {
        ::error(token, message);
        return ParseError{""};
    }
};
