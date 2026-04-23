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

        void operator()(_Ty* const _Obj) const {
            ::bs::delete_object(_Obj);
        }
    };

    TEST(unique_ptr, default_construct) {
        const unique_ptr<int> _Unique;
        EXPECT_EQ(_Unique.get(), nullptr);
    }

    TEST(unique_ptr, null_construct) {
        const unique_ptr<int> _Unique(nullptr);
        EXPECT_EQ(_Unique.get(), nullptr);
    }

    TEST(unique_ptr, move_construct) {
        auto _Unique0                  = ::bs::make_unique<int>(9551);
        const int* const _Ptr          = _Unique0.get();
        const unique_ptr<int> _Unique1 = ::std::move(_Unique0);
        EXPECT_EQ(_Unique0.get(), nullptr);
        EXPECT_EQ(_Unique1.get(), _Ptr);
    }

    TEST(unique_ptr, pointer_construct) {
        int* const _Ptr = ::bs::create_object<int>(5512);
        const unique_ptr<int> _Unique(_Ptr); // destroys _Ptr
        EXPECT_EQ(_Unique.get(), _Ptr);
    }

    TEST(unique_ptr, move_assign) {
        auto _Unique0         = ::bs::make_unique<int>(3863);
        const int* const _Ptr = _Unique0.get();
        unique_ptr<int> _Unique1;
        _Unique1 = ::std::move(_Unique0);
        EXPECT_EQ(_Unique0.get(), nullptr);
        EXPECT_EQ(_Unique1.get(), _Ptr);
    }

    TEST(unique_ptr, null_assign) {
        auto _Unique = ::bs::make_unique<int>(7323);
        _Unique      = nullptr;
        EXPECT_EQ(_Unique.get(), nullptr);
    }

    TEST(unique_ptr, dereference) {
        constexpr int _Value = 1826;
        const auto _Unique   = ::bs::make_unique<int>(_Value);
        EXPECT_EQ(*_Unique, _Value);
    }

    TEST(unique_ptr, get) {
        int* const _Ptr = ::bs::create_object<int>(26);
        const unique_ptr<int> _Unique(_Ptr); // destroys _Ptr
        EXPECT_EQ(_Unique.get(), _Ptr);
    }

    TEST(unique_ptr, get_deleter) {
        using _Deleter_type    = _Named_deleter<int>;
        constexpr char _Name[] = "test-deleter";
        const unique_ptr<int, _Deleter_type> _Unique(nullptr, _Deleter_type{_Name});
        EXPECT_STREQ(_Unique.get_deleter()._Name, _Name);
    }

    TEST(unique_ptr, release) {
        int* const _Ptr = ::bs::create_object<int>(636);
        unique_ptr<int> _Unique(_Ptr);
        EXPECT_EQ(_Unique.release(), _Ptr);
        EXPECT_EQ(_Unique.get(), nullptr);
        ::bs::delete_object(_Ptr);
    }

    TEST(unique_ptr, reset) {
        int* const _Ptr = ::bs::create_object<int>(492);
        unique_ptr<int> _Unique(_Ptr);
        EXPECT_EQ(_Unique.get(), _Ptr);

        _Unique.reset();
        EXPECT_EQ(_Unique.get(), nullptr);
    }

    TEST(unique_ptr, swap) {
        int* const _Ptr0 = ::bs::create_object<int>();
        int* const _Ptr1 = ::bs::create_object<int>();
        unique_ptr<int> _Unique0(_Ptr0);
        unique_ptr<int> _Unique1(_Ptr1);
        _Unique0.swap(_Unique1);
        EXPECT_EQ(_Unique0.get(), _Ptr1);
        EXPECT_EQ(_Unique1.get(), _Ptr0);
    }

    TEST(unique_ptr, compare_unique_ptr) {
        const auto _Unique0 = ::bs::make_unique<int>(1823);
        unique_ptr<int> _Unique1(_Unique0.get());
        EXPECT_EQ(_Unique0, _Unique1);

        (void) _Unique1.release();
        EXPECT_NE(_Unique0, _Unique1);
    }

    TEST(unique_ptr, compare_null) {
        auto _Unique = ::bs::make_unique<int>(8516);
        EXPECT_NE(_Unique, nullptr);

        _Unique.reset();
        EXPECT_EQ(_Unique, nullptr);
    }

    TEST(unique_ptr, make_unique) {
        constexpr bool _Flag        = true;
        constexpr char _Sign        = 'C';
        constexpr int _Number       = 71.122f;
        const auto _Unique          = ::bs::make_unique<_Aggregate_type>(_Flag, _Sign, _Number);
        const _Aggregate_type& _Obj = *_Unique;
        EXPECT_EQ(_Obj._Flag, _Flag);
        EXPECT_EQ(_Obj._Sign, _Sign);
        EXPECT_EQ(_Obj._Number, _Number);
    }

    TEST(unique_ptr, make_unique_for_overwrite) {
        const auto _Unique          = ::bs::make_unique_for_overwrite<_Aggregate_type>();
        const _Aggregate_type& _Obj = *_Unique;
        EXPECT_EQ(_Obj._Flag, _Aggregate_type::_Default_flag);
        EXPECT_EQ(_Obj._Sign, _Aggregate_type::_Default_sign);
        EXPECT_EQ(_Obj._Number, _Aggregate_type::_Default_number);
    }
} // namespace bs