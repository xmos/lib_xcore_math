// Copyright 2020 XMOS LIMITED. This Software is subject to the terms of the 
// XMOS Public License: Version 1

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xs3_math.h"

#include "../src/vect/vpu_helper.h"

#include "../../tst_common.h"

#include "unity.h"

#if DEBUG_ON || 0
#undef DEBUG_ON
#define DEBUG_ON    (1)
#endif

static unsigned seed = 0x47848653;
static char msg_buff[200];





void xs3_push_sample_down_s16(
    int16_t* buffer,
    const unsigned length,
    const int16_t new_value);
    
void xs3_push_sample_up_s16(
    int16_t* buffer,
    const unsigned length,
    const int16_t new_value);


#define MAX_LEN     100
#define REPS        1000
void test_xs3_push_sample_down_s16_case0()
{
    PRINTF("%s...\n", __func__);
    
    int16_t buff[MAX_LEN];
    int16_t buff_exp[MAX_LEN];

    for(int v = 0; v < REPS; v++){

        unsigned length = (pseudo_rand_uint32(&seed) % MAX_LEN) + 1;
        // length = 29;

        PRINTF("\trep %d.. (%u)\n", v, length);

        for(int i = 0; i < length; i++){
            buff[i] = pseudo_rand_int16(&seed);
            if(i) buff_exp[i-1] = buff[i];
        }

        int16_t new_sample = pseudo_rand_int16(&seed);

        buff_exp[length-1] = new_sample;

        // for(int i = 0; i < length; i++){
        //     printf("[%d\t%d]\n", buff[i], buff_exp[i]);
        // }

        xs3_push_sample_down_s16(buff, length, new_sample);

        for(int i = 0; i < length; i++){
            TEST_ASSERT_EQUAL(buff_exp[i], buff[i]);
        }
    }
}
#undef MAX_LEN
#undef REPS




#define MAX_LEN     1000
void test_xs3_push_sample_up_s16_case0()
{
    PRINTF("%s...\n", __func__);
    
    int16_t buff[MAX_LEN];
    int16_t buff_exp[MAX_LEN];

    for(unsigned length = 1; length <= MAX_LEN; length++){

        PRINTF("\tlength %u..\n", length);

        for(int i = 0; i < length; i++){
            buff[i] = pseudo_rand_int16(&seed);
            if(i < length-1) buff_exp[i+1] = buff[i];
        }

        int16_t new_sample = pseudo_rand_int16(&seed);

        buff_exp[0] = new_sample;

        // for(int i = 0; i < length; i++){
        //     printf("[%d\t%d]\n", buff[i], buff_exp[i]);
        // }

        xs3_push_sample_up_s16(buff,  length, new_sample);

        // for(int i = 0; i < length; i++){
        //     printf("! [%d\t%d]\n", buff[i], buff_exp[i]);
        // }
        // printf("\n\n");

        for(int i = 0; i < length; i++){
            TEST_ASSERT_EQUAL(buff_exp[i], buff[i]);
        }
    }
}
#undef MAX_LEN





void test_xs3_push_sample_s16()
{
    SET_TEST_FILE();

    RUN_TEST(test_xs3_push_sample_down_s16_case0);
    RUN_TEST(test_xs3_push_sample_up_s16_case0);

}