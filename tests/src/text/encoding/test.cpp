// test.cpp

// Copyright (c) Bitstro Systems. All rights reserved.
// Licensed under BSDL 1.0

#include <bstk/text/encoding.hpp>
#include <gtest/gtest.h>

namespace bs {
    void _Test_transcode(const wchar_t* const _Unicode, const char* const _Utf8) {
        const utf8_string _Utf8_str   = ::bs::to_utf8_string(_Unicode);
        const unicode_string _Unc_str = ::bs::to_unicode_string(_Utf8_str);
        EXPECT_EQ(_Utf8_str, _Utf8);
        EXPECT_EQ(_Unc_str, _Unicode);

        const size_t _Utf8_size = ::bs::utf8_string_size(_Unicode);
        const size_t _Unc_size  = ::bs::unicode_string_size(_Utf8_str);
        EXPECT_EQ(_Utf8_size, _Utf8_str.size());
        EXPECT_EQ(_Unc_size, _Unc_str.size());
    }

    void _Test_invalid_transcode(const char* const _Str) {
        EXPECT_TRUE(::bs::to_unicode_string(_Str).empty());
    }

    TEST(encoding, empty) {
        _Test_transcode(L"", "");
    }

    TEST(encoding, valid_one_byte) {
        _Test_transcode(L"Ab$3z", "\x41\x62\x24\x33\x7A");
        _Test_transcode(L"Mn*9x", "\x4D\x6E\x2A\x39\x78");
        _Test_transcode(L"Pq!4y", "\x50\x71\x21\x34\x79");
        _Test_transcode(L"Rs#5w", "\x52\x73\x23\x35\x77");
        _Test_transcode(L"Tu&6v", "\x54\x75\x26\x36\x76");
    }

    TEST(encoding, valid_two_bytes) {
        _Test_transcode(L"©éăđħ", "\xC2\xA9\xC3\xA9\xC4\x83\xC4\x91\xC4\xA7");
        _Test_transcode(L"ıłœťŷ", "\xC4\xB1\xC5\x82\xC5\x93\xC5\xA5\xC5\xB7");
        _Test_transcode(L"ƈƙơƴǅ", "\xC6\x88\xC6\x99\xC6\xA1\xC6\xB4\xC7\x85");
        _Test_transcode(L"ǖǧǸȉȚ", "\xC7\x96\xC7\xA7\xC7\xB8\xC8\x89\xC8\x9A");
        _Test_transcode(L"ȫȼɍɞɯ", "\xC8\xAB\xC8\xBC\xC9\x8D\xC9\x9E\xC9\xAF");
    }

    TEST(encoding, valid_three_bytes) {
        _Test_transcode(L"กขฃคฅ", "\xE0\xB8\x81\xE0\xB8\x82\xE0\xB8\x83\xE0\xB8\x84\xE0\xB8\x85");
        _Test_transcode(L"अआइईउ", "\xE0\xA4\x85\xE0\xA4\x86\xE0\xA4\x87\xE0\xA4\x88\xE0\xA4\x89");
        _Test_transcode(L"你好世界人", "\xE4\xBD\xA0\xE5\xA5\xBD\xE4\xB8\x96\xE7\x95\x8C\xE4\xBA\xBA");
        _Test_transcode(L"あいうえお", "\xE3\x81\x82\xE3\x81\x84\xE3\x81\x86\xE3\x81\x88\xE3\x81\x8A");
        _Test_transcode(L"അആഇഈഉ", "\xE0\xB4\x85\xE0\xB4\x86\xE0\xB4\x87\xE0\xB4\x88\xE0\xB4\x89");
    }

    TEST(encoding, valid_four_bytes) {
        _Test_transcode(L"😀😁😂😃😄",
            "\xF0\x9F\x98\x80\xF0\x9F\x98\x81\xF0\x9F\x98\x82\xF0\x9F\x98\x83\xF0\x9F\x98\x84");
        _Test_transcode(L"🚀🚁🚂🚃🚄",
            "\xF0\x9F\x9A\x80\xF0\x9F\x9A\x81\xF0\x9F\x9A\x82\xF0\x9F\x9A\x83\xF0\x9F\x9A\x84");
        _Test_transcode(L"💩💪💫💬💭",
            "\xF0\x9F\x92\xA9\xF0\x9F\x92\xAA\xF0\x9F\x92\xAB\xF0\x9F\x92\xAC\xF0\x9F\x92\xAD");
        _Test_transcode(L"🌍🌎🌏🌐🌑",
            "\xF0\x9F\x8C\x8D\xF0\x9F\x8C\x8E\xF0\x9F\x8C\x8F\xF0\x9F\x8C\x90\xF0\x9F\x8C\x91");
        _Test_transcode(L"🎤🎥🎦🎧🎨",
            "\xF0\x9F\x8E\xA4\xF0\x9F\x8E\xA5\xF0\x9F\x8E\xA6\xF0\x9F\x8E\xA7\xF0\x9F\x8E\xA8");
    }

    TEST(encoding, invalid_two_bytes) {
        _Test_invalid_transcode("\xC0\x80\xC1\x81\xC2\x82");
        _Test_invalid_transcode("\xC2\x20\xC3\x30\xC4\x40");
        _Test_invalid_transcode("\xC2\xE3\xC3\xF4\xC4");
    }

    TEST(encoding, invalid_three_bytes) {
        _Test_invalid_transcode("\xE0\x80\x80\xE0\x81\x81\xE0\x82\x82");
        _Test_invalid_transcode("\xE1\x20\x80\xE2\x30\x81\xE3\x40\x82");
        _Test_invalid_transcode("\xE1\x80\xE2\x81\xE3\x82");
    }

    TEST(encoding, invalid_four_bytes) {
        _Test_invalid_transcode("\xF0\x80\x80\x80\xF0\x81\x81\x81\xF0\x82\x82\x82");
        _Test_invalid_transcode("\xF1\x20\x80\x80\xF2\x30\x81\x81\xF3\x40\x82\x82");
        _Test_invalid_transcode("\xF1\x80\x80\xF2\x81\x81\xF3\x82\x82");
    }
} // namespace bs