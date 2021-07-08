// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xs3_math.h"

#include "../tst_common.h"

#include "unity.h"


static unsigned seed = 56745686;

#define MAX_LEN     1023
#define REPS        (1000)




static void test_xs3_vect_s8_is_negative()
{
    printf("%s...\n", __func__);

    seed = 765467;

    int8_t WORD_ALIGNED A[MAX_LEN];
    int8_t WORD_ALIGNED B[MAX_LEN];

    for(int v = 0; v < REPS; v++){

        unsigned length = (v < 48)? v :  pseudo_rand_uint(&seed, 1, MAX_LEN+1);

        printf("\trep % 3d...\t(seed: 0x%08X) (length: %u)\n", v, seed, length);

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



void test_xs3_vect_s8_boolean()
{
    SET_TEST_FILE();

    RUN_TEST(test_xs3_vect_s8_is_negative);

}
