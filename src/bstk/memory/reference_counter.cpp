// reference_counter.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/memory/reference_counter.hpp>

namespace bs {
    reference_counter::reference_counter() noexcept : _Myrefs(0) {}

    reference_counter::reference_counter(const long _Refs) noexcept : _Myrefs(_Refs) {}

    void reference_counter::increment() noexcept {
        // avoid generating a full memory barrier or fence
        _Myrefs.fetch_add(1, ::std::memory_order_acq_rel);
    }

    long reference_counter::decrement() noexcept {
        // avoid generating a full memory barrier or fence
        return _Myrefs.fetch_sub(1, ::std::memory_order_acq_rel) - 1;
    }

    long reference_counter::use_count() const noexcept {
        return _Myrefs.load(::std::memory_order_acquire);
    }
} // namespace bs