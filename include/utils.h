#ifndef UTILS_H
#define UTILS_H

#include <format>
#include <string>
#include <vector>
#include <variant>

std::string trim(const char *pStr);

template<typename T>
std::string to_string(T Value, bool ForceReal = true) {
    if constexpr (std::is_integral_v<T>) {
        if (ForceReal) return std::format("{}.0", Value);
        return std::to_string(Value);
    } else if constexpr (std::is_floating_point_v<T>) {
        return std::format("{}{}", Value, (ForceReal && int(Value) == Value) ? ".0" : "");
    } else if constexpr (std::is_convertible_v<T, std::string>) {
        return std::string(Value);
    } else if constexpr (std::is_same_v<T, std::string_view>) {
        return Value.data();
    } else if constexpr (std::is_same_v<T, std::monostate> || std::is_same_v<T, std::nullptr_t> ) {
        return "null";
    } else {
        static_assert(!std::is_same_v<T, T>, "One of the passed arguments cannot be converted to a string");
        return {};
    }
}

#endif // UTILS_H
