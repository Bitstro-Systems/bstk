// test.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/concurrency/shared_resource.hpp>
#include <gtest/gtest.h>

namespace bs {
    struct _Aggregate_type {
        static constexpr int _Default_number = 235;
        static constexpr char _Default_sign  = 'B';

        _Aggregate_type(const int _Number = _Default_number, const char _Sign = _Default_sign) noexcept
            : _Number(_Number), _Sign(_Sign) {}

        int _Number;
        char _Sign;
    };

    template <class _Ty>
    _Ty _Get_resource_value(const shared_resource<_Ty>& _Res) noexcept {
        return _Res.visit(
            [](const _Ty& _Value) noexcept {
                return _Value;
            }
        );
    }

    TEST(shared_resource, default_construct) {
        const shared_resource<_Aggregate_type> _Res;
        const _Aggregate_type _Value = _Get_resource_value(_Res);
        EXPECT_EQ(_Value._Number, _Aggregate_type::_Default_number);
        EXPECT_EQ(_Value._Sign, _Aggregate_type::_Default_sign);
    }

    TEST(shared_resource, args_construct) {
        constexpr int _Number = 634;
        constexpr char _Sign  = 'q';
        const shared_resource<_Aggregate_type> _Res(_Number, _Sign);
        const _Aggregate_type _Value = _Get_resource_value(_Res);
        EXPECT_EQ(_Value._Number, _Number);
        EXPECT_EQ(_Value._Sign, _Sign);
    }

    TEST(shared_resource, visit) {
        constexpr int _Number = 7342;
        constexpr char _Sign  = 'Y';
        shared_resource<_Aggregate_type> _Res;
        _Res.visit(
            [=](_Aggregate_type& _Value) noexcept {
                _Value._Number = _Number;
                _Value._Sign   = _Sign;
            }
        );

        const _Aggregate_type _Value = _Get_resource_value(_Res);
        EXPECT_EQ(_Value._Number, _Number);
        EXPECT_EQ(_Value._Sign, _Sign);
    }
} // namespace bs