#pragma once
#include <memory>
#include <string>
#include <vector>

#include "ErrorHandler.hpp"
#include "Token.hpp"
#include "TokenType.hpp"

namespace lox {

class Scanner {
public:
  Scanner(const std::string &source, ErrorHandler &errorHandler)
      : source(source), start(0), current(0), line(0),
        errorHandler(errorHandler) {}

  auto scanTokens() -> const std::vector<Token> &;

private:
  auto isAtEnd() -> bool;
  auto advance() -> char;
  void addToken(const TokenType type);
  void addToken(const TokenType type, const Object &literal);
  void scanToken();
  auto match(char expected) -> bool;
  // Look ahead
  auto peek() -> char;
  auto peekNext() -> char;

  static auto isDigit(char c) -> bool;
  static auto isAlpha(char c) -> bool;
  static auto isAlphaNumeric(char c) -> bool;

  // Handle a literal string
  void string();
  // Handle a number literal
  void number();
  // Handle identifiers
  void identifier();

  const std::string &source;
  std::vector<Token> tokens;
  int start, current, line;
  ErrorHandler &errorHandler;
};

} // namespace lox