#pragma once

#include <any>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include "Error.h"
#include "Expr.h"
#include "RuntimeError.h"
#include "Stmt.h"
#include "Environment.h"

class Interpreter : public ExprVisitor, public StmtVisitor
{
private:
    // data
    std::shared_ptr<Environment> environment{new Environment};

    std::any evaluate(std::shared_ptr<Expr> expr);
    void checkNumberOperand(const Token &op, const std::any &operand);
    void checkNumberOperands(const Token &op, const std::any &left, const std::any &right);
    bool isTruthy(const std::any &object);
    bool isEqual(const std::any &a, const std::any &b);
    std::string stringify(const std::any &object);

    void execute(std::shared_ptr<Stmt> statement);
    void executeBlock(
        const std::vector<std::shared_ptr<Stmt>> &statements,
        std::shared_ptr<Environment> environment);

public:
    std::any visitAssignExpr(std::shared_ptr<AssignExpr> expr) override;
    std::any visitBinaryExpr(std::shared_ptr<BinaryExpr> expr) override;
    std::any visitGroupingExpr(std::shared_ptr<GroupingExpr> expr) override;
    std::any visitLiteralExpr(std::shared_ptr<LiteralExpr> expr) override;
    std::any visitUnaryExpr(std::shared_ptr<UnaryExpr> expr) override;
    std::any visitVariableExpr(std::shared_ptr<VariableExpr> expr) override;

    std::any visitBlockStmt(std::shared_ptr<BlockStmt> stmt) override;
    std::any visitExpressionStmt(std::shared_ptr<ExpressionStmt> stmt) override;
    std::any visitPrintStmt(std::shared_ptr<PrintStmt> stmt) override;
    std::any visitVarStmt(std::shared_ptr<VarStmt> stmt) override;

    void interpret(std::vector<std::shared_ptr<Stmt>> statements);
};
