// system_allocator.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/core/exception.hpp>
#include <bstk/core/impl/utility.hpp>
#include <bstk/memory/system_allocator.hpp>
#include <new>

namespace bs {
    allocator_id system_allocator::id() const noexcept {
        return allocator_id::system;
    }

    system_allocator::size_type system_allocator::max_size() const noexcept {
        return static_cast<size_type>(-1);
    }

    bool system_allocator::is_equal(const allocator& _Other) const noexcept {
        // cast to a pointer to avoid a bad_cast exception
        return dynamic_cast<const system_allocator*>(::std::addressof(_Other)) != nullptr;
    }

    system_allocator::pointer system_allocator::allocate(size_type _Size, size_type _Align) {
        if (_Size == 0) { // no allocation, do nothing
            return nullptr;
        }

        if (_Size > max_size()) { // requested size exceeds the limit, raise an exception
            allocation_limit_exceeded::raise();
        }

#ifdef _DEBUG
        _INTERNAL_ASSERT(bstk::_Is_zero_or_pow_of_2(_Align), "alignment must be a power of 2");
#endif // _DEBUG
        void* _Ptr = nullptr;
        if (_Align != 0) { // use the supplied alignment
            _Ptr = ::operator new(_Size, ::std::align_val_t{_Align}, ::std::nothrow);
        } else { // use the default alignment
            _Ptr = ::operator new(_Size, ::std::nothrow);
        }

        if (!_Ptr) { // allocation failed, raise an exception
            allocation_failure::raise();
        }

        return _Ptr;
    }

    void system_allocator::deallocate(pointer _Ptr, size_type _Size, size_type _Align) {
        if (_Ptr && _Size > 0) { // valid block, attempt to deallocate it
#ifdef _DEBUG
            _INTERNAL_ASSERT(bstk::_Is_zero_or_pow_of_2(_Align), "alignment must be a power of 2");
#endif // _DEBUG
            if (_Align != 0) { // use the supplied alignment
                ::operator delete(_Ptr, _Size, ::std::align_val_t{_Align});
            } else { // use the default alignment
                ::operator delete(_Ptr, _Size);
            }
        }
    }
} // namespace bs