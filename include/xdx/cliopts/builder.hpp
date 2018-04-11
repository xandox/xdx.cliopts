#pragma once

#include <xdx/cliopts/argument.hpp>
#include <xdx/cliopts/flag.hpp>
#include <xdx/cliopts/options.hpp>

namespace xdx::cliopts
{

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
    Builder& argument(std::string_view long_name, std::string_view description, std::string_view type_name,
                      bool required = true) {
        auto argument = std::make_shared<Argument<Type>>(long_name, description);
        argument->set_required(required);
        argument->set_type_name(type_name);
        options_->add(argument);
        return *this;
    }

    template <class Type>
    Builder& argument(std::string_view long_name, std::string_view description, std::string_view type_name,
                      const Type& default_value) {
        auto argument = std::make_shared<Argument<Type>>(long_name, description);
        argument->set_required(false);
        argument->set_type_name(type_name);
        argument->set_default_value(default_value);
        options_->add(argument);
        return *this;
    }

    template <class Type>
    Builder& argument(char short_name, std::string_view description, std::string_view type_name, bool required = true) {
        auto argument = std::make_shared<Argument<Type>>(short_name, description);
        argument->set_required(required);
        argument->set_type_name(type_name);
        options_->add(argument);
        return *this;
    }

    template <class Type>
    Builder& argument(char short_name, std::string_view description, std::string_view type_name,
                      const Type& default_value) {
        auto argument = std::make_shared<Argument<Type>>(short_name, description);
        argument->set_required(false);
        argument->set_type_name(type_name);
        argument->set_default_value(default_value);
        options_->add(argument);
        return *this;
    }

    template <class Type>
    Builder& argument(char short_name, std::string_view long_name, std::string_view description,
                      std::string_view type_name, bool required = true) {
        auto argument = std::make_shared<Argument<Type>>(short_name, long_name, description);
        argument->set_required(required);
        argument->set_type_name(type_name);
        options_->add(argument);
        return *this;
    }

    template <class Type>
    Builder& argument(char short_name, std::string_view long_name, std::string_view description,
                      std::string_view type_name, const Type& default_value) {
        auto argument = std::make_shared<Argument<Type>>(short_name, long_name, description);
        argument->set_required(false);
        argument->set_type_name(type_name);
        argument->set_default_value(default_value);
        options_->add(argument);
        return *this;
    }

    template <class Type>
    Builder& argument_list(std::string_view long_name, std::string_view description, std::string_view type_name,
                           bool required = true) {
        auto argument = std::make_shared<ArgumentList<Type>>(long_name, description);
        argument->set_required(required);
        argument->set_type_name(type_name);
        options_->add(argument);
        return *this;
    }

    template <class Type>
    Builder& argument_list(std::string_view long_name, std::string_view description, std::string_view type_name,
                           const Type& default_value) {
        auto argument = std::make_shared<ArgumentList<Type>>(long_name, description);
        argument->set_required(false);
        argument->set_type_name(type_name);
        argument->set_default_value(default_value);
        options_->add(argument);
        return *this;
    }

    template <class Type>
    Builder& argument_list(char short_name, std::string_view description, std::string_view type_name,
                           bool required = true) {
        auto argument = std::make_shared<ArgumentList<Type>>(short_name, description);
        argument->set_required(required);
        argument->set_type_name(type_name);
        options_->add(argument);
        return *this;
    }

    template <class Type>
    Builder& argument_list(char short_name, std::string_view description, std::string_view type_name,
                           const Type& default_value) {
        auto argument = std::make_shared<ArgumentList<Type>>(short_name, description);
        argument->set_required(false);
        argument->set_type_name(type_name);
        argument->set_default_value(default_value);
        options_->add(argument);
        return *this;
    }

    template <class Type>
    Builder& argument_list(char short_name, std::string_view long_name, std::string_view description,
                           std::string_view type_name, bool required = true) {
        auto argument = std::make_shared<ArgumentList<Type>>(short_name, long_name, description);
        argument->set_required(required);
        argument->set_type_name(type_name);
        options_->add(argument);
        return *this;
    }

    template <class Type>
    Builder& argument_list(char short_name, std::string_view long_name, std::string_view description,
                           std::string_view type_name, const Type& default_value) {
        auto argument = std::make_shared<ArgumentList<Type>>(short_name, long_name, description);
        argument->set_required(false);
        argument->set_type_name(type_name);
        argument->set_default_value(default_value);
        options_->add(argument);
        return *this;
    }

    std::shared_ptr<Options> get_options() const noexcept {
        return options_;
    }

private:
    std::shared_ptr<Options> options_;
};

}  // namespace xdx::cliopts
