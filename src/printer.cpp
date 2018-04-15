#include <xdx/cliopts/argument.hpp>
#include <xdx/cliopts/flag.hpp>
#include <xdx/cliopts/options.hpp>
#include <xdx/cliopts/printer.hpp>

#include <iomanip>

namespace xdx::cliopts
{

namespace
{

template <class Source>
void short_print_name(std::ostream& out, const Source& source) {
    out << '-';
    if (source->get_short_name() != '\0') {
        out << source->get_short_name();
    } else {
        out << '-' << source->get_long_name();
    }
}

void short_print_flag(std::ostream& out, const iOptions::FlagPtr& flag) {
    out << '[';
    short_print_name(out, flag);
    if (std::dynamic_pointer_cast<const FlagCount>(flag) != nullptr) {
        out << '|';
        short_print_name(out, flag);
        out << "...";
    }
    out << ']';
    out << ' ';
}

void short_print_argument(std::ostream& out, const iOptions::ArgumentPtr& argument) {
    if (!argument->is_required()) {
        out << '[';
    }
    short_print_name(out, argument);
    out << ' ' << argument->get_type_name();
    if (argument->is_many_values()) {
        out << '|';
        short_print_name(out, argument);
        out << ' ' << argument->get_type_name();
        out << "...";
    }
    if (!argument->is_required()) {
        out << ']';
    }
    out << ' ';
}

}  // namespace

void Printer::print_short(std::ostream& out) {
    for (size_t i = 0; i < opts_->flags_count(); ++i) {
        short_print_flag(out, opts_->get_flag(i));
    }

    for (size_t i = 0; i < opts_->arguments_count(); ++i) {
        short_print_argument(out, opts_->get_argument(i));
    }

    if (opts_->subcommands_count() > 0) {
        out << '[';

        for (size_t i = 0; i < opts_->subcommands_count(); ++i) {
            const auto& o = opts_->get_subcommand(i);
            if (i != 0) {
                out << '|';
            }
            out << o->get_name();
        }

        out << ']';
    };
}

namespace
{

constexpr static char COUNT_FLAG[] = "COUNT";

struct LongNamePrint
{
    std::shared_ptr<const iOptions> options;
    std::ostream& out;
    size_t long_name_width = 0;
    size_t sub_name_width = 0;
    size_t type_width = 0;
    size_t max_width = 80;
    size_t description_shift = 4;
    size_t description_width = 0;
    size_t shift = 4;
    bool has_short_names = false;

    template <class Source>
    void consider_names(const Source& source) {
        if (source->get_short_name() != '\0') {
            has_short_names = true;
        }
        if (!source->get_long_name().empty()) {
            long_name_width = std::max(long_name_width, source->get_long_name().size());
        }
    }

    LongNamePrint(std::ostream& out, const std::shared_ptr<const iOptions>& options)
        : options(options)
        , out(out) {

        for (size_t i = 0; i < options->flags_count(); ++i) {
            consider_names(options->get_flag(i));
        }

        for (size_t i = 0; i < options->arguments_count(); ++i) {
            const auto argument = options->get_argument(i);
            consider_names(argument);
            type_width = std::max(type_width, argument->get_type_name().size());
        }

        type_width = std::max(type_width, std::size(COUNT_FLAG) - 1);

        description_shift += has_short_names ? 3 : 0;
        description_shift += long_name_width != 0 ? long_name_width + 3 : 0;
        description_shift += type_width != 0 ? type_width + 1 : 0;
        description_width = max_width - description_shift;

        for (size_t i = 0; i < options->subcommands_count(); ++i) {
            const auto option = options->get_subcommand(i);
            sub_name_width = std::max(sub_name_width, option->get_name().size());
        }
    }

    void print_shift(size_t shift) {
        out << std::setw(shift) << ' ';
    }

    void new_line() {
        out << '\n';
    }

    void print_field(size_t width, std::string_view field) {
        out << std::setw(width) << std::left << field;
    }

    void print_description(std::string_view description, size_t width, size_t shift) {
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
            print_shift(shift);
            description = description.substr(space + 1);
        }
    }

    template <class Source>
    void print_short_name(const Source& src) {
        if (has_short_names) {
            if (src->get_short_name() != '\0') {
                out << '-' << src->get_short_name();
            } else {
                out << ' ' << ' ';
            }
        }
    };

    template <class Source>
    void print_long_name(const Source& src) {
        if (long_name_width == 0) {
            print_shift(1);
            return;
        }

        if (!src->get_long_name().empty()) {
            out << ((src->get_short_name() != '\0') ? '|' : ' ');
            out << '-' << '-';
            print_field(long_name_width, src->get_long_name());
        } else {
            print_shift(long_name_width + 3);
        }
    };

    void print_type(const std::string_view& type) {
        print_field(type_width, type.empty() ? " " : type);
    }

    void print_flags() {
        out << "FLAGS:\n";
        for (size_t i = 0; i < options->flags_count(); ++i) {
            const auto flag = options->get_flag(i);
            print_shift(shift);
            print_short_name(flag);
            print_long_name(flag);
            print_shift(1);
            print_type(flag->is_countable() ? COUNT_FLAG : "");
            print_shift(1);
            print_description(flag->get_description(), description_width, description_shift);
            new_line();
        }
    }

    void print_arguments() {
        out << "ARGUMENTS:\n";
        for (size_t i = 0; i < options->arguments_count(); ++i) {
            const auto argument = options->get_argument(i);
            print_shift(argument->is_required() ? shift - 1 : shift);
            if (argument->is_required()) {
                out << '*';
            }
            print_short_name(argument);
            print_long_name(argument);
            print_shift(1);
            print_type(argument->get_type_name());
            print_shift(1);
            print_description(argument->get_description(), description_width, description_shift);
            new_line();
            if (argument->has_default_value()) {
                print_shift(description_shift);
                out << "default: " << argument->get_default_value();
                new_line();
            }
        }
    }

    void print_subcommands() {
        out << "SUBCOMMANDS:\n";
        for (size_t i = 0; i < options->subcommands_count(); ++i) {
            const auto subcommand = options->get_subcommand(i);
            print_shift(shift);
            print_field(sub_name_width, subcommand->get_name());
            print_shift(1);
            print_description(subcommand->get_description(), max_width - (sub_name_width + 1), sub_name_width + 1);
            new_line();
        }
    }
};

}  // namespace

void Printer::print_long(std::ostream& out) {
    LongNamePrint print(out, opts_);

    if (opts_->flags_count() != 0) {
        print.print_flags();
        print.new_line();
    }

    if (opts_->arguments_count() != 0) {
        print.print_arguments();
        print.new_line();
    }

    if (opts_->subcommands_count() != 0) {
        print.print_subcommands();
        print.new_line();
    }
}

}  // namespace xdx::cliopts
