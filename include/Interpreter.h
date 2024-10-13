#pragma once

#include <any>
#include <map>
#include <iostream>
#include <chrono>
#include <memory>
#include <stdexcept>
#include <string>
#include "Error.h"
#include "Expr.h"
#include "RuntimeError.h"
#include "Stmt.h"
#include "Environment.h"
#include "LoxCallable.h"
#include "LoxFunction.h"
#include "LoxReturn.h"

class NativeClock : public LoxCallable
{
public:
  int arity() override { return 0; }

  std::any call(Interpreter &interpreter,
                std::vector<std::any> arguments) override
  {
    auto ticks = std::chrono::system_clock::now().time_since_epoch();
    return std::chrono::duration<double>{ticks}.count() / 1000.0;
  }

  std::string toString() override { return "<native fn>"; }
};

class Interpreter : public ExprVisitor, public StmtVisitor
{
  friend class LoxFunction;

  // data
public:
  std::shared_ptr<Environment> globals{new Environment};
  std::map<std::shared_ptr<Expr>, int> locals;

private:
  std::shared_ptr<Environment> environment = globals;

private:
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

    std::any lookUpVariable(const Token& name,
                          std::shared_ptr<Expr> expr);

public:
  void resolve(std::shared_ptr<Expr> expr, int depth);
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

  void interpret(std::vector<std::shared_ptr<Stmt>> statements);

  Interpreter();
};
