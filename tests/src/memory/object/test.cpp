// test.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/memory/object.hpp>
#include <gtest/gtest.h>

namespace bs {
    enum class _Object_state : unsigned char {
        _Uninitialized,
        _Constructed,
        _Destroyed
    };

    struct _Stateful_object {
        _Object_state& _State_ref;

        explicit _Stateful_object(_Object_state& _State) noexcept : _State_ref(_State) {
            _State_ref = _Object_state::_Constructed;
        }

        ~_Stateful_object() noexcept {
            _State_ref = _Object_state::_Destroyed;
        }
    };

    struct _Aggregate_object {
        static constexpr bool _Default_flag  = false;
        static constexpr char _Default_sign  = 'z';
        static constexpr int _Default_number = 92;

        bool _Flag  = _Default_flag;
        char _Sign  = _Default_sign;
        int _Number = _Default_number;
    };

    TEST(object, construct_and_destruct_object) {
        _Object_state _State                           = _Object_state::_Uninitialized;
        unsigned char _Space[sizeof(_Stateful_object)] = {0};
        _Stateful_object* const _Obj                   = reinterpret_cast<_Stateful_object*>(_Space);
        ::bs::construct_object(_Obj, _State);
        EXPECT_EQ(_State, _Object_state::_Constructed);
        ::bs::destroy_object(_Obj);
        EXPECT_EQ(_State, _Object_state::_Destroyed);
    }

    TEST(object, create_and_delete_object) {
        constexpr bool _Flag          = true;
        constexpr char _Sign          = 'x';
        constexpr int _Number         = 10;
        _Aggregate_object* const _Obj = ::bs::create_object<_Aggregate_object>(_Flag, _Sign, _Number);
        EXPECT_EQ(_Obj->_Flag, _Flag);
        EXPECT_EQ(_Obj->_Sign, _Sign);
        EXPECT_EQ(_Obj->_Number, _Number);
        ::bs::delete_object(_Obj);
    }

    TEST(object, create_and_delete_object_array) {
        constexpr size_t _Count         = 16;
        _Aggregate_object* const _Array = ::bs::create_object_array<_Aggregate_object>(_Count);
        for (size_t _Idx = 0; _Idx < _Count; ++_Idx) {
            EXPECT_EQ(_Array[_Idx]._Flag, _Aggregate_object::_Default_flag);
            EXPECT_EQ(_Array[_Idx]._Sign, _Aggregate_object::_Default_sign);
            EXPECT_EQ(_Array[_Idx]._Number, _Aggregate_object::_Default_number);
        }

        ::bs::delete_object_array(_Array, _Count);
    }
} // namespace bs