// char_traits.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/text/char_traits.hpp>
#include <bstk/text/impl/char_traits.hpp>

namespace bs {
    template <class _Elem>
    void char_traits<_Elem>::assign(
        char_type* const _Dest, const size_t _Count, const char_type _Ch) noexcept {
        bstk::_Char_traits<_Elem>::_Assign(_Dest, _Count, _Ch);
    }

    template <class _Elem>
    int char_traits<_Elem>::compare(const char_type* const _Left,
        const char_type* const _Right, const size_t _Count) noexcept {
        return bstk::_Char_traits<_Elem>::_Compare(_Left, _Right, _Count);
    }

    template <class _Elem>
    bool char_traits<_Elem>::eq(const char_type* const _Left,
        const char_type* const _Right, const size_t _Count) noexcept {
        return bstk::_Char_traits<_Elem>::_Compare(_Left, _Right, _Count) == 0;
    }

    template <class _Elem>
    void char_traits<_Elem>::move(
        char_type* const _Dest, const char_type* const _Src, const size_t _Count) noexcept {
        bstk::_Char_traits<_Elem>::_Move(_Dest, _Src, _Count);
    }

    template <class _Elem>
    void char_traits<_Elem>::copy(
        char_type* const _Dest, const char_type* const _Src, const size_t _Count) noexcept {
        bstk::_Char_traits<_Elem>::_Copy(_Dest, _Src, _Count);
    }

    template <class _Elem>
    size_t char_traits<_Elem>::length(const char_type* const _Str) noexcept {
        return bstk::_Char_traits<_Elem>::_Length(_Str);
    }

    template <class _Elem>
    size_t char_traits<_Elem>::find(
        const char_type* const _Str, const size_t _Size, const char_type _Ch) noexcept {
        return bstk::_Char_traits<_Elem>::_Find(_Str, _Size, _Ch);
    }

    template <class _Elem>
    size_t char_traits<_Elem>::find(const char_type* const _Str, const size_t _Str_size,
        const char_type* const _Substr, const size_t _Substr_size) noexcept {
        if (_Substr_size == 0) { // empty string always matches
            return 0;
        }

        if (_Str_size == 0 || _Substr_size > _Str_size) { // no room for match
            return npos;
        }

        using _Traits = bstk::_Char_traits<_Elem>;
        if (_Substr_size == 1) { // search for a single character
            return _Traits::_Find(_Str, _Str_size, *_Substr);
        } else if (_Substr_size == _Str_size) { // compare full strings
            return eq(_Str, _Substr, _Substr_size) ? 0 : npos;
        }

        size_t _Off  = 0;
        size_t _Size = _Str_size;
        while (_Size >= _Substr_size) {
            size_t _Idx = _Traits::_Find(_Str + _Off, _Size, *_Substr);
            if (_Idx == npos) { // no matching character found
                break;
            }

            _Idx += _Off; // make the index absolute
            if (_Str_size - _Idx >= _Substr_size && eq(_Str + _Idx + 1, _Substr + 1, _Substr_size - 1)) {
                return _Idx;
            }

            _Off  = _Idx + 1;
            _Size = _Str_size - _Off;
        }

        return npos; // match not found
    }

    template <class _Elem>
    size_t char_traits<_Elem>::rfind(
        const char_type* const _Str, const size_t _Size, const char_type _Ch) noexcept {
        if (_Size == 0) { // no room for match
            return npos;
        }

        for (const char_type* _Last = _Str + _Size - 1; _Last >= _Str; --_Last) {
            if (*_Last == _Ch) { // match found
                return _Last - _Str;
            }
        }

        return npos; // match not found
    }

    template <class _Elem>
    size_t char_traits<_Elem>::rfind(const char_type* const _Str, const size_t _Str_size,
        const char_type* const _Substr, const size_t _Substr_size) noexcept {
        if (_Substr_size == 0) { // empty string always matches
            return _Str_size;
        }

        if (_Str_size == 0 || _Substr_size > _Str_size) { // no room for match
            return npos;
        }

        if (_Substr_size == 1) { // search for a single character
            return rfind(_Str, _Str_size, *_Substr);
        } else if (_Substr_size == _Str_size) { // compare full strings
            return eq(_Str, _Substr, _Substr_size) ? 0 : npos;
        }

        for (const char_type* _Last = _Str + _Str_size - _Substr_size;; --_Last) {
            if (eq(_Last, _Substr, _Substr_size)) { // match found
                return _Last - _Str;
            }

            if (_Last == _Str) { // at beginning, no more chance for match
                return npos;
            }
        }

        return npos; // match not found
    }

    template struct _BSTK_EXPORT char_traits<byte_t>;
    template struct _BSTK_EXPORT char_traits<char>;
    template struct _BSTK_EXPORT char_traits<wchar_t>;
} // namespace bs