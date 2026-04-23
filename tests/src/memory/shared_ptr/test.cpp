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

    TEST(shared_ptr, default_construct) {
        const shared_ptr<int> _Shared;
        EXPECT_EQ(_Shared.get(), nullptr);
    }

    TEST(shared_ptr, null_construct) {
        const shared_ptr<int> _Shared(nullptr);
        EXPECT_EQ(_Shared.get(), nullptr);
    }

    TEST(shared_ptr, pointer_construct) {
        int* const _Ptr = ::bs::create_object<int>(14351);
        const shared_ptr<int> _Shared(_Ptr); // destroys _Ptr
        EXPECT_EQ(_Shared.get(), _Ptr);
    }

    TEST(shared_ptr, copy_construct) {
        const auto _Shared0 = ::bs::make_shared<int>(96515);
        const auto _Shared1 = _Shared0;
        EXPECT_EQ(_Shared0.get(), _Shared1.get());
        EXPECT_EQ(_Shared0.use_count(), 2);
        EXPECT_EQ(_Shared1.use_count(), 2);
    }

    TEST(shared_ptr, move_construct) {
        auto _Shared0       = ::bs::make_shared<int>(65162);
        int* const _Ptr     = _Shared0.get();
        const auto _Shared1 = ::std::move(_Shared0);
        EXPECT_EQ(_Shared0.get(), nullptr);
        EXPECT_EQ(_Shared0.use_count(), 0);
        EXPECT_EQ(_Shared1.get(), _Ptr);
        EXPECT_EQ(_Shared1.use_count(), 1);
    }

    TEST(shared_ptr, unique_ptr_construct) {
        auto _Unique    = ::bs::make_unique<int>(41762);
        int* const _Ptr = _Unique.get();
        const shared_ptr<int> _Shared(::std::move(_Unique));
        EXPECT_EQ(_Unique.get(), nullptr);
        EXPECT_EQ(_Shared.get(), _Ptr);
        EXPECT_EQ(_Shared.use_count(), 1);
    }

    TEST(shared_ptr, copy_assign) {
        const auto _Shared0 = ::bs::make_shared<int>(96515);
        shared_ptr<int> _Shared1;
        _Shared1 = _Shared0;
        EXPECT_EQ(_Shared0.get(), _Shared1.get());
        EXPECT_EQ(_Shared0.use_count(), 2);
        EXPECT_EQ(_Shared1.use_count(), 2);
    }

    TEST(shared_ptr, move_assign) {
        auto _Shared0   = ::bs::make_shared<int>(65162);
        int* const _Ptr = _Shared0.get();
        shared_ptr<int> _Shared1;
        _Shared1 = ::std::move(_Shared0);
        EXPECT_EQ(_Shared0.get(), nullptr);
        EXPECT_EQ(_Shared0.use_count(), 0);
        EXPECT_EQ(_Shared1.get(), _Ptr);
        EXPECT_EQ(_Shared1.use_count(), 1);
    }

    TEST(shared_ptr, unique_ptr_assign) {
        auto _Unique    = ::bs::make_unique<int>(7352);
        int* const _Ptr = _Unique.get();
        shared_ptr<int> _Shared;
        _Shared = ::std::move(_Unique);
        EXPECT_EQ(_Unique.get(), nullptr);
        EXPECT_EQ(_Shared.get(), _Ptr);
        EXPECT_EQ(_Shared.use_count(), 1);
    }

    TEST(shared_ptr, dereference) {
        constexpr int _Value = 8416;
        const auto _Shared   = ::bs::make_shared<int>(_Value);
        EXPECT_EQ(*_Shared, _Value);
    }

    TEST(shared_ptr, get) {
        int* const _Ptr = ::bs::create_object<int>(9416);
        const shared_ptr<int> _Shared(_Ptr); // destroys _Ptr
        EXPECT_EQ(_Shared.get(), _Ptr);
    }

    TEST(shared_ptr, get_deleter) {
        using _Deleter_type    = _Named_deleter<int>;
        constexpr char _Name[] = "test-deleter";
        const shared_ptr<int, _Deleter_type> _Shared(nullptr, _Deleter_type{_Name});
        EXPECT_STREQ(_Shared.get_deleter()._Name, _Name);
    }

    TEST(shared_ptr, use_count) {
        const auto _Shared0 = ::bs::make_shared<int>(32574);
        EXPECT_EQ(_Shared0.use_count(), 1);
        {
            const auto _Shared1 = _Shared0;
            const auto _Shared2 = _Shared0;
            EXPECT_EQ(_Shared0.use_count(), 3);
        }

        EXPECT_EQ(_Shared0.use_count(), 1);
    }

    TEST(shared_ptr, reset) {
        auto _Shared = ::bs::make_shared<int>(15763);
        EXPECT_NE(_Shared.get(), nullptr);
        EXPECT_EQ(_Shared.use_count(), 1);

        _Shared.reset();
        EXPECT_EQ(_Shared.get(), nullptr);
        EXPECT_EQ(_Shared.use_count(), 0);

        int* const _Ptr = ::bs::create_object<int>(5167);
        _Shared.reset(_Ptr); // destroys _Ptr
        EXPECT_EQ(_Shared.get(), _Ptr);
        EXPECT_EQ(_Shared.use_count(), 1);
    }

    TEST(shared_ptr, swap) {
        int* const _Ptr0 = ::bs::create_object<int>(8231);
        int* const _Ptr1 = ::bs::create_object<int>(6123);
        shared_ptr<int> _Shared0(_Ptr0); // destroys _Ptr0
        shared_ptr<int> _Shared1(_Ptr1); // destroys _Ptr1
        _Shared0.swap(_Shared1);
        EXPECT_EQ(_Shared0.get(), _Ptr1);
        EXPECT_EQ(_Shared1.get(), _Ptr0);
    }

    TEST(shared_ptr, compare_shared_ptr) {
        const auto _Shared0 = ::bs::make_shared<int>(7416);
        auto _Shared1       = _Shared0;
        EXPECT_EQ(_Shared0, _Shared1);

        _Shared1.reset();
        EXPECT_NE(_Shared0, _Shared1);
    }

    TEST(shared_ptr, compare_null) {
        auto _Shared = ::bs::make_shared<int>(4151);
        EXPECT_NE(_Shared, nullptr);

        _Shared.reset();
        EXPECT_EQ(_Shared, nullptr);
    }

    TEST(shared_ptr, make_shared) {
        constexpr bool _Flag        = true;
        constexpr char _Sign        = 'I';
        constexpr int _Number       = 231;
        const auto _Shared          = ::bs::make_shared<_Aggregate_type>(_Flag, _Sign, _Number);
        const _Aggregate_type& _Obj = *_Shared;
        EXPECT_EQ(_Obj._Flag, _Flag);
        EXPECT_EQ(_Obj._Sign, _Sign);
        EXPECT_EQ(_Obj._Number, _Number);
    }

    TEST(shared_ptr, make_shared_for_overwrite) {
        const auto _Shared          = ::bs::make_shared_for_overwrite<_Aggregate_type>();
        const _Aggregate_type& _Obj = *_Shared;
        EXPECT_EQ(_Obj._Flag, _Aggregate_type::_Default_flag);
        EXPECT_EQ(_Obj._Sign, _Aggregate_type::_Default_sign);
        EXPECT_EQ(_Obj._Number, _Aggregate_type::_Default_number);
    }
} // namespace bs