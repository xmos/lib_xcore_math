
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
#define MAX_LEN     1 


static unsigned seed = 666;


static char msg_buff[200];

#define TEST_ASSERT_EQUAL_MSG(EXPECTED, ACTUAL, EXTRA, LINE_NUM)   do{          \
    if((EXPECTED)!=(ACTUAL)) {                                                  \
      sprintf(msg_buff, "%s (test vector @ line %u)", (EXTRA), (LINE_NUM));     \
      TEST_ASSERT_EQUAL_MESSAGE((EXPECTED), (ACTUAL), msg_buff);                \
    }} while(0)


static void test_bfp_mul_vect_s16_calc_params()
{
    PRINTF("%s...\n", __func__);

    seed = 88845643;

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);
        
        exponent_t b_exp = ((int32_t)(pseudo_rand_uint32(&seed) % 60)) - 30;
        headroom_t b_hr  = pseudo_rand_uint32(&seed) % 16;
        exponent_t c_exp = ((int32_t)(pseudo_rand_uint32(&seed) % 60)) - 30;
        headroom_t c_hr  = pseudo_rand_uint32(&seed) % 16;

        exponent_t a_exp;
        right_shift_t a_shr;

        bfp_mul_vect_s16_calc_params(&a_exp, &a_shr, b_exp, c_exp, b_hr, c_hr);

        // a_exp and a_shr should be calculated for exactly this case
        int16_t WORD_ALIGNED B = (-0x8000) >> b_hr;
        int16_t WORD_ALIGNED C = (-0x8000) >> c_hr;
        int16_t WORD_ALIGNED A;

        xs3_mul_vect_s16(&A, &B, &C, 1, a_shr);

        double q = ldexp(B, b_exp) * ldexp(C, c_exp);
        double p = ldexp(A, a_exp);

        TEST_ASSERT_EQUAL_MSG(a_shr > 0? 0x4000 : B*C, A, "A is wrong", r);
        TEST_ASSERT_EQUAL(q, p);
    }
}


static void test_bfp_mul_vect_s16()
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

        bfp_mul_vect_s16(&A, &B, &C);

        test_s16_from_double(expA, Af, MAX_LEN, A.exp);

        for(int i = 0; i < A.length; i++){
            // PRINTF("! %08X\t%08X\n", (unsigned) expA[i], (unsigned) A.data[i]);
            TEST_ASSERT_INT16_WITHIN(1, expA[i], A.data[i]);
        }
    }
}



static void test_bfp_mul_vect_s32_calc_params()
{
    PRINTF("%s...\n", __func__);

    seed = 0xF100319D;

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);
        
        exponent_t b_exp = ((int32_t)(pseudo_rand_uint32(&seed) % 60)) - 30;
        headroom_t b_hr  = pseudo_rand_uint32(&seed) % 31;
        exponent_t c_exp = ((int32_t)(pseudo_rand_uint32(&seed) % 60)) - 30;
        headroom_t c_hr  = pseudo_rand_uint32(&seed) % 31;

        exponent_t a_exp;
        right_shift_t b_shr, c_shr;

        bfp_mul_vect_s32_calc_params(&a_exp, &b_shr, &c_shr, b_exp, c_exp, b_hr, c_hr);

        // a_exp and a_shr should be calculated for exactly this case
        int32_t WORD_ALIGNED B = -0x80000000;
        int32_t WORD_ALIGNED C = -0x80000000;
        B = B >> b_hr;
        C = C >> c_hr;
        int32_t WORD_ALIGNED A;

        xs3_mul_vect_s32(&A, &B, &C, 1, b_shr, c_shr);

        double q = ldexp(B, b_exp) * ldexp(C, c_exp);
        double p = ldexp(A, a_exp);

        TEST_ASSERT_EQUAL_MSG(0x40000000, A, "A is wrong", r);
        TEST_ASSERT_EQUAL(q, p);
    }
}


static void test_bfp_mul_vect_s32()
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

        bfp_mul_vect_s32(&A, &B, &C);

        test_s32_from_double(expA, Af, MAX_LEN, A.exp);

        for(int i = 0; i < A.length; i++){
            // PRINTF("! %08X\t%08X\n", (unsigned) expA[i], (unsigned) A.data[i]);
            TEST_ASSERT_INT32_WITHIN(1, expA[i], A.data[i]);
        }
    }
}




void test_bfp_mul_vect()
{
    SET_TEST_FILE();
    RUN_TEST(test_bfp_mul_vect_s16_calc_params);
    RUN_TEST(test_bfp_mul_vect_s16);

    RUN_TEST(test_bfp_mul_vect_s32_calc_params);
    RUN_TEST(test_bfp_mul_vect_s32);
}