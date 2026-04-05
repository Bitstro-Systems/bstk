// test.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#ifdef _DEBUG
#include <bstk/memory/impl/debug_block.hpp>
#include <gtest/gtest.h>

namespace bs {
    class _Temp_buffer { // temporary heap-allocated buffer
    public:
        void* _Ptr;

        explicit _Temp_buffer(const size_t _Size) noexcept : _Ptr(::malloc(_Size)) {}

        ~_Temp_buffer() noexcept {
            if (_Ptr) {
                ::free(_Ptr);
                _Ptr = nullptr;
            }
        }
    };

    void _Test_block_size(
        const size_t _Size, const size_t _Align, const size_t _Expected_size) noexcept {
        EXPECT_EQ(bstk::_Calculate_debug_block_size(_Size, _Align), _Expected_size);
    }

    void _Test_prepare_block(const size_t _Size, const size_t _Align, const allocator_id _Id) {
        const size_t _Block_size = bstk::_Calculate_debug_block_size(_Size, _Align);
        _Temp_buffer _Block(_Block_size);
        ASSERT_NE(_Block._Ptr, nullptr);
        bstk::_Prepare_debug_block(_Block._Ptr, _Block_size, _Size, _Align, _Id);

        const auto _Meta = bstk::_Extract_debug_block_metadata(_Block._Ptr, _Size, _Align);
        EXPECT_EQ(_Meta._Header._Size, _Size);
        EXPECT_EQ(_Meta._Header._Align, _Align);
        EXPECT_EQ(_Meta._Header._Id, _Id);
        EXPECT_EQ(_Meta._Header._State, bstk::_Debug_block_state::_Allocated);
    }

    TEST(debug_block, block_size) {
#if _BS_X64
        _Test_block_size(128, 2, 154);
        _Test_block_size(4096, 8, 4128);
#else // ^^^ _BS_X64 ^^^ / vvv _BS_X86 vvv
        _Test_block_size(128, 2, 146);
        _Test_block_size(4096, 8, 4120);
#endif // _BS_X64

        // these are equal on both x64 and x86 due to high alignment
        _Test_block_size(0x0000'FFFF, 32, 0x0001'0040);
        _Test_block_size(0x1000'0000, 128, 0x1000'0100);
        _Test_block_size(0xFFFF'0000, 512, 0xFFFF'0400);
    }

    TEST(debug_block, prepare_block) {
        _Test_prepare_block(37, 1, allocator_id{20});
        _Test_prepare_block(64, 32, allocator_id{52});
        _Test_prepare_block(511, 128, allocator_id{143});
        _Test_prepare_block(4033, 512, allocator_id{212});
        _Test_prepare_block(16384, 2048, allocator_id{255});
    }
} // namespace bs

#endif // _DEBUG