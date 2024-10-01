// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xmath/xmath.h"

#include "../../tst_common.h"

#include "unity_fixture.h"


TEST_GROUP_RUNNER(bfp_complex_real_mul) {
  RUN_TEST_CASE(bfp_complex_real_mul, bfp_complex_s16_real_mul);
  RUN_TEST_CASE(bfp_complex_real_mul, bfp_complex_s32_real_mul);
  RUN_TEST_CASE(bfp_complex_real_mul, bfp_complex_s32_real_mulB);
}

TEST_GROUP(bfp_complex_real_mul);
TEST_SETUP(bfp_complex_real_mul) { fflush(stdout); }
TEST_TEAR_DOWN(bfp_complex_real_mul) {}

#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (128)
#else
#  define REPS       (1000)
#  define MAX_LEN    (512)
#endif

// static char msg_buff[200];

#define TEST_ASSERT_EQUAL_MSG(EXPECTED, ACTUAL, EXTRA, LINE_NUM)   do{          \
    if((EXPECTED)!=(ACTUAL)) {                                                  \
      sprintf(msg_buff, "%s (test vector @ line %u)", (EXTRA), (LINE_NUM));     \
      TEST_ASSERT_EQUAL_MESSAGE((EXPECTED), (ACTUAL), msg_buff);                \
    }} while(0)


TEST(bfp_complex_real_mul, bfp_complex_s16_real_mul)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    struct {
        int16_t WORD_ALIGNED real[MAX_LEN];
        int16_t WORD_ALIGNED imag[MAX_LEN];
    } WORD_ALIGNED A_data, WORD_ALIGNED B_data;

    int16_t WORD_ALIGNED C_data[MAX_LEN];

    bfp_complex_s16_t A, B;
    bfp_s16_t C;

    struct {
        double real[MAX_LEN];
        double imag[MAX_LEN];
    } Af, Bf;

    double Cf[MAX_LEN];

    struct {
        int16_t real[MAX_LEN];
        int16_t imag[MAX_LEN];
    } expA;

    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        bfp_complex_s16_init(&B, B_data.real, B_data.imag,
            pseudo_rand_int(&seed, -30, 30),
            pseudo_rand_uint(&seed, 1, MAX_LEN+1), 0);

        bfp_s16_init(&C, C_data, pseudo_rand_int(&seed, -30, 30), B.length, 0);

        bfp_complex_s16_init(&A, A_data.real, A_data.imag, 0, B.length, 0);

        B.hr = pseudo_rand_uint(&seed, 0, 12);
        C.hr = pseudo_rand_uint(&seed, 0, 12);

        for(unsigned int i = 0; i < B.length; i++){
            B.real[i] = pseudo_rand_int16(&seed) >> B.hr;
            B.imag[i] = pseudo_rand_int16(&seed) >> B.hr;
            C.data[i] = pseudo_rand_int16(&seed) >> C.hr;
        }

        test_double_from_complex_s16(Bf.real, Bf.imag, &B);
        test_double_from_s16(Cf, &C);

        for(unsigned int i = 0; i < B.length; i++){
            Af.real[i] = Bf.real[i] * Cf[i];
            Af.imag[i] = Bf.imag[i] * Cf[i];
        }

        bfp_complex_s16_real_mul(&A, &B, &C);

        TEST_ASSERT_EQUAL(vect_complex_s16_headroom(A.real, A.imag, A.length), A.hr);

        test_complex_s16_from_double(expA.real, expA.imag, Af.real, Af.imag, MAX_LEN, A.exp);

        for(unsigned int i = 0; i < A.length; i++){

            // printf("A.real[%d] = %d <--  %d       %d\n", i,  A.real[i], B.real[i], C.data[i]);
            // printf("A.imag[%d] = %d <--  %d       %d\n", i,  A.imag[i], B.imag[i], C.data[i]);
            // printf("======\n");

            TEST_ASSERT_INT16_WITHIN(1, expA.real[i], A.real[i]);
            TEST_ASSERT_INT16_WITHIN(1, expA.imag[i], A.imag[i]);
        }
    }
}


TEST(bfp_complex_real_mul, bfp_complex_s32_real_mul)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    complex_s32_t A_data[MAX_LEN], B_data[MAX_LEN];

    int32_t C_data[MAX_LEN];

    bfp_complex_s32_t A, B;
    bfp_s32_t C;


    struct {
        double real[MAX_LEN];
        double imag[MAX_LEN];
    } Af, Bf;

    double Cf[MAX_LEN];

    complex_s32_t expected[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        bfp_complex_s32_init(&B, B_data,
            pseudo_rand_int(&seed, -30, 30),
            pseudo_rand_uint(&seed, 1, MAX_LEN+1), 0);
        
        bfp_complex_s32_init(&A, A_data, 0, B.length, 0);

        bfp_s32_init(&C, C_data, pseudo_rand_int(&seed, -30, 30), B.length, 0);

        B.hr = pseudo_rand_uint(&seed, 0, 28);
        C.hr = pseudo_rand_uint(&seed, 0, 28);

        for(unsigned int i = 0; i < B.length; i++){

            B.data[i].re = pseudo_rand_int16(&seed) >> B.hr;
            B.data[i].im = pseudo_rand_int16(&seed) >> B.hr;
            C.data[i]    = pseudo_rand_int16(&seed) >> C.hr;
        }

        test_double_from_complex_s32(Bf.real, Bf.imag, &B);
        test_double_from_s32(Cf, &C);

        for(unsigned int i = 0; i < B.length; i++){
            Af.real[i] = Bf.real[i] * Cf[i];
            Af.imag[i] = Bf.imag[i] * Cf[i];
        }

        bfp_complex_s32_real_mul(&A, &B, &C);

        TEST_ASSERT_EQUAL(vect_complex_s32_headroom(A.data, A.length), A.hr);

        test_complex_s32_from_double(expected, Af.real, Af.imag, MAX_LEN, A.exp);

        for(unsigned int i = 0; i < A.length; i++){

            // printf("A.real[%d] = %d <--  %d       %d\n", i,  A.real[i], B.real[i], C.data[i]);
            // printf("A.imag[%d] = %d <--  %d       %d\n", i,  A.imag[i], B.imag[i], C.data[i]);
            // printf("======\n");

            TEST_ASSERT_INT32_WITHIN(1, expected[i].re, A.data[i].re);
            TEST_ASSERT_INT32_WITHIN(1, expected[i].im, A.data[i].im);
        }
    }
}

/**
  *  astew: 2022/07/01 -- Test case comes from Shuchita via issue #102.
  *         Added to unit tests to make sure this loop is closed.
  *
  *  This test case demonstrated a bug in the logic of vect_complex_s32_real_mul_prepare(),
  *   which was allowing the input vectors to be left-shifted more bits than they had headroom.
  *   vect_complex_s32_real_mul_prepare() has been fixed, so this should no longer be an issue.
  */
TEST(bfp_complex_real_mul, bfp_complex_s32_real_mulB)
{
  complex_s32_t DWORD_ALIGNED in_data[10];
  int32_t DWORD_ALIGNED scale_data[10];
  bfp_complex_s32_t input;
  bfp_s32_t scale;
  bfp_complex_s32_init(&input, in_data, 0, 10, 0);
  bfp_s32_init(&scale, scale_data, 0, 10, 0);

  in_data[0].re = 53956423;
  in_data[0].im = 0;
  in_data[1].re = 62086712;
  in_data[1].im = -33052879;
  in_data[2].re = 85460880;
  in_data[2].im = -60238506;
  in_data[3].re = 121141046;
  in_data[3].im = -76358395;
  in_data[4].re = 164591974;
  in_data[4].im = -77478642;
  in_data[5].re = 210181752;
  in_data[5].im = -61388184;
  in_data[6].re = 251804911;
  in_data[6].im = -27869327;
  in_data[7].re = 283560427;
  in_data[7].im = 21248980;
  in_data[8].re = 300411153;
  in_data[8].im = 82261225;
  in_data[9].re = 298753162;
  in_data[9].im = 149960635;
  input.exp = -34;
  input.hr = 2;

  for(int i=0; i<10; i++) {
      scale_data[i] = 8388608;
  }
  scale.exp = -23;
  scale.hr = 7;

  complex_s32_t DWORD_ALIGNED out_data[10];
  bfp_complex_s32_t output;
  bfp_complex_s32_init(&output, out_data, 0, 10, 0);

  bfp_complex_s32_real_mul(&output, &input, &scale); // Error_ap[0][f] *

  bfp_complex_s32_shl(&output, &output, output.hr);
  bfp_complex_s32_shl(&input, &input, input.hr);

  for(int i=0; i<10; i++) {
    TEST_ASSERT_EQUAL_INT32(input.data[i].re, output.data[i].re);
    TEST_ASSERT_EQUAL_INT32(input.data[i].im, output.data[i].im);
  }
}

