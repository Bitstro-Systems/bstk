// allocator.hpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#pragma once
#ifndef _BSTK_MEMORY_ALLOCATOR_HPP_
#define _BSTK_MEMORY_ALLOCATOR_HPP_
#include <bstk/core/export.hpp>
#include <bstk/core/utility.hpp> 
#include <cstddef>
#include <type_traits>

namespace bs {
    enum class allocator_id : unsigned char {
        none   = 0,
        system = 1
    };

    class _BSTK_EXPORT _BSTK_NOVTABLE allocator { // interface for custom memory allocation
    public:
        using value_type      = void;
        using size_type       = size_t;
        using difference_type = ptrdiff_t;
        using pointer         = void*;
        using const_pointer   = const void*;

        allocator() noexcept                 = default;
        allocator(const allocator&) noexcept = default;
        allocator(allocator&&) noexcept      = default;
        virtual ~allocator() noexcept        = default;

        allocator& operator=(const allocator&) noexcept = default;
        allocator& operator=(allocator&&) noexcept      = default;

        // returns the ID that identifies the allocator type
        virtual allocator_id id() const noexcept = 0;
        
        // returns the largest supported allocation size
        virtual size_type max_size() const noexcept = 0;

        // compares for equality with another allocator
        virtual bool is_equal(const allocator& _Other) const noexcept = 0;

        // allocates uninitialized storage with optional alignment
        [[nodiscard]] virtual pointer allocate(size_type _Size, size_type _Align = 0) = 0;

        // deallocates storage with optional alignment
        virtual void deallocate(pointer _Ptr, size_type _Size, size_type _Align) = 0;
    };

    _BSTK_EXPORT bool operator==(const allocator& _Left, const allocator& _Right) noexcept;

    template <class _Alloc>
    concept any_allocator = ::std::is_base_of_v<allocator, _Alloc>;

    template <class _Alloc>
    struct is_allocator : public ::std::bool_constant<any_allocator<_Alloc>> {};

    _BSTK_EXPORT allocator& get_allocator() noexcept;
    _BSTK_EXPORT void set_allocator(allocator& _Al) noexcept;
} // namespace bs

#endif // _BSTK_MEMORY_ALLOCATOR_HPP_