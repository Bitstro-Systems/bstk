// encoding.hpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#pragma once
#ifndef _BSTK_TEXT_ENCODING_HPP_
#define _BSTK_TEXT_ENCODING_HPP_
#include <bstk/core/export.hpp>
#include <bstk/text/string.hpp>
#include <bstk/text/string_view.hpp>

namespace bs {
    _BSTK_EXPORT size_t byte_string_size(const utf8_string_view _Str) noexcept;
    _BSTK_EXPORT size_t byte_string_size(const unicode_string_view _Str) noexcept;

    _BSTK_EXPORT size_t utf8_string_size(const byte_string_view _Str) noexcept;
    _BSTK_EXPORT size_t utf8_string_size(const unicode_string_view _Str) noexcept;

    _BSTK_EXPORT size_t unicode_string_size(const byte_string_view _Str) noexcept;
    _BSTK_EXPORT size_t unicode_string_size(const utf8_string_view _Str) noexcept;

    _BSTK_EXPORT byte_string to_byte_string(const utf8_string_view _Str);
    _BSTK_EXPORT byte_string to_byte_string(const unicode_string_view _Str);

    _BSTK_EXPORT utf8_string to_utf8_string(const byte_string_view _Str);
    _BSTK_EXPORT utf8_string to_utf8_string(const unicode_string_view _Str);

    _BSTK_EXPORT unicode_string to_unicode_string(const byte_string_view _Str);
    _BSTK_EXPORT unicode_string to_unicode_string(const utf8_string_view _Str);
} // namespace bs

#endif // _BSTK_TEXT_ENCODING_HPP_