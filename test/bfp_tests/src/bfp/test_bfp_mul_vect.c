// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

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
#define MAX_LEN     256


static unsigned seed = 666;


static char msg_buff[200];

#define TEST_ASSERT_EQUAL_MSG(EXPECTED, ACTUAL, EXTRA, LINE_NUM)   do{          \
    if((EXPECTED)!=(ACTUAL)) {                                                  \
      sprintf(msg_buff, "%s (test vector @ line %u)", (EXTRA), (LINE_NUM));     \
      TEST_ASSERT_EQUAL_MESSAGE((EXPECTED), (ACTUAL), msg_buff);                \
    }} while(0)




static void test_bfp_s16_mul()
{
    PRINTF("%s...\n", __func__);

    seed = 0x58E99770;

    int16_t dataA[MAX_LEN];
    int16_t dataB[MAX_LEN];
    int16_t dataC[MAX_LEN];
    int16_t expA[MAX_LEN];
    bfp_s16_t A, B, C;

    A.data = dataA;
    B.data = dataB;
    C.data = dataC;

    double Af[MAX_LEN];
    double Bf[MAX_LEN];
    double Cf[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        test_random_bfp_s16(&B, MAX_LEN, &seed, &A, 0);
        test_random_bfp_s16(&C, MAX_LEN, &seed, &A, B.length);

        test_double_from_s16(Bf, &B);
        test_double_from_s16(Cf, &C);

        for(int i = 0; i < B.length; i++){
            Af[i] = Bf[i] * Cf[i];
        }

        bfp_s16_mul(&A, &B, &C);

        test_s16_from_double(expA, Af, MAX_LEN, A.exp);

        for(int i = 0; i < A.length; i++){
            // PRINTF("! %08X\t%08X\n", (unsigned) expA[i], (unsigned) A.data[i]);
            TEST_ASSERT_INT16_WITHIN(1, expA[i], A.data[i]);
        }
    }
}


static void test_bfp_s32_mul()
{
    PRINTF("%s...\n", __func__);

    seed = 1123441;

    int32_t dataA[MAX_LEN];
    int32_t dataB[MAX_LEN];
    int32_t dataC[MAX_LEN];
    int32_t expA[MAX_LEN];
    bfp_s32_t A, B, C;

    A.data = dataA;
    B.data = dataB;
    C.data = dataC;

    double Af[MAX_LEN];
    double Bf[MAX_LEN];
    double Cf[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        test_random_bfp_s32(&B, MAX_LEN, &seed, &A, 0);
        test_random_bfp_s32(&C, MAX_LEN, &seed, &A, B.length);

        //Just to make the test easier.
        for(int i = 0; i < B.length; i++){
            B.data[i] = B.data[i] & 0xFFFFFFFE;
            C.data[i] = C.data[i] & 0xFFFFFFFE;
        }

        test_double_from_s32(Bf, &B);
        test_double_from_s32(Cf, &C);

        for(int i = 0; i < B.length; i++){
            Af[i] = Bf[i] * Cf[i];
        }

        bfp_s32_mul(&A, &B, &C);

        test_s32_from_double(expA, Af, MAX_LEN, A.exp);

        for(int i = 0; i < A.length; i++){
            // PRINTF("! %08X\t%08X\n", (unsigned) expA[i], (unsigned) A.data[i]);
            TEST_ASSERT_INT32_WITHIN(1, expA[i], A.data[i]);
        }
    }
}



static void test_bfp_s16_scale()
{
    PRINTF("%s...\n", __func__);

    seed = 6262343;

    int16_t WORD_ALIGNED dataA[MAX_LEN];
    int16_t WORD_ALIGNED dataB[MAX_LEN];
    int16_t WORD_ALIGNED expA[MAX_LEN];
    bfp_s16_t A, B;

    A.data = dataA;
    B.data = dataB;

    double Af[MAX_LEN];
    double Bf[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        test_random_bfp_s16(&B, MAX_LEN, &seed, &A, 0);
        int16_t alpha_mant = pseudo_rand_int16(&seed);
        exponent_t alpha_exp = pseudo_rand_int16(&seed) % 30;

        test_double_from_s16(Bf, &B);
        double alpha_f = ldexp(alpha_mant, alpha_exp);

        for(int i = 0; i < B.length; i++){
            Af[i] = Bf[i] * alpha_f;
        }

        float_s16_t alpha = {alpha_mant, alpha_exp};
        
        bfp_s16_scale(&A, &B, alpha);

        test_s16_from_double(expA, Af, MAX_LEN, A.exp);

        for(int i = 0; i < A.length; i++){
            TEST_ASSERT_INT16_WITHIN(1, expA[i], A.data[i]);
        }
    }
}




static void test_bfp_s32_scale()
{
    PRINTF("%s...\n", __func__);

    seed = 0x64ED8AA2;

    int32_t dataA[MAX_LEN];
    int32_t dataB[MAX_LEN];
    int32_t expA[MAX_LEN];
    bfp_s32_t A, B;

    A.data = dataA;
    B.data = dataB;

    double Af[MAX_LEN];
    double Bf[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        test_random_bfp_s32(&B, MAX_LEN, &seed, &A, 0);
        int32_t alpha_mant = pseudo_rand_int32(&seed);
        exponent_t alpha_exp = pseudo_rand_int32(&seed) % 30;

        test_double_from_s32(Bf, &B);
        double alpha_f = ldexp(alpha_mant, alpha_exp);

        for(int i = 0; i < B.length; i++){
            Af[i] = Bf[i] * alpha_f;
        }

        float_s32_t alpha = {alpha_mant, alpha_exp};

        bfp_s32_scale(&A, &B, alpha);

        test_s32_from_double(expA, Af, MAX_LEN, A.exp);

        for(int i = 0; i < A.length; i++){
            TEST_ASSERT_INT32_WITHIN(1, expA[i], A.data[i]);
        }
    }
}





void test_bfp_mul_vect()
{
    SET_TEST_FILE();
    RUN_TEST(test_bfp_s16_mul);
    RUN_TEST(test_bfp_s32_mul);
    

    RUN_TEST(test_bfp_s16_scale);
    RUN_TEST(test_bfp_s32_scale);
}