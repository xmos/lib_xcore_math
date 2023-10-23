// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xmath/xmath.h"

#include "../tst_common.h"

#include "unity_fixture.h"


TEST_GROUP_RUNNER(vect_extract) {
  RUN_TEST_CASE(vect_extract, vect_s16_extract_high_byte);
  RUN_TEST_CASE(vect_extract, vect_s16_extract_low_byte);
}

TEST_GROUP(vect_extract);
TEST_SETUP(vect_extract) { fflush(stdout); }
TEST_TEAR_DOWN(vect_extract) {}



#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (128)
#else
#  define REPS       (1000)
#  define MAX_LEN    (1024)
#endif


TEST(vect_extract, vect_s16_extract_high_byte)
{
    unsigned seed = SEED_FROM_FUNC_NAME();


    int8_t WORD_ALIGNED A[MAX_LEN];
    int16_t WORD_ALIGNED B[MAX_LEN];

    for(unsigned int v = 0; v < REPS; v++){

        unsigned length = (v < 48)? v :  pseudo_rand_uint(&seed, 1, MAX_LEN+1);

        setExtraInfo_RSL(v, seed, length);

        for(unsigned int k = 0; k < length; k++)
          B[k] = pseudo_rand_int16(&seed);

        B[0] = 0x8000;
        B[1] = 0x8001;
        B[2] = 0x80FF;

        memset(A, 0xCC, sizeof(A));


        vect_s16_extract_high_byte(A, B, length);

        for(unsigned int i = 0; i < length; i++) {
          int8_t should_be = (int8_t) (((B[i] & 0xFF00) >> 8) & 0x00FF);

          if(A[i] != should_be){
            printf("B[%d] = 0x%04X\n", i, B[i]);
            printf("A[%d] = 0x%02X\n", i, A[i]);
          }

          TEST_ASSERT_EQUAL_INT8(should_be, A[i]);
        }

        for(int i = length; i < MAX_LEN; i++){

          if(A[i] != (int8_t)0xCC){
            printf("A[%d] = 0x%02X\n", i, A[i]);
          }
          TEST_ASSERT_EQUAL_INT8(0xCC, A[i]);
        }
        
    }
}


TEST(vect_extract, vect_s16_extract_low_byte)
{
    unsigned seed = SEED_FROM_FUNC_NAME();


    int8_t WORD_ALIGNED A[MAX_LEN];
    int16_t WORD_ALIGNED B[MAX_LEN];

    for(unsigned int v = 0; v < REPS; v++){

        unsigned length = (v < 48)? v :  pseudo_rand_uint(&seed, 1, MAX_LEN+1);

        setExtraInfo_RSL(v, seed, length);

        for(unsigned int k = 0; k < length; k++)
          B[k] = pseudo_rand_int16(&seed);

        B[0] = 0x0080;
        B[1] = 0x0180;
        B[2] = 0xFF80;

        memset(A, 0xCC, sizeof(A));

        vect_s16_extract_low_byte(A, B, length);

        for(unsigned int i = 0; i < length; i++) {
          int8_t should_be = (int8_t) (B[i] & 0x00FF);

          if(A[i] != should_be){
            printf("B[%d] = 0x%04X\n", i, B[i]);
            printf("A[%d] = 0x%02X\n", i, A[i]);
          }

          TEST_ASSERT_EQUAL_INT8(should_be, A[i]);
        }

        for(int i = length; i < MAX_LEN; i++){

          if(A[i] != (int8_t)0xCC){
            printf("A[%d] = 0x%02X\n", i, A[i]);
          }
          TEST_ASSERT_EQUAL_INT8(0xCC, A[i]);
        }
        
    }
}

