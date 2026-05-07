// Tests for memmove and memmem

#include <CppUTest/TestHarness.h>
#include <stdlib.h>
#include "string.h"

namespace
{
    TEST_GROUP(memTests) { };

    TEST(memTests, MemmoveForwardOverlap)
    {
        char buf[12] = "0123456789";
        memmove(buf + 2, buf, 6);
        STRCMP_EQUAL("0101234589", buf);
    }

    TEST(memTests, MemmoveBackwardOverlap)
    {
        char buf[12] = "0123456789";
        memmove(buf, buf + 2, 6);
        STRCMP_EQUAL("2345676789", buf);
    }

    TEST(memTests, MemmoveNoOverlap)
    {
        char src[] = "abcdef";
        char dst[7];
        memmove(dst, src, 6);
        STRCMP_EQUAL("abcdef", dst);
    }

    TEST(memTests, MemmemEmptyNeedleReturnsHay)
    {
        const char hay[] = "hello";
        void *p = memmem(hay, sizeof(hay)-1, "", 0);
        CHECK(p == hay);
    }

    TEST(memTests, MemmemSingleByte)
    {
        const char hay[] = "abcde";
        void *p = memmem(hay, sizeof(hay)-1, "c", 1);
        CHECK(p != nullptr);
        STRCMP_EQUAL("cde", (char*)p);
    }

    TEST(memTests, MemmemNotFound)
    {
        const char hay[] = "abcdef";
        const char needle[] = "xyz";
        void *p = memmem(hay, sizeof(hay)-1, needle, sizeof(needle)-1);
        CHECK(p == nullptr);
    }

    TEST(memTests, MemmemAtEnd)
    {
        const char hay[] = "xxneedle";
        const char needle[] = "needle";
        void *p = memmem(hay, sizeof(hay)-1, needle, sizeof(needle)-1);
        CHECK(p != nullptr);
        STRCMP_EQUAL("needle", (char*)p);
    }

    TEST(memTests, MemmemLongerThanHaystack)
    {
        const char hay[] = "short";
        const char needle[] = "longerneedle";
        void *p = memmem(hay, sizeof(hay)-1, needle, sizeof(needle)-1);
        CHECK(p == nullptr);
    }

}

    /* Branch-coverage focused tests moved here from branch_coverage_tests.cpp */

    TEST(memTests, MemmoveAlignedBigBlock)
    {
        size_t len = 128; // should be >= BIGBLOCKSIZE on 64-bit
        char *src = (char*)malloc(len + 16);
        char *dst = (char*)malloc(len + 16);
        for (size_t i = 0; i < len; ++i)
            src[i] = (char)(i & 0xFF);
        memset(dst, 0, len);
        memmove(dst, src, len);
        CHECK(memcmp(dst, src, len) == 0);
        free(src);
        free(dst);
    }

    TEST(memTests, MemmoveUnalignedBigBlock)
    {
        size_t len = 128; // > BIGBLOCKSIZE
        char *base_src = (char*)malloc(len + 16);
        char *base_dst = (char*)malloc(len + 16);
        char *src = base_src + 1; /* make unaligned */
        char *dst = base_dst + 1; /* make unaligned */
        for (size_t i = 0; i < len; ++i)
            src[i] = (char)((i * 31) & 0xFF);
        memset(dst, 0, len);
        memmove(dst, src, len);
        CHECK(memcmp(dst, src, len) == 0);
        free(base_src);
        free(base_dst);
    }

    TEST(memTests, MemmoveSmallAligned)
    {
        const size_t len = 16; // < BIGBLOCKSIZE on 64-bit
        char src[len];
        char dst[len];
        for (size_t i = 0; i < len; ++i) src[i] = (char)(i & 0xFF);
        memset(dst, 0, len);
        memmove(dst, src, len);
        CHECK(memcmp(dst, src, len) == 0);
    }

    TEST(memTests, MemmoveZeroLength)
    {
        char src[] = "hello world";
        char dst[] = "XXXXXXXXXXX"; // same size or larger
        char before[sizeof(dst)];
        memcpy(before, dst, sizeof(dst));
        memmove(dst, src, 0);
        MEMCMP_EQUAL(before, dst, sizeof(dst));
    }

    TEST(memTests, MemmemTwoByteNeedle)
    {
        const char hay[] = "abababababab";
        const char needle[] = "ab"; // length == 2
        void *p = memmem(hay, sizeof(hay)-1, needle, 2);
        CHECK(p != nullptr);
        POINTERS_EQUAL((void*)hay, p);
        CHECK_EQUAL(0, strncmp((char*)p, "ab", 2));
    }

    TEST(memTests, MemmemThreeByteNeedleHashPath)
    {
        const char hay[] = "xxabcxxabcxx";
        const char needle[] = "abc"; // length == 3 should trigger hash path
        void *p = memmem(hay, sizeof(hay)-1, needle, 3);
        CHECK(p != nullptr);
        LONGS_EQUAL(2, (long)((char*)p - hay));
    }

    TEST(memTests, MemmemHashPathNotFound)
    {
        const size_t hay_len = 512;
        const size_t ne_len = 5;
        char *hay = (char*)malloc(hay_len);
        for (size_t i = 0; i < hay_len; ++i) hay[i] = 'A';
        const char needle[] = "BCDEF";
        void *p = memmem(hay, hay_len, needle, ne_len);
        CHECK(p == nullptr);
        free(hay);
    }

    TEST(memTests, MemmemHashPathAtEnd)
    {
        const char prefix[] = "xxxxxxx";
        const char needle[] = "END!";
        size_t hay_len = sizeof(prefix)-1 + sizeof(needle)-1;
        char *hay = (char*)malloc(hay_len);
        memcpy(hay, prefix, sizeof(prefix)-1);
        memcpy(hay + sizeof(prefix)-1, needle, sizeof(needle)-1);
        void *p = memmem(hay, hay_len, needle, sizeof(needle)-1);
        CHECK(p != nullptr);
        LONGS_EQUAL((long)(sizeof(prefix)-1), (long)((char*)p - hay));
        free(hay);
    }

    TEST(memTests, MemmemPeriodicShort)
    {
        const char hay[] = "zzababababzzz";
        const char needle[] = "abababab"; // periodic short needle
        void *p = memmem(hay, sizeof(hay)-1, needle, sizeof(needle)-1);
        CHECK(p != nullptr);
        LONGS_EQUAL(2, (long)((char*)p - hay));
    }

    TEST(memTests, MemmemShortHashPath)
    {
        const char hay[] = "xxabcdxxabcdxx";
        const char needle[] = "abcd"; // should exercise the hash/shift path
        void *p = memmem(hay, sizeof(hay)-1, needle, sizeof(needle)-1);
        CHECK(p != nullptr);
        LONGS_EQUAL(2, (long)((char*)p - hay));
    }

    TEST(memTests, MemmemLongNeedleTwoWay)
    {
        const size_t needle_len = 300; // > 256 to trigger two_way_long_needle
        const size_t hay_len = 2000;
        char *hay = (char*)malloc(hay_len);
        char *needle = (char*)malloc(needle_len);
        for (size_t i = 0; i < hay_len; ++i) hay[i] = 'A';
        for (size_t i = 0; i < needle_len; ++i) needle[i] = (char)('a' + (i % 26));
        size_t insert_at = 500;
        if (insert_at + needle_len <= hay_len)
            memcpy(hay + insert_at, needle, needle_len);

        void *p = memmem(hay, hay_len, needle, needle_len);
        CHECK(p != nullptr);
        LONGS_EQUAL((long)insert_at, (long)((char*)p - hay));

        free(hay);
        free(needle);
    }

    TEST(memTests, MemmemLongNeedlePeriodic)
    {
        const size_t needle_len = 300; // > 256 triggers two_way_long_needle
        const size_t hay_len = 1200;
        char *hay = (char*)malloc(hay_len);
        char *needle = (char*)malloc(needle_len);
        /* make a periodic needle: repeating "ab" */
        for (size_t i = 0; i < needle_len; ++i) needle[i] = (i % 2) ? 'b' : 'a';
        memset(hay, 'X', hay_len);
        size_t pos = 400;
        if (pos + needle_len <= hay_len) memcpy(hay + pos, needle, needle_len);

        void *p = memmem(hay, hay_len, needle, needle_len);
        CHECK(p != nullptr);
        LONGS_EQUAL((long)pos, (long)((char*)p - hay));
        free(hay);
        free(needle);
    }

    TEST(memTests, MemmemLongNeedleNonPeriodic)
    {
        const size_t needle_len = 300; // > 256 triggers two_way_long_needle
        const size_t hay_len = 1200;
        char *hay = (char*)malloc(hay_len);
        char *needle = (char*)malloc(needle_len);
        /* make a non-periodic needle: ascending bytes */
        for (size_t i = 0; i < needle_len; ++i) needle[i] = (char)(i & 0xFF);
        memset(hay, 'Z', hay_len);
        size_t pos = 600;
        if (pos + needle_len <= hay_len) memcpy(hay + pos, needle, needle_len);

        void *p = memmem(hay, hay_len, needle, needle_len);
        CHECK(p != nullptr);
        LONGS_EQUAL((long)pos, (long)((char*)p - hay));
        free(hay);
        free(needle);
    }
