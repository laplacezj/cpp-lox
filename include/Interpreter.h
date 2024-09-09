#pragma once

#include <stdexcept>

#include "Expr.h"

class Interpreter : public ExprVisitor
{
private:
    std::any evaluate(std::shared_ptr<Expr> expr);
    void checkNumberOperand(const Token &op, const std::any &operand);
    void checkNumberOperands(const Token &op, const std::any &left, const std::any &right);
    bool isTruthy(const std::any& object);
    bool isEqual(const std::any &a, const std::any &b);

public:
    std::any visitBinaryExpr(std::shared_ptr<BinaryExpr> expr) override;
    std::any visitGroupingExpr(std::shared_ptr<GroupingExpr> expr) override;
    std::any visitLiteralExpr(std::shared_ptr<LiteralExpr> expr) override;
    std::any visitUnaryExpr(std::shared_ptr<UnaryExpr> expr) override;
};
