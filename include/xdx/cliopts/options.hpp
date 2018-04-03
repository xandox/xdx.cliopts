#pragma once

#include <xdx/cliopts/argument.hpp>
#include <xdx/cliopts/flag.hpp>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <string_view>
#include <vector>

namespace xdx::cliopts
{

struct iOptions
{
    virtual ~iOptions() = default;
    virtual std::string_view get_name() const = 0;
    virtual std::string_view get_description() const = 0;
    virtual void print_help(std::ostream&) const = 0;
};

template <class FinalOptions>
class Options : public iOptions
{
    enum IdxType
    {
        F,
        A,
    };

    struct NameRef
    {
        IdxType type;
        size_t idx;
    };

public:
    using Argument = std::unique_ptr<iArgument>;
    using Flag = std::unique_ptr<FlagBase>;
    using Opts = std::unique_ptr<iOptions>;

public:
    Options() {
    }

public:
    template <class ValueType>
    FinalOptions& arg(char short_name, const std::string_view& description, const std::string_view& type_name,
                      bool required = true) {
        _assert_uniq_short_name(short_name, {A, arguments_.size()});
        auto argument = std::make_unique<xdx::cliopts::Argument<ValueType>>(short_name, description);
        argument->set_required(required).set_type_name(type_name);
        arguments_.emplace_back(std::move(argument));

        return _final_opts();
    }

    template <class ValueType>
    FinalOptions& arg(const std::string_view& long_name, const std::string_view& description,
                      const std::string_view& type_name, bool required = true) {
        _assert_uniq_long_name(long_name, {A, arguments_.size()});
        auto argument = std::make_unique<xdx::cliopts::Argument<ValueType>>(long_name, description);
        argument->set_required(required).set_type_name(type_name);
        arguments_.emplace_back(std::move(argument));

        return _final_opts();
    }

    template <class ValueType>
    FinalOptions& arg(char short_name, const std::string_view& long_name, const std::string_view& description,
                      const std::string_view& type_name, bool required = true) {
        _assert_uniq_long_name(long_name, {A, arguments_.size()});
        _assert_uniq_short_name(short_name, {A, arguments_.size()});
        auto argument = std::make_unique<xdx::cliopts::Argument<ValueType>>(short_name, long_name, description);
        argument->set_required(required).set_type_name(type_name);
        arguments_.emplace_back(std::move(argument));

        return _final_opts();
    }

    template <class ValueType>
    FinalOptions& arg(char short_name, const std::string_view& description, const std::string_view& type_name,
                      const ValueType& default_value) {
        _assert_uniq_short_name(short_name, {A, arguments_.size()});
        auto argument = std::make_unique<xdx::cliopts::Argument<ValueType>>(short_name, description);
        argument->set_type_name(type_name).set_default_value(default_value);
        arguments_.emplace_back(std::move(argument));

        return _final_opts();
    }

    template <class ValueType>
    FinalOptions& arg(const std::string_view& long_name, const std::string_view& description,
                      const std::string_view& type_name, const ValueType& default_value) {
        _assert_uniq_long_name(long_name, {A, arguments_.size()});
        auto argument = std::make_unique<xdx::cliopts::Argument<ValueType>>(long_name, description);
        argument->set_type_name(type_name).set_default_value(default_value);
        arguments_.emplace_back(std::move(argument));

        return _final_opts();
    }

    template <class ValueType>
    FinalOptions& arg(char short_name, const std::string_view& long_name, const std::string_view& description,
                      const std::string_view& type_name, const ValueType& default_value) {
        _assert_uniq_long_name(long_name, {A, arguments_.size()});
        _assert_uniq_short_name(short_name, {A, arguments_.size()});
        auto argument = std::make_unique<xdx::cliopts::Argument<ValueType>>(short_name, long_name, description);
        argument->set_type_name(type_name).set_default_value(default_value);
        arguments_.emplace_back(std::move(argument));

        return _final_opts();
    }

    template <class ValueType>
    FinalOptions& arg_list(char short_name, const std::string_view& description, const std::string_view& type_name,
                           bool required = true) {
        _assert_uniq_short_name(short_name, {A, arguments_.size()});
        auto argument = std::make_unique<xdx::cliopts::ArgumentList<ValueType>>(short_name, description);
        argument->set_required(required).set_type_name(type_name);
        arguments_.emplace_back(std::move(argument));

        return _final_opts();
    }

    template <class ValueType>
    FinalOptions& arg_list(const std::string_view& long_name, const std::string_view& description,
                           const std::string_view& type_name, bool required = true) {
        _assert_uniq_long_name(long_name, {A, arguments_.size()});
        auto argument = std::make_unique<xdx::cliopts::ArgumentList<ValueType>>(long_name, description);
        argument->set_required(required).set_type_name(type_name);
        arguments_.emplace_back(std::move(argument));

        return _final_opts();
    }

    template <class ValueType>
    FinalOptions& arg_list(char short_name, const std::string_view& long_name, const std::string_view& description,
                           const std::string_view& type_name, bool required = true) {
        _assert_uniq_long_name(long_name, {A, arguments_.size()});
        _assert_uniq_short_name(short_name, {A, arguments_.size()});
        auto argument = std::make_unique<xdx::cliopts::ArgumentList<ValueType>>(short_name, long_name, description);
        argument->set_required(required).set_type_name(type_name);
        arguments_.emplace_back(std::move(argument));

        return _final_opts();
    }

    template <class ValueType>
    FinalOptions& arg_list(char short_name, const std::string_view& description, const std::string_view& type_name,
                           const ValueType& default_value) {
        _assert_uniq_short_name(short_name, {A, arguments_.size()});
        auto argument = std::make_unique<xdx::cliopts::ArgumentList<ValueType>>(short_name, description);
        argument->set_type_name(type_name).set_default_value(default_value);
        arguments_.emplace_back(std::move(argument));

        return _final_opts();
    }

    template <class ValueType>
    FinalOptions& arg_list(const std::string_view& long_name, const std::string_view& description,
                           const std::string_view& type_name, const ValueType& default_value) {
        _assert_uniq_long_name(long_name, {A, arguments_.size()});
        auto argument = std::make_unique<xdx::cliopts::ArgumentList<ValueType>>(long_name, description);
        argument->set_type_name(type_name).set_default_value(default_value);
        arguments_.emplace_back(std::move(argument));

        return _final_opts();
    }

    template <class ValueType>
    FinalOptions& arg_list(char short_name, const std::string_view& long_name, const std::string_view& description,
                           const std::string_view& type_name, const ValueType& default_value) {
        _assert_uniq_long_name(long_name, {A, arguments_.size()});
        _assert_uniq_short_name(short_name, {A, arguments_.size()});
        auto argument = std::make_unique<xdx::cliopts::ArgumentList<ValueType>>(short_name, long_name, description);
        argument->set_type_name(type_name).set_default_value(default_value);
        arguments_.emplace_back(std::move(argument));

        return _final_opts();
    }

    FinalOptions& flag(char short_name, const std::string_view& description) {
        _assert_uniq_short_name(short_name, {F, flags_.size()});
        auto flag = std::make_unique<xdx::cliopts::Flag>(short_name, description);
        flags_.emplace_back(std::move(flag));
        return _final_opts();
    }

    FinalOptions& flag(const std::string_view& long_name, const std::string_view& description) {
        _assert_uniq_long_name(long_name, {F, flags_.size()});
        auto flag = std::make_unique<xdx::cliopts::Flag>(long_name, description);
        flags_.emplace_back(std::move(flag));
        return _final_opts();
    }

    FinalOptions& flag(char short_name, const std::string_view& long_name, const std::string_view& description) {
        _assert_uniq_short_name(short_name, {F, flags_.size()});
        _assert_uniq_long_name(long_name, {F, flags_.size()});
        auto flag = std::make_unique<xdx::cliopts::Flag>(short_name, long_name, description);
        flags_.emplace_back(std::move(flag));
        return _final_opts();
    }

    FinalOptions& flag_count(char short_name, const std::string_view& description) {
        _assert_uniq_short_name(short_name, {F, flags_.size()});
        auto flag = std::make_unique<xdx::cliopts::FlagCount>(short_name, description);
        flags_.emplace_back(std::move(flag));
        return _final_opts();
    }

    FinalOptions& flag_count(const std::string_view& long_name, const std::string_view& description) {
        _assert_uniq_long_name(long_name, {F, flags_.size()});
        auto flag = std::make_unique<xdx::cliopts::FlagCount>(long_name, description);
        flags_.emplace_back(std::move(flag));
        return _final_opts();
    }

    FinalOptions& flag_count(char short_name, const std::string_view& long_name, const std::string_view& description) {
        _assert_uniq_short_name(short_name, {F, flags_.size()});
        _assert_uniq_long_name(long_name, {F, flags_.size()});
        auto flag = std::make_unique<xdx::cliopts::FlagCount>(short_name, long_name, description);
        flags_.emplace_back(std::move(flag));
        return _final_opts();
    }

    bool has_flags() const {
        return !flags_.empty();
    }

    bool has_arguments() const {
        return !arguments_.empty();
    }

    bool has_subcommands() const {
        return !opts_.empty();
    }

    void print_help(std::ostream& out) const override {
        constexpr auto name_size_cmp = [](const auto& lhr, const auto& rhr) {
            return lhr.first.size() < rhr.first.size();
        };
        constexpr auto type_name_size_cmp = [](const auto& lhr, const auto& rhr) {
            return lhr->get_type_name().size() < rhr->get_type_name().size();
        };
        constexpr auto out_description = [](std::ostream& out, std::string_view description, size_t width, size_t offset) {
            while (!description.empty()) {
                if (description.size() <= width) {
                    out << description;
                    return;
                }
                auto space = description.rfind(' ', width);
                if (space == 0) {
                    out << description;
                    return;
                }
                auto head = description.substr(0, space);
                out << head << '\n';
                out << std::setw(offset) << ' ';
                description = description.substr(space + 1);
            }
        };
        const auto longest_name_size = !long_names_.empty()
                                           ? std::max_element(long_names_.begin(), long_names_.end(), name_size_cmp)
                                                 ->first.size()
                                           : 0;
        const auto longest_subname_size = !opt_names_.empty()
                                              ? std::max_element(opt_names_.begin(), opt_names_.end(), name_size_cmp)
                                                    ->first.size()
                                              : 0;
        const auto longest_type_name = !arguments_.empty()
                                           ? (*std::max_element(arguments_.begin(), arguments_.end(), type_name_size_cmp))
                                                 ->get_type_name()
                                                 .size()
                                           : 0;
        const auto has_short_naems = !short_names_.empty();

        constexpr size_t shift = 4;
        constexpr size_t max_line_width = 80;
        size_t flag_prefix_size = shift;
        size_t args_prefix_size = shift;
        size_t opts_prefix_size = shift + longest_subname_size + 1;
        if (has_short_naems) {
            flag_prefix_size += 3;
            args_prefix_size += 3;
        }
        if (longest_name_size != 0) {
            flag_prefix_size += longest_name_size + 3;
            args_prefix_size += longest_name_size + 3;
        }

        if (longest_type_name != 0) {
            args_prefix_size += longest_type_name + 1;
        }

        const auto out_short = [&](const auto& src) {
            if (has_short_naems) {
                if (src->short_name() != '\0') {
                    out << '-' << src->short_name();
                } else {
                    out << ' ' << ' ';
                }
            }
        };

        const auto out_long = [&](const auto& src) {
            if (longest_name_size != 0) {
                if (!src->long_name().empty()) {
                    if (src->short_name() != '\0') {
                        out << '|';
                    } else {
                        out << ' ';
                    }
                    out << '-' << '-' << std::setw(longest_name_size) << std::left << src->long_name();
                } else {
                    out << ' ' << std::setw(longest_name_size + 2) << ' ';
                }
            } else {
                out << ' ';
            }
        };

        if (!flags_.empty()) {
            out << "FLAGS:\n";
            for (const auto& f : flags_) {
                out << std::setw(shift) << ' ';

                out_short(f);
                out_long(f);

                out << ' ';
                out_description(out, f->get_description(), max_line_width - flag_prefix_size, flag_prefix_size);
                out << '\n';
            }
            out << '\n';
        }

        if (!arguments_.empty()) {
            out << "ARGUMENTS:\n";
            for (const auto& a : arguments_) {
                out << std::setw(shift) << ' ';

                out_short(a);
                out_long(a);

                if (longest_type_name != 0) {
                    out << ' ' << std::setw(longest_type_name) << std::left;
                    if (!a->get_type_name().empty()) {
                        out << a->get_type_name();
                    } else {
                        out << ' ';
                    }
                }

                out << ' ';
                out_description(out, a->get_description(), max_line_width - args_prefix_size, args_prefix_size);
                out << '\n';
            }
            out << '\n';
        }

        if (!opts_.empty()) {
            out << "SUBCOMMANDS:\n";
            for (const auto& o : opts_) {
                out << std::setw(shift) << ' ';
                out << std::setw(longest_subname_size) << std::left << o->get_name();
                out << ' ';
                out_description(out, o->get_description(), max_line_width - opts_prefix_size, opts_prefix_size);
                out << '\n';
            }
            out << '\n';
        }
    }

protected:
    void append_opts(Opts&& opts) {
        _assert_uniq_opts_name(opts->get_name(), opts_.size());
        opts_.emplace_back(std::move(opts));
    }

private:
    FinalOptions& _final_opts() {
        return *static_cast<FinalOptions*>(this);
    }

    void _assert_uniq_short_name(char sn, NameRef&& ref) {
        if (sn == '\0' || _has_short_name(sn)) {
            throw std::invalid_argument(std::string{"duplicate short name '"} + sn + "'");
        }
        short_names_.emplace(sn, ref);
    }

    void _assert_uniq_long_name(const std::string_view& ln, NameRef&& ref) {
        if (_has_long_name(ln)) {
            throw std::invalid_argument(std::string{"duplicate long name '"} + std::string{ln} + "'");
        }
        long_names_.emplace(ln, ref);
    }

    void _assert_uniq_opts_name(const std::string_view& name, size_t idx) {
        if (_has_opts(name)) {
            throw std::invalid_argument(std::string{"duplicate subcommand name '"} + std::string{name} + "'");
        }
        opt_names_.emplace(name, idx);
    }

    bool _has_short_name(char sn) const {
        return short_names_.find(sn) != short_names_.end();
    }

    bool _has_long_name(const std::string_view& ln) const {
        return long_names_.find(ln) != long_names_.end();
    }

    bool _has_opts(const std::string_view& name) const {
        return opt_names_.find(name) != opt_names_.end();
    }

private:
    std::vector<Opts> opts_;
    std::vector<Argument> arguments_;
    std::vector<Flag> flags_;
    std::map<char, NameRef> short_names_;
    std::map<std::string_view, NameRef> long_names_;
    std::map<std::string_view, size_t> opt_names_;
};

}  // namespace xdx::cliopts
