#pragma once

#include<any>

#include"Expr.hpp"
#include"Token.hpp"

namespace Lox
{
    class LiteralExpr :public Expr
    {
    
    public:
        LiteralExpr(std::any literal);
        ~LiteralExpr();
    private:
        /* data */
        std::any literal;
    };
    
} // namespace Lox
