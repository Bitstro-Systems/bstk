// test.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/text/string.hpp>
#include <gtest/gtest.h>

namespace bs {
    TEST(string_iterator, default_construct) {
        const utf8_string_iterator _Iter;
        EXPECT_FALSE(_Iter.valid());
    }

    TEST(string_iterator, valid) {
        utf8_string_const_iterator _Iter;
        EXPECT_FALSE(_Iter.valid());

        utf8_string _Str = "string";
        _Iter            = _Str.begin();
        EXPECT_TRUE(_Iter.valid());
    }

    TEST(string_iterator, dereference) {
        utf8_string _Str = "string";
        const auto _Iter = _Str.begin();
        EXPECT_EQ(*_Iter, _Str.front());

        *_Iter = 'C';
        EXPECT_EQ(*_Iter, _Str.front());
    }

    TEST(string_iterator, arrow) {
        utf8_string _Str = "string";
        const auto _Iter = _Str.begin();
        auto _Ptr        = _Iter.operator->();
        EXPECT_EQ(_Ptr, _Str.c_str());

        *_Ptr = 'g';
        EXPECT_EQ(*_Ptr, _Str.front());
    }

    TEST(string_iterator, subscript) {
        utf8_string _Str = "string";
        const auto _Iter = _Str.begin();
        for (size_t _Idx = 0; _Idx < _Str.size(); ++_Idx) {
            EXPECT_EQ(_Iter[_Idx], _Str[_Idx]);
        }

        _Iter[2] = 'P';
        EXPECT_EQ(_Iter[2], _Str[2]);
    }

    TEST(string_iterator, increment) {
        utf8_string _Str = "string";
        auto _Iter       = _Str.begin();
        for (size_t _Idx = 0; _Idx < _Str.size(); ++_Idx, ++_Iter) {
            EXPECT_EQ(*_Iter, _Str[_Idx]);
        }
    }

    TEST(string_iterator, decrement) {
        utf8_string _Str = "string";
        auto _Iter       = _Str.begin() + _Str.size() - 1; // points at the last element
        for (size_t _Idx = _Str.size() - 1;; --_Idx, --_Iter) {
            EXPECT_EQ(*_Iter, _Str[_Idx]);
            if (_Idx == 0) {
                break;
            }
        }
    }

    TEST(string_iterator, advance) {
        constexpr size_t _Off = 3;
        utf8_string _Str      = "string";
        auto _Iter            = _Str.begin();
        _Iter                += _Off;
        EXPECT_EQ(_Iter, _Str.begin() + _Off);
    }

    TEST(string_iterator, retreat) {
        constexpr size_t _Off = 2;
        utf8_string _Str      = "string";
        auto _Iter            = _Str.begin() + _Off;
        _Iter                -= _Off;
        EXPECT_EQ(_Iter, _Str.begin());
    }

    TEST(string_iterator, advance_new) {
        constexpr size_t _Off = 3;
        utf8_string _Str      = "string";
        const auto _Iter0     = _Str.begin();
        const auto _Iter1     = _Iter0 + _Off;
        EXPECT_NE(_Iter0, _Iter1); // _Iter0 wasn't modified
        EXPECT_EQ(_Iter1, _Iter0 + _Off);
    }

    TEST(string_iterator, retreat_new) {
        constexpr size_t _Off = 4;
        utf8_string _Str      = "string";
        const auto _Iter0     = _Str.begin() + _Off;
        const auto _Iter1     = _Iter0 - _Off;
        EXPECT_NE(_Iter0, _Iter1); // _Iter0 wasn't modified
        EXPECT_EQ(_Iter1, _Iter0 - _Off);
    }

    TEST(string_iterator, compare) {
        utf8_string _Str0 = "string";
        utf8_string _Str1 = "utf8_string";
        const auto _Iter0 = _Str0.begin();
        const auto _Iter1 = _Str1.begin();
        const auto _Iter2 = _Str0.begin();
        EXPECT_NE(_Iter0, _Iter1);
        EXPECT_EQ(_Iter0, _Iter2);
        EXPECT_NE(_Iter1, _Iter2);
    }
} // namespace bs