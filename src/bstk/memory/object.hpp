// object.hpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#pragma once
#ifndef _BSTK_MEMORY_OBJECT_HPP_
#define _BSTK_MEMORY_OBJECT_HPP_
#include <bstk/memory/allocator.hpp>
#include <new>
#include <type_traits>
#include <utility>

namespace bs {
    template <class _Ty, class... _Types>
    _Ty* construct_object(_Ty* const _Location, _Types&&... _Args)
        noexcept(::std::is_nothrow_constructible_v<_Ty, _Types...>) {
        // constructs an object in-place at the given memory location
        return ::new (static_cast<void*>(_Location)) _Ty(::std::forward<_Types>(_Args)...);
    }

    template <class _Ty>
    void destroy_object(_Ty* const _Obj) noexcept(::std::is_nothrow_destructible_v<_Ty>) {
        // destroys the object (if it has a non-trivial destructor)
        if constexpr (!::std::is_trivially_destructible_v<_Ty>) {
            if (_Obj) {
                _Obj->~_Ty(); // non-trivial destructor, call it
            }
        }
    }

    template <class _Ty, any_allocator _Alloc>
    [[nodiscard]] _Ty* allocate_object_using_allocator(_Alloc& _Al) {
        // allocates memory for an object using the given allocator
        return static_cast<_Ty*>(_Al.allocate(sizeof(_Ty)));
    }

    template <class _Ty, any_allocator _Alloc>
    void deallocate_object_using_allocator(_Ty* const _Obj, _Alloc& _Al) {
        // deallocates the object's memory using the given allocator
        _Al.deallocate(_Obj, sizeof(_Ty));
    }

    template <class _Ty, any_allocator _Alloc>
    [[nodiscard]] _Ty* allocate_object_array_using_allocator(const size_t _Count, _Alloc& _Al) {
        // allocates memory for an array of objects using the given allocator
        return static_cast<_Ty*>(_Al.allocate(_Count * sizeof(_Ty)));
    }

    template <class _Ty, any_allocator _Alloc>
    void deallocate_object_array_using_allocator(_Ty* const _Array, const size_t _Count, _Alloc& _Al) {
        // deallocates the array's memory using the given allocator
        _Al.deallocate(_Array, _Count * sizeof(_Ty));
    }

    namespace bstk {
        template <class _Ty, class _Alloc>
        struct _Deallocate_object_guard { // deallocates an object on scope exit
            _Ty* _Obj;
            _Alloc& _Al;

            ~_Deallocate_object_guard() {
                if (_Obj) {
                    ::bs::deallocate_object_using_allocator(_Obj, _Al);
                }
            }

            void _Release() noexcept {
                _Obj = nullptr;
            }
        };
    } // namespace bstk

    template <class _Ty, any_allocator _Alloc, class... _Types>
    [[nodiscard]] _Ty* create_object_using_allocator(_Alloc& _Al, _Types&&... _Args) {
        // allocates memory for an object using the given allocator, then constructs the object in-place
        _Ty* const _Obj = ::bs::allocate_object_using_allocator<_Ty>(_Al);
        bstk::_Deallocate_object_guard _Guard{_Obj, _Al};
        ::bs::construct_object(_Obj, ::std::forward<_Types>(_Args)...);
        _Guard._Release();
        return _Obj;
    }

    template <class _Ty, any_allocator _Alloc>
    void delete_object_using_allocator(_Ty* const _Obj, _Alloc& _Al) {
        // destroys the object (if it has a non-trivial destructor)
        // and deallocates its memory using the given allocator
        if (_Obj) {
            bstk::_Deallocate_object_guard _Guard{_Obj, _Al};
            ::bs::destroy_object(_Obj);
        }
    }

    template <class _Ty>
    [[nodiscard]] _Ty* allocate_object() {
        // allocates memory for an object using the runtime allocator
        return ::bs::allocate_object_using_allocator<_Ty>(::bs::get_allocator());
    }

    template <class _Ty>
    void deallocate_object(_Ty* const _Obj) {
        // deallocates the object's memory using the runtime allocator
        ::bs::deallocate_object_using_allocator(_Obj, ::bs::get_allocator());
    }

    template <class _Ty>
    [[nodiscard]] _Ty* allocate_object_array(const size_t _Count) {
        // allocates memory for an array of objects using the runtime allocator
        return ::bs::allocate_object_array_using_allocator<_Ty>(_Count, ::bs::get_allocator());
    }

    template <class _Ty>
    void deallocate_object_array(_Ty* const _Array, const size_t _Count) {
        // deallocates the array's memory using the runtime allocator
        ::bs::deallocate_object_array_using_allocator(_Array, _Count, ::bs::get_allocator());
    }

    template <class _Ty, class... _Types>
    [[nodiscard]] _Ty* create_object(_Types&&... _Args) {
        // allocates memory for an object using the runtime allocator, then constructs the object in-place
        return ::bs::create_object_using_allocator<_Ty>(
            ::bs::get_allocator(), ::std::forward<_Types>(_Args)...);
    }

    template <class _Ty>
    void delete_object(_Ty* const _Obj) {
        // destroys the object (if it has a non-trivial destructor)
        // and deallocates its memory using the runtime allocator
        ::bs::delete_object_using_allocator(_Obj, ::bs::get_allocator());
    }
} // namespace bs

#endif // _BSTK_MEMORY_OBJECT_HPP_