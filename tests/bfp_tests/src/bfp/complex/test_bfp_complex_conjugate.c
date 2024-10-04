// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xmath/xmath.h"

#include "../../tst_common.h"
#include "../src/vect/vpu_helper.h"

#include "unity_fixture.h"


TEST_GROUP_RUNNER(bfp_complex_conjugate) {
  RUN_TEST_CASE(bfp_complex_conjugate, bfp_complex_s16_conjugate);
  RUN_TEST_CASE(bfp_complex_conjugate, bfp_complex_s32_conjugate);
}

TEST_GROUP(bfp_complex_conjugate);
TEST_SETUP(bfp_complex_conjugate) { fflush(stdout); }
TEST_TEAR_DOWN(bfp_complex_conjugate) {}

#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (128)
#else
#  define REPS       (1000)
#  define MAX_LEN    (512)
#endif


TEST(bfp_complex_conjugate, bfp_complex_s16_conjugate)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    struct {
      int32_t word_align;
      int16_t real[MAX_LEN];
      int16_t imag[MAX_LEN];
    } A_data, B_data;

    bfp_complex_s16_t A;
    bfp_complex_s16_t B;

    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        bfp_complex_s16_init(&B, B_data.real, B_data.imag,
            pseudo_rand_int(&seed, -100, 100),
            pseudo_rand_int(&seed, 1, MAX_LEN+1), 0);

        bfp_complex_s16_init(&A, A_data.real, A_data.imag, 0, B.length, 0);

        B.hr = pseudo_rand_uint(&seed, 0, 12);

        for(unsigned int i = 0; i < B.length; i++){
            B_data.real[i] = pseudo_rand_int16(&seed) >> B.hr;
            B_data.imag[i] = pseudo_rand_int16(&seed) >> B.hr;
        }
        bfp_complex_s16_headroom(&B);

        bfp_complex_s16_conjugate(&A, &B);

        // headroom might not be exactly correct, since this won't recalculate it for the real part
        // of the vector. It does update it if the new imag has less HR, though
        headroom_t exp_hr = vect_complex_s16_headroom(A.real, A.imag, A.length);
        if(exp_hr <= B.hr)
          TEST_ASSERT_EQUAL(exp_hr, A.hr);
        else
          TEST_ASSERT_EQUAL(B.hr, A.hr);
        TEST_ASSERT_EQUAL(B.exp, A.exp);

        for(unsigned int k = 0; k < B.length; k++){
          
          TEST_ASSERT_INT16_WITHIN(1, SAT(16)(B_data.real[k]), A_data.real[k]);
          TEST_ASSERT_INT16_WITHIN(1, -SAT(16)(B_data.imag[k]), A_data.imag[k]);
        }
    }
}


TEST(bfp_complex_conjugate, bfp_complex_s32_conjugate)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    complex_s32_t WORD_ALIGNED A_data[MAX_LEN];
    bfp_complex_s32_t A;

    complex_s32_t WORD_ALIGNED B_data[MAX_LEN];
    bfp_complex_s32_t B;

    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        bfp_complex_s32_init(&B, B_data,
            pseudo_rand_int(&seed, -100, 100),
            pseudo_rand_int(&seed, 1, MAX_LEN+1), 0);

        bfp_complex_s32_init(&A, A_data, 0, B.length, 0);

        B.hr = pseudo_rand_uint(&seed, 0, 28);

        for(unsigned int i = 0; i < B.length; i++){
            B.data[i].re = pseudo_rand_int32(&seed) >> B.hr;
            B.data[i].im = pseudo_rand_int32(&seed) >> B.hr;
        }
        bfp_complex_s32_headroom(&B);

        bfp_complex_s32_conjugate(&A, &B);

        TEST_ASSERT_EQUAL_MESSAGE(vect_complex_s32_headroom(A.data, A.length), A.hr, "");
        TEST_ASSERT_EQUAL_MESSAGE(B.exp, A.exp, "");

        for(unsigned int k = 0; k < B.length; k++){
          TEST_ASSERT_INT32_WITHIN(1, SAT(32)(B.data[k].re), A.data[k].re);
          TEST_ASSERT_INT32_WITHIN(1, -SAT(32)(B.data[k].im), A.data[k].im);
        }
    }
}

