#pragma once

#include <any>
#include <memory>
#include <utility>
#include <vector>
#include "Token.h"
#include "Expr.h"

struct BlockStmt;
struct ClassStmt;
struct ExpressionStmt;
struct IfStmt;
struct PrintStmt;
struct VarStmt;
struct FunctionStmt;
struct WhileStmt;
struct ReturnStmt;

struct StmtVisitor
{
  virtual std::any visitFunctionStmt(std::shared_ptr<FunctionStmt> stmt) = 0;
  virtual std::any visitClassStmt(std::shared_ptr<ClassStmt> stmt) = 0;
  virtual std::any visitBlockStmt(std::shared_ptr<BlockStmt> stmt) = 0;
  virtual std::any visitExpressionStmt(std::shared_ptr<ExpressionStmt> stmt) = 0;
  virtual std::any visitIfStmt(std::shared_ptr<IfStmt> stmt) = 0;
  virtual std::any visitPrintStmt(std::shared_ptr<PrintStmt> stmt) = 0;
  virtual std::any visitVarStmt(std::shared_ptr<VarStmt> stmt) = 0;
  virtual std::any visitWhileStmt(std::shared_ptr<WhileStmt> stmt) = 0;
  virtual std::any visitReturnStmt(std::shared_ptr<ReturnStmt> stmt) = 0;

  virtual ~StmtVisitor() = default;
};

struct Stmt
{
  virtual std::any accept(StmtVisitor &visitor) = 0;
};

struct BlockStmt : Stmt, public std::enable_shared_from_this<BlockStmt>
{
  BlockStmt(std::vector<std::shared_ptr<Stmt>> statements)
      : statements{std::move(statements)}
  {
  }

  std::any accept(StmtVisitor &visitor) override
  {
    return visitor.visitBlockStmt(shared_from_this());
  }

  const std::vector<std::shared_ptr<Stmt>> statements;
};

struct ExpressionStmt : Stmt, public std::enable_shared_from_this<ExpressionStmt>
{
  ExpressionStmt(std::shared_ptr<Expr> expression)
      : expression{std::move(expression)}
  {
  }

  std::any accept(StmtVisitor &visitor) override
  {
    return visitor.visitExpressionStmt(shared_from_this());
  }

  const std::shared_ptr<Expr> expression;
};

struct PrintStmt : Stmt, public std::enable_shared_from_this<PrintStmt>
{
  PrintStmt(std::shared_ptr<Expr> expression)
      : expression{std::move(expression)}
  {
  }

  std::any accept(StmtVisitor &visitor) override
  {
    return visitor.visitPrintStmt(shared_from_this());
  }

  const std::shared_ptr<Expr> expression;
};

struct VarStmt : Stmt, public std::enable_shared_from_this<VarStmt>
{
  VarStmt(Token name, std::shared_ptr<Expr> initializer)
      : name{std::move(name)}, initializer{std::move(initializer)}
  {
  }

  std::any accept(StmtVisitor &visitor) override
  {
    return visitor.visitVarStmt(shared_from_this());
  }

  const Token name;
  const std::shared_ptr<Expr> initializer;
};

struct IfStmt : Stmt, public std::enable_shared_from_this<IfStmt>
{
  IfStmt(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> thenBranch, std::shared_ptr<Stmt> elseBranch)
      : condition{std::move(condition)}, thenBranch{std::move(thenBranch)}, elseBranch{std::move(elseBranch)}
  {
  }

  std::any accept(StmtVisitor &visitor) override
  {
    return visitor.visitIfStmt(shared_from_this());
  }

  const std::shared_ptr<Expr> condition;
  const std::shared_ptr<Stmt> thenBranch;
  const std::shared_ptr<Stmt> elseBranch;
};

struct WhileStmt : Stmt, public std::enable_shared_from_this<WhileStmt>
{
  WhileStmt(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body)
      : condition{std::move(condition)}, body{std::move(body)}
  {
  }

  std::any accept(StmtVisitor &visitor) override
  {
    return visitor.visitWhileStmt(shared_from_this());
  }

  const std::shared_ptr<Expr> condition;
  const std::shared_ptr<Stmt> body;
};

struct FunctionStmt : Stmt, public std::enable_shared_from_this<FunctionStmt>
{
  FunctionStmt(Token name, std::vector<Token> params, std::vector<std::shared_ptr<Stmt>> body)
      : name{std::move(name)}, params{std::move(params)}, body{std::move(body)}
  {
  }

  std::any accept(StmtVisitor &visitor) override
  {
    return visitor.visitFunctionStmt(shared_from_this());
  }

  const Token name;
  const std::vector<Token> params;
  const std::vector<std::shared_ptr<Stmt>> body;
};

struct ReturnStmt : Stmt, public std::enable_shared_from_this<ReturnStmt>
{
  ReturnStmt(Token keyword, std::shared_ptr<Expr> value)
      : keyword{std::move(keyword)}, value{std::move(value)}
  {
  }

  std::any accept(StmtVisitor &visitor) override
  {
    return visitor.visitReturnStmt(shared_from_this());
  }

  const Token keyword;
  const std::shared_ptr<Expr> value;
};


struct ClassStmt : Stmt, public std::enable_shared_from_this<ClassStmt>
{
  ClassStmt(Token name, std::vector<std::shared_ptr<FunctionStmt>> methods)
      : name{std::move(name)}, methods{std::move(methods)}
  {
  }

  std::any accept(StmtVisitor &visitor) override
  {
    return visitor.visitClassStmt(shared_from_this());
  }

  const Token name;
  const std::vector<std::shared_ptr<FunctionStmt>> methods;
};
