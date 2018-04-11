#include <xdx/cliopts/argv.hpp>

#include <variant>

namespace xdx::cliopts
{

class Tokenizer
{
public:
    enum TokenType
    {
        Unknown,
        Short,
        Long,
        None,
    };

    struct Token
    {
        TokenType type = TokenType::Unknown;
        std::variant<char, std::string_view> value;
    };

    Tokenizer(Argv& argv)
        : argv_(argv) {
    }

    std::pair<bool, Token> next();

private:
    Argv& argv_;
    TokenType current_token_;
    int entry_idx_ = -1;
    int char_idx_ = -1;
    std::string_view current_entry_;
};

}  // namespace xdx::cliopts
