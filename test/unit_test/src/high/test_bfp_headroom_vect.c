
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


#define REPS    IF_QUICK_TEST(10, 50)
#define MAX_LEN IF_QUICK_TEST(100, 300)





static void test_bfp_headroom_vect_s16()
{
    PRINTF("%s...\n", __func__);

    unsigned seed = 769234;
    
    int16_t WORD_ALIGNED data[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep %d..\n", r);

        unsigned length = pseudo_rand_uint32(&seed) % MAX_LEN;
        exponent_t exponent = (pseudo_rand_int32(&seed) % 40) - 20;
        int shr = pseudo_rand_uint32(&seed) % 10;

        for(int i = 0; i < length; i++)
            data[i] = pseudo_rand_int16(&seed) >> shr;
        
        headroom_t exp_hr = xs3_headroom_vect_s16(data, length);

        bfp_s16_t A;

        bfp_init_vect_s16(&A, data, exponent, length, 0);

        TEST_ASSERT_EQUAL(data, A.data);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(0, A.hr);

        headroom_t got_hr = bfp_headroom_vect_s16(&A);

        TEST_ASSERT_EQUAL(data, A.data);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(exp_hr, A.hr);
        TEST_ASSERT_EQUAL(exp_hr, got_hr);
    }
}



static void test_bfp_headroom_vect_s32()
{
    PRINTF("%s...\n", __func__);

    unsigned seed = 56785;
    
    int32_t WORD_ALIGNED data[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep %d..\n", r);

        unsigned length = pseudo_rand_uint32(&seed) % MAX_LEN;
        exponent_t exponent = (pseudo_rand_int32(&seed) % 40) - 20;
        int shr = pseudo_rand_uint32(&seed) % 10;

        for(int i = 0; i < length; i++)
            data[i] = pseudo_rand_int32(&seed) >> shr;
        
        headroom_t exp_hr = xs3_headroom_vect_s32(data, length);

        bfp_s32_t A;

        bfp_init_vect_s32(&A, data, exponent, length, 0);

        TEST_ASSERT_EQUAL(data, A.data);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(0, A.hr);

        headroom_t got_hr = bfp_headroom_vect_s32(&A);

        TEST_ASSERT_EQUAL(data, A.data);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(exp_hr, A.hr);
        TEST_ASSERT_EQUAL(exp_hr, got_hr);
    }
}





static void test_bfp_headroom_vect_complex_s16()
{
    PRINTF("%s...\n", __func__);

    unsigned seed = 7685699;
    
    int16_t WORD_ALIGNED real[MAX_LEN];
    int16_t WORD_ALIGNED imag[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep %d..\n", r);

        unsigned length = pseudo_rand_uint32(&seed) % MAX_LEN;
        exponent_t exponent = (pseudo_rand_int32(&seed) % 40) - 20;
        int shr = pseudo_rand_uint32(&seed) % 10;

        for(int i = 0; i < length; i++){
            real[i] = pseudo_rand_int16(&seed) >> shr;
            imag[i] = pseudo_rand_int16(&seed) >> shr;
        }
        
        headroom_t exp_hr_a = xs3_headroom_vect_s16(real, length);
        headroom_t exp_hr_b = xs3_headroom_vect_s16(imag, length);
        headroom_t exp_hr = exp_hr_a < exp_hr_b?
                            exp_hr_a : exp_hr_b;

        bfp_complex_s16_t A;

        bfp_init_vect_complex_s16(&A, real, imag, exponent, length, 0);

        TEST_ASSERT_EQUAL(real, A.real);
        TEST_ASSERT_EQUAL(imag, A.imag);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(0, A.hr);

        headroom_t got_hr = bfp_headroom_vect_complex_s16(&A);

        TEST_ASSERT_EQUAL(real, A.real);
        TEST_ASSERT_EQUAL(imag, A.imag);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(exp_hr, A.hr);
        TEST_ASSERT_EQUAL(exp_hr, got_hr);
    }
}





static void test_bfp_headroom_vect_complex_s32()
{
    PRINTF("%s...\n", __func__);

    unsigned seed = 54322;
    
    complex_s32_t WORD_ALIGNED data[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep %d..\n", r);

        unsigned length = pseudo_rand_uint32(&seed) % MAX_LEN;
        exponent_t exponent = (pseudo_rand_int32(&seed) % 40) - 20;
        int shr = pseudo_rand_uint32(&seed) % 10;

        for(int i = 0; i < length; i++){
            data[i].re = pseudo_rand_int32(&seed) >> shr;
            data[i].im = pseudo_rand_int32(&seed) >> shr;
        }
        
        headroom_t exp_hr = xs3_headroom_vect_s32((int32_t*)data, length*2);

        bfp_complex_s32_t A;

        bfp_init_vect_complex_s32(&A, data, exponent, length, 0);

        TEST_ASSERT_EQUAL(data, A.data);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(0, A.hr);

        headroom_t got_hr = bfp_headroom_vect_complex_s32(&A);

        TEST_ASSERT_EQUAL(data, A.data);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(exp_hr, A.hr);
        TEST_ASSERT_EQUAL(exp_hr, got_hr);
    }
}





static void test_bfp_headroom_vect_ch_pair_s16()
{
    PRINTF("%s...\n", __func__);

    unsigned seed = 456488;
    
    ch_pair_s16_t WORD_ALIGNED data[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep %d..\n", r);

        unsigned length = pseudo_rand_uint32(&seed) % MAX_LEN;
        exponent_t exponent = (pseudo_rand_int32(&seed) % 40) - 20;
        int shr = pseudo_rand_uint32(&seed) % 10;

        for(int i = 0; i < length; i++){
            data[i].ch_a = pseudo_rand_int16(&seed) >> shr;
            data[i].ch_b = pseudo_rand_int16(&seed) >> shr;
        }
        
        headroom_t exp_hr = xs3_headroom_vect_s16((int16_t*)data, length*2);

        bfp_ch_pair_s16_t A;

        bfp_init_vect_ch_pair_s16(&A, data, exponent, length, 0);

        TEST_ASSERT_EQUAL(data, A.data);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(0, A.hr);

        headroom_t got_hr = bfp_headroom_vect_ch_pair_s16(&A);

        TEST_ASSERT_EQUAL(data, A.data);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(exp_hr, A.hr);
        TEST_ASSERT_EQUAL(exp_hr, got_hr);
    }
}





static void test_bfp_headroom_vect_ch_pair_s32()
{
    PRINTF("%s...\n", __func__);

    unsigned seed = 344;
    
    ch_pair_s32_t WORD_ALIGNED data[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep %d..\n", r);

        unsigned length = pseudo_rand_uint32(&seed) % MAX_LEN;
        exponent_t exponent = (pseudo_rand_int32(&seed) % 40) - 20;
        int shr = pseudo_rand_uint32(&seed) % 10;

        for(int i = 0; i < length; i++){
            data[i].ch_a = pseudo_rand_int32(&seed) >> shr;
            data[i].ch_b = pseudo_rand_int32(&seed) >> shr;
        }
        
        headroom_t exp_hr = xs3_headroom_vect_s32((int32_t*)data, length*2);

        bfp_ch_pair_s32_t A;

        bfp_init_vect_ch_pair_s32(&A, data, exponent, length, 0);

        TEST_ASSERT_EQUAL(data, A.data);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(0, A.hr);

        headroom_t got_hr = bfp_headroom_vect_ch_pair_s32(&A);

        TEST_ASSERT_EQUAL(data, A.data);
        TEST_ASSERT_EQUAL(exponent, A.exp);
        TEST_ASSERT_EQUAL(exp_hr, A.hr);
        TEST_ASSERT_EQUAL(exp_hr, got_hr);
    }
}




void test_bfp_headroom_vect()
{
    SET_TEST_FILE();

    RUN_TEST(test_bfp_headroom_vect_s16);
    RUN_TEST(test_bfp_headroom_vect_s32);
    RUN_TEST(test_bfp_headroom_vect_complex_s16);
    RUN_TEST(test_bfp_headroom_vect_complex_s32);
    RUN_TEST(test_bfp_headroom_vect_ch_pair_s16);
    RUN_TEST(test_bfp_headroom_vect_ch_pair_s32);
}