// char_traits.hpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#pragma once
#ifndef _BSTK_TEXT_CHAR_TRAITS_HPP_
#define _BSTK_TEXT_CHAR_TRAITS_HPP_
#include <bstk/core/export.hpp>
#include <compare>
#include <cstddef>
#include <type_traits>

namespace bs {
    using byte_t = unsigned char;

    namespace bstk {
        template <class _Ty, class... _Types>
        concept _Any_of = ::std::disjunction_v<::std::is_same<_Ty, _Types>...>;

        template <class _Elem>
        concept _Char_traits_element = _Any_of<_Elem, byte_t, char, wchar_t>;
    } // namespace bstk

    template <class _Elem>
    struct _BSTK_EXPORT char_traits { // properties of a character type
        static_assert(bstk::_Char_traits_element<_Elem>, "invalid element type for char_traits<_Elem>");

        using char_type           = _Elem;
        using comparison_category = ::std::strong_ordering;

        static constexpr size_t npos = static_cast<size_t>(-1);

        // assigns a character
        static void assign(char_type* const _Dest, const size_t _Count, const char_type _Ch) noexcept;

        // lexicographically compares two character sequence
        static int compare(
            const char_type* const _Left, const char_type* const _Right, const size_t _Count) noexcept;

        // checks whether two character sequence are equal
        static bool eq(const char_type* const _Left,
            const char_type* const _Right, const size_t _Count) noexcept;

        // moves one character sequence onto another
        static void move(char_type* const _Dest, const char_type* const _Src, const size_t _Count) noexcept;

        // copies a character sequence
        static void copy(char_type* const _Dest, const char_type* const _Src, const size_t _Count) noexcept;

        // returns the length of a character sequence
        static size_t length(const char_type* const _Str) noexcept;

        // finds characters in a character sequence
        static size_t find(const char_type* const _Str, const size_t _Size, const char_type _Ch) noexcept;
        static size_t find(const char_type* const _Str, const size_t _Str_size,
            const char_type* const _Substr, const size_t _Substr_size) noexcept;

        // finds characters in a character sequence, searchs from the end
        static size_t rfind(const char_type* const _Str,const size_t _Size, const char_type _Ch) noexcept;
        static size_t rfind(const char_type* const _Str, const size_t _Str_size,
            const char_type* const _Substr, const size_t _Substr_size) noexcept;
    };
} // namespace bs

#endif // _BSTK_TEXT_CHAR_TRAITS_HPP_