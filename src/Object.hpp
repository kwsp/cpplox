#pragma once
#include <concepts>
#include <memory>
#include <string>
#include <utility>
#include <variant>

namespace lox {
// Lox objects can be float64, boolean, string, or class

// Represents a literal value (double | bool | string)
class Object {
public:
  explicit Object() {}
  explicit Object(const double val) : object(val) {}
  explicit Object(const bool val) : object(val) {}
  explicit Object(const std::string &val) : object(val) {}

  auto toString() const -> std::string {
    if (std::holds_alternative<double>(object))
      return std::to_string(std::get<double>(object));

    if (std::holds_alternative<bool>(object))
      return std::to_string(std::get<bool>(object));

    if (std::holds_alternative<std::string>(object))
      return std::get<std::string>(object);

    return "";
  }

  bool empty() const { return object.index() == 0; }

private:
  std::variant<std::monostate, double, bool, std::string> object;
};

} // namespace lox