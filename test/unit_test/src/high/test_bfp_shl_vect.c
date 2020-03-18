
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


#define REPS        IF_QUICK_TEST(10, 50)
#define MAX_LEN     IF_QUICK_TEST(32, 64)


static unsigned seed = 54346;




static void test_bfp_shl_vect_s16()
{
    PRINTF("%s...\n", __func__);

    seed = 0x038D7C77;

    int16_t dataA[MAX_LEN];
    int16_t dataB[MAX_LEN];
    int16_t dataB_copy[MAX_LEN];
    bfp_s16_t A, B, B_copy;
    A.data = dataA;
    B.data = dataB;

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        test_random_bfp_s16(&B, MAX_LEN, &seed, &A, 0);

        memcpy(&B_copy, &B, sizeof(B));
        memcpy(dataB_copy, dataB, sizeof(dataB));

        unsigned leave_hr = 1;

        bfp_shl_vect_s16(&A, &B, B.hr-leave_hr);

        TEST_ASSERT_EQUAL(B_copy.length, A.length);
        TEST_ASSERT_EQUAL(B_copy.length, B.length);
        TEST_ASSERT_EQUAL(leave_hr, A.hr);
        TEST_ASSERT_EQUAL(B_copy.hr, B.hr);
        TEST_ASSERT_EQUAL(B_copy.exp, A.exp);
        TEST_ASSERT_EQUAL(B_copy.exp, B.exp);

        for(int i = 0; i < A.length; i++)
            TEST_ASSERT_EQUAL(B_copy.data[i] << (B_copy.hr-leave_hr), A.data[i]);

        int shl = -(pseudo_rand_uint32(&seed) % 5) - 1;

        memcpy(&B_copy, &A, sizeof(A));
        memcpy(dataB_copy, dataA, sizeof(dataA));

        bfp_shl_vect_s16(&B, &A,  shl);
        
        TEST_ASSERT_EQUAL(B_copy.length, B.length);
        TEST_ASSERT_EQUAL(B_copy.length, A.length);
        TEST_ASSERT_EQUAL(-shl+leave_hr, B.hr);
        TEST_ASSERT_EQUAL(leave_hr, A.hr);
        TEST_ASSERT_EQUAL(B_copy.exp, B.exp);
        TEST_ASSERT_EQUAL(B_copy.exp, A.exp);

        for(int i = 0; i < A.length; i++)
            TEST_ASSERT_EQUAL(B_copy.data[i] >> (-shl), B.data[i]);
    }
}



static void test_bfp_shl_vect_s32()
{
    PRINTF("%s...\n", __func__);

    seed = 373456;

    int32_t dataA[MAX_LEN];
    int32_t dataB[MAX_LEN];
    int32_t dataB_copy[MAX_LEN];
    bfp_s32_t A, B, B_copy;
    A.data = dataA;
    B.data = dataB;

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        test_random_bfp_s32(&B, MAX_LEN, &seed, &A, 0);

        memcpy(&B_copy, &B, sizeof(B));
        memcpy(dataB_copy, dataB, sizeof(dataB));

        unsigned leave_hr = 1;

        bfp_shl_vect_s32(&A, &B, B.hr-leave_hr);

        TEST_ASSERT_EQUAL(B_copy.length, A.length);
        TEST_ASSERT_EQUAL(B_copy.length, B.length);
        TEST_ASSERT_EQUAL(leave_hr, A.hr);
        TEST_ASSERT_EQUAL(B_copy.hr, B.hr);
        TEST_ASSERT_EQUAL(B_copy.exp, A.exp);
        TEST_ASSERT_EQUAL(B_copy.exp, B.exp);

        for(int i = 0; i < A.length; i++)
            TEST_ASSERT_EQUAL(B_copy.data[i] << (B_copy.hr-leave_hr), A.data[i]);

        int shl = -(pseudo_rand_uint32(&seed) % 5) - 1;

        memcpy(&B_copy, &A, sizeof(A));
        memcpy(dataB_copy, dataA, sizeof(dataA));

        bfp_shl_vect_s32(&B, &A, shl);
        
        TEST_ASSERT_EQUAL(B_copy.length, B.length);
        TEST_ASSERT_EQUAL(B_copy.length, A.length);
        TEST_ASSERT_EQUAL(-shl+leave_hr, B.hr);
        TEST_ASSERT_EQUAL(leave_hr, A.hr);
        TEST_ASSERT_EQUAL(B_copy.exp, B.exp);
        TEST_ASSERT_EQUAL(B_copy.exp, A.exp);

        for(int i = 0; i < A.length; i++)
            TEST_ASSERT_EQUAL(B_copy.data[i] >> (-shl), B.data[i]);
    }
}





static void test_bfp_shl_vect_complex_s16()
{
    PRINTF("%s...\n", __func__);

    seed = 66777245;

    int16_t realA[MAX_LEN];
    int16_t imagA[MAX_LEN];
    int16_t realB[MAX_LEN];
    int16_t imagB[MAX_LEN];
    int16_t realB_copy[MAX_LEN];
    int16_t imagB_copy[MAX_LEN];
    bfp_complex_s16_t A, B, B_copy;
    A.real = realA;
    A.imag = imagA;
    B.real = realB;
    B.imag = imagB;
    B_copy.real = realB_copy;
    B_copy.imag = imagB_copy;

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        test_random_bfp_complex_s16(&B, MAX_LEN, &seed, &A, 0);

        memcpy(&B_copy, &B, sizeof(B));
        memcpy(realB_copy, realB, sizeof(realB));
        memcpy(imagB_copy, imagB, sizeof(imagB));

        unsigned leave_hr = 1;

        bfp_shl_vect_complex_s16(&A, &B, B.hr-leave_hr);

        TEST_ASSERT_EQUAL(B_copy.length, A.length);
        TEST_ASSERT_EQUAL(B_copy.length, B.length);
        TEST_ASSERT_EQUAL(leave_hr, A.hr);
        TEST_ASSERT_EQUAL(B_copy.hr, B.hr);
        TEST_ASSERT_EQUAL(B_copy.exp, A.exp);
        TEST_ASSERT_EQUAL(B_copy.exp, B.exp);

        for(int i = 0; i < A.length; i++){
            TEST_ASSERT_EQUAL(B_copy.real[i] << (B_copy.hr-leave_hr), A.real[i]);
            TEST_ASSERT_EQUAL(B_copy.imag[i] << (B_copy.hr-leave_hr), A.imag[i]);
        }

        int shl = -(pseudo_rand_uint32(&seed) % 5) - 1;

        memcpy(&B_copy, &A, sizeof(A));
        memcpy(realB_copy, realA, sizeof(realA));
        memcpy(imagB_copy, imagA, sizeof(imagA));

        bfp_shl_vect_complex_s16(&B, &A, shl);
        
        TEST_ASSERT_EQUAL(B_copy.length, B.length);
        TEST_ASSERT_EQUAL(B_copy.length, A.length);
        TEST_ASSERT_EQUAL(-shl+leave_hr, B.hr);
        TEST_ASSERT_EQUAL(leave_hr, A.hr);
        TEST_ASSERT_EQUAL(B_copy.exp, B.exp);
        TEST_ASSERT_EQUAL(B_copy.exp, A.exp);

        for(int i = 0; i < A.length; i++){
            TEST_ASSERT_EQUAL(B_copy.real[i] >> (-shl), B.real[i]);
            TEST_ASSERT_EQUAL(B_copy.imag[i] >> (-shl), B.imag[i]);
        }
    }
}





static void test_bfp_shl_vect_complex_s32()
{
    PRINTF("%s...\n", __func__);

    seed = 85678488;

    complex_s32_t dataA[MAX_LEN];
    complex_s32_t dataB[MAX_LEN];
    complex_s32_t dataB_copy[MAX_LEN];
    bfp_complex_s32_t A, B, B_copy;
    A.data = dataA;
    B.data = dataB;

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        test_random_bfp_complex_s32(&B, MAX_LEN, &seed, &A, 0);

        memcpy(&B_copy, &B, sizeof(B));
        memcpy(dataB_copy, dataB, sizeof(dataB));

        unsigned leave_hr = 1;

        bfp_shl_vect_complex_s32(&A, &B, B.hr-leave_hr);

        TEST_ASSERT_EQUAL(B_copy.length, A.length);
        TEST_ASSERT_EQUAL(B_copy.length, B.length);
        TEST_ASSERT_EQUAL(leave_hr, A.hr);
        TEST_ASSERT_EQUAL(B_copy.hr, B.hr);
        TEST_ASSERT_EQUAL(B_copy.exp, A.exp);
        TEST_ASSERT_EQUAL(B_copy.exp, B.exp);

        for(int i = 0; i < A.length; i++){
            TEST_ASSERT_EQUAL(B_copy.data[i].re << (B_copy.hr-leave_hr), A.data[i].re);
            TEST_ASSERT_EQUAL(B_copy.data[i].im << (B_copy.hr-leave_hr), A.data[i].im);
        }

        int shl = -(pseudo_rand_uint32(&seed) % 5) - 1;

        memcpy(&B_copy, &A, sizeof(A));
        memcpy(dataB_copy, dataA, sizeof(dataA));

        bfp_shl_vect_complex_s32(&B, &A, shl);
        
        TEST_ASSERT_EQUAL(B_copy.length, B.length);
        TEST_ASSERT_EQUAL(B_copy.length, A.length);
        TEST_ASSERT_EQUAL(-shl+leave_hr, B.hr);
        TEST_ASSERT_EQUAL(leave_hr, A.hr);
        TEST_ASSERT_EQUAL(B_copy.exp, B.exp);
        TEST_ASSERT_EQUAL(B_copy.exp, A.exp);

        for(int i = 0; i < A.length; i++){
            TEST_ASSERT_EQUAL(B_copy.data[i].re >> (-shl), B.data[i].re);
            TEST_ASSERT_EQUAL(B_copy.data[i].im >> (-shl), B.data[i].im);
        }
    }
}





static void test_bfp_shl_vect_ch_pair_s16()
{
    PRINTF("%s...\n", __func__);

    seed = 23335;

    ch_pair_s16_t dataA[MAX_LEN];
    ch_pair_s16_t dataB[MAX_LEN];
    ch_pair_s16_t dataB_copy[MAX_LEN];
    bfp_ch_pair_s16_t A, B, B_copy;
    A.data = dataA;
    B.data = dataB;

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        test_random_bfp_ch_pair_s16(&B, MAX_LEN, &seed, &A, 0);

        memcpy(&B_copy, &B, sizeof(B));
        memcpy(dataB_copy, dataB, sizeof(dataB));

        unsigned leave_hr = 1;

        bfp_shl_vect_ch_pair_s16(&A, &B, B.hr-leave_hr);

        TEST_ASSERT_EQUAL(B_copy.length, A.length);
        TEST_ASSERT_EQUAL(B_copy.length, B.length);
        TEST_ASSERT_EQUAL(leave_hr, A.hr);
        TEST_ASSERT_EQUAL(B_copy.hr, B.hr);
        TEST_ASSERT_EQUAL(B_copy.exp, A.exp);
        TEST_ASSERT_EQUAL(B_copy.exp, B.exp);

        for(int i = 0; i < A.length; i++){
            TEST_ASSERT_EQUAL(B_copy.data[i].ch_a << (B_copy.hr-leave_hr), A.data[i].ch_a);
            TEST_ASSERT_EQUAL(B_copy.data[i].ch_b << (B_copy.hr-leave_hr), A.data[i].ch_b);
        }

        int shl = -(pseudo_rand_uint32(&seed) % 5) - 1;

        memcpy(&B_copy, &A, sizeof(A));
        memcpy(dataB_copy, dataA, sizeof(dataA));

        bfp_shl_vect_ch_pair_s16(&B, &A, shl);
        
        TEST_ASSERT_EQUAL(B_copy.length, B.length);
        TEST_ASSERT_EQUAL(B_copy.length, A.length);
        TEST_ASSERT_EQUAL(-shl+leave_hr, B.hr);
        TEST_ASSERT_EQUAL(leave_hr, A.hr);
        TEST_ASSERT_EQUAL(B_copy.exp, B.exp);
        TEST_ASSERT_EQUAL(B_copy.exp, A.exp);

        for(int i = 0; i < A.length; i++){
            TEST_ASSERT_EQUAL(B_copy.data[i].ch_a >> (-shl), B.data[i].ch_a);
            TEST_ASSERT_EQUAL(B_copy.data[i].ch_b >> (-shl), B.data[i].ch_b);
        }
    }
}





static void test_bfp_shl_vect_ch_pair_s32()
{
    PRINTF("%s...\n", __func__);

    seed = 7775533;

    ch_pair_s32_t dataA[MAX_LEN];
    ch_pair_s32_t dataB[MAX_LEN];
    ch_pair_s32_t dataB_copy[MAX_LEN];
    bfp_ch_pair_s32_t A, B, B_copy;
    A.data = dataA;
    B.data = dataB;

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        test_random_bfp_ch_pair_s32(&B, MAX_LEN, &seed, &A, 0);

        memcpy(&B_copy, &B, sizeof(B));
        memcpy(dataB_copy, dataB, sizeof(dataB));

        unsigned leave_hr = 1;

        bfp_shl_vect_ch_pair_s32(&A, &B, B.hr-leave_hr);

        TEST_ASSERT_EQUAL(B_copy.length, A.length);
        TEST_ASSERT_EQUAL(B_copy.length, B.length);
        TEST_ASSERT_EQUAL(leave_hr, A.hr);
        TEST_ASSERT_EQUAL(B_copy.hr, B.hr);
        TEST_ASSERT_EQUAL(B_copy.exp, A.exp);
        TEST_ASSERT_EQUAL(B_copy.exp, B.exp);

        for(int i = 0; i < A.length; i++){
            TEST_ASSERT_EQUAL(B_copy.data[i].ch_a << (B_copy.hr-leave_hr), A.data[i].ch_a);
            TEST_ASSERT_EQUAL(B_copy.data[i].ch_b << (B_copy.hr-leave_hr), A.data[i].ch_b);
        }

        int shl = -(pseudo_rand_uint32(&seed) % 5) - 1;

        memcpy(&B_copy, &A, sizeof(A));
        memcpy(dataB_copy, dataA, sizeof(dataA));

        bfp_shl_vect_ch_pair_s32(&B, &A, shl);
        
        TEST_ASSERT_EQUAL(B_copy.length, B.length);
        TEST_ASSERT_EQUAL(B_copy.length, A.length);
        TEST_ASSERT_EQUAL(-shl+leave_hr, B.hr);
        TEST_ASSERT_EQUAL(leave_hr, A.hr);
        TEST_ASSERT_EQUAL(B_copy.exp, B.exp);
        TEST_ASSERT_EQUAL(B_copy.exp, A.exp);

        for(int i = 0; i < A.length; i++){
            TEST_ASSERT_EQUAL(B_copy.data[i].ch_a >> (-shl), B.data[i].ch_a);
            TEST_ASSERT_EQUAL(B_copy.data[i].ch_b >> (-shl), B.data[i].ch_b);
        }
    }
}




int test_bfp_ldexp_vect()
{
    int retval = 0;
    UnityBegin(__FILE__);

    RUN_TEST(test_bfp_shl_vect_s16);
    RUN_TEST(test_bfp_shl_vect_s32);
    RUN_TEST(test_bfp_shl_vect_complex_s16);
    RUN_TEST(test_bfp_shl_vect_complex_s32);
    RUN_TEST(test_bfp_shl_vect_ch_pair_s16);
    RUN_TEST(test_bfp_shl_vect_ch_pair_s32);

    retval = UnityEnd();
    return retval;
}