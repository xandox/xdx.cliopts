#include <gtest/gtest.h>

#include <xdx/cliopts/cliopts.hpp>

using namespace xdx::cliopts;

TEST(xdx_cliopts_parser_tests, empty_options) {
    const char* argv[] = {"test"};
    Builder builder("test", "test options");
    auto result = parse_argv(builder.get_options(), std::size(argv), argv);
    ASSERT_FALSE(static_cast<bool>(result.error));
}

TEST(xdx_cliopts_parser_tests, options_flags_and_flags_count) {
    auto builder = Builder("test", "test options")
                       .flag('s', "simple", "simple flag. just set or not")
                       .flag_count('c', "countable", "countable flag. counts how many times it set");
    {
        const char* argv[] = {"test"};
        auto result = parse_argv(builder.get_options(), std::size(argv), argv);
        ASSERT_FALSE(static_cast<bool>(result.error));

        auto options = builder.get_options();
        auto simple_flag = options->find_flag("simple");
        ASSERT_EQ(nullptr, options->find_flag_count("simple"));
        ASSERT_TRUE(simple_flag != nullptr);
        ASSERT_FALSE(simple_flag->is_set());
        auto countalbe_flag = options->find_flag_count("countable");
        ASSERT_TRUE(countalbe_flag != nullptr);
        ASSERT_FALSE(countalbe_flag->is_set());
        ASSERT_EQ(0, countalbe_flag->get_count());
        options->reset_to_default();
    }

    {
        const char* argv[] = {"test", "-sc", "-sc"};
        auto result = parse_argv(builder.get_options(), std::size(argv), argv);
        ASSERT_FALSE(static_cast<bool>(result.error));
        auto options = builder.get_options();
        auto simple_flag = options->find_flag("simple");
        ASSERT_EQ(nullptr, options->find_flag_count("simple"));
        ASSERT_TRUE(simple_flag != nullptr);
        ASSERT_TRUE(simple_flag->is_set());
        auto countalbe_flag = options->find_flag_count("countable");
        ASSERT_TRUE(countalbe_flag != nullptr);
        ASSERT_TRUE(countalbe_flag->is_set());
        ASSERT_EQ(2, countalbe_flag->get_count());
        options->reset_to_default();
    }
}

TEST(xdx_cliopts_parser_tests, options_default_arguments) {
    auto builder = Builder("test", "test options")
                       .argument('i', "input", "single int. will be overrided on second entries", 10)
                       .argument_list('l', "input-list", "list of int. return all entered values", 100);
    {
        const char* argv[] = {"test"};
        auto result = parse_argv(builder.get_options(), std::size(argv), argv);
        ASSERT_FALSE(static_cast<bool>(result.error));

        auto options = builder.get_options();
        auto single_arg = options->find_typed_argument<int>("input");
        ASSERT_TRUE(single_arg != nullptr);
        ASSERT_TRUE(options->find_typed_argument<int>("input-list") == nullptr);
        ASSERT_TRUE(options->find_typed_argument<short>("input") == nullptr);
        ASSERT_TRUE(single_arg->has_default_value());
        ASSERT_TRUE(single_arg->has_value());
        ASSERT_EQ(10, single_arg->get_value());

        auto list_arg = options->find_typed_argument_list<int>("input-list");
        ASSERT_TRUE(list_arg != nullptr);
        ASSERT_TRUE(list_arg->has_default_value());
        ASSERT_TRUE(list_arg->has_value());
        ASSERT_EQ(std::vector<int>{100}, list_arg->get_values());
        options->reset_to_default();
    }

    {
        const char* argv[] = {"test", "-i", "20", "-l", "30"};
        auto result = parse_argv(builder.get_options(), std::size(argv), argv);
        ASSERT_FALSE(static_cast<bool>(result.error));

        auto options = builder.get_options();
        auto single_arg = options->find_typed_argument<int>("input");
        ASSERT_TRUE(single_arg != nullptr);
        ASSERT_TRUE(options->find_typed_argument<int>("input-list") == nullptr);
        ASSERT_TRUE(options->find_typed_argument<short>("input") == nullptr);
        ASSERT_TRUE(single_arg->has_default_value());
        ASSERT_TRUE(single_arg->has_value());
        ASSERT_EQ(20, single_arg->get_value());

        auto list_arg = options->find_typed_argument_list<int>("input-list");
        ASSERT_TRUE(list_arg != nullptr);
        ASSERT_TRUE(list_arg->has_default_value());
        ASSERT_TRUE(list_arg->has_value());
        ASSERT_EQ(std::vector<int>{30}, list_arg->get_values());
        options->reset_to_default();
    }

    {
        const char* argv[] = {"test", "-i", "20", "-l", "30", "-i", "40", "--input-list=50"};
        auto result = parse_argv(builder.get_options(), std::size(argv), argv);
        ASSERT_FALSE(static_cast<bool>(result.error));

        auto options = builder.get_options();
        auto single_arg = options->find_typed_argument<int>("input");
        ASSERT_TRUE(single_arg != nullptr);
        ASSERT_TRUE(options->find_typed_argument<int>("input-list") == nullptr);
        ASSERT_TRUE(options->find_typed_argument<short>("input") == nullptr);
        ASSERT_TRUE(single_arg->has_default_value());
        ASSERT_TRUE(single_arg->has_value());
        ASSERT_EQ(40, single_arg->get_value());

        auto list_arg = options->find_typed_argument_list<int>("input-list");
        ASSERT_TRUE(list_arg != nullptr);
        ASSERT_TRUE(list_arg->has_default_value());
        ASSERT_TRUE(list_arg->has_value());
        ASSERT_EQ((std::vector<int>{30, 50}), list_arg->get_values());
        options->reset_to_default();
    }
}

TEST(xdx_cliopts_parser_tests, options_required_arguments) {
    auto builder = Builder("test", "test options")
                       .argument<int>('i', "input", "single required value")
                       .argument_list<float>('l', "input-f-list", "list of floats");

    {
        const char* argv[] = {"test", "-i", "20"};
        auto result = parse_argv(builder.get_options(), std::size(argv), argv);
        ASSERT_TRUE(static_cast<bool>(result.error));
        auto error_value = static_cast<ProcessingArgumentsError>(result.error.value());
        ASSERT_EQ(ProcessingArgumentsError::RequiredArgument, error_value);
    }
}
