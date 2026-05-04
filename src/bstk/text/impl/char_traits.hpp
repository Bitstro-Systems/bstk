// char_traits.hpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#pragma once
#ifndef _BSTK_TEXT_IMPL_CHAR_TRAITS_HPP_
#define _BSTK_TEXT_IMPL_CHAR_TRAITS_HPP_
#include <cstring>
#include <cwchar>
#include <type_traits>

namespace bs {
    namespace bstk {
        template <class _Elem>
        struct _Narrow_char_traits { // properties of 1-byte characters
            static void _Assign(_Elem* const _Dest, const size_t _Count, const _Elem _Ch) noexcept {
#if _BS_GCC
                __builtin_memset(_Dest, _Ch, _Count);
#else // ^^^ _BS_GCC ^^^ / vvv _BS_MSVC vvv
                ::memset(_Dest, _Ch, _Count);
#endif // _BS_GCC
            }

            static int _Compare(const _Elem* const _Left,
                const _Elem* const _Right, const size_t _Count) noexcept {
                return __builtin_memcmp(_Left, _Right, _Count);
            }

            static void _Move(_Elem* const _Dest, const _Elem* const _Src, const size_t _Count) noexcept {
#if _BS_GCC
                __builtin_memmove(_Dest, _Src, _Count);
#else // ^^^ _BS_GCC ^^^ / vvv _BS_MSVC vvv
                ::memmove(_Dest, _Src, _Count);
#endif // _BS_GCC
            }

            static void _Copy(_Elem* const _Dest, const _Elem* const _Src, const size_t _Count) noexcept {
#if _BS_GCC
                __builtin_memcpy(_Dest, _Src, _Count);
#else // ^^^ _BS_GCC ^^^ / vvv _BS_MSVC vvv
                ::memcpy(_Dest, _Src, _Count);
#endif // _BS_GCC
            }

            static size_t _Length(const _Elem* const _Str) noexcept {
                return __builtin_strlen(reinterpret_cast<const char*>(_Str));
            }

            static size_t _Find(const _Elem* const _Str, const size_t _Size, const _Elem _Ch) noexcept {
                const _Elem* const _Substr = reinterpret_cast<const _Elem*>(
#if _BS_MSVC
                    __builtin_char_memchr(reinterpret_cast<const char*>(_Str), _Ch, _Size)
#else // ^^^ _BS_MSVC ^^^ / vvv _BS_GCC vvv
                    __builtin_memchr(_Str, _Ch, _Size)
#endif // _BS_MSVC
                );
                return _Substr ? _Substr - _Str : static_cast<size_t>(-1);
            }
        };

        struct _Wide_char_traits { // properties of wchar_t characters
            static void _Assign(wchar_t* const _Dest, const size_t _Count, const wchar_t _Ch) noexcept {
                ::wmemset(_Dest, _Ch, _Count);
            }

            static int _Compare(const wchar_t* const _Left,
                const wchar_t* const _Right, const size_t _Count) noexcept {
#if _BS_MSVC
                return __builtin_wmemcmp(_Left, _Right, _Count);
#else // ^^^ _BS_MSVC ^^^ / vvv _BS_GCC vvv
                return ::wmemcmp(_Left, _Right, _Count);
#endif // _BS_MSVC
            }

            static void _Move(wchar_t* const _Dest, const wchar_t* const _Src, const size_t _Count) noexcept {
                ::wmemmove(_Dest, _Src, _Count);
            }

            static void _Copy(wchar_t* const _Dest, const wchar_t* const _Src, const size_t _Count) noexcept {
                ::wmemcpy(_Dest, _Src, _Count);
            }

            static size_t _Length(const wchar_t* const _Str) noexcept {
#if _BS_MSVC
                return __builtin_wcslen(_Str);
#else // ^^^ _BS_MSVC ^^^ / vvv _BS_GCC vvv
                return ::wcslen(_Str);
#endif // _BS_MSVC
            }

            static size_t _Find(const wchar_t* const _Str, const size_t _Size, const wchar_t _Ch) noexcept {
#if _BS_MSVC
                const wchar_t* const _Substr = __builtin_wmemchr(_Str, _Ch, _Size);
#else // ^^^ _BS_MSVC ^^^ / vvv _BS_GCC vvv
                const wchar_t* const _Substr = ::wmemchr(_Str, _Ch, _Size);
#endif // _BS_MSVC
                return _Substr ? _Substr - _Str : static_cast<size_t>(-1);
            }
        };

        template <class _Elem>
        using _Char_traits = ::std::conditional_t<sizeof(_Elem) == 1,
            _Narrow_char_traits<_Elem>, _Wide_char_traits>;
    } // namespace bstk
} // namespace bs

#endif // _BSTK_TEXT_IMPL_CHAR_TRAITS_HPP_