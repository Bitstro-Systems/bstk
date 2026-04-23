// test.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/memory/smart_pointer.hpp>
#include <gtest/gtest.h>

namespace bs {
    struct _Incomplete_type;

    struct _Complete_type {
        int _Obj = 0;

        void _Func() const noexcept {}
    };

    TEST(smart_pointer, valid_element_type) {
        EXPECT_TRUE(bstk::_Smart_ptr_element<bool>);
        EXPECT_TRUE(bstk::_Smart_ptr_element<const char>);
        EXPECT_TRUE(bstk::_Smart_ptr_element<volatile short>);
        EXPECT_TRUE(bstk::_Smart_ptr_element<const volatile int>);
        EXPECT_TRUE(bstk::_Smart_ptr_element<void*>);
        EXPECT_TRUE(bstk::_Smart_ptr_element<void* const>);
        EXPECT_TRUE(bstk::_Smart_ptr_element<const long*>);
        EXPECT_TRUE(bstk::_Smart_ptr_element<const long* const>);
        EXPECT_TRUE(bstk::_Smart_ptr_element<volatile long long*>);
        EXPECT_TRUE(bstk::_Smart_ptr_element<volatile long long* const>);
        EXPECT_TRUE(bstk::_Smart_ptr_element<const volatile float*>);
        EXPECT_TRUE(bstk::_Smart_ptr_element<const volatile float* const>);
        EXPECT_TRUE(bstk::_Smart_ptr_element<double(*)(void*)>);
        EXPECT_TRUE(bstk::_Smart_ptr_element<long double(*)[]>);
        EXPECT_TRUE(bstk::_Smart_ptr_element<int _Complete_type::*>);
        EXPECT_TRUE(bstk::_Smart_ptr_element<void(_Complete_type::*)()>);
    }

    TEST(smart_pointer, invalid_element_type) {
        EXPECT_FALSE(bstk::_Smart_ptr_element<void>);
        EXPECT_FALSE(bstk::_Smart_ptr_element<bool&>);
        EXPECT_FALSE(bstk::_Smart_ptr_element<const char&>);
        EXPECT_FALSE(bstk::_Smart_ptr_element<volatile short&>);
        EXPECT_FALSE(bstk::_Smart_ptr_element<const volatile int&>);
        EXPECT_FALSE(bstk::_Smart_ptr_element<long[]>);
        EXPECT_FALSE(bstk::_Smart_ptr_element<long long[4]>);
        EXPECT_FALSE(bstk::_Smart_ptr_element<float()>);
        EXPECT_FALSE(bstk::_Smart_ptr_element<double(long double)>);
        EXPECT_FALSE(bstk::_Smart_ptr_element<_Incomplete_type>);
    }
} // namespace bs