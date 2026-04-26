// lock.hpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#pragma once
#ifndef _BSTK_CONCURRENCY_LOCK_HPP_
#define _BSTK_CONCURRENCY_LOCK_HPP_
#include <bstk/core/export.hpp>

namespace bs {
    namespace bstk {
        class _Shared_lock_impl;
    } // namespace bstk

    class _BSTK_EXPORT shared_lock { // shared mutual exclusion facility
    public:
        shared_lock();
        ~shared_lock();

        shared_lock(const shared_lock&)            = delete;
        shared_lock& operator=(const shared_lock&) = delete;

        // acquires the lock in exclusive mode
        void lock() noexcept;

        // acquires the lock in shared mode
        void lock_shared() noexcept;

        // releases the lock that was acquired in exclusive mode
        void unlock() noexcept;

        // releases the lock that was acquired in shared mode
        void unlock_shared() noexcept;

    private:
        bstk::_Shared_lock_impl* _Myimpl;
    };

    template <class _LockTy>
    class lock_guard { // scope-based exclusive lock wrapper
    public:
        explicit lock_guard(_LockTy& _Lock) : _Mylock(_Lock) {
            _Mylock.lock();
        }

        ~lock_guard() {
            _Mylock.unlock();
        }

        lock_guard(const lock_guard&)            = delete;
        lock_guard& operator=(const lock_guard&) = delete;

    private:
        _LockTy& _Mylock;
    };

    template <class _LockTy>
    class shared_lock_guard { // scope-based shared lock wrapper
    public:
        explicit shared_lock_guard(_LockTy& _Lock) : _Mylock(_Lock) {
            _Mylock.lock_shared();
        }

        ~shared_lock_guard() {
            _Mylock.unlock_shared();
        }

        shared_lock_guard(const shared_lock_guard&)            = delete;
        shared_lock_guard& operator=(const shared_lock_guard&) = delete;

    private:
        _LockTy& _Mylock;
    };
} // namespace bs

#endif // _BSTK_CONCURRENCY_LOCK_HPP_