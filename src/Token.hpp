#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "magic_enum/magic_enum.hpp"

#include "Object.hpp"
#include "Token.hpp"
#include "TokenType.hpp"

namespace lox {

struct Token {
  TokenType type;
  std::string lexeme;
  std::shared_ptr<Object> literal;
  int line;

  Token(TokenType type, const std::string &lexeme,
        std::shared_ptr<Object> literal, int line)
      : type(type), lexeme(lexeme), literal(std::move(literal)), line(line) {}

  inline std::string toString() const {
    using namespace std::string_literals;
    std::string s = "Token(type=";
    s += magic_enum::enum_name(type);
    s += ", lexeme='" + lexeme + "'";
    if (literal) {
      s += ", literal=" + literal->toString();
    }
    s += ")";
    return s;
  }
};

} // namespace lox

inline std::ostream &operator<<(std::ostream &os, const lox::Token &token) {
  os << token.toString();
  return os;
}