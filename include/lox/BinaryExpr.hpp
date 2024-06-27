#pragma one
#include <memory>
#include<cassert>

#include "Expr.hpp"
#include "Token.hpp"

namespace Lox
{
    class BinaryExpr : public Expr
    {

    public:
        BinaryExpr(std::unique_ptr<Expr> left, Token oper, std::unique_ptr<Expr> right);
        ~BinaryExpr();

    private:
        /* data */
        std::unique_ptr<Expr> left;          //左表达式
        Token oper;                             //运算符
        std::unique_ptr<Expr> right;            //右表达式

    };

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
}
