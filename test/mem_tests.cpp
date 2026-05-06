// Tests for memmove and memmem

#include <CppUTest/TestHarness.h>
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
