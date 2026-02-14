// exception.hpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#pragma once
#ifndef _BSTK_CORE_EXCEPTION_HPP_
#define _BSTK_CORE_EXCEPTION_HPP_
#include <bstk/core/export.hpp>

namespace bs {
    class _BSTK_EXPORT allocation_failure {
    public:
        allocation_failure() noexcept                          = default;
        allocation_failure(const allocation_failure&) noexcept = default;
        allocation_failure(allocation_failure&&) noexcept      = default;
        ~allocation_failure() noexcept                         = default;

        allocation_failure& operator=(const allocation_failure&) noexcept = default;
        allocation_failure& operator=(allocation_failure&&) noexcept      = default;

        // raises an exception of this type
        [[noreturn]] static void raise();
    };

    class _BSTK_EXPORT allocation_limit_exceeded {
    public:
        allocation_limit_exceeded() noexcept                                 = default;
        allocation_limit_exceeded(const allocation_limit_exceeded&) noexcept = default;
        allocation_limit_exceeded(allocation_limit_exceeded&&) noexcept      = default;
        ~allocation_limit_exceeded() noexcept                                = default;

        allocation_limit_exceeded& operator=(const allocation_limit_exceeded&) noexcept = default;
        allocation_limit_exceeded& operator=(allocation_limit_exceeded&&) noexcept      = default;

        // raises an exception of this type
        [[noreturn]] static void raise();
    };

    class _BSTK_EXPORT resource_overrun {
    public:
        resource_overrun() noexcept                        = default;
        resource_overrun(const resource_overrun&) noexcept = default;
        resource_overrun(resource_overrun&&) noexcept      = default;
        ~resource_overrun() noexcept                       = default;

        resource_overrun& operator=(const resource_overrun&) noexcept = default;
        resource_overrun& operator=(resource_overrun&&) noexcept      = default;

        // raises an exception of this type
        [[noreturn]] static void raise();
    };
} // namespace bs

#endif // _BSTK_CORE_EXCEPTION_HPP_