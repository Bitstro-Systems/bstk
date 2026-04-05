// debug_block.hpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#pragma once
#ifndef _BSTK_MEMORY_IMPL_DEBUG_BLOCK_HPP_
#define _BSTK_MEMORY_IMPL_DEBUG_BLOCK_HPP_
#ifdef _DEBUG
#include <bstk/core/impl/utility.hpp>
#include <bstk/memory/allocator.hpp>
#include <bstk/memory/impl/address.hpp>

namespace bs {
    namespace bstk {
        enum class _Debug_block_state : unsigned char {
            _Uninitialized = 0,
            _Deallocated   = 1,
            _Allocated     = 2
        };

        struct _Debug_block_sentinel { // sentinel used to detect memory corruption
            static constexpr uint32_t _Original_value = 0xCCCC'CCCC;
            uint32_t _Value                           = _Original_value;

            constexpr bool _Is_original() const noexcept {
                return _Value == _Original_value;
            }
        };

#pragma pack(push)
#pragma pack(1) // ensure no padding between members
        struct _Debug_block_header { // basic allocation info
            size_t _Size              = 0;
            size_t _Align             = 0;
            allocator_id _Id          = allocator_id::none;
            _Debug_block_state _State = _Debug_block_state::_Uninitialized;
        };
#pragma pack(pop)

        struct _Debug_block_metadata { // allocation info with corruption detection
            _Debug_block_header _Header;
            _Debug_block_sentinel _Underrun_sentinel; // sentinel for block underrun detection
            _Debug_block_sentinel _Overrun_sentinel; // sentinel for block overrun detection
        };

        struct _Debug_block_traits {
            static constexpr intptr_t _Header_offset = 0;

            static constexpr intptr_t _Header_padding_offset() noexcept {
                // header padding is located directly after the header
                return static_cast<intptr_t>(sizeof(_Debug_block_header));
            }

            static constexpr intptr_t _Underrun_sentinel_offset(const size_t _Align) noexcept {
                // underrun sentinel is located directly before the aligned user block
                return _User_block_offset(_Align) - sizeof(_Debug_block_sentinel);
            }

            static constexpr intptr_t _User_block_offset(const size_t _Align) noexcept {
                // user block is located at the next aligned address after the underrun sentinel
                return _Align_up_pow_of_2(sizeof(_Debug_block_header) + sizeof(_Debug_block_sentinel), _Align);
            }

            static constexpr intptr_t _Overrun_sentinel_offset(
                const size_t _Size, const size_t _Align) noexcept {
                // overrun sentinel is located directly after the user block
                return _User_block_offset(_Align) + _Size;
            }

            static constexpr intptr_t _Block_padding_offset(
                const size_t _Size, const size_t _Align) noexcept {
                // block padding (if any) is located after the overrun sentinel
                return _Overrun_sentinel_offset(_Size, _Align) + sizeof(_Debug_block_sentinel);
            }
        };

        constexpr size_t _Calculate_debug_block_size(const size_t _Size, const size_t _Align) noexcept {
            // calculate block size including user block and metadata
            constexpr size_t _Sentinel_size = sizeof(_Debug_block_sentinel);
            return _Align_up_pow_of_2(sizeof(_Debug_block_header) + _Sentinel_size, _Align)
                + _Align_up_pow_of_2(_Size + _Sentinel_size, _Align);
        }

        constexpr size_t _Calculate_debug_block_header_padding_size(const size_t _Align) noexcept {
            // calculate header padding size, return zero if no padding is required
            return _Debug_block_traits::_Underrun_sentinel_offset(_Align)
                - _Debug_block_traits::_Header_padding_offset();
        }

        constexpr size_t _Calculate_debug_block_padding_size(
            const size_t _Block_size, const size_t _User_size, const size_t _Align) noexcept {
            // calculate block padding size, return zero if no padding is required
            return _Block_size - _Debug_block_traits::_Block_padding_offset(_User_size, _Align);
        }

        inline void _Fill_at(
            void* const _Dest, const size_t _Size, const intptr_t _Off, const int _Value) noexcept {
            // fill _Size bytes at (_Dest + _Off) with _Value
            ::memset(_Adjust_address_by_offset(_Dest, _Off), _Value, _Size);
        }

        inline void _Copy_dest_at(
            void* const _Dest, const void* const _Src, const size_t _Size, const size_t _Off) noexcept {
            // copy _Size bytes from _Src to (_Dest + _Off)
            ::memcpy(_Adjust_address_by_offset(_Dest, _Off), _Src, _Size);
        }

        inline void _Copy_src_at(
            void* const _Dest, const void* const _Src, const size_t _Size, const size_t _Off) noexcept {
            // copy _Size bytes from (_Src + _Off) to _Dest
            ::memcpy(_Dest, _Adjust_address_by_offset(_Src, _Off), _Size);
        }

        inline void* _Prepare_debug_block(void* const _Block, const size_t _Block_size,
            const size_t _User_size, const size_t _Align, const allocator_id _Id) noexcept {
            // embed metadata into the given memory block
            constexpr size_t _Sentinel_size   = sizeof(_Debug_block_sentinel);
            const _Debug_block_header _Header = {_User_size, _Align, _Id, _Debug_block_state::_Allocated};
            const _Debug_block_sentinel _Sentinel;
            _Copy_dest_at(_Block, &_Header, sizeof(_Debug_block_header), _Debug_block_traits::_Header_offset);
            
            size_t _Padding = _Calculate_debug_block_header_padding_size(_Align);
            if (_Padding > 0) { // fill header padding with 'BF' (Block Fill)
                _Fill_at(_Block, _Padding, _Debug_block_traits::_Header_padding_offset(), 0xBF);
            }

            // place underrun and overrun sentinels to detect memory boundary violations
            _Copy_dest_at(_Block, &_Sentinel, _Sentinel_size,
                _Debug_block_traits::_Underrun_sentinel_offset(_Align));
            _Copy_dest_at(_Block, &_Sentinel, _Sentinel_size,
                _Debug_block_traits::_Overrun_sentinel_offset(_User_size, _Align));

            _Padding = _Calculate_debug_block_padding_size(_Block_size, _User_size, _Align);
            if (_Padding > 0) { // fill block padding with 'BF' (Block Fill)
                _Fill_at(_Block, _Padding,
                    _Debug_block_traits::_Block_padding_offset(_User_size, _Align), 0xBF);
            }

            return _Adjust_address_by_offset(_Block, _Debug_block_traits::_User_block_offset(_Align));
        }

        inline void _Validate_debug_block_state(
            void* const _Block, const void* const _Ptr, _Debug_block_header& _Header) noexcept {
            // validate the current block state and detect invalid memory usage
            switch (_Header._State) {
            case _Debug_block_state::_Uninitialized:
                _REPORT_ERROR("Corrupted block at 0x%p. Memory was not allocated.", _Ptr);
                break;
            case _Debug_block_state::_Deallocated:
                _REPORT_ERROR("Corrupted block at 0x%p. Memory was already deallocated.", _Ptr);
                break;
            default:
                break;
            }

            // mark the block as deallocated to prevent double-free
            _Header._State = _Debug_block_state::_Deallocated;
            _Copy_dest_at(_Block, &_Header, sizeof(_Debug_block_header),
                _Debug_block_traits::_Header_offset);
        }

        inline _Debug_block_metadata _Extract_debug_block_metadata(
            void* const _Block, const size_t _Size, const size_t _Align) noexcept {
            // extract block metadata (header and sentinels) from the given memory block
            constexpr size_t _Sentinel_size = sizeof(_Debug_block_sentinel);
            _Debug_block_metadata _Meta;
            _Copy_src_at(&_Meta._Header, _Block,
                sizeof(_Debug_block_header), _Debug_block_traits::_Header_offset);
            _Copy_src_at(&_Meta._Underrun_sentinel, _Block, _Sentinel_size,
                _Debug_block_traits::_Underrun_sentinel_offset(_Align));
            _Copy_src_at(&_Meta._Overrun_sentinel, _Block, _Sentinel_size,
                _Debug_block_traits::_Overrun_sentinel_offset(_Size, _Align));
            return _Meta;
        }

        inline void* _Extract_and_validate_debug_block(void* const _Ptr,
            const size_t _Size, const size_t _Align, const allocator_id _Id) noexcept {
            // extract the original block from the user pointer and validate its metadata and integrity
            void* const _Block = _Adjust_address_by_offset(
                _Ptr, -_Debug_block_traits::_User_block_offset(_Align));
            _Debug_block_metadata _Meta = _Extract_debug_block_metadata(_Block, _Size, _Align);
            if (_Meta._Header._Size != _Size) { // report size mismatch
                _REPORT_ERROR("Corrupted block at 0x%p. Size is %zu, but should be %zu.",
                    _Ptr, _Size, _Meta._Header._Size);
            }

            if (_Meta._Header._Align != _Align) { // report alignment mismatch
                _REPORT_ERROR("Corrupted block at 0x%p. Alignment is %zu, but should be %zu.",
                    _Ptr, _Align, _Meta._Header._Align);
            }

            if (_Meta._Header._Id != _Id) { // report allocator ID mismatch
                _REPORT_ERROR("Corrupted block at 0x%p. ID is %u, but should be %u.",
                    _Ptr, static_cast<uint32_t>(_Id), static_cast<uint32_t>(_Meta._Header._Id));
            }

            _Validate_debug_block_state(_Block, _Ptr, _Meta._Header);
            if (!_Meta._Underrun_sentinel._Is_original()) { // report sentinel corruption
                _REPORT_ERROR(
                    "Corrupted block at 0x%p. Memory was written before the begin of the block.", _Ptr);
            }

            if (!_Meta._Overrun_sentinel._Is_original()) { // report sentinel corruption
                _REPORT_ERROR(
                    "Corrupted block at 0x%p. Memory was written after the end of the block.", _Ptr);
            }

            return _Block;
        }

        template <class _AllocFn>
        void* _Allocate_debug_block(size_t _Size, size_t _Align, const allocator_id _Id,
            _AllocFn _Alloc) noexcept(noexcept(::std::declval<_AllocFn>()(_Size, _Align))) {
            // allocate and prepare a debug block with alignment and metadata
            _Align                   = _Get_effective_alignment(_Align);
            _Size                    = _Align_up_pow_of_2(_Size, _Align);
            const size_t _Block_size = _Calculate_debug_block_size(_Size, _Align);
            void* const _Block       = _Alloc(_Block_size, _Align);
            if (!_Block) { // allocation failed, break
                return nullptr;
            }

            return _Prepare_debug_block(_Block, _Block_size, _Size, _Align, _Id);
        }

        template <class _DeallocFn>
        void _Deallocate_debug_block(void* const _Ptr, size_t _Size, size_t _Align, const allocator_id _Id,
            _DeallocFn _Dealloc) noexcept(noexcept(::std::declval<_DeallocFn>()(_Ptr, _Size, _Align))) {
            // extract and deallocate a debug block with alignment and metadata
            _Align = _Get_effective_alignment(_Align);
            _Size  = _Align_up_pow_of_2(_Size, _Align);
            _Dealloc(_Extract_and_validate_debug_block(_Ptr, _Size, _Align, _Id),
                _Calculate_debug_block_size(_Size, _Align), _Align);
        }
    } // namespace bstk
} // namespace bs

#endif // _DEBUG
#endif // _BSTK_MEMORY_IMPL_DEBUG_BLOCK_HPP_