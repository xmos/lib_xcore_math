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


TEST_GROUP_RUNNER(bfp_energy) {
  RUN_TEST_CASE(bfp_energy, bfp_s16_energy);
  RUN_TEST_CASE(bfp_energy, bfp_s32_energy);
}

TEST_GROUP(bfp_energy);
TEST_SETUP(bfp_energy) { fflush(stdout); }
TEST_TEAR_DOWN(bfp_energy) {}

#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (128)
#else
#  define REPS       (1000)
#  define MAX_LEN    (512)
#endif


TEST(bfp_energy, bfp_s16_energy)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    int16_t dataB[MAX_LEN];
    bfp_s16_t B;

    B.data = dataB;

    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        B.length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        B.exp = pseudo_rand_int(&seed, -5, 5);
        B.hr = pseudo_rand_uint(&seed, 0, 15);

        int64_t sum64 = 0;

        for(unsigned int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int16(&seed) >> B.hr;

            sum64 += ((int32_t)B.data[i]) * B.data[i];
        }

        bfp_s16_headroom(&B);

        float_s64_t result = bfp_s16_energy(&B);

        double expf = ldexp((double) sum64, 2*B.exp);
        double resf = ldexp((double) result.mant, result.exp);

        TEST_ASSERT(expf == resf);
    }
}


TEST(bfp_energy, bfp_s32_energy)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    int32_t dataB[MAX_LEN];
    bfp_s32_t B;

    B.data = dataB;

    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        bfp_s32_init(&B, dataB, pseudo_rand_int(&seed, -5, 5),
                            pseudo_rand_uint(&seed, 1, MAX_LEN+1), 0);

        B.hr = pseudo_rand_uint(&seed, 0, 28);

        double expected = 0;

        for(unsigned int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int32(&seed) >> B.hr;

            expected += pow(ldexp(B.data[i], B.exp), 2);
        }

        bfp_s32_headroom(&B);
        
        float_s64_t result = bfp_s32_energy(&B);

        double diff = expected-ldexp((double) result.mant, result.exp);
        double error = fabs(diff/expected);

        TEST_ASSERT( error < ldexp(1,-20) );
    }
}

