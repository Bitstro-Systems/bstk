// string_view.hpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#pragma once
#ifndef _BSTK_TEXT_STRING_VIEW_HPP_
#define _BSTK_TEXT_STRING_VIEW_HPP_
#include <bstk/core/export.hpp>
#include <bstk/text/char_traits.hpp>
#include <compare>
#include <iterator>

namespace bs {
    template <class>
    class string_view;

    template <class _Elem>
    class _BSTK_EXPORT string_view_iterator { // random access iterator for string_view<_Elem>
    public:
        static_assert(bstk::_Char_type<_Elem>, "invalid element type for string_view_iterator<_Elem>");

        using value_type        = _Elem;
        using difference_type   = ptrdiff_t;
        using pointer           = const _Elem*;
        using reference         = const _Elem&;
        using iterator_category = ::std::random_access_iterator_tag;

        string_view_iterator() noexcept;

        string_view_iterator(const string_view_iterator&) noexcept = default;
        string_view_iterator(string_view_iterator&&) noexcept      = default;
        ~string_view_iterator() noexcept                           = default;

        string_view_iterator& operator=(const string_view_iterator&) noexcept = default;
        string_view_iterator& operator=(string_view_iterator&&) noexcept      = default;

        // checks whether the iterator is valid
        bool valid() const noexcept;

        // returns a reference to the character pointed to by the iterator
        reference operator*() const noexcept;

        // returns a pointer to the currently referenced character
        pointer operator->() const noexcept;

        // returns a reference to the character at the specified offset from the current position
        reference operator[](const difference_type _Off) const noexcept;

        // advances the iterator to the next position
        string_view_iterator& operator++() noexcept;

        // advances the iterator to the next position and returns the previous state
        string_view_iterator operator++(int) noexcept;

        // moves the iterator to the previous position
        string_view_iterator& operator--() noexcept;
    
        // moves the iterator to the previous position and returns the previous state
        string_view_iterator operator--(int) noexcept;

        // moves the iterator forward by _Off elements
        string_view_iterator& operator+=(const difference_type _Off) noexcept;

        // moves the iterator backward by _Off elements
        string_view_iterator& operator-=(const difference_type _Off) noexcept;

        // returns a new iterator moved forward by _Off elements
        string_view_iterator operator+(const difference_type _Off) const noexcept;

        // returns a new iterator moved backward by _Off elements
        string_view_iterator operator-(const difference_type _Off) const noexcept;

        // checks whether two iterators refer to the same position
        bool operator==(const string_view_iterator& _Other) const noexcept;

        // performs a three-way comparison between iterator positions
        ::std::strong_ordering operator<=>(const string_view_iterator& _Other) const noexcept;

    private:
        friend string_view<_Elem>;

#ifdef _DEBUG
        string_view_iterator(pointer _Begin, pointer _End) noexcept;
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        explicit string_view_iterator(pointer _Ptr) noexcept;
#endif // _DEBUG

        pointer _Myptr;
#ifdef _DEBUG
        pointer _Mybegin;
        pointer _Myend;
#endif // _DEBUG
    };

    using byte_string_view_iterator    = string_view_iterator<byte_t>;
    using utf8_string_view_iterator    = string_view_iterator<char>;
    using unicode_string_view_iterator = string_view_iterator<wchar_t>;

    template <class _Elem>
    class _BSTK_EXPORT string_view { // lightweight non-owning read-only view of a string
    public:
        static_assert(bstk::_Char_type<_Elem>, "invalid element type for string_view<_Elem>");

        using value_type      = _Elem;
        using size_type       = size_t;
        using difference_type = ptrdiff_t;
        using pointer         = _Elem*;
        using const_pointer   = const _Elem*;
        using reference       = _Elem&;
        using const_reference = const _Elem&;
        using traits_type     = char_traits<_Elem>;

        using const_iterator         = string_view_iterator<_Elem>;
        using iterator               = const_iterator;
        using const_reverse_iterator = ::std::reverse_iterator<const_iterator>;
        using reverse_iterator       = const_reverse_iterator;

        static constexpr size_type npos = traits_type::npos;

        string_view() noexcept;
        string_view(const_pointer _Ptr, const size_type _Count) noexcept;
        string_view(const_pointer _Ptr) noexcept;

        string_view(const string_view&) noexcept            = default;
        string_view& operator=(const string_view&) noexcept = default;

        string_view(::std::nullptr_t) = delete;

        // accesses the specified character
        const_reference operator[](const size_type _Idx) const noexcept;

        // returns an iterator to the beginning
        const_iterator begin() const noexcept;
        const_iterator cbegin() const noexcept;

        // returns an iterator to the end
        const_iterator end() const noexcept;
        const_iterator cend() const noexcept;

        // returns a reverse iterator to the beginning
        const_reverse_iterator rbegin() const noexcept;
        const_reverse_iterator crbegin() const noexcept;

        // returns a reverse iterator to the end
        const_reverse_iterator rend() const noexcept;
        const_reverse_iterator crend() const noexcept;

        // accesses the specified character with bounds checking
        const_reference at(const size_type _Idx) const;

        // accesses the first character
        const_reference front() const noexcept;

        // accesses the last character
        const_reference back() const noexcept;

        // returns a pointer to the first character of a view
        const_pointer data() const noexcept;

        // returns the number of characters
        size_type size() const noexcept;
        size_type length() const noexcept;

        // returns the maximum number of characters
        size_type max_size() const noexcept;

        // checks whether the view is empty
        bool empty() const noexcept;

        // shrinks the view by moving its start forward
        void remove_prefix(const size_type _Count) noexcept;

        // shrinks the view by moving its end backward
        void remove_suffix(const size_type _Count) noexcept;

        // swaps the contents
        void swap(string_view& _Other) noexcept;

        // copies characters
        size_type copy(pointer _Dest, size_type _Count, const size_type _Off = 0) const;

        // returns a substring
        string_view substr(const size_type _Off = 0, size_type _Count = npos) const;

        // compares two views
        int compare(const string_view _Str) const noexcept;
        int compare(const_pointer _Ptr, const size_type _Count) const noexcept;
        int compare(const_pointer _Ptr) const noexcept;

        // checks if the string view starts with the given prefix
        bool starts_with(const string_view _Str) const noexcept;
        bool starts_with(const value_type _Ch) const noexcept;
        bool starts_with(const_pointer _Ptr) const noexcept;

        // checks if the string view ends with the given suffix
        bool ends_with(const string_view _Str) const noexcept;
        bool ends_with(const value_type _Ch) const noexcept;
        bool ends_with(const_pointer _Ptr) const noexcept;

        // checks if the string view contains the given substring or character
        bool contains(const string_view _Str) const noexcept;
        bool contains(const value_type _Ch) const noexcept;
        bool contains(const_pointer _Ptr) const noexcept;

        // find characters in the view
        size_type find(const string_view _Str, const size_type _Off = 0) const noexcept;
        size_type find(const value_type _Ch, const size_type _Off = 0) const noexcept;
        size_type find(const_pointer _Ptr, const size_type _Off, const size_type _Count) const noexcept;
        size_type find(const_pointer _Ptr, const size_type _Off = 0) const noexcept;

        // finds the last occurrence of a substring
        size_type rfind(const string_view _Str, const size_type _Off = npos) const noexcept;
        size_type rfind(const value_type _Ch, const size_type _Off = npos) const noexcept;
        size_type rfind(const_pointer _Ptr, const size_type _Off, const size_type _Count) const noexcept;
        size_type rfind(const_pointer _Ptr, const size_type _Off = npos) const noexcept;

    private:
        // checks whether the offset is in the bounds
        void _Check_offset(const size_type _Off) const;

        const_pointer _Mydata;
        size_type _Mysize;
    };

    using byte_string_view    = string_view<byte_t>;
    using utf8_string_view    = string_view<char>;
    using unicode_string_view = string_view<wchar_t>;

    template <class _Elem>
    bool operator==(const string_view<_Elem> _Left, const string_view<_Elem> _Right) noexcept {
        return _Left.compare(_Right) == 0;
    }

    template <class _Elem>
    bool operator==(const string_view<_Elem> _Left, const _Elem* const _Right) noexcept {
        return _Left.compare(_Right) == 0;
    }

    template <class _Elem>
    ::std::strong_ordering operator<=>(const string_view<_Elem> _Left, const string_view<_Elem> _Right) noexcept {
        return _Left.compare(_Right) <=> 0;
    }

    template <class _Elem>
    ::std::strong_ordering operator<=>(const string_view<_Elem> _Left, const _Elem* const _Right) noexcept {
        return _Left.compare(_Right) <=> 0;
    }
} // namespace bs

#endif // _BSTK_TEXT_STRING_VIEW_HPP_