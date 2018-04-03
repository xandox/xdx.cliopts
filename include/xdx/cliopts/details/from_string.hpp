#pragma once

#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>

namespace xdx::cliopts::details
{

template <class ValueType>
inline std::pair<bool, std::string> from_string(std::optional<ValueType>* result, const std::string& str) {
    std::istringstream stream{str};
    ValueType value;
    stream >> value;

    if (!stream) {
        return {false, "can't parse"};
    }

    *result = value;

    return {true, std::string{}};
}

template <class Numeric, class ParseResult>
inline Numeric parse_from_string(ParseResult (*func)(const std::string&, size_t*, int), const std::string& str) {
    size_t pos;
    ParseResult result = func(str, &pos, 10);
    if (pos != str.size()) {
        throw std::invalid_argument("unexpected trailing chars");
    }

    if (std::numeric_limits<Numeric>::max() < result) {
        throw std::out_of_range("to big");
    }

    if (result < std::numeric_limits<Numeric>::min()) {
        throw std::out_of_range("to small");
    }

    return static_cast<Numeric>(result);
}

template <class Numeric>
inline Numeric parse_from_string(Numeric (*func)(const std::string&, size_t*), const std::string& str) {
    size_t pos;
    Numeric result = func(str, &pos);
    if (pos != str.size()) {
        throw std::invalid_argument("unexpected trailing chars");
    }
    return static_cast<Numeric>(result);
}

template <>
inline std::pair<bool, std::string> from_string(std::optional<short>* result, const std::string& str) {
    try {
        *result = parse_from_string<short>(std::stoi, str);
        return {true, std::string{}};
    } catch (std::exception& ex) {
        return {false, ex.what()};
    }
}

template <>
inline std::pair<bool, std::string> from_string(std::optional<unsigned short>* result, const std::string& str) {
    try {
        *result = parse_from_string<unsigned short>(std::stoul, str);
        return {true, std::string{}};
    } catch (std::exception& ex) {
        return {false, ex.what()};
    }
}

template <>
inline std::pair<bool, std::string> from_string(std::optional<int>* result, const std::string& str) {
    try {
        *result = parse_from_string<int>(std::stoi, str);
        return {true, std::string{}};
    } catch (std::exception& ex) {
        return {false, ex.what()};
    }
}

template <>
inline std::pair<bool, std::string> from_string(std::optional<unsigned int>* result, const std::string& str) {
    try {
        *result = parse_from_string<unsigned int>(std::stoul, str);
        return {true, std::string{}};
    } catch (std::exception& ex) {
        return {false, ex.what()};
    }
}

template <>
inline std::pair<bool, std::string> from_string(std::optional<long>* result, const std::string& str) {
    try {
        *result = parse_from_string<long>(std::stol, str);
        return {true, std::string{}};
    } catch (std::exception& ex) {
        return {false, ex.what()};
    }
}

template <>
inline std::pair<bool, std::string> from_string(std::optional<unsigned long>* result, const std::string& str) {
    try {
        *result = parse_from_string<unsigned long>(std::stoul, str);
        return {true, std::string{}};
    } catch (std::exception& ex) {
        return {false, ex.what()};
    }
}

template <>
inline std::pair<bool, std::string> from_string(std::optional<long long>* result, const std::string& str) {
    try {
        *result = parse_from_string<long long>(std::stoll, str);
        return {true, std::string{}};
    } catch (std::exception& ex) {
        return {false, ex.what()};
    }
}

template <>
inline std::pair<bool, std::string> from_string(std::optional<unsigned long long>* result, const std::string& str) {
    try {
        *result = parse_from_string<unsigned long long>(std::stoull, str);
        return {true, std::string{}};
    } catch (std::exception& ex) {
        return {false, ex.what()};
    }
}

template <>
inline std::pair<bool, std::string> from_string(std::optional<float>* result, const std::string& str) {
    try {
        *result = parse_from_string(std::stof, str);
        return {true, std::string{}};
    } catch (std::exception& ex) {
        return {false, ex.what()};
    }
}

template <>
inline std::pair<bool, std::string> from_string(std::optional<double>* result, const std::string& str) {
    try {
        *result = parse_from_string(std::stod, str);
        return {true, std::string{}};
    } catch (std::exception& ex) {
        return {false, ex.what()};
    }
}

template <>
inline std::pair<bool, std::string> from_string(std::optional<long double>* result, const std::string& str) {
    try {
        *result = parse_from_string(std::stold, str);
        return {true, std::string{}};
    } catch (std::exception& ex) {
        return {false, ex.what()};
    }
}

template <>
inline std::pair<bool, std::string> from_string(std::optional<std::string>* result, const std::string& str) {
    *result = str;
    return {true, std::string{}};
}

}  // namespace xdx::cliopts::details
