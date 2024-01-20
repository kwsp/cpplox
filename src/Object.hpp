#pragma once
#include <concepts>
#include <memory>
#include <string>
#include <variant>

namespace lox {
// Lox objects can be float64, boolean, string, or class

class Object {
public:
  template <typename T> static std::shared_ptr<Object> make(const T &val) {
    static_assert(std::is_same_v<T, double> || std::is_same_v<T, bool> ||
                  std::is_same_v<T, std::string>);
    return std::make_shared<Object>(val);
  }

  template <typename T> Object(const T &val) : object(val) {}

  std::string toString() const {
    if (std::holds_alternative<double>(object))
      return std::to_string(std::get<double>(object));

    if (std::holds_alternative<bool>(object))
      return std::to_string(std::get<bool>(object));

    if (std::holds_alternative<std::string>(object))
      return std::get<std::string>(object);

    return "";
  }

private:
  std::variant<double, bool, std::string> object;
};

} // namespace lox