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


#define MUL32(X, Y)     (((((int64_t)(X)) * (Y)) + (1<<29)) >> 30)





#define MAX_TAPS    100
void test_xs3_filter_fir_s32_case0()
{
    PRINTF("%s...\n", __func__);
    
    int32_t coefs[MAX_TAPS];
    int32_t state[MAX_TAPS];

    xs3_filter_fir_s32_t filter;

    for(int i = 0; i < MAX_TAPS; i++)
        coefs[i] = 0x40000000; // All coefficients have weight 1.0

    for(int N = 1; N < MAX_TAPS; N++){
        PRINTF("\ttap count: %d\n", N);

        memset(state, 0, sizeof(state));

        xs3_filter_fir_s32_init(&filter, state, N, coefs, 0);

        int32_t exp = 0;
        for(int i = 0; i < N; i++){
            xs3_filter_fir_s32_add_sample(&filter, i);
            exp += i;
        }

        for(int i = 0; i < 20; i++){
            // PRINTF("\t\tsamp.. %d\n", i);

            exp += N;  // old sample (i) leaves as new sample (N+i) comes in.

            int32_t res = xs3_filter_fir_s32(&filter, N+i);
            TEST_ASSERT_EQUAL(exp, res);
        }

    }
}
#undef MAX_TAPS


#define MAX_TAPS    100
void test_xs3_filter_fir_s32_case1()
{
    PRINTF("%s...\n", __func__);
    
    int32_t coefs[MAX_TAPS];
    int32_t state[MAX_TAPS];

    xs3_filter_fir_s32_t filter;

    for(int i = 0; i < MAX_TAPS; i++)
        coefs[i] = i;

    for(int N = 1; N < MAX_TAPS; N++){
        PRINTF("\ttap count: %d\n", N);

        memset(state, 0, sizeof(state));

        xs3_filter_fir_s32_init(&filter, state, N, coefs, 0);

        int32_t exp = 0;
        for(int i = 0; i < N; i++){
            xs3_filter_fir_s32_add_sample(&filter, 0x40000000);
        }

        for(int i = 0; i < 20; i++){
            
            int32_t exp = N*(N-1) / 2;

            int32_t res = xs3_filter_fir_s32(&filter, 0x40000000);
            TEST_ASSERT_EQUAL(exp, res);
        }

    }
}
#undef MAX_TAPS


// Tests whether the logic for summing up the accumulators works correctly.
#define TAPS    32
void test_xs3_filter_fir_s32_case2()
{
    PRINTF("%s...\n", __func__);
    
    int32_t coefs[TAPS];
    int32_t state[TAPS];

    xs3_filter_fir_s32_t filter;

    xs3_filter_fir_s32_init(&filter, state, TAPS, coefs, 0);

    for(int i = 0; i < TAPS; i++){
        coefs[i] = 0x40000000;
        state[i] = 0x40000000;
    }
    
    //All coefs and samples are 0x40000000. So each of them should add 0x40000000 to the sum
    // which should be split evenly across 8 accumulators.
    // res = 32 * 2**30 = 2**35
    // So this should saturate if our shift is zero

    // But if we shift by 6 bits, we should have 2**29 = 0x20000000
    filter.shift = 6;

    for(int i = 0; i < 2*TAPS; i++){
        int32_t res = xs3_filter_fir_s32(&filter, 0x40000000);
        int64_t exp = ((int64_t)TAPS) * 0x40000000;
        exp >>= 6;

        TEST_ASSERT_EQUAL(exp, res);
    }

}
#undef TAPS


/*
    Random Taps/data.

    The maximum product of  (((int64_t)((int32_t)A))*((int32_t)B) >> 30) is -0x80000000 * -0x80000000 * N_taps >> 30
        = -(2**31) * -(2**31) * N_taps >> 30 = 2**32 * N_taps.
    We want to avoid saturating any single accumulator, which can hold    [-(2**39), (2**39)-1]. Let's just pretend 
    that's [- 2**39, 2**39] since getting literally all 0x80000000 from our random number generator is... unlikely. So   
        2**39 = 2**32 * N_taps -->  2**(39-32) = N_taps = 2**7 = 128
    So maximum tap count should be 128.

    NOTE: This isn't normally the tap limit on actual filters, because usually filters won't have coefficients that
          are nearly maximum magnitude. Samples probably wouldn't be either.
*/
#define MAX_TAPS    128
#define REPS        500
void test_xs3_filter_fir_s32_case3()
{
    PRINTF("%s...\n", __func__);
    
    int32_t coefs[MAX_TAPS];
    int32_t state[MAX_TAPS];

    seed = 45763472;

    xs3_filter_fir_s32_t filter;

    for(int v = 0; v < REPS; v++){

        const unsigned old_seed = seed;

        // Number of filter taps
        unsigned N = (pseudo_rand_uint32(&seed) % MAX_TAPS) + 1;

        PRINTF("\trep %d... (%u taps)\t(seed: 0x%08X)\n", v, N, old_seed);

        xs3_filter_fir_s32_init(&filter, state, N, coefs, 0);

        // This makes it so that coefs[i] corresponds to state[i]
        // state[0] will be the next sample
        filter.head = 0;

        int64_t expected64 = 0;

        for(int i = 0; i < N; i++){
            coefs[i] = pseudo_rand_int32(&seed);
            state[i] = pseudo_rand_int32(&seed);

            expected64 += MUL32(coefs[i], state[i]);
        }
           
        filter.shift = 34 - HR_S64(expected64); // Shift up or down to about 30 bits

        int32_t expected32;

        if(filter.shift >= 0){
            expected32 = (int32_t) ((expected64 + (1LL << (filter.shift-1))) >> filter.shift);
        } else {
            expected32 = (int32_t) (expected64 << -filter.shift);
        }   

        // Apply the filter
        int32_t res = xs3_filter_fir_s32(&filter, state[0]);
        
        if(expected32 != res){
            sprintf(msg_buff, "(rep %d;   %u taps;   seed 0x%08X)", v, N, old_seed);

            PRINTF("======================\n");
            PRINTF("N = %u\n", N);
            PRINTF("expected64 = %lld\t(0x%016llX)\n", expected64, (uint64_t)expected64);
            PRINTF("filter.shift = %d\n", filter.shift);
            PRINTF("expected32 = %ld\t(0x%08X)\n", expected32, (unsigned)expected32);

            // for(int i = 0; i < N; i++){
            //     PRINTF("\tstate[%d]: % 12ld\tcoefs[%d]: % 12ld\n", i, state[i], i, coefs[i]);
            // }
            PRINTF("======================\n");
        }

        TEST_ASSERT_EQUAL_MESSAGE(expected32, res, msg_buff);
    }

}
#undef MAX_TAPS
#undef REPS




void test_xs3_filter_fir_s32()
{
    SET_TEST_FILE();

    RUN_TEST(test_xs3_filter_fir_s32_case0);
    RUN_TEST(test_xs3_filter_fir_s32_case1);
    RUN_TEST(test_xs3_filter_fir_s32_case2);
    RUN_TEST(test_xs3_filter_fir_s32_case3);

}