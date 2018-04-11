#include <xdx/cliopts/tokenizer.hpp>

namespace xdx::cliopts
{

constexpr char arg_prefix = '-';
constexpr char val_split = '=';

using Token = Tokenizer::Token;

std::pair<bool, Token> Tokenizer::next() {
    if (char_idx_ < 0) {
        entry_idx_ += 1;
        if (entry_idx_ >= argv_.size()) {
            return {false, {TokenType::Unknown, {}}};
        }
        current_entry_ = argv_[entry_idx_];
        char_idx_ = 0;
        current_token_ = TokenType::Unknown;
    }

    if (current_token_ == TokenType::Unknown) {
        if (current_entry_[char_idx_] != arg_prefix) {
            auto tail = current_entry_.substr(char_idx_);
            char_idx_ = -1;
            return {true, {TokenType::None, tail}};
        }

        char_idx_ += 1;
        if (current_entry_[char_idx_] != arg_prefix) {
            current_token_ = TokenType::Short;
            return {true, {TokenType::Short, current_entry_[char_idx_]}};
        }

        char_idx_ += 1;

        auto split_idx = current_entry_.find(val_split, char_idx_);
        if (split_idx != current_entry_.npos) {
            auto name = current_entry_.substr(char_idx_, split_idx - char_idx_);
            char_idx_ = split_idx += 1;
            current_token_ = TokenType::None;
            return {true, {TokenType::Long, name}};
        }

        auto name = current_entry_.substr(char_idx_);
        char_idx_ = -1;
        return {true, {TokenType::Long, name}};
    }

    if (current_token_ == TokenType::None) {
        current_token_ = TokenType::Unknown;
        auto tail = current_entry_.substr(char_idx_);
        char_idx_ = -1;
        return {true, {TokenType::None, tail}};
    }

    if (current_token_ == TokenType::Short) {
        char_idx_ += 1;
        if (char_idx_ >= current_entry_.size()) {
            char_idx_ = -1;
            return next();
        }
        return {true, {TokenType::Short, current_entry_[char_idx_]}};
    }

    return {false, {}};
}

}  // namespace xdx::cliopts
