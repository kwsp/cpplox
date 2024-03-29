#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <sys/wait.h>
#include <vector>

#include "ErrorHandler.hpp"
#include "Object.hpp"
#include "Scanner.hpp"
#include "Token.hpp"

#include "AstPrinter.hpp"
#include "Expr.hpp"
#include "TokenType.hpp"
#include <memory>

namespace lox {

// Runtime
class Runtime {
public:
  Runtime() {}

  void run(const std::string &src) {
    Scanner scanner(src, errorHandler);
    const auto &tokens = scanner.scanTokens();
    for (const auto &token : tokens) {
      std::cout << token << "\n";
    }
  }

  int runFile(const std::string &path) {
    std::ifstream ifile(path);

    //   ifile.seekg(0, std::ios::end);
    //   size_t length = ifile.tellg();
    //   ifile.seekg(0, std::ios::beg);
    //   std::vector<char> buffer;
    //   buffer.reserve(length);
    //   ifile.read(buffer.data(), length);

    std::string line;
    while (std::getline(ifile, line)) {
      std::cout << line << "\n";
    }

    if (errorHandler.hadError()) {
      return 65;
    }

    return 0;
  }

  int runPrompt() {
    std::string line;
    while (true) {
      std::cout << "> ";
      if (!std::getline(std::cin, line)) {
        break;
      }
      run(line);
      errorHandler.resetError();
    }
    return 0;
  }

private:
  ErrorHandler errorHandler;
};

} // namespace lox

int main(int argc, char **argv) {
  // lox::Runtime runtime;
  // if (argc > 2) {
  //   std::cout << "Usage: cpplox [script]\n";
  //   return 64;
  // } else if (argc == 2) {
  //   return runtime.runFile(argv[1]);
  // } else {
  //   return runtime.runPrompt();
  // }

  // Test Ast
  using namespace lox::ast;
  auto expression = std::make_unique<Binary>(
      std::make_unique<Unary>(
          std::make_unique<lox::Token>(lox::TokenType::MINUS, "-",
                                       lox::Object{}, 1),
          std::make_unique<Literal>(std::make_unique<lox::Object>(123.0))),
      std::make_unique<lox::Token>(lox::TokenType::STAR, "*", lox::Object{}, 1),
      std::make_unique<Grouping>(
          std::make_unique<Literal>(std::make_unique<lox::Object>(45.67))));

  AstPrinterRPN printer;

  std::cout << printer.print(expression.get()) << "\n";
}
