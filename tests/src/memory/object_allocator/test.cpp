// test.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/core/impl/utility.hpp>
#include <bstk/memory/object_allocator.hpp>
#include <gtest/gtest.h>

namespace bs {
    class _Tracking_allocator : public allocator {
    public:
        _Tracking_allocator(size_type& _Size, size_type& _Align) noexcept
            : _Mysize(_Size), _Myalign(_Align) {}

        ~_Tracking_allocator() noexcept override = default;

        _Tracking_allocator()                                      = delete;
        _Tracking_allocator(const _Tracking_allocator&)            = delete;
        _Tracking_allocator& operator=(const _Tracking_allocator&) = delete;

        allocator_id id() const noexcept override {
            return allocator_id{0xFF};
        }

        size_type max_size() const noexcept override {
            return 0xFFFF;
        }

        bool is_equal(const allocator&) const noexcept override {
            return true;
        }

        bool is_thread_safe() const noexcept override {
            return false;
        }

        [[nodiscard]] pointer allocate(size_type _Size, size_type _Align) override {
            _Capture_allocation_size(_Size, _Align);
            return nullptr;
        }

        void deallocate(pointer, size_type _Size, size_type _Align) override {
            _Capture_allocation_size(_Size, _Align);
        }

    private:
        void _Capture_allocation_size(const size_type _Size, const size_type _Align) noexcept {
            _Mysize  = bstk::_Align_up_pow_of_2(_Size, _Align);
            _Myalign = _Align;
        }

        size_type& _Mysize;
        size_type& _Myalign;
    };

    struct alignas(32) _Aligned_type {
        uint8_t _Block[64] = {0};
    };

    class object_allocator_test : public ::testing::Test {
    protected:
        using _Value_type = _Aligned_type;
        using _Alloc_type = object_allocator<_Value_type>;

        static void SetUpTestSuite() {
            ::bs::set_allocator(_Real_al);
        }

        static size_t _Captured_size;
        static size_t _Captured_align;
        static _Tracking_allocator _Real_al;

        _Alloc_type _Al;
    };

    size_t object_allocator_test::_Captured_size  = 0;
    size_t object_allocator_test::_Captured_align = 0;
    _Tracking_allocator object_allocator_test::_Real_al(_Captured_size, _Captured_align);

    TEST_F(object_allocator_test, id) {
        EXPECT_EQ(_Al.id(), _Real_al.id());
    }

    TEST_F(object_allocator_test, max_size) {
        EXPECT_EQ(_Al.max_size(), _Real_al.max_size() / sizeof(_Value_type));
    }

    TEST_F(object_allocator_test, is_equal) {
        EXPECT_TRUE(_Al.is_equal(_Real_al));
    }

    TEST_F(object_allocator_test, is_thread_safe) {
        EXPECT_EQ(_Al.is_thread_safe(), _Real_al.is_thread_safe());
    }

    TEST_F(object_allocator_test, allocate_type_alignment) {
        constexpr size_t _Count         = 4;
        constexpr size_t _Expected_size = _Count * sizeof(_Value_type);
        (void) _Al.allocate(_Count);
        EXPECT_EQ(_Captured_size, _Expected_size);
        EXPECT_EQ(_Captured_align, alignof(_Value_type));
    }

    TEST_F(object_allocator_test, allocate_custom_alignment) {
        constexpr size_t _Count         = 7;
        constexpr size_t _Align         = 128;
        constexpr size_t _Expected_size = bstk::_Align_up_pow_of_2(_Count * sizeof(_Value_type), _Align);
        (void) _Al.allocate(_Count, _Align);
        EXPECT_EQ(_Captured_size, _Expected_size);
        EXPECT_EQ(_Captured_align, _Align);
    }

    TEST_F(object_allocator_test, deallocate_type_alignment) {
        constexpr size_t _Count         = 15;
        constexpr size_t _Expected_size = _Count * sizeof(_Value_type);
        _Al.deallocate(nullptr, _Count);
        EXPECT_EQ(_Captured_size, _Expected_size);
        EXPECT_EQ(_Captured_align, alignof(_Value_type));
    }

    TEST_F(object_allocator_test, deallocate_custom_alignment) {
        constexpr size_t _Count         = 21;
        constexpr size_t _Align         = 256;
        constexpr size_t _Expected_size = bstk::_Align_up_pow_of_2(_Count * sizeof(_Value_type), _Align);
        _Al.deallocate(nullptr, _Count, _Align);
        EXPECT_EQ(_Captured_size, _Expected_size);
        EXPECT_EQ(_Captured_align, _Align);
    }
} // namespace bs