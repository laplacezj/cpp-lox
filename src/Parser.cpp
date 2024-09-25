#include "Parser.h"

Parser::Parser(const std::vector<Token> &tokens)
    : tokens(tokens)
{
}

Parser::~Parser()
{
}

std::shared_ptr<Expr> Parser::expression()
{
    return equality();
}

std::shared_ptr<Expr> Parser::equality()
{
    std::shared_ptr<Expr> expr = comparison();

    while (match(BANG_EQUAL, EQUAL_EQUAL))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = comparison();
        expr = std::make_shared<BinaryExpr>(expr, std::move(op), right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::comparison()
{
    std::shared_ptr<Expr> expr = term();

    while (match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = term();
        expr = std::make_shared<BinaryExpr>(expr, std::move(op), right);
    }

    return expr;
}

std::shared_ptr<Expr> Parser::term()
{
    std::shared_ptr<Expr> expr = factor();

    while (match(MINUS, PLUS))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = factor();
        expr = std::make_shared<BinaryExpr>(expr, std::move(op), right);
    }

    return expr;
}

std::shared_ptr<Expr> Parser::factor()
{
    std::shared_ptr<Expr> expr = unary();

    while (match(SLASH, STAR))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = unary();
        expr = std::make_shared<BinaryExpr>(expr, std::move(op), right);
    }

    return expr;
}

std::shared_ptr<Expr> Parser::unary()
{
    if (match(BANG, MINUS))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = unary();
        return std::make_shared<UnaryExpr>(std::move(op), right);
    }

    return primary();
}

std::shared_ptr<Expr> Parser::primary()
{

    if (match(FALSE))
        return std::make_shared<LiteralExpr>(false);
    if (match(TRUE))
        return std::make_shared<LiteralExpr>(true);
    if (match(NIL))
        return std::make_shared<LiteralExpr>(nullptr);

    if (match(NUMBER, STRING))
    {
        return std::make_shared<LiteralExpr>(previous().literal);
    }

    if (match(LEFT_PAREN))
    {
        std::shared_ptr<Expr> expr = expression();
        consume(RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_shared<GroupingExpr>(expr);
    }

    if (match(IDENTIFIER)) {
      return std::make_shared<VariableExpr>(previous());
    }

    throw error(peek(), "Expect expression.");
}

void Parser::synchronize()
{
    advance();

    while (!isAtEnd())
    {
        if (previous().type == SEMICOLON)
            return;

        switch (peek().type)
        {
        case CLASS:
        case FUN:
        case VAR:
        case FOR:
        case IF:
        case WHILE:
        case PRINT:
        case RETURN:
            return;
        }

        advance();
    }
}

std::vector<std::shared_ptr<Stmt>> Parser::parse()
{
    // try
    // {
    //     return expression();
    // }
    // catch (ParseError error)
    // {
    //     return nullptr;
    // }

    std::vector<std::shared_ptr<Stmt>> statements;

    while (!isAtEnd())
    {
        statements.push_back(declaration());
    }
    return statements;
}

std::shared_ptr<Stmt> Parser::statement()
{
    if (match(PRINT))
        return printStatement();

    return expressionStatement();
}

std::shared_ptr<Stmt> Parser::printStatement()
{
    std::shared_ptr<Expr> value = expression();
    consume(SEMICOLON, "Expect ';' after value.");
    return std::make_shared<PrintStmt>(value);
}

std::shared_ptr<Stmt> Parser::expressionStatement()
{
    std::shared_ptr<Expr> value = expression();
    consume(SEMICOLON, "Expect ';' after value.");
    return std::make_shared<ExpressionStmt>(value);
}

std::shared_ptr<Stmt> Parser::declaration()
{
    try
    {
        if (match(VAR))
            return varDeclaration();

        return statement();
    }
    catch (ParseError error)
    {
        synchronize();
        return nullptr;
    }
}


std::shared_ptr<Stmt> Parser::varDeclaration()
{
    Token name = consume(IDENTIFIER, "Expect variable name.");

    std::shared_ptr<Expr> initializer = nullptr;
    if (match(EQUAL)) {
      initializer = expression();
    }

    consume(SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_shared<VarStmt>(std::move(name), initializer);
}