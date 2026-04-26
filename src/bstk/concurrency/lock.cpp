// lock.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/concurrency/impl/shared_lock.hpp>
#include <bstk/concurrency/lock.hpp>
#include <bstk/memory/object.hpp>

namespace bs {
    shared_lock::shared_lock() : _Myimpl(::bs::create_object<bstk::_Shared_lock_impl>()) {}

    shared_lock::~shared_lock() {
        if (_Myimpl) {
            ::bs::delete_object(_Myimpl);
            _Myimpl = nullptr;
        }
    }

    void shared_lock::lock() noexcept {
        if (_Myimpl) {
            _Myimpl->_Lock();
        }
    }

    void shared_lock::lock_shared() noexcept {
        if (_Myimpl) {
            _Myimpl->_Lock_shared();
        }
    }

    void shared_lock::unlock() noexcept {
        if (_Myimpl) {
            _Myimpl->_Unlock();
        }
    }

    void shared_lock::unlock_shared() noexcept {
        if (_Myimpl) {
            _Myimpl->_Unlock_shared();
        }
    }
} // namespace bs