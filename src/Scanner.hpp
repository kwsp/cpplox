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

  const std::vector<Token> &scanTokens();

private:
  bool isAtEnd();
  char advance();
  void addToken(TokenType type);
  void addToken(TokenType type, std::shared_ptr<Object> literal);
  void scanToken();
  bool match(char expected);
  // Look ahead
  char peek();
  char peekNext();

  static bool isDigit(char c);
  static bool isAlpha(char c);
  static bool isAlphaNumeric(char c);

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