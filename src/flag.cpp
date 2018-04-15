#include <xdx/cliopts/flag.hpp>

namespace xdx::cliopts
{

FlagBase::FlagBase(const std::string_view& long_name, const std::string_view& description)
    : long_name_{long_name}
    , description_{description} {
}

FlagBase::FlagBase(char short_name, const std::string_view& description)
    : short_name_{short_name}
    , description_{description} {
}

FlagBase::FlagBase(char short_name, const std::string_view& long_name, const std::string_view& description)
    : short_name_{short_name}
    , long_name_{long_name}
    , description_{description} {
}

char FlagBase::get_short_name() const noexcept {
    return short_name_;
}

std::string_view FlagBase::get_long_name() const noexcept {
    return long_name_;
}

std::string_view FlagBase::get_description() const noexcept {
    return description_;
}

Flag::Flag(const std::string_view& long_name, const std::string_view& description)
    : FlagBase{long_name, description} {
}

Flag::Flag(char short_name, const std::string_view& description)
    : FlagBase{short_name, description} {
}

Flag::Flag(char short_name, const std::string_view& long_name, const std::string_view& description)
    : FlagBase{short_name, long_name, description} {
}

void Flag::set_found() noexcept {
    was_ = true;
}

bool Flag::is_set() const noexcept {
    return was_;
}

void Flag::reset_to_default() noexcept {
    was_ = false;
}

FlagCount::FlagCount(const std::string_view& long_name, const std::string_view& description)
    : FlagBase{long_name, description} {
}

FlagCount::FlagCount(char short_name, const std::string_view& description)
    : FlagBase{short_name, description} {
}

FlagCount::FlagCount(char short_name, const std::string_view& long_name, const std::string_view& description)
    : FlagBase{short_name, long_name, description} {
}

void FlagCount::set_found() noexcept {
    was_ += 1;
}

bool FlagCount::is_set() const noexcept {
    return was_ != 0;
}

size_t FlagCount::get_count() const noexcept {
    return was_;
}

void FlagCount::reset_to_default() noexcept {
    was_ = 0;
}

}  // namespace xdx::cliopts
