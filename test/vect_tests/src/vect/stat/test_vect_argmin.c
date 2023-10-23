// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "xmath/xmath.h"

#include "../../tst_common.h"

#include "unity_fixture.h"


TEST_GROUP_RUNNER(vect_argmin) {
  RUN_TEST_CASE(vect_argmin, vect_s16_argmin_A);
  RUN_TEST_CASE(vect_argmin, vect_s16_argmin_B);
  RUN_TEST_CASE(vect_argmin, vect_s32_argmin_A);
  RUN_TEST_CASE(vect_argmin, vect_s32_argmin_B);
}

TEST_GROUP(vect_argmin);
TEST_SETUP(vect_argmin) { fflush(stdout); }
TEST_TEAR_DOWN(vect_argmin) {}



#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (64)
#else
#  define REPS       (1000)
#  define MAX_LEN    (256)
#endif


TEST(vect_argmin, vect_s16_argmin_A)
{
    

    // Elements 0 through 31 are the data. element 33 is what's expected. The remaining elements just keep
    // word alignment at the start of each row.
    int16_t WORD_ALIGNED data[][36] = {
        { 1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,     2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,     0,  },
        { 2,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,     2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,     1,  },
        { 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,     2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,    31,  },
        { 1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,     2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,     0,  },
        { 1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,     1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,     0,  },
        { 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,     1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,    16,  },
        { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,     1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,     0,  },
        { 2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,     1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,     1,  },
    };


    TEST_ASSERT_EQUAL(data[0][32], vect_s16_argmin(&data[0][0],32));
    TEST_ASSERT_EQUAL(data[1][32], vect_s16_argmin(&data[1][0],32));
    TEST_ASSERT_EQUAL(data[2][32], vect_s16_argmin(&data[2][0],32));
    TEST_ASSERT_EQUAL(data[3][32], vect_s16_argmin(&data[3][0],32));
    TEST_ASSERT_EQUAL(data[4][32], vect_s16_argmin(&data[4][0],32));
    TEST_ASSERT_EQUAL(data[5][32], vect_s16_argmin(&data[5][0],32));
    TEST_ASSERT_EQUAL(data[6][32], vect_s16_argmin(&data[6][0],32));
    TEST_ASSERT_EQUAL(data[7][32], vect_s16_argmin(&data[7][0],32));

}


TEST(vect_argmin, vect_s16_argmin_B)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    unsigned result;
    int16_t B[MAX_LEN];

    for(unsigned int v = 0; v < REPS; v++){

        unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        // len = 32;

        setExtraInfo_RSL(v, seed, len);

        unsigned exp = 0;

        for(unsigned int i = 0; i < len; i++){
            B[i] = pseudo_rand_int16(&seed);
            exp = (B[i] < B[exp])? i : exp;
        }

        result = vect_s16_argmin(B, len);

        TEST_ASSERT_EQUAL(exp, result);
    }
}


TEST(vect_argmin, vect_s32_argmin_A)
{
    

    // Elements 0 through 31 are the data. element 33 is what's expected.
    int32_t data[][33] = {
        { 1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,     2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,     0,  },
        { 2,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,     2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,     1,  },
        { 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,     2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,    31,  },
        { 1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,     2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,     0,  },
        { 1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,     1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,     0,  },
        { 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,     1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,    16,  },
        { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,     1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,     0,  },
        { 2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,     1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,     1,  },
    };


    TEST_ASSERT_EQUAL(data[0][32], vect_s32_argmin(&data[0][0],32));
    TEST_ASSERT_EQUAL(data[1][32], vect_s32_argmin(&data[1][0],32));
    TEST_ASSERT_EQUAL(data[2][32], vect_s32_argmin(&data[2][0],32));
    TEST_ASSERT_EQUAL(data[3][32], vect_s32_argmin(&data[3][0],32));
    TEST_ASSERT_EQUAL(data[4][32], vect_s32_argmin(&data[4][0],32));
    TEST_ASSERT_EQUAL(data[5][32], vect_s32_argmin(&data[5][0],32));
    TEST_ASSERT_EQUAL(data[6][32], vect_s32_argmin(&data[6][0],32));
    TEST_ASSERT_EQUAL(data[7][32], vect_s32_argmin(&data[7][0],32));

}


TEST(vect_argmin, vect_s32_argmin_B)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    unsigned result;
    int32_t B[MAX_LEN];

    for(unsigned int v = 0; v < REPS; v++){

        unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN+1);

        setExtraInfo_RSL(v, seed, len);

        unsigned exp = 0;

        for(unsigned int i = 0; i < len; i++){
            B[i] = pseudo_rand_int32(&seed);
            exp = (B[i] < B[exp])? i : exp;
        }

        result = vect_s32_argmin(B, len);

        TEST_ASSERT_EQUAL(exp, result);
        
    }
}
#undef MAX_LEN
#undef REPS

