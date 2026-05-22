// encoding.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/text/encoding.hpp>
#include <bstk/text/impl/encoding.hpp>

namespace bs {
    size_t byte_string_size(const utf8_string_view _Str) noexcept {
        return _Str.size();
    }

    size_t byte_string_size(const unicode_string_view _Str) noexcept {
        return bstk::_Transcode_string_size<byte_t>(_Str);
    }

    size_t utf8_string_size(const byte_string_view _Str) noexcept {
        return _Str.size();
    }

    size_t utf8_string_size(const unicode_string_view _Str) noexcept {
        return bstk::_Transcode_string_size<char>(_Str);
    }

    size_t unicode_string_size(const byte_string_view _Str) noexcept {
        return bstk::_Transcode_string_size<wchar_t>(_Str);
    }

    size_t unicode_string_size(const utf8_string_view _Str) noexcept {
        return bstk::_Transcode_string_size<wchar_t>(_Str);
    }

    byte_string to_byte_string(const utf8_string_view _Str) {
        return byte_string{reinterpret_cast<const byte_t*>(_Str.data()), _Str.size()};
    }

    byte_string to_byte_string(const unicode_string_view _Str) {
        return bstk::_Transcode_string<byte_t>(_Str);
    }

    utf8_string to_utf8_string(const byte_string_view _Str) {
        return utf8_string{reinterpret_cast<const char*>(_Str.data()), _Str.size()};
    }

    utf8_string to_utf8_string(const unicode_string_view _Str) {
        return bstk::_Transcode_string<char>(_Str);
    }

    unicode_string to_unicode_string(const byte_string_view _Str) {
        return bstk::_Transcode_string<wchar_t>(_Str);
    }

    unicode_string to_unicode_string(const utf8_string_view _Str) {
        return bstk::_Transcode_string<wchar_t>(_Str);
    }
} // namespace bs