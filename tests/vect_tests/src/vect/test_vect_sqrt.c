// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <math.h>

#include "xmath/xmath.h"

#include "../tst_common.h"

#include "unity_fixture.h"


TEST_GROUP_RUNNER(vect_sqrt) {
  RUN_TEST_CASE(vect_sqrt, vect_s32_sqrt_prepare);
  RUN_TEST_CASE(vect_sqrt, vect_s16_sqrt_prepare);
  RUN_TEST_CASE(vect_sqrt, vect_s32_sqrt_prepare_saturation);
  RUN_TEST_CASE(vect_sqrt, vect_s16_sqrt_prepare_saturation);
  RUN_TEST_CASE(vect_sqrt, vect_s16_sqrt_A);
  RUN_TEST_CASE(vect_sqrt, vect_s16_sqrt_B);
  RUN_TEST_CASE(vect_sqrt, vect_s32_sqrt_A);
  RUN_TEST_CASE(vect_sqrt, vect_s32_sqrt_B);
}

TEST_GROUP(vect_sqrt);
TEST_SETUP(vect_sqrt) { fflush(stdout); }
TEST_TEAR_DOWN(vect_sqrt) {}


static char msg_buff[200];



#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (64)
#else
#  define REPS       (1000)
#  define MAX_LEN    (256)
#endif


TEST(vect_sqrt, vect_s32_sqrt_prepare)
{

    typedef struct {
        struct {
            exponent_t exp;
            headroom_t hr;
        } B;

        struct {
            exponent_t a_exp;
            right_shift_t b_shr;
        } expected;

        unsigned line;
    } test_vector;

    test_vector test_vectors[] = {
        // B{  exp,  hr },      expected{  a_exp,  b_shr },         line
        {   {  -30,   0 },              {    -29,      2 },     __LINE__    },  //sqrt(1)
        {   {  -28,   2 },              {    -30,     -2 },     __LINE__    },  //sqrt(1)
        {   {    0,  26 },              {    -28,     -26},     __LINE__    },  //sqrt(16)
        {   {  -29,   0 },              {    -29,      1 },     __LINE__    },  //sqrt(2)

    };


    const unsigned N_vectors = sizeof(test_vectors) / sizeof(test_vector);

    const unsigned start_vector = 0;

    for(unsigned int v = start_vector; v < N_vectors; v++){

        test_vector* vector = &test_vectors[v];

        sprintf(msg_buff, "(vector @ line %u)", vector->line);

        exponent_t a_exp;
        right_shift_t b_shr;

        vect_s32_sqrt_prepare(&a_exp, &b_shr, vector->B.exp, vector->B.hr);

        int32_t B = 0x40000000 >> vector->B.hr;
        int32_t A;

        const headroom_t a_hr = vect_s32_sqrt(&A, &B, 1, b_shr, VECT_SQRT_S32_MAX_DEPTH);
        const int32_t p = vlmul32(A, A);
        const int32_t target = vlashr32(B, b_shr);
        const int32_t delta = target - p;

        TEST_ASSERT_EQUAL_MESSAGE(vector->expected.a_exp, a_exp, msg_buff);
        TEST_ASSERT_EQUAL_MESSAGE(vector->expected.b_shr, b_shr, msg_buff);
        TEST_ASSERT_LESS_OR_EQUAL_INT32_MESSAGE(target, p, msg_buff);
        TEST_ASSERT_LESS_OR_EQUAL_INT32_MESSAGE(1, delta, msg_buff);
        TEST_ASSERT_LESS_THAN_MESSAGE(2, a_hr, msg_buff);
    }

}


TEST(vect_sqrt, vect_s32_sqrt_prepare_saturation)
{
    // Regression test: bfp_complex_s32_squared_mag()'s exponent selection can
    // deliberately drive a headroom-0 element all the way to (or very near)
    // INT32_MAX. vect_s32_sqrt_prepare() used to leave b_shr such that this
    // fed straight into vect_s32_sqrt()'s digit-recurrence, whose internal
    // candidate-squaring then saturates and aliases with the target,
    // spuriously keeping every remaining bit and returning ~sqrt(2)x too
    // large a result. This is only visible against a real (double-precision)
    // reference -- the algorithm's own A^2<=target<(A+1)^2 invariant can
    // appear satisfied even when wrong, because both sides saturate
    // together at this exact edge.
    //
    // b_exp MUST be even here: with b_hr==0, the (unfixed) b_shr starts at 0,
    // and the existing odd-exponent parity fixup ("if ((b_exp+b_shr)%2==1)
    // b_shr+=1") would otherwise add the same +1 bit of headroom by
    // coincidence, masking the bug this test is meant to catch.
    static const int32_t WORD_ALIGNED values[] = { INT32_MAX, INT32_MAX - 1, 0x7FFFFFF0, 0x40000001 };
    const exponent_t b_exp = -30;

    for(unsigned v = 0; v < sizeof(values)/sizeof(values[0]); v++){
        int32_t WORD_ALIGNED B = values[v];
        const headroom_t b_hr = vect_s32_headroom(&B, 1);
        TEST_ASSERT_EQUAL(0, b_hr);

        exponent_t a_exp;
        right_shift_t b_shr;
        vect_s32_sqrt_prepare(&a_exp, &b_shr, b_exp, b_hr);

        int32_t WORD_ALIGNED A;
        vect_s32_sqrt(&A, &B, 1, b_shr, VECT_SQRT_S32_MAX_DEPTH);

        double expected = sqrt(ldexp((double) B, b_exp));
        double actual = ldexp((double) A, a_exp);

        TEST_ASSERT( fabs(expected - actual) <= ldexp(0.9, a_exp) );
    }
}


TEST(vect_sqrt, vect_s16_sqrt_prepare)
{

    typedef struct {
        struct {
            exponent_t exp;
            headroom_t hr;
        } B;

        struct {
            exponent_t a_exp;
            right_shift_t b_shr;
        } expected;

        unsigned line;
    } test_vector;

    test_vector test_vectors[] = {
        // B{  exp,  hr },      expected{  a_exp,  b_shr },         line
        {   {  -14,   0 },              {    -13,      2 },     __LINE__    },  //sqrt(1)
        {   {  -12,   2 },              {    -14,     -2 },     __LINE__    },  //sqrt(1)
        {   {    0,  10 },              {    -12,     -10},     __LINE__    },  //sqrt(16)
        {   {  -13,   0 },              {    -13,      1 },     __LINE__    },  //sqrt(2)

    };


    const unsigned N_vectors = sizeof(test_vectors) / sizeof(test_vector);

    const unsigned start_vector = 0;

    for(unsigned int v = start_vector; v < N_vectors; v++){

        test_vector* vector = &test_vectors[v];

        sprintf(msg_buff, "(vector @ line %u)", vector->line);

        exponent_t a_exp;
        right_shift_t b_shr;

        vect_s16_sqrt_prepare(&a_exp, &b_shr, vector->B.exp, vector->B.hr);

        int16_t WORD_ALIGNED B = 0x4000 >> vector->B.hr;
        int16_t WORD_ALIGNED A;

        const headroom_t a_hr = vect_s16_sqrt(&A, &B, 1, b_shr, VECT_SQRT_S16_MAX_DEPTH);
        int16_t p = vlmul16(A, A);
        const int16_t target = vlashr16(B, b_shr);

        // vlmul16 has a different shift in vx4b
#if defined(__VX4B__)
        p <<= 1;
#endif

        const int16_t delta = target - p;

        TEST_ASSERT_EQUAL_MESSAGE(vector->expected.a_exp, a_exp, msg_buff);
        TEST_ASSERT_EQUAL_MESSAGE(vector->expected.b_shr, b_shr, msg_buff);
#if defined(__VX4B__)
        TEST_ASSERT_LESS_OR_EQUAL_INT16_MESSAGE(target + 2, p, msg_buff);
#else
        TEST_ASSERT_LESS_OR_EQUAL_INT16_MESSAGE(target, p, msg_buff);
#endif
        TEST_ASSERT_LESS_OR_EQUAL_INT16_MESSAGE(1, delta, msg_buff);
        TEST_ASSERT_LESS_THAN_MESSAGE(2, a_hr, msg_buff);
    }

}


TEST(vect_sqrt, vect_s16_sqrt_prepare_saturation)
{
    // s16 counterpart of vect_s32_sqrt_prepare_saturation --
    // see that test for the full explanation
    
    // b_exp must be even (otherwise the existing odd-exponent parity fixup coincidentally
    // reserves the same 1 bit of headroom and masks the bug).
    static const int16_t WORD_ALIGNED values[] = { INT16_MAX, INT16_MAX - 1, 0x7FF0, 0x4001 };
    const exponent_t b_exp = -14;

    for(unsigned v = 0; v < sizeof(values)/sizeof(values[0]); v++){
        int16_t WORD_ALIGNED B  = values[v];
        const headroom_t b_hr = vect_s16_headroom(&B, 1);
        TEST_ASSERT_EQUAL(0, b_hr);

        exponent_t a_exp;
        right_shift_t b_shr;
        vect_s16_sqrt_prepare(&a_exp, &b_shr, b_exp, b_hr);

        int16_t WORD_ALIGNED A;
        vect_s16_sqrt(&A, &B, 1, b_shr, VECT_SQRT_S16_MAX_DEPTH);

        double expected = sqrt(ldexp((double) B, b_exp));
        double actual = ldexp((double) A, a_exp);

        TEST_ASSERT( fabs(expected - actual) <= ldexp(1.1, a_exp) );
    }
}


TEST(vect_sqrt, vect_s16_sqrt_A)
{

    unsigned seed = SEED_FROM_FUNC_NAME();


    int16_t WORD_ALIGNED B[MAX_LEN];
    int16_t WORD_ALIGNED A[MAX_LEN];


    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        const unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN-1);

        const exponent_t b_exp = pseudo_rand_int(&seed, -30, 30);
        headroom_t b_hr = pseudo_rand_uint(&seed, 0, 12);

        for(unsigned int i = 0; i < length; i++){
            B[i] = pseudo_rand_uint(&seed, 0, INT16_MAX) >> b_hr;
        }

        b_hr = vect_s16_headroom(B, length);

        exponent_t a_exp;
        right_shift_t b_shr;

        vect_s16_sqrt_prepare(&a_exp, &b_shr, b_exp, b_hr);


        const headroom_t a_hr = vect_s16_sqrt(A, B, length, b_shr, VECT_SQRT_S16_MAX_DEPTH);

        TEST_ASSERT_EQUAL(vect_s16_headroom(A, length), a_hr);

        TEST_ASSERT_EQUAL(b_exp+b_shr, 2*a_exp + 14);

        for(unsigned int i = 0; i < length; i++){

            int16_t target = vlashr16(B[i], b_shr);

            #if defined(__VX4B__)
            target >>= 1;
            #endif

            int16_t p = vlmul16(A[i], A[i]);
            int16_t p2 = vlmul16(A[i]+1, A[i]+1);

            #if defined(__VX4B__)
                TEST_ASSERT_LESS_OR_EQUAL_INT16(target+2, p);
            #else
                TEST_ASSERT_LESS_OR_EQUAL_INT16(target, p);
            #endif
            TEST_ASSERT_LESS_OR_EQUAL_INT16(2, target - p);
            TEST_ASSERT_GREATER_OR_EQUAL_INT16(target, p2);
        }
    }
}


TEST(vect_sqrt, vect_s16_sqrt_B)
{

    unsigned seed = SEED_FROM_FUNC_NAME();

    #define LENGTH ( 10 )
    int16_t WORD_ALIGNED B[LENGTH];
    int16_t WORD_ALIGNED A[LENGTH];
    int16_t WORD_ALIGNED A_full[LENGTH];

    for(unsigned int v = 0; v < REPS; v++){

        setExtraInfo_RS(v, seed);

        const exponent_t b_exp = pseudo_rand_int(&seed, -30, 30);
        headroom_t b_hr = pseudo_rand_uint(&seed, 0, 12);

        for(unsigned int i = 0; i < LENGTH; i++){
            B[i] = pseudo_rand_uint(&seed, 0, INT16_MAX) >> b_hr;
        }

        b_hr = vect_s16_headroom(B, LENGTH);

        exponent_t a_exp;
        right_shift_t b_shr;

        vect_s16_sqrt_prepare(&a_exp, &b_shr, b_exp, b_hr);

        vect_s16_sqrt(A_full, B, LENGTH, b_shr, VECT_SQRT_S16_MAX_DEPTH);

        for(unsigned depth = 1; depth <= VECT_SQRT_S16_MAX_DEPTH; depth++){

            vect_s16_sqrt(A, B, LENGTH, b_shr, depth);

            const uint16_t mask = (0x7FFF >> depth) ^ 0xFFFFFFFF;

            for(unsigned int i = 0; i < LENGTH; i++){

                TEST_ASSERT_LESS_OR_EQUAL(A_full[i], A[i]);

                const int16_t A_masked = A_full[i] & mask;

                TEST_ASSERT_EQUAL(A_masked, A[i]);
            }
        }
    }
}


TEST(vect_sqrt, vect_s32_sqrt_A)
{

    unsigned seed = SEED_FROM_FUNC_NAME();


    int32_t B[MAX_LEN];
    int32_t A[MAX_LEN];


    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        const unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN-1);

        const exponent_t b_exp = pseudo_rand_int(&seed, -30, 30);
        headroom_t b_hr = pseudo_rand_uint(&seed, 0, 28);

        for(unsigned int i = 0; i < length; i++){
            B[i] = pseudo_rand_uint(&seed, 0, INT32_MAX) >> b_hr;
        }

        b_hr = vect_s32_headroom(B, length);

        exponent_t a_exp;
        right_shift_t b_shr;

        vect_s32_sqrt_prepare(&a_exp, &b_shr, b_exp, b_hr);


        const headroom_t a_hr = vect_s32_sqrt(A, B, length, b_shr, VECT_SQRT_S32_MAX_DEPTH);

        TEST_ASSERT_EQUAL(vect_s32_headroom(A, length), a_hr);

        TEST_ASSERT_EQUAL(b_exp+b_shr, 2*a_exp + 30);

        for(unsigned int i = 0; i < length; i++){

            int32_t target = vlashr32(B[i], b_shr);

            int32_t p = vlmul32(A[i], A[i]);
            int32_t p2 = vlmul32(A[i]+1, A[i]+1);

            TEST_ASSERT_LESS_OR_EQUAL_INT32(target, p);
            TEST_ASSERT_GREATER_OR_EQUAL_INT32(target, p2);
        }
    }
}


TEST(vect_sqrt, vect_s32_sqrt_B)
{

    unsigned seed = SEED_FROM_FUNC_NAME();


    #define LENGTH ( 10 )

    int32_t B[LENGTH];
    int32_t A[LENGTH];
    int32_t A_full[LENGTH];

    for(unsigned int v = 0; v < REPS; v++){

        setExtraInfo_RS(v, seed);

        const exponent_t b_exp = pseudo_rand_int(&seed, -30, 30);
        headroom_t b_hr = pseudo_rand_uint(&seed, 0, 28);

        for(unsigned int i = 0; i < LENGTH; i++){
            B[i] = pseudo_rand_uint(&seed, 0, INT32_MAX) >> b_hr;
        }

        b_hr = vect_s32_headroom(B, LENGTH);

        exponent_t a_exp;
        right_shift_t b_shr;

        vect_s32_sqrt_prepare(&a_exp, &b_shr, b_exp, b_hr);

        vect_s32_sqrt(A_full, B, LENGTH, b_shr, VECT_SQRT_S32_MAX_DEPTH);

        for(unsigned depth = 1; depth <= VECT_SQRT_S32_MAX_DEPTH; depth++){

            vect_s32_sqrt(A, B, LENGTH, b_shr, depth);

            const uint32_t mask = (0x7FFFFFFF >> depth) ^ 0xFFFFFFFF;

            for(unsigned int i = 0; i < LENGTH; i++){

                TEST_ASSERT_LESS_OR_EQUAL(A_full[i], A[i]);

                const int32_t A_masked = A_full[i] & mask;

                TEST_ASSERT_EQUAL(A_masked, A[i]);
            }
        }
    }
}

