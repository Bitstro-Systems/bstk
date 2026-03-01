// test.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/memory/system_allocator.hpp>
#include <gtest/gtest.h>

namespace bs {
    TEST(system_allocator, allocate_aligned) {
        constexpr size_t _Count = 1000;
        constexpr size_t _Align = 64;
        system_allocator _Al;
        void* const _Ptr           = _Al.allocate(_Count, _Align);
        const uintptr_t _Ptr_bytes = reinterpret_cast<uintptr_t>(_Ptr);
        EXPECT_EQ(_Ptr_bytes % _Align, 0); // address should also be aligned
        _Al.deallocate(_Ptr, _Count, _Align);
    }

    TEST(system_allocator, id) {
        const system_allocator _Al;
        EXPECT_EQ(_Al.id(), allocator_id::system);
    }

    TEST(system_allocator, max_size) {
        const system_allocator _Al;
#if _BS_X64
        EXPECT_EQ(_Al.max_size(), 0xFFFF'FFFF'FFFF'FFFF);
#else // ^^^ _BS_X64 ^^^ / vvv _BS_X86 vvv
        EXPECT_EQ(_Al.max_size(), 0xFFFF'FFFF);
#endif // _BS_X64
    }

    TEST(system_allocator, is_equal) {
        const system_allocator _Al0;
        const system_allocator _Al1;
        EXPECT_TRUE(_Al0.is_equal(_Al1));
        EXPECT_TRUE(_Al1.is_equal(_Al0));
    }
} // namespace bs