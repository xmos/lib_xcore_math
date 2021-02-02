// Copyright 2021 XMOS LIMITED. This Software is subject to the terms of the 
// XMOS Public License: Version 1

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "bfp_math.h"

#include "../tst_common.h"

#include "unity.h"

#if DEBUG_ON || 0
#undef DEBUG_ON
#define DEBUG_ON    (1)
#endif


#define REPS        1000
#define MAX_LEN     1024 


static unsigned seed = 666;


static void test_bfp_s16_energy()
{
    PRINTF("%s...\t(random vectors)\n", __func__);

    seed = 8566;

    int16_t dataB[MAX_LEN];
    bfp_s16_t B;

    B.data = dataB;

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        B.length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        B.exp = pseudo_rand_int(&seed, -5, 5);
        B.hr = pseudo_rand_uint(&seed, 0, 15);

        int64_t sum64 = 0;

        for(int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int16(&seed) >> B.hr;

            sum64 += ((int32_t)B.data[i]) * B.data[i];
        }

        bfp_s16_headroom(&B);

        float_s64_t result = bfp_s16_energy(&B);

        double expf = ldexp(sum64, 2*B.exp);
        double resf = ldexp(result.mant, result.exp);

        TEST_ASSERT(expf == resf);
    }
}



static void test_bfp_s32_energy_B()
{
    PRINTF("%s...\t(random vectors)\n", __func__);

    unsigned seed = 0x648E34A3;

    int32_t dataB[MAX_LEN];
    bfp_s32_t B;

    B.data = dataB;

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        bfp_s32_init(&B, dataB, pseudo_rand_int(&seed, -5, 5),
                            pseudo_rand_uint(&seed, 1, MAX_LEN+1), 0);

        B.hr = pseudo_rand_uint(&seed, 0, 28);

        double expected = 0;

        for(int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int32(&seed) >> B.hr;

            expected += pow(ldexp(B.data[i], B.exp), 2);
        }

        bfp_s32_headroom(&B);
        
        float_s64_t result = bfp_s32_energy(&B);

        double diff = expected-ldexp(result.mant, result.exp);
        double error = fabs(diff/expected);

        TEST_ASSERT( error < ldexp(1,-20) );
    }
}






void test_bfp_energy()
{
    SET_TEST_FILE();
    RUN_TEST(test_bfp_s16_energy);
    RUN_TEST(test_bfp_s32_energy_B);
}