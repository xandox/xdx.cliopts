#include <xdx/cliopts/argument.hpp>
#include <xdx/cliopts/flag.hpp>
#include <xdx/cliopts/options.hpp>

#include <algorithm>

namespace xdx::cliopts
{

Options::Options(const std::string_view& name, const std::string_view& description)
    : name_{name}
    , description_{description} {
}

std::string_view Options::get_name() const noexcept {
    return name_;
}

std::string_view Options::get_description() const noexcept {
    return description_;
}

size_t Options::flags_count() const noexcept {
    return flags_.size();
}

size_t Options::arguments_count() const noexcept {
    return arguments_.size();
}

size_t Options::subcommands_count() const noexcept {
    return subcommands_.size();
}

Options::FlagPtr Options::get_flag(size_t idx) const noexcept {
    return flags_[idx];
}

Options::ArgumentPtr Options::get_argument(size_t idx) const noexcept {
    return arguments_[idx];
}

Options::SubcommandPtr Options::get_subcommand(size_t idx) const noexcept {
    return subcommands_[idx];
}

Options::FlagPtr Options::find_flag(char short_name) const noexcept {
    const auto it = std::find_if(flags_.begin(), flags_.end(),
                                 [short_name](const auto& flag) { return flag->get_short_name() == short_name; });
    return it != flags_.end() ? *it : nullptr;
}

Options::FlagPtr Options::find_flag(std::string_view long_name) const noexcept {
    const auto it = std::find_if(flags_.begin(), flags_.end(),
                                 [long_name](const auto& flag) { return flag->get_long_name() == long_name; });
    return it != flags_.end() ? *it : nullptr;
}

Options::FlagCountPtr Options::find_flag_count(char short_name) const noexcept {
    return std::dynamic_pointer_cast<FlagCount>(find_flag(short_name));
}

Options::FlagCountPtr Options::find_flag_count(std::string_view long_name) const noexcept {
    return std::dynamic_pointer_cast<FlagCount>(find_flag(long_name));
}

Options::ArgumentPtr Options::find_argument(char short_name) const noexcept {
    const auto it = std::find_if(arguments_.begin(), arguments_.end(), [short_name](const auto& argument) {
        return argument->get_short_name() == short_name;
    });
    return it != arguments_.end() ? *it : nullptr;
}

Options::ArgumentPtr Options::find_argument(std::string_view long_name) const noexcept {
    const auto it = std::find_if(arguments_.begin(), arguments_.end(),
                                 [long_name](const auto& argument) { return argument->get_long_name() == long_name; });
    return it != arguments_.end() ? *it : nullptr;
}

Options::SubcommandPtr Options::find_subcommand(std::string_view name) const noexcept {
    const auto it = std::find_if(subcommands_.begin(), subcommands_.end(),
                                 [name](const auto& subcommand) { return subcommand->get_name() == name; });
    return it != subcommands_.end() ? *it : nullptr;
}

void Options::add(FlagPtr&& flag) {
    _assert_short_name(flag->get_short_name());
    _assert_long_name(flag->get_long_name());
    flags_.emplace_back(std::move(flag));
}

void Options::add(ArgumentPtr&& arg) {
    _assert_short_name(arg->get_short_name());
    _assert_long_name(arg->get_long_name());
    arguments_.emplace_back(std::move(arg));
}

void Options::add(SubcommandPtr&& sub) {
    _assert_sub_name(sub->get_name());
    subcommands_.emplace_back(std::move(sub));
}

void Options::reset_to_default() noexcept {
    for (auto& flag : flags_) {
        flag->reset_to_default();
    }

    for (auto& arg : arguments_) {
        arg->reset_to_default();
    }

    for (auto& sub : subcommands_) {
        sub->reset_to_default();
    }
}

void Options::_assert_short_name(char ch) {
    if (ch == '\0') {
        return;
    }

    for (const auto& f : flags_) {
        if (f->get_short_name() == ch) {
            throw std::invalid_argument(std::string("dublicated short name: '") + std::string{ch} + "'");
        }
    }

    for (const auto& a : arguments_) {
        if (a->get_short_name() == ch) {
            throw std::invalid_argument(std::string("dublicated short name: '") + std::string{ch} + "'");
        }
    }
}

void Options::_assert_long_name(const std::string_view& lname) {
    if (lname.empty()) {
        return;
    }

    for (const auto& f : flags_) {
        if (f->get_long_name() == lname) {
            throw std::invalid_argument(std::string("dublicated long name: '") + std::string(lname) + "'");
        }
    }

    for (const auto& a : arguments_) {
        if (a->get_long_name() == lname) {
            throw std::invalid_argument(std::string("dublicated long name: '") + std::string(lname) + "'");
        }
    }
}

void Options::_assert_sub_name(const std::string_view& name) {
    if (name.empty()) {
        throw std::invalid_argument("subcommand name can't be empty");
    }

    for (const auto& s : subcommands_) {
        if (s->get_name() == name) {
            throw std::invalid_argument(std::string("dublicated subcommand name: '") + std::string(name) + "'");
        }
    }
}
}  // namespace xdx::cliopts
