#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Scanner.hpp"
#include "Token.hpp"
#include "TokenType.hpp"
#include <memory>

namespace lox {

const static std::unordered_map<std::string, TokenType> KEYWORDS = {
    {"and", TokenType::AND},       {"class", TokenType::CLASS},
    {"else", TokenType::ELSE},     {"false", TokenType::FALSE},
    {"for", TokenType::FOR},       {"fun", TokenType::FUN},
    {"if", TokenType::IF},         {"nil", TokenType::NIL},
    {"or", TokenType::OR},         {"print", TokenType::PRINT},
    {"return", TokenType::RETURN}, {"super", TokenType::SUPER},
    {"this", TokenType::THIS},     {"true", TokenType::TRUE},
    {"var", TokenType::VAR},       {"while", TokenType::WHILE},
};

bool Scanner::isAtEnd() { return current >= source.size(); }

char Scanner::advance() { return source[current++]; }

void Scanner::addToken(TokenType type) { addToken(type, nullptr); }

void Scanner::addToken(TokenType type, std::shared_ptr<Object> literal) {
  const auto text = source.substr(start, current - start);
  // tokens.push_back(Token(type, text, std::move(literal), line));
  tokens.emplace_back(type, text, std::move(literal), line);
}

void Scanner::scanToken() {
  const char c = advance();
  switch (c) {
  case '(':
    addToken(TokenType::LEFT_PAREN);
    break;
  case ')':
    addToken(TokenType::RIGHT_PAREN);
    break;
  case '{':
    addToken(TokenType::LEFT_BRACE);
    break;
  case '}':
    addToken(TokenType::RIGHT_BRACE);
    break;
  case ',':
    addToken(TokenType::COMMA);
    break;
  case '.':
    addToken(TokenType::DOT);
    break;
  case '-':
    addToken(TokenType::MINUS);
    break;
  case '+':
    addToken(TokenType::PLUS);
    break;
  case ';':
    addToken(TokenType::SEMICOLON);
    break;
  case '*':
    addToken(TokenType::STAR);
    break;

  // Operators
  case '!':
    addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
    break;
  case '=':
    addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
    break;
  case '<':
    addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
    break;
  case '>':
    addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
    break;

  // Longer lexemes
  case '/':
    if (match('/')) {
      // A comment goes until the end of the line.
      while (peek() != '\n' && !isAtEnd())
        advance();
    } else {
      addToken(TokenType::SLASH);
    }
    break;
  case ' ':
  case '\r':
  case '\t':
    // ignore whitespace
    break;
  case '\n':
    line++;
    break;

  // Literals
  case '"':
    string();
    break;

  case 'o':
    if (match('r')) {
      addToken(TokenType::OR);
    }
    break;

  default:

    if (isDigit(c)) {
      number();
    } else if (isAlpha(c)) {
      // Reserved words and identifiers
      identifier();
    } else {
      errorHandler.error(line, "Unexpected character.");
    }

    break;
  }
}

const std::vector<Token> &Scanner::scanTokens() {
  while (!isAtEnd()) {
    // At the beginning of the next lexeme.
    start = current;
    scanToken();
  }
  tokens.push_back(Token(TokenType::LOX_EOF, "", nullptr, line));
  return tokens;
};

bool Scanner::match(char expected) {
  if (isAtEnd())
    return false;
  if (source[current] != expected)
    return false;
  current++;
  return true;
}

char Scanner::peek() {
  if (isAtEnd())
    return '\0';
  return source[current];
}

char Scanner::peekNext() {
  if (current + 1 >= source.length())
    return '\0';
  return source[current + 1];
}

void Scanner::string() {
  while (peek() != '"' && !isAtEnd()) {
    if (peek() == '\n')
      line++;
    advance();
  }
  if (isAtEnd()) {
    errorHandler.error(line, "Unterminated string.");
    return;
  }
  // The closing "
  advance();
  // Trim the surrounding quotes
  const std::string value = source.substr(start + 1, current - 2 - start);
  addToken(TokenType::STRING, Object::make(value));
}

void Scanner::number() {
  while (isDigit(peek()))
    advance();

  // Look for a fractional part
  if (peek() == '.' && isDigit(peekNext())) {
    // Consume the "."
    advance();

    while (isDigit(peek()))
      advance();
  }

  const double value = std::stod(source.substr(start, current - start));
  addToken(TokenType::NUMBER, Object::make(value));
}

bool Scanner::isDigit(char c) { return c >= '0' && c <= '9'; }

bool Scanner::isAlpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Scanner::isAlphaNumeric(char c) { return isAlpha(c) || isDigit(c); }

void Scanner::identifier() {
  while (isAlphaNumeric(peek()))
    advance();

  const auto text = source.substr(start, current - start);
  TokenType type = [&]() {
    if (auto it = KEYWORDS.find(text); it != KEYWORDS.end()) {
      return it->second;
    }
    return TokenType::IDENTIFIER;
  }();

  addToken(type);
}

} // namespace lox