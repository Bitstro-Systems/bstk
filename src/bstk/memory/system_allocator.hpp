// system_allocator.hpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#pragma once
#ifndef _BSTK_MEMORY_SYSTEM_ALLOCATOR_HPP_
#define _BSTK_MEMORY_SYSTEM_ALLOCATOR_HPP_
#include <bstk/core/export.hpp>
#include <bstk/memory/allocator.hpp>

namespace bs {
    class _BSTK_EXPORT system_allocator
        : public allocator { // allocator that wraps the system-provided memory management
    public:
        using value_type      = allocator::value_type;
        using size_type       = allocator::size_type;
        using difference_type = allocator::difference_type;
        using pointer         = allocator::pointer;
        using const_pointer   = allocator::const_pointer;

        system_allocator() noexcept                        = default;
        system_allocator(const system_allocator&) noexcept = default;
        system_allocator(system_allocator&&) noexcept      = default;
        ~system_allocator() noexcept override              = default;

        system_allocator& operator=(const system_allocator&) noexcept = default;
        system_allocator& operator=(system_allocator&&) noexcept      = default;

        // returns the ID that identifies the allocator type
        allocator_id id() const noexcept override;
        
        // returns the largest supported allocation size
        size_type max_size() const noexcept override;

        // compares for equality with another allocator
        bool is_equal(const allocator& _Other) const noexcept override;

        // allocates uninitialized storage with optional alignment
        pointer allocate(size_type _Size, size_type _Align = 0) override;

        // deallocates storage with optional alignment
        void deallocate(pointer _Ptr, size_type _Size, size_type _Align) override;
    };
} // namespace bs

#endif // _BSTK_MEMORY_SYSTEM_ALLOCATOR_HPP_