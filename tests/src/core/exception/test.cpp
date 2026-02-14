// test.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/core/exception.hpp>
#include <gtest/gtest.h>

namespace bs {
    TEST(exception, allocation_failure) {
        bool _Caught = false;
        try {
            allocation_failure::raise();
        } catch (const allocation_failure&) {
            _Caught = true;
        } catch (...) {
            // unexpected exception
        }

        EXPECT_TRUE(_Caught);
    }

    TEST(exception, allocation_limit_exceeded) {
        bool _Caught = false;
        try {
            allocation_limit_exceeded::raise();
        } catch (const allocation_limit_exceeded&) {
            _Caught = true;
        } catch (...) {
            // unexpected exception
        }

        EXPECT_TRUE(_Caught);
    }

    TEST(exception, resource_overrun) {
        bool _Caught = false;
        try {
            resource_overrun::raise();
        } catch (const resource_overrun&) {
            _Caught = true;
        } catch (...) {
            // unexpected exception
        }

        EXPECT_TRUE(_Caught);
    }
} // namespace bs