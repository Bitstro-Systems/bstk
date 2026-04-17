// compressed_pair.hpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#pragma once
#ifndef _BSTK_CORE_COMPRESSED_PAIR_HPP_
#define _BSTK_CORE_COMPRESSED_PAIR_HPP_
#include <functional>
#include <type_traits>
#include <utility>

namespace bs {
    namespace bstk {
        template <class _Ty>
        inline constexpr bool _Is_compressible = ::std::is_empty_v<_Ty> && !::std::is_final_v<_Ty>;

        template <class _Ty1, class _Ty2>
        struct _Compressed_pair_traits { // common traits for a pair of types
            template <template <class> class _Trait>
            static constexpr bool _Is_common_trait = ::std::conjunction_v<_Trait<_Ty1>, _Trait<_Ty2>>;

            template <class _Ty>
            using _Is_nothrow_comparable = ::std::is_nothrow_invocable<::std::equal_to<_Ty>>;

            static constexpr bool _Nothrow_default_constructible =
                _Is_common_trait<::std::is_nothrow_default_constructible>;

            static constexpr bool _Nothrow_destructible =
                _Is_common_trait<::std::is_nothrow_destructible>;

            static constexpr bool _Nothrow_copy_constructible =
                _Is_common_trait<::std::is_nothrow_copy_constructible>;

            static constexpr bool _Nothrow_move_constructible =
                _Is_common_trait<::std::is_nothrow_move_constructible>;

            static constexpr bool _Nothrow_copy_assignable =
                _Is_common_trait<::std::is_nothrow_copy_assignable>;

            static constexpr bool _Nothrow_move_assignable =
                _Is_common_trait<::std::is_nothrow_move_assignable>;
            
            static constexpr bool _Nothrow_swappable  = _Is_common_trait<::std::is_nothrow_swappable>;
            static constexpr bool _Nothrow_comparable = _Is_common_trait<_Is_nothrow_comparable>;
        };

        template <class _Ty1, class _Ty2, bool _Compressible = _Is_compressible<_Ty2>>
        class _Compressed_pair : public _Ty2 { // pair of types stored in shared space
        public:
            using _Traits = _Compressed_pair_traits<_Ty1, _Ty2>;

            static constexpr bool _Compressed = _Compressible;

            constexpr _Compressed_pair() noexcept(_Traits::_Nothrow_default_constructible) = default;
            constexpr ~_Compressed_pair() noexcept(_Traits::_Nothrow_destructible)         = default;

            constexpr _Compressed_pair(const _Ty1& _First, const _Ty2& _Second) noexcept(
                _Traits::_Nothrow_copy_constructible) : _First(_First), _Ty2(_Second) {}

            constexpr _Compressed_pair(_Ty1&& _First, _Ty2&& _Second)
                noexcept(_Traits::_Nothrow_move_constructible)
                : _First(::std::move(_First)), _Ty2(::std::move(_Second)) {}

            _Ty1 _First{};
        };

        template <class _Ty1, class _Ty2>
        class _Compressed_pair<_Ty1, _Ty2, false> { // pair of types with independent storage
        public:
            using _Traits = _Compressed_pair_traits<_Ty1, _Ty2>;

            static constexpr bool _Compressed = false;

            constexpr _Compressed_pair() noexcept(_Traits::_Nothrow_default_constructible) = default;
            constexpr ~_Compressed_pair() noexcept(_Traits::_Nothrow_destructible)         = default;

            constexpr _Compressed_pair(const _Ty1& _First, const _Ty2& _Second) noexcept(
                _Traits::_Nothrow_copy_constructible) : _First(_First), _Second(_Second) {}

            constexpr _Compressed_pair(_Ty1&& _First, _Ty2&& _Second)
                noexcept(_Traits::_Nothrow_move_constructible)
                : _First(::std::move(_First)), _Second(::std::move(_Second)) {}

            _Ty1 _First{};
            _Ty2 _Second{};
        };
    } // namespace bstk

    template <class _Ty1, class _Ty2>
    class compressed_pair final : private bstk::_Compressed_pair<_Ty1, _Ty2> {
    private:
        using _Base   = bstk::_Compressed_pair<_Ty1, _Ty2>;
        using _Traits = bstk::_Compressed_pair_traits<_Ty1, _Ty2>;

    public:
        using first_type  = _Ty1;
        using second_type = _Ty2;

        constexpr compressed_pair() noexcept(_Traits::_Nothrow_default_constructible) = default;
        constexpr ~compressed_pair() noexcept(_Traits::_Nothrow_destructible)         = default;

        constexpr compressed_pair(const compressed_pair& _Other) noexcept(
            _Traits::_Nothrow_copy_constructible) : _Base(_Other._First, _Other.second()) {}

        constexpr compressed_pair(compressed_pair&& _Other) noexcept(_Traits::_Nothrow_move_constructible)
            : _Base(::std::move(_Other._First), ::std::move(_Other.second())) {}

        constexpr compressed_pair(const first_type& _First, const second_type& _Second)
            noexcept(_Traits::_Nothrow_copy_constructible) : _Base(_First, _Second) {}

        constexpr compressed_pair(first_type&& _First, second_type&& _Second) noexcept(
            _Traits::_Nothrow_move_constructible) : _Base(::std::move(_First), ::std::move(_Second)) {}

        constexpr compressed_pair& operator=(
            const compressed_pair& _Other) noexcept(_Traits::_Nothrow_copy_assignable) {
            if (this != &_Other) {
                this->_First = _Other._First;
                second()     = _Other.second();
            }

            return *this;
        }

        constexpr compressed_pair& operator=(
            compressed_pair&& _Other) noexcept(_Traits::_Nothrow_move_assignable) {
            if (this != &_Other) {
                this->_First = ::std::move(_Other._First);
                second()     = ::std::move(_Other.second());
            }

            return *this;
        }

        constexpr first_type& first() noexcept {
            return this->_First;
        }

        constexpr const first_type& first() const noexcept {
            return this->_First;
        }

        constexpr second_type& second() noexcept {
            if constexpr (_Base::_Compressed) {
                return *this;
            } else {
                return this->_Second;
            }
        }

        constexpr const second_type& second() const noexcept {
            if constexpr (_Base::_Compressed) {
                return *this;
            } else {
                return this->_Second;
            }
        }

        constexpr void swap(compressed_pair& _Other) noexcept(_Traits::_Nothrow_swappable) {
            ::std::swap(this->_First, _Other._First);
            if constexpr (_Base::_Compressed) {
                ::std::swap(static_cast<second_type&>(*this), static_cast<second_type&>(_Other));
            } else {
                ::std::swap(this->_Second, _Other._Second);
            }
        }
    };

    template <class _Ty1, class _Ty2>
    constexpr bool operator==(const compressed_pair<_Ty1, _Ty2>& _Left,
        const compressed_pair<_Ty1, _Ty2>& _Right) noexcept(
            bstk::_Compressed_pair_traits<_Ty1, _Ty2>::_Nothrow_comparable) {
        return _Left.first() == _Right.first() && _Left.second() == _Right.second();
    }
} // namespace bs

#endif // _BSTK_CORE_COMPRESSED_PAIR_HPP_