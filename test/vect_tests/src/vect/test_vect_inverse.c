// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "xmath/xmath.h"

#include "../tst_common.h"
#include "xmath/xs3/vpu_scalar_ops.h"

#include "unity_fixture.h"
TEST_GROUP_RUNNER(vect_inverse) {
  RUN_TEST_CASE(vect_inverse, vect_s16_inverse_prepare);
  RUN_TEST_CASE(vect_inverse, vect_s32_inverse_prepare);
  RUN_TEST_CASE(vect_inverse, vect_s16_inverse);
  RUN_TEST_CASE(vect_inverse, vect_s32_inverse);
}

TEST_GROUP(vect_inverse);
TEST_SETUP(vect_inverse) { fflush(stdout); }
TEST_TEAR_DOWN(vect_inverse) {}


#define MAX_LEN     30
#define REPS        30


#define LEN  4


TEST(vect_inverse, vect_s16_inverse_prepare)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    
    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        exponent_t b_exp = pseudo_rand_int(&seed, -30, 30);
        headroom_t hr = pseudo_rand_uint(&seed, 0, 12);

        int16_t b[LEN] = { INT16_MIN_POS(hr), INT16_MAX_POS(hr), INT16_MIN_NEG(hr), INT16_MAX_NEG(hr) };
         
        exponent_t a_exp;
        unsigned scale;

        vect_s16_inverse_prepare(&a_exp, &scale, b, b_exp, LEN);

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

        vect_s16_inverse(a, b, LEN, scale);

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

TEST(vect_inverse, vect_s32_inverse_prepare)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    
    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        exponent_t b_exp = pseudo_rand_int(&seed, -30, 30);
        headroom_t hr = pseudo_rand_uint(&seed, 0, 28);

        int32_t b[LEN] = { INT32_MIN_POS(hr), INT32_MAX_POS(hr), INT32_MIN_NEG(hr), INT32_MAX_NEG(hr) };
         
        exponent_t a_exp;
        unsigned scale;

        vect_s32_inverse_prepare(&a_exp, &scale, b, b_exp, LEN);

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

        vect_s32_inverse(a, b, LEN, scale);

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


TEST(vect_inverse, vect_s16_inverse)
{

    
    unsigned seed = SEED_FROM_FUNC_NAME();


    int16_t B[MAX_LEN];
    int16_t A[MAX_LEN];


    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        const unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN-1);

        const exponent_t b_exp = pseudo_rand_int(&seed, -30, 30);
        headroom_t b_hr = pseudo_rand_uint(&seed, 0, 12);

        for(unsigned int i = 0; i < length; i++){
            B[i] = pseudo_rand_int16(&seed) >> b_hr;
            if( B[i] == 0 )
                B[i] = 1;
        }

        exponent_t a_exp;
        unsigned scale;

        vect_s16_inverse_prepare(&a_exp, &scale, B, b_exp, length);

        vect_s16_inverse(A, B, length, scale);

        double expected_flt[MAX_LEN];

        for(unsigned int i = 0; i < length; i++){
            expected_flt[i] = 1.0 / ldexp(B[i], b_exp);
        }

        int16_t expected[MAX_LEN];

        test_s16_from_double(expected, expected_flt, length, a_exp);

        for(unsigned int i = 0; i < length; i++){
            TEST_ASSERT_INT16_WITHIN(2, expected[i], A[i]);
        }
    }
}


TEST(vect_inverse, vect_s32_inverse)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    int32_t B[MAX_LEN];
    int32_t A[MAX_LEN];


    for(unsigned int v = 0; v < REPS; v++){
        const unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN-1);
        setExtraInfo_RSL(v, seed, length);

        const exponent_t b_exp = pseudo_rand_int(&seed, -30, 30);
        headroom_t b_hr = pseudo_rand_uint(&seed, 0, 28);

        for(unsigned int i = 0; i < length; i++){
            B[i] = pseudo_rand_int32(&seed) >> b_hr;
            if( B[i] == 0 )
                B[i] = 1;
        }

        exponent_t a_exp;
        unsigned scale;

        vect_s32_inverse_prepare(&a_exp, &scale, B, b_exp, length);

        vect_s32_inverse(A, B, length, scale);

        double expected_flt[MAX_LEN];

        for(unsigned int i = 0; i < length; i++){
            expected_flt[i] = 1.0 / ldexp(B[i], b_exp);
        }

        int32_t expected[MAX_LEN];

        test_s32_from_double(expected, expected_flt, length, a_exp);

        for(unsigned int i = 0; i < length; i++){

            if( abs(expected[i] - A[i]) > 2 ) {

                printf("i = %d\n", i);

                printf("b_exp = %d\n", b_exp);
                printf("scale = %u\n", scale);
                for(unsigned int i = 0; i < length; i++)
                    printf("  b[%d] = %ld    (0x%08X)\n", i, (unsigned long int) B[i], (unsigned) B[i] );


                printf("a_exp = %d\n", a_exp);
                for(unsigned int i = 0; i < length; i++)
                    printf("  a[%d] = %ld    (0x%08X)\n", i, (unsigned long int) A[i], (unsigned) A[i] );

            }

            TEST_ASSERT_INT32_WITHIN_MESSAGE(2, expected[i], A[i], "");


        }
    }
}

