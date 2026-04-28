// test.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/memory/sync_allocator.hpp>
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

    class sync_allocator_test : public ::testing::Test {
    protected:
        sync_allocator_test() noexcept : _Fake(), _Al(_Fake) {}

        _Fake_allocator _Fake;
        sync_allocator _Al;
    };

    TEST_F(sync_allocator_test, id) {
        EXPECT_EQ(_Al.id(), allocator_id::sync);
    }

    TEST_F(sync_allocator_test, max_size) {
        constexpr size_t _Max_size = 0xABCD;
        EXPECT_CALL(_Fake, max_size()).Times(1).WillOnce(::testing::Return(_Max_size));
        EXPECT_EQ(_Al.max_size(), _Max_size);
    }

    TEST_F(sync_allocator_test, is_equal) {
        constexpr bool _Equal = true;
        const _Fake_allocator _Other_fake;
        EXPECT_CALL(_Fake, is_equal(::testing::Ref(_Other_fake))).Times(1).WillOnce(::testing::Return(_Equal));
        EXPECT_EQ(_Al.is_equal(_Other_fake), _Equal);
    }

    TEST_F(sync_allocator_test, is_thread_safe) {
        EXPECT_TRUE(_Al.is_thread_safe());
    }

    TEST_F(sync_allocator_test, allocate) {
        void* const _Ptr        = reinterpret_cast<void*>(uintptr_t{0x631F'95CA});
        constexpr size_t _Size  = 1024;
        constexpr size_t _Align = 8;
        EXPECT_CALL(_Fake, allocate(_Size, _Align)).Times(1).WillOnce(::testing::Return(_Ptr));
        EXPECT_EQ(_Al.allocate(_Size, _Align), _Ptr);
    }

    TEST_F(sync_allocator_test, deallocate) {
        void* const _Ptr        = reinterpret_cast<void*>(uintptr_t{0x12AB'96CC});
        constexpr size_t _Size  = 1024;
        constexpr size_t _Align = 8;
        EXPECT_CALL(_Fake, deallocate(_Ptr, _Size, _Align)).Times(1);
        _Al.deallocate(_Ptr, _Size, _Align);
    }
} // namespace bs