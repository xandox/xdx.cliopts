#pragma once

#include <cassert>
#include <string_view>

namespace xdx::cliopts
{

class Argv
{
public:
    Argv(int argc, const char** argv)
        : cmd_{argv[0]}
        , argc_(argc)
        , argv_(argv) {

        shift_from(0);
    }

    std::string_view cmd() const {
        return cmd_;
    }

    size_t size() const {
        return static_cast<size_t>(argc_);
    }

    bool empty() const {
        return size() == 0;
    }

    const char* operator[](size_t idx) {
        assert(idx < size());
        return argv_[idx];
    }

    const char* const* begin() const {
        return argv_;
    }

    const char* const* end() const {
        return argv_ + argc_;
    }

    void erase(size_t idx) {
        shift_from(static_cast<int>(idx));
    }

private:
    void shift_from(int idx) {
        for (int i = idx + 1; i < argc_; ++i) {
            argv_[i - 1] = argv_[i];
        }

        argc_ -= (idx + 1);
    }

private:
    std::string_view cmd_;
    int argc_;
    const char** argv_;
};

}  // namespace xdx::cliopts
