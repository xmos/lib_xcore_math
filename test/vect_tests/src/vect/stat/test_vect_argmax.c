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


TEST_GROUP_RUNNER(vect_argmax) {
  RUN_TEST_CASE(vect_argmax, vect_s16_argmax_A);
  RUN_TEST_CASE(vect_argmax, vect_s16_argmax_B);
  RUN_TEST_CASE(vect_argmax, vect_s32_argmax_A);
  RUN_TEST_CASE(vect_argmax, vect_s32_argmax_B);
}

TEST_GROUP(vect_argmax);
TEST_SETUP(vect_argmax) { fflush(stdout); }
TEST_TEAR_DOWN(vect_argmax) {}



#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (64)
#else
#  define REPS       (1000)
#  define MAX_LEN    (256)
#endif


TEST(vect_argmax, vect_s16_argmax_A)
{
    

    // Elements 0 through 31 are the data. element 33 is what's expected. The remaining elements just keep
    // word alignment at the start of each row.
    int16_t WORD_ALIGNED data[][36] = {
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,     0,  },
        { 0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,     1,  },
        { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,    31,  },
        { 1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,     0,  },
        { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,     1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,     0,  },
        { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,     1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    16,  },
        { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,     1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,     0,  },
        { 0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,     1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,     1,  },
    };


    TEST_ASSERT_EQUAL(data[0][32], vect_s16_argmax(&data[0][0],32));
    TEST_ASSERT_EQUAL(data[1][32], vect_s16_argmax(&data[1][0],32));
    TEST_ASSERT_EQUAL(data[2][32], vect_s16_argmax(&data[2][0],32));
    TEST_ASSERT_EQUAL(data[3][32], vect_s16_argmax(&data[3][0],32));
    TEST_ASSERT_EQUAL(data[4][32], vect_s16_argmax(&data[4][0],32));
    TEST_ASSERT_EQUAL(data[5][32], vect_s16_argmax(&data[5][0],32));
    TEST_ASSERT_EQUAL(data[6][32], vect_s16_argmax(&data[6][0],32));
    TEST_ASSERT_EQUAL(data[7][32], vect_s16_argmax(&data[7][0],32));

}


TEST(vect_argmax, vect_s16_argmax_B)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    unsigned result;
    int16_t B[MAX_LEN];

    for(unsigned int v = 0; v < REPS; v++){
        const unsigned init_seed = seed;

        unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        // len = 1;

        setExtraInfo_RSL(v, init_seed, len);

        unsigned exp = 0;

        for(unsigned int i = 0; i < len; i++){
            B[i] = pseudo_rand_int16(&seed);
            exp = (B[i] > B[exp])? i : exp;
        }

        result = vect_s16_argmax(B, len);

        TEST_ASSERT_EQUAL(exp, result);
    }
}


TEST(vect_argmax, vect_s32_argmax_A)
{
    

    // Elements 0 through 31 are the data. element 33 is what's expected. 
    int32_t data[][33] = {
        { 1,0,0,0,0,0,0,0,     0,0,0,0,0,0,0,0,     0,0,0,0,0,0,0,0,     0,0,0,0,0,0,0,0,     0,  },
        { 0,1,0,0,0,0,0,0,     0,0,0,0,0,0,0,0,     0,0,0,0,0,0,0,0,     0,0,0,0,0,0,0,0,     1,  },
        { 0,0,0,0,0,0,0,0,     0,0,0,0,0,0,0,0,     0,0,0,0,0,0,0,0,     0,0,0,0,0,0,0,1,    31,  },
        { 1,1,0,0,0,0,0,0,     0,0,0,0,0,0,0,0,     0,0,0,0,0,0,0,0,     0,0,0,0,0,0,0,0,     0,  },
        { 1,0,0,0,0,0,0,0,     0,0,0,0,0,0,0,0,     1,0,0,0,0,0,0,0,     0,0,0,0,0,0,0,0,     0,  },
        { 0,0,0,0,0,0,0,0,     0,0,0,0,0,0,0,0,     1,1,0,0,0,0,0,0,     0,0,0,0,0,0,0,0,    16,  },
        { 1,1,1,1,1,1,1,1,     1,1,1,1,1,1,1,1,     1,1,1,1,1,1,1,1,     1,1,1,1,1,1,1,1,     0,  },
        { 0,1,1,1,1,1,1,1,     1,1,1,1,1,1,1,1,     1,1,1,1,1,1,1,1,     1,1,1,1,1,1,1,1,     1,  },
    };


    // TEST_ASSERT_EQUAL(data[0][32], vect_s32_argmax(&data[0][0],32));
    // TEST_ASSERT_EQUAL(data[1][32], vect_s32_argmax(&data[1][0],32));
    TEST_ASSERT_EQUAL(data[2][32], vect_s32_argmax(&data[2][0],32));
    TEST_ASSERT_EQUAL(data[3][32], vect_s32_argmax(&data[3][0],32));
    TEST_ASSERT_EQUAL(data[4][32], vect_s32_argmax(&data[4][0],32));
    TEST_ASSERT_EQUAL(data[5][32], vect_s32_argmax(&data[5][0],32));
    TEST_ASSERT_EQUAL(data[6][32], vect_s32_argmax(&data[6][0],32));
    TEST_ASSERT_EQUAL(data[7][32], vect_s32_argmax(&data[7][0],32));

}


TEST(vect_argmax, vect_s32_argmax_B)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    unsigned result;
    int32_t B[MAX_LEN];

    for(unsigned int v = 0; v < REPS; v++){
        const unsigned init_seed = seed;

        unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN+1);

        setExtraInfo_RSL(v, init_seed, len);

        unsigned exp = 0;

        for(unsigned int i = 0; i < len; i++){
            // printf("i = %d     &B = 0x%08X\n", i, (unsigned) B);
            B[i] = pseudo_rand_int32(&seed);
            exp = (B[i] > B[exp])? i : exp;
        }

        result = vect_s32_argmax(B, len);

        TEST_ASSERT_EQUAL(exp, result);
        
    }
}


