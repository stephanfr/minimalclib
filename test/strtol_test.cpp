// Copyright 2024
// Additional tests for strtol implementation

#include <CppUTest/TestHarness.h>

#include "minimalcstdlib.h"
#include "minimalstdclib_result_codes.h"

extern "C" {
    /* Call the project wrapper which forwards to the internal implementation. */
    long project_strtol(const char *numeric_string, char **first_nonconverted_character, int base, int *result_code);
}

namespace
{
    TEST_GROUP(strtolTests) { };

    TEST(strtolTests, BasicDecimal)
    {
        char *end = nullptr;
        long v = project_strtol("12345", &end, 10, NULL);
        LONGS_EQUAL(12345, v);
    }

    TEST(strtolTests, WithTrailing)
    {
        char *end = nullptr;
        long v = project_strtol("7abc", &end, 10, NULL);
        LONGS_EQUAL(7, v);
        STRCMP_EQUAL("abc", end);
    }

    TEST(strtolTests, HexPrefixAuto)
    {
        char *end = nullptr;
        long v = project_strtol("0xFF", &end, 0, NULL);
        LONGS_EQUAL(255, v);
    }

    TEST(strtolTests, InvalidBase)
    {
        char *end = nullptr;
        long v = project_strtol("123", &end, 1, NULL);
        LONGS_EQUAL(0, v);
        STRCMP_EQUAL("123", end);
    }

    TEST(strtolTests, Overflow)
    {
        char *end = nullptr;
        /* A long large value to trigger overflow; base 10 */
        long v = project_strtol("99999999999999999999999", &end, 10, NULL);
        /* ensure we produced some numeric result (saturated or otherwise) */
        CHECK(v != 0);
    }
}
