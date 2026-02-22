// test.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/memory/allocator.hpp>
#include <gtest/gtest.h>

namespace bs {
    class _Valid_allocator : public allocator { // allocator that satisfies any_allocator concept
    public:
        _Valid_allocator() noexcept                        = default;
        _Valid_allocator(const _Valid_allocator&) noexcept = default;
        _Valid_allocator(_Valid_allocator&&) noexcept      = default;
        ~_Valid_allocator() noexcept override              = default;

        _Valid_allocator& operator=(const _Valid_allocator&) noexcept = default;
        _Valid_allocator& operator=(_Valid_allocator&&) noexcept      = default;

        allocator_id id() const noexcept override {
            return allocator_id{0xFF};
        }

        size_type max_size() const noexcept override {
            return 0;
        }

        bool is_equal(const allocator&) const noexcept override {
            return false;
        }

        pointer allocate(size_type, size_type) override {
            return nullptr;
        }

        void deallocate(pointer, size_type, size_type) override {}
    };

    class _Invalid_allocator { // allocator that doesn't satisfy any_allocator concept
    public:
        using value_type      = void;
        using size_type       = size_t;
        using difference_type = ptrdiff_t;
        using pointer         = void*;
        using const_pointer   = const void*;

        _Invalid_allocator() noexcept                          = default;
        _Invalid_allocator(const _Invalid_allocator&) noexcept = default;
        _Invalid_allocator(_Invalid_allocator&&) noexcept      = default;
        ~_Invalid_allocator() noexcept                         = default;

        _Invalid_allocator& operator=(const _Invalid_allocator&) noexcept = default;
        _Invalid_allocator& operator=(_Invalid_allocator&&) noexcept      = default;

        allocator_id id() const noexcept {
            return allocator_id{0xFF};
        }

        size_type max_size() const noexcept {
            return 0;
        }

        bool is_equal(const allocator&) const noexcept {
            return false;
        }

        pointer allocate(size_type, size_type) {
            return nullptr;
        }

        void deallocate(pointer, size_type, size_type) {}
    };

    TEST(any_allocator, custom_allocator) {
        EXPECT_TRUE(any_allocator<_Valid_allocator>);
        EXPECT_FALSE(any_allocator<_Invalid_allocator>);
    }

    TEST(is_allocator, custom_allocator) {
        EXPECT_TRUE(is_allocator<_Valid_allocator>::value);
        EXPECT_FALSE(is_allocator<_Invalid_allocator>::value);
    }
} // namespace bs