// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "xs3_math.h"

#include "../tst_common.h"
#include "xs3_vpu_scalar_ops.h"

#include "unity.h"

static unsigned seed = 2314567;

static char msg_buff[200];


#if DEBUG_ON || 0
#undef DEBUG_ON
#define DEBUG_ON    (1)
#endif


#define MAX_LEN     30
#define REPS        30



#define LEN  4
static void test_xs3_vect_s16_inverse_prepare()
{
    PRINTF("%s...\n", __func__);
    seed = 56456;

    
    for(int v = 0; v < REPS; v++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", v, seed);

        exponent_t b_exp = pseudo_rand_int(&seed, -30, 30);
        headroom_t hr = pseudo_rand_uint(&seed, 0, 12);

        int16_t b[LEN] = { INT16_MIN_POS(hr), INT16_MAX_POS(hr), INT16_MIN_NEG(hr), INT16_MAX_NEG(hr) };
         
        exponent_t a_exp;
        unsigned scale;

        xs3_vect_s16_inverse_prepare(&a_exp, &scale, b, b_exp, LEN);

        // printf("hr = %u\n", hr);
        // printf("b_exp = %d\n", b_exp);
        // printf("scale = %u\n", scale);
        // for(int i = 0; i < LEN; i++)
        //     printf("  b[%d] = %d    (0x%04X)\n", i, b[i], (unsigned) b[i] );
    

        // Verify that I've set up the test correctly.
        
        TEST_ASSERT_EQUAL(hr, HR_S16(b[0]));
        TEST_ASSERT_EQUAL(hr, HR_S16(b[1]));
        TEST_ASSERT_EQUAL(hr, HR_S16(b[2]));
        TEST_ASSERT_EQUAL(hr, HR_S16(b[3]));

        TEST_ASSERT_EQUAL(hr+1, HR_S16(b[0]-1));
        TEST_ASSERT_EQUAL(hr+1, HR_S16(b[3]+1));

        if(hr != 0){
            TEST_ASSERT_EQUAL(hr-1, HR_S16(b[1]+1));
            TEST_ASSERT_EQUAL(hr-1, HR_S16(b[2]-1));
        }

        int16_t a[4];

        xs3_vect_s16_inverse(a, b, LEN, scale);

        double b_float = ldexp(b[0], b_exp);
        double b_float_inv = 1.0 / b_float;
        double a_float = ldexp(a[0], a_exp);

        // printf("a_exp = %d\n", a_exp);
        // for(int i = 0; i < LEN; i++)
        //     printf("  a[%d] = %d    (0x%04X)\n", i, a[i], (unsigned) a[i] );

        // printf("b_float_inv = %e\n", b_float_inv);
        // printf("a_float = %e\n", a_float);
        // printf("! %e\n", b_float_inv - a_float);

        TEST_ASSERT_EQUAL_INT16(0x4000, a[0]);

        TEST_ASSERT( b_float_inv == a_float );

    }

}
#undef LEN

#define LEN  4
static void test_xs3_vect_s32_inverse_prepare()
{
    PRINTF("%s...\n", __func__);
    seed = 435634;

    
    for(int v = 0; v < REPS; v++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", v, seed);

        exponent_t b_exp = pseudo_rand_int(&seed, -30, 30);
        headroom_t hr = pseudo_rand_uint(&seed, 0, 28);

        int32_t b[LEN] = { INT32_MIN_POS(hr), INT32_MAX_POS(hr), INT32_MIN_NEG(hr), INT32_MAX_NEG(hr) };
         
        exponent_t a_exp;
        unsigned scale;

        xs3_vect_s32_inverse_prepare(&a_exp, &scale, b, b_exp, LEN);

        // printf("hr = %u\n", hr);
        // printf("b_exp = %d\n", b_exp);
        // printf("scale = %u\n", scale);
        // for(int i = 0; i < LEN; i++)
        //     printf("  b[%d] = %ld    (0x%08X)\n", i, b[i], (unsigned) b[i] );
    

        // Verify that I've set up the test correctly.
        
        TEST_ASSERT_EQUAL(hr, HR_S32(b[0]));
        TEST_ASSERT_EQUAL(hr, HR_S32(b[1]));
        TEST_ASSERT_EQUAL(hr, HR_S32(b[2]));
        TEST_ASSERT_EQUAL(hr, HR_S32(b[3]));

        TEST_ASSERT_EQUAL(hr+1, HR_S32(b[0]-1));
        TEST_ASSERT_EQUAL(hr+1, HR_S32(b[3]+1));

        if(hr != 0){
            TEST_ASSERT_EQUAL(hr-1, HR_S32(b[1]+1));
            TEST_ASSERT_EQUAL(hr-1, HR_S32(b[2]-1));
        }

        int32_t a[4];

        xs3_vect_s32_inverse(a, b, LEN, scale);

        TEST_ASSERT_EQUAL_INT32(0x40000000, a[0]);

        double b_float = ldexp(b[0], b_exp);
        double b_float_inv = 1.0 / b_float;
        double a_float = ldexp(a[0], a_exp);

        // printf("a_exp = %d\n", a_exp);
        // for(int i = 0; i < LEN; i++)
        //     printf("  a[%d] = %ld    (0x%08X)\n", i, a[i], (unsigned) a[i] );

        // printf("b_float_inv = %e\n", b_float_inv);
        // printf("a_float = %e\n", a_float);
        // printf("! %e\n", b_float_inv - a_float);

        TEST_ASSERT( b_float_inv == a_float );

    }

}
#undef LEN



static void test_xs3_vect_s16_inverse()
{

    PRINTF("%s...\n", __func__);
    seed = 0xF80C98BE;

    int16_t B[MAX_LEN];
    int16_t A[MAX_LEN];


    for(int v = 0; v < REPS; v++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", v, seed);

        const unsigned length = pseudo_rand_uint(&seed, 0, MAX_LEN-1);

        const exponent_t b_exp = pseudo_rand_int(&seed, -30, 30);
        headroom_t b_hr = pseudo_rand_uint(&seed, 0, 12);

        for(int i = 0; i < length; i++){
            B[i] = pseudo_rand_int16(&seed) >> b_hr;
            if( B[i] == 0 )
                B[i] = 1;
        }

        exponent_t a_exp;
        unsigned scale;

        xs3_vect_s16_inverse_prepare(&a_exp, &scale, B, b_exp, length);

        xs3_vect_s16_inverse(A, B, length, scale);

        double expected_flt[MAX_LEN];

        for(int i = 0; i < length; i++){
            expected_flt[i] = 1.0 / ldexp(B[i], b_exp);
        }

        int16_t expected[MAX_LEN];

        test_s16_from_double(expected, expected_flt, length, a_exp);

        for(int i = 0; i < length; i++){
            TEST_ASSERT_INT16_WITHIN(2, expected[i], A[i]);
        }
    }
}



static void test_xs3_vect_s32_inverse()
{

    PRINTF("%s...\n", __func__);
    seed = 0xF80C98BE;

    int32_t B[MAX_LEN];
    int32_t A[MAX_LEN];


    for(int v = 0; v < REPS; v++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", v, seed);

        const unsigned length = pseudo_rand_uint(&seed, 0, MAX_LEN-1);

        const exponent_t b_exp = pseudo_rand_int(&seed, -30, 30);
        headroom_t b_hr = pseudo_rand_uint(&seed, 0, 28);

        for(int i = 0; i < length; i++){
            B[i] = pseudo_rand_int32(&seed) >> b_hr;
            if( B[i] == 0 )
                B[i] = 1;
        }

        exponent_t a_exp;
        unsigned scale;

        xs3_vect_s32_inverse_prepare(&a_exp, &scale, B, b_exp, length);

        headroom_t a_hr = xs3_vect_s32_inverse(A, B, length, scale);

        // TEST_ASSERT_EQUAL(xs3_vect_s32_headroom(A, length), a_hr);

        double expected_flt[MAX_LEN];

        for(int i = 0; i < length; i++){
            expected_flt[i] = 1.0 / ldexp(B[i], b_exp);
        }

        int32_t expected[MAX_LEN];

        test_s32_from_double(expected, expected_flt, length, a_exp);

        for(int i = 0; i < length; i++){

            // if( abs(expected[i] - A[i]) > 2 ) {

            //     printf("i = %d\n", i);

            //     printf("b_exp = %d\n", b_exp);
            //     printf("scale = %u\n", scale);
            //     for(int i = 0; i < length; i++)
            //         printf("  b[%d] = %ld    (0x%08X)\n", i, B[i], (unsigned) B[i] );


            //     printf("a_exp = %d\n", a_exp);
            //     for(int i = 0; i < length; i++)
            //         printf("  a[%d] = %ld    (0x%08X)\n", i, A[i], (unsigned) A[i] );

            // }

            TEST_ASSERT_INT32_WITHIN(2, expected[i], A[i]);



        }
    }
}




void test_xs3_inverse_vect()
{
    SET_TEST_FILE();
    
    RUN_TEST(test_xs3_vect_s16_inverse_prepare);
    RUN_TEST(test_xs3_vect_s32_inverse_prepare);
    RUN_TEST(test_xs3_vect_s16_inverse);
    RUN_TEST(test_xs3_vect_s32_inverse);

}