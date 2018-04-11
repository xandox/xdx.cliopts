#pragma once

#include <memory>
#include <vector>

namespace xdx::cliopts
{

struct iFlag;
struct iArgument;

struct iOptions
{
    using FlagPtr = std::shared_ptr<iFlag>;
    using FlagCPtr = std::shared_ptr<const iFlag>;
    using ArgumentPtr = std::shared_ptr<iArgument>;
    using ArgumentCPtr = std::shared_ptr<const iArgument>;
    using SubcommandPtr = std::shared_ptr<iOptions>;
    using SubcommandCPtr = std::shared_ptr<const iOptions>;

    virtual ~iOptions() = default;
    virtual std::string_view get_name() const noexcept = 0;
    virtual std::string_view get_description() const noexcept = 0;

    virtual size_t flags_count() const noexcept = 0;
    virtual size_t arguments_count() const noexcept = 0;
    virtual size_t subcommands_count() const noexcept = 0;

    virtual FlagCPtr get_flag(size_t idx) const noexcept = 0;
    virtual ArgumentCPtr get_argument(size_t idx) const noexcept = 0;
    virtual SubcommandCPtr get_subcommand(size_t idx) const noexcept = 0;

    virtual void add(FlagPtr&& flag) = 0;
    virtual void add(ArgumentPtr&& arg) = 0;
    virtual void add(SubcommandPtr&& sub) = 0;
};

class Options : public iOptions
{
public:
    Options(const std::string_view& name, const std::string_view& description);
    ~Options() = default;
    std::string_view get_name() const noexcept override;
    std::string_view get_description() const noexcept override;
    size_t flags_count() const noexcept override;
    size_t arguments_count() const noexcept override;
    size_t subcommands_count() const noexcept override;
    FlagCPtr get_flag(size_t idx) const noexcept override;
    ArgumentCPtr get_argument(size_t idx) const noexcept override;
    SubcommandCPtr get_subcommand(size_t idx) const noexcept override;
    void add(FlagPtr&& flag) override;
    void add(ArgumentPtr&& arg) override;
    void add(SubcommandPtr&& sub) override;

private:
    void _assert_short_name(char ch);
    void _assert_long_name(const std::string_view& lname);
    void _assert_sub_name(const std::string_view& name);

private:
    std::string_view name_;
    std::string_view description_;
    std::vector<FlagPtr> flags_;
    std::vector<ArgumentPtr> arguments_;
    std::vector<SubcommandPtr> subcommands_;
};

}  // namespace xdx::cliopts
