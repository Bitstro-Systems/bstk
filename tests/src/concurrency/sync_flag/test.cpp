// test.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/concurrency/sync_flag.hpp>
#include <gtest/gtest.h>

namespace bs {
    TEST(sync_flag, default_construct) {
        const sync_flag _Flag;
        EXPECT_EQ(_Flag.is_set(), false);
    }

    TEST(sync_flag, value_construct) {
        constexpr bool _Value = true;
        const sync_flag _Flag(_Value);
        EXPECT_EQ(_Flag.is_set(), _Value);
    }

    TEST(sync_flag, is_set) {
        sync_flag _Flag;
        EXPECT_FALSE(_Flag.is_set());

        _Flag.set();
        EXPECT_TRUE(_Flag.is_set());

        _Flag.clear();
        EXPECT_FALSE(_Flag.is_set());
    }

    TEST(sync_flag, clear) {
        sync_flag _Flag(true);
        EXPECT_TRUE(_Flag.is_set());

        _Flag.clear();
        EXPECT_FALSE(_Flag.is_set());
    }

    TEST(sync_flag, set) {
        sync_flag _Flag(false);
        EXPECT_FALSE(_Flag.is_set());
        EXPECT_FALSE(_Flag.set());
        EXPECT_TRUE(_Flag.is_set());
    }
} // namespace bs