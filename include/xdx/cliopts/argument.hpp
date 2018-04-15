#pragma once

#include <xdx/cliopts/details/from_string.hpp>

#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace xdx::cliopts
{

struct iArgument
{
    virtual ~iArgument() = default;
    virtual char get_short_name() const noexcept = 0;
    virtual std::string_view get_long_name() const noexcept = 0;
    virtual std::string_view get_description() const noexcept = 0;
    virtual std::string_view get_type_name() const = 0;
    virtual std::string_view get_default_value() const = 0;
    virtual bool has_value() const noexcept = 0;
    virtual bool has_default_value() const noexcept = 0;
    virtual bool is_required() const noexcept = 0;
    virtual bool is_many_values() const noexcept = 0;
    virtual void reset_to_default() noexcept = 0;
    virtual std::pair<bool, std::string> set_string_value(const std::string_view& value) noexcept = 0;
};

class ArgumentBase : public iArgument
{
public:
    ArgumentBase(const std::string_view& long_name, const std::string_view& description)
        : long_name_{long_name}
        , description_{description} {
    }

    ArgumentBase(char short_name, const std::string_view& description)
        : short_name_{short_name}
        , description_{description} {
    }

    ArgumentBase(char short_name, const std::string_view& long_name, const std::string_view& description)
        : short_name_{short_name}
        , long_name_{long_name}
        , description_{description} {
    }

    bool is_required() const noexcept final {
        return is_required_;
    }

    void set_type_name(const std::string_view& type_name) {
        type_name_ = type_name;
    }

    void set_required(bool required = true) {
        is_required_ = required;
    }

    char get_short_name() const noexcept final {
        return short_name_;
    }

    std::string_view get_long_name() const noexcept final {
        return long_name_;
    }

    std::string_view get_description() const noexcept final {
        return description_;
    }

    std::string_view get_type_name() const override {
        return type_name_;
    }

private:
    char short_name_ = '\0';
    bool is_required_ = false;
    std::string long_name_;
    std::string description_;
    std::string type_name_;
};

template <class ValueType>
class Argument : public ArgumentBase
{
public:
    Argument(const std::string_view& long_name, const std::string_view& description)
        : ArgumentBase{long_name, description} {
    }

    Argument(char short_name, const std::string_view& description)
        : ArgumentBase{short_name, description} {
    }

    Argument(char short_name, const std::string_view& long_name, const std::string_view& description)
        : ArgumentBase{short_name, long_name, description} {
    }

public:
    std::string_view get_default_value() const final {
        if (!has_default_value()) {
            return {};
        }

        std::ostringstream stream;
        stream << *default_value_;
        return stream.str();
    }

    bool has_default_value() const noexcept final {
        return default_value_.has_value();
    }

    std::pair<bool, std::string> set_string_value(const std::string_view& str_value) noexcept final {
        return details::from_string(&value_, (std::string{str_value}));
    }

    bool has_value() const noexcept final {
        return value_.has_value() || has_default_value();
    }

    void set_default_value(const ValueType& v) {
        default_value_ = v;
    }

    ValueType get_value() const noexcept {
        return value_ ? *value_ : *default_value_;
    }

    bool is_many_values() const noexcept override {
        return false;
    }

    void reset_to_default() noexcept final {
        value_.reset();
    }

private:
    std::optional<ValueType> default_value_;
    std::optional<ValueType> value_;
};

template <class ValueType>
class ArgumentList : public ArgumentBase
{
public:
    ArgumentList(const std::string_view& long_name, const std::string_view& description)
        : ArgumentBase{long_name, description} {
    }

    ArgumentList(char short_name, const std::string_view& description)
        : ArgumentBase{short_name, description} {
    }

    ArgumentList(char short_name, const std::string_view& long_name, const std::string_view& description)
        : ArgumentBase{short_name, long_name, description} {
    }

public:
    std::string_view get_default_value() const final {
        if (!has_default_value()) {
            return {};
        }

        std::ostringstream stream;
        stream << *default_value_;
        return stream.str();
    }

    bool has_default_value() const noexcept final {
        return default_value_.has_value();
    }

    std::pair<bool, std::string> set_string_value(const std::string_view& str_value) noexcept final {
        std::optional<ValueType> val;
        auto parse_res = details::from_string(&val, (std::string{str_value}));

        if (!parse_res.first) {
            return parse_res;
        }

        values_.emplace_back(std::move(*val));

        return {true, std::string{}};
    }

    bool has_value() const noexcept final {
        return !values_.empty() || has_default_value();
    }

    void set_default_value(const ValueType& v) {
        default_value_ = v;
    }

    bool is_many_values() const noexcept override {
        return true;
    }

    std::vector<ValueType> get_values() const noexcept {
        return !values_.empty() ? values_ : std::vector<ValueType>{*default_value_};
    }

    void reset_to_default() noexcept final {
        values_.clear();
    }

private:
    std::optional<ValueType> default_value_;
    std::vector<ValueType> values_;
};

}  // namespace xdx::cliopts
