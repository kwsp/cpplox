#pragma once
#include <iostream>
#include <string>
#include <string_view>

namespace lox {
class ErrorHandler {
public:
  ErrorHandler() : m_hadError(false) {}

  void report(int line, std::string_view where, std::string_view message) {
    std::cerr << "[line " << line << "] Error" << where << ": " << message
              << "\n";
  }

  void error(int line, const std::string &message) {
    report(line, "", message);
  }
  bool hadError() const { return m_hadError; }
  void resetError() { m_hadError = false; }

private:
  bool m_hadError;
};

} // namespace lox