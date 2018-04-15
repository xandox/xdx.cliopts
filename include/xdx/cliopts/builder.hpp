#pragma once

#include <xdx/cliopts/argument.hpp>
#include <xdx/cliopts/flag.hpp>
#include <xdx/cliopts/options.hpp>

namespace xdx::cliopts
{

namespace details
{

template <class Type>
struct TypeNameSelector
{ static constexpr std::string_view NAME = "Unknown"; };

template <>
struct TypeNameSelector<char>
{ static constexpr std::string_view NAME = "S-BYTE"; };

template <>
struct TypeNameSelector<unsigned char>
{ static constexpr std::string_view NAME = "U-BYTE"; };

template <>
struct TypeNameSelector<short>
{ static constexpr std::string_view NAME = "SHORT"; };

template <>
struct TypeNameSelector<unsigned short>
{ static constexpr std::string_view NAME = "USHORT"; };

template <>
struct TypeNameSelector<int>
{ static constexpr std::string_view NAME = "INT"; };

template <>
struct TypeNameSelector<unsigned int>
{ static constexpr std::string_view NAME = "UINT"; };

// TODO: check that long and int have different size
template <>
struct TypeNameSelector<long>
{ static constexpr std::string_view NAME = "LONG"; };

template <>
struct TypeNameSelector<unsigned long>
{ static constexpr std::string_view NAME = "ULONG"; };

template <>
struct TypeNameSelector<float>
{ static constexpr std::string_view NAME = "FLOAT"; };

template <>
struct TypeNameSelector<double>
{ static constexpr std::string_view NAME = "DOUBLE"; };

template <>
struct TypeNameSelector<std::string>
{ static constexpr std::string_view NAME = "STRING"; };

template <class Type>
inline constexpr std::string_view type_name() {
    return TypeNameSelector<Type>::NAME;
}

}  // namespace details

class Builder
{
public:
    Builder(const std::string_view& name, const std::string_view& description)
        : options_(std::make_shared<Options>(name, description)) {
    }

    Builder& flag(char short_name, std::string_view description) {
        options_->add(std::make_shared<Flag>(short_name, description));
        return *this;
    }

    Builder& flag(std::string_view long_name, std::string_view description) {
        options_->add(std::make_shared<Flag>(long_name, description));
        return *this;
    }

    Builder& flag(char short_name, std::string_view long_name, std::string_view description) {
        options_->add(std::make_shared<Flag>(short_name, long_name, description));
        return *this;
    }

    Builder& flag_count(char short_name, std::string_view description) {
        options_->add(std::make_shared<FlagCount>(short_name, description));
        return *this;
    }

    Builder& flag_count(std::string_view long_name, std::string_view description) {
        options_->add(std::make_shared<FlagCount>(long_name, description));
        return *this;
    }

    Builder& flag_count(char short_name, std::string_view long_name, std::string_view description) {
        options_->add(std::make_shared<FlagCount>(short_name, long_name, description));
        return *this;
    }

    template <class Type>
    Builder& argument(std::string_view long_name, std::string_view description, bool required = true,
                      std::string_view type_name = details::type_name<Type>()) {
        auto argument = std::make_shared<Argument<Type>>(long_name, description);
        argument->set_required(required);
        argument->set_type_name(type_name);
        options_->add(argument);
        return *this;
    }

    template <class Type>
    Builder& argument(std::string_view long_name, std::string_view description, const Type& default_value,
                      std::string_view type_name = details::type_name<Type>()) {
        auto argument = std::make_shared<Argument<Type>>(long_name, description);
        argument->set_required(false);
        argument->set_type_name(type_name);
        argument->set_default_value(default_value);
        options_->add(argument);
        return *this;
    }

    template <class Type>
    Builder& argument(char short_name, std::string_view description, bool required = true,
                      std::string_view type_name = details::type_name<Type>()) {
        auto argument = std::make_shared<Argument<Type>>(short_name, description);
        argument->set_required(required);
        argument->set_type_name(type_name);
        options_->add(argument);
        return *this;
    }

    template <class Type>
    Builder& argument(char short_name, std::string_view description, const Type& default_value,
                      std::string_view type_name = details::type_name<Type>()) {
        auto argument = std::make_shared<Argument<Type>>(short_name, description);
        argument->set_required(false);
        argument->set_type_name(type_name);
        argument->set_default_value(default_value);
        options_->add(argument);
        return *this;
    }

    template <class Type>
    Builder& argument(char short_name, std::string_view long_name, std::string_view description, bool required = true,
                      std::string_view type_name = details::type_name<Type>()) {
        auto argument = std::make_shared<Argument<Type>>(short_name, long_name, description);
        argument->set_required(required);
        argument->set_type_name(type_name);
        options_->add(argument);
        return *this;
    }

    template <class Type>
    Builder& argument(char short_name, std::string_view long_name, std::string_view description,
                      const Type& default_value, std::string_view type_name = details::type_name<Type>()) {
        auto argument = std::make_shared<Argument<Type>>(short_name, long_name, description);
        argument->set_required(false);
        argument->set_type_name(type_name);
        argument->set_default_value(default_value);
        options_->add(argument);
        return *this;
    }

    template <class Type>
    Builder& argument_list(std::string_view long_name, std::string_view description, bool required = true,
                           std::string_view type_name = details::type_name<Type>()) {
        auto argument = std::make_shared<ArgumentList<Type>>(long_name, description);
        argument->set_required(required);
        argument->set_type_name(type_name);
        options_->add(argument);
        return *this;
    }

    template <class Type>
    Builder& argument_list(std::string_view long_name, std::string_view description, const Type& default_value,
                           std::string_view type_name = details::type_name<Type>()) {
        auto argument = std::make_shared<ArgumentList<Type>>(long_name, description);
        argument->set_required(false);
        argument->set_type_name(type_name);
        argument->set_default_value(default_value);
        options_->add(argument);
        return *this;
    }

    template <class Type>
    Builder& argument_list(char short_name, std::string_view description, bool required = true,
                           std::string_view type_name = details::type_name<Type>()) {
        auto argument = std::make_shared<ArgumentList<Type>>(short_name, description);
        argument->set_required(required);
        argument->set_type_name(type_name);
        options_->add(argument);
        return *this;
    }

    template <class Type>
    Builder& argument_list(char short_name, std::string_view description, const Type& default_value,
                           std::string_view type_name = details::type_name<Type>()) {
        auto argument = std::make_shared<ArgumentList<Type>>(short_name, description);
        argument->set_required(false);
        argument->set_type_name(type_name);
        argument->set_default_value(default_value);
        options_->add(argument);
        return *this;
    }

    template <class Type>
    Builder& argument_list(char short_name, std::string_view long_name, std::string_view description,
                           bool required = true, std::string_view type_name = details::type_name<Type>()) {
        auto argument = std::make_shared<ArgumentList<Type>>(short_name, long_name, description);
        argument->set_required(required);
        argument->set_type_name(type_name);
        options_->add(argument);
        return *this;
    }

    template <class Type>
    Builder& argument_list(char short_name, std::string_view long_name, std::string_view description,
                           const Type& default_value, std::string_view type_name = details::type_name<Type>()) {
        auto argument = std::make_shared<ArgumentList<Type>>(short_name, long_name, description);
        argument->set_required(false);
        argument->set_type_name(type_name);
        argument->set_default_value(default_value);
        options_->add(argument);
        return *this;
    }

    Builder& add_subcommand(OptionsPtr subcommand) {
        options_->add(std::static_pointer_cast<iOptions>(subcommand));
        return *this;
    }

    std::shared_ptr<Options> get_options() const noexcept {
        return options_;
    }

private:
    std::shared_ptr<Options> options_;
};

}  // namespace xdx::cliopts
