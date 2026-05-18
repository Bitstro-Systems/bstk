// string.hpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#pragma once
#ifndef _BSTK_TEXT_STRING_HPP_
#define _BSTK_TEXT_STRING_HPP_
#include <bstk/core/export.hpp>
#include <bstk/memory/allocator.hpp>
#include <bstk/memory/object.hpp>
#include <bstk/text/char_traits.hpp>
#include <bstk/text/string_view.hpp>
#include <compare>
#include <iterator>

namespace bs {
    template <class>
    class string;
    
    template <class>
    class string_iterator;

    template <class _Elem>
    class _BSTK_EXPORT string_const_iterator { // read-only random access iterator for string<_Elem>
    public:
        static_assert(bstk::_Char_type<_Elem>, "invalid element type for string_const_iterator<_Elem>");

        using value_type        = _Elem;
        using difference_type   = ptrdiff_t;
        using pointer           = const _Elem*;
        using reference         = const _Elem&;
        using iterator_category = ::std::random_access_iterator_tag;

        string_const_iterator() noexcept;

        string_const_iterator(const string_const_iterator&) noexcept = default;
        string_const_iterator(string_const_iterator&&) noexcept      = default;
        ~string_const_iterator() noexcept                            = default;

        string_const_iterator& operator=(const string_const_iterator&) noexcept = default;
        string_const_iterator& operator=(string_const_iterator&&) noexcept      = default;

        // checks whether the iterator is valid
        bool valid() const noexcept;

        // returns a reference to the character pointed to by the iterator
        reference operator*() const noexcept;

        // returns a pointer to the currently referenced character
        pointer operator->() const noexcept;

        // returns a reference to the character at the specified offset from the current position
        reference operator[](const difference_type _Off) const noexcept;

        // advances the iterator to the next position
        string_const_iterator& operator++() noexcept;

        // advances the iterator to the next position and returns the previous state
        string_const_iterator operator++(int) noexcept;

        // moves the iterator to the previous position
        string_const_iterator& operator--() noexcept;
    
        // moves the iterator to the previous position and returns the previous state
        string_const_iterator operator--(int) noexcept;

        // moves the iterator forward by _Off elements
        string_const_iterator& operator+=(const difference_type _Off) noexcept;

        // moves the iterator backward by _Off elements
        string_const_iterator& operator-=(const difference_type _Off) noexcept;

        // returns a new iterator moved forward by _Off elements
        string_const_iterator operator+(const difference_type _Off) const noexcept;

        // returns a new iterator moved backward by _Off elements
        string_const_iterator operator-(const difference_type _Off) const noexcept;

        // checks whether two iterators refer to the same position
        bool operator==(const string_const_iterator& _Other) const noexcept;

        // performs a three-way comparison between iterator positions
        ::std::strong_ordering operator<=>(const string_const_iterator& _Other) const noexcept;

    private:
        friend string<_Elem>;
        friend string_iterator<_Elem>;

#ifdef _DEBUG
        string_const_iterator(pointer _Begin, pointer _End) noexcept;
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        explicit string_const_iterator(pointer _Ptr) noexcept;
#endif // _DEBUG

        pointer _Myptr;
#ifdef _DEBUG
        pointer _Mybegin;
        pointer _Myend;
#endif // _DEBUG
    };

    using byte_string_const_iterator    = string_const_iterator<byte_t>;
    using utf8_string_const_iterator    = string_const_iterator<char>;
    using unicode_string_const_iterator = string_const_iterator<wchar_t>;

    template <class _Elem>
    class _BSTK_EXPORT string_iterator
        : public string_const_iterator<_Elem> { // random access iterator for string<_Elem>
    public:
        static_assert(bstk::_Char_type<_Elem>, "invalid element type for string_iterator<_Elem>");

        using value_type        = _Elem;
        using difference_type   = ptrdiff_t;
        using pointer           = _Elem*;
        using reference         = _Elem&;
        using iterator_category = ::std::random_access_iterator_tag;

        string_iterator() noexcept                       = default;
        string_iterator(const string_iterator&) noexcept = default;
        string_iterator(string_iterator&&) noexcept      = default;
        ~string_iterator() noexcept                      = default;

        string_iterator& operator=(const string_iterator&) noexcept = default;
        string_iterator& operator=(string_iterator&&) noexcept      = default;

        // checks whether the iterator is valid
        bool valid() const noexcept;

        // returns a reference to the character pointed to by the iterator
        reference operator*() const noexcept;

        // returns a pointer to the currently referenced character
        pointer operator->() const noexcept;

        // returns a reference to the character at the specified offset from the current position
        reference operator[](const difference_type _Off) const noexcept;

        // advances the iterator to the next position
        string_iterator& operator++() noexcept;

        // advances the iterator to the next position and returns the previous state
        string_iterator operator++(int) noexcept;

        // moves the iterator to the previous position
        string_iterator& operator--() noexcept;
    
        // moves the iterator to the previous position and returns the previous state
        string_iterator operator--(int) noexcept;

        // moves the iterator forward by _Off elements
        string_iterator& operator+=(const difference_type _Off) noexcept;

        // moves the iterator backward by _Off elements
        string_iterator& operator-=(const difference_type _Off) noexcept;

        // returns a new iterator moved forward by _Off elements
        string_iterator operator+(const difference_type _Off) const noexcept;

        // returns a new iterator moved backward by _Off elements
        string_iterator operator-(const difference_type _Off) const noexcept;

        // checks whether two iterators refer to the same position
        bool operator==(const string_iterator& _Other) const noexcept;

        // performs a three-way comparison between iterator positions
        ::std::strong_ordering operator<=>(const string_iterator& _Other) const noexcept;

    private:
        using _Mybase = string_const_iterator<_Elem>;

        friend string<_Elem>;

#ifdef _DEBUG
        string_iterator(pointer _Begin, pointer _End) noexcept;
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        explicit string_iterator(pointer _Ptr) noexcept;
#endif // _DEBUG
    };

    using byte_string_iterator    = string_iterator<byte_t>;
    using utf8_string_iterator    = string_iterator<char>;
    using unicode_string_iterator = string_iterator<wchar_t>;

    template <class _Elem>
    class _BSTK_EXPORT string { // dynamic sequence of characters
    public:
        static_assert(bstk::_Char_type<_Elem>, "invalid element type for string<_Elem>");

        using value_type      = _Elem;
        using size_type       = size_t;
        using difference_type = ptrdiff_t;
        using pointer         = _Elem*;
        using const_pointer   = const _Elem*;
        using reference       = _Elem&;
        using const_reference = const _Elem&;
        using traits_type     = char_traits<_Elem>;

        using iterator               = string_iterator<_Elem>;
        using const_iterator         = string_const_iterator<_Elem>;
        using reverse_iterator       = ::std::reverse_iterator<iterator>;
        using const_reverse_iterator = ::std::reverse_iterator<const_iterator>;

        static constexpr size_type npos = traits_type::npos;

        string() noexcept;
        string(const string& _Other);
        string(string&& _Other) noexcept;

        explicit string(allocator& _Al) noexcept;

        string(const string& _Other, allocator& _Al);
        string(string&& _Other, allocator& _Al) noexcept;

        string(const size_type _Count, const value_type _Ch);
        string(const size_type _Count, const value_type _Ch, allocator& _Al);

        string(const_pointer _Ptr, const size_type _Count);
        string(const_pointer _Ptr, const size_type _Count, allocator& _Al);

        string(const_pointer _Ptr);
        string(const_pointer _Ptr, allocator& _Al);

        string(const string_view<_Elem> _Str);
        string(const string_view<_Elem> _Str, allocator& _Al);

        ~string() noexcept = default;

        string(::std::nullptr_t)            = delete;
        string& operator=(::std::nullptr_t) = delete;

        // assigns characters to the string
        string& operator=(const string& _Str);
        string& operator=(string&& _Str) noexcept;
        string& operator=(const_pointer _Ptr);
        string& operator=(const value_type _Ch);
        string& operator=(const string_view<_Elem> _Str);

        // appends characters to the end
        string& operator+=(const string& _Str);
        string& operator+=(const_pointer _Ptr);
        string& operator+=(const value_type _Ch);
        string& operator+=(const string_view<_Elem> _Str);

        // returns a non-modifiable string_view into the entire string
        operator string_view<_Elem>() const noexcept;

        // accesses the specified character
        reference operator[](const size_type _Idx) noexcept;
        const_reference operator[](const size_type _Idx) const noexcept;

        // returns the associated allocator
        allocator& get_allocator() noexcept;

        // returns an iterator to the beginning
        iterator begin() noexcept;
        const_iterator begin() const noexcept;
        const_iterator cbegin() const noexcept;

        // returns an iterator to the end
        iterator end() noexcept;
        const_iterator end() const noexcept;
        const_iterator cend() const noexcept;

        // returns a reverse iterator to the beginning
        reverse_iterator rbegin() noexcept;
        const_reverse_iterator rbegin() const noexcept;
        const_reverse_iterator crbegin() const noexcept;

        // returns a reverse iterator to the end
        reverse_iterator rend() noexcept;
        const_reverse_iterator rend() const noexcept;
        const_reverse_iterator crend() const noexcept;

        // accesses the specified character with bounds checking
        reference at(const size_type _Idx);
        const_reference at(const size_type _Idx) const;

        // accesses the first character
        reference front() noexcept;
        const_reference front() const noexcept;

        // accesses the last character
        reference back() noexcept;
        const_reference back() const noexcept;

        // returns a pointer to the first character of a string
        pointer data() noexcept;
        const_pointer data() const noexcept;

        // returns a non-modifiable standard C character array version of the string
        const_pointer c_str() const noexcept;

        // checks whether the string is empty
        bool empty() const noexcept;

        // returns the number of characters that can be held in currently allocated storage
        size_type capacity() const noexcept;

        // returns the number of characters
        size_type size() const noexcept;
        size_type length() const noexcept;

        // returns the maximum number of characters
        size_type max_size() const noexcept;

        // reserves storage
        void reserve(size_type _New_capacity);

        // copies characters
        size_type copy(pointer _Dest, size_type _Count, const size_type _Off = 0) const;

        // swaps the contents
        void swap(string& _Other) noexcept;

        // clears the contents
        void clear() noexcept;

        // changes the number of characters stored
        void resize(const size_type _New_size, const value_type _Ch = value_type{0});

        // decreases the number of characters stored
        void shrink(const size_type _Count) noexcept;

        // reduces memory usage by freeing unused memory
        void shrink_to_fit();

        // assigns characters to the string
        string& assign(const size_type _Count, const value_type _Ch);
        string& assign(const string& _Str);
        string& assign(string&& _Str) noexcept;
        string& assign(const_pointer _Ptr, const size_type _Count);
        string& assign(const_pointer _Ptr);
        string& assign(const string_view<_Elem> _Str);

        // appends characters to the end
        string& append(const size_type _Count, const value_type _Ch);
        string& append(const string& _Str);
        string& append(const_pointer _Ptr, const size_type _Count);
        string& append(const_pointer _Ptr);
        string& append(const string_view<_Elem> _Str);

        // appends a character to the end
        void push_back(const value_type _Ch);

        // remvoes the last character
        void pop_back() noexcept;

        // removes characters
        string& erase(const size_type _Off = 0, size_type _Count = npos);
        iterator erase(const const_iterator _Where);
        iterator erase(const const_iterator _First, const const_iterator _Last);

        // inserts characters
        string& insert(const size_type _Off, const size_type _Count, const value_type _Ch);
        string& insert(const size_type _Off, const_pointer _Ptr);
        string& insert(const size_type _Off, const_pointer _Ptr, const size_type _Count);
        string& insert(const size_type _Off, const string& _Str);
        iterator insert(const const_iterator _Where, const value_type _Ch);
        string& insert(const size_type _Off, const string_view<_Elem> _Str);

        // replaces specified portion of the string
        string& replace(const size_type _Off, size_type _Count, const string& _Str);
        string& replace(const const_iterator _First, const const_iterator _Last, const string& _Str);
        string& replace(const size_type _Off, size_type _Count, const_pointer _Ptr, const size_type _Ptr_count);
        string& replace(const const_iterator _First, const const_iterator _Last,
            const_pointer _Ptr, const size_type _Count);
        string& replace(const size_type _Off, size_type _Count, const_pointer _Ptr);
        string& replace(const const_iterator _First, const const_iterator _Last, const_pointer _Ptr);
        string& replace(const size_type _Off, size_type _Count, const size_type _Ch_count, const value_type _Ch);
        string& replace(const const_iterator _First, const const_iterator _Last,
            const size_type _Count, const value_type _Ch);
        string& replace(const size_type _Off, size_type _Count, const string_view<_Elem> _Str);
        string& replace(const const_iterator _First, const const_iterator _Last, const string_view<_Elem> _Str);

        // finds the first occurrence of the given substring
        size_type find(const string& _Str, const size_type _Off = 0) const noexcept;
        size_type find(const_pointer _Ptr, const size_type _Off, const size_type _Count) const noexcept;
        size_type find(const_pointer _Ptr, const size_type _Off) const noexcept;
        size_type find(const value_type _Ch, const size_type _Off = 0) const noexcept;
        size_type find(const string_view<_Elem> _Str, const size_type _Off = 0) const noexcept;

        // finds the last occurrence of the given substring
        size_type rfind(const string& _Str, const size_type _Off = npos) const noexcept;
        size_type rfind(const_pointer _Ptr, const size_type _Off, const size_type _Count) const noexcept;
        size_type rfind(const_pointer _Ptr, const size_type _Off = npos) const noexcept;
        size_type rfind(const value_type _Ch, const size_type _Off = npos) const noexcept;
        size_type rfind(const string_view<_Elem> _Str, const size_type _Off = npos) const noexcept;

        // compares two strings
        int compare(const string& _Str) const noexcept;
        int compare(const_pointer _Ptr, const size_type _Count) const noexcept;
        int compare(const_pointer _Ptr) const noexcept;
        int compare(const string_view<_Elem> _Str) const noexcept;

        // checks if the string starts with the given prefix
        bool starts_with(const string_view<_Elem> _Str) const noexcept;
        bool starts_with(const value_type _Ch) const noexcept;
        bool starts_with(const_pointer _Ptr) const noexcept;

        // checks if the string ends with the given suffix
        bool ends_with(const string_view<_Elem> _Str) const noexcept;
        bool ends_with(const value_type _Ch) const noexcept;
        bool ends_with(const_pointer _Ptr) const noexcept;

        // checks if the string contains the given substring or character
        bool contains(const string_view<_Elem> _Str) const noexcept;
        bool contains(const value_type _Ch) const noexcept;
        bool contains(const_pointer _Ptr) const noexcept;

        // returns a substring
        string substr(const size_type _Off = 0, size_type _Count = npos) const;

    private:
        // round up dynamically allocated buffer size to 16 bytes
        static constexpr size_type _Alloc_mask = (16 / sizeof(value_type)) - 1;

        // store up to 16 bytes in small buffer
        static constexpr size_type _Small_buffer_size     = 16 / sizeof(value_type);
        static constexpr size_type _Small_buffer_capacity = _Small_buffer_size - 1;

        class _String_buffer { // stores string buffers
        public:
            explicit _String_buffer(allocator* const _Al = nullptr) noexcept;
            ~_String_buffer() noexcept;

            _String_buffer(const _String_buffer&)            = delete;
            _String_buffer& operator=(const _String_buffer&) = delete;

            // checks whether small buffer is used
            bool _Is_small() const noexcept;

            // returns the currently used buffer
            pointer _Get() noexcept;
            const_pointer _Get() const noexcept;

            // switchs from large to small buffer
            void _Switch_to_small();

            // swaps small buffer with large one
            void _Swap_small_with_large(_String_buffer& _Other) noexcept;

            // deallocates large buffer
            void _Deallocate_large_buffer();

            // destroys the buffer
            void _Destroy();
            
            // moves the contents of another buffer
            void _Move_contents(_String_buffer& _Other, allocator* const _New_al = nullptr);

            allocator* _Al;
            size_type _Capacity; // number of characters that can be stored without reallocating memory
            size_type _Size; // number of characters currently stored in the string
            union {
                value_type _Small[_Small_buffer_size];
                pointer _Large;
            };
        };

        // allocates memory for the string capacity
        pointer _Allocate_new_buffer(size_type& _Capacity);

        // checks whether the given offset is within [0, size())
        void _Check_offset(const size_type _Off) const;

        // checks whether the given offset is within [0, size()]
        void _Check_insert_offset(const size_type _Off) const;

        // constructs the string from a pointer
        void _Construct_from_ptr(const_pointer _Ptr, const size_type _Count);

        // constructs the string from a character sequence
        void _Construct_from_chars(const size_type _Count, const value_type _Ch);

        // resizes buffer capacity and stores new data at the beginning
        void _Reallocate_assign(const size_type _Count, const value_type _Ch);
        void _Reallocate_assign(const_pointer _Ptr, const size_type _Count);

        // increases buffer capacity and stores new data at the end
        void _Reallocate_insert_back(const size_type _Count, const value_type _Ch);
        void _Reallocate_insert_back(const_pointer _Ptr, const size_type _Count);

        // increases buffer capacity and stores new data at the specified position
        void _Reallocate_insert_at(const size_type _Off, const size_type _Count, const value_type _Ch);
        void _Reallocate_insert_at(const size_type _Off, const_pointer _Ptr, const size_type _Count);

        // increases buffer capacity and replaces the data at the specified position
        void _Reallocate_replace(const size_type _Off, const size_type _Count,
            const size_type _Ch_count, const value_type _Ch);
        void _Reallocate_replace(const size_type _Off, const size_type _Count,
            const_pointer _Ptr, const size_type _Ptr_count);

        template <class _Fn>
        void _Construct(const size_type _Count, _Fn&& _Func) {
            if (_Count <= _Small_buffer_capacity) { // use small buffer
                _Func(_Mybuf._Small);
                _Mybuf._Size                = _Count;
                _Mybuf._Small[_Mybuf._Size] = _Elem{0};
            } else { // use large buffer
                size_type _New_capacity = _Count;
                pointer _New_ptr        = _Allocate_new_buffer(_New_capacity); // may raise an exception
                _Func(_New_ptr);
                _Mybuf._Capacity            = _New_capacity;
                _Mybuf._Size                = _Count;
                _Mybuf._Large               = _New_ptr;
                _Mybuf._Large[_Mybuf._Size] = _Elem{0};
            }
        }

        template <class _Fn>
        void _Reallocate(const size_type _New_size, _Fn&& _Func) {
            size_type _New_capacity = _New_size;
            pointer _New_ptr        = _Allocate_new_buffer(_New_capacity); // may raise an exception
            bstk::_Deallocate_object_array_guard _Guard{_New_ptr, _New_capacity + 1, *_Mybuf._Al};
            _Func(_New_ptr);
            _Mybuf._Deallocate_large_buffer();
            _Guard._Release();
            _Mybuf._Capacity            = _New_capacity;
            _Mybuf._Size                = _New_size;
            _Mybuf._Large               = _New_ptr;
            _Mybuf._Large[_Mybuf._Size] = _Elem{0};
        }

        _String_buffer _Mybuf;
    };

    using byte_string    = string<byte_t>;
    using utf8_string    = string<char>;
    using unicode_string = string<wchar_t>;

    template <class _Elem>
    bool operator==(const string<_Elem>& _Left, const string<_Elem>& _Right) noexcept {
        return _Left.compare(_Right) == 0;
    }

    template <class _Elem>
    bool operator==(const string<_Elem>& _Left, const _Elem* const _Right) noexcept {
        return _Left.compare(_Right) == 0;
    }

    template <class _Elem>
    ::std::strong_ordering operator<=>(const string<_Elem>& _Left, const string<_Elem>& _Right) noexcept {
        return _Left.compare(_Right) <=> 0;
    }

    template <class _Elem>
    ::std::strong_ordering operator<=>(const string<_Elem>& _Left, const _Elem* const _Right) noexcept {
        return _Left.compare(_Right) <=> 0;
    }

    template <class _Elem>
    string<_Elem> operator+(const string<_Elem>& _Left, const string<_Elem>& _Right) {
        string<_Elem> _Str(_Left);
        return _Str.append(_Right);
    }

    template <class _Elem>
    string<_Elem> operator+(const string<_Elem>& _Left, const _Elem* const _Right) {
        string<_Elem> _Str(_Left);
        return _Str.append(_Right);
    }

    template <class _Elem>
    string<_Elem> operator+(const string<_Elem>& _Left, const _Elem _Right) {
        string<_Elem> _Str(_Left);
        _Str.push_back(_Right);
        return _Str;
    }

    template <class _Elem>
    string<_Elem> operator+(const string<_Elem>& _Left, const string_view<_Elem> _Right) {
        string<_Elem> _Str(_Left);
        return _Str.append(_Right);
    }

    template <class _Elem>
    string<_Elem> operator+(const _Elem* const _Left, const string<_Elem>& _Right) {
        string<_Elem> _Str(_Right);
        return _Str.append(_Left);
    }

    template <class _Elem>
    string<_Elem> operator+(const _Elem _Left, const string<_Elem>& _Right) {
        string<_Elem> _Str(_Right);
        return _Str.insert(0, 1, _Left);
    }

    template <class _Elem>
    string<_Elem> operator+(const string_view<_Elem> _Left, const string<_Elem>& _Right) {
        string<_Elem> _Str(_Left);
        return _Str.append(_Right);
    }

    template <class _Elem>
    string<_Elem> operator+(string<_Elem>&& _Left, string<_Elem>&& _Right) {
        return ::std::move(_Left.append(::std::move(_Right)));
    }

    template <class _Elem>
    string<_Elem> operator+(string<_Elem>&& _Left, const string<_Elem>& _Right) {
        return ::std::move(_Left.append(_Right));
    }

    template <class _Elem>
    string<_Elem> operator+(string<_Elem>&& _Left, const _Elem* const _Right) {
        return ::std::move(_Left.append(_Right));
    }

    template <class _Elem>
    string<_Elem> operator+(string<_Elem>&& _Left, const _Elem _Right) {
        _Left.push_back(_Right);
        return ::std::move(_Left);
    }

    template <class _Elem>
    string<_Elem> operator+(string<_Elem>&& _Left, const string_view<_Elem> _Right) {
        return ::std::move(_Left.append(_Right));
    }

    template <class _Elem>
    string<_Elem> operator+(const string<_Elem>& _Left, string<_Elem>&& _Right) {
        return ::std::move(_Right.append(_Left));
    }

    template <class _Elem>
    string<_Elem> operator+(const _Elem* const _Left, string<_Elem>&& _Right) {
        return ::std::move(_Right.append(_Left));
    }

    template <class _Elem>
    string<_Elem> operator+(const _Elem _Left, string<_Elem>&& _Right) {
        _Right.push_back(_Left);
        return ::std::move(_Right);
    }

    template <class _Elem>
    string<_Elem> operator+(const string_view<_Elem> _Left, string<_Elem>&& _Right) {
        return ::std::move(_Right.append(_Left));
    }
} // namespace bs

#endif // _BSTK_TEXT_STRING_HPP_