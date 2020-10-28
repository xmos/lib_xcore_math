
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



#define MAX_LEN     200
#define REPS        IF_QUICK_TEST(10, 400)
void test_xs3_min_s16_random()
{
    PRINTF("%s...\n", __func__);
    seed = 0x6D734F1E;

    int16_t result;
    int16_t B[MAX_LEN];

    for(int v = 0; v < REPS; v++){

        unsigned len = (pseudo_rand_uint32(&seed) % MAX_LEN) + 1;
        // len = 32;

        PRINTF("\trepetition %d.. (seed: 0x%08X; len: %u)\n", v, seed, len);

        int16_t min = 0x7FFF;

        for(int i = 0; i < len; i++){
            B[i] = pseudo_rand_int16(&seed);
            min = MIN(min, B[i]);
        }

        result = xs3_min_s16(B, len);

        TEST_ASSERT_EQUAL(min, result);
    }
}
#undef MAX_LEN
#undef REPS





#define MAX_LEN     200
#define REPS        IF_QUICK_TEST(10, 400)
void test_xs3_min_s32_random()
{
    PRINTF("%s...\n", __func__);
    seed = 346332123;

    int32_t result;
    int32_t B[MAX_LEN];

    for(int v = 0; v < REPS; v++){

        unsigned len = (pseudo_rand_uint32(&seed) % MAX_LEN) + 1;
        // len = 1;

        PRINTF("\trepetition %d.. (seed: 0x%08X; len: %u)\n", v, seed, len);

        int32_t min = 0x7FFFFFFF;

        for(int i = 0; i < len; i++){
            // B[i] = pseudo_rand_int32(&seed);
            // B[i] = i;
            min = MIN(min, B[i]);
        }

        result = xs3_min_s32(B, len);

        TEST_ASSERT_EQUAL(min, result);
        
    }
}
#undef MAX_LEN
#undef REPS



void test_xs3_min()
{
    SET_TEST_FILE();

    RUN_TEST(test_xs3_min_s16_random);
    RUN_TEST(test_xs3_min_s32_random);
}