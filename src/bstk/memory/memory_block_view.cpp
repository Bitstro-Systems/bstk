// memory_block_view.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <algorithm>
#include <bstk/core/exception.hpp>
#include <bstk/memory/impl/address.hpp>
#include <bstk/memory/memory_block_view.hpp>
#include <utility>

namespace bs {
    memory_block_view::memory_block_view() noexcept : _Myptr(nullptr), _Mysize(0) {}

    memory_block_view::memory_block_view(const_pointer _Ptr, const size_type _Size) noexcept
        : _Myptr(_Ptr), _Mysize(_Size) {
        // reset both pointer and size if either is null or zero
        if (!_Myptr || _Mysize == 0) {
            _Myptr  = nullptr;
            _Mysize = 0;
        }
    }

    void memory_block_view::_Check_offset(const size_type _Off) const {
        if (_Off >= _Mysize) {
            resource_overrun::raise();
        }
    }

    bool memory_block_view::empty() const noexcept {
        return _Myptr == nullptr && _Mysize == 0;
    }

    memory_block_view::const_pointer memory_block_view::begin() const noexcept {
        return _Myptr;
    }

    memory_block_view::const_pointer memory_block_view::end() const noexcept {
        return bstk::_Adjust_address_by_offset(_Myptr, _Mysize);
    }

    memory_block_view::size_type memory_block_view::size() const noexcept {
        return _Mysize;
    }

    bool memory_block_view::contains(const_pointer _Ptr) const noexcept {
        if (empty() || !_Ptr) {
            return false;
        }

        return bstk::_Is_within_memory_block(_Myptr, end(), _Ptr);
    }

    bool memory_block_view::contains(const memory_block_view _Block) const noexcept {
        if (empty() || _Block.empty()) {
            return false;
        }

        return bstk::_Is_within_memory_block(_Myptr, end(), _Block._Myptr, _Block.end());
    }

    memory_block_view memory_block_view::block(const size_type _Off, size_type _Size) const {
        _Check_offset(_Off);
        _Size = (::std::min)(_Size, _Mysize - _Off); // trim block size
        return memory_block_view{_Size > 0
            ? bstk::_Adjust_address_by_offset(_Myptr, _Off) : nullptr, _Size};
    }

    void memory_block_view::swap(memory_block_view& _Other) noexcept {
        ::std::swap(_Myptr, _Other._Myptr);
        ::std::swap(_Mysize, _Other._Mysize);
    }

    bool operator==(const memory_block_view _Left, const memory_block_view _Right) noexcept {
        return _Left.begin() == _Right.begin() && _Left.size() == _Right.size();
    }
} // namespace bs