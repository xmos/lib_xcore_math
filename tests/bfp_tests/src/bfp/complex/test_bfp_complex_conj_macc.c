// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// XMOS Public License: Version 1

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xmath/xmath.h"

#include "../../tst_common.h"

#include "unity_fixture.h"


TEST_GROUP_RUNNER(bfp_complex_conj_macc) {
  RUN_TEST_CASE(bfp_complex_conj_macc, bfp_complex_s16_conj_macc);
  RUN_TEST_CASE(bfp_complex_conj_macc, bfp_complex_s16_conj_nmacc);
  RUN_TEST_CASE(bfp_complex_conj_macc, bfp_complex_s32_conj_macc);
  RUN_TEST_CASE(bfp_complex_conj_macc, bfp_complex_s32_conj_nmacc);
}

TEST_GROUP(bfp_complex_conj_macc);
TEST_SETUP(bfp_complex_conj_macc) { fflush(stdout); }
TEST_TEAR_DOWN(bfp_complex_conj_macc) {}

#if SMOKE_TEST
#  define REPS       (100)
#  define LEN        (128)
#else
#  define REPS       (1000)
#  define LEN        (512)
#endif


TEST(bfp_complex_conj_macc, bfp_complex_s16_conj_macc)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    struct {
      int16_t real[LEN + (LEN%2)];  // LEN%2 is so 'im' stays word-aligned
      int16_t imag[LEN + (LEN%2)];
    } dataA, dataB, dataC, expA;

    bfp_complex_s16_t A, B, C;

    struct {
      double real[LEN];
      double imag[LEN];
    } Af;
    // complex_double_t Af[LEN];

    bfp_complex_s16_init(&A, dataA.real, dataA.imag, -1024, LEN, 0);
    bfp_complex_s16_init(&B, dataB.real, dataB.imag, -1024, LEN, 0);
    bfp_complex_s16_init(&C, dataC.real, dataC.imag, -1024, LEN, 0);

    complex_s16_t zero = {0,0};
    bfp_complex_s16_set(&A, zero, -1024);

    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        B.exp = 10 + (pseudo_rand_int32(&seed) % 10);
        C.exp = 10 + (pseudo_rand_int32(&seed) % 10);

        for(int i = 0; i < LEN; i++){
            unsigned B_shr = pseudo_rand_uint16(&seed) % 8;
            unsigned C_shr = pseudo_rand_uint16(&seed) % 8;

            B.real[i] = pseudo_rand_int16(&seed) >> B_shr;
            B.imag[i] = pseudo_rand_int16(&seed) >> B_shr;
            
            C.real[i] = pseudo_rand_int16(&seed) >> C_shr;
            C.imag[i] = pseudo_rand_int16(&seed) >> C_shr;

            Af.real[i] = ldexp(A.real[i], A.exp);
            Af.imag[i] = ldexp(A.imag[i], A.exp);
        }

        bfp_complex_s16_headroom(&B);
        bfp_complex_s16_headroom(&C);

        for(unsigned int i = 0; i < B.length; i++){
            Af.real[i] = Af.real[i] + (  ldexp(B.real[i], B.exp) * ldexp(C.real[i], C.exp) 
                                       + ldexp(B.imag[i], B.exp) * ldexp(C.imag[i], C.exp));
            Af.imag[i] = Af.imag[i] + (- ldexp(B.real[i], B.exp) * ldexp(C.imag[i], C.exp) 
                                       + ldexp(B.imag[i], B.exp) * ldexp(C.real[i], C.exp));
        }

        bfp_complex_s16_conj_macc(&A, &B, &C);

        test_complex_s16_from_double(expA.real, expA.imag, Af.real, Af.imag, LEN, A.exp);

        for(unsigned int i = 0; i < A.length; i++){
            TEST_ASSERT_INT16_WITHIN(2, expA.real[i], A.real[i]);
            TEST_ASSERT_INT16_WITHIN(2, expA.imag[i], A.imag[i]);
        }
    }
}


TEST(bfp_complex_conj_macc, bfp_complex_s16_conj_nmacc)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    struct {
      int16_t real[LEN + (LEN%2)];  // LEN%2 is so 'im' stays word-aligned
      int16_t imag[LEN + (LEN%2)];
    } dataA, dataB, dataC, expA;

    bfp_complex_s16_t A, B, C;

    struct {
      double real[LEN];
      double imag[LEN];
    } Af;
    // complex_double_t Af[LEN];

    bfp_complex_s16_init(&A, dataA.real, dataA.imag, -1024, LEN, 0);
    bfp_complex_s16_init(&B, dataB.real, dataB.imag, -1024, LEN, 0);
    bfp_complex_s16_init(&C, dataC.real, dataC.imag, -1024, LEN, 0);

    complex_s16_t zero = {0,0};
    bfp_complex_s16_set(&A, zero, -1024);

    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        B.exp = 10 + (pseudo_rand_int32(&seed) % 10);
        C.exp = 10 + (pseudo_rand_int32(&seed) % 10);

        for(int i = 0; i < LEN; i++){
            unsigned B_shr = pseudo_rand_uint16(&seed) % 8;
            unsigned C_shr = pseudo_rand_uint16(&seed) % 8;

            B.real[i] = pseudo_rand_int16(&seed) >> B_shr;
            B.imag[i] = pseudo_rand_int16(&seed) >> B_shr;
            
            C.real[i] = pseudo_rand_int16(&seed) >> C_shr;
            C.imag[i] = pseudo_rand_int16(&seed) >> C_shr;

            Af.real[i] = ldexp(A.real[i], A.exp);
            Af.imag[i] = ldexp(A.imag[i], A.exp);
        }

        bfp_complex_s16_headroom(&B);
        bfp_complex_s16_headroom(&C);

        for(unsigned int i = 0; i < B.length; i++){
            Af.real[i] = Af.real[i] - (  ldexp(B.real[i], B.exp) * ldexp(C.real[i], C.exp) 
                                       + ldexp(B.imag[i], B.exp) * ldexp(C.imag[i], C.exp));
            Af.imag[i] = Af.imag[i] - (- ldexp(B.real[i], B.exp) * ldexp(C.imag[i], C.exp) 
                                       + ldexp(B.imag[i], B.exp) * ldexp(C.real[i], C.exp));
        }

        bfp_complex_s16_conj_nmacc(&A, &B, &C);

        test_complex_s16_from_double(expA.real, expA.imag, Af.real, Af.imag, LEN, A.exp);

        for(unsigned int i = 0; i < A.length; i++){
            TEST_ASSERT_INT16_WITHIN(2, expA.real[i], A.real[i]);
            TEST_ASSERT_INT16_WITHIN(2, expA.imag[i], A.imag[i]);
        }
    }
}


TEST(bfp_complex_conj_macc, bfp_complex_s32_conj_macc)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    complex_s32_t dataA[LEN];
    complex_s32_t dataB[LEN];
    complex_s32_t dataC[LEN];
    complex_s32_t expA[LEN];
    bfp_complex_s32_t A, B, C;

    struct {
      double real[LEN];
      double imag[LEN];
    } Af;

    bfp_complex_s32_init(&A, dataA, -1024, LEN, 0);
    bfp_complex_s32_init(&B, dataB, -1024, LEN, 0);
    bfp_complex_s32_init(&C, dataC, -1024, LEN, 0);

    complex_s32_t zero = {0,0};
    bfp_complex_s32_set(&A, zero, -1024);

    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        B.exp = 10 + (pseudo_rand_int32(&seed) % 10);
        C.exp = 10 + (pseudo_rand_int32(&seed) % 10);

        for(int i = 0; i < LEN; i++){
            unsigned B_shr = pseudo_rand_uint16(&seed) % 8;
            unsigned C_shr = pseudo_rand_uint16(&seed) % 8;

            B.data[i].re = pseudo_rand_int32(&seed) >> B_shr;
            B.data[i].im = pseudo_rand_int32(&seed) >> B_shr;
            C.data[i].re = pseudo_rand_int32(&seed) >> C_shr;
            C.data[i].im = pseudo_rand_int32(&seed) >> C_shr;

            Af.real[i] = ldexp(A.data[i].re, A.exp);
            Af.imag[i] = ldexp(A.data[i].im, A.exp);
        }

        bfp_complex_s32_headroom(&B);
        bfp_complex_s32_headroom(&C);

        for(unsigned int i = 0; i < B.length; i++){
            Af.real[i] = Af.real[i] + (  ldexp(B.data[i].re, B.exp) * ldexp(C.data[i].re, C.exp) 
                                       + ldexp(B.data[i].im, B.exp) * ldexp(C.data[i].im, C.exp));
            Af.imag[i] = Af.imag[i] + (- ldexp(B.data[i].re, B.exp) * ldexp(C.data[i].im, C.exp) 
                                       + ldexp(B.data[i].im, B.exp) * ldexp(C.data[i].re, C.exp));
        }

        bfp_complex_s32_conj_macc(&A, &B, &C);

        test_complex_s32_from_double(expA, Af.real, Af.imag, LEN, A.exp);

        for(unsigned int i = 0; i < A.length; i++){
            TEST_ASSERT_INT32_WITHIN(3, expA[i].re, A.data[i].re);
            TEST_ASSERT_INT32_WITHIN(3, expA[i].im, A.data[i].im);
        }
    }
}


TEST(bfp_complex_conj_macc, bfp_complex_s32_conj_nmacc)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    complex_s32_t dataA[LEN];
    complex_s32_t dataB[LEN];
    complex_s32_t dataC[LEN];
    complex_s32_t expA[LEN];
    bfp_complex_s32_t A, B, C;

    struct {
      double real[LEN];
      double imag[LEN];
    } Af;

    bfp_complex_s32_init(&A, dataA, -1024, LEN, 0);
    bfp_complex_s32_init(&B, dataB, -1024, LEN, 0);
    bfp_complex_s32_init(&C, dataC, -1024, LEN, 0);

    complex_s32_t zero = {0,0};
    bfp_complex_s32_set(&A, zero, -1024);

    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);

        B.exp = 10 + (pseudo_rand_int32(&seed) % 10);
        C.exp = 10 + (pseudo_rand_int32(&seed) % 10);

        for(int i = 0; i < LEN; i++){
            unsigned B_shr = pseudo_rand_uint16(&seed) % 8;
            unsigned C_shr = pseudo_rand_uint16(&seed) % 8;

            B.data[i].re = pseudo_rand_int32(&seed) >> B_shr;
            B.data[i].im = pseudo_rand_int32(&seed) >> B_shr;
            C.data[i].re = pseudo_rand_int32(&seed) >> C_shr;
            C.data[i].im = pseudo_rand_int32(&seed) >> C_shr;

            Af.real[i] = ldexp(A.data[i].re, A.exp);
            Af.imag[i] = ldexp(A.data[i].im, A.exp);
        }

        bfp_complex_s32_headroom(&B);
        bfp_complex_s32_headroom(&C);

        for(unsigned int i = 0; i < B.length; i++){
            Af.real[i] = Af.real[i] - (  ldexp(B.data[i].re, B.exp) * ldexp(C.data[i].re, C.exp) 
                                       + ldexp(B.data[i].im, B.exp) * ldexp(C.data[i].im, C.exp));
            Af.imag[i] = Af.imag[i] - (- ldexp(B.data[i].re, B.exp) * ldexp(C.data[i].im, C.exp) 
                                       + ldexp(B.data[i].im, B.exp) * ldexp(C.data[i].re, C.exp));
        }

        bfp_complex_s32_conj_nmacc(&A, &B, &C);

        test_complex_s32_from_double(expA, Af.real, Af.imag, LEN, A.exp);

        for(unsigned int i = 0; i < A.length; i++){
            TEST_ASSERT_INT32_WITHIN(3, expA[i].re, A.data[i].re);
            TEST_ASSERT_INT32_WITHIN(3, expA[i].im, A.data[i].im);
        }
    }
}
