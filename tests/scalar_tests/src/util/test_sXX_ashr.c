// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "xmath/xmath.h"

#include "../tst_common.h"

#include "unity_fixture.h"

TEST_GROUP_RUNNER(sXX_ashr) {
  RUN_TEST_CASE(sXX_ashr, s32_ashr_right);
  RUN_TEST_CASE(sXX_ashr, s32_ashr_left);
  RUN_TEST_CASE(sXX_ashr, s32_ashr_boundary);
  RUN_TEST_CASE(sXX_ashr, s16_ashr_right);
  RUN_TEST_CASE(sXX_ashr, s16_ashr_left);
  RUN_TEST_CASE(sXX_ashr, s16_ashr_boundary);
}

TEST_GROUP(sXX_ashr);
TEST_SETUP(sXX_ashr) { fflush(stdout); }
TEST_TEAR_DOWN(sXX_ashr) {}

#if SMOKE_TEST
#  define REPS       (128)
#else
#  define REPS       (1024)
#endif

// Reference implementation used to compute expected values
static int32_t ref_s32_ashr(int32_t x, right_shift_t shr)
{
    if (shr > 31) {
        return (x >= 0) ? 0 : (int32_t)0xffffffff;
    } else if (shr >= 0) {
        return x >> shr;
    } else if (shr < -31) {
        return (x == 0) ? 0 : (x > 0) ? INT32_MAX : INT32_MIN;
    } else {
        int64_t tmp = ((int64_t)x) << -shr;
        return (tmp > INT32_MAX) ? INT32_MAX : (tmp < INT32_MIN) ? INT32_MIN : (int32_t)tmp;
    }
}

static void s32_test(unsigned seed, right_shift_t shr)
{
    int32_t x = pseudo_rand_int32(&seed);
    int32_t expected = ref_s32_ashr(x, shr);
    int32_t actual   = s32_ashr(x, shr);

    TEST_ASSERT_EQUAL_INT32(expected, actual);
}

// Arithmetic right shift: shr in [0, 40], verifying sign-extension and large shifts
TEST(sXX_ashr, s32_ashr_right)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    for (unsigned v = 0; v < REPS; v++) {
        setExtraInfo_RS(v, seed);

        right_shift_t shr = (right_shift_t) pseudo_rand_uint(&seed, 0, 40);
        s32_test(seed, shr);
    }
}

// Left-shifting with saturation: shr in [-40, -1]
TEST(sXX_ashr, s32_ashr_left)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    for (unsigned v = 0; v < REPS; v++) {
        setExtraInfo_RS(v, seed);

        right_shift_t shr = -((right_shift_t) pseudo_rand_uint(&seed, 1, 40));
        s32_test(seed, shr);
    }
}

// Boundary and corner cases
TEST(sXX_ashr, s32_ashr_boundary)
{
    // shr = 0: identity
    TEST_ASSERT_EQUAL_INT32( 0,          s32_ashr( 0,          0));
    TEST_ASSERT_EQUAL_INT32( INT32_MAX,  s32_ashr( INT32_MAX,  0));
    TEST_ASSERT_EQUAL_INT32( INT32_MIN,  s32_ashr( INT32_MIN,  0));

    // shr = 1: simple halving
    TEST_ASSERT_EQUAL_INT32( 1,          s32_ashr( 3,          1));
    TEST_ASSERT_EQUAL_INT32(-1,          s32_ashr(-1,          1));

    // shr = 31: only sign bit should remain
    TEST_ASSERT_EQUAL_INT32( 0,          s32_ashr( INT32_MAX,  31));
    TEST_ASSERT_EQUAL_INT32(-1,          s32_ashr( INT32_MIN,  31));

    // shr > 31: large positive shift
    TEST_ASSERT_EQUAL_INT32( 0,          s32_ashr( INT32_MAX,  32));
    TEST_ASSERT_EQUAL_INT32( 0,          s32_ashr( 1,          32));
    TEST_ASSERT_EQUAL_INT32((int32_t)0xffffffff, s32_ashr( INT32_MIN,  32));
    TEST_ASSERT_EQUAL_INT32( 0,          s32_ashr( 0,         100));

    // shr = -1: left shift by 1 — saturates for INT32_MAX/MIN
    TEST_ASSERT_EQUAL_INT32( INT32_MAX,  s32_ashr( INT32_MAX, -1));
    TEST_ASSERT_EQUAL_INT32( INT32_MIN,  s32_ashr( INT32_MIN, -1));
    TEST_ASSERT_EQUAL_INT32( 2,          s32_ashr( 1,         -1));
    TEST_ASSERT_EQUAL_INT32(-2,          s32_ashr(-1,         -1));

    // shr = -31: 1 << 31 = 2^31 overflows INT32_MAX (2^31-1), saturates upward
    TEST_ASSERT_EQUAL_INT32( INT32_MAX,  s32_ashr( 1,        -31));
    TEST_ASSERT_EQUAL_INT32( INT32_MAX,  s32_ashr( INT32_MAX,-31));
    TEST_ASSERT_EQUAL_INT32( INT32_MIN,  s32_ashr( INT32_MIN,-31));

    // shr < -31: saturates to INT32_MAX/MIN unless x == 0
    TEST_ASSERT_EQUAL_INT32( 0,          s32_ashr( 0,        -32));
    TEST_ASSERT_EQUAL_INT32( INT32_MAX,  s32_ashr( 1,        -32));
    TEST_ASSERT_EQUAL_INT32( INT32_MIN,  s32_ashr(-1,        -32));
    TEST_ASSERT_EQUAL_INT32( INT32_MAX,  s32_ashr( INT32_MAX,-100));
    TEST_ASSERT_EQUAL_INT32( INT32_MIN,  s32_ashr( INT32_MIN,-100));
}

// Reference implementation used to compute expected values
static int16_t ref_s16_ashr(int16_t x, right_shift_t shr)
{
    if (shr > 15) {
        return (x >= 0) ? 0 : (int16_t)-1;
    } else if (shr >= 0) {
        return x >> shr;
    } else if (shr < -15) {
        return (x == 0) ? 0 : (x > 0) ? INT16_MAX : INT16_MIN;
    } else {
        int32_t tmp = ((int32_t)x) << -shr;
        return (tmp > INT16_MAX) ? INT16_MAX : (tmp < INT16_MIN) ? INT16_MIN : (int16_t)tmp;
    }
}

static void s16_test(unsigned seed, right_shift_t shr)
{
    int16_t x = (int16_t) pseudo_rand_int32(&seed);
    int16_t expected = ref_s16_ashr(x, shr);
    int16_t actual   = s16_ashr(x, shr);

    TEST_ASSERT_EQUAL_INT16(expected, actual);
}

// Arithmetic right shift: shr in [0, 24], verifying sign-extension and large shifts
TEST(sXX_ashr, s16_ashr_right)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    for (unsigned v = 0; v < REPS; v++) {
        setExtraInfo_RS(v, seed);

        right_shift_t shr = (right_shift_t) pseudo_rand_uint(&seed, 0, 24);
        s16_test(seed, shr);
    }
}

// Left-shifting with saturation: shr in [-24, -1]
TEST(sXX_ashr, s16_ashr_left)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    for (unsigned v = 0; v < REPS; v++) {
        setExtraInfo_RS(v, seed);

        right_shift_t shr = -((right_shift_t) pseudo_rand_uint(&seed, 1, 24));
        s16_test(seed, shr);
    }
}

// Boundary and corner cases
TEST(sXX_ashr, s16_ashr_boundary)
{
    // shr = 0: identity
    TEST_ASSERT_EQUAL_INT16( 0,          s16_ashr( 0,          0));
    TEST_ASSERT_EQUAL_INT16( INT16_MAX,  s16_ashr( INT16_MAX,  0));
    TEST_ASSERT_EQUAL_INT16( INT16_MIN,  s16_ashr( INT16_MIN,  0));

    // shr = 1: simple halving
    TEST_ASSERT_EQUAL_INT16( 1,          s16_ashr( 3,          1));
    TEST_ASSERT_EQUAL_INT16(-1,          s16_ashr(-1,          1));

    // shr = 15: only sign bit should remain
    TEST_ASSERT_EQUAL_INT16( 0,          s16_ashr( INT16_MAX,  15));
    TEST_ASSERT_EQUAL_INT16(-1,          s16_ashr( INT16_MIN,  15));

    // shr > 15: large positive shift
    TEST_ASSERT_EQUAL_INT16( 0,          s16_ashr( INT16_MAX,  16));
    TEST_ASSERT_EQUAL_INT16( 0,          s16_ashr( 1,          16));
    TEST_ASSERT_EQUAL_INT16(-1,          s16_ashr( INT16_MIN,  16));
    TEST_ASSERT_EQUAL_INT16( 0,          s16_ashr( 0,         100));

    // shr = -1: left shift by 1 — saturates for INT16_MAX/MIN
    TEST_ASSERT_EQUAL_INT16( INT16_MAX,  s16_ashr( INT16_MAX, -1));
    TEST_ASSERT_EQUAL_INT16( INT16_MIN,  s16_ashr( INT16_MIN, -1));
    TEST_ASSERT_EQUAL_INT16( 2,          s16_ashr( 1,         -1));
    TEST_ASSERT_EQUAL_INT16(-2,          s16_ashr(-1,         -1));

    // shr = -15: 1 << 15 = 32768 overflows INT16_MAX (32767), saturates upward
    TEST_ASSERT_EQUAL_INT16( INT16_MAX,  s16_ashr( 1,        -15));
    TEST_ASSERT_EQUAL_INT16( INT16_MIN,  s16_ashr(-1,        -15));  // -1 << 15 = INT16_MIN exactly
    TEST_ASSERT_EQUAL_INT16( INT16_MAX,  s16_ashr( INT16_MAX,-15));
    TEST_ASSERT_EQUAL_INT16( INT16_MIN,  s16_ashr( INT16_MIN,-15));

    // shr < -15: saturates to INT16_MAX/MIN unless x == 0
    TEST_ASSERT_EQUAL_INT16( 0,          s16_ashr( 0,        -16));
    TEST_ASSERT_EQUAL_INT16( INT16_MAX,  s16_ashr( 1,        -16));
    TEST_ASSERT_EQUAL_INT16( INT16_MIN,  s16_ashr(-1,        -16));
    TEST_ASSERT_EQUAL_INT16( INT16_MAX,  s16_ashr( INT16_MAX,-100));
    TEST_ASSERT_EQUAL_INT16( INT16_MIN,  s16_ashr( INT16_MIN,-100));
}
