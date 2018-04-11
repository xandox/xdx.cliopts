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

    void print_long_help(std::ostream& out) const override {
        print_short_help(out);
        Options<Programm>::print_long_help(out);
    }

    void print_short_help(std::ostream& out) const override {
        out << "usage: " << argv_.cmd() << ' ';
        Options<Programm>::print_short_help(out);
        out << '\n';
    }

private:
    Argv argv_;
};

}  // namespace xdx::cliopts
