// sync_allocator.hpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#pragma once
#ifndef _BSTK_MEMORY_SYNC_ALLOCATOR_HPP_
#define _BSTK_MEMORY_SYNC_ALLOCATOR_HPP_
#include <bstk/concurrency/lock.hpp>
#include <bstk/core/export.hpp>
#include <bstk/memory/allocator.hpp>
#include <utility>

namespace bs {
    class _BSTK_EXPORT sync_allocator : public allocator { // thread-safe wrapper around a provided allocator
    public:
        using value_type      = allocator::value_type;
        using size_type       = allocator::size_type;
        using difference_type = allocator::difference_type;
        using pointer         = allocator::pointer;
        using const_pointer   = allocator::const_pointer;

        explicit sync_allocator(allocator& _Al) noexcept;

        ~sync_allocator() noexcept override = default;

        sync_allocator()                                 = delete;
        sync_allocator(const sync_allocator&)            = delete;
        sync_allocator& operator=(const sync_allocator&) = delete;

        // returns the ID that identifies the allocator type
        allocator_id id() const noexcept override;
        
        // returns the largest supported allocation size
        size_type max_size() const noexcept override;

        // compares for equality with another allocator
        bool is_equal(const allocator& _Other) const noexcept override;

        // indicates whether concurrent access is thread-safe
        bool is_thread_safe() const noexcept override;

        // allocates uninitialized storage with optional alignment
        [[nodiscard]] pointer allocate(size_type _Size, size_type _Align = 0) override;

        // deallocates storage with optional alignment
        void deallocate(pointer _Ptr, size_type _Size, size_type _Align = 0) override;

    private:
        template <class _Fn>
        decltype(auto) _Invoke_thread_safe(_Fn&& _Func) noexcept(::std::is_nothrow_invocable_v<_Fn>) {
            // invoke _Func thread-safely, locking if the wrapped allocator is not thread-safe
            if (_Myal.is_thread_safe()) {
                return ::std::forward<_Fn>(_Func)();
            } else {
                lock_guard _Guard(_Mylock);
                return ::std::forward<_Fn>(_Func)();
            }
        }

        template <class _Fn>
        decltype(auto) _Invoke_thread_safe(_Fn&& _Func) const noexcept(::std::is_nothrow_invocable_v<_Fn>) {
            // invoke _Func thread-safely, locking if the wrapped allocator is not thread-safe
            if (_Myal.is_thread_safe()) {
                return ::std::forward<_Fn>(_Func)();
            } else {
                shared_lock_guard _Guard(_Mylock);
                return ::std::forward<_Fn>(_Func)();
            }
        }

        allocator& _Myal;
        mutable shared_lock _Mylock;
    };
} // namespace bs

#endif // _BSTK_MEMORY_SYNC_ALLOCATOR_HPP_