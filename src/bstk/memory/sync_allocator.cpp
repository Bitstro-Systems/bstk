// sync_allocator.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/memory/sync_allocator.hpp>

namespace bs {
    sync_allocator::sync_allocator(allocator& _Al) noexcept : _Myal(_Al) {}

    allocator_id sync_allocator::id() const noexcept {
        return allocator_id::sync;
    }

    sync_allocator::size_type sync_allocator::max_size() const noexcept {
        return _Invoke_thread_safe(
            [this]() noexcept {
                return _Myal.max_size();
            }
        );
    }

    bool sync_allocator::is_equal(const allocator& _Other) const noexcept {
        return _Invoke_thread_safe(
            [this, &_Other]() noexcept {
                return _Myal.is_equal(_Other);
            }
        );
    }

    bool sync_allocator::is_thread_safe() const noexcept {
        return true;
    }

    [[nodiscard]] sync_allocator::pointer sync_allocator::allocate(size_type _Size, size_type _Align) {
        return _Invoke_thread_safe(
            [this, _Size, _Align] {
                return _Myal.allocate(_Size, _Align);
            }
        );
    }

    void sync_allocator::deallocate(pointer _Ptr, size_type _Size, size_type _Align) {
        _Invoke_thread_safe(
            [this, _Ptr, _Size, _Align] {
                _Myal.deallocate(_Ptr, _Size, _Align);
            }
        );
    }
} // namespace bs