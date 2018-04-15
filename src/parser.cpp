#include <xdx/cliopts/argument.hpp>
#include <xdx/cliopts/flag.hpp>
#include <xdx/cliopts/parser.hpp>
#include <xdx/cliopts/tokenizer.hpp>

#include <iostream>

namespace xdx::cliopts
{

Parser::ProcessResult Parser::process(Argv&& argv, std::ostream& errout) {
    ProcessResult result;

    Options::SubcommandPtr current_command = options_;
    Tokenizer tokenizer(argv);

    Options::ArgumentPtr current_argument;

    bool not_end = false;
    Tokenizer::Token token;

    auto output_argument = [&errout](const auto& arg) {
        if (!arg->get_long_name().empty()) {
            errout << "'--" << arg->get_long_name() << '\'';
        } else {
            errout << "'-" << arg->get_short_name() << '\'';
        }
    };

    for (std::tie(not_end, token) = tokenizer.next(); not_end; std::tie(not_end, token) = tokenizer.next()) {
        assert(token.type != Tokenizer::TokenType::Unknown && "must be here");

        if (current_argument && token.type != Tokenizer::TokenType::None) {
            errout << "Argument ";
            output_argument(current_argument);
            errout << " expected value" << std::endl;
            result.error = make_error_code(ProcessingArgumentsError::ExpectingValue);
            return result;
        }

        switch (token.type) {
            case Tokenizer::TokenType::Short: {
                auto flag = current_command->find_flag(token.get_short());
                if (flag) {
                    flag->set_found();
                } else {
                    auto argument = current_command->find_argument(token.get_short());
                    if (argument) {
                        current_argument = argument;
                    } else {
                        errout << "Unknown switcher: '-" << token.get_short() << "'" << std::endl;
                        result.error = make_error_code(ProcessingArgumentsError::UnknonwSwitcher);
                        return result;
                    }
                }
            } break;
            case Tokenizer::TokenType::Long: {
                auto flag = current_command->find_flag(token.get_long());
                if (flag) {
                    flag->set_found();
                } else {
                    auto argument = current_command->find_argument(token.get_long());
                    if (argument) {
                        current_argument = argument;
                    } else {
                        errout << "Unknown switcher: '-" << token.get_short() << "'" << std::endl;
                        result.error = make_error_code(ProcessingArgumentsError::UnknonwSwitcher);
                        return result;
                    }
                }
            } break;
            case Tokenizer::TokenType::None: {
                if (current_argument) {
                    auto [success, error_message] = current_argument->set_string_value(token.get_long());
                    if (!success) {
                        errout << error_message << std::endl;
                        result.error = make_error_code(ProcessingArgumentsError::WrongValueType);
                        return result;
                    }
                    current_argument.reset();
                } else {
                    auto command = current_command->find_subcommand(token.get_long());
                    if (!command) {
                        result.unparsed_arguments.emplace_back(token.get_long());
                    }

                    if (!result.unparsed_arguments.empty()) {
                        result.unparsed_arguments.emplace_back(token.get_long());
                        continue;
                    }

                    for (size_t aidx = 0; aidx < current_command->arguments_count(); ++aidx) {
                        const auto argument = current_command->get_argument(aidx);
                        if (argument->is_required() && !argument->has_value()) {
                            errout << "Argument ";
                            output_argument(argument);
                            errout << " required value" << std::endl;
                            result.error = make_error_code(ProcessingArgumentsError::RequiredArgument);
                        }
                    }

                    if (result.error) {
                        return result;
                    }

                    result.subcommand_path.push_back(token.get_long());
                    current_command = command;
                }
            } break;
            case Tokenizer::TokenType::Unknown:
                continue;
        }
    }

    for (size_t aidx = 0; aidx < current_command->arguments_count(); ++aidx) {
        const auto argument = current_command->get_argument(aidx);
        if (argument->is_required() && !argument->has_value()) {
            errout << "Argument ";
            output_argument(argument);
            errout << " required value" << std::endl;
            result.error = make_error_code(ProcessingArgumentsError::RequiredArgument);
        }
    }

    return result;
}

}  // namespace xdx::cliopts
