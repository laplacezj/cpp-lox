#pragma once
#include"Expr.hpp"
#include"Token.hpp"
#include<memory>

namespace Lox
{
    class UnaryExpr : public Expr
    {
    
    public:
        UnaryExpr(Token oper, std::unique_ptr<Expr> right);
    private:
        Token oper;
        std::unique_ptr<Expr> right;

    };
    
} // namespace Lox

