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

int64_t sx[8] = {
    0x0000000100000000LL,
    0x0000000100000500LL,
    0x0000010000050000LL,
    0x0123456789ABCDEFLL,
    -0x0000000100000000LL,
    -0x0000000100000500LL,
    -0x0000010000050000LL,
    -0x0123456789ABCDEFLL,
};

uint64_t ux[8] = {
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
    0x12345678,
    0x23456780,
    0x00010000,
    0x08765432,
    -0x12345678,
    -0x23456780,
    -0x00010000,
    -0x08765432,
};

int32_t uy[8] = {
    0x12345678,
    0x23456780,
    0x00010000,
    0x08765432,
    0x1F345678,
    0x2F456780,
    0x0F010000,
    0x0F765432,
};

int err = 0;

int testuu = 0;
int testus = 0;
int testsu = 0;
int testss = 0;

uint64_t abs64(int64_t x) {
    if (x<0) return -x;
    return x;
}

void compareuu(int mode, uint64_t x, uint32_t y) {
    if (x < ((uint64_t)y<<32)) {
        uint32_t s = x/y;
        uint32_t o_o = u32_divide_u64_u32(x, y);
        TEST_ASSERT( o_o == s);
        testuu++;
    }
}

void compareus(int mode, uint64_t x, int32_t y) {
    if (x < abs64((uint64_t)y<<32)) {
        int32_t s = ((int64_t)x)/y;
        int32_t o_o = s32_divide_u64_s32(x, y);
        TEST_ASSERT( o_o == s);
        testus++;
    }
}

void comparesu(int mode, int64_t x, uint32_t y) {
    if (abs64(x) < ((uint64_t)y<<32)) {
        int32_t o_o = s32_divide_s64_u32(x, y) ;
        int32_t s = x/(int64_t) y;
        TEST_ASSERT( o_o == s);
        testsu++;
    }
}

void comparess(int mode, int64_t x, int32_t y) {
    if (abs64(x) < abs64((int64_t)y<<32)) {
        int32_t o_o = s32_divide_s64_s32(x, y);
        int32_t s = x/y;
        TEST_ASSERT( o_o == s);
        testss++;
    }
}

TEST(divide_32, divide_32_all) {
    for(int xi = 0; xi < 8; xi++) {
        for(int yi = 0; yi < 8; yi++) {
            compareuu(0, ux[xi], uy[yi]);
            compareus(1, ux[xi], sy[yi]);
            comparesu(2, sx[xi], uy[yi]);
            comparess(3, sx[xi], sy[yi]);
        }
    }
    if (err != 0) {
        printf("FAIL %d errors\n", err);
    } else {
        printf("PASS %d %d %d %d tests\n", testuu, testus, testsu, testss);
    }
}

