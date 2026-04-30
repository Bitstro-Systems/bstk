// sync_flag.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/concurrency/sync_flag.hpp>

namespace bs {
    sync_flag::sync_flag() noexcept : _Myvalue(false) {}

    sync_flag::sync_flag(const bool _Value) noexcept : _Myvalue(_Value) {}

    bool sync_flag::is_set(const ::std::memory_order _Order) const noexcept {
        return _Myvalue.load(_Order);
    }

    void sync_flag::clear(const ::std::memory_order _Order) noexcept {
        _Myvalue.store(false, _Order);
    }

    bool sync_flag::set(const ::std::memory_order _Order) noexcept {
        return _Myvalue.exchange(true, _Order);
    }
} // namespace bs