// test.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/memory/reference_counter.hpp>
#include <gtest/gtest.h>

namespace bs {
    TEST(reference_counter, default_counter) {
        const reference_counter _Counter;
        EXPECT_EQ(_Counter.use_count(), 0);
    }

    TEST(reference_counter, custom_counter) {
        constexpr long _Refs = 10;
        const reference_counter _Counter(_Refs);
        EXPECT_EQ(_Counter.use_count(), _Refs);
    }

    TEST(reference_counter, increment) {
        reference_counter _Counter(0);
        _Counter.increment();
        EXPECT_EQ(_Counter.use_count(), 1);

        _Counter.increment();
        EXPECT_EQ(_Counter.use_count(), 2);
    }

    TEST(reference_counter, decrement) {
        reference_counter _Counter(2);
        EXPECT_EQ(_Counter.decrement(), 1);
        EXPECT_EQ(_Counter.use_count(), 1);

        EXPECT_EQ(_Counter.decrement(), 0);
        EXPECT_EQ(_Counter.use_count(), 0);
    }

    TEST(reference_counter, parallel_increment) {
        // Note: Should test parallel increment once threading is implemented.
    }

    TEST(reference_counter, parallel_decrement) {
        // Note: Should test parallel decrement once threading is implemented.
    }
} // namespace bs