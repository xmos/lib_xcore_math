// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "xmath/xmath.h"

#include "../tst_common.h"

#include "unity_fixture.h"

TEST_GROUP_RUNNER(divide_32) {
  RUN_TEST_CASE(divide_32, divide_32_all);
}

TEST_GROUP(divide_32);
TEST_SETUP(divide_32) { fflush(stdout); }
TEST_TEAR_DOWN(divide_32) {}

int64_t sx[9] = {
    0x0000000000000000LL,
    0x0000000100000500LL,
    0x0000010000050000LL,
    0x00F0000100000000LL,
    0x0123456789ABCDEFLL,
    -0x0000000100000000LL,
    -0x0000010000050000LL,
    -0x00F0000100000000LL,
    -0x0123456789ABCDEFLL,
};

uint64_t ux[9] = {
    0x0000000000000000LL,
    0x0000000100000000LL,
    0x0000000100000500LL,
    0x0000010000050000LL,
    0x0123456789ABCDEFLL,
    0x00F0000100000000LL,
    0x00F0000100000500LL,
    0x00F0010000050000LL,
    0x01F3456789ABCDEFLL,
};

int32_t sy[8] = {
    0x01234567,
    0x23456780,
    0x00010000,
    0x08765432,
    -0x01234567,
    -0x23456780,
    -0x00010000,
    -0x08765432,
};

int32_t uy[8] = {
    0x01234567,
    0x23456780,
    0x00010000,
    0x08765432,
    0x1F345678,
    0x2F456780,
    0x0F010000,
    0x0F765432,
};

uint64_t abs64(int64_t x) {
    if (x<0) return -x;
    return x;
}

uint32_t abs32(int32_t x) {
    if (x<0) return -x;
    return x;
}

void compareuu(uint64_t x, uint32_t y) {
    uint32_t  s = 0;

    if (x < ((uint64_t)y<<32)) {
        s = x/y;
    }
    else {
        s = UINT32_MAX;
    }

    // printf("u %llu, u %lu\n", x, y);
    uint32_t o_o = u32_divide_u64_u32(x , y);
    TEST_ASSERT_EQUAL(s, o_o);
}

void compareus(uint64_t x, int32_t y) {
    int32_t s = 0;

    if (x < ((uint64_t)abs32(y))<<31) {
        s = ((int64_t)x)/y;
    }
    else {
        s = (y > 0) ? INT32_MAX : INT32_MIN;
    }

    // printf("u %llu, s %ld\n", x, y);
    int32_t o_o = s32_divide_u64_s32(x, y);
    TEST_ASSERT_EQUAL(s, o_o);
}

void comparesu(int64_t x, uint32_t y) {
    int32_t s = 0;

    if (abs64(x) < ((uint64_t)y<<31)) {
        s = x/(int64_t) y;
    }
    else {
        s = (x > 0) ? INT32_MAX: INT32_MIN;
    }

    // printf("s %lld, u %lu\n", x, y);
    int32_t o_o = s32_divide_s64_u32(x, y);
    TEST_ASSERT_EQUAL(s, o_o);
}

void comparess(int64_t x, int32_t y) {
    int32_t s = 0;

    if (abs64(x) < ((uint64_t)abs32(y))<<31) {
        s = x/y;
    }
    else {
        if (((x > 0) && (y > 0)) || ((x < 0) && (y < 0))) {s = INT32_MAX;}
        else if (((x > 0) && (y < 0)) || ((x < 0) && (y > 0))) {s = INT32_MIN;}
        else {TEST_ASSERT(0);}
    }

    // printf("s %lld, s %ld\n", x, y);
    int32_t o_o = s32_divide_s64_s32(x, y);
    TEST_ASSERT_EQUAL(s, o_o);
}

TEST(divide_32, divide_32_all) {
    for(int xi = 0; xi < 9; xi++) {
        for(int yi = 0; yi < 8; yi++) {
            compareuu(ux[xi], uy[yi]);
            compareus(ux[xi], sy[yi]);
            comparesu(sx[xi], uy[yi]);
            comparess(sx[xi], sy[yi]);
        }
    }
}
