// utility.hpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#pragma once
#ifndef _BSTK_CORE_IMPL_UTILITY_HPP_
#define _BSTK_CORE_IMPL_UTILITY_HPP_
#include <type_traits>
#if _BS_WINDOWS
#include <crtdbg.h>
#else // ^^^ _BS_WINDOWS ^^^ / vvv _BS_LINUX vvv
#include <cstdio>
#include <cstdlib>
#endif // _BS_WINDOWS

// generic assertion macros, useful in debug mode
#if _BS_WINDOWS
#define _REPORT_ERROR(_Fmt, ...) \
    ::_CrtDbgReport(_CRT_ERROR, __FILE__, __LINE__, nullptr, _Fmt, __VA_ARGS__)
#else // ^^^ _BS_WINDOWS ^^^ / vvv _BS_LINUX vvv
#define _REPORT_ERROR(_Fmt, ...)                                               \
    ::fprintf(stderr, "%s:%d: %s: ", __FILE__, __LINE__, __PRETTY_FUNCTION__); \
    ::fprintf(stderr, _Fmt, __VA_ARGS__);                                      \
    ::abort()
#endif // _BS_WINDOWS

#define _INTERNAL_ASSERT(_Cond, _Fmt, ...) \
    if (!(_Cond)) {                        \
        _REPORT_ERROR(_Fmt, __VA_ARGS__);  \
    }

namespace bs {
    namespace bstk {
        template <class _Integer>
        constexpr bool _Is_zero_or_pow_of_2(const _Integer _Value) noexcept {
            // check if the given value is zero or a power of 2 (only one bit is set)
            static_assert(::std::is_unsigned_v<_Integer>, "integer must be unsigned");
            return (_Value & (_Value - 1)) == 0;
        }

        template <class _Integer>
        constexpr bool _Is_pow_of_2(const _Integer _Value) noexcept {
            // check if the given value is a power of 2 (only one bit is set)
            static_assert(::std::is_unsigned_v<_Integer>, "integer must be unsigned");
            return (_Value ^ (_Value - 1)) > _Value - 1;
        }
    } // namespace bstk
} // namespace bs

#endif // _BSTK_CORE_IMPL_UTILITY_HPP_