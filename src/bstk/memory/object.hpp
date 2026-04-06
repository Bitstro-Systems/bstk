// object.hpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#pragma once
#ifndef _BSTK_MEMORY_OBJECT_HPP_
#define _BSTK_MEMORY_OBJECT_HPP_
#include <bstk/memory/allocator.hpp>
#include <exception>
#include <new>
#include <type_traits>
#include <utility>

namespace bs {
    template <class _Ty, class... _Types>
    _Ty* construct_object(_Ty* const _Location, _Types&&... _Args)
        noexcept(::std::is_nothrow_constructible_v<_Ty, _Types...>) {
        // construct an object in-place at the given memory location
        return ::new (static_cast<void*>(_Location)) _Ty(::std::forward<_Types>(_Args)...);
    }

    template <class _Ty>
    void destroy_object(_Ty* const _Obj) noexcept(::std::is_nothrow_destructible_v<_Ty>) {
        // destroy the object (if it has a non-trivial destructor)
        if constexpr (!::std::is_trivially_destructible_v<_Ty>) {
            if (_Obj) {
                _Obj->~_Ty(); // non-trivial destructor, call it
            }
        }
    }

    template <class _Ty, any_allocator _Alloc>
    [[nodiscard]] _Ty* allocate_object_using_allocator(_Alloc& _Al) {
        // allocate memory for an object using the given allocator
        return static_cast<_Ty*>(_Al.allocate(sizeof(_Ty)));
    }

    template <class _Ty, any_allocator _Alloc>
    void deallocate_object_using_allocator(_Ty* const _Obj, _Alloc& _Al) {
        // deallocate the object's memory using the given allocator
        _Al.deallocate(_Obj, sizeof(_Ty));
    }

    template <class _Ty, any_allocator _Alloc>
    [[nodiscard]] _Ty* allocate_object_array_using_allocator(const size_t _Count, _Alloc& _Al) {
        // allocate memory for an array of objects using the given allocator
        return static_cast<_Ty*>(_Al.allocate(_Count * sizeof(_Ty)));
    }

    template <class _Ty, any_allocator _Alloc>
    void deallocate_object_array_using_allocator(_Ty* const _Array, const size_t _Count, _Alloc& _Al) {
        // deallocate the array's memory using the given allocator
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

        template <class _Ty, class _Alloc>
        struct _Deallocate_object_array_guard { // deallocates an array of objects on scope exit
            _Ty* _Array;
            size_t _Count;
            _Alloc& _Al;

            ~_Deallocate_object_array_guard() {
                if (_Array && _Count > 0) {
                    ::bs::deallocate_object_array_using_allocator(_Array, _Count, _Al);
                }
            }

            void _Release() noexcept {
                _Array = nullptr;
                _Count = 0;
            }
        };

        template <class _Ty>
        void _Destroy_object_array(_Ty* const _Array, size_t _Count) {
            while (_Count-- > 0) {
                try {
                    ::bs::destroy_object(_Array + _Count);
                } catch (...) {
                    // Note: Destructors are implicitly noexcept(true) by default. If such
                    //       a destructor throws, std::terminate() is invoked immediately
                    //       and this catch block is never reached. This catch block only
                    //       handles destructors explicitly declared noexcept(false). In that case,
                    //       we still treat throwing as a fatal error and call std::terminate().
                    ::std::terminate();
                }
            }
        }
    } // namespace bstk

    template <class _Ty, any_allocator _Alloc, class... _Types>
    [[nodiscard]] _Ty* create_object_using_allocator(_Alloc& _Al, _Types&&... _Args) {
        // allocate memory for an object using the given allocator, then construct the object in-place
        _Ty* const _Obj = ::bs::allocate_object_using_allocator<_Ty>(_Al);
        bstk::_Deallocate_object_guard _Guard{_Obj, _Al};
        ::bs::construct_object(_Obj, ::std::forward<_Types>(_Args)...);
        _Guard._Release();
        return _Obj;
    }

    template <class _Ty, any_allocator _Alloc>
    void delete_object_using_allocator(_Ty* const _Obj, _Alloc& _Al) {
        // destroy the object (if it has a non-trivial destructor)
        // and deallocate its memory using the given allocator
        if (_Obj) {
            bstk::_Deallocate_object_guard _Guard{_Obj, _Al};
            ::bs::destroy_object(_Obj);
        }
    }

    template <class _Ty, any_allocator _Alloc>
    [[nodiscard]] _Ty* create_object_array_using_allocator(const size_t _Count, _Alloc& _Al) {
        // allocate memory for an array of objects using the given allocator,
        // then construct the objects in-place
        if (_Count == 0) {
            return nullptr;
        }

        _Ty* const _Array = ::bs::allocate_object_array_using_allocator<_Ty>(_Count, _Al);
        bstk::_Deallocate_object_array_guard _Guard{_Array, _Count, _Al};
        size_t _Off = 0;
        try {
            for (; _Off < _Count; ++_Off) {
                ::bs::construct_object(_Array + _Off);
            }
        } catch (...) {
            bstk::_Destroy_object_array(_Array, _Off);
            
            // Note: _Destroy_object_array() calls std::terminate() if any destructor throws.
            //       If control returns, all constructed objects have been destroyed successfully.
            //       In that case, it is safe to rethrow the original exception from construction.
            throw;
        }

        _Guard._Release();
        return _Array;
    }

    template <class _Ty, any_allocator _Alloc>
    void delete_object_array_using_allocator(_Ty* const _Array, size_t _Count, _Alloc& _Al) {
        // destroy the object (if it has a non-trivial destructor)
        // and deallocate its memory using the given allocator
        if (_Array && _Count > 0) {
            bstk::_Deallocate_object_array_guard _Guard{_Array, _Count, _Al};
            bstk::_Destroy_object_array(_Array, _Count);
        }
    }

    template <class _Ty>
    [[nodiscard]] _Ty* allocate_object() {
        // allocate memory for an object using the runtime allocator
        return ::bs::allocate_object_using_allocator<_Ty>(::bs::get_allocator());
    }

    template <class _Ty>
    void deallocate_object(_Ty* const _Obj) {
        // deallocate the object's memory using the runtime allocator
        ::bs::deallocate_object_using_allocator(_Obj, ::bs::get_allocator());
    }

    template <class _Ty>
    [[nodiscard]] _Ty* allocate_object_array(const size_t _Count) {
        // allocate memory for an array of objects using the runtime allocator
        return ::bs::allocate_object_array_using_allocator<_Ty>(_Count, ::bs::get_allocator());
    }

    template <class _Ty>
    void deallocate_object_array(_Ty* const _Array, const size_t _Count) {
        // deallocate the array's memory using the runtime allocator
        ::bs::deallocate_object_array_using_allocator(_Array, _Count, ::bs::get_allocator());
    }

    template <class _Ty, class... _Types>
    [[nodiscard]] _Ty* create_object(_Types&&... _Args) {
        // allocate memory for an object using the runtime allocator, then construct the object in-place
        return ::bs::create_object_using_allocator<_Ty>(
            ::bs::get_allocator(), ::std::forward<_Types>(_Args)...);
    }

    template <class _Ty>
    void delete_object(_Ty* const _Obj) {
        // destroy the object (if it has a non-trivial destructor)
        // and deallocate its memory using the runtime allocator
        ::bs::delete_object_using_allocator(_Obj, ::bs::get_allocator());
    }

    template <class _Ty>
    [[nodiscard]] _Ty* create_object_array(const size_t _Count) {
        // allocate memory for an array of objects using the runtime allocator,
        // then construct the objects in-place
        return ::bs::create_object_array_using_allocator<_Ty>(_Count, ::bs::get_allocator());
    }

    template <class _Ty>
    void delete_object_array(_Ty* const _Array, const size_t _Count) {
        // destroy the object (if it has a non-trivial destructor)
        // and deallocate its memory using the runtime allocator
        ::bs::delete_object_array_using_allocator(_Array, _Count, ::bs::get_allocator());
    }
} // namespace bs

#endif // _BSTK_MEMORY_OBJECT_HPP_