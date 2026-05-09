// test.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/core/exception.hpp>
#include <bstk/memory/memory_block_view.hpp>
#include <gtest/gtest.h>

// casts a number into a pointer
#define _AS_PTR(_Value) reinterpret_cast<void*>(uintptr_t{_Value})

namespace bs {
    TEST(memory_block_view, default_construct) {
        const memory_block_view _Block;
        EXPECT_TRUE(_Block.empty());
        EXPECT_EQ(_Block.begin(), nullptr);
        EXPECT_EQ(_Block.end(), nullptr);
        EXPECT_EQ(_Block.size(), 0);
    }

    TEST(memory_block_view, copy_construct) {
        const memory_block_view _Block0(_AS_PTR(0xAABB'CCDD), 16);
        const memory_block_view _Block1(_Block0);
        EXPECT_EQ(_Block0.begin(), _Block1.begin());
        EXPECT_EQ(_Block0.end(), _Block1.end());
        EXPECT_EQ(_Block0.size(), _Block1.size());
    }

    TEST(memory_block_view, construct_from_ptr_and_size) {
        const void* const _Ptr = _AS_PTR(0x1100'2200);
        constexpr size_t _Size = 32;
        const memory_block_view _Block(_Ptr, _Size);
        EXPECT_EQ(_Block.begin(), _Ptr);
        EXPECT_EQ(_Block.size(), _Size);
    }

    TEST(memory_block_view, copy_assign) {
        const memory_block_view _Block0(_AS_PTR(0xFF99'EE88), 64);
        memory_block_view _Block1;
        EXPECT_EQ(_Block1.begin(), nullptr);
        EXPECT_EQ(_Block1.end(), nullptr);
        EXPECT_EQ(_Block1.size(), 0);

        _Block1 = _Block0;
        EXPECT_EQ(_Block0.begin(), _Block1.begin());
        EXPECT_EQ(_Block0.end(), _Block1.end());
        EXPECT_EQ(_Block0.size(), _Block1.size());
    }

    TEST(memory_block_view, empty) {
        memory_block_view _Block;
        EXPECT_TRUE(_Block.empty());

        _Block = memory_block_view{_AS_PTR(0x5522'CCAA), 128};
        EXPECT_FALSE(_Block.empty());

        _Block = memory_block_view{};
        EXPECT_TRUE(_Block.empty());
    }

    TEST(memory_block_view, begin) {
        memory_block_view _Block;
        EXPECT_EQ(_Block.begin(), nullptr);

        const void* const _Ptr = _AS_PTR(0xCABE'2981);
        _Block                 = memory_block_view{_Ptr, 256};
        EXPECT_EQ(_Block.begin(), _Ptr);
    }

    TEST(memory_block_view, end) {
        memory_block_view _Block;
        EXPECT_EQ(_Block.begin(), nullptr);

        const void* const _Ptr     = _AS_PTR(0x0000'0010);
        constexpr size_t _Size     = 0x10;
        const void* const _Ptr_end = _AS_PTR(0x0000'0020); // _Ptr + _Size
        _Block                     = memory_block_view{_Ptr, _Size};
        EXPECT_EQ(_Block.end(), _Ptr_end);
    }

    TEST(memory_block_view, size) {
        memory_block_view _Block;
        EXPECT_EQ(_Block.size(), 0);

        constexpr size_t _Size = 284;
        _Block                 = memory_block_view{_AS_PTR(0xACCA'9226), _Size};
        EXPECT_EQ(_Block.size(), _Size);
    }

    TEST(memory_block_view, contains_ptr) {
        const memory_block_view _Block(_AS_PTR(0x0000'2000), 0x1000);
        EXPECT_FALSE(_Block.contains(_AS_PTR(0x0000'1FFF)));
        EXPECT_FALSE(_Block.contains(_AS_PTR(0x0000'3000)));
        EXPECT_FALSE(_Block.contains(_AS_PTR(0x0000'3001)));
        EXPECT_TRUE(_Block.contains(_AS_PTR(0x0000'2000)));
        EXPECT_TRUE(_Block.contains(_AS_PTR(0x0000'2500)));
        EXPECT_TRUE(_Block.contains(_AS_PTR(0x0000'2FFF)));
    }

    TEST(memory_block_view, contains_block) {
        const memory_block_view _Block(_AS_PTR(0x1000'0000), 0x5000);
        EXPECT_FALSE(_Block.contains(memory_block_view{_AS_PTR(0x0FFF'0000), 0xFFFF}));
        EXPECT_FALSE(_Block.contains(memory_block_view{_AS_PTR(0x1000'0000), 0x5001}));
        EXPECT_FALSE(_Block.contains(memory_block_view{_AS_PTR(0x1000'5001), 0xA00C}));
        EXPECT_TRUE(_Block.contains(memory_block_view{_AS_PTR(0x1000'0000), 0x2000}));
        EXPECT_TRUE(_Block.contains(memory_block_view{_AS_PTR(0x1000'0000), 0x5000}));
        EXPECT_TRUE(_Block.contains(memory_block_view{_AS_PTR(0x1000'4000), 0x1000}));
    }

    TEST(memory_block_view, block_valid_bounds) {
        const memory_block_view _Block(_AS_PTR(0x1000'0000), 0x1000);
        const memory_block_view _Full_block = _Block.block();
        EXPECT_EQ(_Full_block.begin(), _Block.begin());
        EXPECT_EQ(_Full_block.size(), _Block.size());

        const memory_block_view _Partial_block = _Block.block(0x700);
        EXPECT_EQ(_Partial_block.begin(), _AS_PTR(0x1000'0700));
        EXPECT_EQ(_Partial_block.size(), 0x900);

        const memory_block_view _Trimmed_block = _Block.block(0x900, 0x1000);
        EXPECT_EQ(_Trimmed_block.begin(), _AS_PTR(0x1000'0900));
        EXPECT_EQ(_Trimmed_block.size(), 0x700);
    }

    TEST(memory_block_view, block_invalid_bounds) {
        const memory_block_view _Block(_AS_PTR(0x1000'0000), 0x1000);
        EXPECT_THROW(_Block.block(0x2000), resource_overrun);
    }

    TEST(memory_block_view, swap) {
        const void* const _Ptr0 = _AS_PTR(0x0000'1000);
        constexpr size_t _Size0 = 92;
        const void* const _Ptr1 = _AS_PTR(0x1000'1000);
        constexpr size_t _Size1 = 152;
        memory_block_view _Block0(_Ptr0, _Size0);
        memory_block_view _Block1(_Ptr1, _Size1);
        _Block0.swap(_Block1);
        EXPECT_EQ(_Block0.begin(), _Ptr1);
        EXPECT_EQ(_Block0.size(), _Size1);
        EXPECT_EQ(_Block1.begin(), _Ptr0);
        EXPECT_EQ(_Block1.size(), _Size0);
    }

    TEST(memory_block_view, equality) {
        const memory_block_view _Block0(_AS_PTR(0xBBC5'2841), 0xFFA0);
        memory_block_view _Block1(_AS_PTR(0x652C'991E), 0x1234);
        EXPECT_NE(_Block0, _Block1);

        _Block1 = _Block0;
        EXPECT_EQ(_Block0, _Block1);
    }
} // namespace bs