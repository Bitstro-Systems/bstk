// sync_flag.hpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#pragma once
#ifndef _BSTK_CONCURRENCY_SYNC_FLAG_HPP_
#define _BSTK_CONCURRENCY_SYNC_FLAG_HPP_
#include <atomic>
#include <bstk/core/export.hpp>

namespace bs {
    class _BSTK_EXPORT sync_flag { // atomic flag for thread synchronization
    public:
        sync_flag() noexcept;
        explicit sync_flag(const bool _Value) noexcept;

        ~sync_flag() noexcept = default;

        sync_flag(const sync_flag&)            = delete;
        sync_flag& operator=(const sync_flag&) = delete;

        // checks if the flag is set
        bool is_set(const ::std::memory_order _Order = ::std::memory_order_seq_cst) const noexcept;

        // clears the flag
        void clear(const ::std::memory_order _Order = ::std::memory_order_seq_cst) noexcept;

        // sets the flag and obtains its previous value
        bool set(const ::std::memory_order _Order = ::std::memory_order_seq_cst) noexcept;

    private:
        ::std::atomic<bool> _Myvalue;
    };
} // namespace bs

#endif // _BSTK_CONCURRENCY_SYNC_FLAG_HPP_