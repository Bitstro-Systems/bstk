// shared_resource.hpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#pragma once
#ifndef _BSTK_CONCURRENCY_SHARED_RESOURCE_HPP_
#define _BSTK_CONCURRENCY_SHARED_RESOURCE_HPP_
#include <bstk/concurrency/lock.hpp>
#include <utility>
#include <type_traits>

namespace bs {
    template <class _Ty>
    class shared_resource { // manages access to a shared resource
    public:
        using value_type      = _Ty;
        using reference       = _Ty&;
        using const_reference = const _Ty&;

        constexpr shared_resource() noexcept(::std::is_nothrow_default_constructible_v<_Ty>)
            : _Myvalue(), _Mylock() {}

        template <class... _Types>
        constexpr explicit shared_resource(_Types&&... _Args)
            noexcept(::std::is_nothrow_constructible_v<_Ty, _Types...>)
            : _Myvalue(::std::forward<_Types>(_Args)...), _Mylock() {}

        constexpr ~shared_resource() noexcept(::std::is_nothrow_destructible_v<_Ty>) = default;

        shared_resource(const shared_resource&)            = delete;
        shared_resource& operator=(const shared_resource&) = delete;

        template <class _Visitor>
        constexpr decltype(auto) visit(_Visitor&& _Vis)
            noexcept(::std::is_nothrow_invocable_v<_Visitor, _Ty&>) {
            lock_guard _Guard(_Mylock);
            return ::std::forward<_Visitor>(_Vis)(_Myvalue);
        }

        template <class _Visitor>
        constexpr decltype(auto) visit(_Visitor&& _Vis) const
            noexcept(::std::is_nothrow_invocable_v<_Visitor, const _Ty&>) {
            shared_lock_guard _Guard(_Mylock);
            return ::std::forward<_Visitor>(_Vis)(_Myvalue);
        }

    private:
        _Ty _Myvalue;
        mutable shared_lock _Mylock;
    };
} // namespace bs

#endif // _BSTK_CONCURRENCY_SHARED_RESOURCE_HPP_