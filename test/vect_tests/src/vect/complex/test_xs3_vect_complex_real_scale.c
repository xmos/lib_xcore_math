// Copyright 2020 XMOS LIMITED. This Software is subject to the terms of the 
// XMOS Public License: Version 1

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xs3_math.h"
#include "xs3_vpu_scalar_ops.h"

#include "../src/vect/vpu_helper.h"

#include "../../tst_common.h"

#include "unity.h"


static unsigned seed = 2314567;
static char msg_buff[200];

#define TEST_ASSERT_EQUAL_MSG(EXPECTED, ACTUAL, LINE_NUM)   do{       \
    if((EXPECTED)!=(ACTUAL)) {                                        \
      sprintf(msg_buff, "(test vector @ line %u)", (LINE_NUM));       \
      TEST_ASSERT_EQUAL_MESSAGE((EXPECTED), (ACTUAL), msg_buff);      \
    }} while(0)


#if DEBUG_ON || 0
#undef DEBUG_ON
#define DEBUG_ON    (1)
#endif


#define REPS        1000
#define MAX_LEN     256



static void test_xs3_vect_s16_scale_prepare()
{
    PRINTF("%s...\n", __func__);

    seed = 0x0E538276;

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);
        
        exponent_t b_exp = pseudo_rand_int(&seed, -30, 30);
        headroom_t b_hr  = pseudo_rand_uint(&seed, 0, 15);
        exponent_t c_exp = pseudo_rand_int(&seed, -30, 30);
        headroom_t c_hr  = pseudo_rand_uint(&seed, 0, 15);

        int16_t WORD_ALIGNED B_re = ((int16_t)-0x8000) >> b_hr;
        int16_t WORD_ALIGNED B_im = ((int16_t)-0x8000) >> b_hr;
        int16_t WORD_ALIGNED C = ((int16_t)-0x8000) >> c_hr;
        int16_t WORD_ALIGNED A_re;
        int16_t WORD_ALIGNED A_im;

        exponent_t a_exp;
        right_shift_t sat, c_shr;

        xs3_vect_s16_scale_prepare(&a_exp, &sat, b_exp, c_exp, b_hr, c_hr);

        xs3_vect_complex_s16_real_scale(&A_re, &A_im, &B_re, &B_im, C, 1, sat);

        const int32_t p = ((int32_t)B_re) * C;
        int16_t expected = p;

        if( p > VPU_INT16_MAX )
            expected = VPU_INT16_MAX;

        TEST_ASSERT_EQUAL_HEX16(expected, A_re);
        TEST_ASSERT_EQUAL_HEX16(expected, A_im);

        // Make sure the exponent was chosen correctly

        double fp  = ldexp(B_re, b_exp) * ldexp(C, c_exp);
        double fa  = ldexp(A_re, a_exp);

        // (A should be no more than 1 LSb off)
        TEST_ASSERT( fabs(fp - fa) <= ldexp(1,a_exp) );
        
    }
}






static void test_xs3_vect_complex_s16_real_scale()
{
    PRINTF("%s...\n", __func__);
    seed = 0xAD24398D;
    
    struct { 
        int16_t real[MAX_LEN]; 
        int16_t imag[MAX_LEN]; 
    } A, B;
    
    int16_t C;

    for(int v = 0; v < REPS; v++){

        PRINTF("\trepetition % 3d..\t(seed: 0x%08X)\n", v, seed);

        unsigned length = pseudo_rand_uint(&seed, 0, MAX_LEN+1);

        headroom_t b_hr = pseudo_rand_uint(&seed, 0, 15);
        headroom_t c_hr = pseudo_rand_uint(&seed, 0, 15);

        // Ideal sat would be  15 - (b_hr+c_hr).

        right_shift_t sat = 15 - (b_hr+c_hr) + pseudo_rand_int(&seed, -1, 3 );
        sat = MAX(sat, 0);
        
        for(int i = 0; i < length; i++){
            B.real[i] = pseudo_rand_int16(&seed) >> b_hr;
            B.imag[i] = pseudo_rand_int16(&seed) >> b_hr;
            C = pseudo_rand_int16(&seed) >> c_hr;
        }
        

        {
            headroom_t hr = xs3_vect_complex_s16_real_scale(A.real, A.imag, 
                                                            B.real, B.imag, 
                                                            C, length, sat);

            headroom_t exp_hr = xs3_vect_complex_s16_headroom(A.real, A.imag, length);

            TEST_ASSERT_EQUAL(exp_hr, hr);

            for(int i = 0; i < length; i++){
                int32_t exp = ldexp( ((int32_t)B.real[i]) * C, -sat) + ldexp(1, -40);
                exp = MIN(exp, VPU_INT16_MAX);
                exp = MAX(exp, VPU_INT16_MIN);

                TEST_ASSERT_INT16_WITHIN(1, exp, A.real[i]);

                exp = round(ldexp( ((int32_t)B.imag[i]) * C, -sat) + ldexp(1, -40));
                exp = MIN(exp, VPU_INT16_MAX);
                exp = MAX(exp, VPU_INT16_MIN);
                TEST_ASSERT_INT16_WITHIN(1, exp, A.imag[i]);
            }
        }

        {
            memcpy(A.real, B.real, sizeof(A.real));
            memcpy(A.imag, B.imag, sizeof(A.imag));


            headroom_t hr = xs3_vect_complex_s16_real_scale(A.real, A.imag, 
                                                            A.real, A.imag, 
                                                            C, length, sat);

            headroom_t exp_hr = xs3_vect_complex_s16_headroom(A.real, A.imag, length);

            TEST_ASSERT_EQUAL(exp_hr, hr);

            for(int i = 0; i < length; i++){
                int32_t exp = ldexp( ((int32_t)B.real[i]) * C, -sat) + ldexp(1, -40);
                exp = MIN(exp, VPU_INT16_MAX);
                exp = MAX(exp, VPU_INT16_MIN);
                TEST_ASSERT_INT16_WITHIN(1, exp, A.real[i]);

                exp = round(ldexp( ((int32_t)B.imag[i]) * C, -sat) + ldexp(1, -40));
                exp = MIN(exp, VPU_INT16_MAX);
                exp = MAX(exp, VPU_INT16_MIN);
                TEST_ASSERT_INT16_WITHIN(1, exp, A.imag[i]);
            }
        }

        
    }
}






static void test_xs3_vect_complex_s32_real_scale()
{
    PRINTF("%s...\n", __func__);

    seed = 0xB9A64595;

    complex_s32_t A[MAX_LEN];
    complex_s32_t B[MAX_LEN];

    struct {
        double real[MAX_LEN];
        double imag[MAX_LEN];
    } Af;

    complex_s32_t expA[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        const unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);

        const exponent_t b_exp = 0;
        const exponent_t c_exp = 0;

        const headroom_t b_hr = pseudo_rand_uint(&seed, 0, 18);
        const headroom_t c_hr = pseudo_rand_uint(&seed, 0, 18);

        int32_t C = pseudo_rand_int32(&seed) >> c_hr;

        right_shift_t b_shr, c_shr;
        exponent_t a_exp;
        xs3_vect_s32_mul_prepare(&a_exp, &b_shr, &c_shr, b_exp, c_exp, b_hr, c_hr);

        for(int i = 0; i < length; i++){
            B[i].re = pseudo_rand_int32(&seed) >> b_hr;
            B[i].im = pseudo_rand_int32(&seed) >> b_hr;

            Af.real[i] = ldexp(B[i].re, b_exp) * ldexp(C, c_exp);
            Af.imag[i] = ldexp(B[i].im, b_exp) * ldexp(C, c_exp);
        }

        headroom_t hr = xs3_vect_complex_s32_real_scale(&A[0], &B[0], C, length, b_shr, c_shr);
        
        TEST_ASSERT_EQUAL( xs3_vect_complex_s32_headroom(A, length), hr );

        test_complex_s32_from_double(expA, Af.real, Af.imag, length, a_exp);

        for(int i = 0; i < length; i++){
            TEST_ASSERT_INT32_WITHIN(2, expA[i].re, A[i].re);
            TEST_ASSERT_INT32_WITHIN(2, expA[i].im, A[i].im);
        }
    }
}





void test_xs3_scalar_mul_vect_complex()
{
    SET_TEST_FILE();
    
    RUN_TEST(test_xs3_vect_s16_scale_prepare);
    RUN_TEST(test_xs3_vect_complex_s16_real_scale);
    RUN_TEST(test_xs3_vect_complex_s32_real_scale);

}