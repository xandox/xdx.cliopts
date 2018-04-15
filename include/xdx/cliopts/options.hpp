#pragma once

#include <memory>
#include <vector>

namespace xdx::cliopts
{

struct iFlag;
struct iArgument;
class FlagCount;

template <class Type>
class Argument;

template <class Type>
class ArgumentList;

struct iOptions
{
    using FlagPtr = std::shared_ptr<iFlag>;
    using FlagCountPtr = std::shared_ptr<FlagCount>;
    using ArgumentPtr = std::shared_ptr<iArgument>;
    using SubcommandPtr = std::shared_ptr<iOptions>;
    template <class Type>
    using TypedArgumentPtr = std::shared_ptr<Argument<Type>>;
    template <class Type>
    using TypedArgumentListPtr = std::shared_ptr<ArgumentList<Type>>;
    // using ArgumentListPtr = std::shared_ptr<ArgumentList>;

    virtual ~iOptions() = default;
    virtual std::string_view get_name() const noexcept = 0;
    virtual std::string_view get_description() const noexcept = 0;

    virtual size_t flags_count() const noexcept = 0;
    virtual size_t arguments_count() const noexcept = 0;
    virtual size_t subcommands_count() const noexcept = 0;

    virtual FlagPtr get_flag(size_t idx) const noexcept = 0;
    virtual ArgumentPtr get_argument(size_t idx) const noexcept = 0;
    virtual SubcommandPtr get_subcommand(size_t idx) const noexcept = 0;

    virtual FlagPtr find_flag(char short_name) const noexcept = 0;
    virtual FlagPtr find_flag(std::string_view long_name) const noexcept = 0;

    virtual FlagCountPtr find_flag_count(char short_name) const noexcept = 0;
    virtual FlagCountPtr find_flag_count(std::string_view long_name) const noexcept = 0;

    virtual ArgumentPtr find_argument(char short_name) const noexcept = 0;
    virtual ArgumentPtr find_argument(std::string_view long_name) const noexcept = 0;

    template <class Type>
    TypedArgumentPtr<Type> find_typed_argument(char short_name) const noexcept {
        return std::dynamic_pointer_cast<Argument<Type>>(find_argument(short_name));
    }

    template <class Type>
    TypedArgumentPtr<Type> find_typed_argument(std::string_view long_name) const noexcept {
        return std::dynamic_pointer_cast<Argument<Type>>(find_argument(long_name));
    }

    template <class Type>
    TypedArgumentListPtr<Type> find_typed_argument_list(char short_name) const noexcept {
        return std::dynamic_pointer_cast<ArgumentList<Type>>(find_argument(short_name));
    }

    template <class Type>
    TypedArgumentListPtr<Type> find_typed_argument_list(std::string_view long_name) const noexcept {
        return std::dynamic_pointer_cast<ArgumentList<Type>>(find_argument(long_name));
    }

    virtual SubcommandPtr find_subcommand(std::string_view name) const noexcept = 0;

    virtual void add(FlagPtr&& flag) = 0;
    virtual void add(ArgumentPtr&& arg) = 0;
    virtual void add(SubcommandPtr&& sub) = 0;

    virtual void reset_to_default() noexcept = 0;
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
    FlagPtr get_flag(size_t idx) const noexcept override;
    ArgumentPtr get_argument(size_t idx) const noexcept override;
    SubcommandPtr get_subcommand(size_t idx) const noexcept override;
    void add(FlagPtr&& flag) override;
    void add(ArgumentPtr&& arg) override;
    void add(SubcommandPtr&& sub) override;

    FlagPtr find_flag(char short_name) const noexcept override;
    FlagPtr find_flag(std::string_view long_name) const noexcept override;
    FlagCountPtr find_flag_count(char short_name) const noexcept override;
    FlagCountPtr find_flag_count(std::string_view long_name) const noexcept override;
    ArgumentPtr find_argument(char short_name) const noexcept override;
    ArgumentPtr find_argument(std::string_view long_name) const noexcept override;
    SubcommandPtr find_subcommand(std::string_view name) const noexcept override;

    void reset_to_default() noexcept override;

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

using OptionsPtr = std::shared_ptr<iOptions>;

}  // namespace xdx::cliopts
