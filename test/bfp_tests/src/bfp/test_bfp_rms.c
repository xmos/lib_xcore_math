
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


#define REPS        100
#define MAX_LEN     100


static unsigned seed;


static void test_bfp_s16_rms()
{
    PRINTF("%s...\t(random vectors)\n", __func__);

    seed = 0x042EAD51;

    int16_t WORD_ALIGNED dataB[MAX_LEN];
    bfp_s16_t B;

    B.data = dataB;

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        bfp_s16_t B;

        bfp_s16_init(&B, dataB,
                          pseudo_rand_int(&seed, -30, 30),
                          pseudo_rand_uint(&seed, 1, MAX_LEN+1), 0);
                          
        B.hr = pseudo_rand_uint(&seed, 0, 12);

        for(int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int16(&seed) >> B.hr;
        }

        bfp_s16_headroom(&B);

        exponent_t a_exp;

        int32_t result = bfp_s16_rms(&a_exp, &B);

        double energy = 0;
        for(int i = 0; i < B.length; i++){
            energy += pow(ldexp(B.data[i], B.exp), 2);
        }

        const double mean_energy = energy / B.length;

        const double expectedF = sqrt(mean_energy);

        const exponent_t ideal_exponent = floor( log2(expectedF) ) - 30;

        // printf("B.length = %u\n", B.length);
        // printf("B.exp = %d\n", B.exp);
        // printf("B.hr = %u\n", B.hr);

        // for(int i = 0; i < B.length; i++){
        //     printf("  B.data[%d] = %d\n", i, B.data[i]);
        // }
        // printf("result = %ld\n", result);
        // printf("a_exp = %d\n", a_exp);

        // printf("energy = %e\n", energy);
        // printf("mean_energy = %e\n", mean_energy);
        // printf("expectedF = %e\n", expectedF);
        // printf("ideal_exponent = %d\n", ideal_exponent);
        // printf("ldexp(result, a_exp) = %e\n", ldexp(result, a_exp));

        TEST_ASSERT_INT32_WITHIN(3, ideal_exponent, a_exp);
        

        const int32_t ideal_result = round( expectedF / ldexp(1,a_exp) );

        TEST_ASSERT_INT32_WITHIN(3, ideal_result, result);
    }
}


static void test_bfp_s32_rms()
{
    PRINTF("%s...\t(random vectors)\n", __func__);

    seed = 0x3E71643B;

    int32_t dataB[MAX_LEN];
    bfp_s32_t B;

    B.data = dataB;

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        bfp_s32_t B;

        bfp_s32_init(&B, dataB,
                          pseudo_rand_int(&seed, -30, 30),
                          pseudo_rand_uint(&seed, 1, MAX_LEN+1), 0);
                          
        B.hr = pseudo_rand_uint(&seed, 0, 28);

        for(int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int32(&seed) >> B.hr;
        }

        bfp_s32_headroom(&B);

        exponent_t a_exp;

        int32_t result = bfp_s32_rms(&a_exp, &B);

        double energy = 0;
        for(int i = 0; i < B.length; i++){
            energy += pow(ldexp(B.data[i], B.exp), 2);
        }

        const double mean_energy = energy / B.length;

        const double expectedF = sqrt(mean_energy);

        const exponent_t ideal_exponent = floor( log2(expectedF) ) - 30;

        // printf("B.length = %u\n", B.length);
        // printf("B.exp = %d\n", B.exp);
        // printf("B.hr = %u\n", B.hr);

        // for(int i = 0; i < B.length; i++){
        //     printf("  B.data[%d] = %d\n", i, B.data[i]);
        // }
        // printf("result = %ld\n", result);
        // printf("a_exp = %d\n", a_exp);

        // printf("energy = %e\n", energy);
        // printf("mean_energy = %e\n", mean_energy);
        // printf("expectedF = %e\n", expectedF);
        // printf("ideal_exponent = %d\n", ideal_exponent);
        // printf("ldexp(result, a_exp) = %e\n", ldexp(result, a_exp));

        TEST_ASSERT_INT32_WITHIN(3, ideal_exponent, a_exp);
        

        const int32_t ideal_result = round( expectedF / ldexp(1,a_exp) );

        TEST_ASSERT_INT32_WITHIN(3, ideal_result, result);
    }
}







void test_bfp_rms()
{
    SET_TEST_FILE();
    RUN_TEST(test_bfp_s16_rms);
    RUN_TEST(test_bfp_s32_rms);
}