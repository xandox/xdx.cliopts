#include <gtest/gtest.h>

#include <xdx/cliopts/builder.hpp>
#include <xdx/cliopts/options.hpp>
#include <xdx/cliopts/printer.hpp>

using namespace xdx::cliopts;

TEST(xdx_cliopts_options_tests, empty_list) {
    using namespace std;
    Builder builder("test", "test options");
    builder.flag('0', "flag"sv, "some flag description"sv);
    builder.flag('l',
                 "some flag description some flag description some flag description some flag description some flag "
                 "description some flag description"sv);

    builder.flag("flag2"sv, "some flag description"sv);
    builder.flag_count('v', "set verbosity level"sv);
    builder.argument_list<std::string>('f', "file"sv, "some argument"sv, "FILE"sv);
    builder.argument<int>('a', "arg"sv, "some argument"sv, "INT"sv);
    builder.argument<int>('b', "some argument"sv, "INT"sv);
    builder.argument<int>('c', "C"sv, "some int value"sv, "INT"sv, 10);
    builder.argument<int>("argument"sv,
                          "some argument description some argument description some argument description some argument "
                          "description some argument description some argument description some argument description "
                          "some "
                          "argument description"sv,
                          "INT"sv);

    Printer printer(builder.get_options());
    printer.print_short(std::cout);
    std::cout << std::endl;
    printer.print_long(std::cout);
    std::cout << std::endl;
}
