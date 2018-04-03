#include <xdx/cliopts/options.hpp>

namespace xdx::cliopts
{

class Subcommand : Options<Subcommand>
{
public:
    Subcommand(const std::string_view& name)
        : name_(name) {
    }

private:
    std::string_view name_;
};

}  // namespace xdx::cliopts
