#pragma once

#include <any>
#include <memory>
#include <utility> // std::move
#include <vector>
#include "Token.h"

struct AssignExpr;
struct LogicalExpr;
struct BinaryExpr;
struct GroupingExpr;
struct LiteralExpr;
struct UnaryExpr;
struct CallExpr;
struct VariableExpr;
struct GetExpr;
struct SetExpr;
struct ThisExpr;

struct ExprVisitor
{
  virtual std::any visitAssignExpr(std::shared_ptr<AssignExpr> expr) = 0;
  
  virtual std::any visitBinaryExpr(std::shared_ptr<BinaryExpr> expr) = 0;
  virtual std::any visitCallExpr(std::shared_ptr<CallExpr> expr) = 0;
  virtual std::any visitGetExpr(std::shared_ptr<GetExpr> expr) = 0;
  virtual std::any visitUnaryExpr(std::shared_ptr<UnaryExpr> expr) = 0;
  virtual std::any visitGroupingExpr(std::shared_ptr<GroupingExpr> expr) = 0;
  virtual std::any visitLiteralExpr(std::shared_ptr<LiteralExpr> expr) = 0;
  virtual std::any visitLogicalExpr(std::shared_ptr<LogicalExpr> expr) = 0;
  virtual std::any visitSetExpr(std::shared_ptr<SetExpr> expr) = 0;
  virtual std::any visitThisExpr(std::shared_ptr<ThisExpr> expr) = 0;
  virtual std::any visitVariableExpr(std::shared_ptr<VariableExpr> expr) = 0;
  virtual ~ExprVisitor() = default;
};

struct Expr
{
  virtual std::any accept(ExprVisitor &visitor) = 0;
};

struct BinaryExpr : Expr, public std::enable_shared_from_this<BinaryExpr>
{
  BinaryExpr(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
      : left{std::move(left)},
        op{std::move(op)},
        right{std::move(right)}
  {
  }

  std::any accept(ExprVisitor &visitor)
  {
    return visitor.visitBinaryExpr(shared_from_this());
  }

  const std::shared_ptr<Expr> left;
  const Token op;
  const std::shared_ptr<Expr> right;
};

struct GroupingExpr : Expr, public std::enable_shared_from_this<GroupingExpr>
{
  GroupingExpr(std::shared_ptr<Expr> expression)
      : expression{std::move(expression)}
  {
  }

  std::any accept(ExprVisitor &visitor) override
  {
    return visitor.visitGroupingExpr(shared_from_this());
  }

  const std::shared_ptr<Expr> expression;
};

struct LiteralExpr : Expr, public std::enable_shared_from_this<LiteralExpr>
{
  LiteralExpr(std::any value)
      : value{std::move(value)}
  {
  }

  std::any accept(ExprVisitor &visitor) override
  {
    return visitor.visitLiteralExpr(shared_from_this());
  }

  const std::any value;
};

struct UnaryExpr : Expr, public std::enable_shared_from_this<UnaryExpr>
{
  UnaryExpr(Token op, std::shared_ptr<Expr> right)
      : op{std::move(op)}, right{std::move(right)}
  {
  }

  std::any accept(ExprVisitor &visitor) override
  {
    return visitor.visitUnaryExpr(shared_from_this());
  }

  const Token op;
  const std::shared_ptr<Expr> right;
};

struct VariableExpr : Expr, public std::enable_shared_from_this<VariableExpr>
{
  VariableExpr(Token name)
      : name{std::move(name)}
  {
  }

  std::any accept(ExprVisitor &visitor) override
  {
    return visitor.visitVariableExpr(shared_from_this());
  }

  const Token name;
};


struct LogicalExpr : Expr, public std::enable_shared_from_this<LogicalExpr>
{
  LogicalExpr(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
      : left{std::move(left)}, op{std::move(op)}, right{std::move(right)}
  {
  }

  std::any accept(ExprVisitor &visitor) override
  {
    return visitor.visitLogicalExpr(shared_from_this());
  }

  const std::shared_ptr<Expr> left;
  const Token op;
  const std::shared_ptr<Expr> right;
};


struct AssignExpr : Expr, public std::enable_shared_from_this<AssignExpr>
{
  AssignExpr(Token name, std::shared_ptr<Expr> value)
      : name{std::move(name)}, value{std::move(value)}
  {
  }

  std::any accept(ExprVisitor &visitor) override
  {
    return visitor.visitAssignExpr(shared_from_this());
  }

  const Token name;
  const std::shared_ptr<Expr> value;
};


struct CallExpr: Expr, public std::enable_shared_from_this<CallExpr> {
  CallExpr(std::shared_ptr<Expr> callee, Token paren, std::vector<std::shared_ptr<Expr>> arguments)
    : callee{std::move(callee)}, paren{std::move(paren)}, arguments{std::move(arguments)}
  {}

  std::any accept(ExprVisitor& visitor) override {
    return visitor.visitCallExpr(shared_from_this());
  }

  const std::shared_ptr<Expr> callee;
  const Token paren;
  const std::vector<std::shared_ptr<Expr>> arguments;
};

struct GetExpr: Expr, public std::enable_shared_from_this<GetExpr> {
  GetExpr(std::shared_ptr<Expr> object, Token name)
    : object{std::move(object)}, name{std::move(name)}
  {}

  std::any accept(ExprVisitor& visitor) override {
    return visitor.visitGetExpr(shared_from_this());
  }

  const std::shared_ptr<Expr> object;
  const Token name;
};



struct SetExpr: Expr, public std::enable_shared_from_this<SetExpr> {
  SetExpr(std::shared_ptr<Expr> object, Token name, std::shared_ptr<Expr> value)
    : object{std::move(object)}, name{std::move(name)}, value{std::move(value)}
  {}

  std::any accept(ExprVisitor& visitor) override {
    return visitor.visitSetExpr(shared_from_this());
  }

  const std::shared_ptr<Expr> object;
  const Token name;
  const std::shared_ptr<Expr> value;
};

struct ThisExpr: Expr, public std::enable_shared_from_this<ThisExpr> {
  ThisExpr(Token keyword)
    : keyword{std::move(keyword)}
  {}

  std::any accept(ExprVisitor& visitor) override {
    return visitor.visitThisExpr(shared_from_this());
  }

  const Token keyword;
};
