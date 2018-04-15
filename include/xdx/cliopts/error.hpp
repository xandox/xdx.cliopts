#pragma once

#include <system_error>

namespace xdx::cliopts
{

enum class ProcessingArgumentsError : int
{
    UnknonwSwitcher = 1,
    ExpectingValue = 2,
    WrongValueType = 3,
    UnknownSubcommand = 4,
    RequiredArgument = 5,
};

class ProcessingArgumentsErrorCategory : public std::error_category
{
public:
    const char* name() const noexcept override;
    std::string message(int val) const override;
};

std::error_code make_error_code(ProcessingArgumentsError error);
std::error_condition make_error_condition(ProcessingArgumentsError error);

}  // namespace xdx::cliopts
