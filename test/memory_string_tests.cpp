// Tests for memory and string helpers

#include <CppUTest/TestHarness.h>

#include "minimalcstdlib.h"
#include "string.h"

namespace
{
    TEST_GROUP(memoryStringTests) { };

    TEST(memoryStringTests, MemcpyMemcmpMemsetMemchr)
    {
        char src[16] = "hello\0world";
        char dst[16];
        memset(dst, 0, sizeof(dst));
        memcpy(dst, src, 11);
        CHECK(memcmp(dst, src, 11) == 0);
        char *p = (char*)memchr(dst, 'w', 16);
        CHECK(p != nullptr);
        STRCMP_EQUAL("world", p);
    }

    TEST(memoryStringTests, MemmoveOverlap)
    {
        char buf[16] = "0123456789";
        memmove(buf + 2, buf, 8);
        STRCMP_EQUAL("0101234567", buf);
    }

    TEST(memoryStringTests, StrchrStrrchr)
    {
        const char *s = "abcbcde";
        char *first = strchr(s, 'c');
        char *last = strrchr(s, 'c');
        CHECK(first != nullptr);
        CHECK(last != nullptr);
        LONGS_EQUAL(2, (int)(first - s));
        LONGS_EQUAL(4, (int)(last - s));
    }

    TEST(memoryStringTests, StrlcpyStrnstr)
    {
        char dst[8];
        size_t n = strlcpy(dst, "hello", sizeof(dst));
        STRCMP_EQUAL("hello", dst);
        LONGS_EQUAL(5, (int)n);
        const char *found = strnstr("abcde12345", "de12", 8);
        CHECK(found != nullptr);
        CHECK(strncmp(found, "de12", 4) == 0);
    }

    TEST(memoryStringTests, StrncmpStrnicmp)
    {
        LONGS_EQUAL(0, strncmp("abc", "abc", 3));
        LONGS_EQUAL(0, strnicmp("AbC", "aBc", 3));
    }

    TEST(memoryStringTests, MemmemFind)
    {
        const char hay[] = "xxneedleyy";
        const char nd[] = "needle";
        void *res = memmem(hay, sizeof(hay)-1, nd, sizeof(nd)-1);
        CHECK(res != nullptr);
        STRCMP_EQUAL("needleyy", (char*)res);
    }

}
