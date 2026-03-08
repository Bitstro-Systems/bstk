// object_allocator.hpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#pragma once
#ifndef _BSTK_MEMORY_OBJECT_ALLOCATOR_HPP_
#define _BSTK_MEMORY_OBJECT_ALLOCATOR_HPP_
#include <bstk/memory/allocator.hpp>
#include <type_traits>

namespace bs {
    template <class _Ty>
    class object_allocator { // type-specific wrapper around the global allocator
    public:
        static_assert(::std::negation_v<::std::disjunction<::std::is_same<_Ty, void>,
            ::std::is_const<_Ty>, ::std::is_reference<_Ty>, ::std::is_function<_Ty>,
            ::std::is_volatile<_Ty>>>, "invalid value type");

        using value_type      = _Ty;
        using size_type       = allocator::size_type;
        using difference_type = allocator::difference_type;
        using pointer         = _Ty*;
        using const_pointer   = const _Ty*;
        using reference       = _Ty&;
        using const_reference = const _Ty&;

        template <class _Other>
        struct rebind {
            using other = object_allocator<_Other>;
        };

        // the least alignment required for proper allocation
        static constexpr size_type required_alignment = alignof(_Ty) > __STDCPP_DEFAULT_NEW_ALIGNMENT__
            ? alignof(_Ty) : __STDCPP_DEFAULT_NEW_ALIGNMENT__;

        object_allocator() noexcept                        = default;
        object_allocator(const object_allocator&) noexcept = default;
        object_allocator(object_allocator&&) noexcept      = default;
        ~object_allocator() noexcept                       = default;

        template <class _Other>
        object_allocator(const object_allocator<_Other>&) noexcept {}

        object_allocator& operator=(const object_allocator&) noexcept = default;
        object_allocator& operator=(object_allocator&&) noexcept      = default;

        template <class _Other>
        object_allocator& operator=(const object_allocator<_Other>&) noexcept {
            return *this;
        }

        allocator_id id() const noexcept {
            return ::bs::get_allocator().id();
        }

        size_type max_size() const noexcept {
            return ::bs::get_allocator().max_size() / sizeof(_Ty);
        }

        bool is_equal(const allocator& _Other) const noexcept {
            return ::bs::get_allocator().is_equal(_Other);
        }

        pointer allocate(const size_type _Count, const size_type _Align = 0) {
            return static_cast<pointer>(
                ::bs::get_allocator().allocate(_Count * sizeof(_Ty), _Choose_align(_Align)));
        }

        void deallocate(pointer _Ptr, const size_type _Count, const size_type _Align = 0) {
            ::bs::get_allocator().deallocate(_Ptr, _Count * sizeof(_Ty), _Choose_align(_Align));
        }

    private:
        static constexpr size_type _Choose_align(const size_type _Align) noexcept {
            // choose between the required and specified alignment for allocation
            return _Align > required_alignment ? _Align : required_alignment;
        }
    };

    template <class _Ty1, class _Ty2>
    constexpr bool operator==(const object_allocator<_Ty1>&, const object_allocator<_Ty2>&) noexcept {
        return true; // always equal
    }
} // namespace bs

#endif // _BSTK_MEMORY_OBJECT_ALLOCATOR_HPP_