// test.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/core/exception.hpp>
#include <bstk/text/string_view.hpp>
#include <gtest/gtest.h>

namespace bs {
    TEST(string_view, default_construct) {
        const utf8_string_view _Str;
        EXPECT_EQ(_Str.data(), nullptr);
        EXPECT_EQ(_Str.size(), 0);
    }

    TEST(string_view, ptr_count_construct) {
        const char* const _Ptr = "c-string";
        constexpr size_t _Size = 8; // strlen(_Ptr)
        const utf8_string_view _Str(_Ptr, _Size);
        EXPECT_EQ(_Str.data(), _Ptr);
        EXPECT_EQ(_Str.size(), _Size);
    }

    TEST(string_view, ptr_construct) {
        const char* const _Ptr = "c-string";
        constexpr size_t _Size = 8; // strlen(_Ptr)
        const utf8_string_view _Str(_Ptr);
        EXPECT_EQ(_Str.data(), _Ptr);
        EXPECT_EQ(_Str.size(), _Size);
    }

    TEST(string_view, subscript) {
        const char* const _Ptr      = "c-string";
        const utf8_string_view _Str = _Ptr;
        for (size_t _Idx = 0; _Idx < _Str.size(); ++_Idx) {
            EXPECT_EQ(_Str[_Idx], _Ptr[_Idx]);
        }
    }

    TEST(string_view, begin_cbegin) {
        const char* const _Ptr      = "c-string";
        const utf8_string_view _Str = _Ptr;
        const auto _Iter            = _Str.begin();
        const auto _CIter           = _Str.cbegin();
        EXPECT_EQ(_Iter, _CIter);
        EXPECT_EQ(*_Iter, *_Ptr);
        EXPECT_EQ(*_CIter, *_Ptr);
    }

    TEST(string_view, end_cend) {
        const utf8_string_view _Str = "string_view";
        const auto _Iter            = _Str.end();
        const auto _CIter           = _Str.cend();
        EXPECT_EQ(_Iter, _CIter);
        EXPECT_EQ(_Iter, _Str.begin() + _Str.size());
        EXPECT_EQ(_CIter, _Str.cbegin() + _Str.size());
    }

    TEST(string_view, rbegin_crbegin) {
        const utf8_string_view _Str = "string_view";
        const auto _Iter            = _Str.rbegin();
        const auto _CIter           = _Str.crbegin();
        EXPECT_EQ(_Iter, _CIter);
        EXPECT_EQ(_Iter.base(), _Str.end());
        EXPECT_EQ(_CIter.base(), _Str.cend());
    }

    TEST(string_view, rend_crend) {
        const utf8_string_view _Str = "string_view";
        const auto _Iter            = _Str.rend();
        const auto _CIter           = _Str.crend();
        EXPECT_EQ(_Iter, _CIter);
        EXPECT_EQ(_Iter.base(), _Str.begin());
        EXPECT_EQ(_CIter.base(), _Str.cbegin());
    }

    TEST(string_view, at_valid) {
        const char* const _Ptr      = "c-string";
        const utf8_string_view _Str = _Ptr;
        for (size_t _Idx = 0; _Idx < _Str.size(); ++_Idx) {
            EXPECT_EQ(_Str.at(_Idx), _Ptr[_Idx]);
        }
    }

    TEST(string_view, at_invalid) {
        const utf8_string_view _Str = "string_view";
        EXPECT_THROW(_Str.at(_Str.size() * 2), resource_overrun);
    }

    TEST(string_view, front) {
        const char* _Ptr      = "c-string";
        utf8_string_view _Str = _Ptr;
        EXPECT_EQ(_Str.front(), _Ptr[0]);

        _Ptr = "next-c-string";
        _Str = _Ptr;
        EXPECT_EQ(_Str.front(), _Ptr[0]);
    }

    TEST(string_view, back) {
        const char* _Ptr = "c-string";
        size_t _Size     = 8; // strlen(_Ptr)
        utf8_string_view _Str(_Ptr, _Size);
        EXPECT_EQ(_Str.back(), _Ptr[_Size - 1]);

        _Ptr  = "next-c-string";
        _Size = 13; // strlen(_Ptr)
        _Str  = utf8_string_view{_Ptr, _Size};
        EXPECT_EQ(_Str.back(), _Ptr[_Size - 1]);
    }

    TEST(string_view, data) {
        const char* _Ptr      = "c-string";
        utf8_string_view _Str = _Ptr;
        EXPECT_STREQ(_Str.data(), _Ptr);

        _Ptr = "next-c-string";
        _Str = _Ptr;
        EXPECT_STREQ(_Str.data(), _Ptr);
    }

    TEST(string_view, size_length) {
        const char* _Ptr = "c-string";
        size_t _Size     = 8; // strlen(_Ptr)
        utf8_string_view _Str(_Ptr, _Size);
        EXPECT_EQ(_Str.size(), _Size);
        EXPECT_EQ(_Str.length(), _Size);

        _Ptr  = "next-c-string";
        _Size = 13; // strlen(_Ptr)
        _Str  = utf8_string_view{_Ptr, _Size};
        EXPECT_EQ(_Str.size(), _Size);
        EXPECT_EQ(_Str.length(), _Size);
    }

    TEST(string_view, max_size) {
        EXPECT_EQ(utf8_string_view{}.max_size(), PTRDIFF_MAX);
    }

    TEST(string_view, empty) {
        utf8_string_view _Str;
        EXPECT_TRUE(_Str.empty());

        _Str = "string_view";
        EXPECT_FALSE(_Str.empty());

        _Str = "";
        EXPECT_TRUE(_Str.empty());
    }

    TEST(string_view, remove_prefix) {
        utf8_string_view _Str = "utf8_string_view";
        _Str.remove_prefix(5);
        EXPECT_EQ(_Str, "string_view");

        _Str.remove_prefix(7);
        EXPECT_EQ(_Str, "view");

        _Str.remove_prefix(_Str.size());
        EXPECT_TRUE(_Str.empty());
    }

    TEST(string_view, remove_suffix) {
        utf8_string_view _Str = "utf8_string_view";
        _Str.remove_suffix(5);
        EXPECT_EQ(_Str, "utf8_string");

        _Str.remove_suffix(7);
        EXPECT_EQ(_Str, "utf8");

        _Str.remove_suffix(_Str.size());
        EXPECT_TRUE(_Str.empty());
    }

    TEST(string_view, swap) {
        const char* const _Ptr0 = "c-string";
        const char* const _Ptr1 = "next-c-string";
        constexpr size_t _Size0 = 8; // strlen(_Ptr0)
        constexpr size_t _Size1 = 13; // strlen(_Ptr1)
        utf8_string_view _Str0(_Ptr0, _Size0);
        utf8_string_view _Str1(_Ptr1, _Size1);
        _Str0.swap(_Str1);
        EXPECT_EQ(_Str0.data(), _Ptr1);
        EXPECT_EQ(_Str0.size(), _Size1);
        EXPECT_EQ(_Str1.data(), _Ptr0);
        EXPECT_EQ(_Str1.size(), _Size0);
    }

    TEST(string_view, copy_valid) {
        using _Traits               = utf8_string_view::traits_type;
        const utf8_string_view _Str = "string_view";
        constexpr size_t _Buf_size  = 16;
        char _Buf[_Buf_size]        = {'\0'};
        _Str.copy(_Buf, 6);
        EXPECT_STREQ(_Buf, "string");

        _Traits::assign(_Buf, _Buf_size, '\0'); // reset buffer
        _Str.copy(_Buf, 4, 7);
        EXPECT_STREQ(_Buf, "view");
    }

    TEST(string_view, copy_invalid) {
        const utf8_string_view _Str = "string_view";
        char _Buf[16]               = {'\0'};
        EXPECT_THROW(_Str.copy(_Buf, _Str.size(), _Str.size()), resource_overrun);
    }

    TEST(string_view, substr_valid) {
        const utf8_string_view _Str = "string_view";
        EXPECT_EQ(_Str.substr(), _Str);
        EXPECT_EQ(_Str.substr(7), "view");
        EXPECT_EQ(_Str.substr(7, 2), "vi");
        EXPECT_EQ(_Str.substr(7, _Str.size() * 5), "view"); // count is adjusted
    }

    TEST(string_view, substr_invalid) {
        const utf8_string_view _Str = "string_view";
        EXPECT_THROW(_Str.substr(_Str.size() * 2), resource_overrun);
    }

    TEST(string_view, compare) {
        EXPECT_LT(utf8_string_view{"abc"}.compare("abcd"), 0);
        EXPECT_GT(utf8_string_view{"abcd"}.compare("abc"), 0);
        EXPECT_EQ(utf8_string_view{"abc"}.compare("abc"), 0);
        EXPECT_EQ(utf8_string_view{""}.compare(""), 0);
    }

    TEST(string_view, starts_with) {
        const utf8_string_view _Str = "string_view";
        EXPECT_TRUE(_Str.starts_with(""));
        EXPECT_TRUE(_Str.starts_with("str"));
        EXPECT_TRUE(_Str.starts_with("string_"));
        EXPECT_TRUE(_Str.starts_with("string_view"));
        EXPECT_FALSE(_Str.starts_with("tr"));
        EXPECT_FALSE(_Str.starts_with(" str"));
        EXPECT_FALSE(_Str.starts_with("Str"));
        EXPECT_FALSE(_Str.starts_with("string_view "));
    }

    TEST(string_view, ends_with) {
        const utf8_string_view _Str = "string_view";
        EXPECT_TRUE(_Str.ends_with(""));
        EXPECT_TRUE(_Str.ends_with("w"));
        EXPECT_TRUE(_Str.ends_with("_view"));
        EXPECT_TRUE(_Str.ends_with("string_view"));
        EXPECT_FALSE(_Str.ends_with("vie"));
        EXPECT_FALSE(_Str.ends_with("view_"));
        EXPECT_FALSE(_Str.ends_with("View"));
        EXPECT_FALSE(_Str.ends_with("string_view "));
    }

    TEST(string_view, contains) {
        const utf8_string_view _Str = "string_view";
        EXPECT_TRUE(_Str.contains(""));
        EXPECT_TRUE(_Str.contains("str"));
        EXPECT_TRUE(_Str.contains("g_v"));
        EXPECT_TRUE(_Str.contains("string_view"));
        EXPECT_FALSE(_Str.contains("utf8"));
        EXPECT_FALSE(_Str.contains("Str"));
        EXPECT_FALSE(_Str.contains("g-v"));
        EXPECT_FALSE(_Str.contains("string_view "));
    }

    TEST(string_view, find) {
        const utf8_string_view _Str = " long long int;";
        EXPECT_EQ(_Str.find(""), 0);
        EXPECT_EQ(_Str.find("long"), 1);
        EXPECT_EQ(_Str.find("long", 2), 6);
        EXPECT_EQ(_Str.find(' '), 0);
        EXPECT_EQ(_Str.find('o', 1), 2);
        EXPECT_EQ(_Str.find("on"), 2);
        EXPECT_EQ(_Str.find("long double", 5, 4), 6);
        EXPECT_EQ(_Str.find("float"), utf8_string_view::npos);
    }

    TEST(string_view, rfind) {
        const utf8_string_view _Str = "AB AB AB";
        EXPECT_EQ(_Str.rfind(""), _Str.size());
        EXPECT_EQ(_Str.rfind("AB"), 6);
        EXPECT_EQ(_Str.rfind("ABCD", utf8_string_view::npos, 2), 6);
        EXPECT_EQ(_Str.rfind("AB", 5), 3);
        EXPECT_EQ(_Str.rfind("AB", 0), 0);
        EXPECT_EQ(_Str.rfind('B'), 7);
        EXPECT_EQ(_Str.rfind('B', 4), 4);
        EXPECT_EQ(_Str.rfind('C'), utf8_string_view::npos);
    }
} // namespace bs