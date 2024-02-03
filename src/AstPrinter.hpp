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

  R parenthesize(const std::string &name,
                 std::convertible_to<Expr *> auto &&...exprs) {
    std::stringstream ss;
    ss << "(" << name;

    for (auto expr : std::initializer_list<Expr *>{exprs...}) {
      expr->accept(*this);
      ss << " " << acceptRet;
    }

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