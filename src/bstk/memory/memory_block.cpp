// memory_block.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/core/exception.hpp>
#include <bstk/core/impl/utility.hpp>
#include <bstk/memory/impl/address.hpp>
#include <bstk/memory/memory_block.hpp>
#include <bstk/memory/object.hpp>
#include <cstring>
#include <utility>

namespace bs {
    memory_block::memory_block() noexcept : _Myal(&::bs::get_allocator()),
        _Myptr(nullptr), _Mysize(0), _Myalign(required_alignment) {}

    memory_block::memory_block(const memory_block& _Other) {
        _Copy(_Other);
    }

    memory_block::memory_block(memory_block&& _Other) noexcept {
        _Move(_Other);
    }

    memory_block::memory_block(allocator& _Al) noexcept
        : _Myal(&_Al), _Myptr(nullptr), _Mysize(0), _Myalign(required_alignment) {}

    memory_block::memory_block(const size_type _Align) noexcept
        : _Myal(&::bs::get_allocator()), _Myptr(nullptr), _Mysize(0), _Myalign(_Choose_align(_Align)) {}

    memory_block::memory_block(const size_type _Align, allocator& _Al) noexcept
        : _Myal(&_Al), _Myptr(nullptr), _Mysize(0), _Myalign(_Choose_align(_Align)) {}

    memory_block::memory_block(const memory_block_view _Block, const size_type _Align)
        : _Myal(&::bs::get_allocator()), _Myptr(nullptr), _Mysize(0), _Myalign(_Choose_align(_Align)) {
        _Construct_from_view(_Block);
    }

    memory_block::memory_block(const memory_block_view _Block, const size_type _Align, allocator& _Al)
        : _Myal(&_Al), _Myptr(nullptr), _Mysize(0), _Myalign(_Choose_align(_Align)) {
        _Construct_from_view(_Block);
    }

    memory_block::memory_block(const size_type _Size, const size_type _Align)
        : _Myal(&::bs::get_allocator()), _Myptr(nullptr), _Mysize(0), _Myalign(_Choose_align(_Align)) {
        _Construct_from_size(_Size);
    }

    memory_block::memory_block(const size_type _Size, const size_type _Align, allocator& _Al)
        : _Myal(&_Al), _Myptr(nullptr), _Mysize(0), _Myalign(_Choose_align(_Align)) {
        _Construct_from_size(_Size);
    }

    memory_block::~memory_block() {
        clear();
    }

    memory_block& memory_block::operator=(const memory_block& _Other) {
        if (this != &_Other) {
            clear();
            _Copy(_Other);
        }

        return *this;
    }

    memory_block& memory_block::operator=(memory_block&& _Other) {
        if (this != &_Other) {
            clear();
            _Move(_Other);
        }

        return *this;
    }

    memory_block& memory_block::operator=(const memory_block_view _Block) {
        clear();
        _Construct_from_view(_Block);
        return *this;
    }

    memory_block::size_type memory_block::_Choose_align(const size_type _Align) noexcept {
        // choose between the required and specified alignment
#ifdef _DEBUG
        _INTERNAL_ASSERT(bstk::_Is_pow_of_2(_Align), "alignment must be a power of 2");
#endif // _DEBUG
        return _Align > required_alignment ? _Align : required_alignment;
    }

    void memory_block::_Copy(const memory_block& _Other) {
        _Myal    = _Other._Myal;
        _Myalign = _Other._Myalign;
        if (!_Other.empty()) {
            _Myptr  = _Myal->allocate(_Other._Mysize, _Myalign);
            _Mysize = _Other._Mysize;
            ::memcpy(_Myptr, _Other._Myptr, _Mysize);
        } else {
            _Myptr  = nullptr;
            _Mysize = 0;
        }

    }

    void memory_block::_Move(memory_block& _Other) noexcept {
        // copy the allocator and alignment, move everything else
        _Myal          = _Other._Myal;
        _Myptr         = _Other._Myptr;
        _Mysize        = _Other._Mysize;
        _Myalign       = _Other._Myalign;
        _Other._Myptr  = nullptr;
        _Other._Mysize = 0;
    }

    void memory_block::_Construct_from_view(const memory_block_view _Block) {
        if (!_Block.empty()) {
            _Myptr  = _Myal->allocate(_Block.size(), _Myalign);
            _Mysize = _Block.size();
            ::memcpy(_Myptr, _Block.begin(), _Mysize);
        }
    }

    void memory_block::_Construct_from_size(const size_type _Size) {
        if (_Size > 0) {
            _Myptr  = _Myal->allocate(_Size, _Myalign);
            _Mysize = _Size;
            ::memset(_Myptr, 0, _Mysize);
        }
    }

    memory_block::operator memory_block_view() const noexcept {
        return memory_block_view{_Myptr, _Mysize};
    }

    allocator& memory_block::get_allocator() const noexcept {
        return *_Myal;
    }

    bool memory_block::empty() const noexcept {
        return _Myptr == nullptr && _Mysize == 0;
    }

    memory_block::pointer memory_block::begin() noexcept {
        return _Myptr;
    }

    memory_block::const_pointer memory_block::begin() const noexcept {
        return _Myptr;
    }

    memory_block::pointer memory_block::end() noexcept {
        return _Myptr ? bstk::_Adjust_address_by_offset(_Myptr, _Mysize) : nullptr;
    }

    memory_block::const_pointer memory_block::end() const noexcept {
        return _Myptr ? bstk::_Adjust_address_by_offset(_Myptr, _Mysize) : nullptr;
    }

    memory_block::size_type memory_block::size() const noexcept {
        return _Mysize;
    }

    memory_block::size_type memory_block::alignment() const noexcept {
        return _Myalign;
    }

    bool memory_block::contains(const_pointer _Ptr) const noexcept {
        return static_cast<memory_block_view>(*this).contains(_Ptr);
    }

    bool memory_block::contains(const memory_block_view _Block) const noexcept {
        return static_cast<memory_block_view>(*this).contains(_Block);
    }

    bool memory_block::contains(const memory_block& _Block) const noexcept {
        return static_cast<memory_block_view>(*this).contains(_Block);
    }

    memory_block memory_block::block(const size_type _Off, size_type _Size) const {
        return memory_block{
            static_cast<memory_block_view>(*this).block(_Off, _Size), _Myalign, *_Myal};
    }

    void memory_block::clear() {
        if (!empty()) {
            _Myal->deallocate(_Myptr, _Mysize, _Myalign);
            _Myptr  = nullptr;
            _Mysize = 0;
        }
    }

    void memory_block::swap(memory_block& _Other) noexcept {
        ::std::swap(_Myal, _Other._Myal);
        ::std::swap(_Myptr, _Other._Myptr);
        ::std::swap(_Mysize, _Other._Mysize);
        ::std::swap(_Myalign, _Other._Myalign);
    }

    bool operator==(const memory_block& _Left, const memory_block& _Right) noexcept {
        return _Left.begin() == _Right.begin() && _Left.size() == _Right.size()
            && _Left.alignment() == _Right.alignment();
    }
} // namespace bs