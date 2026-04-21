// test.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/memory/smart_pointer.hpp>
#include <gtest/gtest.h>

namespace bs {
    struct _Incomplete_type;

    struct _Complete_type {
        int _Obj = 0;

        void _Func() const noexcept {}
    };

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

    TEST(unique_array, valid_element_type) {
        EXPECT_TRUE(bstk::_Smart_ptr_element<bool>);
        EXPECT_TRUE(bstk::_Smart_ptr_element<const char>);
        EXPECT_TRUE(bstk::_Smart_ptr_element<volatile short>);
        EXPECT_TRUE(bstk::_Smart_ptr_element<const volatile int>);
        EXPECT_TRUE(bstk::_Smart_ptr_element<void*>);
        EXPECT_TRUE(bstk::_Smart_ptr_element<void* const>);
        EXPECT_TRUE(bstk::_Smart_ptr_element<const long*>);
        EXPECT_TRUE(bstk::_Smart_ptr_element<const long* const>);
        EXPECT_TRUE(bstk::_Smart_ptr_element<volatile long long*>);
        EXPECT_TRUE(bstk::_Smart_ptr_element<volatile long long* const>);
        EXPECT_TRUE(bstk::_Smart_ptr_element<const volatile float*>);
        EXPECT_TRUE(bstk::_Smart_ptr_element<const volatile float* const>);
        EXPECT_TRUE(bstk::_Smart_ptr_element<double(*)(void*)>);
        EXPECT_TRUE(bstk::_Smart_ptr_element<long double(*)[]>);
        EXPECT_TRUE(bstk::_Smart_ptr_element<int _Complete_type::*>);
        EXPECT_TRUE(bstk::_Smart_ptr_element<void(_Complete_type::*)()>);
    }

    TEST(unique_array, invalid_element_type) {
        EXPECT_FALSE(bstk::_Smart_ptr_element<void>);
        EXPECT_FALSE(bstk::_Smart_ptr_element<bool&>);
        EXPECT_FALSE(bstk::_Smart_ptr_element<const char&>);
        EXPECT_FALSE(bstk::_Smart_ptr_element<volatile short&>);
        EXPECT_FALSE(bstk::_Smart_ptr_element<const volatile int&>);
        EXPECT_FALSE(bstk::_Smart_ptr_element<long[]>);
        EXPECT_FALSE(bstk::_Smart_ptr_element<long long[4]>);
        EXPECT_FALSE(bstk::_Smart_ptr_element<float()>);
        EXPECT_FALSE(bstk::_Smart_ptr_element<double(long double)>);
        EXPECT_FALSE(bstk::_Smart_ptr_element<_Incomplete_type>);
    }

    TEST(unique_array, default_construct) {
        const unique_array<int> _Unique;
        EXPECT_EQ(_Unique.get(), nullptr);
        EXPECT_EQ(_Unique.size(), 0);
    }

    TEST(unique_array, move_construct) {
        constexpr size_t _Size = 16;
        auto _Unique0          = ::bs::make_unique_array<int>(_Size);
        const int* const _Ptr  = _Unique0.get();
        const unique_array<int> _Unique1(::std::move(_Unique0));
        EXPECT_EQ(_Unique0.get(), nullptr);
        EXPECT_EQ(_Unique0.size(), 0);
        EXPECT_EQ(_Unique1.get(), _Ptr);
        EXPECT_EQ(_Unique1.size(), _Size);
    }

    TEST(unique_array, array_construct) {
        constexpr size_t _Size = 16;
        int* const _Ptr        = ::bs::create_object_array<int>(_Size);
        const unique_array<int> _Unique({_Ptr, _Size}); // destroys _Ptr
        EXPECT_EQ(_Unique.get(), _Ptr);
        EXPECT_EQ(_Unique.size(), _Size);
    }

    TEST(unique_array, move_assign) {
        constexpr size_t _Size = 16;
        auto _Unique0          = ::bs::make_unique_array<int>(_Size);
        const int* const _Ptr  = _Unique0.get();
        unique_array<int> _Unique1;
        _Unique1 = ::std::move(_Unique0);
        EXPECT_EQ(_Unique0.get(), nullptr);
        EXPECT_EQ(_Unique0.size(), 0);
        EXPECT_EQ(_Unique1.get(), _Ptr);
        EXPECT_EQ(_Unique1.size(), _Size);
    }

    TEST(unique_array, subscript_valid) {
        constexpr size_t _Size = 16;
        constexpr size_t _Idx  = 10;
        constexpr int _Value   = 6327;
        int* const _Ptr        = ::bs::create_object_array<int>(_Size);
        _Ptr[_Idx]             = _Value;
        const unique_array<int> _Unique({_Ptr, _Size}); // destroys _Ptr
        EXPECT_EQ(_Unique[_Idx], _Value);
    }

    TEST(unique_array, subscript_invalid) {
        constexpr size_t _Size = 16;
        const auto _Unique     = ::bs::make_unique_array<int>(_Size);
        bool _Caught           = false;
        try {
            _Unique[_Size + 1];
        } catch (const resource_overrun&) {
            _Caught = true;
        }

        EXPECT_TRUE(_Caught);
    }

    TEST(unique_array, get) {
        constexpr size_t _Size = 16;
        int* const _Ptr        = ::bs::create_object_array<int>(_Size);
        const unique_array<int> _Unique({_Ptr, _Size}); // destroys _Ptr
        EXPECT_EQ(_Unique.get(), _Ptr);
    }

    TEST(unique_array, size) {
        constexpr size_t _Size = 16;
        int* const _Ptr        = ::bs::create_object_array<int>(_Size);
        const unique_array<int> _Unique({_Ptr, _Size}); // destroys _Ptr
        EXPECT_EQ(_Unique.size(), _Size);
    }

    TEST(unique_array, get_deleter) {
        using _Deleter_type    = _Named_deleter<int>;
        constexpr char _Name[] = "test-deleter";
        constexpr size_t _Size = 16;
        const unique_array<int, _Deleter_type> _Unique(
            {::bs::create_object_array<int>(_Size), _Size}, _Deleter_type{_Name});
        EXPECT_STREQ(_Unique.get_deleter()._Name, _Name);
    }

    TEST(unique_array, release) {
        constexpr size_t _Size = 16;
        int* const _Ptr        = ::bs::create_object_array<int>(_Size);
        unique_array<int> _Unique({_Ptr, _Size});
        const auto _Array = _Unique.release();
        EXPECT_EQ(_Array.ptr, _Ptr);
        EXPECT_EQ(_Array.size, _Size);
        EXPECT_EQ(_Unique.get(), nullptr);
        EXPECT_EQ(_Unique.size(), 0);
        ::bs::delete_object_array(_Ptr, _Size);
    }

    TEST(unique_array, reset) {
        constexpr size_t _Size = 16;
        int* const _Ptr        = ::bs::create_object_array<int>(_Size);
        unique_array<int> _Unique({_Ptr, _Size}); // destroys _Ptr
        EXPECT_EQ(_Unique.get(), _Ptr);
        EXPECT_EQ(_Unique.size(), _Size);

        _Unique.reset();
        EXPECT_EQ(_Unique.get(), nullptr);
        EXPECT_EQ(_Unique.size(), 0);
    }

    TEST(unique_array, swap) {
        constexpr size_t _Size0 = 16;
        constexpr size_t _Size1 = 32;
        int* const _Ptr0        = ::bs::create_object_array<int>(_Size0);
        int* const _Ptr1        = ::bs::create_object_array<int>(_Size1);
        unique_array<int> _Unique0({_Ptr0, _Size0}); // destroys _Ptr0
        unique_array<int> _Unique1({_Ptr1, _Size1}); // destroys _Ptr1
        _Unique0.swap(_Unique1);
        EXPECT_EQ(_Unique0.get(), _Ptr1);
        EXPECT_EQ(_Unique0.size(), _Size1);
        EXPECT_EQ(_Unique1.get(), _Ptr0);
        EXPECT_EQ(_Unique1.size(), _Size0);
    }

    TEST(unique_array, compare_unique_array) {
        constexpr size_t _Size = 16;
        const auto _Unique0    = ::bs::make_unique_array<int>(_Size);
        unique_array<int> _Unique1({_Unique0.get(), _Size});
        EXPECT_EQ(_Unique0, _Unique1);

        (void) _Unique1.release();
        EXPECT_NE(_Unique0, _Unique1);
    }

    TEST(unique_array, compare_null) {
        auto _Unique = ::bs::make_unique_array<int>(16);
        EXPECT_NE(_Unique, nullptr);

        _Unique.reset();
        EXPECT_EQ(_Unique, nullptr);
    }

    TEST(unique_array, make_unique_array) {
        constexpr size_t _Size = 4;
        const auto _Unique     = ::bs::make_unique_array<_Aggregate_type>(_Size);
        for (size_t _Idx = 0; _Idx < _Size; ++_Idx) {
            const auto& _Elem = _Unique[_Idx];
            EXPECT_EQ(_Elem._Flag, _Aggregate_type::_Default_flag);
            EXPECT_EQ(_Elem._Sign, _Aggregate_type::_Default_sign);
            EXPECT_EQ(_Elem._Number, _Aggregate_type::_Default_number);
        }
    }
} // namespace bs