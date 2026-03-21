// memory_block_view.hpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#pragma once
#ifndef _BSTK_MEMORY_MEMORY_BLOCK_VIEW_HPP_
#define _BSTK_MEMORY_MEMORY_BLOCK_VIEW_HPP_
#include <bstk/core/export.hpp>
#include <bstk/memory/allocator.hpp>

namespace bs {
    class _BSTK_EXPORT memory_block_view { // non-owning view of a contiguous memory block
    public:
        using size_type       = allocator::size_type;
        using difference_type = allocator::difference_type;
        using pointer         = allocator::pointer;
        using const_pointer   = allocator::const_pointer;
        
        memory_block_view() noexcept;
        memory_block_view(const_pointer _Ptr, const size_type _Size) noexcept;

        ~memory_block_view() noexcept = default;

        memory_block_view(const memory_block_view&) noexcept            = default;
        memory_block_view& operator=(const memory_block_view&) noexcept = default;

        // checks whether the block is empty
        bool empty() const noexcept;

        // returns the beginning of the stored data
        const_pointer begin() const noexcept;

        // returns the end of the stored data
        const_pointer end() const noexcept;

        // returns the size of the block
        size_type size() const noexcept;

        // checks whether the given memory region is within this block
        bool contains(const_pointer _Ptr) const noexcept;
        bool contains(const memory_block_view _Block) const noexcept;

        // returns the sub-block of this block
        memory_block_view block(
            const size_type _Off = 0, size_type _Size = static_cast<size_type>(-1)) const;

        // swaps the contents
        void swap(memory_block_view& _Other) noexcept;

    private:
        // checks whether the given offset is within the block bounds
        void _Check_offset(const size_type _Off) const;

        const_pointer _Myptr;
        size_type _Mysize;
    };

    _BSTK_EXPORT bool operator==(const memory_block_view _Left, const memory_block_view _Right) noexcept;
} // namespace bs

#endif // _BSTK_MEMORY_MEMORY_BLOCK_VIEW_HPP_