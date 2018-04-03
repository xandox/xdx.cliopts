#pragma once

#include <string>
#include <string_view>

namespace xdx::cliopts
{

class FlagBase
{
public:
    FlagBase(const std::string_view& long_name, const std::string_view& description)
        : long_name_{long_name}
        , description_{description} {
    }

    FlagBase(char short_name, const std::string_view& description)
        : short_name_{short_name}
        , description_{description} {
    }

    FlagBase(char short_name, const std::string_view& long_name, const std::string_view& description)
        : short_name_{short_name}
        , long_name_{long_name}
        , description_{description} {
    }

    char short_name() const {
        return short_name_;
    }

    std::string_view long_name() const {
        return long_name_;
    }

    std::string_view get_description() const {
        return description_;
    }

    virtual void set_found() = 0;

private:
    char short_name_ = '\0';
    bool is_required_ = false;
    std::string long_name_;
    std::string description_;
};

class Flag : public FlagBase
{
public:
    Flag(const std::string_view& long_name, const std::string_view& description)
        : FlagBase{long_name, description} {
    }

    Flag(char short_name, const std::string_view& description)
        : FlagBase{short_name, description} {
    }

    Flag(char short_name, const std::string_view& long_name, const std::string_view& description)
        : FlagBase{short_name, long_name, description} {
    }

    void set_found() final {
        was_ = true;
    }

private:
    bool was_;
};

class FlagCount : public FlagBase
{
public:
    FlagCount(const std::string_view& long_name, const std::string_view& description)
        : FlagBase{long_name, description} {
    }

    FlagCount(char short_name, const std::string_view& description)
        : FlagBase{short_name, description} {
    }

    FlagCount(char short_name, const std::string_view& long_name, const std::string_view& description)
        : FlagBase{short_name, long_name, description} {
    }

    void set_found() final {
        was_ += 1;
    }

private:
    size_t was_ = 0;
};

}  // namespace xdx::cliopts
