// smart_pointer.hpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#pragma once
#ifndef _BSTK_MEMORY_SMART_POINTER_HPP_
#define _BSTK_MEMORY_SMART_POINTER_HPP_
#include <bstk/core/compressed_pair.hpp>
#include <bstk/memory/object.hpp>
#include <compare>
#include <type_traits>
#include <utility>

namespace bs {
    namespace bstk {
        template <class _Ty>
        concept _Smart_ptr_element = !::std::is_array_v<_Ty> && requires {
            static_cast<_Ty*>(nullptr); // must be a valid pointer type (_Ty*)
            sizeof(_Ty); // must be a complete type (sizeof(incomplete-type) is ill-formed)
        };
    } // namespace bstk

    template <class _Ty>
    struct default_delete { // default smart pointer deleter
        default_delete() noexcept  = default;
        ~default_delete() noexcept = default;

        template <class _Other>
        default_delete(const default_delete<_Other>&) noexcept {}

        void operator()(_Ty* const _Ptr) const {
            ::bs::delete_object(_Ptr);
        }

        void operator()(_Ty* const _Array, const size_t _Count) const {
            ::bs::delete_object_array(_Array, _Count);
        }
    };

    template <class _Ty, class _Deleter = default_delete<_Ty>>
    class unique_ptr { // smart pointer with unique object ownership semantics
    public:
        static_assert(bstk::_Smart_ptr_element<_Ty>, "invalid element type for unique_ptr<_Ty>");

        using element_type = _Ty;
        using pointer      = _Ty*;
        using deleter_type = _Deleter;

        unique_ptr() noexcept : _Mypair() {}

        unique_ptr(::std::nullptr_t) noexcept : _Mypair() {}

        explicit unique_ptr(pointer _Ptr) noexcept : _Mypair(_Ptr, _Deleter{}) {}

        unique_ptr(pointer _Ptr, const _Deleter& _Del) noexcept : _Mypair(_Ptr, _Del) {}

        unique_ptr(pointer _Ptr, _Deleter&& _Del) noexcept : _Mypair(_Ptr, ::std::move(_Del)) {}

        unique_ptr(unique_ptr&& _Other) noexcept
            : _Mypair(_Other.release(), ::std::move(_Other._Mypair.second())) {}

        ~unique_ptr() noexcept {
            pointer _Ptr = _Mypair.first();
            if (_Ptr) {
                _Mypair.second()(_Ptr);
            }
        }

        unique_ptr& operator=(unique_ptr&& _Other) noexcept {
            reset(_Other.release());
            _Mypair.second() = ::std::move(_Other._Mypair.second());
            return *this;
        }

        unique_ptr& operator=(::std::nullptr_t) noexcept {
            reset();
            return *this;
        }

        unique_ptr(const unique_ptr&)            = delete;
        unique_ptr& operator=(const unique_ptr&) = delete;

        explicit operator bool() const noexcept {
            return _Mypair.first() != nullptr;
        }

        element_type& operator*() const noexcept {
            // the behavior is undefined if the stored pointer is null
            return *_Mypair.first();
        }

        pointer operator->() const noexcept {
            return _Mypair.first();
        }

        pointer get() const noexcept {
            return _Mypair.first();
        }

        deleter_type& get_deleter() noexcept {
            return _Mypair.second();
        }

        const deleter_type& get_deleter() const noexcept {
            return _Mypair.second();
        }

        [[nodiscard]] pointer release() noexcept {
            pointer _Ptr    = _Mypair.first();
            _Mypair.first() = nullptr;
            return _Ptr;
        }

        void reset(pointer _New_ptr = nullptr) {
            pointer _Ptr    = _Mypair.first();
            _Mypair.first() = _New_ptr;
            if (_Ptr) {
                _Mypair.second()(_Ptr);
            }
        }

        void swap(unique_ptr& _Other) noexcept {
            _Mypair.swap(_Other._Mypair);
        }

    private:
        compressed_pair<pointer, _Deleter> _Mypair;
    };

    template <class _Ty>
    bool operator==(const unique_ptr<_Ty>& _Left, const unique_ptr<_Ty>& _Right) noexcept {
        return _Left.get() == _Right.get();
    }

    template <class _Ty>
    bool operator==(const unique_ptr<_Ty>& _Left, ::std::nullptr_t) noexcept {
        return _Left.get() == nullptr;
    }

    template <class _Ty>
    ::std::strong_ordering operator<=>(
        const unique_ptr<_Ty>& _Left, const unique_ptr<_Ty>& _Right) noexcept {
        return _Left.get() <=> _Right.get();
    }

    template <class _Ty, class... _Types>
    unique_ptr<_Ty> make_unique(_Types&&... _Args) {
        // create a unique pointer that manages a newly created object
        return unique_ptr<_Ty>(::bs::create_object<_Ty>(::std::forward<_Types>(_Args)...));
    }

    template <class _Ty>
    unique_ptr<_Ty> make_unique_for_overwrite() {
        // create a unique pointer that manages a newly default-initialized object
        return unique_ptr<_Ty>(::bs::create_object<_Ty>());
    }
} // namespace bs

#endif // _BSTK_MEMORY_SMART_POINTER_HPP_