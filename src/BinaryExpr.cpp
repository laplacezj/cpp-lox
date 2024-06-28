#include"lox/BinaryExpr.hpp"

namespace Lox
{
    BinaryExpr::BinaryExpr(std::unique_ptr<Expr> left, Token oper, std::unique_ptr<Expr> right):
    left(std::move(left)),
    oper(oper),
    right(std::move(right))
    {
        assert(this->left != nullptr);
        assert(this->right != nullptr);
    }

    BinaryExpr::~BinaryExpr()
    {
    }
    
} // namespace Lox
