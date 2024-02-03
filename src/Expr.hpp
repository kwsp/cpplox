#pragma once

#include <memory>

#include "Token.hpp"

namespace lox::ast {

// Forward declarations
struct Binary;
struct Grouping;
struct Literal;
struct Unary;

// Baseclass
struct Expr {

  struct Visitor {
    virtual void visitBinaryExpr(Binary &) = 0;
    virtual void visitGroupingExpr(Grouping &) = 0;
    virtual void visitLiteralExpr(Literal &) = 0;
    virtual void visitUnaryExpr(Unary &) = 0;
  };
  virtual ~Expr() = default;
  virtual void accept(Visitor &visitor) = 0;
};

struct Binary : public Expr {
  Binary(std::unique_ptr<Expr> left, std::unique_ptr<Token> op,
         std::unique_ptr<Expr> right)
      : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

  void accept(Visitor &visitor) override { visitor.visitBinaryExpr(*this); }

  std::unique_ptr<Expr> left;
  std::unique_ptr<Token> op;
  std::unique_ptr<Expr> right;
};

struct Grouping : public Expr {
  Grouping(std::unique_ptr<Expr> expression)
      : expression(std::move(expression)) {}

  void accept(Visitor &visitor) override { visitor.visitGroupingExpr(*this); }

  std::unique_ptr<Expr> expression;
};

struct Literal : public Expr {
  Literal(std::unique_ptr<Object> value) : value(std::move(value)) {}

  void accept(Visitor &visitor) override { visitor.visitLiteralExpr(*this); }

  std::unique_ptr<Object> value;
};

struct Unary : public Expr {
  Unary(std::unique_ptr<Token> op, std::unique_ptr<Expr> right)
      : op(std::move(op)), right(std::move(right)) {}

  void accept(Visitor &visitor) override { visitor.visitUnaryExpr(*this); }

  std::unique_ptr<Token> op;
  std::unique_ptr<Expr> right;
};

} // namespace lox::ast
