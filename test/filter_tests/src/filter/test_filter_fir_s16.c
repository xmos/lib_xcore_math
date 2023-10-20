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

TEST_GROUP_RUNNER(filter_fir_s16) {
  RUN_TEST_CASE(filter_fir_s16, case0);
  RUN_TEST_CASE(filter_fir_s16, case1);
  RUN_TEST_CASE(filter_fir_s16, case2);
}

TEST_GROUP(filter_fir_s16);
TEST_SETUP(filter_fir_s16) { fflush(stdout); }
TEST_TEAR_DOWN(filter_fir_s16) {}


static char msg_buff[200];

#define MAX_TAPS    100
TEST(filter_fir_s16, case0)
{
    int16_t coefs[MAX_TAPS];
    int16_t state[MAX_TAPS];

    filter_fir_s16_t filter;

    for(int i = 0; i < MAX_TAPS; i++)
        coefs[i] = 0x1;

    for(int N = 1; N < MAX_TAPS; N++){
        sprintf(msg_buff, "( Tap Count: %d )", N);
        UNITY_SET_DETAIL(msg_buff);

        memset(state, 0, sizeof(state));

        filter_fir_s16_init(&filter, state, N, coefs, 0);

        int16_t exp = 0;
        for(int i = 0; i < N; i++){
            filter_fir_s16_add_sample(&filter, i);
            exp += i;
        }

        for(int i = 0; i < 20; i++){
            exp += N;  // old sample (i) leaves as new sample (N+i) comes in.
            int16_t res = filter_fir_s16(&filter, N+i);
            TEST_ASSERT_EQUAL(exp, res);
        }

    }
}
#undef MAX_TAPS


#define MAX_TAPS    100
TEST(filter_fir_s16, case1)
{
    int16_t coefs[MAX_TAPS];
    int16_t state[MAX_TAPS];

    filter_fir_s16_t filter;

    for(int i = 0; i < MAX_TAPS; i++)
        coefs[i] = i;

    for(int N = 1; N < MAX_TAPS; N++){
        sprintf(msg_buff, "( Tap Count: %d )", N);
        UNITY_SET_DETAIL(msg_buff);

        memset(state, 0, sizeof(state));

        filter_fir_s16_init(&filter, state, N, coefs, 0);

        for(int i = 0; i < N; i++){
            filter_fir_s16_add_sample(&filter, 1);
        }

        for(int i = 0; i < 20; i++){
            
            int16_t exp = N*(N-1) / 2;

            int16_t res = filter_fir_s16(&filter, 1);
            TEST_ASSERT_EQUAL(exp, res);
        }

    }
}
#undef MAX_TAPS


/*
    Random Taps/data.

    NOTE: This isn't normally the tap limit on actual filters, because usually filters won't have coefficients that
          are nearly maximum magnitude. Samples probably wouldn't be either.
*/
#define MAX_TAPS    128

#if SMOKE_TEST
#  define REPS       (100)
#else
#  define REPS       (1000)
#endif

TEST(filter_fir_s16, case2)
{
    unsigned seed = SEED_FROM_FUNC_NAME();
    
    int16_t coefs[MAX_TAPS];
    int16_t state[MAX_TAPS];

    filter_fir_s16_t filter;

    for(unsigned int v = 0; v < REPS; v++){

        const unsigned old_seed = seed;

        // Number of filter taps
        unsigned N = (pseudo_rand_uint32(&seed) % MAX_TAPS) + 1;
        N = (v % MAX_TAPS) + 1;

        unsigned log2_N = u32_ceil_log2(N);

        sprintf(msg_buff, "( Rep: %d; Tap Count: %u; Seed: 0x%08X )", v, N, old_seed);
        UNITY_SET_DETAIL(msg_buff);

        filter_fir_s16_init(&filter, state, N, coefs, 0);

        int32_t expected32 = 0;

        for(unsigned int i = 0; i < N; i++){
            coefs[i] = pseudo_rand_int16(&seed) >> log2_N;
            state[i] = pseudo_rand_int16(&seed) >> log2_N;
        }

        for(unsigned int i = 1; i < N; i++){
            expected32 += coefs[i] * state[i-1];
        }

        int16_t new_sample = pseudo_rand_int16(&seed) >> log2_N;

        expected32 += coefs[0] * new_sample;
        
        headroom_t hr = HR_S32(expected32);
        filter.shift = 17 - hr;

        filter.shift = MAX(filter.shift, 0);

        int16_t expected16;

        if(filter.shift >= 0){
            expected16 = (int16_t) ((expected32 + (1LL << (filter.shift-1))) >> filter.shift);
        } else {
            expected16 = (int16_t) (expected32 << -filter.shift);
        }

        // Apply the filter
        int16_t res = filter_fir_s16(&filter, new_sample);

        TEST_ASSERT_EQUAL_MESSAGE(expected16, res, msg_buff);
    }

}
#undef MAX_TAPS
#undef REPS
