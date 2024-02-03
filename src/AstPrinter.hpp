#pragma once

#include <concepts>
#include <sstream>
#include <string>

#include "Expr.hpp"

namespace lox::ast {

struct AstPrinter : public Expr::Visitor {
  using R = std::string;

  R currRet;
  R acceptRet;

  template <typename T>
  R print(T *expr)
    requires(std::is_base_of_v<Expr, T>)
  {
    expr->accept(*this);
    return acceptRet;
  }

  template <typename T>
  void parenthesize_inner(std::stringstream &ss, T *val)
    requires(std::is_base_of_v<Expr, T>)
  {
    val->accept(*this);
    ss << acceptRet;
  }

  template <typename T, typename... Targs>
  void parenthesize_inner(std::stringstream &ss, T *val, Targs &&...rest)
    requires(std::is_base_of_v<Expr, T>)
  {
    val->accept(*this);
    ss << acceptRet << " ";
    parenthesize_inner(ss, rest...);
  }

  template <typename T, typename... Targs>
  R parenthesize(const std::string &name, T *expr, Targs &&...exprs)
    requires(std::is_base_of_v<Expr, T>)
  {
    std::stringstream ss;
    ss << "(" << name << " ";
    expr->accept(*this);
    ss << acceptRet << " ";
    parenthesize_inner(ss, exprs...);
    ss << ")";
    return ss.str();
  }

  template <typename T>
  R parenthesize(const std::string &name, T *exprs)
    requires(std::is_base_of_v<Expr, T>)
  {
    std::stringstream ss;
    ss << "(" << name << " ";
    parenthesize_inner(ss, exprs);
    ss << ")";
    return ss.str();
  }

  inline void visitBinaryExpr(Binary &expr) override {
    const auto &_expr = static_cast<Binary &>(expr);
    acceptRet =
        parenthesize(_expr.op->lexeme, _expr.left.get(), _expr.right.get());
  }

  inline void visitGroupingExpr(Grouping &expr) override {
    const auto &_expr = static_cast<Grouping &>(expr);
    using namespace std::string_literals;
    acceptRet = parenthesize("group"s, _expr.expression.get());
  }

  inline void visitLiteralExpr(Literal &expr) override {
    using namespace std::string_literals;
    const auto &_expr = static_cast<Literal &>(expr);
    acceptRet = (_expr.value->empty()) ? "nil"s : _expr.value->toString();
  }

  inline void visitUnaryExpr(Unary &expr) override {
    const auto &_expr = static_cast<Unary &>(expr);
    acceptRet = parenthesize(_expr.op->lexeme, _expr.right.get());
  }
};

} // namespace lox::ast