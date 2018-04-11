#include <gtest/gtest.h>
#include <xdx/cliopts/tokenizer.hpp>

using namespace xdx::cliopts;

TEST(xdx_cliopts_parser_tests, tokenize_simple) {
    const char* arguments[] = {"program", "a", "b", "c"};
    Argv argv((int)std::size(arguments), arguments);
    Tokenizer parser(argv);

    {
        auto [result, token] = parser.next();
        ASSERT_TRUE(result);
        ASSERT_EQ(Tokenizer::TokenType::None, token.type);
        ASSERT_EQ("a", std::get<std::string_view>(token.value));
    }

    {
        auto [result, token] = parser.next();
        ASSERT_TRUE(result);
        ASSERT_EQ(Tokenizer::TokenType::None, token.type);
        ASSERT_EQ("b", std::get<std::string_view>(token.value));
    }

    {
        auto [result, token] = parser.next();
        ASSERT_TRUE(result);
        ASSERT_EQ(Tokenizer::TokenType::None, token.type);
        ASSERT_EQ("c", std::get<std::string_view>(token.value));
    }

    {
        auto [result, token] = parser.next();
        ASSERT_FALSE(result);
        ASSERT_EQ(Tokenizer::TokenType::Unknown, token.type);
    }
}

void test_short(int argc, const char** argv) {
    Argv args(argc, argv);
    Tokenizer parser(args);

    {
        auto [result, token] = parser.next();
        ASSERT_TRUE(result);
        ASSERT_EQ(Tokenizer::TokenType::Short, token.type);
        ASSERT_EQ('a', std::get<char>(token.value));
    }

    {
        auto [result, token] = parser.next();
        ASSERT_TRUE(result);
        ASSERT_EQ(Tokenizer::TokenType::Short, token.type);
        ASSERT_EQ('b', std::get<char>(token.value));
    }

    {
        auto [result, token] = parser.next();
        ASSERT_TRUE(result);
        ASSERT_EQ(Tokenizer::TokenType::Short, token.type);
        ASSERT_EQ('c', std::get<char>(token.value));
    }

    {
        auto [result, token] = parser.next();
        ASSERT_FALSE(result);
        ASSERT_EQ(Tokenizer::TokenType::Unknown, token.type);
    }
}

TEST(xdx_cliopts_parser_tests, tokenize_short_all_separted) {
    const char* arguments[] = {"program", "-a", "-b", "-c"};
    ASSERT_NO_FATAL_FAILURE(test_short(static_cast<int>(std::size(arguments)), arguments));
}

TEST(xdx_cliopts_parser_tests, tokenize_short_all_in_one) {
    const char* arguments[] = {"program", "-abc"};
    ASSERT_NO_FATAL_FAILURE(test_short(static_cast<int>(std::size(arguments)), arguments));
}

TEST(xdx_cliopts_parser_tests, tokenize_short_mixed) {
    const char* arguments[] = {"program", "-ab", "-c"};
    ASSERT_NO_FATAL_FAILURE(test_short(static_cast<int>(std::size(arguments)), arguments));
}

TEST(xdx_cliopts_parser_tests, tokenize_long_and_none) {
    const char* arguments[] = {"program", "--long-name", "separate-value", "--long-name-2=value", "-mistype=value"};
    Argv args(static_cast<int>(std::size(arguments)), arguments);
    Tokenizer parser(args);

    {
        auto [result, token] = parser.next();
        ASSERT_TRUE(result);
        ASSERT_EQ(Tokenizer::TokenType::Long, token.type);
        ASSERT_EQ("long-name", std::get<std::string_view>(token.value));
    }

    {
        auto [result, token] = parser.next();
        ASSERT_TRUE(result);
        ASSERT_EQ(Tokenizer::TokenType::None, token.type);
        ASSERT_EQ("separate-value", std::get<std::string_view>(token.value));
    }

    {
        auto [result, token] = parser.next();
        ASSERT_TRUE(result);
        ASSERT_EQ(Tokenizer::TokenType::Long, token.type);
        ASSERT_EQ("long-name-2", std::get<std::string_view>(token.value));
    }

    {
        auto [result, token] = parser.next();
        ASSERT_TRUE(result);
        ASSERT_EQ(Tokenizer::TokenType::None, token.type);
        ASSERT_EQ("value", std::get<std::string_view>(token.value));
    }

    for (char ch : "mistype=value") {
        if (ch == '\0')
            break;

        auto [result, token] = parser.next();
        ASSERT_TRUE(result);
        ASSERT_EQ(Tokenizer::TokenType::Short, token.type) << "'" << ch << "'";
        ASSERT_EQ(ch, std::get<char>(token.value));
    }

    {
        auto [result, token] = parser.next();
        ASSERT_FALSE(result);
        ASSERT_EQ(Tokenizer::TokenType::Unknown, token.type);
    }
}
