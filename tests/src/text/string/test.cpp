// test.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/core/exception.hpp>
#include <bstk/memory/allocator.hpp>
#include <bstk/memory/system_allocator.hpp>
#include <bstk/text/string.hpp>
#include <gtest/gtest.h>

namespace bs {
    class _Small_allocator : public allocator {
    public:
        explicit _Small_allocator(const size_type _Max_size) noexcept : _Mymax(_Max_size) {}

        ~_Small_allocator() noexcept override = default;

        allocator_id id() const noexcept override {
            return allocator_id{0xFF};
        }

        size_type max_size() const noexcept override {
            return _Mymax;
        }

        bool is_equal(const allocator& _Other) const noexcept override {
            const _Small_allocator* const _Other_ptr = dynamic_cast<const _Small_allocator*>(&_Other);
            return _Other_ptr != nullptr && _Other_ptr->_Mymax == _Mymax;
        }

        bool is_thread_safe() const noexcept override {
            return true;
        }

        pointer allocate(size_type, size_type) override {
            return nullptr;
        }

        void deallocate(pointer, size_type, size_type) override {}

    private:
        size_type _Mymax;
    };

    template <class _StrTy>
    utf8_string _Concat_string_with(const utf8_string& _Left, const _StrTy& _Right) {
        return _Left + _Right;
    }

    TEST(string, default_construct) {
        utf8_string _Str;
        EXPECT_STREQ(_Str.c_str(), "");
        EXPECT_EQ(_Str.size(), 0);
        EXPECT_EQ(_Str.capacity(), 15); // small buffer capacity
        EXPECT_EQ(_Str.get_allocator(), ::bs::get_allocator());
    }

    TEST(string, copy_construct) {
        utf8_string _Str0 = "string";
        utf8_string _Str1 = _Str0;
        EXPECT_STREQ(_Str1.c_str(), _Str0.c_str());
        EXPECT_EQ(_Str1.size(), _Str0.size());
        EXPECT_EQ(_Str1.capacity(), _Str0.capacity());
        EXPECT_EQ(_Str1.get_allocator(), _Str0.get_allocator());
    }

    TEST(string, move_construct) {
        const char* const _Ptr = "string";
        constexpr size_t _Size = 6; // strlen(_Ptr)
        utf8_string _Str0(_Ptr, _Size);
        utf8_string _Str1 = ::std::move(_Str0);
        EXPECT_STREQ(_Str1.c_str(), _Ptr);
        EXPECT_EQ(_Str1.size(), _Size);
        EXPECT_EQ(_Str1.capacity(), 15); // small buffer capacity
        EXPECT_EQ(_Str1.get_allocator(), ::bs::get_allocator());
        EXPECT_STREQ(_Str0.c_str(), "");
        EXPECT_EQ(_Str0.size(), 0);
        EXPECT_EQ(_Str0.capacity(), 15); // small buffer capacity
        EXPECT_EQ(_Str0.get_allocator(), ::bs::get_allocator()); // allocator is never moved
    }

    TEST(string, alloc_construct) {
        system_allocator _Al;
        utf8_string _Str(_Al);
        EXPECT_STREQ(_Str.c_str(), "");
        EXPECT_EQ(_Str.size(), 0);
        EXPECT_EQ(_Str.capacity(), 15); // small buffer capacity
        EXPECT_EQ(_Str.get_allocator(), _Al);
    }

    TEST(string, copy_alloc_construct) {
        system_allocator _Al;
        utf8_string _Str0 = "string";
        utf8_string _Str1(_Str0, _Al);
        EXPECT_STREQ(_Str1.c_str(), _Str0.c_str());
        EXPECT_EQ(_Str1.size(), _Str0.size());
        EXPECT_EQ(_Str1.capacity(), _Str0.capacity());
        EXPECT_EQ(_Str1.get_allocator(), _Al);
    }

    TEST(string, move_alloc_construct) {
        const char* const _Ptr = "string";
        constexpr size_t _Size = 6; // strlen(_Ptr)
        system_allocator _Al;
        utf8_string _Str0(_Ptr, _Size);
        utf8_string _Str1(::std::move(_Str0), _Al);
        EXPECT_STREQ(_Str1.c_str(), _Ptr);
        EXPECT_EQ(_Str1.size(), _Size);
        EXPECT_EQ(_Str1.capacity(), 15); // small buffer capacity
        EXPECT_EQ(_Str1.get_allocator(), _Al);
        EXPECT_STREQ(_Str0.c_str(), "");
        EXPECT_EQ(_Str0.size(), 0);
        EXPECT_EQ(_Str0.capacity(), 15); // small buffer capacity
        EXPECT_EQ(_Str0.get_allocator(), ::bs::get_allocator()); // allocator is never moved
    }

    TEST(string, size_char_construct) {
        constexpr size_t _Count = 5;
        constexpr char _Ch      = 'X';
        utf8_string _Str(_Count, _Ch);
        EXPECT_STREQ(_Str.c_str(), "XXXXX"); // _Count * _Ch
        EXPECT_EQ(_Str.size(), _Count);
        EXPECT_EQ(_Str.capacity(), 15); // small buffer capacity
        EXPECT_EQ(_Str.get_allocator(), ::bs::get_allocator());
    }

    TEST(string, size_char_alloc_construct) {
        constexpr size_t _Count = 5;
        constexpr char _Ch      = 'X';
        system_allocator _Al;
        utf8_string _Str(_Count, _Ch, _Al);
        EXPECT_STREQ(_Str.c_str(), "XXXXX"); // _Count * _Ch
        EXPECT_EQ(_Str.size(), _Count);
        EXPECT_EQ(_Str.capacity(), 15); // small buffer capacity
        EXPECT_EQ(_Str.get_allocator(), _Al);
    }

    TEST(string, ptr_size_construct) {
        const char* const _Ptr = "c-string";
        constexpr size_t _Size = 2; // strlen("c-")
        utf8_string _Str(_Ptr, _Size);
        EXPECT_STREQ(_Str.c_str(), "c-");
        EXPECT_EQ(_Str.size(), _Size);
        EXPECT_EQ(_Str.capacity(), 15); // small buffer capacity
        EXPECT_EQ(_Str.get_allocator(), ::bs::get_allocator());
    }

    TEST(string, ptr_size_alloc_construct) {
        const char* const _Ptr = "c-string";
        constexpr size_t _Size = 2; // strlen("c-")
        system_allocator _Al;
        utf8_string _Str(_Ptr, _Size, _Al);
        EXPECT_STREQ(_Str.c_str(), "c-");
        EXPECT_EQ(_Str.size(), _Size);
        EXPECT_EQ(_Str.capacity(), 15); // small buffer capacity
        EXPECT_EQ(_Str.get_allocator(), _Al);
    }

    TEST(string, ptr_construct) {
        const char* const _Ptr = "c-string";
        constexpr size_t _Size = 8; // strlen(_Ptr)
        utf8_string _Str(_Ptr, _Size);
        EXPECT_STREQ(_Str.c_str(), _Ptr);
        EXPECT_EQ(_Str.size(), _Size);
        EXPECT_EQ(_Str.capacity(), 15); // small buffer capacity
        EXPECT_EQ(_Str.get_allocator(), ::bs::get_allocator());
    }

    TEST(string, ptr_alloc_construct) {
        const char* const _Ptr = "c-string";
        constexpr size_t _Size = 8; // strlen(_Ptr)
        system_allocator _Al;
        utf8_string _Str(_Ptr, _Size, _Al);
        EXPECT_STREQ(_Str.c_str(), _Ptr);
        EXPECT_EQ(_Str.size(), _Size);
        EXPECT_EQ(_Str.capacity(), 15); // small buffer capacity
        EXPECT_EQ(_Str.get_allocator(), _Al);
    }

    TEST(string, view_construct) {
        const utf8_string_view _View = "string_view";
        utf8_string _Str             = _View;
        EXPECT_STREQ(_Str.c_str(), _View.data());
        EXPECT_EQ(_Str.size(), _View.size());
        EXPECT_EQ(_Str.capacity(), 15); // small buffer capacity
        EXPECT_EQ(_Str.get_allocator(), ::bs::get_allocator());
    }

    TEST(string, view_alloc_construct) {
        const utf8_string_view _View = "string_view";
        system_allocator _Al;
        utf8_string _Str(_View, _Al);
        EXPECT_STREQ(_Str.c_str(), _View.data());
        EXPECT_EQ(_Str.size(), _View.size());
        EXPECT_EQ(_Str.capacity(), 15); // small buffer capacity
        EXPECT_EQ(_Str.get_allocator(), _Al);
    }

    TEST(string, operator_assign) {
        utf8_string _Str;
        _Str = utf8_string{"string"};
        EXPECT_STREQ(_Str.c_str(), "string");
        EXPECT_EQ(_Str.size(), 6);

        _Str = "c-string";
        EXPECT_STREQ(_Str.c_str(), "c-string");
        EXPECT_EQ(_Str.size(), 8);

        _Str = 's';
        EXPECT_STREQ(_Str.c_str(), "s");
        EXPECT_EQ(_Str.size(), 1);

        _Str = utf8_string_view{"string_view"};
        EXPECT_STREQ(_Str.c_str(), "string_view");
        EXPECT_EQ(_Str.size(), 11);
    }

    TEST(string, operator_append) {
        utf8_string _Str;
        _Str += utf8_string{"utf8"};
        EXPECT_STREQ(_Str.c_str(), "utf8");
        EXPECT_EQ(_Str.size(), 4);

        _Str += '_';
        EXPECT_STREQ(_Str.c_str(), "utf8_");
        EXPECT_EQ(_Str.size(), 5);

        _Str += "str";
        EXPECT_STREQ(_Str.c_str(), "utf8_str");
        EXPECT_EQ(_Str.size(), 8);

        _Str += utf8_string_view{"ing"};
        EXPECT_STREQ(_Str.c_str(), "utf8_string");
        EXPECT_EQ(_Str.size(), 11);
    }

    TEST(string, operator_view) {
        const utf8_string _Str       = "string";
        const utf8_string_view _View = _Str;
        EXPECT_EQ(_View.data(), _Str.c_str());
        EXPECT_EQ(_View.size(), _Str.size());
    }

    TEST(string, get_allocator) {
        utf8_string _Str0;
        EXPECT_EQ(_Str0.get_allocator(), ::bs::get_allocator());

        system_allocator _Al;
        utf8_string _Str1(_Al);
        EXPECT_EQ(_Str1.get_allocator(), _Al);
    }

    TEST(string, begin_cbegin) {
        const utf8_string _Str = "string";
        const char* const _Ptr = _Str.c_str();
        const auto _Iter       = _Str.begin();
        const auto _CIter      = _Str.cbegin();
        EXPECT_EQ(_Iter, _CIter);
        EXPECT_EQ(*_Iter, *_Ptr);
        EXPECT_EQ(*_CIter, *_Ptr);
    }

    TEST(string, end_cend) {
        const utf8_string _Str = "string";
        const auto _Iter       = _Str.end();
        const auto _CIter      = _Str.cend();
        EXPECT_EQ(_Iter, _CIter);
        EXPECT_EQ(_Iter, _Str.begin() + _Str.size());
        EXPECT_EQ(_CIter, _Str.cbegin() + _Str.size());
    }

    TEST(string, rbegin_crbegin) {
        const utf8_string _Str = "string";
        const auto _Iter       = _Str.rbegin();
        const auto _CIter      = _Str.crbegin();
        EXPECT_EQ(_Iter, _CIter);
        EXPECT_EQ(_Iter.base(), _Str.end());
        EXPECT_EQ(_CIter.base(), _Str.cend());
    }

    TEST(string, rend_crend) {
        const utf8_string _Str = "string";
        const auto _Iter       = _Str.rend();
        const auto _CIter      = _Str.crend();
        EXPECT_EQ(_Iter, _CIter);
        EXPECT_EQ(_Iter.base(), _Str.begin());
        EXPECT_EQ(_CIter.base(), _Str.cbegin());
    }

    TEST(string, at_valid) {
        const char* const _Ptr = "c-string";
        const utf8_string _Str = _Ptr;
        for (size_t _Idx = 0; _Idx < _Str.size(); ++_Idx) {
            EXPECT_EQ(_Str[_Idx], _Ptr[_Idx]);
        }
    }

    TEST(string, at_invalid) {
        const utf8_string _Str = "string";
        EXPECT_THROW(_Str.at(_Str.size() * 2), resource_overrun);
    }

    TEST(string, front) {
        const char* _Ptr = "c-string";
        utf8_string _Str = _Ptr;
        EXPECT_EQ(_Str.front(), _Ptr[0]);

        _Ptr = "next-c-string";
        _Str = _Ptr;
        EXPECT_EQ(_Str.front(), _Ptr[0]);
    }

    TEST(string, back) {
        const char* _Ptr = "c-string";
        size_t _Size     = 8; // strlen(_Ptr)
        utf8_string _Str(_Ptr, _Size);
        EXPECT_EQ(_Str.back(), _Ptr[_Size - 1]);

        _Ptr  = "next-c-string";
        _Size = 13; // strlen(_Ptr)
        _Str  = _Ptr;
        EXPECT_EQ(_Str.back(), _Ptr[_Size - 1]);
    }

    TEST(string, data) {
        utf8_string _Str = "string";
        EXPECT_STREQ(_Str.data(), "string");

        _Str.data()[0] = 'V';
        EXPECT_STREQ(_Str.data(), "Vtring");
    }

    TEST(string, c_str) {
        const char* _Ptr = "c-string";
        utf8_string _Str = _Ptr;
        EXPECT_STREQ(_Str.c_str(), _Ptr);

        _Ptr = "next-c-string";
        _Str = _Ptr;
        EXPECT_STREQ(_Str.c_str(), _Ptr);
    }

    TEST(string, empty) {
        utf8_string _Str;
        EXPECT_TRUE(_Str.empty());

        _Str = "string";
        EXPECT_FALSE(_Str.empty());

        _Str.clear();
        EXPECT_TRUE(_Str.empty());
    }

    TEST(string, capacity) {
        utf8_string _Str;
        EXPECT_EQ(_Str.capacity(), 15); // small buffer capacity

        _Str = "string";
        EXPECT_EQ(_Str.capacity(), 15); // still fits in the small buffer

        _Str = "long-enough-utf8-string";
        EXPECT_EQ(_Str.capacity(), 31);
    }

    TEST(string, size_length) {
        const char* _Ptr = "c-string";
        size_t _Size     = 8; // strlen(_Ptr)
        utf8_string _Str(_Ptr, _Size);
        EXPECT_EQ(_Str.size(), _Size);
        EXPECT_EQ(_Str.length(), _Size);

        _Ptr  = "next-c-string";
        _Size = 13; // strlen(_Ptr)
        _Str.assign(_Ptr, _Size);
        EXPECT_EQ(_Str.size(), _Size);
        EXPECT_EQ(_Str.length(), _Size);
    }

    TEST(string, max_size) {
        const utf8_string _Str0;
        EXPECT_EQ(_Str0.max_size(), PTRDIFF_MAX);

        _Small_allocator _Al0(1000);
        const utf8_string _Str1(_Al0);
        EXPECT_EQ(_Str1.max_size(), _Al0.max_size() - 1);

        _Small_allocator _Al1(10);
        const utf8_string _Str2(_Al1);
        EXPECT_EQ(_Str2.max_size(), 15); // small buffer capacity
    }

    TEST(string, reserve) {
        utf8_string _Str;
        EXPECT_EQ(_Str.capacity(), 15); // small buffer capacity

        _Str.reserve(10);
        EXPECT_EQ(_Str.capacity(), 15); // small buffer capacity

        constexpr size_t _Capacity = 100;
        constexpr size_t _Mask     = 15; // align (16) - 1
        _Str.reserve(_Capacity);
        EXPECT_EQ(_Str.capacity(), _Capacity | _Mask);
    }

    TEST(string, copy_valid) {
        using _Traits              = utf8_string::traits_type;
        const utf8_string _Str     = "utf8_string";
        constexpr size_t _Buf_size = 16;
        char _Buf[_Buf_size]       = {'\0'};
        _Str.copy(_Buf, 4);
        EXPECT_STREQ(_Buf, "utf8");

        _Traits::assign(_Buf, _Buf_size, '\0'); // reset buffer
        _Str.copy(_Buf, 6, 5);
        EXPECT_STREQ(_Buf, "string");
    }

    TEST(string, copy_invalid) {
        const utf8_string _Str = "string";
        char _Buf[16]          = {'\0'};
        EXPECT_THROW(_Str.copy(_Buf, _Str.size(), _Str.size() * 5), resource_overrun);
    }

    TEST(string, swap) {
        const char* const _Ptr0 = "c-string";
        const char* const _Ptr1 = "c-utf8-string";
        constexpr size_t _Size0 = 8; // strlen(_Ptr0)
        constexpr size_t _Size1 = 13; // strlen(_Ptr1)
        system_allocator _Al0;
        _Small_allocator _Al1(0xFFFF);
        utf8_string _Str0(_Ptr0, _Size0, _Al0);
        utf8_string _Str1(_Ptr1, _Size1, _Al1);
        _Str0.swap(_Str1);
        EXPECT_STREQ(_Str0.c_str(), _Ptr1);
        EXPECT_EQ(_Str0.size(), _Size1);
        EXPECT_EQ(_Str0.get_allocator(), _Al1);
        EXPECT_STREQ(_Str1.c_str(), _Ptr0);
        EXPECT_EQ(_Str1.size(), _Size0);
        EXPECT_EQ(_Str1.get_allocator(), _Al0);
    }

    TEST(string, clear) {
        utf8_string _Str = "string";
        EXPECT_FALSE(_Str.empty());

        _Str.clear();
        EXPECT_TRUE(_Str.empty());
    }

    TEST(string, resize) {
        utf8_string _Str = "aaaabbbb";
        _Str.resize(_Str.size() + 4, 'c'); // append 4x 'c'
        EXPECT_STREQ(_Str.c_str(), "aaaabbbbcccc");
        EXPECT_EQ(_Str.size(), 12);

        _Str.resize(4); // erase [4, 12)
        EXPECT_STREQ(_Str.c_str(), "aaaa");
        EXPECT_EQ(_Str.size(), 4);
    }

    TEST(string, shrink) {
        utf8_string _Str = "utf8_string";
        _Str.shrink(6);
        EXPECT_STREQ(_Str.c_str(), "utf8_");
        EXPECT_EQ(_Str.size(), 5);

        _Str.shrink(1);
        EXPECT_STREQ(_Str.c_str(), "utf8");
        EXPECT_EQ(_Str.size(), 4);

        _Str.shrink(4);
        EXPECT_STREQ(_Str.c_str(), "");
        EXPECT_EQ(_Str.size(), 0);
    }

    TEST(string, shrink_to_fit) {
        utf8_string _Str = "very-long-utf8-string";
        EXPECT_EQ(_Str.capacity(), 31);

        _Str.clear();
        _Str.shrink_to_fit();
        EXPECT_EQ(_Str.capacity(), 15); // small buffer capacity
    }

    TEST(string, assign) {
        utf8_string _Str;
        _Str.assign(4, 'Z');
        EXPECT_STREQ(_Str.c_str(), "ZZZZ");
        EXPECT_EQ(_Str.size(), 4);

        _Str.assign("utf8_string_view", 11);
        EXPECT_STREQ(_Str.c_str(), "utf8_string");
        EXPECT_EQ(_Str.size(), 11);
    }

    TEST(string, append) {
        utf8_string _Str;
        _Str.append(5, 'v');
        EXPECT_STREQ(_Str.c_str(), "vvvvv");
        EXPECT_EQ(_Str.size(), 5);

        _Str.append("aaaabbbb", 4);
        EXPECT_STREQ(_Str.c_str(), "vvvvvaaaa");
        EXPECT_EQ(_Str.size(), 9);
    }

    TEST(string, push_back) {
        utf8_string _Str = "strin";
        _Str.push_back('g');
        EXPECT_STREQ(_Str.c_str(), "string");
        EXPECT_EQ(_Str.size(), 6);

        _Str.push_back('0');
        EXPECT_STREQ(_Str.c_str(), "string0");
        EXPECT_EQ(_Str.size(), 7);
    }

    TEST(string, pop_back) {
        utf8_string _Str = "utf8_string_";
        _Str.pop_back();
        EXPECT_STREQ(_Str.c_str(), "utf8_string");
        EXPECT_EQ(_Str.size(), 11);

        constexpr size_t _Reduction = 7; // strlen("_string")
        for (size_t _Reduced = 0; _Reduced < _Reduction; ++_Reduced) {
            _Str.pop_back();
        }

        EXPECT_STREQ(_Str.c_str(), "utf8");
        EXPECT_EQ(_Str.size(), 4);
    }

    TEST(string, erase) {
        utf8_string _Str = "This Is An Example";
        _Str.erase(7, 3);
        EXPECT_STREQ(_Str.c_str(), "This Is Example");
        EXPECT_EQ(_Str.size(), 15);

        _Str.erase(_Str.begin() + _Str.find(' '));
        EXPECT_STREQ(_Str.c_str(), "ThisIs Example");
        EXPECT_EQ(_Str.size(), 14);

        _Str.erase(_Str.find(' '));
        EXPECT_STREQ(_Str.c_str(), "ThisIs");
        EXPECT_EQ(_Str.size(), 6);

        const utf8_string_const_iterator _First = _Str.begin() + _Str.find('s');
        _Str.erase(_First, _First + 2);
        EXPECT_STREQ(_Str.c_str(), "This");
        EXPECT_EQ(_Str.size(), 4);
    }

    TEST(string, insert) {
        utf8_string _Str = "xmplr";
        _Str.insert(0, 1, 'E');
        EXPECT_STREQ(_Str.c_str(), "Exmplr");
        EXPECT_EQ(_Str.size(), 6);

        _Str.insert(2, "e");
        EXPECT_STREQ(_Str.c_str(), "Exemplr");
        EXPECT_EQ(_Str.size(), 7);

        _Str.insert(6, utf8_string{"a"});
        EXPECT_STREQ(_Str.c_str(), "Exemplar");
        EXPECT_EQ(_Str.size(), 8);
            
        _Str.insert(8, " is an example.");
        EXPECT_STREQ(_Str.c_str(), "Exemplar is an example.");
        EXPECT_EQ(_Str.size(), 23);
        
        _Str.insert(_Str.begin() + 14, ':');
        EXPECT_STREQ(_Str.c_str(), "Exemplar is an: example.");
        EXPECT_EQ(_Str.size(), 24);
    }

    TEST(string, replace) {
        utf8_string _Str = "Hello World!";
        _Str.replace(0, 5, "Hi");
        EXPECT_STREQ(_Str.c_str(), "Hi World!");
        EXPECT_EQ(_Str.size(), 9);
        {
            const auto& _Iter = _Str.begin() + _Str.find('W');
            _Str.replace(_Iter, _Iter + 5, "John");
            EXPECT_STREQ(_Str.c_str(), "Hi John!");
            EXPECT_EQ(_Str.size(), 8);
        }

        {
            const auto& _Iter = _Str.begin() + _Str.size() - 1;
            _Str.replace(_Iter, _Iter + 1, 3, '.');
            EXPECT_STREQ(_Str.c_str(), "Hi John...");
            EXPECT_EQ(_Str.size(), 10);
        }
    }

    TEST(string, find) {
        const utf8_string _Str = " long long int;";
        EXPECT_EQ(_Str.find(utf8_string_view{""}), 0);
        EXPECT_EQ(_Str.find(utf8_string_view{"long"}), 1);
        EXPECT_EQ(_Str.find("long", 2), 6);
        EXPECT_EQ(_Str.find(' '), 0);
        EXPECT_EQ(_Str.find('o', 1), 2);
        EXPECT_EQ(_Str.find(utf8_string_view{"on"}), 2);
        EXPECT_EQ(_Str.find("long double", 5, 4), 6);
        EXPECT_EQ(_Str.find(utf8_string_view{"float"}), utf8_string::npos);
    }

    TEST(string, rfind) {
        const utf8_string _Str = "AB AB AB";
        EXPECT_EQ(_Str.rfind(utf8_string_view{""}), _Str.size());
        EXPECT_EQ(_Str.rfind(utf8_string_view{"AB"}), 6);
        EXPECT_EQ(_Str.rfind("ABCD", utf8_string::npos, 2), 6);
        EXPECT_EQ(_Str.rfind(utf8_string_view{"AB"}, 5), 3);
        EXPECT_EQ(_Str.rfind(utf8_string_view{"AB"}, 0), 0);
        EXPECT_EQ(_Str.rfind('B'), 7);
        EXPECT_EQ(_Str.rfind('B', 4), 4);
        EXPECT_EQ(_Str.rfind('C'), utf8_string::npos);
    }

    TEST(string, compare) {
        EXPECT_LT(utf8_string{"abc"}.compare("abcd"), 0);
        EXPECT_GT(utf8_string{"abcd"}.compare("abc"), 0);
        EXPECT_EQ(utf8_string{"abc"}.compare("abc"), 0);
        EXPECT_EQ(utf8_string{""}.compare(""), 0);
    }

    TEST(string, starts_with) {
        const utf8_string _Str = "utf8_string";
        EXPECT_TRUE(_Str.starts_with(""));
        EXPECT_TRUE(_Str.starts_with("utf8"));
        EXPECT_TRUE(_Str.starts_with("utf8_"));
        EXPECT_TRUE(_Str.starts_with("utf8_string"));
        EXPECT_FALSE(_Str.starts_with("tf8"));
        EXPECT_FALSE(_Str.starts_with(" utf8"));
        EXPECT_FALSE(_Str.starts_with("Utf8"));
        EXPECT_FALSE(_Str.starts_with("utf8_string "));
    }

    TEST(string, ends_with) {
        const utf8_string _Str = "utf8_string";
        EXPECT_TRUE(_Str.ends_with(""));
        EXPECT_TRUE(_Str.ends_with("g"));
        EXPECT_TRUE(_Str.ends_with("_string"));
        EXPECT_TRUE(_Str.ends_with("utf8_string"));
        EXPECT_FALSE(_Str.ends_with("rin"));
        EXPECT_FALSE(_Str.ends_with("utf8_"));
        EXPECT_FALSE(_Str.ends_with("Utf8"));
        EXPECT_FALSE(_Str.ends_with("utf8_string "));
    }

    TEST(string, contains) {
        const utf8_string _Str = "utf8_string";
        EXPECT_TRUE(_Str.contains(""));
        EXPECT_TRUE(_Str.contains("str"));
        EXPECT_TRUE(_Str.contains("8_s"));
        EXPECT_TRUE(_Str.contains("utf8_string"));
        EXPECT_FALSE(_Str.contains("view"));
        EXPECT_FALSE(_Str.contains("Utf8"));
        EXPECT_FALSE(_Str.contains("8-s"));
        EXPECT_FALSE(_Str.contains("utf8_string "));
    }

    TEST(string, substr_valid) {
        const utf8_string _Str = "utf8_string";
        EXPECT_EQ(_Str.substr(), _Str);
        EXPECT_EQ(_Str.substr(5), "string");
        EXPECT_EQ(_Str.substr(5, 3), "str");
        EXPECT_EQ(_Str.substr(5, _Str.size() * 5), "string"); // count is adjusted
    }

    TEST(string, substr_invalid) {
        const utf8_string _Str = "string";
        EXPECT_THROW(_Str.substr(_Str.size() * 3), resource_overrun);
    }

    TEST(string, operator_concat) {
        EXPECT_EQ(_Concat_string_with("utf8_", utf8_string{"string"}), "utf8_string");
        EXPECT_EQ(_Concat_string_with("utf8_", "string"), "utf8_string");
        EXPECT_EQ(_Concat_string_with("utf", '8'), "utf8");
        EXPECT_EQ(_Concat_string_with("utf8_", utf8_string_view{"string"}), "utf8_string");
    }
} // namespace bs