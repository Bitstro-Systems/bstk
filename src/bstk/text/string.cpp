// string.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/core/exception.hpp>
#include <bstk/core/impl/utility.hpp>
#include <bstk/text/string.hpp>
#include <utility>

namespace bs {
    template <class _Elem>
    string_const_iterator<_Elem>::string_const_iterator() noexcept
#ifdef _DEBUG
        : _Myptr(nullptr), _Mybegin(nullptr), _Myend(nullptr) {}
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        : _Myptr(nullptr) {}
#endif // _DEBUG

    template <class _Elem>
    string_const_iterator<_Elem>::string_const_iterator(
#ifdef _DEBUG
        pointer _Begin, pointer _End) noexcept : _Myptr(_Begin), _Mybegin(_Begin), _Myend(_End) {
        _INTERNAL_ASSERT(_Begin <= _End, "invalid iterator bounds");
    }
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        pointer _Ptr) noexcept : _Myptr(_Ptr) {}
#endif // _DEBUG

    template <class _Elem>
    bool string_const_iterator<_Elem>::valid() const noexcept {
#ifdef _DEBUG
        return _Myptr != nullptr && _Myptr != _Myend;
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        return _Myptr != nullptr;
#endif // _DEBUG
    }

    template <class _Elem>
    typename string_const_iterator<_Elem>::reference string_const_iterator<_Elem>::operator*() const noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Myptr != nullptr && _Myptr != _Myend, "attempt to dereference invalid iterator");
#endif // _DEBUG
        return *_Myptr;
    }

    template <class _Elem>
    typename string_const_iterator<_Elem>::pointer string_const_iterator<_Elem>::operator->() const noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Myptr != nullptr && _Myptr != _Myend, "attempt to dereference invalid iterator");
#endif // _DEBUG
        return _Myptr;
    }

    template <class _Elem>
    typename string_const_iterator<_Elem>::reference
        string_const_iterator<_Elem>::operator[](const difference_type _Off) const noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Myptr != nullptr, "attempt to use invalid iterator");
        _INTERNAL_ASSERT(_Myend - _Myptr >= _Off, "attempt to access non-existent element");
#endif // _DEBUG
        return _Myptr[_Off];
    }

    template <class _Elem>
    string_const_iterator<_Elem>& string_const_iterator<_Elem>::operator++() noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Myptr != nullptr, "attempt to use invalid iterator");
        _INTERNAL_ASSERT(_Myend - _Myptr > 0, "attempt to advance iterator that points to the end");
#endif // _DEBUG
        ++_Myptr;
        return *this;
    }

    template <class _Elem>
    string_const_iterator<_Elem> string_const_iterator<_Elem>::operator++(int) noexcept {
        const string_const_iterator _Temp = *this;
        ++*this;
        return _Temp;
    }
    
    template <class _Elem>
    string_const_iterator<_Elem>& string_const_iterator<_Elem>::operator--() noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Myptr != nullptr, "attempt to use invalid iterator");
        _INTERNAL_ASSERT(_Myptr - _Mybegin > 0, "attempt to move backward iterator that points to the beginning");
#endif // _DEBUG
        --_Myptr;
        return *this;
    }

    template <class _Elem>
    string_const_iterator<_Elem> string_const_iterator<_Elem>::operator--(int) noexcept {
        const string_const_iterator _Temp = *this;
        --*this;
        return _Temp;
    }

    template <class _Elem>
    string_const_iterator<_Elem>& string_const_iterator<_Elem>::operator+=(const difference_type _Off) noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Myptr != nullptr, "attempt to use invalid iterator");
        _INTERNAL_ASSERT(_Myend - _Myptr >= _Off, "attempt to advance iterator beyond the end");
#endif // _DEBUG
        _Myptr += _Off;
        return *this;
    }

    template <class _Elem>
    string_const_iterator<_Elem>& string_const_iterator<_Elem>::operator-=(const difference_type _Off) noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Myptr != nullptr, "attempt to use invalid iterator");
        _INTERNAL_ASSERT(_Myptr - _Mybegin >= _Off, "attempt to move backward iterator beyond the beginning");
#endif // _DEBUG
        _Myptr -= _Off;
        return *this;
    }

    template <class _Elem>
    string_const_iterator<_Elem> string_const_iterator<_Elem>::operator+(const difference_type _Off) const noexcept {
        string_const_iterator _Temp = *this;
        _Temp                      += _Off;
        return _Temp;
    }

    template <class _Elem>
    string_const_iterator<_Elem> string_const_iterator<_Elem>::operator-(const difference_type _Off) const noexcept {
        string_const_iterator _Temp = *this;
        _Temp                      -= _Off;
        return _Temp;
    }

    template <class _Elem>
    bool string_const_iterator<_Elem>::operator==(const string_const_iterator& _Other) const noexcept {
        return _Myptr == _Other._Myptr;
    }

    template <class _Elem>
    ::std::strong_ordering string_const_iterator<_Elem>::operator<=>(
        const string_const_iterator& _Other) const noexcept {
        return _Myptr <=> _Other._Myptr;
    }

    template class _BSTK_EXPORT string_const_iterator<byte_t>;
    template class _BSTK_EXPORT string_const_iterator<char>;
    template class _BSTK_EXPORT string_const_iterator<wchar_t>;

    template <class _Elem>
    string_iterator<_Elem>::string_iterator(
#ifdef _DEBUG
        pointer _Begin, pointer _End) noexcept : _Mybase(_Begin, _End) {}
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        pointer _Ptr) noexcept : _Mybase(_Ptr) {}
#endif // _DEBUG

    template <class _Elem>
    bool string_iterator<_Elem>::valid() const noexcept {
        return _Mybase::valid();
    }

    template <class _Elem>
    typename string_iterator<_Elem>::reference string_iterator<_Elem>::operator*() const noexcept {
        return const_cast<reference>(_Mybase::operator*());
    }

    template <class _Elem>
    typename string_iterator<_Elem>::pointer string_iterator<_Elem>::operator->() const noexcept {
        return const_cast<pointer>(_Mybase::operator->());
    }

    template <class _Elem>
    typename string_iterator<_Elem>::reference
        string_iterator<_Elem>::operator[](const difference_type _Off) const noexcept {
        return const_cast<reference>(_Mybase::operator[](_Off));
    }

    template <class _Elem>
    string_iterator<_Elem>& string_iterator<_Elem>::operator++() noexcept {
        _Mybase::operator++();
        return *this;
    }

    template <class _Elem>
    string_iterator<_Elem> string_iterator<_Elem>::operator++(int) noexcept {
        string_iterator _Temp = *this;
        _Mybase::operator++();
        return _Temp;
    }

    template <class _Elem>
    string_iterator<_Elem>& string_iterator<_Elem>::operator--() noexcept {
        _Mybase::operator--();
        return *this;
    }

    template <class _Elem>
    string_iterator<_Elem> string_iterator<_Elem>::operator--(int) noexcept {
        string_iterator _Temp = *this;
        _Mybase::operator--();
        return _Temp;
    }

    template <class _Elem>
    string_iterator<_Elem>& string_iterator<_Elem>::operator+=(const difference_type _Off) noexcept {
        _Mybase::operator+=(_Off);
        return *this;
    }

    template <class _Elem>
    string_iterator<_Elem>& string_iterator<_Elem>::operator-=(const difference_type _Off) noexcept {
        _Mybase::operator-=(_Off);
        return *this;
    }

    template <class _Elem>
    string_iterator<_Elem> string_iterator<_Elem>::operator+(const difference_type _Off) const noexcept {
        string_iterator _Temp = *this;
        _Temp                += _Off;
        return _Temp;
    }

    template <class _Elem>
    string_iterator<_Elem> string_iterator<_Elem>::operator-(const difference_type _Off) const noexcept {
        string_iterator _Temp = *this;
        _Temp                -= _Off;
        return _Temp;
    }

    template <class _Elem>
    bool string_iterator<_Elem>::operator==(const string_iterator& _Other) const noexcept {
        using _Const_iter = const string_const_iterator<_Elem>&;
        return static_cast<_Const_iter>(*this) == static_cast<_Const_iter>(_Other);
    }

    template <class _Elem>
    ::std::strong_ordering string_iterator<_Elem>::operator<=>(const string_iterator& _Other) const noexcept {
        using _Const_iter = const string_const_iterator<_Elem>&;
        return static_cast<_Const_iter>(*this) <=> static_cast<_Const_iter>(_Other);
    }

    template class _BSTK_EXPORT string_iterator<byte_t>;
    template class _BSTK_EXPORT string_iterator<char>;
    template class _BSTK_EXPORT string_iterator<wchar_t>;

    template <class _Elem>
    string<_Elem>::string() noexcept : _Mybuf() {}

    template <class _Elem>
    string<_Elem>::string(const string& _Other) : _Mybuf(_Other._Mybuf._Al) {
        _Construct_from_ptr(_Other._Mybuf._Get(), _Other._Mybuf._Size);
    }

    template <class _Elem>
    string<_Elem>::string(string&& _Other) noexcept : _Mybuf() {
        _Mybuf._Move_contents(_Other._Mybuf);
    }

    template <class _Elem>
    string<_Elem>::string(allocator& _Al) noexcept : _Mybuf(&_Al) {}

    template <class _Elem>
    string<_Elem>::string(const string& _Other, allocator& _Al) : _Mybuf(&_Al) {
        _Construct_from_ptr(_Other._Mybuf._Get(), _Other._Mybuf._Size);
    }

    template <class _Elem>
    string<_Elem>::string(string&& _Other, allocator& _Al) noexcept : _Mybuf() {
        _Mybuf._Move_contents(_Other._Mybuf, &_Al);
    }

    template <class _Elem>
    string<_Elem>::string(const size_type _Count, const value_type _Ch) : _Mybuf() {
        _Construct_from_chars(_Count, _Ch);
    }

    template <class _Elem>
    string<_Elem>::string(const size_type _Count, const value_type _Ch, allocator& _Al) : _Mybuf(&_Al) {
        _Construct_from_chars(_Count, _Ch);
    }

    template <class _Elem>
    string<_Elem>::string(const_pointer _Ptr, const size_type _Count) : _Mybuf() {
        _Construct_from_ptr(_Ptr, _Count);
    }

    template <class _Elem>
    string<_Elem>::string(const_pointer _Ptr, const size_type _Count, allocator& _Al) : _Mybuf(&_Al) {
        _Construct_from_ptr(_Ptr, _Count);
    }

    template <class _Elem>
    string<_Elem>::string(const_pointer _Ptr) : _Mybuf() {
        _Construct_from_ptr(_Ptr, traits_type::length(_Ptr));
    }

    template <class _Elem>
    string<_Elem>::string(const_pointer _Ptr, allocator& _Al) : _Mybuf(&_Al) {
        _Construct_from_ptr(_Ptr, traits_type::length(_Ptr));
    }

    template <class _Elem>
    string<_Elem>::string(const string_view<_Elem> _Str) : _Mybuf() {
        _Construct_from_ptr(_Str.data(), _Str.size());
    }

    template <class _Elem>
    string<_Elem>::string(const string_view<_Elem> _Str, allocator& _Al) : _Mybuf(&_Al) {
        _Construct_from_ptr(_Str.data(), _Str.size());
    }

    template <class _Elem>
    string<_Elem>::_String_buffer::_String_buffer(allocator* const _Al) noexcept
        : _Al(_Al ? _Al : &::bs::get_allocator()), _Capacity(_Small_buffer_capacity), _Size(0), _Small{_Elem{0}} {}

    template <class _Elem>
    string<_Elem>::_String_buffer::~_String_buffer() noexcept {
        _Destroy();
    }

    template <class _Elem>
    bool string<_Elem>::_String_buffer::_Is_small() const noexcept {
        return _Capacity <= _Small_buffer_capacity;
    }

    template <class _Elem>
    typename string<_Elem>::pointer string<_Elem>::_String_buffer::_Get() noexcept {
        return _Capacity <= _Small_buffer_capacity ? _Small : _Large;
    }

    template <class _Elem>
    typename string<_Elem>::const_pointer string<_Elem>::_String_buffer::_Get() const noexcept {
        return _Capacity <= _Small_buffer_capacity ? _Small : _Large;
    }

    template <class _Elem>
    void string<_Elem>::_String_buffer::_Switch_to_small() {
        _Elem _Buf[_Small_buffer_size]; // temporary buffer for _Large's data
        traits_type::copy(_Buf, _Large, _Size + 1);
        ::bs::deallocate_object_array_using_allocator(_Large, _Capacity + 1, *_Al);
        traits_type::copy(_Small, _Buf, _Size + 1);
        _Capacity = _Small_buffer_capacity;
    }

    template <class _Elem>
    void string<_Elem>::_String_buffer::_Swap_small_with_large(_String_buffer& _Other) noexcept {
        _Elem _Buf[_Small_buffer_size]; // temporary buffer for _Small's data
        traits_type::copy(_Buf, _Small, _Size + 1);
        _Large = _Other._Large;
        traits_type::copy(_Other._Small, _Buf, _Size + 1);
    }

    template <class _Elem>
    void string<_Elem>::_String_buffer::_Deallocate_large_buffer() {
        if (_Capacity > _Small_buffer_capacity) {
            ::bs::deallocate_object_array_using_allocator(_Large, _Capacity + 1, *_Al);
            _Large = nullptr;
        }
    }

    template <class _Elem>
    void string<_Elem>::_String_buffer::_Destroy() {
        _Deallocate_large_buffer();
        _Capacity = 0;
        _Size     = 0;
    }

    template <class _Elem>
    void string<_Elem>::_String_buffer::_Move_contents(_String_buffer& _Other, allocator* const _New_al) {
        _Destroy(); // destroy the current buffer
        _Al       = _New_al ? _New_al : _Other._Al;
        _Capacity = _Other._Capacity;
        _Size     = _Other._Size;
        if (_Other._Is_small()) { // move small buffer
            traits_type::copy(_Small, _Other._Small, _Other._Size + 1);
            _Other._Small[0] = _Elem{0};
        } else { // move large buffer
            _Large           = _Other._Large;
            _Other._Large    = nullptr;
            _Other._Small[0] = _Elem{0}; // start lifetime of the small buffer
        }

        // move only data, _Other's allocator must remain valid
        _Other._Capacity = _Small_buffer_capacity;
        _Other._Size     = 0;
    }

    template <class _Elem>
    typename string<_Elem>::pointer string<_Elem>::_Allocate_new_buffer(size_type& _Capacity) {
        // allocate space for at least _Capacity + 1 (null-terminator) elements
        _Capacity |= _Alloc_mask;
        if (_Capacity > max_size() - 1) { // requested size exceeds the limit, raise an exception
            allocation_limit_exceeded::raise();
        }

        return ::bs::allocate_object_array_using_allocator<_Elem>(_Capacity + 1, *_Mybuf._Al);
    }

    template <class _Elem>
    void string<_Elem>::_Check_offset(const size_type _Off) const {
        if (_Off >= _Mybuf._Size) { // must be within [0, _Mybuf._Size)
            resource_overrun::raise();
        }
    }

    template <class _Elem>
    void string<_Elem>::_Check_insert_offset(const size_type _Off) const {
        if (_Off > _Mybuf._Size) { // must be within [0, _Mybuf._Size]
            resource_overrun::raise();
        }
    }

    template <class _Elem>
    void string<_Elem>::_Construct_from_ptr(const_pointer _Ptr, const size_type _Count) {
        _Construct(_Count,
            [=](pointer _New_ptr) noexcept {
                traits_type::copy(_New_ptr, _Ptr, _Count);
            }
        );
    }

    template <class _Elem>
    void string<_Elem>::_Construct_from_chars(const size_type _Count, const value_type _Ch) {
        _Construct(_Count,
            [=](pointer _New_ptr) noexcept {
                traits_type::assign(_New_ptr, _Count, _Ch);
            }
        );
    }

    template <class _Elem>
    void string<_Elem>::_Reallocate_assign(const size_type _Count, const value_type _Ch) {
        _Reallocate(_Count,
            [=](pointer _New_ptr) noexcept {
                traits_type::assign(_New_ptr, _Count, _Ch);
            }
        );
    }

    template <class _Elem>
    void string<_Elem>::_Reallocate_assign(const_pointer _Ptr, const size_type _Count) {
        _Reallocate(_Count,
            [=](pointer _New_ptr) noexcept {
                traits_type::copy(_New_ptr, _Ptr, _Count);
            }
        );
    }

    template <class _Elem>
    void string<_Elem>::_Reallocate_insert_back(const size_type _Count, const value_type _Ch) {
        _Reallocate(_Mybuf._Size + _Count,
            [=](pointer _New_ptr) noexcept {
                traits_type::copy(_New_ptr, _Mybuf._Get(), _Mybuf._Size);
                traits_type::assign(_New_ptr + _Mybuf._Size, _Count, _Ch);
            }
        );
    }

    template <class _Elem>
    void string<_Elem>::_Reallocate_insert_back(const_pointer _Ptr, const size_type _Count) {
        _Reallocate(_Mybuf._Size + _Count,
            [=](pointer _New_ptr) noexcept {
                traits_type::copy(_New_ptr, _Mybuf._Get(), _Mybuf._Size);
                traits_type::copy(_New_ptr + _Mybuf._Size, _Ptr, _Count);
            }
        );
    }

    template <class _Elem>
    void string<_Elem>::_Reallocate_insert_at(
        const size_type _Off, const size_type _Count, const value_type _Ch) {
        _Reallocate(_Mybuf._Size + _Count,
            [=](pointer _New_ptr) noexcept {
                pointer _Old_ptr = _Mybuf._Get();
                traits_type::copy(_New_ptr, _Old_ptr, _Off);
                traits_type::assign(_New_ptr + _Off, _Count, _Ch);
                traits_type::copy(_New_ptr + _Off + _Count, _Old_ptr + _Off, _Mybuf._Size - _Off);
            }
        );
    }

    template <class _Elem>
    void string<_Elem>::_Reallocate_insert_at(
        const size_type _Off, const_pointer _Ptr, const size_type _Count) {
        _Reallocate(_Mybuf._Size + _Count,
            [=](pointer _New_ptr) noexcept {
                pointer _Old_ptr = _Mybuf._Get();
                traits_type::copy(_New_ptr, _Old_ptr, _Off);
                traits_type::copy(_New_ptr + _Off, _Ptr, _Count);
                traits_type::copy(_New_ptr + _Off + _Count, _Old_ptr + _Off, _Mybuf._Size - _Off);
            }
        );
    }

    template <class _Elem>
    void string<_Elem>::_Reallocate_replace(
        const size_type _Off, const size_type _Count, const size_type _Ch_count, const value_type _Ch) {
        _Reallocate(_Mybuf._Size + (_Ch_count - _Count), // assumes growth
            [=](pointer _New_ptr) noexcept {
                pointer _Old_ptr = _Mybuf._Get();
                traits_type::copy(_New_ptr, _Old_ptr, _Off);
                traits_type::assign(_New_ptr + _Off, _Ch_count, _Ch);
                traits_type::copy(_New_ptr + _Off + _Ch_count,
                    _Old_ptr + _Off + _Count, _Mybuf._Size - _Off - _Count);
            }
        );
    }

    template <class _Elem>
    void string<_Elem>::_Reallocate_replace(
        const size_type _Off, const size_type _Count, const_pointer _Ptr, const size_type _Ptr_count) {
        _Reallocate(_Mybuf._Size + (_Ptr_count - _Count), // assumes growth
            [=](pointer _New_ptr) noexcept {
                pointer _Old_ptr = _Mybuf._Get();
                traits_type::copy(_New_ptr, _Old_ptr, _Off);
                traits_type::copy(_New_ptr + _Off, _Ptr, _Ptr_count);
                traits_type::copy(_New_ptr + _Off + _Ptr_count,
                    _Old_ptr + _Off + _Count, _Mybuf._Size - _Off - _Count);
            }
        );
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::operator=(const string& _Str) {
        return assign(_Str._Mybuf._Get(), _Str._Mybuf._Size);
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::operator=(string&& _Str) noexcept {
        return assign(::std::move(_Str));
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::operator=(const_pointer _Ptr) {
        return assign(_Ptr, traits_type::length(_Ptr));
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::operator=(const value_type _Ch) {
        return assign(1, _Ch);
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::operator=(const string_view<_Elem> _Str) {
        return assign(_Str.data(), _Str.size());
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::operator+=(const string& _Str) {
        return append(_Str._Mybuf._Get(), _Str._Mybuf._Size);
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::operator+=(const_pointer _Ptr) {
        return append(_Ptr, traits_type::length(_Ptr));
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::operator+=(const value_type _Ch) {
        push_back(_Ch);
        return *this;
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::operator+=(const string_view<_Elem> _Str) {
        return append(_Str.data(), _Str.size());
    }

    template <class _Elem>
    string<_Elem>::operator string_view<_Elem>() const noexcept {
        return string_view<_Elem>{_Mybuf._Get(), _Mybuf._Size};
    }

    template <class _Elem>
    typename string<_Elem>::reference string<_Elem>::operator[](const size_type _Idx) noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Idx <= _Mybuf._Size, "attempt to access non-existent element");
#endif // _DEBUG
        return _Mybuf._Get()[_Idx];
    }

    template <class _Elem>
    typename string<_Elem>::const_reference string<_Elem>::operator[](const size_type _Idx) const noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Idx <= _Mybuf._Size, "attempt to access non-existent element");
#endif // _DEBUG
        return _Mybuf._Get()[_Idx];
    }

    template <class _Elem>
    allocator& string<_Elem>::get_allocator() noexcept {
        return *_Mybuf._Al;
    }

    template <class _Elem>
    typename string<_Elem>::iterator string<_Elem>::begin() noexcept {
#ifdef _DEBUG
        return iterator{_Mybuf._Get(), _Mybuf._Get() + _Mybuf._Size};
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        return iterator{_Mybuf._Get()};
#endif // _DEBUG
    }

    template <class _Elem>
    typename string<_Elem>::const_iterator string<_Elem>::begin() const noexcept {
#ifdef _DEBUG
        return const_iterator{_Mybuf._Get(), _Mybuf._Get() + _Mybuf._Size};
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        return const_iterator{_Mybuf._Get()};
#endif // _DEBUG
    }

    template <class _Elem>
    typename string<_Elem>::const_iterator string<_Elem>::cbegin() const noexcept {
        return begin();
    }

    template <class _Elem>
    typename string<_Elem>::iterator string<_Elem>::end() noexcept {
#ifdef _DEBUG
        return iterator{_Mybuf._Get() + _Mybuf._Size, _Mybuf._Get() + _Mybuf._Size};
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        return iterator{_Mybuf._Get() + _Mybuf._Size};
#endif // _DEBUG
    }

    template <class _Elem>
    typename string<_Elem>::const_iterator string<_Elem>::end() const noexcept {
#ifdef _DEBUG
        return const_iterator{_Mybuf._Get() + _Mybuf._Size, _Mybuf._Get() + _Mybuf._Size};
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        return const_iterator{_Mybuf._Get() + _Mybuf._Size};
#endif // _DEBUG
    }

    template <class _Elem>
    typename string<_Elem>::const_iterator string<_Elem>::cend() const noexcept {
        return end();
    }

    template <class _Elem>
    typename string<_Elem>::reverse_iterator string<_Elem>::rbegin() noexcept {
        return reverse_iterator{end()};
    }

    template <class _Elem>
    typename string<_Elem>::const_reverse_iterator string<_Elem>::rbegin() const noexcept {
        return const_reverse_iterator{end()};
    }

    template <class _Elem>
    typename string<_Elem>::const_reverse_iterator string<_Elem>::crbegin() const noexcept {
        return rbegin();
    }

    template <class _Elem>
    typename string<_Elem>::reverse_iterator string<_Elem>::rend() noexcept {
        return reverse_iterator{begin()};
    }

    template <class _Elem>
    typename string<_Elem>::const_reverse_iterator string<_Elem>::rend() const noexcept {
        return const_reverse_iterator{begin()};
    }

    template <class _Elem>
    typename string<_Elem>::const_reverse_iterator string<_Elem>::crend() const noexcept {
        return rend();
    }

    template <class _Elem>
    typename string<_Elem>::reference string<_Elem>::at(const size_type _Idx) {
        _Check_offset(_Idx);
        return _Mybuf._Get()[_Idx];
    }

    template <class _Elem>
    typename string<_Elem>::const_reference string<_Elem>::at(const size_type _Idx) const {
        _Check_offset(_Idx);
        return _Mybuf._Get()[_Idx];
    }

    template <class _Elem>
    typename string<_Elem>::reference string<_Elem>::front() noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Mybuf._Size > 0, "attempt to access non-existent element");
#endif // _DEBUG
        return _Mybuf._Get()[0];
    }

    template <class _Elem>
    typename string<_Elem>::const_reference string<_Elem>::front() const noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Mybuf._Size > 0, "attempt to access non-existent element");
#endif // _DEBUG
        return _Mybuf._Get()[0];
    }

    template <class _Elem>
    typename string<_Elem>::reference string<_Elem>::back() noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Mybuf._Size > 0, "attempt to access non-existent element");
#endif // _DEBUG
        return _Mybuf._Get()[_Mybuf._Size - 1];
    }

    template <class _Elem>
    typename string<_Elem>::const_reference string<_Elem>::back() const noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Mybuf._Size > 0, "attempt to access non-existent element");
#endif // _DEBUG
        return _Mybuf._Get()[_Mybuf._Size - 1];
    }

    template <class _Elem>
    typename string<_Elem>::pointer string<_Elem>::data() noexcept {
        return _Mybuf._Get();
    }

    template <class _Elem>
    typename string<_Elem>::const_pointer string<_Elem>::data() const noexcept {
        return _Mybuf._Get();
    }

    template <class _Elem>
    typename string<_Elem>::const_pointer string<_Elem>::c_str() const noexcept {
        return _Mybuf._Get();
    }

    template <class _Elem>
    bool string<_Elem>::empty() const noexcept {
        return _Mybuf._Size == 0;
    }

    template <class _Elem>
    typename string<_Elem>::size_type string<_Elem>::capacity() const noexcept {
        return _Mybuf._Capacity;
    }

    template <class _Elem>
    typename string<_Elem>::size_type string<_Elem>::size() const noexcept {
        return _Mybuf._Size;
    }

    template <class _Elem>
    typename string<_Elem>::size_type string<_Elem>::length() const noexcept {
        return _Mybuf._Size;
    }

    template <class _Elem>
    typename string<_Elem>::size_type string<_Elem>::max_size() const noexcept {
        // maximum representable size limited by pointer arithmetic and storage capacity
        const size_type _Storage_max = (::std::max)(
            _Small_buffer_capacity, (_Mybuf._Al->max_size() / sizeof(_Elem)) - 1);
        return (::std::min)(size_type{PTRDIFF_MAX}, _Storage_max);
    }

    template <class _Elem>
    void string<_Elem>::reserve(size_type _New_capacity) {
        if (_Mybuf._Capacity < _New_capacity && _New_capacity > _Small_buffer_capacity) {
            pointer _New_ptr = _Allocate_new_buffer(_New_capacity); // may raise an exception
            bstk::_Deallocate_object_array_guard _Guard{_New_ptr, _New_capacity + 1, *_Mybuf._Al};
            traits_type::copy(_New_ptr, _Mybuf._Get(), _Mybuf._Size + 1);
            _Mybuf._Deallocate_large_buffer();
            _Guard._Release();
            _Mybuf._Capacity = _New_capacity;
            _Mybuf._Large    = _New_ptr;
        }
    }

    template <class _Elem>
    typename string<_Elem>::size_type string<_Elem>::copy(
        pointer _Dest, size_type _Count, const size_type _Off) const {
        return static_cast<string_view<_Elem>>(*this).copy(_Dest, _Count, _Off);
    }

    template <class _Elem>
    void string<_Elem>::swap(string& _Other) noexcept {
        if (this == &_Other) {
            return;
        }

        const bool _This_small  = _Mybuf._Is_small();
        const bool _Other_small = _Other._Mybuf._Is_small();
        if (_This_small && _Other_small) { // swap small buffers
            _Elem _Buf[_Small_buffer_size]; // temporary buffer for _Mybuf._Small's data
            traits_type::copy(_Buf, _Mybuf._Small, _Mybuf._Size + 1);
            traits_type::copy(_Mybuf._Small, _Other._Mybuf._Small, _Other._Mybuf._Size + 1);
            traits_type::copy(_Other._Mybuf._Small, _Buf, _Mybuf._Size + 1);
        } else if (_This_small) { // swap small with large buffer
            _Mybuf._Swap_small_with_large(_Other._Mybuf);
        } else if (_Other_small) { // swap large with small buffer
            _Other._Mybuf._Swap_small_with_large(_Mybuf);
        } else { // swap large buffers
            ::std::swap(_Mybuf._Large, _Other._Mybuf._Large);
        }

        ::std::swap(_Mybuf._Al, _Other._Mybuf._Al);
        ::std::swap(_Mybuf._Capacity, _Other._Mybuf._Capacity);
        ::std::swap(_Mybuf._Size, _Other._Mybuf._Size);
    }

    template <class _Elem>
    void string<_Elem>::clear() noexcept {
        if (_Mybuf._Size > 0) {
            *_Mybuf._Get() = _Elem{0};
            _Mybuf._Size   = 0;
        }
    }

    template <class _Elem>
    void string<_Elem>::resize(const size_type _New_size, const value_type _Ch) {
        if (_Mybuf._Size >= _New_size) { // shrink buffer
            _Mybuf._Size                = _New_size;
            _Mybuf._Get()[_Mybuf._Size] = _Elem{0};
        } else { // expand buffer
            append(_New_size - _Mybuf._Size, _Ch);
        }
    }

    template <class _Elem>
    void string<_Elem>::shrink(const size_type _Count) noexcept {
        if (_Count > 0) {
            _Mybuf._Size               -= (::std::min)(_Count, _Mybuf._Size);
            _Mybuf._Get()[_Mybuf._Size] = _Elem{0};
        }
    }

    template <class _Elem>
    void string<_Elem>::shrink_to_fit() {
        if (_Mybuf._Is_small()) { // small buffer is considered fit
            return;
        }

        if (_Mybuf._Size <= _Small_buffer_capacity) { // switch to the small buffer
            _Mybuf._Switch_to_small();
            return;
        }

        size_type _New_capacity = _Mybuf._Size | _Alloc_mask;
        if (_New_capacity < _Mybuf._Capacity) { // worth shrinking, do it
            pointer _New_ptr = _Allocate_new_buffer(_New_capacity); // may raise an exception
            bstk::_Deallocate_object_array_guard _Guard{_New_ptr, _New_capacity + 1, *_Mybuf._Al};
            traits_type::copy(_New_ptr, _Mybuf._Large, _Mybuf._Size + 1);
            _Mybuf._Deallocate_large_buffer();
            _Guard._Release();
            _Mybuf._Capacity = _New_capacity;
            _Mybuf._Large    = _New_ptr;
        }
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::assign(const size_type _Count, const value_type _Ch) {
        if (_Count <= _Mybuf._Capacity) { // buffer has enough space, don't reallocate the memory
            pointer _Old_ptr = _Mybuf._Get();
            traits_type::assign(_Old_ptr, _Count, _Ch);
            _Mybuf._Size           = _Count;
            _Old_ptr[_Mybuf._Size] = _Elem{0};
        } else { // buffer has not enough space, reallocate the memory
            _Reallocate_assign(_Count, _Ch);
        }

        return *this;
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::assign(const string& _Str) {
        if (this != &_Str) { // must be a different string
            assign(_Str._Mybuf._Get(), _Str._Mybuf._Size);
        }

        return *this;
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::assign(string&& _Str) noexcept {
        if (this != &_Str) { // must be a different string
            _Mybuf._Move_contents(_Str._Mybuf);
        }

        return *this;
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::assign(const_pointer _Ptr, const size_type _Count) {
        if (_Count <= _Mybuf._Capacity) { // buffer has enough space, don't reallocate the memory
            pointer _Old_ptr = _Mybuf._Get();
            traits_type::copy(_Old_ptr, _Ptr, _Count);
            _Mybuf._Size           = _Count;
            _Old_ptr[_Mybuf._Size] = _Elem{0};
        } else { // buffer has not enough space, reallocate the memory
            _Reallocate_assign(_Ptr, _Count);
        }

        return *this;
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::assign(const_pointer _Ptr) {
        return assign(_Ptr, traits_type::length(_Ptr));
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::assign(const string_view<_Elem> _Str) {
        return assign(_Str.data(), _Str.size());
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::append(const size_type _Count, const value_type _Ch) {
        if (_Mybuf._Size + _Count <= _Mybuf._Capacity) { // buffer has enough space, don't reallocate the memory
            pointer _Old_ptr = _Mybuf._Get();
            traits_type::assign(_Old_ptr + _Mybuf._Size, _Count, _Ch);
            _Mybuf._Size          += _Count;
            _Old_ptr[_Mybuf._Size] = _Elem{0};
        } else { // buffer has not enough space, reallocate the memory
            _Reallocate_insert_back(_Count, _Ch);
        }

        return *this;
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::append(const string& _Str) {
        return append(_Str._Mybuf._Get(), _Str._Mybuf._Size);
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::append(const_pointer _Ptr, const size_type _Count) {
        if (_Mybuf._Size + _Count <= _Mybuf._Capacity) { // buffer has enough space, don't reallocate the memory
            pointer _Old_ptr = _Mybuf._Get();
            traits_type::copy(_Old_ptr + _Mybuf._Size, _Ptr, _Count);
            _Mybuf._Size          += _Count;
            _Old_ptr[_Mybuf._Size] = _Elem{0};
        } else { // buffer has not enough space, reallocate the memory
            _Reallocate_insert_back(_Ptr, _Count);
        }

        return *this;
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::append(const_pointer _Ptr) {
        return append(_Ptr, traits_type::length(_Ptr));
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::append(const string_view<_Elem> _Str) {
        return append(_Str.data(), _Str.size());
    }

    template <class _Elem>
    void string<_Elem>::push_back(const value_type _Ch) {
        append(1, _Ch);
    }

    template <class _Elem>
    void string<_Elem>::pop_back() noexcept {
#ifdef _DEBUG
        _INTERNAL_ASSERT(_Mybuf._Size > 0, "attempt to shrink empty string");
#endif // _DEBUG
        _Mybuf._Get()[--_Mybuf._Size] = _Elem{0};
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::erase(const size_type _Off, size_type _Count) {
        _Check_offset(_Off);
        _Count = (::std::min) (_Count, _Mybuf._Size - _Off);
        if (_Count > 0) { // erase [_Off, _Off + _Count) from the buffer
            pointer _Old_ptr          = _Mybuf._Get() + _Off;
            const size_type _New_size = _Mybuf._Size - _Count;
            traits_type::move(_Old_ptr, _Old_ptr + _Count, _New_size - _Off + 1);
            _Mybuf._Size = _New_size;
        }

        return *this;
    }

    template <class _Elem>
    typename string<_Elem>::iterator string<_Elem>::erase(const const_iterator _Where) {
        const size_type _Off = static_cast<size_type>(_Where._Myptr - _Mybuf._Get());
        erase(_Off, 1);
        return begin() + static_cast<difference_type>(_Off);
    }

    template <class _Elem>
    typename string<_Elem>::iterator string<_Elem>::erase(const const_iterator _First, const const_iterator _Last) {
        const size_type _Off = static_cast<size_type>(_First._Myptr - _Mybuf._Get());
        erase(_Off, static_cast<size_type>(_Last._Myptr - _First._Myptr));
        return begin() + static_cast<difference_type>(_Off);
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::insert(const size_type _Off, const size_type _Count, const value_type _Ch) {
        _Check_insert_offset(_Off);
        if (_Mybuf._Size + _Count <= _Mybuf._Capacity) { // buffer has enough space, don't reallocate the memory
            pointer _Old_ptr = _Mybuf._Get() + _Off;
            traits_type::move(_Old_ptr + _Count, _Old_ptr, _Mybuf._Size - _Off + 1);
            traits_type::assign(_Old_ptr, _Count, _Ch);
            _Mybuf._Size += _Count;
        } else { // buffer has not enough space, reallocate the memory
            _Reallocate_insert_at(_Off, _Count, _Ch);
        }

        return *this;
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::insert(const size_type _Off, const_pointer _Ptr) {
        return insert(_Off, _Ptr, traits_type::length(_Ptr));
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::insert(const size_type _Off, const_pointer _Ptr, const size_type _Count) {
        _Check_insert_offset(_Off);
        if (_Mybuf._Size + _Count <= _Mybuf._Capacity) { // buffer has enough space, don't reallocate the memory
            pointer _Old_ptr = _Mybuf._Get() + _Off;
            traits_type::move(_Old_ptr + _Count, _Old_ptr, _Mybuf._Size - _Off + 1);
            traits_type::copy(_Old_ptr, _Ptr, _Count);
            _Mybuf._Size += _Count;
        } else { // buffer has not enough space, reallocate the memory
            _Reallocate_insert_at(_Off, _Ptr, _Count);
        }

        return *this;
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::insert(const size_type _Off, const string& _Str) {
        return insert(_Off, _Str._Mybuf._Get(), _Str._Mybuf._Size);
    }

    template <class _Elem>
    typename string<_Elem>::iterator string<_Elem>::insert(const const_iterator _Where, const value_type _Ch) {
        const size_type _Off = static_cast<size_type>(_Where._Myptr - _Mybuf._Get());
        insert(_Off, 1, _Ch);
        return begin() + static_cast<difference_type>(_Off);
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::insert(const size_type _Off, const string_view<_Elem> _Str) {
        return insert(_Off, _Str.data(), _Str.size());
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::replace(const size_type _Off, size_type _Count, const string& _Str) {
        return replace(_Off, _Count, _Str._Mybuf._Get(), _Str._Mybuf._Size);
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::replace(
        const const_iterator _First, const const_iterator _Last, const string& _Str) {
        return replace(static_cast<size_type>(_First._Myptr - _Mybuf._Get()),
            static_cast<size_type>(_Last._Myptr - _First._Myptr), _Str._Mybuf._Get(), _Str._Mybuf._Size);
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::replace(
        const size_type _Off, size_type _Count, const_pointer _Ptr, const size_type _Ptr_count) {
        _Check_offset(_Off);
        _Count = (::std::min)(_Count, _Mybuf._Size);
        if (_Count >= _Ptr_count) { // buffer size will remain the same
            const size_type _Reduction = _Count - _Ptr_count;
            pointer _Old_ptr           = _Mybuf._Get() + _Off;
            traits_type::copy(_Old_ptr, _Ptr, _Ptr_count);
            if (_Reduction > 0) { // erase [_Off + _Ch_count, _Off + _Count)
                traits_type::move(_Old_ptr + _Ptr_count, _Old_ptr + _Count, _Mybuf._Size - _Off - _Count + 1);
                _Mybuf._Size -= _Reduction;
            }

            return *this;
        }

        const size_type _Growth = _Ptr_count - _Count;
        if (_Mybuf._Size + _Growth <= _Mybuf._Capacity) { // buffer has enough space, don't reallocate the memory
            pointer _Old_ptr = _Mybuf._Get() + _Off;
            traits_type::move(_Old_ptr + _Ptr_count, _Old_ptr + _Count, _Mybuf._Size - _Off - _Count + 1);
            traits_type::copy(_Old_ptr, _Ptr, _Ptr_count);
            _Mybuf._Size += _Growth;
        } else { // buffer has not enough space, reallocate the memory
            _Reallocate_replace(_Off, _Count, _Ptr, _Ptr_count);
        }

        return *this;
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::replace(const const_iterator _First,
        const const_iterator _Last, const const_pointer _Ptr, const size_type _Count) {
        return replace(static_cast<size_type>(_First._Myptr - _Mybuf._Get()),
            static_cast<size_type>(_Last._Myptr - _First._Myptr), _Ptr, _Count);
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::replace(const size_type _Off, size_type _Count, const_pointer _Ptr) {
        return replace(_Off, _Count, _Ptr, traits_type::length(_Ptr));
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::replace(
        const const_iterator _First, const const_iterator _Last, const_pointer _Ptr) {
        return replace(static_cast<size_type>(_First._Myptr - _Mybuf._Get()),
            static_cast<size_type>(_Last._Myptr - _First._Myptr), _Ptr, traits_type::length(_Ptr));
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::replace(
        const size_type _Off, size_type _Count, const size_type _Ch_count, const value_type _Ch) {
        _Check_offset(_Off);
        _Count = (::std::min)(_Count, _Mybuf._Size);
        if (_Count >= _Ch_count) { // buffer size will remain the same
            const size_type _Reduction = _Count - _Ch_count;
            pointer _Old_ptr           = _Mybuf._Get() + _Off;
            traits_type::assign(_Old_ptr, _Ch_count, _Ch);
            if (_Reduction > 0) { // erase [_Off + _Ch_count, _Off + _Count)
                traits_type::move(_Old_ptr + _Ch_count, _Old_ptr + _Count, _Mybuf._Size - _Off - _Count + 1);
                _Mybuf._Size -= _Reduction;
            }

            return *this;
        }

        const size_type _Growth = _Ch_count - _Count;
        if (_Mybuf._Size + _Growth <= _Mybuf._Capacity) { // buffer has enough space, don't reallocate the memory
            pointer _Old_ptr = _Mybuf._Get() + _Off;
            traits_type::move(_Old_ptr + _Ch_count, _Old_ptr + _Count, _Mybuf._Size - _Off - _Count + 1);
            traits_type::assign(_Old_ptr, _Ch_count, _Ch);
            _Mybuf._Size += _Growth;
        } else { // buffer has not enough space, reallocate the memory
            _Reallocate_replace(_Off, _Count, _Ch_count, _Ch);
        }

        return *this;
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::replace(const const_iterator _First,
        const const_iterator _Last, const size_type _Count, const value_type _Ch) {
        return replace(static_cast<size_type>(_First._Myptr - _Mybuf._Get()),
            static_cast<size_type>(_Last._Myptr - _First._Myptr), _Count, _Ch);
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::replace(const size_type _Off, size_type _Count, const string_view<_Elem> _Str) {
        return replace(_Off, _Count, _Str.data(), _Str.size());
    }

    template <class _Elem>
    string<_Elem>& string<_Elem>::replace(const const_iterator _First,
        const const_iterator _Last, const string_view<_Elem> _Str) {
        return replace(static_cast<size_type>(_First._Myptr - _Mybuf._Get()),
            static_cast<size_type>(_Last._Myptr - _First._Myptr), _Str.data(), _Str.size());
    }

    template <class _Elem>
    typename string<_Elem>::size_type string<_Elem>::find(
        const string& _Str, const size_type _Off) const noexcept {
        return static_cast<string_view<_Elem>>(*this).find(_Str, _Off);
    }

    template <class _Elem>
    typename string<_Elem>::size_type string<_Elem>::find(
        const_pointer _Ptr, const size_type _Off, const size_type _Count) const noexcept {
        return static_cast<string_view<_Elem>>(*this).find(_Ptr, _Off, _Count);
    }

    template <class _Elem>
    typename string<_Elem>::size_type string<_Elem>::find(
        const_pointer _Ptr, const size_type _Off) const noexcept {
        return static_cast<string_view<_Elem>>(*this).find(_Ptr, _Off);
    }

    template <class _Elem>
    typename string<_Elem>::size_type string<_Elem>::find(
        const value_type _Ch, const size_type _Off) const noexcept {
        return static_cast<string_view<_Elem>>(*this).find(_Ch);
    }

    template <class _Elem>
    typename string<_Elem>::size_type string<_Elem>::find(
        const string_view<_Elem> _Str, const size_type _Off) const noexcept {
        return static_cast<string_view<_Elem>>(*this).find(_Str, _Off);
    }

    template <class _Elem>
    typename string<_Elem>::size_type string<_Elem>::rfind(
        const string& _Str, const size_type _Off) const noexcept {
        return static_cast<string_view<_Elem>>(*this).rfind(_Str, _Off);
    }

    template <class _Elem>
    typename string<_Elem>::size_type string<_Elem>::rfind(
        const_pointer _Ptr, const size_type _Off, const size_type _Count) const noexcept {
        return static_cast<string_view<_Elem>>(*this).rfind(_Ptr, _Off, _Count);
    }

    template <class _Elem>
    typename string<_Elem>::size_type string<_Elem>::rfind(
        const_pointer _Ptr, const size_type _Off) const noexcept {
        return static_cast<string_view<_Elem>>(*this).rfind(_Ptr, _Off);
    }

    template <class _Elem>
    typename string<_Elem>::size_type string<_Elem>::rfind(
        const value_type _Ch, const size_type _Off) const noexcept {
        return static_cast<string_view<_Elem>>(*this).rfind(_Ch, _Off);
    }

    template <class _Elem>
    typename string<_Elem>::size_type string<_Elem>::rfind(
        const string_view<_Elem> _Str, const size_type _Off) const noexcept {
        return static_cast<string_view<_Elem>>(*this).rfind(_Str, _Off);
    }

    template <class _Elem>
    int string<_Elem>::compare(const string& _Str) const noexcept {
        return static_cast<string_view<_Elem>>(*this).compare(_Str);
    }

    template <class _Elem>
    int string<_Elem>::compare(const_pointer _Ptr, const size_type _Count) const noexcept {
        return static_cast<string_view<_Elem>>(*this).compare(_Ptr, _Count);
    }

    template <class _Elem>
    int string<_Elem>::compare(const_pointer _Ptr) const noexcept {
        return static_cast<string_view<_Elem>>(*this).compare(_Ptr);
    }

    template <class _Elem>
    int string<_Elem>::compare(const string_view<_Elem> _Str) const noexcept {
        return static_cast<string_view<_Elem>>(*this).compare(_Str);
    }

    template <class _Elem>
    bool string<_Elem>::starts_with(const string_view<_Elem> _Str) const noexcept {
        return static_cast<string_view<_Elem>>(*this).starts_with(_Str);
    }

    template <class _Elem>
    bool string<_Elem>::starts_with(const value_type _Ch) const noexcept {
        return static_cast<string_view<_Elem>>(*this).starts_with(_Ch);
    }

    template <class _Elem>
    bool string<_Elem>::starts_with(const_pointer _Ptr) const noexcept {
        return static_cast<string_view<_Elem>>(*this).starts_with(_Ptr);
    }

    template <class _Elem>
    bool string<_Elem>::ends_with(const string_view<_Elem> _Str) const noexcept {
        return static_cast<string_view<_Elem>>(*this).ends_with(_Str);
    }

    template <class _Elem>
    bool string<_Elem>::ends_with(const value_type _Ch) const noexcept {
        return static_cast<string_view<_Elem>>(*this).ends_with(_Ch);
    }

    template <class _Elem>
    bool string<_Elem>::ends_with(const_pointer _Ptr) const noexcept {
        return static_cast<string_view<_Elem>>(*this).ends_with(_Ptr);
    }

    template <class _Elem>
    bool string<_Elem>::contains(const string_view<_Elem> _Str) const noexcept {
        return static_cast<string_view<_Elem>>(*this).contains(_Str);
    }

    template <class _Elem>
    bool string<_Elem>::contains(const value_type _Ch) const noexcept {
        return static_cast<string_view<_Elem>>(*this).contains(_Ch);
    }

    template <class _Elem>
    bool string<_Elem>::contains(const_pointer _Ptr) const noexcept {
        return static_cast<string_view<_Elem>>(*this).contains(_Ptr);
    }

    template <class _Elem>
    string<_Elem> string<_Elem>::substr(const size_type _Off, size_type _Count) const {
        _Check_offset(_Off);
        _Count = (::std::min)(_Count, _Mybuf._Size - _Off);
        return string{_Mybuf._Get() + _Off, _Count};
    }

    template class _BSTK_EXPORT string<byte_t>;
    template class _BSTK_EXPORT string<char>;
    template class _BSTK_EXPORT string<wchar_t>;
} // namespace bs