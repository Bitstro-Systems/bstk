// endian.hpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#pragma once
#ifndef _BSTK_MEMORY_ENDIAN_HPP_
#define _BSTK_MEMORY_ENDIAN_HPP_
#include <bstk/core/utility.hpp>
#include <cstdint>
#include <type_traits>
#if _BS_MSVC
#include <cstdlib>
#endif // _BS_MSVC

namespace bs {
#if _BS_INT128_SUPPORTED
    using int128_t  = __int128;
    using uint128_t = unsigned __int128;
#endif // _BS_INT128_SUPPORTED

    template <class _Ty>
    concept integral = ::std::disjunction_v<::std::is_integral<_Ty>
#if _BS_INT128_SUPPORTED
        , ::std::is_same<_Ty, int128_t>, ::std::is_same<_Ty, uint128_t>
#endif // _BS_INT128_SUPPORTED
    >;

    enum class endian : unsigned char {
        little = 0,
        big    = 1,
#if _BS_BIG_ENDIAN
        native = big
#else // ^^^ _BS_BIG_ENDIAN ^^^ / vvv _BS_LITTLE_ENDIAN vvv
        native = little
#endif // _BS_BIG_ENDIAN
    };

    // Note: The functions below should be placed in an internal header as they are
    //       part of the implementation details. However, to maintain swap_endian() constexpr,
    //       these functions must be implemented in a public header and should not be exported.
    namespace bstk {
        constexpr uint16_t _Bswap16(const uint16_t _Value) noexcept {
#if _BS_GCC
            return __builtin_bswap16(_Value);
#else // ^^^ _BS_GCC ^^^ / vvv _BS_MSVC vvv
            if (!::std::is_constant_evaluated()) {
                return ::_byteswap_ushort(_Value);
            } else {
                return (_Value >> 8) | (_Value << 8);
            }
#endif // _BS_GCC
        }

        constexpr uint32_t _Bswap32(const uint32_t _Value) noexcept {
#if _BS_GCC
            return __builtin_bswap32(_Value);
#else // ^^^ _BS_GCC ^^^ / vvv _BS_MSVC vvv
            if (!::std::is_constant_evaluated()) {
                return ::_byteswap_ulong(_Value);
            } else {
                return (_Value >> 24) | ((_Value & 0x00FF'0000) >> 8)
                    | ((_Value & 0x0000'FF00) << 8) | (_Value << 24);
            }
#endif // _BS_GCC
        }

        constexpr uint64_t _Bswap64(const uint64_t _Value) noexcept {
#if _BS_GCC
            return __builtin_bswap64(_Value);
#else // ^^^ _BS_GCC ^^^ / vvv _BS_MSVC vvv
            if (!::std::is_constant_evaluated()) {
                return ::_byteswap_uint64(_Value);
            } else {
                return (_Value >> 56) | ((_Value & 0x00FF'0000'0000'0000) >> 40)
                    | ((_Value & 0x0000'FF00'0000'0000) >> 24) | ((_Value & 0x0000'00FF'0000'0000) >> 8)
                    | ((_Value & 0x0000'0000'FF00'0000) << 8) | ((_Value & 0x0000'0000'00FF'0000) << 24)
                    | ((_Value & 0x0000'0000'0000'FF00) << 40) | (_Value << 56);
            }
#endif // _BS_GCC
        }

#if _BS_INT128_SUPPORTED
        constexpr uint128_t _Bswap128(const uint128_t _Value) noexcept {
#if _BS_GCC
            return __builtin_bswap128(_Value);
#else // ^^^ _BS_GCC ^^^ / vvv _BS_MSVC vvv
            return (static_cast<uint128_t>(
                _Bswap64(static_cast<uint64_t>(_Value & 0xFFFF'FFFF'FFFF'FFFF))) << 64)
                | static_cast<uint128_t>(_Bswap64(static_cast<uint64_t>(_Value >> 64)));
#endif // _BS_GCC
        }
#endif // _BS_INT128_SUPPORTED
    } // namespace bstk

    template <integral _Ty>
    constexpr _Ty swap_endian(const _Ty _Value) noexcept {
        // reverse the bytes of the given integral
        if constexpr (sizeof(_Ty) == 1) {
            return _Value;
        } else if constexpr (sizeof(_Ty) == 2) {
            return static_cast<_Ty>(bstk::_Bswap16(static_cast<uint16_t>(_Value)));
        } else if constexpr (sizeof(_Ty) == 4) {
            return static_cast<_Ty>(bstk::_Bswap32(static_cast<uint32_t>(_Value)));
        } else if constexpr (sizeof(_Ty) == 8) {
            return static_cast<_Ty>(bstk::_Bswap64(static_cast<uint64_t>(_Value)));
        }
#if _BS_INT128_SUPPORTED
        else if constexpr (sizeof(_Ty) == 16) {
            return static_cast<_Ty>(bstk::_Bswap128(static_cast<uint128_t>(_Value)));
        }
#endif // _BS_INT128_SUPPORTED
        else {
            static_assert(always_false<_Ty>::value, "unexpected integral size");
        }
    }

    template <integral _Ty>
    constexpr _Ty to_big_endian(const _Ty _Value) noexcept {
#if _BS_BIG_ENDIAN
        return _Value;
#else // ^^^ _BS_BIG_ENDIAN ^^^ / vvv _BS_LITTLE_ENDIAN vvv
        return ::bs::swap_endian(_Value);
#endif // _BS_BIG_ENDIAN
    }

    template <integral _Ty>
    constexpr _Ty to_little_endian(const _Ty _Value) noexcept {
#if _BS_LITTLE_ENDIAN
        return _Value;
#else // ^^^ _BS_LITTLE_ENDIAN ^^^ / vvv _BS_BIG_ENDIAN vvv
        return ::bs::swap_endian(_Value);
#endif // _BS_LITTLE_ENDIAN
    }

    template <integral _Ty>
    constexpr _Ty from_big_endian(const _Ty _Value) noexcept {
#if _BS_BIG_ENDIAN
        return _Value;
#else // ^^^ _BS_BIG_ENDIAN ^^^ / vvv _BS_LITTLE_ENDIAN vvv
        return ::bs::swap_endian(_Value);
#endif // _BS_BIG_ENDIAN
    }

    template <integral _Ty>
    constexpr _Ty from_little_endian(const _Ty _Value) noexcept {
#if _BS_LITTLE_ENDIAN
        return _Value;
#else // ^^^ _BS_LITTLE_ENDIAN ^^^ / vvv _BS_BIG_ENDIAN vvv
        return ::bs::swap_endian(_Value);
#endif // _BS_LITTLE_ENDIAN
    }
} // namespace bs

#endif // _BSTK_MEMORY_ENDIAN_HPP_