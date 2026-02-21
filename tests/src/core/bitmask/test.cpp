// test.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/core/bitmask.hpp>
#include <gtest/gtest.h>

namespace bs {
    enum class _Test_enum : uint8_t {};

    TEST(bitmask, enumeration) {
        EXPECT_FALSE(enumeration<uint8_t>);
        EXPECT_TRUE(enumeration<_Test_enum>);
    }

    TEST(bitmask, operator_bitor) {
        EXPECT_EQ(_Test_enum{0b0000} | _Test_enum{0b0000}, _Test_enum{0b0000});
        EXPECT_EQ(_Test_enum{0b0000} | _Test_enum{0b0011}, _Test_enum{0b0011});
        EXPECT_EQ(_Test_enum{0b1100} | _Test_enum{0b0000}, _Test_enum{0b1100});
        EXPECT_EQ(_Test_enum{0b1010} | _Test_enum{0b0101}, _Test_enum{0b1111});
        EXPECT_EQ(_Test_enum{0b1110} | _Test_enum{0b0111}, _Test_enum{0b1111});
        EXPECT_EQ(_Test_enum{0b1111} | _Test_enum{0b1111}, _Test_enum{0b1111});
    }

    TEST(bitmask, operator_bitand) {
        EXPECT_EQ(_Test_enum{0b0000} & _Test_enum{0b0000}, _Test_enum{0b0000});
        EXPECT_EQ(_Test_enum{0b0000} & _Test_enum{0b0011}, _Test_enum{0b0000});
        EXPECT_EQ(_Test_enum{0b1100} & _Test_enum{0b0000}, _Test_enum{0b0000});
        EXPECT_EQ(_Test_enum{0b1010} & _Test_enum{0b0101}, _Test_enum{0b0000});
        EXPECT_EQ(_Test_enum{0b1110} & _Test_enum{0b0111}, _Test_enum{0b0110});
        EXPECT_EQ(_Test_enum{0b1111} & _Test_enum{0b1111}, _Test_enum{0b1111});
    }

    TEST(bitmask, operator_bitxor) {
        EXPECT_EQ(_Test_enum{0b0000} ^ _Test_enum{0b0000}, _Test_enum{0b0000});
        EXPECT_EQ(_Test_enum{0b0000} ^ _Test_enum{0b0011}, _Test_enum{0b0011});
        EXPECT_EQ(_Test_enum{0b1100} ^ _Test_enum{0b0000}, _Test_enum{0b1100});
        EXPECT_EQ(_Test_enum{0b1010} ^ _Test_enum{0b0101}, _Test_enum{0b1111});
        EXPECT_EQ(_Test_enum{0b1110} ^ _Test_enum{0b0111}, _Test_enum{0b1001});
        EXPECT_EQ(_Test_enum{0b1111} ^ _Test_enum{0b1111}, _Test_enum{0b0000});
    }

    TEST(bitmask, operator_bitnot) {
        EXPECT_EQ(~_Test_enum{0b0000'0000}, _Test_enum{0b1111'1111});
        EXPECT_EQ(~_Test_enum{0b1100'1100}, _Test_enum{0b0011'0011});
        EXPECT_EQ(~_Test_enum{0b1010'1010}, _Test_enum{0b0101'0101});
        EXPECT_EQ(~_Test_enum{0b0011'1110}, _Test_enum{0b1100'0001});
        EXPECT_EQ(~_Test_enum{0b1111'1111}, _Test_enum{0b0000'0000});
    }

    void _Test_bitor_assign(
        const _Test_enum _Mask, const _Test_enum _Value, const _Test_enum _Expected) {
        _Test_enum _Mask_ref = _Mask;
        _Mask_ref           |= _Value;
        EXPECT_EQ(_Mask_ref, _Expected);
    }

    void _Test_bitand_assign(
        const _Test_enum _Mask, const _Test_enum _Value, const _Test_enum _Expected) {
        _Test_enum _Mask_ref = _Mask;
        _Mask_ref           &= _Value;
        EXPECT_EQ(_Mask_ref, _Expected);
    }

    void _Test_bitxor_assign(
        const _Test_enum _Mask, const _Test_enum _Value, const _Test_enum _Expected) {
        _Test_enum _Mask_ref = _Mask;
        _Mask_ref           ^= _Value;
        EXPECT_EQ(_Mask_ref, _Expected);
    }

    TEST(bitmask, operator_bitor_assign) {
        _Test_bitor_assign(_Test_enum{0b0000}, _Test_enum{0b0000}, _Test_enum{0b0000});
        _Test_bitor_assign(_Test_enum{0b0000}, _Test_enum{0b0011}, _Test_enum{0b0011});
        _Test_bitor_assign(_Test_enum{0b1100}, _Test_enum{0b0000}, _Test_enum{0b1100});
        _Test_bitor_assign(_Test_enum{0b1010}, _Test_enum{0b0101}, _Test_enum{0b1111});
        _Test_bitor_assign(_Test_enum{0b1110}, _Test_enum{0b0111}, _Test_enum{0b1111});
        _Test_bitor_assign(_Test_enum{0b1111}, _Test_enum{0b1111}, _Test_enum{0b1111});
    }

    TEST(bitmask, operator_bitand_assign) {
        _Test_bitand_assign(_Test_enum{0b0000}, _Test_enum{0b0000}, _Test_enum{0b0000});
        _Test_bitand_assign(_Test_enum{0b0000}, _Test_enum{0b0011}, _Test_enum{0b0000});
        _Test_bitand_assign(_Test_enum{0b1100}, _Test_enum{0b0000}, _Test_enum{0b0000});
        _Test_bitand_assign(_Test_enum{0b1010}, _Test_enum{0b0101}, _Test_enum{0b0000});
        _Test_bitand_assign(_Test_enum{0b1110}, _Test_enum{0b0111}, _Test_enum{0b0110});
        _Test_bitand_assign(_Test_enum{0b1111}, _Test_enum{0b1111}, _Test_enum{0b1111});
    }

    TEST(bitmask, operator_bitxor_assign) {
        _Test_bitxor_assign(_Test_enum{0b0000}, _Test_enum{0b0000}, _Test_enum{0b0000});
        _Test_bitxor_assign(_Test_enum{0b0000}, _Test_enum{0b0011}, _Test_enum{0b0011});
        _Test_bitxor_assign(_Test_enum{0b1100}, _Test_enum{0b0000}, _Test_enum{0b1100});
        _Test_bitxor_assign(_Test_enum{0b1010}, _Test_enum{0b0101}, _Test_enum{0b1111});
        _Test_bitxor_assign(_Test_enum{0b1110}, _Test_enum{0b0111}, _Test_enum{0b1001});
        _Test_bitxor_assign(_Test_enum{0b1111}, _Test_enum{0b1111}, _Test_enum{0b0000});
    }

    TEST(bitmask, bitmask_contains_any) {
        EXPECT_FALSE(::bs::bitmask_contains_any(0b0000, 0b0000));
        EXPECT_FALSE(::bs::bitmask_contains_any(0b0000, 0b0001));
        EXPECT_FALSE(::bs::bitmask_contains_any(0b1010, 0b0101));
        EXPECT_FALSE(::bs::bitmask_contains_any(0b1111, 0b0000));
        EXPECT_TRUE(::bs::bitmask_contains_any(0b1010, 0b0010));
        EXPECT_TRUE(::bs::bitmask_contains_any(0b1111, 0b1111));
    }

    TEST(bitmask, bitmask_contains_all) {
        EXPECT_FALSE(::bs::bitmask_contains_all(0b1010, 0b0101));
        EXPECT_FALSE(::bs::bitmask_contains_all(0b1110, 0b1111));
        EXPECT_TRUE(::bs::bitmask_contains_all(0b0001, 0b0000));
        EXPECT_TRUE(::bs::bitmask_contains_all(0b0000, 0b0000));
        EXPECT_TRUE(::bs::bitmask_contains_all(0b0011, 0b0001));
        EXPECT_TRUE(::bs::bitmask_contains_all(0b1111, 0b1111));
    }
} // namespace bs