#include"lox/UnaryExpr.hpp"
#include<cassert>

namespace Lox
{

    UnaryExpr::UnaryExpr(Token oper, std::unique_ptr<Expr> right):
    oper(std::move(oper)),
    right(std::move(right))
    {
        assert( right != nullptr);
    }

    
} // namespace Lox


