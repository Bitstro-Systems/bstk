// test.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/core/exception.hpp>
#include <gtest/gtest.h>

namespace bs {
    TEST(exception, allocation_failure) {
        EXPECT_THROW(allocation_failure::raise(), allocation_failure);
    }

    TEST(exception, allocation_limit_exceeded) {
        EXPECT_THROW(allocation_limit_exceeded::raise(), allocation_limit_exceeded);
    }

    TEST(exception, resource_overrun) {
        EXPECT_THROW(resource_overrun::raise(), resource_overrun);
    }
} // namespace bs