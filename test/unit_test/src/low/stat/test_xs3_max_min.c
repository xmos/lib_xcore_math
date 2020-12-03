
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "xs3_math.h"

#include "../../tst_common.h"

#include "unity.h"

static unsigned seed = 2314567;
static char msg_buff[200];


#define TEST_ASSERT_EQUAL_MSG(EXPECTED, ACTUAL, LINE_NUM)   do{       \
    if((EXPECTED)!=(ACTUAL)) {                                        \
      sprintf(msg_buff, "(test vector @ line %u)", (LINE_NUM));       \
      TEST_ASSERT_EQUAL_MESSAGE((EXPECTED), (ACTUAL), msg_buff);      \
    }} while(0)

#if DEBUG_ON || 0
#undef DEBUG_ON
#define DEBUG_ON    (1)
#endif



#define MAX_LEN     256
#define REPS        1000





static void test_xs3_vect_s16_max_random()
{
    PRINTF("%s...\n", __func__);
    seed = 0x6D734F1E;

    int16_t result;
    int16_t B[MAX_LEN];

    for(int v = 0; v < REPS; v++){

        unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        // len = 32;

        PRINTF("\trepetition %d.. (seed: 0x%08X; len: %u)\n", v, seed, len);

        int16_t max = -0x8000;

        for(int i = 0; i < len; i++){
            B[i] = pseudo_rand_int16(&seed);
            max = MAX(max, B[i]);
        }

        result = xs3_vect_s16_max(B, len);

        TEST_ASSERT_EQUAL(max, result);
    }
}







static void test_xs3_vect_s32_max_random()
{
    PRINTF("%s...\n", __func__);
    seed = 346332123;

    int32_t result;
    int32_t B[MAX_LEN];

    for(int v = 0; v < REPS; v++){

        unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        // len = 8;

        PRINTF("\trepetition %d.. (seed: 0x%08X; len: %u)\n", v, seed, len);

        int32_t max = -0x80000000;

        for(int i = 0; i < len; i++){
            B[i] = pseudo_rand_int32(&seed);
            max = MAX(max, B[i]);
        }

        result = xs3_vect_s32_max(B, len);

        TEST_ASSERT_EQUAL(max, result);
        
    }
}









static void test_xs3_vect_s16_min_random()
{
    PRINTF("%s...\n", __func__);
    seed = 0x6D734F1E;

    int16_t result;
    int16_t B[MAX_LEN];

    for(int v = 0; v < REPS; v++){

        unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        // len = 32;

        PRINTF("\trepetition %d.. (seed: 0x%08X; len: %u)\n", v, seed, len);

        int16_t min = 0x7FFF;

        for(int i = 0; i < len; i++){
            B[i] = pseudo_rand_int16(&seed);
            min = MIN(min, B[i]);
        }

        result = xs3_vect_s16_min(B, len);

        TEST_ASSERT_EQUAL(min, result);
    }
}









static void test_xs3_vect_s32_min_random()
{
    PRINTF("%s...\n", __func__);
    seed = 346332123;

    int32_t result;
    int32_t B[MAX_LEN];

    for(int v = 0; v < REPS; v++){

        unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        // len = 1;

        PRINTF("\trepetition %d.. (seed: 0x%08X; len: %u)\n", v, seed, len);

        int32_t min = 0x7FFFFFFF;

        for(int i = 0; i < len; i++){
            // B[i] = pseudo_rand_int32(&seed);
            // B[i] = i;
            min = MIN(min, B[i]);
        }

        result = xs3_vect_s32_min(B, len);

        TEST_ASSERT_EQUAL(min, result);
        
    }
}




static void test_xs3_vect_s16_argmax_A()
{
    PRINTF("%s...\n", __func__);

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


    TEST_ASSERT_EQUAL(data[0][32], xs3_vect_s16_argmax(&data[0][0],32));
    TEST_ASSERT_EQUAL(data[1][32], xs3_vect_s16_argmax(&data[1][0],32));
    TEST_ASSERT_EQUAL(data[2][32], xs3_vect_s16_argmax(&data[2][0],32));
    TEST_ASSERT_EQUAL(data[3][32], xs3_vect_s16_argmax(&data[3][0],32));
    TEST_ASSERT_EQUAL(data[4][32], xs3_vect_s16_argmax(&data[4][0],32));
    TEST_ASSERT_EQUAL(data[5][32], xs3_vect_s16_argmax(&data[5][0],32));
    TEST_ASSERT_EQUAL(data[6][32], xs3_vect_s16_argmax(&data[6][0],32));
    TEST_ASSERT_EQUAL(data[7][32], xs3_vect_s16_argmax(&data[7][0],32));

}




static void test_xs3_vect_s16_argmax_B()
{
    PRINTF("%s...\n", __func__);
    seed = 0x6D734F1E;

    unsigned result;
    int16_t B[MAX_LEN];

    for(int v = 0; v < REPS; v++){
        const unsigned init_seed = seed;

        unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        // len = 1;

        PRINTF("\trepetition %d.. (seed: 0x%08X; len: %u)\n", v, init_seed, len);

        unsigned exp = 0;

        for(int i = 0; i < len; i++){
            B[i] = pseudo_rand_int16(&seed);
            exp = (B[i] > B[exp])? i : exp;
        }

        result = xs3_vect_s16_argmax(B, len);

        TEST_ASSERT_EQUAL(exp, result);
    }
}







static void test_xs3_vect_s32_argmax_A()
{
    PRINTF("%s...\n", __func__);

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


    // TEST_ASSERT_EQUAL(data[0][32], xs3_vect_s32_argmax(&data[0][0],32));
    // TEST_ASSERT_EQUAL(data[1][32], xs3_vect_s32_argmax(&data[1][0],32));
    TEST_ASSERT_EQUAL(data[2][32], xs3_vect_s32_argmax(&data[2][0],32));
    TEST_ASSERT_EQUAL(data[3][32], xs3_vect_s32_argmax(&data[3][0],32));
    TEST_ASSERT_EQUAL(data[4][32], xs3_vect_s32_argmax(&data[4][0],32));
    TEST_ASSERT_EQUAL(data[5][32], xs3_vect_s32_argmax(&data[5][0],32));
    TEST_ASSERT_EQUAL(data[6][32], xs3_vect_s32_argmax(&data[6][0],32));
    TEST_ASSERT_EQUAL(data[7][32], xs3_vect_s32_argmax(&data[7][0],32));

}





static void test_xs3_vect_s32_argmax_B()
{
    PRINTF("%s...\n", __func__);
    seed = 0x5634564;

    unsigned result;
    int32_t B[MAX_LEN];

    for(int v = 0; v < REPS; v++){
        const unsigned init_seed = seed;

        unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN+1);

        PRINTF("\trepetition %d.. (seed: 0x%08X; len: %u)\n", v, init_seed, len);

        unsigned exp = 0;

        for(int i = 0; i < len; i++){
            // printf("i = %d     &B = 0x%08X\n", i, (unsigned) B);
            B[i] = pseudo_rand_int32(&seed);
            exp = (B[i] > B[exp])? i : exp;
        }

        result = xs3_vect_s32_argmax(B, len);

        TEST_ASSERT_EQUAL(exp, result);
        
    }
}









static void test_xs3_vect_s16_argmin_A()
{
    PRINTF("%s...\n", __func__);

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


    TEST_ASSERT_EQUAL(data[0][32], xs3_vect_s16_argmin(&data[0][0],32));
    TEST_ASSERT_EQUAL(data[1][32], xs3_vect_s16_argmin(&data[1][0],32));
    TEST_ASSERT_EQUAL(data[2][32], xs3_vect_s16_argmin(&data[2][0],32));
    TEST_ASSERT_EQUAL(data[3][32], xs3_vect_s16_argmin(&data[3][0],32));
    TEST_ASSERT_EQUAL(data[4][32], xs3_vect_s16_argmin(&data[4][0],32));
    TEST_ASSERT_EQUAL(data[5][32], xs3_vect_s16_argmin(&data[5][0],32));
    TEST_ASSERT_EQUAL(data[6][32], xs3_vect_s16_argmin(&data[6][0],32));
    TEST_ASSERT_EQUAL(data[7][32], xs3_vect_s16_argmin(&data[7][0],32));

}




static void test_xs3_vect_s16_argmin_B()
{
    PRINTF("%s...\n", __func__);
    seed = 0x6D734F1E;

    unsigned result;
    int16_t B[MAX_LEN];

    for(int v = 0; v < REPS; v++){

        unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        // len = 32;

        PRINTF("\trepetition %d.. (seed: 0x%08X; len: %u)\n", v, seed, len);

        unsigned exp = 0;

        for(int i = 0; i < len; i++){
            B[i] = pseudo_rand_int16(&seed);
            exp = (B[i] < B[exp])? i : exp;
        }

        result = xs3_vect_s16_argmin(B, len);

        TEST_ASSERT_EQUAL(exp, result);
    }
}








static void test_xs3_vect_s32_argmin_A()
{
    PRINTF("%s...\n", __func__);

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


    TEST_ASSERT_EQUAL(data[0][32], xs3_vect_s32_argmin(&data[0][0],32));
    TEST_ASSERT_EQUAL(data[1][32], xs3_vect_s32_argmin(&data[1][0],32));
    TEST_ASSERT_EQUAL(data[2][32], xs3_vect_s32_argmin(&data[2][0],32));
    TEST_ASSERT_EQUAL(data[3][32], xs3_vect_s32_argmin(&data[3][0],32));
    TEST_ASSERT_EQUAL(data[4][32], xs3_vect_s32_argmin(&data[4][0],32));
    TEST_ASSERT_EQUAL(data[5][32], xs3_vect_s32_argmin(&data[5][0],32));
    TEST_ASSERT_EQUAL(data[6][32], xs3_vect_s32_argmin(&data[6][0],32));
    TEST_ASSERT_EQUAL(data[7][32], xs3_vect_s32_argmin(&data[7][0],32));

}


static void test_xs3_vect_s32_argmin_B()
{
    PRINTF("%s...\n", __func__);
    seed = 346332123;

    unsigned result;
    int32_t B[MAX_LEN];

    for(int v = 0; v < REPS; v++){

        unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN+1);

        PRINTF("\trepetition %d.. (seed: 0x%08X; len: %u)\n", v, seed, len);

        unsigned exp = 0;

        for(int i = 0; i < len; i++){
            B[i] = pseudo_rand_int32(&seed);
            exp = (B[i] < B[exp])? i : exp;
        }

        result = xs3_vect_s32_argmin(B, len);

        TEST_ASSERT_EQUAL(exp, result);
        
    }
}
#undef MAX_LEN
#undef REPS



void test_xs3_max_min()
{
    SET_TEST_FILE();

    RUN_TEST(test_xs3_vect_s16_max_random);
    RUN_TEST(test_xs3_vect_s32_max_random);
    RUN_TEST(test_xs3_vect_s16_min_random);
    RUN_TEST(test_xs3_vect_s32_min_random);
    RUN_TEST(test_xs3_vect_s16_argmax_A);
    RUN_TEST(test_xs3_vect_s16_argmax_B);
    RUN_TEST(test_xs3_vect_s32_argmax_A);
    RUN_TEST(test_xs3_vect_s32_argmax_B);
    RUN_TEST(test_xs3_vect_s16_argmin_A);
    RUN_TEST(test_xs3_vect_s16_argmin_B);
    RUN_TEST(test_xs3_vect_s32_argmin_A);
    RUN_TEST(test_xs3_vect_s32_argmin_B);
}