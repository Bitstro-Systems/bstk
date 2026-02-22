// reference_counter.hpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#pragma once
#ifndef _BSTK_MEMORY_REFERENCE_COUNTER_HPP_
#define _BSTK_MEMORY_REFERENCE_COUNTER_HPP_
#include <atomic>
#include <bstk/core/export.hpp>

namespace bs {
    class _BSTK_EXPORT reference_counter { // thread-safe reference counter
    public:
        reference_counter() noexcept;

        explicit reference_counter(const long _Refs) noexcept;

        ~reference_counter() noexcept = default;

        reference_counter(const reference_counter&)            = delete;
        reference_counter& operator=(const reference_counter&) = delete;

        // increments the number of references
        void increment() noexcept;

        // decrements the number of references
        long decrement() noexcept;

        // returns the number of references
        long use_count() const noexcept;

    private:
        ::std::atomic<long> _Myrefs;
    };
} // namespace bs

#endif // _BSTK_MEMORY_REFERENCE_COUNTER_HPP_