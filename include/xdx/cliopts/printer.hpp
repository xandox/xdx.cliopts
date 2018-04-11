#pragma once

#include <memory>
#include <ostream>

namespace xdx::cliopts
{

class iOptions;

class Printer
{
public:
    Printer(const std::shared_ptr<const iOptions>& opts)
        : opts_(opts) {
    }

    void print_short(std::ostream& out);
    void print_long(std::ostream& out);

private:
    std::shared_ptr<const iOptions> opts_;
};

}  // namespace xdx::cliopts
