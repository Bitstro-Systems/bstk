// test.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/memory/object_allocator.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace bs {
    class _Fake_allocator : public allocator {
    public:
        MOCK_METHOD(allocator_id, id, (), (const, noexcept, override));
        MOCK_METHOD(size_type, max_size, (), (const, noexcept, override));
        MOCK_METHOD(bool, is_equal, (const allocator&), (const, noexcept, override));
        MOCK_METHOD(bool, is_thread_safe, (), (const, noexcept, override));
        MOCK_METHOD(pointer, allocate, (size_type, size_type), (override));
        MOCK_METHOD(void, deallocate, (pointer, size_type, size_type), (override));
    };

    struct alignas(32) _Aligned_type {
        uint8_t _Block[64] = {0};
    };

    class object_allocator_test : public ::testing::Test {
    protected:
        object_allocator_test() noexcept : _Fake(), _Al() {
            ::bs::set_allocator(_Fake);
        }

        _Fake_allocator _Fake;
        object_allocator<_Aligned_type> _Al;
    };

    TEST_F(object_allocator_test, id) {
        constexpr allocator_id _Id = static_cast<allocator_id>(200);
        EXPECT_CALL(_Fake, id()).Times(1).WillOnce(::testing::Return(_Id));
        EXPECT_EQ(_Al.id(), _Id);
    }

    TEST_F(object_allocator_test, max_size) {
        constexpr size_t _Raw_max_size = 0xFF80;
        constexpr size_t _Max_size     = _Raw_max_size / sizeof(_Aligned_type);
        EXPECT_CALL(_Fake, max_size()).Times(1).WillOnce(::testing::Return(_Raw_max_size));
        EXPECT_EQ(_Al.max_size(), _Max_size);
    }

    TEST_F(object_allocator_test, is_equal) {
        constexpr bool _Equal = true;
        const _Fake_allocator _Other_fake;
        EXPECT_CALL(_Fake, is_equal(::testing::Ref(_Other_fake))).Times(1).WillOnce(::testing::Return(_Equal));
        EXPECT_EQ(_Al.is_equal(_Other_fake), _Equal);
    }

    TEST_F(object_allocator_test, is_thread_safe) {
        constexpr bool _Thread_safe = true;
        EXPECT_CALL(_Fake, is_thread_safe()).Times(1).WillOnce(::testing::Return(_Thread_safe));
        EXPECT_EQ(_Al.is_thread_safe(), _Thread_safe);
    }

    TEST_F(object_allocator_test, allocate_type_alignment) {
        _Aligned_type* const _Ptr    = reinterpret_cast<_Aligned_type*>(uintptr_t{0xABCD'9561});
        constexpr size_t _Count      = 4;
        constexpr size_t _Align      = 0; // default alignment
        constexpr size_t _Size       = _Count * sizeof(_Aligned_type);
        constexpr size_t _Real_align = alignof(_Aligned_type);
        EXPECT_CALL(_Fake, allocate(_Size, _Real_align)).Times(1).WillOnce(::testing::Return(_Ptr));
        EXPECT_EQ(_Al.allocate(_Count, _Align), _Ptr);
    }

    TEST_F(object_allocator_test, allocate_custom_alignment) {
        _Aligned_type* const _Ptr = reinterpret_cast<_Aligned_type*>(uintptr_t{0xDA61'789F});
        constexpr size_t _Count   = 7;
        constexpr size_t _Align   = 128;
        constexpr size_t _Size    = _Count * sizeof(_Aligned_type);
        EXPECT_CALL(_Fake, allocate(_Size, _Align)).Times(1).WillOnce(::testing::Return(_Ptr));
        EXPECT_EQ(_Al.allocate(_Count, _Align), _Ptr);
    }

    TEST_F(object_allocator_test, deallocate_type_alignment) {
        _Aligned_type* const _Ptr    = reinterpret_cast<_Aligned_type*>(uintptr_t{0x5152'AAEB});
        constexpr size_t _Count      = 15;
        constexpr size_t _Align      = 0; // default alignment
        constexpr size_t _Size       = _Count * sizeof(_Aligned_type);
        constexpr size_t _Real_align = alignof(_Aligned_type);
        EXPECT_CALL(_Fake, deallocate(_Ptr, _Size, _Real_align)).Times(1);
        _Al.deallocate(_Ptr, _Count, _Align);
    }

    TEST_F(object_allocator_test, deallocate_custom_alignment) {
        _Aligned_type* const _Ptr = reinterpret_cast<_Aligned_type*>(uintptr_t{0xF9C2'A0E4});
        constexpr size_t _Count   = 21;
        constexpr size_t _Align   = 256;
        constexpr size_t _Size    = _Count * sizeof(_Aligned_type);
        EXPECT_CALL(_Fake, deallocate(_Ptr, _Size, _Align)).Times(1);
        _Al.deallocate(_Ptr, _Count, _Align);
    }
} // namespace bs