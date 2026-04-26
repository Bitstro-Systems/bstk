// test.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <atomic>
#include <bstk/concurrency/lock.hpp>
#include <gtest/gtest.h>
#include <thread>

namespace bs {
    // Revisit: Should use common concurrency features.

    enum class _Lock_mode : bool {
        _Exclusive,
        _Shared
    };

    class _Lock_guard {
    public:
        _Lock_guard(shared_lock& _Lock, const _Lock_mode _Mode) noexcept
            : _Mylock(_Lock), _Mymode(_Mode) {
            if (_Mymode == _Lock_mode::_Exclusive) {
                _Mylock.lock();
            } else {
                _Mylock.lock_shared();
            }
        }

        ~_Lock_guard() noexcept {
            if (_Mymode == _Lock_mode::_Exclusive) {
                _Mylock.unlock();
            } else {
                _Mylock.unlock_shared();
            }
        }

    private:
        shared_lock& _Mylock;
        _Lock_mode _Mymode;
    };

    ::std::jthread _Start_locking_thread(
        const _Lock_mode _Mode, shared_lock& _Lock, ::std::atomic<bool>& _Locked) {
        return ::std::jthread{
            [_Mode, &_Lock, &_Locked]{
                _Lock_guard _Guard{_Lock, _Mode}; // yields execution
                _Locked.store(true, ::std::memory_order_release);
            }
        };
    }

    void _Wait_ms(const uint32_t _Duration) {
        ::std::this_thread::sleep_for(::std::chrono::milliseconds{_Duration});
    }

    TEST(shared_lock, lock) {
        ::std::atomic<bool> _Locked = false;
        shared_lock _Lock;
        _Lock.lock();
        {
            const ::std::jthread _Thread = _Start_locking_thread(_Lock_mode::_Exclusive, _Lock, _Locked);
            _Wait_ms(50); // wait for thread start
            EXPECT_FALSE(_Locked.load(::std::memory_order_acquire));

            _Lock.unlock();
        }

        EXPECT_TRUE(_Locked.load(::std::memory_order_acquire));
    }

    TEST(shared_lock, lock_shared) {
        ::std::atomic<bool> _Locked = false;
        shared_lock _Lock;
        _Lock.lock_shared();
        {
            const ::std::jthread _Thread = _Start_locking_thread(_Lock_mode::_Shared, _Lock, _Locked);
            _Wait_ms(50); // wait for thread start
            EXPECT_TRUE(_Locked.load(::std::memory_order_acquire));
            
            _Lock.unlock_shared();
        }
    }
} // namespace bs