// memory_block.hpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#pragma once
#ifndef _BSTK_MEMORY_MEMORY_BLOCK_HPP_
#define _BSTK_MEMORY_MEMORY_BLOCK_HPP_
#include <bstk/core/export.hpp>
#include <bstk/memory/allocator.hpp>
#include <bstk/memory/memory_block_view.hpp>

namespace bs {
    class _BSTK_EXPORT memory_block { // owning contiguous memory block
    public:
        using size_type       = allocator::size_type;
        using difference_type = allocator::difference_type;
        using pointer         = allocator::pointer;
        using const_pointer   = allocator::const_pointer;

        memory_block() noexcept;
        memory_block(const memory_block& _Other);
        memory_block(memory_block&& _Other) noexcept;
        ~memory_block();

        explicit memory_block(allocator& _Al) noexcept;

        explicit memory_block(const size_type _Align) noexcept;
        memory_block(const size_type _Align, allocator& _Al) noexcept;

        memory_block(const memory_block_view _Block, const size_type _Align);
        memory_block(const memory_block_view _Block, const size_type _Align, allocator& _Al);

        memory_block(const size_type _Size, const size_type _Align);
        memory_block(const size_type _Size, const size_type _Align, allocator& _Al);

        memory_block& operator=(const memory_block& _Other);
        memory_block& operator=(memory_block&& _Other);
        memory_block& operator=(const memory_block_view _Block);

        // the least alignment required for proper allocation
        static constexpr size_type required_alignment = __STDCPP_DEFAULT_NEW_ALIGNMENT__;

        // returns the block as view
        operator memory_block_view() const noexcept;

        // returns the associated allocator
        allocator& get_allocator() const noexcept;

        // checks whether the block is empty
        bool empty() const noexcept;

        // returns the beginning of the stored data
        pointer begin() noexcept;
        const_pointer begin() const noexcept;

        // returns the end of the stored data
        pointer end() noexcept;
        const_pointer end() const noexcept;

        // returns the size of the block
        size_type size() const noexcept;

        // returns the alignment of the block
        size_type alignment() const noexcept;

        // checks whether the given memory region is within this block
        bool contains(const_pointer _Ptr) const noexcept;
        bool contains(const memory_block_view _Block) const noexcept;
        bool contains(const memory_block& _Block) const noexcept;

        // returns the sub-block of this block
        memory_block block(
            const size_type _Off = 0, size_type _Size = static_cast<size_type>(-1)) const;

        // clears the contents
        void clear();

        // swaps the contents
        void swap(memory_block& _Other) noexcept;

    private:
        // chooses between the required and specified alignment
        static size_type _Choose_align(const size_type _Align) noexcept;

        // copies the block
        void _Copy(const memory_block& _Other);

        // moves the block
        void _Move(memory_block& _Other) noexcept;

        // allocates a new block and copies the contents from the given view
        void _Construct_from_view(const memory_block_view _Block);

        // allocates a new block with the specified size
        void _Construct_from_size(const size_type _Size);

        allocator* _Myal;
        pointer _Myptr;
        size_type _Mysize;
        size_type _Myalign;
    };

    _BSTK_EXPORT bool operator==(const memory_block& _Left, const memory_block& _Right) noexcept;
} // namespace bs

#endif // _BSTK_MEMORY_MEMORY_BLOCK_HPP_