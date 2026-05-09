// test.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/memory/smart_pointer.hpp>
#include <gtest/gtest.h>

namespace bs {
    struct _Aggregate_type {
        static constexpr bool _Default_flag  = false;
        static constexpr char _Default_sign  = 'z';
        static constexpr int _Default_number = 92;

        bool _Flag  = _Default_flag;
        char _Sign  = _Default_sign;
        int _Number = _Default_number;
    };

    template <class _Ty>
    struct _Named_deleter { // deleter that is identified by a name
        const char* _Name = "";

        void operator()(_Ty* const _Array, const size_t _Size) const {
            ::bs::delete_object_array(_Array, _Size);
        }
    };

    TEST(shared_array, default_construct) {
        const shared_array<int> _Shared;
        EXPECT_EQ(_Shared.get(), nullptr);
        EXPECT_EQ(_Shared.size(), 0);
        EXPECT_EQ(_Shared.use_count(), 0);
    }

    TEST(shared_array, copy_construct) {
        const auto _Shared0 = ::bs::make_shared_array<int>(16);
        const auto _Shared1 = _Shared0;
        EXPECT_EQ(_Shared0.get(), _Shared1.get());
        EXPECT_EQ(_Shared0.size(), _Shared1.size());
        EXPECT_EQ(_Shared0.use_count(), 2);
        EXPECT_EQ(_Shared1.use_count(), 2);
    }

    TEST(shared_array, move_construct) {
        constexpr size_t _Size = 16;
        int* const _Ptr        = ::bs::create_object_array<int>(_Size);
        shared_array<int> _Shared0({_Ptr, _Size}); // destroys _Ptr
        const auto _Shared1 = ::std::move(_Shared0);
        EXPECT_EQ(_Shared0.get(), nullptr);
        EXPECT_EQ(_Shared0.size(), 0);
        EXPECT_EQ(_Shared0.use_count(), 0);
        EXPECT_EQ(_Shared1.get(), _Ptr);
        EXPECT_EQ(_Shared1.size(), _Size);
        EXPECT_EQ(_Shared1.use_count(), 1);
    }

    TEST(shared_array, array_construct) {
        constexpr size_t _Size = 16;
        int* const _Ptr        = ::bs::create_object_array<int>(_Size);
        const shared_array<int> _Shared({_Ptr, _Size}); // destroys _Ptr
        EXPECT_EQ(_Shared.get(), _Ptr);
        EXPECT_EQ(_Shared.size(), _Size);
    }

    TEST(shared_array, unique_ptr_construct) {
        constexpr size_t _Size = 16;
        int* const _Ptr        = ::bs::create_object_array<int>(_Size);
        unique_array<int> _Unique({_Ptr, _Size}); // destroys _Ptr
        const shared_array<int> _Shared(::std::move(_Unique));
        EXPECT_EQ(_Unique.get(), nullptr);
        EXPECT_EQ(_Unique.size(), 0);
        EXPECT_EQ(_Shared.get(), _Ptr);
        EXPECT_EQ(_Shared.size(), _Size);
        EXPECT_EQ(_Shared.use_count(), 1);
    }

    TEST(shared_array, copy_assign) {
        const auto _Shared0 = ::bs::make_shared_array<int>(16);
        shared_array<int> _Shared1;
        _Shared1 = _Shared0;
        EXPECT_EQ(_Shared0.get(), _Shared1.get());
        EXPECT_EQ(_Shared0.size(), _Shared1.size());
        EXPECT_EQ(_Shared0.use_count(), 2);
        EXPECT_EQ(_Shared1.use_count(), 2);
    }

    TEST(shared_array, move_assign) {
        constexpr size_t _Size = 16;
        int* const _Ptr        = ::bs::create_object_array<int>(_Size);
        shared_array<int> _Shared0({_Ptr, _Size}); // destroys _Ptr
        shared_array<int> _Shared1;
        _Shared1 = ::std::move(_Shared0);
        EXPECT_EQ(_Shared0.get(), nullptr);
        EXPECT_EQ(_Shared0.size(), 0);
        EXPECT_EQ(_Shared0.use_count(), 0);
        EXPECT_EQ(_Shared1.get(), _Ptr);
        EXPECT_EQ(_Shared1.size(), _Size);
        EXPECT_EQ(_Shared1.use_count(), 1);
    }

    TEST(shared_array, unique_ptr_assign) {
        constexpr size_t _Size = 16;
        int* const _Ptr        = ::bs::create_object_array<int>(_Size);
        unique_array<int> _Unique({_Ptr, _Size}); // destroys _Ptr
        shared_array<int> _Shared;
        _Shared = ::std::move(_Unique);
        EXPECT_EQ(_Unique.get(), nullptr);
        EXPECT_EQ(_Unique.size(), 0);
        EXPECT_EQ(_Shared.get(), _Ptr);
        EXPECT_EQ(_Shared.size(), _Size);
        EXPECT_EQ(_Shared.use_count(), 1);
    }

    TEST(shared_array, subscript_valid) {
        constexpr size_t _Size = 16;
        constexpr size_t _Idx  = 13;
        constexpr int _Value   = 362;
        int* const _Ptr        = ::bs::create_object_array<int>(_Size);
        _Ptr[_Idx]             = _Value;
        const shared_array<int> _Shared({_Ptr, _Size}); // destroys _Ptr
        EXPECT_EQ(_Shared[_Idx], _Value);
    }

    TEST(shared_array, subscript_invalid) {
        constexpr size_t _Size = 16;
        const auto _Shared     = ::bs::make_shared_array<int>(_Size);
        EXPECT_THROW(_Shared[_Size + 1], resource_overrun);
    }

    TEST(shared_array, get) {
        constexpr size_t _Size = 16;
        int* const _Ptr        = ::bs::create_object_array<int>(_Size);
        const shared_array<int> _Shared({_Ptr, _Size}); // destroys _Ptr
        EXPECT_EQ(_Shared.get(), _Ptr);
    }

    TEST(shared_array, size) {
        constexpr size_t _Size = 16;
        const auto _Shared     = ::bs::make_shared_array<int>(_Size);
        EXPECT_EQ(_Shared.size(), _Size);
    }

    TEST(shared_array, get_deleter) {
        using _Deleter_type    = _Named_deleter<int>;
        constexpr char _Name[] = "test-deleter";
        constexpr size_t _Size = 16;
        const shared_array<int, _Deleter_type> _Shared(
            {::bs::create_object_array<int>(_Size), _Size}, _Deleter_type{_Name});
        EXPECT_STREQ(_Shared.get_deleter()._Name, _Name);
    }

    TEST(shared_array, use_count) {
        const auto _Shared0 = ::bs::make_shared_array<int>(16);
        EXPECT_EQ(_Shared0.use_count(), 1);
        {
            const auto _Shared1 = _Shared0;
            const auto _Shared2 = _Shared0;
            EXPECT_EQ(_Shared0.use_count(), 3);
        }

        EXPECT_EQ(_Shared0.use_count(), 1);
    }

    TEST(shared_array, reset) {
        size_t _Size = 16;
        auto _Shared = ::bs::make_shared_array<int>(_Size);
        EXPECT_NE(_Shared.get(), nullptr);
        EXPECT_EQ(_Shared.size(), _Size);
        EXPECT_EQ(_Shared.use_count(), 1);

        _Shared.reset();
        EXPECT_EQ(_Shared.get(), nullptr);
        EXPECT_EQ(_Shared.size(), 0);
        EXPECT_EQ(_Shared.use_count(), 0);

        _Size           = 32;
        int* const _Ptr = ::bs::create_object_array<int>(_Size);
        _Shared.reset({_Ptr, _Size}); // destroys _Ptr
        EXPECT_EQ(_Shared.get(), _Ptr);
        EXPECT_EQ(_Shared.size(), _Size);
        EXPECT_EQ(_Shared.use_count(), 1);
    }

    TEST(shared_array, swap) {
        constexpr size_t _Size0 = 16;
        constexpr size_t _Size1 = 32;
        int* const _Ptr0        = ::bs::create_object_array<int>(_Size0);
        int* const _Ptr1        = ::bs::create_object_array<int>(_Size1);
        shared_array<int> _Shared0({_Ptr0, _Size0}); // destroys _Ptr0
        shared_array<int> _Shared1({_Ptr1, _Size1}); // destroys _Ptr1
        _Shared0.swap(_Shared1);
        EXPECT_EQ(_Shared0.get(), _Ptr1);
        EXPECT_EQ(_Shared0.size(), _Size1);
        EXPECT_EQ(_Shared1.get(), _Ptr0);
        EXPECT_EQ(_Shared1.size(), _Size0);
    }

    TEST(shared_array, compare_shared_array) {
        const auto _Shared0 = ::bs::make_shared_array<int>(16);
        auto _Shared1       = _Shared0;
        EXPECT_EQ(_Shared0, _Shared1);

        _Shared1.reset();
        EXPECT_NE(_Shared0, _Shared1);
    }

    TEST(shared_array, compare_null) {
        auto _Shared = ::bs::make_shared_array<int>(16);
        EXPECT_NE(_Shared, nullptr);

        _Shared.reset();
        EXPECT_EQ(_Shared, nullptr);
    }

    TEST(shared_array, make_shared_array) {
        constexpr size_t _Size = 4;
        const auto _Shared     = ::bs::make_shared_array<_Aggregate_type>(_Size);
        for (size_t _Idx = 0; _Idx < _Size; ++_Idx) {
            const auto& _Elem = _Shared[_Idx];
            EXPECT_EQ(_Elem._Flag, _Aggregate_type::_Default_flag);
            EXPECT_EQ(_Elem._Sign, _Aggregate_type::_Default_sign);
            EXPECT_EQ(_Elem._Number, _Aggregate_type::_Default_number);
        }
    }
} // namespace bs