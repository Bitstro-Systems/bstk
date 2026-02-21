// bitmask.hpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#pragma once
#ifndef _BSTK_CORE_BITMASK_HPP_
#define _BSTK_CORE_BITMASK_HPP_
#include <type_traits>

namespace bs {
    template <class _Ty>
    concept enumeration = ::std::is_enum_v<_Ty>;

    template <enumeration _Enum>
    constexpr _Enum operator|(const _Enum _Left, const _Enum _Right) noexcept {
        using _Integer = ::std::underlying_type_t<_Enum>;
        return static_cast<_Enum>(static_cast<_Integer>(_Left) | static_cast<_Integer>(_Right));
    }

    template <enumeration _Enum>
    constexpr _Enum operator&(const _Enum _Left, const _Enum _Right) noexcept {
        using _Integer = ::std::underlying_type_t<_Enum>;
        return static_cast<_Enum>(static_cast<_Integer>(_Left) & static_cast<_Integer>(_Right));
    }

    template <enumeration _Enum>
    constexpr _Enum operator^(const _Enum _Left, const _Enum _Right) noexcept {
        using _Integer = ::std::underlying_type_t<_Enum>;
        return static_cast<_Enum>(static_cast<_Integer>(_Left) ^ static_cast<_Integer>(_Right));
    }

    template <enumeration _Enum>
    constexpr _Enum operator~(const _Enum _Value) noexcept {
        using _Integer = ::std::underlying_type_t<_Enum>;
        return static_cast<_Enum>(~static_cast<_Integer>(_Value));
    }

    template <enumeration _Enum>
    constexpr _Enum& operator|=(_Enum& _Mask, const _Enum _Value) noexcept {
        _Mask = _Mask | _Value;
        return _Mask;
    }

    template <enumeration _Enum>
    constexpr _Enum& operator&=(_Enum& _Mask, const _Enum _Value) noexcept {
        _Mask = _Mask & _Value;
        return _Mask;
    }

    template <enumeration _Enum>
    constexpr _Enum& operator^=(_Enum& _Mask, const _Enum _Value) noexcept {
        _Mask = _Mask ^ _Value;
        return _Mask;
    }

    template <class _Ty>
    constexpr bool bitmask_contains_any(const _Ty _Mask, const _Ty _Value) noexcept {
        // check whether any bit set in _Value is also set in _Mask
        return (_Mask & _Value) != _Ty{0};
    }

    template <class _Ty>
    constexpr bool bitmask_contains_all(const _Ty _Mask, const _Ty _Value) noexcept {
        // check whether all bits set in _Value are also set in _Mask
        return (_Mask & _Value) == _Value;
    }
} // namespace bs

#endif // _BSTK_CORE_BITMASK_HPP_