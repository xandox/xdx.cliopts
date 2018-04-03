#pragma once

#include <xdx/cliopts/argv.hpp>
#include <xdx/cliopts/options.hpp>

namespace xdx::cliopts
{

class Programm : public Options<Programm>
{
public:
    Programm(Argv&& argv)
        : argv_(std::move(argv)) {
    }

    void print_help(std::ostream& out) const override {
        out << "usage: " << argv_.cmd();
        if (has_flags()) {
            out << " [FLAGS]";
        }

        if (has_arguments()) {
            out << " [ARGUMENTS]";
        }

        if (has_subcommands()) {
            out << " [COMMAND]";
        }

        Options<Programm>::print_help(out);
    }

private:
    Argv argv_;
};

}  // namespace xdx::cliopts
