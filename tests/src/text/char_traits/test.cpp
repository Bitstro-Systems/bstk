// test.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/text/char_traits.hpp>
#include <gtest/gtest.h>

namespace bs {
    TEST(char_traits, valid_element_type) {
        EXPECT_TRUE(bstk::_Char_type<byte_t>);
        EXPECT_TRUE(bstk::_Char_type<char>);
        EXPECT_TRUE(bstk::_Char_type<wchar_t>);
    }

    TEST(char_traits, invalid_element_type) {
        EXPECT_FALSE(bstk::_Char_type<char8_t>);
        EXPECT_FALSE(bstk::_Char_type<char16_t>);
        EXPECT_FALSE(bstk::_Char_type<char32_t>);
    }

    TEST(char_traits, assign) {
        using _Traits = char_traits<char>;
        char _Str[5]  = {'\0'};
        _Traits::assign(_Str, 2, 'X');
        EXPECT_STREQ(_Str, "XX");

        _Traits::assign(_Str, 4, 'Y');
        EXPECT_STREQ(_Str, "YYYY");
    }

    TEST(char_traits, compare) {
        using _Traits = char_traits<char>;
        EXPECT_EQ(_Traits::compare("utf8_string", "unicode_string", 0), 0); // empty strings are always equal
        EXPECT_EQ(_Traits::compare("string", "string", 6), 0);
        EXPECT_EQ(_Traits::compare("string", "string_view", 6), 0);
        EXPECT_LT(_Traits::compare("String", "string", 6), 0);
        EXPECT_GT(_Traits::compare("BCDE", "ABCD", 4), 0);
    }

    TEST(char_traits, eq) {
        using _Traits = char_traits<char>;
        EXPECT_TRUE(_Traits::eq("string", "string", 6));
        EXPECT_TRUE(_Traits::eq("string", "string_view", 6));
        EXPECT_FALSE(_Traits::eq("String", "string", 6));
        EXPECT_FALSE(_Traits::eq("String_view", "string_view", 6));
    }

    TEST(char_traits, move) {
        using _Traits = char_traits<char>;
        char _Str[5]  = {'\0'};
        _Traits::copy(_Str, "ABCD", 4);
        _Traits::move(_Str, _Str, 4); // should handle overlapping
        EXPECT_STREQ(_Str, "ABCD");
    }

    TEST(char_traits, copy) {
        using _Traits = char_traits<char>;
        char _Str[5]  = {'\0'};
        _Traits::copy(_Str, "ABCD", 4);
        EXPECT_TRUE(_Traits::eq(_Str, "ABCD", 4));

        _Traits::copy(_Str, "EFG", 3);
        EXPECT_TRUE(_Traits::eq(_Str, "EFG", 3));

        _Traits::copy(_Str, "Z", 1);
        EXPECT_TRUE(_Traits::eq(_Str, "Z", 1));
    }

    TEST(char_traits, length) {
        using _Traits = char_traits<char>;
        EXPECT_EQ(_Traits::length("foo"), 3);
        EXPECT_EQ(_Traits::length("booo"), 4);
    }

    TEST(char_traits, find) {
        using _Traits = char_traits<char>;
        EXPECT_EQ(_Traits::find("C++20", 5, '2'), 3);
        EXPECT_EQ(_Traits::find("C++23", 5, '6'), _Traits::npos);
        EXPECT_EQ(_Traits::find("string_view", 11, "view", 4), 7);
        EXPECT_EQ(_Traits::find("string_view", 11, "utf8", 4), _Traits::npos);
    }
} // namespace bs