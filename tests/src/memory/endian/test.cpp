// test.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/memory/endian.hpp>
#include <gtest/gtest.h>

#if _BS_INT128_SUPPORTED
// since 128-bit literals aren't supported, manually merge the lower and upper bits
#define _MAKE_UINT128(_Low, _High) \
    uint128_t{static_cast<uint128_t>(_Low) | (static_cast<uint128_t>(_High) << 64)}
#endif // _BS_INT128_SUPPORTED

namespace bs {
    template <integral _Ty>
    void _Test_to_endian(const _Ty _Value) noexcept {
        const _Ty _Swapped = ::bs::swap_endian(_Value);
#if _BS_BIG_ENDIAN
        EXPECT_EQ(::bs::to_big_endian(_Value), _Value);
        EXPECT_EQ(::bs::to_little_endian(_Value), _Swapped);
#else // ^^^ _BS_BIG_ENDIAN ^^^ / vvv _BS_LITTLE_ENDIAN vvv
        EXPECT_EQ(::bs::to_big_endian(_Value), _Swapped);
        EXPECT_EQ(::bs::to_little_endian(_Value), _Value);
#endif // _BS_BIG_ENDIAN
    }

    template <integral _Ty>
    void _Test_from_endian(const _Ty _Value) noexcept {
        const _Ty _Swapped = ::bs::swap_endian(_Value);
#if _BS_BIG_ENDIAN
        EXPECT_EQ(::bs::from_big_endian(_Value), _Value);
        EXPECT_EQ(::bs::from_little_endian(_Value), _Swapped);
#else // ^^^ _BS_BIG_ENDIAN ^^^ / vvv _BS_LITTLE_ENDIAN vvv
        EXPECT_EQ(::bs::from_big_endian(_Value), _Swapped);
        EXPECT_EQ(::bs::from_little_endian(_Value), _Value);
#endif // _BS_BIG_ENDIAN
    }

    TEST(endian, native) {
#if _BS_BIG_ENDIAN
        EXPECT_EQ(endian::native, endian::big);
#else // ^^^ _BS_BIG_ENDIAN ^^^ / vvv _BS_LITTLE_ENDIAN vvv
        EXPECT_EQ(endian::native, endian::little);
#endif // _BS_BIG_ENDIAN
    }

    TEST(endian, swap_endian_16) {
        EXPECT_EQ(::bs::swap_endian(uint16_t{0x1A2B}), 0x2B1A);
        EXPECT_EQ(::bs::swap_endian(uint16_t{0x3C4D}), 0x4D3C);
        EXPECT_EQ(::bs::swap_endian(uint16_t{0x5E6F}), 0x6F5E);
        EXPECT_EQ(::bs::swap_endian(uint16_t{0x7F8A}), 0x8A7F);
        EXPECT_EQ(::bs::swap_endian(uint16_t{0x9BCD}), 0xCD9B);
    }

    TEST(endian, swap_endian_32) {
        EXPECT_EQ(::bs::swap_endian(uint32_t{0x1234'5678}), 0x7856'3412);
        EXPECT_EQ(::bs::swap_endian(uint32_t{0xA1B2'C3D4}), 0xD4C3'B2A1);
        EXPECT_EQ(::bs::swap_endian(uint32_t{0x89AB'CDEF}), 0xEFCD'AB89);
        EXPECT_EQ(::bs::swap_endian(uint32_t{0x0F1E'2D3C}), 0x3C2D'1E0F);
        EXPECT_EQ(::bs::swap_endian(uint32_t{0x5566'7788}), 0x8877'6655);
    }

    TEST(endian, swap_endian_64) {
        EXPECT_EQ(::bs::swap_endian(uint64_t{0x1234'5678'90AB'CDEF}), 0xEFCD'AB90'7856'3412);
        EXPECT_EQ(::bs::swap_endian(uint64_t{0xA1B2'C3D4'E5F6'0789}), 0x8907'F6E5'D4C3'B2A1);
        EXPECT_EQ(::bs::swap_endian(uint64_t{0x89AB'CDEF'0123'4567}), 0x6745'2301'EFCD'AB89);
        EXPECT_EQ(::bs::swap_endian(uint64_t{0x0F1E'2D3C'4B5A'6978}), 0x7869'5A4B'3C2D'1E0F);
        EXPECT_EQ(::bs::swap_endian(uint64_t{0x5566'7788'99AA'BBCC}), 0xCCBB'AA99'8877'6655);
    }

#if _BS_INT128_SUPPORTED
    TEST(endian, swap_endian_128) {
        EXPECT_EQ(::bs::swap_endian(_MAKE_UINT128(0x1234'5678'9ABC'DEF0, 0x0FED'CBA9'8765'4321)),
            _MAKE_UINT128(0x2143'6587'A9CB'ED0F, 0xF0DE'BC9A'7856'3412));
        EXPECT_EQ(::bs::swap_endian(_MAKE_UINT128(0xA1B2'C3D4'E5F6'0789, 0x9876'FEDC'BA12'3456)),
            _MAKE_UINT128(0x5634'12BA'DCFE'7698, 0x8907'F6E5'D4C3'B2A1));
        EXPECT_EQ(::bs::swap_endian(_MAKE_UINT128(0x89AB'CDEF'0123'4567, 0xFEDC'BA98'7654'3210)),
            _MAKE_UINT128(0x1032'5476'98BA'DCFE, 0x6745'2301'EFCD'AB89));
        EXPECT_EQ(::bs::swap_endian(_MAKE_UINT128(0x0F1E'2D3C'4B5A'6978, 0x8769'5A4B'3C2D'1E0F)),
            _MAKE_UINT128(0x0F1E'2D3C'4B5A'6987, 0x7869'5A4B'3C2D'1E0F));
        EXPECT_EQ(::bs::swap_endian(_MAKE_UINT128(0x5566'7788'99AA'BBCC, 0xCCDD'BBAA'9988'7766)),
            _MAKE_UINT128(0x6677'8899'AABB'DDCC, 0xCCBB'AA99'8877'6655));
    }
#endif // _BS_INT128_SUPPORTED

    TEST(endian, to_endian) {
        _Test_to_endian(uint16_t{0x1A2B});
        _Test_to_endian(uint32_t{0x1234'5678});
        _Test_to_endian(uint64_t{0x1234'5678'9ABC'DEF0});
#if _BS_INT128_SUPPORTED
        _Test_to_endian(_MAKE_UINT128(0xBEEF'CAFE'1234'5678, 0x0FED'CBA9'8765'4321));
#endif // _BS_INT128_SUPPORTED
    }

    TEST(endian, from_endian) {
        _Test_from_endian(uint16_t{0x3C4D});
        _Test_from_endian(uint32_t{0x9ABC'DEF0});
        _Test_from_endian(uint64_t{0x0F1E'2D3C'4B5A'6978});
#if _BS_INT128_SUPPORTED
        _Test_from_endian(_MAKE_UINT128(0x1234'5678'9ABC'DEF0, 0xA1B2'C3D4'E5F6'7890));
#endif // _BS_INT128_SUPPORTED
    }
} // namespace bs