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

    std::shared_ptr<Expr> parse();

private:
    std::shared_ptr<Expr> expression();

    std::shared_ptr<Expr> equality();
    std::shared_ptr<Expr> comparison();
    std::shared_ptr<Expr> term();
    std::shared_ptr<Expr> factor();
    std::shared_ptr<Expr> unary();
    std::shared_ptr<Expr> primary();

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
