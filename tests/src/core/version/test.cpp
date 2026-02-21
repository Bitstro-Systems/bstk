// test.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/core/version.hpp>
#include <gtest/gtest.h>

namespace bs {
    struct _Version_info {
        uint64_t _Major : 16;
        uint64_t _Minor : 24;
        uint64_t _Patch : 24;
    };

    constexpr _Version_info _Decode_version(const uint64_t _Version) noexcept {
        return {_Version >> 48ULL,
            (_Version & 0x0000'FFFF'FF00'0000ULL) >> 24, _Version & 0x0000'0000'00FF'FFFFULL};
    }

    void _Test_version_encoding(
        const uint64_t _Major, const uint64_t _Minor, const uint64_t _Patch) noexcept {
        const auto _Version = _Decode_version(_ENCODE_VERSION(_Major, _Minor, _Patch));
        EXPECT_EQ(_Version._Major, _Major);
        EXPECT_EQ(_Version._Minor, _Minor);
        EXPECT_EQ(_Version._Patch, _Patch);
    }

    void _Test_version_bounds(
        const uint64_t _Major, const uint64_t _Minor, const uint64_t _Patch) noexcept {
        const auto _Version       = _Decode_version(_ENCODE_VERSION(_Major, _Minor, _Patch));
        const bool _Correct_major = _Version._Major == _Major;
        const bool _Correct_minor = _Version._Minor == _Minor;
        const bool _Correct_patch = _Version._Patch == _Patch;
        EXPECT_FALSE(_Correct_major && _Correct_minor && _Correct_patch);
    }

    TEST(version, encoding) {
        _Test_version_encoding(2, 4, 1);
        _Test_version_encoding(6, 7, 9);
        _Test_version_encoding(30, 66, 24);
        _Test_version_encoding(90, 80, 56);
        _Test_version_encoding(436, 105, 304);
        _Test_version_encoding(990, 584, 466);
        _Test_version_encoding(4187, 3660, 2886);
        _Test_version_encoding(7146, 9500, 6035);
        _Test_version_encoding(22256, 39409, 43925);
        _Test_version_encoding(38963, 59099, 61202);
    }

    TEST(version, bounds) {
        _Test_version_bounds(2, 15, 0xFFFF'FF01);
        _Test_version_bounds(51, 0xFFFF'FF02, 62);
        _Test_version_bounds(0x0001'0000, 734, 913);
        _Test_version_bounds(9051, 0xFFFF'FF02, 0xFFFF'FF01);
        _Test_version_bounds(0x0001'0000, 0xFFFF'FF02, 41097);
        _Test_version_bounds(0x0001'0000, 0xFFFF'FF02, 0xFFFF'FF01);
    }
} // namespace bs