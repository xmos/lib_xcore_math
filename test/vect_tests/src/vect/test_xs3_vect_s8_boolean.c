// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xs3_math.h"

#include "../tst_common.h"

#include "unity_fixture.h"


TEST_GROUP_RUNNER(xs3_vect_boolean) {
  RUN_TEST_CASE(xs3_vect_boolean, xs3_vect_s8_is_negative);
}

TEST_GROUP(xs3_vect_boolean);
TEST_SETUP(xs3_vect_boolean) { fflush(stdout); }
TEST_TEAR_DOWN(xs3_vect_boolean) {}



#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (64)
#else
#  define REPS       (1000)
#  define MAX_LEN    (256)
#endif


TEST(xs3_vect_boolean, xs3_vect_s8_is_negative)
{
    unsigned seed = SEED_FROM_FUNC_NAME();


    int8_t WORD_ALIGNED A[MAX_LEN];
    int8_t WORD_ALIGNED B[MAX_LEN];

    for(int v = 0; v < REPS; v++){

        unsigned length = (v < 48)? v :  pseudo_rand_uint(&seed, 1, MAX_LEN+1);

        setExtraInfo_RSL(v, seed, length);

        for(int k = 0; k < length; k++)
          B[k] = pseudo_rand_int8(&seed);

        memset(A, 0xCC, sizeof(A));

        xs3_vect_s8_is_negative(A, B, length);

        for(int i = 0; i < MAX_LEN; i++) {
          int8_t expected = (B[i] < 0)? 1 : 0;
          expected = (i >= length)? 0xCC : expected;

          if(A[i] != expected) printf("A[%d] = 0x%02X\n", i, A[i]);
          
          TEST_ASSERT_EQUAL_INT8(expected, A[i]);
        }
    }
}

