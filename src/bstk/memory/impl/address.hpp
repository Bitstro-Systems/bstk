// address.hpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#pragma once
#ifndef _BSTK_MEMORY_IMPL_ADDRESS_HPP_
#define _BSTK_MEMORY_IMPL_ADDRESS_HPP_
#include <bstk/memory/endian.hpp>
#include <type_traits>

namespace bs {
    namespace bstk {
        template <integral _Integral>
        void* _Adjust_address_by_offset(void* const _Address, const _Integral _Off) noexcept {
            // return a new address by moving the base address by the given offset
            return static_cast<void*>(static_cast<unsigned char*>(_Address) + _Off);
        }

        template <integral _Integral>
        const void* _Adjust_address_by_offset(const void* const _Address, const _Integral _Off) noexcept {
            // return a new address by moving the base address by the given offset
            return static_cast<const void*>(static_cast<const unsigned char*>(_Address) + _Off);
        }

        inline bool _Is_within_memory_block(const void* const _Base_begin,
            const void* const _Base_end, const void* const _Ptr) noexcept {
            // check whether the memory address is within the base memory block
            return _Ptr >= _Base_begin && _Ptr < _Base_end;
        }

        inline bool _Is_within_memory_block(const void* const _Base_begin, const void* const _Base_end,
            const void* const _Block_begin, const void* const _Block_end) noexcept {
            // check whether the memory block is within the base memory block
            return _Block_begin >= _Base_begin && _Block_end <= _Base_end;
        }

        constexpr size_t _Get_effective_alignment(const size_t _Align) noexcept {
            // return the effective alignment for allocation and deallocation
            return _Align >= __STDCPP_DEFAULT_NEW_ALIGNMENT__ ? _Align : __STDCPP_DEFAULT_NEW_ALIGNMENT__;
        }
    } // namespace bstk
} // namespace bs

#endif // _BSTK_MEMORY_IMPL_ADDRESS_HPP_