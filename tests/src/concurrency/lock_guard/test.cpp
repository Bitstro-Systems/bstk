// test.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/concurrency/lock.hpp>
#include <gtest/gtest.h>

namespace bs {
    class _Fake_lock {
    public:
        _Fake_lock() noexcept : _Mylocks(0), _Myunlocks(0) {}

        ~_Fake_lock() noexcept = default;

        _Fake_lock(const _Fake_lock&)            = delete;
        _Fake_lock& operator=(const _Fake_lock&) = delete;

        size_t _Get_locks() const noexcept {
            return _Mylocks;
        }

        size_t _Get_unlocks() const noexcept {
            return _Myunlocks;
        }

        void lock() noexcept {
            ++_Mylocks;
        }

        void unlock() noexcept {
            ++_Myunlocks;
        }

    private:
        size_t _Mylocks;
        size_t _Myunlocks;
    };

    TEST(lock_guard, locking) {
        _Fake_lock _Lock;
        EXPECT_EQ(_Lock._Get_locks(), 0);
        EXPECT_EQ(_Lock._Get_unlocks(), 0);
        {
            lock_guard _Guard(_Lock);
            EXPECT_EQ(_Lock._Get_locks(), 1);
            EXPECT_EQ(_Lock._Get_unlocks(), 0);
        }

        EXPECT_EQ(_Lock._Get_locks(), 1);
        EXPECT_EQ(_Lock._Get_unlocks(), 1);
    }
} // namespace bs