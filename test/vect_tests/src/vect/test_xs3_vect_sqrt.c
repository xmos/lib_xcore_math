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

#include "unity_fixture.h"


TEST_GROUP_RUNNER(xs3_vect_sqrt) {
  RUN_TEST_CASE(xs3_vect_sqrt, xs3_vect_s32_sqrt_prepare);
  RUN_TEST_CASE(xs3_vect_sqrt, xs3_vect_s16_sqrt_prepare);
  RUN_TEST_CASE(xs3_vect_sqrt, xs3_vect_s16_sqrt_A);
  RUN_TEST_CASE(xs3_vect_sqrt, xs3_vect_s16_sqrt_B);
  RUN_TEST_CASE(xs3_vect_sqrt, xs3_vect_s32_sqrt_A);
  RUN_TEST_CASE(xs3_vect_sqrt, xs3_vect_s32_sqrt_B);
}

TEST_GROUP(xs3_vect_sqrt);
TEST_SETUP(xs3_vect_sqrt) { fflush(stdout); }
TEST_TEAR_DOWN(xs3_vect_sqrt) {}


static char msg_buff[200];



#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (64)
#else
#  define REPS       (1000)
#  define MAX_LEN    (256)
#endif


TEST(xs3_vect_sqrt, xs3_vect_s32_sqrt_prepare)
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
        {   {  -30,   0 },              {    -30,      0 },     __LINE__    },  //sqrt(1)
        {   {  -28,   2 },              {    -30,     -2 },     __LINE__    },  //sqrt(1)
        {   {    0,  26 },              {    -28,     -26},     __LINE__    },  //sqrt(16)
        {   {  -29,   0 },              {    -29,      1 },     __LINE__    },  //sqrt(2)

    };


    const unsigned N_vectors = sizeof(test_vectors) / sizeof(test_vector);

    const unsigned start_vector = 0;

    for(int v = start_vector; v < N_vectors; v++){

        test_vector* vector = &test_vectors[v];

        sprintf(msg_buff, "(vector @ line %u)", vector->line);
        
        exponent_t a_exp;
        right_shift_t b_shr;

        xs3_vect_s32_sqrt_prepare(&a_exp, &b_shr, vector->B.exp, vector->B.hr);

        int32_t B = 0x40000000 >> vector->B.hr;
        int32_t A;

        const headroom_t a_hr = xs3_vect_s32_sqrt(&A, &B, 1, b_shr, XS3_VECT_SQRT_S32_MAX_DEPTH);
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


TEST(xs3_vect_sqrt, xs3_vect_s16_sqrt_prepare)
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
        {   {  -14,   0 },              {    -14,      0 },     __LINE__    },  //sqrt(1)
        {   {  -12,   2 },              {    -14,     -2 },     __LINE__    },  //sqrt(1)
        {   {    0,  10 },              {    -12,     -10},     __LINE__    },  //sqrt(16)
        {   {  -13,   0 },              {    -13,      1 },     __LINE__    },  //sqrt(2)

    };


    const unsigned N_vectors = sizeof(test_vectors) / sizeof(test_vector);

    const unsigned start_vector = 0;

    for(int v = start_vector; v < N_vectors; v++){

        test_vector* vector = &test_vectors[v];

        sprintf(msg_buff, "(vector @ line %u)", vector->line);
        
        exponent_t a_exp;
        right_shift_t b_shr;

        xs3_vect_s16_sqrt_prepare(&a_exp, &b_shr, vector->B.exp, vector->B.hr);

        int16_t WORD_ALIGNED B = 0x4000 >> vector->B.hr;
        int16_t WORD_ALIGNED A;

        const headroom_t a_hr = xs3_vect_s16_sqrt(&A, &B, 1, b_shr, XS3_VECT_SQRT_S16_MAX_DEPTH);
        const int16_t p = vlmul16(A, A);
        const int16_t target = vlashr16(B, b_shr);
        const int16_t delta = target - p;

        TEST_ASSERT_EQUAL_MESSAGE(vector->expected.a_exp, a_exp, msg_buff);
        TEST_ASSERT_EQUAL_MESSAGE(vector->expected.b_shr, b_shr, msg_buff);
        TEST_ASSERT_LESS_OR_EQUAL_INT16_MESSAGE(target, p, msg_buff);
        TEST_ASSERT_LESS_OR_EQUAL_INT16_MESSAGE(1, delta, msg_buff);
        TEST_ASSERT_LESS_THAN_MESSAGE(2, a_hr, msg_buff);
    }

}


TEST(xs3_vect_sqrt, xs3_vect_s16_sqrt_A)
{

    
    unsigned seed = SEED_FROM_FUNC_NAME();


    int16_t WORD_ALIGNED B[MAX_LEN];
    int16_t WORD_ALIGNED A[MAX_LEN];


    for(int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        const unsigned length = pseudo_rand_uint(&seed, 0, MAX_LEN-1);

        const exponent_t b_exp = pseudo_rand_int(&seed, -30, 30);
        headroom_t b_hr = pseudo_rand_uint(&seed, 0, 12);

        for(int i = 0; i < length; i++){
            B[i] = pseudo_rand_uint(&seed, 0, INT16_MAX) >> b_hr;
        }

        b_hr = xs3_vect_s16_headroom(B, length);

        exponent_t a_exp;
        right_shift_t b_shr;

        xs3_vect_s16_sqrt_prepare(&a_exp, &b_shr, b_exp, b_hr);


        const headroom_t a_hr = xs3_vect_s16_sqrt(A, B, length, b_shr, XS3_VECT_SQRT_S16_MAX_DEPTH);

        TEST_ASSERT_EQUAL(xs3_vect_s16_headroom(A, length), a_hr);

        TEST_ASSERT_EQUAL(b_exp+b_shr, 2*a_exp + 14);

        for(int i = 0; i < length; i++){

            int16_t target = vlashr16(B[i], b_shr);

            int16_t p = vlmul16(A[i], A[i]);
            int16_t p2 = vlmul16(A[i]+1, A[i]+1);

            TEST_ASSERT_LESS_OR_EQUAL_INT16(target, p);
            TEST_ASSERT_GREATER_OR_EQUAL_INT16(target, p2);
        }
    }
}


TEST(xs3_vect_sqrt, xs3_vect_s16_sqrt_B)
{

    
    unsigned seed = SEED_FROM_FUNC_NAME();


    const unsigned length = 10;

    int16_t WORD_ALIGNED B[length];
    int16_t WORD_ALIGNED A[length];
    int16_t WORD_ALIGNED A_full[length];

    for(int v = 0; v < REPS; v++){

        setExtraInfo_RS(v, seed);

        const exponent_t b_exp = pseudo_rand_int(&seed, -30, 30);
        headroom_t b_hr = pseudo_rand_uint(&seed, 0, 12);

        for(int i = 0; i < length; i++){
            B[i] = pseudo_rand_uint(&seed, 0, INT16_MAX) >> b_hr;
        }

        b_hr = xs3_vect_s16_headroom(B, length);

        exponent_t a_exp;
        right_shift_t b_shr;

        xs3_vect_s16_sqrt_prepare(&a_exp, &b_shr, b_exp, b_hr);

        xs3_vect_s16_sqrt(A_full, B, length, b_shr, XS3_VECT_SQRT_S16_MAX_DEPTH);

        for(unsigned depth = 1; depth <= XS3_VECT_SQRT_S16_MAX_DEPTH; depth++){

            xs3_vect_s16_sqrt(A, B, length, b_shr, depth);

            const uint16_t mask = (0x7FFF >> depth) ^ 0xFFFFFFFF;

            for(int i = 0; i < length; i++){

                TEST_ASSERT_LESS_OR_EQUAL(A_full[i], A[i]);

                const int16_t A_masked = A_full[i] & mask;

                TEST_ASSERT_EQUAL(A_masked, A[i]);
            }
        }
    }
}


TEST(xs3_vect_sqrt, xs3_vect_s32_sqrt_A)
{

    
    unsigned seed = SEED_FROM_FUNC_NAME();


    int32_t B[MAX_LEN];
    int32_t A[MAX_LEN];


    for(int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        const unsigned length = pseudo_rand_uint(&seed, 0, MAX_LEN-1);

        const exponent_t b_exp = pseudo_rand_int(&seed, -30, 30);
        headroom_t b_hr = pseudo_rand_uint(&seed, 0, 28);

        for(int i = 0; i < length; i++){
            B[i] = pseudo_rand_uint(&seed, 0, INT32_MAX) >> b_hr;
        }

        b_hr = xs3_vect_s32_headroom(B, length);

        exponent_t a_exp;
        right_shift_t b_shr;

        xs3_vect_s32_sqrt_prepare(&a_exp, &b_shr, b_exp, b_hr);


        const headroom_t a_hr = xs3_vect_s32_sqrt(A, B, length, b_shr, XS3_VECT_SQRT_S32_MAX_DEPTH);

        TEST_ASSERT_EQUAL(xs3_vect_s32_headroom(A, length), a_hr);

        TEST_ASSERT_EQUAL(b_exp+b_shr, 2*a_exp + 30);

        for(int i = 0; i < length; i++){

            int32_t target = vlashr32(B[i], b_shr);

            int32_t p = vlmul32(A[i], A[i]);
            int32_t p2 = vlmul32(A[i]+1, A[i]+1);

            TEST_ASSERT_LESS_OR_EQUAL_INT32(target, p);
            TEST_ASSERT_GREATER_OR_EQUAL_INT32(target, p2);
        }
    }
}


TEST(xs3_vect_sqrt, xs3_vect_s32_sqrt_B)
{

    
    unsigned seed = SEED_FROM_FUNC_NAME();


    const unsigned length = 10;

    int32_t B[length];
    int32_t A[length];
    int32_t A_full[length];

    for(int v = 0; v < REPS; v++){

        setExtraInfo_RS(v, seed);

        const exponent_t b_exp = pseudo_rand_int(&seed, -30, 30);
        headroom_t b_hr = pseudo_rand_uint(&seed, 0, 28);

        for(int i = 0; i < length; i++){
            B[i] = pseudo_rand_uint(&seed, 0, INT32_MAX) >> b_hr;
        }

        b_hr = xs3_vect_s32_headroom(B, length);

        exponent_t a_exp;
        right_shift_t b_shr;

        xs3_vect_s32_sqrt_prepare(&a_exp, &b_shr, b_exp, b_hr);

        xs3_vect_s32_sqrt(A_full, B, length, b_shr, XS3_VECT_SQRT_S32_MAX_DEPTH);

        for(unsigned depth = 1; depth <= XS3_VECT_SQRT_S32_MAX_DEPTH; depth++){

            xs3_vect_s32_sqrt(A, B, length, b_shr, depth);

            const uint32_t mask = (0x7FFFFFFF >> depth) ^ 0xFFFFFFFF;

            for(int i = 0; i < length; i++){

                TEST_ASSERT_LESS_OR_EQUAL(A_full[i], A[i]);

                const int32_t A_masked = A_full[i] & mask;

                TEST_ASSERT_EQUAL(A_masked, A[i]);
            }
        }
    }
}

