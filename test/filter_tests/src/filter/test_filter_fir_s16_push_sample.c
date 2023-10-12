// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xmath/xmath.h"

#include "../src/vect/vpu_helper.h"

#include "../tst_common.h"

#include "unity_fixture.h"

TEST_GROUP_RUNNER(filter_fir_s16_push_sample) {
  RUN_TEST_CASE(filter_fir_s16_push_sample, filter_fir_s16_push_sample_down);
  RUN_TEST_CASE(filter_fir_s16_push_sample, filter_fir_s16_push_sample_up);
}

TEST_GROUP(filter_fir_s16_push_sample);
TEST_SETUP(filter_fir_s16_push_sample) { fflush(stdout); }
TEST_TEAR_DOWN(filter_fir_s16_push_sample) {}

#if !SMOKE_TEST
static char msg_buff[200];
#endif

void filter_fir_s16_push_sample_down(
    int16_t* buffer,
    const unsigned length,
    const int16_t new_value);
    
void filter_fir_s16_push_sample_up(
    int16_t* buffer,
    const unsigned length,
    const int16_t new_value);


#define MAX_LEN     100

#if SMOKE_TEST
#  define REPS       (100)
#else
#  define REPS       (1000)
#endif
TEST(filter_fir_s16_push_sample, filter_fir_s16_push_sample_down)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    int16_t buff[MAX_LEN];
    int16_t buff_exp[MAX_LEN];

    for(unsigned int v = 0; v < REPS; v++){
#if !SMOKE_TEST
        unsigned old_seed = seed;
#endif
        unsigned length = (pseudo_rand_uint32(&seed) % MAX_LEN) + 1;

#if !SMOKE_TEST
        sprintf(msg_buff, "( rep: %d; seed: 0x%08X; length: %u )", v, old_seed, length);
        UNITY_SET_DETAIL(msg_buff);
#endif

        for(unsigned int i = 0; i < length; i++){
            buff[i] = pseudo_rand_int16(&seed);
            if(i) buff_exp[i-1] = buff[i];
        }

        int16_t new_sample = pseudo_rand_int16(&seed);

        buff_exp[length-1] = new_sample;

        filter_fir_s16_push_sample_down(buff, length, new_sample);

        for(unsigned int i = 0; i < length; i++){
            TEST_ASSERT_EQUAL(buff_exp[i], buff[i]);
        }
    }
}
#undef MAX_LEN
#undef REPS



#if SMOKE_TEST
#  define MAX_LEN       (100)
#else
#  define MAX_LEN       (1000)
#endif
TEST(filter_fir_s16_push_sample, filter_fir_s16_push_sample_up)
{
    unsigned seed = SEED_FROM_FUNC_NAME();
    
    int16_t buff[MAX_LEN];
    int16_t buff_exp[MAX_LEN];

    for(unsigned length = 1; length <= MAX_LEN; length++){
      
#if !SMOKE_TEST
        sprintf(msg_buff, "( Length: %u )", length);
        UNITY_SET_DETAIL(msg_buff);
#endif

        for(unsigned int i = 0; i < length; i++){
            buff[i] = pseudo_rand_int16(&seed);
            if(i < length-1) buff_exp[i+1] = buff[i];
        }

        int16_t new_sample = pseudo_rand_int16(&seed);
        buff_exp[0] = new_sample;

        filter_fir_s16_push_sample_up(buff,  length, new_sample);

        for(unsigned int i = 0; i < length; i++){
            TEST_ASSERT_EQUAL(buff_exp[i], buff[i]);
        }
    }
}
#undef MAX_LEN
