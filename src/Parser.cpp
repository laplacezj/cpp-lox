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
    return assignment();
}

std::shared_ptr<Expr> Parser::assignment()
{
    std::shared_ptr<Expr> expr = orExpression();

    if (match(EQUAL))
    {
        Token equals = previous();
        std::shared_ptr<Expr> value = assignment();

        if (VariableExpr *e = dynamic_cast<VariableExpr *>(expr.get()))
        {
            Token name = e->name;
            return std::make_shared<AssignExpr>(std::move(name), value);
        }
        else if (GetExpr *get = dynamic_cast<GetExpr *>(expr.get()))
        {
            return std::make_shared<SetExpr>(get->object, get->name, value);
        }

        error(std::move(equals), "Invalid assignment target.");
    }

    return expr;
}

std::shared_ptr<Expr> Parser::orExpression()
{
    std::shared_ptr<Expr> expr = andExpression();

    while (match(OR))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = andExpression();
        expr = std::make_shared<LogicalExpr>(expr, std::move(op), right);
    }

    return expr;
}

std::shared_ptr<Expr> Parser::andExpression()
{
    std::shared_ptr<Expr> expr = equality();

    while (match(AND))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = equality();
        expr = std::make_shared<LogicalExpr>(expr, std::move(op), right);
    }

    return expr;
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

    return call();
}

std::shared_ptr<Expr> Parser::finishCall(std::shared_ptr<Expr> callee)
{
    std::vector<std::shared_ptr<Expr>> arguments;

    if (!check(RIGHT_PAREN))
    {
        do
        {
            if (arguments.size() >= 255)
            {
                error(peek(), "Can't have more than 255 arguments.");
            }
            arguments.push_back(expression());
        } while (match(COMMA));
    }

    Token paren = consume(RIGHT_PAREN,
                          "Expect ')' after arguments.");

    return std::make_shared<CallExpr>(callee,
                                      std::move(paren),
                                      std::move(arguments));
}

std::shared_ptr<Expr> Parser::call()
{
    std::shared_ptr<Expr> expr = primary();

    while (true)
    {
        if (match(LEFT_PAREN))
        {
            expr = finishCall(expr);
        }
        else if (match(DOT))
        {
            Token name = consume(IDENTIFIER, "Expect property name after '.'.");
            expr = std::make_shared<GetExpr>(expr, std::move(name));
        }
        else
        {
            break;
        }
    }

    return expr;
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

    if (match(THIS)) return std::make_shared<ThisExpr>(previous());

    if (match(IDENTIFIER))
    {
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
    if (match(IF))
        return ifStatement();

    if (match(PRINT))
        return printStatement();

    if (match(LEFT_BRACE))
        return std::make_shared<BlockStmt>(block());

    if (match(WHILE))
        return whileStatement();

    if (match(FOR))
    {
        return forStatement();
    }

    if (match(RETURN))
        return returnStatement();

    return expressionStatement();
}

std::shared_ptr<Stmt> Parser::returnStatement()
{
    Token keyword = previous();
    std::shared_ptr<Expr> value = nullptr;
    if (!check(SEMICOLON))
    {
        value = expression();
    }

    consume(SEMICOLON, "Expect ';' after return value.");
    return std::make_shared<ReturnStmt>(keyword, value);
}

std::shared_ptr<Stmt> Parser::forStatement()
{
    consume(LEFT_PAREN, "Expect '(' after 'for'.");

    std::shared_ptr<Stmt> initializer;

    if (match(SEMICOLON))
    {
        initializer = nullptr;
    }
    else if (match(VAR))
    {
        initializer = varDeclaration();
    }
    else
    {
        initializer = expressionStatement();
    }

    std::shared_ptr<Expr> condition = nullptr;
    if (!check(SEMICOLON))
    {
        condition = expression();
    }
    consume(SEMICOLON, "Expect ';' after loop condition.");

    std::shared_ptr<Expr> increment = nullptr;
    if (!check(RIGHT_PAREN))
    {
        increment = expression();
    }
    consume(RIGHT_PAREN, "Expect ')' after for clauses.");
    std::shared_ptr<Stmt> body = statement();

    if (increment != nullptr)
    {
        body = std::make_shared<BlockStmt>(
            std::vector<std::shared_ptr<Stmt>>{
                body,
                std::make_shared<ExpressionStmt>(increment)});
    }

    if (condition == nullptr)
    {
        condition = std::make_shared<LiteralExpr>(true);
    }
    body = std::make_shared<WhileStmt>(condition, body);

    if (initializer != nullptr)
    {
        body = std::make_shared<BlockStmt>(
            std::vector<std::shared_ptr<Stmt>>{initializer, body});
    }

    return body;
}

std::shared_ptr<Stmt> Parser::whileStatement()
{
    consume(LEFT_PAREN, "Expect '(' after 'while'.");
    std::shared_ptr<Expr> condition = expression();
    consume(RIGHT_PAREN, "Expect ')' after condition.");
    std::shared_ptr<Stmt> body = statement();

    return std::make_shared<WhileStmt>(condition, body);
}

std::shared_ptr<Stmt> Parser::ifStatement()
{
    consume(LEFT_PAREN, "Expect '(' after 'if'.");
    std::shared_ptr<Expr> condition = expression();
    consume(RIGHT_PAREN, "Expect ')' after if condition.");

    std::shared_ptr<Stmt> thenBranch = statement();
    std::shared_ptr<Stmt> elseBranch = nullptr;

    if (match(ELSE))
    {
        elseBranch = statement();
    }

    return std::make_shared<IfStmt>(condition, thenBranch, elseBranch);
}

std::shared_ptr<Stmt> Parser::printStatement()
{
    std::shared_ptr<Expr> value = expression();
    consume(SEMICOLON, "Expect ';' after value.");
    return std::make_shared<PrintStmt>(value);
}

std::vector<std::shared_ptr<Stmt>> Parser::block()
{
    std::vector<std::shared_ptr<Stmt>> statements;

    while (!check(RIGHT_BRACE) && !isAtEnd())
    {
        statements.push_back(declaration());
    }

    consume(RIGHT_BRACE, "Expect '}' after block.");
    return statements;
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
        if (match(FUN))
            return function("function");

        if (match(VAR))
            return varDeclaration();

        if (match(CLASS))
            return classDeclaration();

        return statement();
    }
    catch (ParseError error)
    {
        synchronize();
        return nullptr;
    }
}

std::shared_ptr<Stmt> Parser::classDeclaration()
{
    Token name = consume(IDENTIFIER, "Expect class name.");
    consume(LEFT_BRACE, "Expect '{' before class body.");

    std::vector<std::shared_ptr<FunctionStmt>> methods;
    while (!check(RIGHT_BRACE) && !isAtEnd())
    {
        methods.push_back(function("method"));
    }

    consume(RIGHT_BRACE, "Expect '}' after class body.");
    return std::make_shared<ClassStmt>(std::move(name), std::move(methods));
}

std::shared_ptr<FunctionStmt> Parser::function(std::string kind)
{
    Token name = consume(IDENTIFIER, "Expect " + kind + " name.");
    consume(LEFT_PAREN, "Expect '(' after " + kind + " name.");
    std::vector<Token> parameters;
    if (!check(RIGHT_PAREN))
    {
        do
        {
            if (parameters.size() >= 255)
            {
                error(peek(), "Can't have more than 255 parameters.");
            }

            parameters.push_back(
                consume(IDENTIFIER, "Expect parameter name."));
        } while (match(COMMA));
    }
    consume(RIGHT_PAREN, "Expect ')' after parameters.");

    consume(LEFT_BRACE, "Expect '{' before " + kind + " body.");
    std::vector<std::shared_ptr<Stmt>> body = block();
    return std::make_shared<FunctionStmt>(std::move(name),
                                          std::move(parameters),
                                          std::move(body));
}

std::shared_ptr<Stmt> Parser::varDeclaration()
{
    Token name = consume(IDENTIFIER, "Expect variable name.");

    std::shared_ptr<Expr> initializer = nullptr;
    if (match(EQUAL))
    {
        initializer = expression();
    }

    consume(SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_shared<VarStmt>(std::move(name), initializer);
}