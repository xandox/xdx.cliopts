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
    virtual std::pair<bool, std::string> set_string_value(const std::string_view& value) = 0;
    virtual char short_name() const = 0;
    virtual std::string_view long_name() const = 0;
    virtual std::string_view get_description() const = 0;
    virtual std::string_view get_type_name() const = 0;
    virtual std::string_view get_default_value() const = 0;
    virtual bool has_value() const = 0;
    virtual bool has_default_value() const = 0;
    virtual bool is_required() const = 0;
};

template <class FinalArgument>
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

    bool is_required() const final {
        return is_required_;
    }

    FinalArgument& set_type_name(const std::string_view& type_name) {
        type_name_ = type_name;
        return _final_this();
    }

    FinalArgument& set_required(bool required = true) {
        is_required_ = required;
        return _final_this();
    }

    char short_name() const final {
        return short_name_;
    }

    std::string_view long_name() const final {
        return long_name_;
    }

    std::string_view get_description() const final {
        return description_;
    }

    std::string_view get_type_name() const override {
        return type_name_;
    }

private:
    FinalArgument& _final_this() {
        return *static_cast<FinalArgument*>(this);
    }

private:
    char short_name_ = '\0';
    bool is_required_ = false;
    std::string long_name_;
    std::string description_;
    std::string type_name_;
};

template <class ValueType>
class Argument : public ArgumentBase<Argument<ValueType>>
{
    using Base = ArgumentBase<Argument<ValueType>>;

public:
    Argument(const std::string_view& long_name, const std::string_view& description)
        : Base{long_name, description} {
    }

    Argument(char short_name, const std::string_view& description)
        : Base{short_name, description} {
    }

    Argument(char short_name, const std::string_view& long_name, const std::string_view& description)
        : Base{short_name, long_name, description} {
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

    bool has_default_value() const final {
        return default_value_.has_value();
    }

    std::pair<bool, std::string> set_string_value(const std::string_view& str_value) final {
        return details::from_string(&value_, (std::string{str_value}));
    }

    bool has_value() const final {
        return value_.has_value() || has_default_value();
    }

    void set_default_value(const ValueType& v) {
        default_value_ = v;
    }

private:
    std::optional<ValueType> default_value_;
    std::optional<ValueType> value_;
};

template <class ValueType>
class ArgumentList : public ArgumentBase<ArgumentList<ValueType>>
{
    using Base = ArgumentBase<ArgumentList<ValueType>>;

public:
    ArgumentList(const std::string_view& long_name, const std::string_view& description)
        : Base{long_name, description} {
    }

    ArgumentList(char short_name, const std::string_view& description)
        : Base{short_name, description} {
    }

    ArgumentList(char short_name, const std::string_view& long_name, const std::string_view& description)
        : Base{short_name, long_name, description} {
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

    bool has_default_value() const final {
        return default_value_.has_value();
    }

    std::pair<bool, std::string> set_string_value(const std::string_view& str_value) final {
        std::optional<ValueType> val;
        auto parse_res = details::from_string(&val, (std::string{str_value}));

        if (!parse_res.first) {
            return parse_res;
        }

        values_.emplace_back(std::move(*val));

        return {true, std::string{}};
    }

    bool has_value() const final {
        return !values_.empty() || has_default_value();
    }

    void set_default_value(const ValueType& v) {
        default_value_ = v;
    }

private:
    std::optional<ValueType> default_value_;
    std::vector<ValueType> values_;
};

}  // namespace xdx::cliopts
