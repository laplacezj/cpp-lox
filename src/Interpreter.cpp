#include "Interpreter.h"
#include "RuntimeError.h"

std::any Interpreter::visitBinaryExpr(std::shared_ptr<BinaryExpr> expr)
{
    std::any left = evaluate(expr->left);
    std::any right = evaluate(expr->right);

    switch (expr->op.type)
    {
    case MINUS:
        checkNumberOperands(expr->op, left, right);
        return std::any_cast<double>(left) - std::any_cast<double>(right);
        break;
    case SLASH:
        checkNumberOperands(expr->op, left, right);
        return std::any_cast<double>(left) / std::any_cast<double>(right);
        break;
    case STAR:
        checkNumberOperands(expr->op, left, right);
        return std::any_cast<double>(left) * std::any_cast<double>(right);
        break;

    case PLUS:
        if (left.type() == typeid(double) &&
            right.type() == typeid(double))
        {
            return std::any_cast<double>(left) +
                   std::any_cast<double>(right);
        }

        if (left.type() == typeid(std::string) &&
            right.type() == typeid(std::string))
        {
            return std::any_cast<std::string>(left) +
                   std::any_cast<std::string>(right);
        }
        throw RuntimeError{expr->op,
                           "Operands must be two numbers or two strings."};

    case GREATER:
        checkNumberOperands(expr->op, left, right);
        return std::any_cast<double>(left) >
               std::any_cast<double>(right);
    case GREATER_EQUAL:
        checkNumberOperands(expr->op, left, right);
        return std::any_cast<double>(left) >=
               std::any_cast<double>(right);
    case LESS:
        checkNumberOperands(expr->op, left, right);
        return std::any_cast<double>(left) <
               std::any_cast<double>(right);
    case LESS_EQUAL:
        checkNumberOperands(expr->op, left, right);
        return std::any_cast<double>(left) <=
               std::any_cast<double>(right);

    case BANG_EQUAL:
        return !isEqual(left, right);
    case EQUAL_EQUAL:
        return isEqual(left, right);

    default:
        break;
    }

    return nullptr;
}

std::any Interpreter::visitGroupingExpr(std::shared_ptr<GroupingExpr> expr)
{
    return evaluate(expr->expression);
}
std::any Interpreter::visitLiteralExpr(std::shared_ptr<LiteralExpr> expr)
{
    return expr->value;
}
std::any Interpreter::visitUnaryExpr(std::shared_ptr<UnaryExpr> expr)
{
    std::any right = evaluate(expr->right);
    switch (expr->op.type)
    {
    case MINUS:
        checkNumberOperand(expr->op, right);
        return -std::any_cast<double>(right);
        break;

    case BANG:
        return !isTruthy(right);
        break;
    default:
        break;
    }
}

std::any Interpreter::evaluate(std::shared_ptr<Expr> expr)
{
    return expr->accept(*this);
}

void Interpreter::checkNumberOperand(const Token &op,
                                     const std::any &operand)
{
    if (operand.type() == typeid(double))
        return;
    throw RuntimeError{op, "Operand must be a number."};
}

void Interpreter::checkNumberOperands(const Token &op, const std::any &left, const std::any &right)
{
    if (left.type() == typeid(double) &&
        right.type() == typeid(double))
    {
        return;
    }

    throw RuntimeError{op, "Operands must be numbers."};
}

bool Interpreter::isTruthy(const std::any &object)
{
    if (object.type() == typeid(nullptr))
        return false;
    if (object.type() == typeid(bool))
    {
        return std::any_cast<bool>(object);
    }
    return true;
}

bool Interpreter::isEqual(const std::any &a, const std::any &b)
{
    if (a.type() == typeid(nullptr) && b.type() == typeid(nullptr))
    {
        return true;
    }
    if (a.type() == typeid(nullptr))
        return false;

    if (a.type() == typeid(std::string) &&
        b.type() == typeid(std::string))
    {
        return std::any_cast<std::string>(a) ==
               std::any_cast<std::string>(b);
    }
    if (a.type() == typeid(double) && b.type() == typeid(double))
    {
        return std::any_cast<double>(a) == std::any_cast<double>(b);
    }
    if (a.type() == typeid(bool) && b.type() == typeid(bool))
    {
        return std::any_cast<bool>(a) == std::any_cast<bool>(b);
    }

    return false;
}