// shared_lock.hpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#pragma once
#ifndef _BSTK_CONCURRENCY_IMPL_SHARED_LOCK_HPP_
#define _BSTK_CONCURRENCY_IMPL_SHARED_LOCK_HPP_
#if _BS_WINDOWS
#include <bstk/core/impl/tinywin.hpp>
#else // ^^^ _BS_WINDOWS ^^^ / vvv _BS_LINUX vvv
#include <bstk/core/impl/utility.hpp>
#include <pthread.h>
#endif // _BS_WINDOWS

namespace bs {
    namespace bstk {
        class _Shared_lock_impl {
        public:
            _Shared_lock_impl() noexcept : _Myhandle{0} {
#if _BS_LINUX
                _INTERNAL_ASSERT(::pthread_rwlock_init(
                    &_Myhandle, nullptr) == 0, "failed to initialize read-write Lock");
#endif // _BS_LINUX
            }

            ~_Shared_lock_impl() noexcept {
#if _BS_LINUX
                _INTERNAL_ASSERT(::pthread_rwlock_destroy(&_Myhandle) == 0, "failed to destroy read-write Lock");
#endif // _BS_LINUX
            }

            void _Lock() noexcept {
#if _BS_WINDOWS
                ::AcquireSRWLockExclusive(&_Myhandle);
#else // ^^^ _BS_WINDOWS ^^^ / vvv _BS_LINUX vvv
                _INTERNAL_ASSERT(::pthread_rwlock_wrlock(&_Myhandle) == 0, "failed to acquire write lock");
#endif // _BS_WINDOWS
            }

            void _Lock_shared() noexcept {
#if _BS_WINDOWS
                ::AcquireSRWLockShared(&_Myhandle);
#else // ^^^ _BS_WINDOWS ^^^ / vvv _BS_LINUX vvv
                _INTERNAL_ASSERT(::pthread_rwlock_rdlock(&_Myhandle) == 0, "failed to acquire read lock");
#endif // _BS_WINDOWS
            }

            void _Unlock() noexcept {
#if _BS_WINDOWS
                _Analysis_assume_lock_acquired_(_Myhandle);
                ::ReleaseSRWLockExclusive(&_Myhandle);
#else // ^^^ _BS_WINDOWS ^^^ / vvv _BS_LINUX vvv
                _INTERNAL_ASSERT(::pthread_rwlock_unlock(&_Myhandle) == 0, "failed to release write lock");
#endif // _BS_WINDOWS
            }

            void _Unlock_shared() noexcept {
#if _BS_WINDOWS
                _Analysis_assume_lock_acquired_(_Myhandle);
                ::ReleaseSRWLockShared(&_Myhandle);
#else // ^^^ _BS_WINDOWS ^^^ / vvv _BS_LINUX vvv
                _INTERNAL_ASSERT(::pthread_rwlock_unlock(&_Myhandle) == 0, "failed to release read lock");
#endif // _BS_WINDOWS
            }

        private:
#if _BS_WINDOWS
            SRWLOCK _Myhandle;
#else // ^^^ _BS_WINDOWS ^^^ / vvv _BS_LINUX vvv
            pthread_rwlock_t _Myhandle;
#endif // _BS_WINDOWS
        };
    } // namespace bstk
} // namespace bs

#endif // _BSTK_CONCURRENCY_IMPL_SHARED_LOCK_HPP_