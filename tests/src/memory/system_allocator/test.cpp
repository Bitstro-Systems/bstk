// test.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/memory/system_allocator.hpp>
#include <gtest/gtest.h>

namespace bs {
    class system_allocator_test : public ::testing::Test {
    protected:
        system_allocator _Al;
    };

    TEST_F(system_allocator_test, allocate_aligned) {
        constexpr size_t _Count    = 1000;
        constexpr size_t _Align    = 64;
        void* const _Ptr           = _Al.allocate(_Count, _Align);
        const uintptr_t _Ptr_bytes = reinterpret_cast<uintptr_t>(_Ptr);
        EXPECT_EQ(_Ptr_bytes % _Align, 0); // address should also be aligned
        _Al.deallocate(_Ptr, _Count, _Align);
    }

    TEST_F(system_allocator_test, id) {
        EXPECT_EQ(_Al.id(), allocator_id::system);
    }

    TEST_F(system_allocator_test, max_size) {
#if _BS_X64
        EXPECT_EQ(_Al.max_size(), 0xFFFF'FFFF'FFFF'FFFF);
#else // ^^^ _BS_X64 ^^^ / vvv _BS_X86 vvv
        EXPECT_EQ(_Al.max_size(), 0xFFFF'FFFF);
#endif // _BS_X64
    }

    TEST_F(system_allocator_test, is_equal) {
        const system_allocator _Al2;
        EXPECT_TRUE(_Al.is_equal(_Al2));
        EXPECT_TRUE(_Al2.is_equal(_Al));
    }
} // namespace bs