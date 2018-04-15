#pragma once

#include <xdx/cliopts/argv.hpp>
#include <xdx/cliopts/error.hpp>
#include <xdx/cliopts/options.hpp>

#include <iostream>
#include <vector>

namespace xdx::cliopts
{

class Parser
{
public:
    Parser(const OptionsPtr& options)
        : options_{options} {
    }

    using SubcommandsPath = std::vector<std::string_view>;
    using UnparsedArguments = std::vector<std::string_view>;
    struct ProcessResult
    {
        std::error_code error;
        SubcommandsPath subcommand_path;
        UnparsedArguments unparsed_arguments;
    };

    ProcessResult process(Argv&& argv, std::ostream& errout = std::cerr);

private:
    OptionsPtr options_;
};

inline Parser::ProcessResult parse_argv(const OptionsPtr& options, int argc, const char** argv) {
    Parser parser(options);
    return parser.process({argc, argv});
}

}  // namespace xdx::cliopts
