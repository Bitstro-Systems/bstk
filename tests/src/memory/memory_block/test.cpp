// test.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/core/exception.hpp>
#include <bstk/memory/impl/address.hpp>
#include <bstk/memory/memory_block.hpp>
#include <bstk/memory/system_allocator.hpp>
#include <gtest/gtest.h>

namespace bs {
    TEST(memory_block, default_construct) {
        const memory_block _Block;
        EXPECT_EQ(_Block.get_allocator(), ::bs::get_allocator());
        EXPECT_EQ(_Block.begin(), nullptr);
        EXPECT_EQ(_Block.size(), 0);
        EXPECT_EQ(_Block.alignment(), memory_block::required_alignment);
    }

    TEST(memory_block, copy_construct) {
        constexpr size_t _Size  = 1024;
        constexpr size_t _Align = 64;
        system_allocator _Al;
        const memory_block _Block0(_Size, _Align, _Al);
        const memory_block _Block1(_Block0);
        EXPECT_EQ(_Block1.get_allocator(), _Al);
        EXPECT_NE(_Block1.begin(), nullptr);
        EXPECT_EQ(_Block1.size(), _Size);
        EXPECT_EQ(_Block1.alignment(), _Align);
    }

    TEST(memory_block, move_construct) {
        constexpr size_t _Size  = 1024;
        constexpr size_t _Align = 64;
        system_allocator _Al;
        memory_block _Block0(_Size, _Align, _Al);
        const memory_block _Block1(::std::move(_Block0));
        EXPECT_EQ(_Block0.get_allocator(), _Al); // never moved
        EXPECT_EQ(_Block0.begin(), nullptr);
        EXPECT_EQ(_Block0.size(), 0);
        EXPECT_EQ(_Block0.alignment(), _Align); // never moved
        EXPECT_EQ(_Block1.get_allocator(), _Al);
        EXPECT_NE(_Block1.begin(), nullptr);
        EXPECT_EQ(_Block1.size(), _Size);
        EXPECT_EQ(_Block1.alignment(), _Align);
    }

    TEST(memory_block, alloc_construct) {
        system_allocator _Al;
        const memory_block _Block(_Al);
        EXPECT_EQ(_Block.get_allocator(), _Al);
        EXPECT_EQ(_Block.begin(), nullptr);
        EXPECT_EQ(_Block.size(), 0);
        EXPECT_EQ(_Block.alignment(), memory_block::required_alignment);
    }

    TEST(memory_block, align_construct) {
        constexpr size_t _Align = 128;
        const memory_block _Block(_Align);
        EXPECT_EQ(_Block.get_allocator(), ::bs::get_allocator());
        EXPECT_EQ(_Block.begin(), nullptr);
        EXPECT_EQ(_Block.size(), 0);
        EXPECT_EQ(_Block.alignment(), _Align);
    }

    TEST(memory_block, align_alloc_construct) {
        constexpr size_t _Align = 128;
        system_allocator _Al;
        const memory_block _Block(_Align, _Al);
        EXPECT_EQ(_Block.get_allocator(), _Al);
        EXPECT_EQ(_Block.begin(), nullptr);
        EXPECT_EQ(_Block.size(), 0);
        EXPECT_EQ(_Block.alignment(), _Align);
    }

    TEST(memory_block, block_align_construct) {
        constexpr size_t _Size          = 1024;
        constexpr size_t _Align         = 128;
        unsigned char _Raw_block[_Size] = {0};
        const memory_block_view _View(_Raw_block, _Size);
        const memory_block _Block(_View, _Align);
        EXPECT_EQ(_Block.get_allocator(), ::bs::get_allocator());
        EXPECT_NE(_Block.begin(), nullptr);
        EXPECT_EQ(_Block.size(), _Size);
        EXPECT_EQ(_Block.alignment(), _Align);
    }

    TEST(memory_block, block_align_alloc_construct) {
        constexpr size_t _Size          = 1024;
        constexpr size_t _Align         = 128;
        unsigned char _Raw_block[_Size] = {0};
        system_allocator _Al;
        const memory_block_view _View(_Raw_block, _Size);
        const memory_block _Block(_View, _Align, _Al);
        EXPECT_EQ(_Block.get_allocator(), _Al);
        EXPECT_NE(_Block.begin(), nullptr);
        EXPECT_EQ(_Block.size(), _Size);
        EXPECT_EQ(_Block.alignment(), _Align);
    }

    TEST(memory_block, size_align_construct) {
        constexpr size_t _Size  = 1024;
        constexpr size_t _Align = 64;
        const memory_block _Block(_Size, _Align);
        EXPECT_EQ(_Block.get_allocator(), ::bs::get_allocator());
        EXPECT_NE(_Block.begin(), nullptr);
        EXPECT_EQ(_Block.size(), _Size);
        EXPECT_EQ(_Block.alignment(), _Align);
    }

    TEST(memory_block, size_align_alloc_construct) {
        constexpr size_t _Size  = 1024;
        constexpr size_t _Align = 64;
        system_allocator _Al;
        const memory_block _Block(_Size, _Align, _Al);
        EXPECT_EQ(_Block.get_allocator(), _Al);
        EXPECT_NE(_Block.begin(), nullptr);
        EXPECT_EQ(_Block.size(), _Size);
        EXPECT_EQ(_Block.alignment(), _Align);
    }

    TEST(memory_block, copy_assign) {
        constexpr size_t _Size  = 1024;
        constexpr size_t _Align = 64;
        system_allocator _Al;
        const memory_block _Block0(_Size, _Align, _Al);
        memory_block _Block1;
        _Block1 = _Block0;
        EXPECT_EQ(_Block1.get_allocator(), _Al);
        EXPECT_NE(_Block1.begin(), nullptr);
        EXPECT_EQ(_Block1.size(), _Size);
        EXPECT_EQ(_Block1.alignment(), _Align);
    }

    TEST(memory_block, move_assign) {
        constexpr size_t _Size  = 1024;
        constexpr size_t _Align = 64;
        system_allocator _Al;
        memory_block _Block0(_Size, _Align, _Al);
        memory_block _Block1;
        _Block1 = ::std::move(_Block0);
        EXPECT_EQ(_Block0.get_allocator(), _Al); // never moved
        EXPECT_EQ(_Block0.begin(), nullptr);
        EXPECT_EQ(_Block0.size(), 0);
        EXPECT_EQ(_Block0.alignment(), _Align); // never moved
        EXPECT_EQ(_Block1.get_allocator(), _Al);
        EXPECT_NE(_Block1.begin(), nullptr);
        EXPECT_EQ(_Block1.size(), _Size);
        EXPECT_EQ(_Block1.alignment(), _Align);
    }

    TEST(memory_block, view_assign) {
        constexpr size_t _Size          = 1024;
        unsigned char _Raw_block[_Size] = {0};
        const memory_block_view _View(_Raw_block, _Size);
        memory_block _Block;
        _Block = _View;
        EXPECT_NE(_Block.begin(), nullptr);
        EXPECT_EQ(_Block.size(), _Size);
    }

    TEST(memory_block, view_operator) {
        const memory_block _Block(1024, 64);
        const memory_block_view _View(_Block);
        EXPECT_EQ(_View.begin(), _Block.begin());
        EXPECT_EQ(_View.size(), _Block.size());
    }

    TEST(memory_block, get_allocator_default) {
        const memory_block _Block;
        EXPECT_EQ(_Block.get_allocator(), ::bs::get_allocator());
    }

    TEST(memory_block, get_allocator_custom) {
        system_allocator _Al;
        const memory_block _Block(_Al);
        EXPECT_EQ(_Block.get_allocator(), _Al);
    }

    TEST(memory_block, empty) {
        memory_block _Block;
        EXPECT_TRUE(_Block.empty());

        _Block = memory_block{1024, 64};
        EXPECT_FALSE(_Block.empty());

        _Block.clear();
        EXPECT_TRUE(_Block.empty());
    }

    TEST(memory_block, begin) {
        memory_block _Block;
        EXPECT_EQ(_Block.begin(), nullptr);

        _Block = memory_block{1024, 64};
        EXPECT_NE(_Block.begin(), nullptr);

        _Block.clear();
        EXPECT_EQ(_Block.begin(), nullptr);
    }

    TEST(memory_block, end) {
        memory_block _Block;
        EXPECT_EQ(_Block.end(), nullptr);

        _Block = memory_block{1024, 64};
        EXPECT_EQ(_Block.end(), bstk::_Adjust_address_by_offset(_Block.begin(), _Block.size()));

        _Block.clear();
        EXPECT_EQ(_Block.end(), nullptr);
    }

    TEST(memory_block, size) {
        memory_block _Block;
        EXPECT_EQ(_Block.size(), 0);

        constexpr size_t _Size = 1024;
        _Block                 = memory_block{_Size, 64};
        EXPECT_EQ(_Block.size(), _Size);

        _Block.clear();
        EXPECT_EQ(_Block.size(), 0);
    }

    TEST(memory_block, alignment) {
        memory_block _Block;
        EXPECT_EQ(_Block.alignment(), memory_block::required_alignment);

        _Block = memory_block{2}; // less than required, will be boosted
        EXPECT_EQ(_Block.alignment(), memory_block::required_alignment);

        constexpr size_t _Align = 128;
        _Block                  = memory_block{_Align};
        EXPECT_EQ(_Block.alignment(), _Align);
    }

    void _Test_contains_ptr(const memory_block& _Block, const intptr_t _Off, const bool _Contains) {
        EXPECT_EQ(_Block.contains(bstk::_Adjust_address_by_offset(_Block.begin(), _Off)), _Contains);
    }

    TEST(memory_block, contains_ptr) {
        const memory_block _Block(1024, 64);
        _Test_contains_ptr(_Block, -1, false);
        _Test_contains_ptr(_Block, 1024, false);
        _Test_contains_ptr(_Block, 2048, false);
        _Test_contains_ptr(_Block, 0, true);
        _Test_contains_ptr(_Block, 128, true);
        _Test_contains_ptr(_Block, 1023, true);
    }

    void _Test_contains_block(const memory_block& _Block,
        const intptr_t _Off, const size_t _Size, const bool _Contains) {
        EXPECT_EQ(_Block.contains(memory_block_view{
            bstk::_Adjust_address_by_offset(_Block.begin(), _Off), _Size}), _Contains);
    }

    TEST(memory_block, contains_block) {
        const memory_block _Block(1024, 64);
        _Test_contains_block(_Block, -512, 256, false);
        _Test_contains_block(_Block, 0, 1025, false);
        _Test_contains_block(_Block, 128, 2048, false);
        _Test_contains_block(_Block, 0, 128, true);
        _Test_contains_block(_Block, 512, 64, true);
        _Test_contains_block(_Block, 0, 1024, true);
    }

    void _Test_block_valid_bounds(const memory_block& _Block,
        const size_t _Off, const size_t _Size, const size_t _Expected_size) {
        EXPECT_EQ(_Block.block(_Off, _Size).size(), _Expected_size);
    }

    TEST(memory_block, block_valid_bounds) {
        const memory_block _Block(1024, 64);
        _Test_block_valid_bounds(_Block, 0, static_cast<size_t>(-1), 1024);
        _Test_block_valid_bounds(_Block, 24, 500, 500);
        _Test_block_valid_bounds(_Block, 1000, 50, 24);
    }

    TEST(memory_block, block_invalid_bounds) {
        const memory_block _Block(1024, 128);
        bool _Caught = false;
        try {
            _Block.block(2048);
        } catch (const resource_overrun&) {
            _Caught = true;
        }

        EXPECT_TRUE(_Caught);
    }

    TEST(memory_block, clear) {
        memory_block _Block;
        EXPECT_TRUE(_Block.empty());

        _Block.clear();
        EXPECT_TRUE(_Block.empty());

        _Block = memory_block{1024, 64};
        EXPECT_FALSE(_Block.empty());

        _Block.clear();
        EXPECT_TRUE(_Block.empty());

        _Block.clear();
        EXPECT_TRUE(_Block.empty());
    }

    TEST(memory_block, swap) {
        constexpr size_t _Size0  = 1024;
        constexpr size_t _Size1  = 2048;
        constexpr size_t _Align0 = 64;
        constexpr size_t _Align1 = 128;
        system_allocator _Al;
        memory_block _Block0(_Size0, _Align0);
        memory_block _Block1(_Size1, _Align1, _Al);
        const void* const _Ptr0 = _Block0.begin();
        const void* const _Ptr1 = _Block1.begin();
        _Block0.swap(_Block1);
        EXPECT_EQ(_Block0.get_allocator(), _Al);
        EXPECT_EQ(_Block0.begin(), _Ptr1);
        EXPECT_EQ(_Block0.size(), _Size1);
        EXPECT_EQ(_Block0.alignment(), _Align1);
        EXPECT_EQ(_Block1.get_allocator(), ::bs::get_allocator());
        EXPECT_EQ(_Block1.begin(), _Ptr0);
        EXPECT_EQ(_Block1.size(), _Size0);
        EXPECT_EQ(_Block1.alignment(), _Align0);
    }

    TEST(memory_block, equality) {
        constexpr size_t _Size  = 1024;
        constexpr size_t _Align = 64;
        memory_block _Block0(_Size, _Align);
        memory_block _Block1(_Size, _Align);
        EXPECT_EQ(_Block0, _Block0);
        EXPECT_NE(_Block0, _Block1); // different base addresses
    }
} // namespace bs