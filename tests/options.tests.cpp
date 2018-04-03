#include <gtest/gtest.h>

#include <xdx/cliopts/options.hpp>
#include <xdx/cliopts/parser.hpp>

using namespace xdx::cliopts;

struct TestOpts : public Options<TestOpts>
{
    std::string_view get_name() const override {
        return std::string_view{};
    }

    std::string_view get_description() const override {
        return std::string_view{};
    }
};

TEST(xdx_cliopts_options_tests, empty_list) {
    using namespace std;
    TestOpts opts;
    opts.flag('f', "flag"sv, "some flag description"sv);
    opts.flag('l',
              "some flag description some flag description some flag description some flag description some flag "
              "description some flag description"sv);
    opts.flag("flag2"sv, "some flag description"sv);
    opts.arg<int>('a', "arg"sv, "some argument"sv, "INT"sv);
    opts.arg<int>('b', "some argument"sv, "INT"sv);
    opts.arg<int>("argument"sv,
                  "some argument description some argument description some argument description some argument "
                  "description some argument description some argument description some argument description some "
                  "argument description"sv,
                  "INT"sv);
    opts.print_help(std::cout);
}
