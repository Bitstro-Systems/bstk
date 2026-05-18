// string_view.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/core/exception.hpp>
#include <bstk/core/impl/utility.hpp>
#include <bstk/text/string_view.hpp>
#include <cstdint>
#include <utility>

namespace bs {
    template <class _Elem>
    string_view_iterator<_Elem>::string_view_iterator() noexcept
#ifdef _DEBUG
        : _Myptr(nullptr), _Mybegin(nullptr), _Myend(nullptr) {}
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        : _Myptr(nullptr) {}
#endif // _DEBUG

    template <class _Elem>
    string_view_iterator<_Elem>::string_view_iterator(
#ifdef _DEBUG
        pointer _Begin, pointer _End) noexcept : _Myptr(_Begin), _Mybegin(_Begin), _Myend(_End) {
        _INTERNAL_ASSERT(_Begin <= _End, "invalid iterator bounds");
    }
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        pointer _Ptr) noexcept : _Myptr(_Ptr) {}
#endif // _DEBUG

    template <class _Elem>
    bool string_view_iterator<_Elem>::valid() const noexcept {
#ifdef _DEBUG
        return _Myptr != nullptr && _Myptr != _Myend;
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        return _Myptr != nullptr;
#endif // _DEBUG
    }

    template <class _Elem>
    typename string_view_iterator<_Elem>::reference string_view_iterator<_Elem>::operator*() const noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Myptr != nullptr && _Myptr != _Myend, "attempt to dereference invalid iterator");
#endif // _DEBUG
        return *_Myptr;
    }

    template <class _Elem>
    typename string_view_iterator<_Elem>::pointer string_view_iterator<_Elem>::operator->() const noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Myptr != nullptr && _Myptr != _Myend, "attempt to dereference invalid iterator");
#endif // _DEBUG
        return _Myptr;
    }

    template <class _Elem>
    typename string_view_iterator<_Elem>::reference
        string_view_iterator<_Elem>::operator[](const difference_type _Off) const noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Myptr != nullptr, "attempt to use invalid iterator");
        _INTERNAL_ASSERT(_Myend - _Myptr >= _Off, "attempt to access non-existent element");
#endif // _DEBUG
        return _Myptr[_Off];
    }

    template <class _Elem>
    string_view_iterator<_Elem>& string_view_iterator<_Elem>::operator++() noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Myptr != nullptr, "attempt to use invalid iterator");
        _INTERNAL_ASSERT(_Myend - _Myptr > 0, "attempt to advance iterator that points to the end");
#endif // _DEBUG
        ++_Myptr;
        return *this;
    }

    template <class _Elem>
    string_view_iterator<_Elem> string_view_iterator<_Elem>::operator++(int) noexcept {
        const string_view_iterator _Temp = *this;
        ++*this;
        return _Temp;
    }
    
    template <class _Elem>
    string_view_iterator<_Elem>& string_view_iterator<_Elem>::operator--() noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Myptr != nullptr, "attempt to use invalid iterator");
        _INTERNAL_ASSERT(_Myptr - _Mybegin > 0, "attempt to move backward iterator that points to the beginning");
#endif // _DEBUG
        --_Myptr;
        return *this;
    }

    template <class _Elem>
    string_view_iterator<_Elem> string_view_iterator<_Elem>::operator--(int) noexcept {
        const string_view_iterator _Temp = *this;
        --*this;
        return _Temp;
    }

    template <class _Elem>
    string_view_iterator<_Elem>& string_view_iterator<_Elem>::operator+=(const difference_type _Off) noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Myptr != nullptr, "attempt to use invalid iterator");
        _INTERNAL_ASSERT(_Myend - _Myptr >= _Off, "attempt to advance iterator beyond the end");
#endif // _DEBUG
        _Myptr += _Off;
        return *this;
    }

    template <class _Elem>
    string_view_iterator<_Elem>& string_view_iterator<_Elem>::operator-=(const difference_type _Off) noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Myptr != nullptr, "attempt to use invalid iterator");
        _INTERNAL_ASSERT(_Myptr - _Mybegin >= _Off, "attempt to move backward iterator beyond the beginning");
#endif // _DEBUG
        _Myptr -= _Off;
        return *this;
    }

    template <class _Elem>
    string_view_iterator<_Elem> string_view_iterator<_Elem>::operator+(const difference_type _Off) const noexcept {
        string_view_iterator _Temp = *this;
        _Temp                     += _Off;
        return _Temp;
    }

    template <class _Elem>
    string_view_iterator<_Elem> string_view_iterator<_Elem>::operator-(const difference_type _Off) const noexcept {
        string_view_iterator _Temp = *this;
        _Temp                     -= _Off;
        return _Temp;
    }

    template <class _Elem>
    bool string_view_iterator<_Elem>::operator==(const string_view_iterator& _Other) const noexcept {
        return _Myptr == _Other._Myptr;
    }

    template <class _Elem>
    ::std::strong_ordering string_view_iterator<_Elem>::operator<=>(
        const string_view_iterator& _Other) const noexcept {
        return _Myptr <=> _Other._Myptr;
    }

    template class _BSTK_EXPORT string_view_iterator<byte_t>;
    template class _BSTK_EXPORT string_view_iterator<char>;
    template class _BSTK_EXPORT string_view_iterator<wchar_t>;

    template <class _Elem>
    string_view<_Elem>::string_view() noexcept : _Mydata(nullptr), _Mysize(0) {}

    template <class _Elem>
    string_view<_Elem>::string_view(const_pointer _Ptr, const size_type _Count) noexcept
        : _Mydata(_Ptr), _Mysize(_Count) {}

    template <class _Elem>
    string_view<_Elem>::string_view(const_pointer _Ptr) noexcept
        : _Mydata(_Ptr), _Mysize(traits_type::length(_Ptr)) {}

    template <class _Elem>
    void string_view<_Elem>::_Check_offset(const size_type _Off) const {
        if (_Off >= _Mysize) { // offset out of bounds, raise an exception
            resource_overrun::raise();
        }
    }

    template <class _Elem>
    typename string_view<_Elem>::const_reference
        string_view<_Elem>::operator[](const size_type _Idx) const noexcept {
        // no bounds checking is performed, the behavior is undefined if _Idx >= size()
        return _Mydata[_Idx];
    }

    template <class _Elem>
    typename string_view<_Elem>::const_iterator string_view<_Elem>::begin() const noexcept {
#ifdef _DEBUG
        return const_iterator{_Mydata, _Mydata + _Mysize};
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        return const_iterator{_Mydata};
#endif // _DEBUG
    }

    template <class _Elem>
    typename string_view<_Elem>::const_iterator string_view<_Elem>::cbegin() const noexcept {
        return begin();
    }

    template <class _Elem>
    typename string_view<_Elem>::const_iterator string_view<_Elem>::end() const noexcept {
#ifdef _DEBUG
        return const_iterator{_Mydata + _Mysize, _Mydata + _Mysize};
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        return const_iterator{_Mydata + _Mysize};
#endif // _DEBUG
    }

    template <class _Elem>
    typename string_view<_Elem>::const_iterator string_view<_Elem>::cend() const noexcept {
        return end();
    }

    template <class _Elem>
    typename string_view<_Elem>::const_reverse_iterator string_view<_Elem>::rbegin() const noexcept {
        return const_reverse_iterator{end()};
    }

    template <class _Elem>
    typename string_view<_Elem>::const_reverse_iterator string_view<_Elem>::crbegin() const noexcept {
        return rbegin();
    }

    template <class _Elem>
    typename string_view<_Elem>::const_reverse_iterator string_view<_Elem>::rend() const noexcept {
        return const_reverse_iterator{begin()};
    }

    template <class _Elem>
    typename string_view<_Elem>::const_reverse_iterator string_view<_Elem>::crend() const noexcept {
        return rend();
    }

    template <class _Elem>
    typename string_view<_Elem>::const_reference string_view<_Elem>::at(const size_type _Idx) const {
        _Check_offset(_Idx);
        return _Mydata[_Idx];
    }

    template <class _Elem>
    typename string_view<_Elem>::const_reference string_view<_Elem>::front() const noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Mysize > 0, "attempt to access non-existent element");
#endif // _DEBUG
        return _Mydata[0];
    }

    template <class _Elem>
    typename string_view<_Elem>::const_reference string_view<_Elem>::back() const noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Mysize > 0, "attempt to access non-existent element");
#endif // _DEBUG
        return _Mydata[_Mysize - 1];
    }

    template <class _Elem>
    typename string_view<_Elem>::const_pointer string_view<_Elem>::data() const noexcept {
        return _Mydata;
    }

    template <class _Elem>
    typename string_view<_Elem>::size_type string_view<_Elem>::size() const noexcept {
        return _Mysize;
    }

    template <class _Elem>
    typename string_view<_Elem>::size_type string_view<_Elem>::length() const noexcept {
        return _Mysize;
    }

    template <class _Elem>
    typename string_view<_Elem>::size_type string_view<_Elem>::max_size() const noexcept {
        // maximum representable size limited by pointer arithmetic and size_type capacity
        return (::std::min)(size_type{PTRDIFF_MAX}, static_cast<size_type>(-1) / sizeof(_Elem));
    }

    template <class _Elem>
    bool string_view<_Elem>::empty() const noexcept {
        return _Mysize == 0;
    }

    template <class _Elem>
    void string_view<_Elem>::remove_prefix(const size_type _Count) noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Count <= _Mysize, "attempt to remove prefix longer than total string size");
#endif // _DEBUG
        _Mydata += _Count;
        _Mysize -= _Count;
    }

    template <class _Elem>
    void string_view<_Elem>::remove_suffix(const size_type _Count) noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Count <= _Mysize, "attempt to remove suffix longer than total string size");
#endif // _DEBUG
        _Mysize -= _Count;
    }

    template <class _Elem>
    void string_view<_Elem>::swap(string_view& _Other) noexcept {
        ::std::swap(_Mydata, _Other._Mydata);
        ::std::swap(_Mysize, _Other._Mysize);
    }

    template <class _Elem>
    typename string_view<_Elem>::size_type string_view<_Elem>::copy(
        pointer _Dest, size_type _Count, const size_type _Off) const {
        _Check_offset(_Off);
        _Count = (::std::min)(_Count, _Mysize - _Off);
        traits_type::copy(_Dest, _Mydata + _Off, _Count);
        return _Count;
    }

    template <class _Elem>
    string_view<_Elem> string_view<_Elem>::substr(const size_type _Off, size_type _Count) const {
        _Check_offset(_Off);
        _Count = (::std::min)(_Count, _Mysize - _Off);
        return string_view{_Mydata + _Off, _Count};
    }

    template <class _Elem>
    int string_view<_Elem>::compare(const string_view _Str) const noexcept {
        const int _Result = traits_type::compare(_Mydata, _Str._Mydata, (::std::min)(_Mysize, _Str._Mysize));
        if (_Result != 0) { // strings differ within the common length
            return _Result;
        }

        // strings match over the common length, compare lengths
        if (_Mysize == _Str._Mysize) {
            return 0;
        } else {
            return _Mysize < _Str._Mysize ? -1 : 1;
        }
    }

    template <class _Elem>
    int string_view<_Elem>::compare(const_pointer _Ptr, const size_type _Count) const noexcept {
        return compare(string_view{_Ptr, _Count});
    }

    template <class _Elem>
    int string_view<_Elem>::compare(const_pointer _Ptr) const noexcept {
        return compare(string_view{_Ptr});
    }

    template <class _Elem>
    bool string_view<_Elem>::starts_with(const string_view _Str) const noexcept {
        if (_Mysize < _Str._Mysize) { // prefix is longer than the string
            return false;
        }

        return traits_type::eq(_Mydata, _Str._Mydata, _Str._Mysize);
    }

    template <class _Elem>
    bool string_view<_Elem>::starts_with(const value_type _Ch) const noexcept {
        return _Mysize > 0 ? _Mydata[0] == _Ch : false;
    }

    template <class _Elem>
    bool string_view<_Elem>::starts_with(const_pointer _Ptr) const noexcept {
        return starts_with(string_view{_Ptr});
    }

    template <class _Elem>
    bool string_view<_Elem>::ends_with(const string_view _Str) const noexcept {
        if (_Mysize < _Str._Mysize) { // suffix is longer than the string
            return false;
        }

        return traits_type::eq(_Mydata + (_Mysize - _Str._Mysize), _Str._Mydata, _Str._Mysize);
    }

    template <class _Elem>
    bool string_view<_Elem>::ends_with(const value_type _Ch) const noexcept {
        return _Mysize > 0 ? _Mydata[_Mysize - 1] == _Ch : false;
    }

    template <class _Elem>
    bool string_view<_Elem>::ends_with(const_pointer _Ptr) const noexcept {
        return ends_with(string_view{_Ptr});
    }

    template <class _Elem>
    bool string_view<_Elem>::contains(const string_view _Str) const noexcept {
        return find(_Str) != npos;
    }

    template <class _Elem>
    bool string_view<_Elem>::contains(const value_type _Ch) const noexcept {
        return find(_Ch) != npos;
    }

    template <class _Elem>
    bool string_view<_Elem>::contains(const_pointer _Ptr) const noexcept {
        return find(_Ptr) != npos;
    }

    template <class _Elem>
    typename string_view<_Elem>::size_type string_view<_Elem>::find(
        const string_view _Str, const size_type _Off) const noexcept {
        if (_Off >= _Mysize || _Mysize - _Off < _Str._Mysize) { // no room for match
            return npos;
        }

        if (_Str._Mysize == 0) { // empty string always matches
            return _Off;
        }
        
        if (_Off == 0) { // search from the beginning
            return traits_type::find(_Mydata, _Mysize, _Str._Mydata, _Str._Mysize);
        } else { // search from the given offset
            const size_type _Idx = traits_type::find(_Mydata + _Off, _Mysize - _Off, _Str._Mydata, _Str._Mysize);
            return _Idx != npos ? _Idx + _Off : npos;
        }
    }

    template <class _Elem>
    typename string_view<_Elem>::size_type string_view<_Elem>::find(
        const value_type _Ch, const size_type _Off) const noexcept {
        if (_Off >= _Mysize) { // no room for match
            return npos;
        }

        if (_Off == 0) { // start from the beginning
            return traits_type::find(_Mydata, _Mysize, _Ch);
        } else { // start from the given offset
            const size_type _Idx = traits_type::find(_Mydata + _Off, _Mysize - _Off, _Ch);
            return _Idx != npos ? _Idx + _Off : npos;
        }
    }

    template <class _Elem>
    typename string_view<_Elem>::size_type string_view<_Elem>::find(
        const_pointer _Ptr, const size_type _Off, const size_type _Count) const noexcept {
        return find(string_view{_Ptr, _Count}, _Off);
    }

    template <class _Elem>
    typename string_view<_Elem>::size_type string_view<_Elem>::find(
        const_pointer _Ptr, const size_type _Off) const noexcept {
        return find(string_view{_Ptr}, _Off);
    }

    template <class _Elem>
    typename string_view<_Elem>::size_type
        string_view<_Elem>::rfind(const string_view _Str, const size_type _Off) const noexcept {
        if (_Str._Mysize == 0) { // empty string always matches
            return (::std::min)(_Off, _Mysize);
        }

        if (_Str._Mysize > _Mysize) { // no room for match
            return npos;
        }

        if (_Str._Mysize == 1) { // search for a single character
            return rfind(*_Str._Mydata, _Off);
        }

        const size_type _Real_off = (::std::min)(_Off, _Mysize - _Str._Mysize);
        for (const _Elem* _Last = _Mydata + _Real_off; _Last >= _Mydata; --_Last) {
            if (traits_type::eq(_Last, _Str._Mydata, _Str._Mysize)) { // match found
                return _Last - _Mydata;
            }
        }

        return npos; // match not found
    }

    template <class _Elem>
    typename string_view<_Elem>::size_type string_view<_Elem>::rfind(
        const value_type _Ch, const size_type _Off) const noexcept {
        if (_Mysize == 0) { // no room for match
            return npos;
        }
        
        const size_type _Real_off = (::std::min)(_Off, _Mysize - 1);
        for (const _Elem* _Last = _Mydata + _Real_off; _Last >= _Mydata; --_Last) {
            if (*_Last == _Ch) { // match found
                return static_cast<size_type>(_Last - _Mydata);
            }
        }

        return npos; // match not found
    }

    template <class _Elem>
    typename string_view<_Elem>::size_type string_view<_Elem>::rfind(
        const_pointer _Ptr, const size_type _Off, const size_type _Count) const noexcept {
        return rfind(string_view{_Ptr, _Count}, _Off);
    }

    template <class _Elem>
    typename string_view<_Elem>::size_type string_view<_Elem>::rfind(
        const_pointer _Ptr, const size_type _Off) const noexcept {
        return rfind(string_view{_Ptr}, _Off);
    }

    template class _BSTK_EXPORT string_view<byte_t>;
    template class _BSTK_EXPORT string_view<char>;
    template class _BSTK_EXPORT string_view<wchar_t>;
} // namespace bs