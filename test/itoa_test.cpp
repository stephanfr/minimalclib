// Tests for itoa and utoa

#include <CppUTest/TestHarness.h>

#include "minimalcstdlib.h"

namespace
{
    TEST_GROUP(itoaTests) { };

    TEST(itoaTests, ItoaDecimal)
    {
        char buf[32];
        char *p = itoa(-12345, buf, 10);
        STRCMP_EQUAL("-12345", p);
    }

    TEST(itoaTests, UtoaHex)
    {
        char buf[32];
        char *p = utoa(255u, buf, 16);
        STRCMP_EQUAL("ff", p);
    }

    TEST(itoaTests, ItoaZero)
    {
        char buf[32];
        char *p = itoa(0, buf, 10);
        STRCMP_EQUAL("0", p);
    }
}
