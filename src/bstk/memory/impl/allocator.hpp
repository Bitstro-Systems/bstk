// allocator.hpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#pragma once
#ifndef _BSTK_MEMORY_IMPL_ALLOCATOR_HPP_
#define _BSTK_MEMORY_IMPL_ALLOCATOR_HPP_
#include <atomic>
#include <bstk/memory/allocator.hpp>
#include <bstk/memory/system_allocator.hpp>

namespace bs {
    namespace bstk {
        class _Runtime_allocator { // wraps default or user-supplied allocator
        public:
            _Runtime_allocator() noexcept : _Mydef(), _Mycur(&_Mydef) {}

            ~_Runtime_allocator() noexcept = default;

            _Runtime_allocator(const _Runtime_allocator&)            = delete;
            _Runtime_allocator& operator=(const _Runtime_allocator&) = delete;

            allocator& _Get_wrapped() const noexcept {
                return *_Mycur.load(::std::memory_order_acquire);
            }

            void _Set_wrapped(allocator& _Al) noexcept {
                _Mycur.store(&_Al, ::std::memory_order_release);
            }

        private:
            system_allocator _Mydef; // default allocator that has no state or associated resource
            ::std::atomic<allocator*> _Mycur; // pointer to the currently used allocator
        };

        inline _Runtime_allocator& _Get_runtime_allocator() noexcept {
            static _Runtime_allocator _Al;
            return _Al;
        }
    } // namespace bstk
} // namespace bs

#endif // _BSTK_MEMORY_IMPL_ALLOCATOR_HPP_