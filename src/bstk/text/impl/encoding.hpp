// encoding.hpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#pragma once
#ifndef _BSTK_TEXT_IMPL_ENCODING_HPP_
#define _BSTK_TEXT_IMPL_ENCODING_HPP_
#include <bstk/text/string.hpp>
#include <bstk/text/string_view.hpp>
#include <type_traits>
#if _BS_WINDOWS
#include <bstk/core/impl/tinywin.hpp>
#else // ^^^ _BS_WINDOWS ^^^ / vvv _BS_LINUX vvv
#include <cstdint>
#endif // _BS_WINDOWS

namespace bs {
    namespace bstk {
#if _BS_LINUX
        // Note: On Linux, Unicode characters are typically stored in UTF-32 encoding,
        //       which means wchar_t should be 4 bytes long. This implementation handles
        //       UCS-4 code points, which also rely on a 4-byte wchar_t type.
        static_assert(sizeof(wchar_t) == 4, "unexpected wchar_t size");
#endif // _BS_LINUX

        template <class _Elem>
        struct _Utf8_to_unicode_traits { // traits for decoding UTF-8 to Unicode
            using _Intern_type = _Elem;
            using _Extern_type = wchar_t;

            static size_t _Transcode_size(const _Elem* const _Str, const size_t _Size) noexcept {
                // calculate the required buffer size for _Str decoding (null-terminator excluded)
                if (_Size == 0) {
                    return 0;
                }

#if _BS_WINDOWS
                const int _Decoded_size = ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
                    reinterpret_cast<const char*>(_Str), static_cast<int>(_Size), nullptr, 0);
                return _Decoded_size > 0 ? static_cast<size_t>(_Decoded_size) : static_cast<size_t>(-1);
#else // ^^^ _BS_WINDOWS ^^^ / vvv _BS_LINUX vvv
                const _Elem* const _Last = _Str + _Size;
                size_t _Count            = 0;
                size_t _Trailing;
                uint8_t _Byte;
                for (; _Str != _Last; ++_Count) {
                    _Byte = static_cast<uint8_t>(*_Str);
                    if (_Byte <= 0x7F) { // 0XXXXXXX pattern, single byte
                        _Trailing = 0;
                    } else if (_Byte >= 0xC0 && _Byte <= 0xDF) { // 110XXXXX pattern, two bytes
                        _Trailing = 1;
                    } else if (_Byte <= 0xEF) { // 1110XXXX pattern, three bytes
                        _Trailing = 2;
                    } else if (_Byte <= 0xF7) { // 11110XXX pattern, four bytes
                        _Trailing = 3;
                    } else { // invalid leading byte, break
                        return static_cast<size_t>(-1);
                    }

                    if (_Last - _Str < _Trailing) { // incomplete sequence, break
                        return static_cast<size_t>(-1);
                    }

                    _Str += _Trailing;
                }

                return _Count;
#endif // _BS_WINDOWS
            }

            static bool _Transcode(const _Elem* const _Str, const size_t _Str_size,
                wchar_t* const _Buf, const size_t _Buf_size) noexcept {
                // decode the given string from UTF-8 to Unicode
#if _BS_WINDOWS
                return static_cast<size_t>(::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
                    reinterpret_cast<const char*>(_Str), static_cast<int>(_Str_size),
                        _Buf, static_cast<int>(_Buf_size))) == _Buf_size;
#else // ^^^ _BS_WINDOWS ^^^ / vvv _BS_LINUX vvv
                (void) _Buf_size;
                const _Elem* const _Last = _Str + _Str_size;
                size_t _Trailing;
                uint8_t _Byte;
                uint32_t _Least_code_point;
                uint32_t _Code_point;
                while (_Str != _Last) {
                    _Byte = static_cast<uint8_t>(*_Str++);
                    if (_Byte <= 0x7F) { // 0XXXXXXX pattern, single byte
                        _Least_code_point = 0x00;
                        _Code_point       = static_cast<uint32_t>(_Byte);
                        _Trailing         = 0;
                    } else if (_Byte >= 0xC0 && _Byte <= 0xDF) { // 110XXXXX pattern, two bytes
                        _Least_code_point = 0x80;
                        _Code_point       = static_cast<uint32_t>(_Byte & 0x1F);
                        _Trailing         = 1;
                    } else if (_Byte <= 0xEF) { // 1110XXXX pattern, three bytes
                        _Least_code_point = 0x0800;
                        _Code_point       = static_cast<uint32_t>(_Byte & 0x0F);
                        _Trailing         = 2;
                    } else if (_Byte <= 0xF7) { // 11110XXX pattern, four bytes
                        _Least_code_point = 0x0001'0000;
                        _Code_point       = static_cast<uint32_t>(_Byte & 0x07);
                        _Trailing         = 3;
                    } else { // invalid leading byte, break
                        return false;
                    }

                    if (_Last - _Str < _Trailing) { // incomplete sequence, break
                        return false;
                    }

                    while (_Trailing-- > 0) {
                        _Byte = static_cast<uint8_t>(*_Str++);
                        if (_Byte < 0x80 || _Byte > 0xBF) { // invalid trailing byte, break
                            return false;
                        }

                        _Code_point = (_Code_point << 6) | (_Byte & 0x3F);
                    }

                    if (_Code_point < _Least_code_point) { // overlong encoding detected, break
                        return false;
                    }

                    *_Buf++ = static_cast<wchar_t>(_Code_point);
                }

                return true;
#endif // _BS_WINDOWS
            }
        };

        template <class _Elem>
        struct _Unicode_to_utf8_traits { // traits for encoding Unicode to UTF-8
            using _Intern_type = wchar_t;
            using _Extern_type = _Elem;

            static size_t _Transcode_size(const wchar_t* const _Str, const size_t _Size) noexcept {
                // calculate the required buffer size for _Str encoding (null-terminator excluded)
                if (_Size == 0) {
                    return 0;
                }

#if _BS_WINDOWS
                const int _Encoded_size = ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS,
                    _Str, static_cast<int>(_Size), nullptr, 0, nullptr, nullptr);
                return _Encoded_size > 0 ? static_cast<size_t>(_Encoded_size) : static_cast<size_t>(-1);
#else // ^^^ _BS_WINDOWS ^^^ / vvv _BS_LINUX vvv
                (void) _Buf_size;
                const wchar_t* const _Last = _Str + _Size;
                size_t _Count              = 0;
                uint32_t _Code_point;
                for (; _Str != _Last; ++_Str) {
                    _Code_point = static_cast<uint32_t>(*_Str);
                    if (_Code_point <= 0x7F) { // U+0000...U+007F, single byte
                        ++_Count;
                    } else if (_Code_point <= 0x07FF) { // U+0080...U+07FF, two bytes
                        _Count += 2;
                    } else if (_Code_point <= 0xFFFF) { // U+0800...U+FFFF, three bytes
                        _Count += 3;
                    } else if (_Code_point <= 0x0010'FFFF) { // U+010000...U+01FFFF, four bytes
                        _Count += 4;
                    } else { // invalid code point (too big), break
                        return static_cast<size_t>(-1);
                    }
                }

                return _Count;
#endif // _BS_WINDOWS
            }

            static bool _Transcode(const wchar_t* const _Str, const size_t _Str_size,
                _Elem* const _Buf, const size_t _Buf_size) noexcept {
                // encode the given string Unicode to UTF-8
#if _BS_WINDOWS
                return static_cast<size_t>(::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS,
                    _Str, static_cast<int>(_Str_size), reinterpret_cast<char*>(_Buf),
                        static_cast<int>(_Buf_size), nullptr, nullptr)) == _Buf_size;
#else // ^^^ _BS_WINDOWS ^^^ / vvv _BS_LINUX vvv
                const wchar_t* const _Last = _Str + _Str_size;
                size_t _Trailing;
                uint32_t _Code_point;
                for (; _Str != _Last; ++_Str) {
                    _Code_point = static_cast<uint32_t>(*_Str);
                    if (_Code_point <= 0x7F) { // U+0000...U+007F, single byte
                        *_Buf++   = static_cast<_Elem>(_Code_point);
                        _Trailing = 0;
                    } else if (_Code_point <= 0x07FF) { // U+0080...U+07FF, two bytes
                        *_Buf++   = static_cast<_Elem>(0xC0 | (_Code_point >> 6));
                        _Trailing = 1;
                    } else if (_Code_point <= 0xFFFF) { // U+0800...U+FFFF, three bytes
                        *_Buf++   = static_cast<_Elem>(0xE0 | (_Code_point >> 12));
                        _Trailing = 2;
                    } else if (_Code_point <= 0x0010'FFFF) { // U+010000...U+10FFFF, four bytes
                        *_Buf++   = static_cast<_Elem>(0xF0 | (_Code_point >> 18));
                        _Trailing = 3;
                    } else { // invalid code point (too big), break
                        return false;
                    }

                    for (; _Trailing > 0; ++_Buf) { // append trailing bytes, if any
                        *_Buf = static_cast<_Elem>(0x80 | ((_Code_point >> (6 * --_Trailing)) & 0x3F));
                    }
                }

                return true;
#endif // _BS_WINDOWS
            }
        };

        template <class _Intern, class _Extern>
        using _Select_transcode_traits = ::std::conditional_t<::std::is_same_v<_Intern, wchar_t>,
            _Unicode_to_utf8_traits<_Extern>, _Utf8_to_unicode_traits<_Intern>>;

        template <class _Extern, class _Intern>
        string<_Extern> _Transcode_string(const string_view<_Intern> _Str) {
            using _Traits          = _Select_transcode_traits<_Intern, _Extern>;
            using _Extern_string   = string<_Extern>;
            const size_t _Buf_size = _Traits::_Transcode_size(_Str.data(), _Str.size());
            if (_Buf_size == 0 || _Buf_size == static_cast<size_t>(-1)) {
                // no transcoding needed or an error occured, break
                return _Extern_string{};
            }

            _Extern_string _Buf(_Buf_size, _Extern{0});
            return _Traits::_Transcode(_Str.data(), _Str.size(), _Buf.data(), _Buf_size)
                ? _Buf : _Extern_string{};
        }

        template <class _Extern, class _Intern>
        size_t _Transcode_string_size(const string_view<_Intern> _Str) noexcept {
            return _Select_transcode_traits<_Intern, _Extern>::_Transcode_size(_Str.data(), _Str.size());
        }
    } // namespace bstk
} // namespace bs

#endif // _BSTK_TEXT_IMPL_ENCODING_HPP_