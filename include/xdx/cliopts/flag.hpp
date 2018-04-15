#pragma once

#include <string>
#include <string_view>

namespace xdx::cliopts
{

struct iFlag
{
    virtual ~iFlag() = default;
    virtual char get_short_name() const noexcept = 0;
    virtual std::string_view get_long_name() const noexcept = 0;
    virtual std::string_view get_description() const noexcept = 0;
    virtual bool is_countable() const noexcept = 0;
    virtual void set_found() noexcept = 0;
    virtual bool is_set() const noexcept = 0;
    virtual void reset_to_default() noexcept = 0;
};

class FlagBase : public iFlag
{
public:
    FlagBase(const std::string_view& long_name, const std::string_view& description);
    FlagBase(char short_name, const std::string_view& description);
    FlagBase(char short_name, const std::string_view& long_name, const std::string_view& description);

    char get_short_name() const noexcept override;
    std::string_view get_long_name() const noexcept override;
    std::string_view get_description() const noexcept override;

private:
    char short_name_ = '\0';
    bool is_required_ = false;
    std::string long_name_;
    std::string description_;
};

class Flag : public FlagBase
{
public:
    Flag(const std::string_view& long_name, const std::string_view& description);
    Flag(char short_name, const std::string_view& description);
    Flag(char short_name, const std::string_view& long_name, const std::string_view& description);

    void set_found() noexcept final;
    bool is_set() const noexcept final;
    void reset_to_default() noexcept final;

    bool is_countable() const noexcept {
        return false;
    }

private:
    bool was_;
};

class FlagCount : public FlagBase
{
public:
    FlagCount(const std::string_view& long_name, const std::string_view& description);
    FlagCount(char short_name, const std::string_view& description);
    FlagCount(char short_name, const std::string_view& long_name, const std::string_view& description);
    void set_found() noexcept final;
    bool is_set() const noexcept final;
    size_t get_count() const noexcept;
    void reset_to_default() noexcept final;

    bool is_countable() const noexcept {
        return true;
    }

private:
    size_t was_ = 0;
};

}  // namespace xdx::cliopts
