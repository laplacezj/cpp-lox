#pragma once

#include "Expr.h"
#include "Stmt.h"
#include "Interpreter.h"
#include "LoxFunction.h"
#include <memory>
#include <vector>
#include<functional>

class Resolver : public ExprVisitor, public StmtVisitor
{
private:
    Interpreter &interpreter;
    std::vector<std::map<std::string, bool>> scopes;

    enum class FunctionType
    {
        NONE,
        FUNCTION
    };

    FunctionType currentFunction = FunctionType::NONE;

private:
    void resolve(const std::shared_ptr<Stmt> &stmt);
    void resolve(const std::shared_ptr<Expr> &expr);
    void beginScope();
    void endScope();
    void declare(const Token &name);
    void define(const Token &name);
    void resolveLocal(std::shared_ptr<Expr> expr, const Token &name);
    void resolveFunction(
        std::shared_ptr<FunctionStmt> function, FunctionType type);

public:
    Resolver(Interpreter &interpreter);
    void resolve(const std::vector<std::shared_ptr<Stmt>> &statements);

    std::any visitAssignExpr(std::shared_ptr<AssignExpr> expr) override;
    std::any visitBinaryExpr(std::shared_ptr<BinaryExpr> expr) override;
    std::any visitGroupingExpr(std::shared_ptr<GroupingExpr> expr) override;
    std::any visitLiteralExpr(std::shared_ptr<LiteralExpr> expr) override;
    std::any visitUnaryExpr(std::shared_ptr<UnaryExpr> expr) override;
    std::any visitVariableExpr(std::shared_ptr<VariableExpr> expr) override;
    std::any visitLogicalExpr(std::shared_ptr<LogicalExpr> expr) override;
    std::any visitCallExpr(std::shared_ptr<CallExpr> expr) override;

    std::any visitBlockStmt(std::shared_ptr<BlockStmt> stmt) override;
    std::any visitExpressionStmt(std::shared_ptr<ExpressionStmt> stmt) override;
    std::any visitPrintStmt(std::shared_ptr<PrintStmt> stmt) override;
    std::any visitVarStmt(std::shared_ptr<VarStmt> stmt) override;
    std::any visitIfStmt(std::shared_ptr<IfStmt> stmt) override;
    std::any visitWhileStmt(std::shared_ptr<WhileStmt> stmt) override;
    std::any visitFunctionStmt(std::shared_ptr<FunctionStmt> stmt) override;
    std::any visitReturnStmt(std::shared_ptr<ReturnStmt> stmt) override;
};
