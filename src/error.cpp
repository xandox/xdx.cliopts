#include <xdx/cliopts/error.hpp>

namespace xdx::cliopts
{

static std::error_category& get_error_category() {
    static ProcessingArgumentsErrorCategory _category;
    return _category;
}

const char* ProcessingArgumentsErrorCategory::name() const noexcept {
    return "xdx::cliopts::process_error_category";
}

std::string ProcessingArgumentsErrorCategory::message(int val) const {
    switch (static_cast<ProcessingArgumentsError>(val)) {
        case ProcessingArgumentsError::ExpectingValue:
            return "Expecting value";
        case ProcessingArgumentsError::UnknonwSwitcher:
            return "Unknown switcher";
        case ProcessingArgumentsError::WrongValueType:
            return "Provided value has wrong type";
    }
}

std::error_code make_error_code(ProcessingArgumentsError error) {
    return std::error_code(static_cast<int>(error), get_error_category());
}

std::error_condition make_error_condition(ProcessingArgumentsError error) {
    return std::error_condition(static_cast<int>(error), get_error_category());
}

}  // namespace xdx::cliopts
