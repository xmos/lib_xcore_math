// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xs3_math.h"

#include "../src/vect/vpu_helper.h"

#include "../tst_common.h"

#include "unity_fixture.h"

TEST_GROUP_RUNNER(xs3_push_sample) {
  RUN_TEST_CASE(xs3_push_sample, xs3_push_sample_down_s16);
  RUN_TEST_CASE(xs3_push_sample, xs3_push_sample_up_s16);
}

TEST_GROUP(xs3_push_sample);
TEST_SETUP(xs3_push_sample) {}
TEST_TEAR_DOWN(xs3_push_sample) {}


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
TEST(xs3_push_sample, xs3_push_sample_down_s16)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    int16_t buff[MAX_LEN];
    int16_t buff_exp[MAX_LEN];

    for(int v = 0; v < REPS; v++){
        unsigned old_seed = seed;
        unsigned length = (pseudo_rand_uint32(&seed) % MAX_LEN) + 1;

        sprintf(msg_buff, "( rep: %d; seed: 0x%08X; length: %u )", v, old_seed, length);
        UNITY_SET_DETAIL(msg_buff);

        for(int i = 0; i < length; i++){
            buff[i] = pseudo_rand_int16(&seed);
            if(i) buff_exp[i-1] = buff[i];
        }

        int16_t new_sample = pseudo_rand_int16(&seed);

        buff_exp[length-1] = new_sample;

        xs3_push_sample_down_s16(buff, length, new_sample);

        for(int i = 0; i < length; i++){
            TEST_ASSERT_EQUAL(buff_exp[i], buff[i]);
        }
    }
}
#undef MAX_LEN
#undef REPS


#define MAX_LEN     1000
TEST(xs3_push_sample, xs3_push_sample_up_s16)
{
    unsigned seed = SEED_FROM_FUNC_NAME();
    
    int16_t buff[MAX_LEN];
    int16_t buff_exp[MAX_LEN];

    for(unsigned length = 1; length <= MAX_LEN; length++){
        sprintf(msg_buff, "( Length: %u )", length);
        UNITY_SET_DETAIL(msg_buff);

        for(int i = 0; i < length; i++){
            buff[i] = pseudo_rand_int16(&seed);
            if(i < length-1) buff_exp[i+1] = buff[i];
        }

        int16_t new_sample = pseudo_rand_int16(&seed);
        buff_exp[0] = new_sample;

        xs3_push_sample_up_s16(buff,  length, new_sample);

        for(int i = 0; i < length; i++){
            TEST_ASSERT_EQUAL(buff_exp[i], buff[i]);
        }
    }
}
#undef MAX_LEN
