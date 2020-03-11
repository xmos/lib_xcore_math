
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xs3_math.h"

#include "../tst_common.h"

#include "unity.h"


static unsigned seed = 2314567;
static char msg_buff[200];

#define TEST_ASSERT_EQUAL_MSG(EXPECTED, ACTUAL, LINE_NUM)   do{       \
    if((EXPECTED)!=(ACTUAL)) {                                        \
      sprintf(msg_buff, "(test vector @ line %u)", (LINE_NUM));       \
      TEST_ASSERT_EQUAL_MESSAGE((EXPECTED), (ACTUAL), msg_buff);      \
    }} while(0)


#ifndef DEBUG_ON
#define DEBUG_ON    (1)
#endif


#define MAX_LEN 50
static void test_xs3_set_array_s16()
{
    PRINTF("%s...\n", __func__);

    unsigned lengths[] = {1, 4, 16, 32, 40 };
    int16_t WORD_ALIGNED buff[MAX_LEN];

    for(int i = 0; i < sizeof(lengths)/sizeof(lengths[0]); i++){

        unsigned len = lengths[i];

        for(int k = 0; k < MAX_LEN; k++)
            buff[k] = 0xbeef;

        xs3_set_array_s16(buff, 0x7E57, len);

        for(int k = 0; k < len; k++)
            TEST_ASSERT_EQUAL((int16_t) 0x7E57, buff[k]);
        for(int k = len; k < MAX_LEN; k++)
            TEST_ASSERT_EQUAL((int16_t) 0xbeef, buff[k]);
    }
}
#undef MAX_LEN


#define MAX_LEN 50
static void test_xs3_set_array_s32()
{
    PRINTF("%s...\n", __func__);

    unsigned lengths[] = {1, 4, 16, 32, 40 };
    int32_t buff[MAX_LEN];

    for(int i = 0; i < sizeof(lengths)/sizeof(lengths[0]); i++){

        unsigned len = lengths[i];

        for(int k = 0; k < MAX_LEN; k++)
            buff[k] = 0xbeef1355;

        xs3_set_array_s32(buff, 0x7E571d1d, len);

        for(int k = 0; k < len; k++)
            TEST_ASSERT_EQUAL((int32_t) 0x7E571d1d, buff[k]);
        for(int k = len; k < MAX_LEN; k++)
            TEST_ASSERT_EQUAL((int32_t) 0xbeef1355, buff[k]);
    }
}
#undef MAX_LEN




#define MAX_LEN 50
static void test_xs3_set_array_complex_s32()
{
    PRINTF("%s...\n", __func__);

    unsigned lengths[] = {1, 4, 16, 32, 39 };
    complex_s32_t buff[MAX_LEN];

    for(int i = 0; i < sizeof(lengths)/sizeof(lengths[0]); i++){

        unsigned len = lengths[i];

        for(int k = 0; k < MAX_LEN; k++){
            buff[k].re = 0x12312312;
            buff[k].im = 0x34534534;
        }

        xs3_set_array_complex_s32(buff, 0xABCABCAB, 0xDEFDEFDE, len);

        for(int k = 0; k < len; k++){
            TEST_ASSERT_EQUAL((int32_t) 0xABCABCAB, buff[k].re);
            TEST_ASSERT_EQUAL((int32_t) 0xDEFDEFDE, buff[k].im);
        }
        for(int k = len; k < MAX_LEN; k++){
            TEST_ASSERT_EQUAL((int32_t) 0x12312312, buff[k].re);
            TEST_ASSERT_EQUAL((int32_t) 0x34534534, buff[k].im);
        }
    }
}
#undef MAX_LEN



int test_xs3_set_array()
{
    int retval = 0;
    
    UnityBegin(__FILE__);

    RUN_TEST(test_xs3_set_array_s16);
    RUN_TEST(test_xs3_set_array_s32);
    RUN_TEST(test_xs3_set_array_complex_s32);

    retval = UnityEnd();
    

    return retval;
}