// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xmath/xmath.h"

#include "../../tst_common.h"

#include "unity_fixture.h"


TEST_GROUP_RUNNER(bfp_add_scalar) {
  RUN_TEST_CASE(bfp_add_scalar, bfp_s16_add_scalar);
  RUN_TEST_CASE(bfp_add_scalar, bfp_s32_add_scalar);
}

TEST_GROUP(bfp_add_scalar);
TEST_SETUP(bfp_add_scalar) { fflush(stdout); }
TEST_TEAR_DOWN(bfp_add_scalar) {}

#if SMOKE_TEST
#  define REPS       (100)
#else
# define REPS        (1000)
#endif

#define MAX_LEN     18  //Smaller lengths mean larger variance w.r.t. individual element headroom

// static char msg_buff[200];

#define TEST_ASSERT_EQUAL_MSG(EXPECTED, ACTUAL, EXTRA, LINE_NUM)   do{          \
    if((EXPECTED)!=(ACTUAL)) {                                                  \
      sprintf(msg_buff, "%s (test vector @ line %u)", (EXTRA), (LINE_NUM));     \
      TEST_ASSERT_EQUAL_MESSAGE((EXPECTED), (ACTUAL), msg_buff);                \
    }} while(0)


TEST(bfp_add_scalar, bfp_s16_add_scalar)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    int16_t dataA[MAX_LEN];
    int16_t dataB[MAX_LEN];
    int16_t expA[MAX_LEN];
    bfp_s16_t A, B;

    A.data = dataA;
    B.data = dataB;

    float C;

    double Af[MAX_LEN];
    double Bf[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        const unsigned old_seed = seed;

        test_random_bfp_s16(&B, MAX_LEN, &seed, &A, 0);

        setExtraInfo_RSL(r, old_seed, B.length);

        C = ldexpf(pseudo_rand_int16(&seed), B.exp + pseudo_rand_int(&seed, -2, 3));

        test_double_from_s16(Bf, &B);

        for(unsigned int i = 0; i < B.length; i++){
            Af[i] = Bf[i] + C;
        }

        bfp_s16_add_scalar(&A, &B, C);

        test_s16_from_double(expA, Af, MAX_LEN, A.exp);

        XTEST_ASSERT_VECT_S16_WITHIN(2, expA, A.data, A.length,
            "B.hr = %u\n"
            "Expected: %d * 2^%d <-- (%d * 2^%d) + %e\n"
            "Actual: %d * 2^%d\n",
            B.hr, expA[i], A.exp, B.data[i], B.exp,
            C, A.data[i], A.exp);
    }
}


TEST(bfp_add_scalar, bfp_s32_add_scalar)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    int32_t dataA[MAX_LEN];
    int32_t dataB[MAX_LEN];
    int32_t expA[MAX_LEN];
    bfp_s32_t A, B;

    A.data = dataA;
    B.data = dataB;

    float_s32_t C;

    double Af[MAX_LEN];
    double Bf[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        test_random_bfp_s32(&B, MAX_LEN, &seed, &A, 0);

        C.exp = B.exp + pseudo_rand_int(&seed, -2, 3);
        C.mant = pseudo_rand_int32(&seed);

        double Cf = float_s32_to_double(C);

        test_double_from_s32(Bf, &B);

        for(unsigned int i = 0; i < B.length; i++){
            Af[i] = Bf[i] + Cf;
        }

        bfp_s32_add_scalar(&A, &B, C);

        test_s32_from_double(expA, Af, MAX_LEN, A.exp);

        for(unsigned int i = 0; i < A.length; i++){
            TEST_ASSERT_INT32_WITHIN(2, expA[i], A.data[i]);
        }
    }
}


