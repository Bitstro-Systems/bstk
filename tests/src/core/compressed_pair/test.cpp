// test.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/core/compressed_pair.hpp>
#include <gtest/gtest.h>

namespace bs {
    struct _Empty_type {};

    template <class _Ty>
    class _Movable_type {
    public:
        constexpr _Movable_type() : _Myval() {}

        constexpr _Movable_type(const _Ty& _Value) : _Myval(_Value) {}

        constexpr _Movable_type(_Movable_type&& _Other) : _Myval(::std::move(_Other._Myval)) {
            _Other._Myval = _Ty{};
        }

        constexpr _Movable_type& operator=(_Movable_type&& _Other) {
            _Myval        = ::std::move(_Other._Myval);
            _Other._Myval = _Ty{};
            return *this;
        }

        constexpr bool operator==(const _Ty& _Value) const {
            return _Myval == _Value;
        }

    private:
        _Ty _Myval;
    };

    template <class _Ty>
    const void* _Get_address(const _Ty& _Value) noexcept {
        return static_cast<const void*>(::std::addressof(_Value));
    }

    TEST(compressed_pair, default_construct) {
        const compressed_pair<int, char> _Pair;
        EXPECT_EQ(_Pair.first(), int{});
        EXPECT_EQ(_Pair.second(), char{});
    }

    TEST(compressed_pair, copy_construct) {
        const compressed_pair<int, char> _Pair0(18, 'i');
        const compressed_pair<int, char> _Pair1(_Pair0);
        EXPECT_EQ(_Pair0, _Pair1);
    }

    TEST(compressed_pair, move_construct) {
        constexpr int _First   = 72;
        constexpr char _Second = 'e';
        compressed_pair<_Movable_type<int>, _Movable_type<char>> _Pair0(_First, _Second);
        const compressed_pair<_Movable_type<int>, _Movable_type<char>> _Pair1(::std::move(_Pair0));
        EXPECT_EQ(_Pair0.first(), int{});
        EXPECT_EQ(_Pair0.second(), char{});
        EXPECT_EQ(_Pair1.first(), _First);
        EXPECT_EQ(_Pair1.second(), _Second);
    }

    TEST(compressed_pair, value_copy_construct) {
        constexpr int _First   = 692;
        constexpr char _Second = 'm';
        const compressed_pair<int, char> _Pair(_First, _Second);
        EXPECT_EQ(_Pair.first(), _First);
        EXPECT_EQ(_Pair.second(), _Second);
    }

    TEST(compressed_pair, value_move_construct) {
        constexpr int _First   = 823;
        constexpr char _Second = 'u';
        const compressed_pair<int, char> _Pair(int{_First}, char{_Second});
        EXPECT_EQ(_Pair.first(), _First);
        EXPECT_EQ(_Pair.second(), _Second);
    }

    TEST(compressed_pair, copy_assignable) {
        const compressed_pair<int, char> _Pair0(8, 'g');
        compressed_pair<int, char> _Pair1;
        _Pair1 = _Pair0;
        EXPECT_EQ(_Pair0, _Pair1);
    }

    TEST(compressed_pair, move_assignable) {
        constexpr int _First   = 38;
        constexpr char _Second = 'h';
        compressed_pair<_Movable_type<int>, _Movable_type<char>> _Pair0(_First, _Second);
        compressed_pair<_Movable_type<int>, _Movable_type<char>> _Pair1;
        _Pair1 = ::std::move(_Pair0);
        EXPECT_EQ(_Pair0.first(), int{});
        EXPECT_EQ(_Pair0.second(), char{});
        EXPECT_EQ(_Pair1.first(), _First);
        EXPECT_EQ(_Pair1.second(), _Second);
    }

    TEST(compressed_pair, first) {
        int _Value = 52;
        compressed_pair<int, char> _Pair(_Value, 'a');
        EXPECT_EQ(_Pair.first(), _Value);

        _Value        = 16;
        _Pair.first() = _Value;
        EXPECT_EQ(_Pair.first(), _Value);
    }

    TEST(compressed_pair, second) {
        char _Value = 'a';
        compressed_pair<int, char> _Pair(52, _Value);
        EXPECT_EQ(_Pair.second(), _Value);

        _Value         = 't';
        _Pair.second() = _Value;
        EXPECT_EQ(_Pair.second(), _Value);
    }

    TEST(compressed_pair, swap) {
        constexpr int _First0   = 100;
        constexpr char _Second0 = 'c';
        constexpr int _First1   = 200;
        constexpr char _Second1 = 'y';
        compressed_pair<int, char> _Pair0(_First0, _Second0);
        compressed_pair<int, char> _Pair1(_First1, _Second1);

        _Pair0.swap(_Pair1);
        EXPECT_EQ(_Pair0.first(), _First1);
        EXPECT_EQ(_Pair0.second(), _Second1);
        EXPECT_EQ(_Pair1.first(), _First0);
        EXPECT_EQ(_Pair1.second(), _Second0);
    }

    TEST(compressed_pair, compare) {
        const compressed_pair<int, char> _Pair0(10, 'x');
        const compressed_pair<int, char> _Pair1(10, 'x');
        const compressed_pair<int, char> _Pair2(20, 'z');
        EXPECT_EQ(_Pair0, _Pair1);
        EXPECT_NE(_Pair0, _Pair2);
        EXPECT_NE(_Pair1, _Pair2);
    }

    TEST(compressed_pair, compression) {
        const compressed_pair<int, _Empty_type> _Compressed;
        EXPECT_EQ(_Get_address(_Compressed.first()), _Get_address(_Compressed.second()));

        const compressed_pair<int, char> _Not_compressed;
        EXPECT_NE(_Get_address(_Not_compressed.first()), _Get_address(_Not_compressed.second()));
    }
} // namespace bs