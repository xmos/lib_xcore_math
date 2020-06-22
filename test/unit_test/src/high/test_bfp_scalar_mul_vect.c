
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xs3_math.h"

#include "../tst_common.h"

#include "unity.h"

#if DEBUG_ON || 0
#undef DEBUG_ON
#define DEBUG_ON    (1)
#endif


#define REPS        IF_QUICK_TEST(100, 100)
#define MAX_LEN     38 


static unsigned seed = 666;





static void test_bfp_scalar_mul_vect_s16()
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
        
        bfp_scalar_mul_vect_s16(&A, &B, alpha_mant, alpha_exp);

        test_s16_from_double(expA, Af, MAX_LEN, A.exp);

        for(int i = 0; i < A.length; i++){
            TEST_ASSERT_INT16_WITHIN(1, expA[i], A.data[i]);
        }
    }
}




static void test_bfp_scalar_mul_vect_s32()
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

        bfp_scalar_mul_vect_s32(&A, &B, alpha_mant, alpha_exp);

        test_s32_from_double(expA, Af, MAX_LEN, A.exp);

        for(int i = 0; i < A.length; i++){
            TEST_ASSERT_INT32_WITHIN(1, expA[i], A.data[i]);
        }
    }
}




void test_bfp_scalar_mul_vect()
{
    SET_TEST_FILE();

    RUN_TEST(test_bfp_scalar_mul_vect_s16);
    RUN_TEST(test_bfp_scalar_mul_vect_s32);
}