
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xs3_math.h"

#include "../src/low/vpu_helper.h"

#include "../../tst_common.h"

#include "unity.h"

#if DEBUG_ON || 0
#undef DEBUG_ON
#define DEBUG_ON    (1)
#endif

static unsigned seed = 0xC502CE07;
static char msg_buff[200];




#define MAX_TAPS    100
void test_xs3_filter_fir_s16_case0()
{
    PRINTF("%s...\n", __func__);
    
    int16_t coefs[MAX_TAPS];
    int16_t state[MAX_TAPS];

    xs3_fir_filter_s16_t filter;

    for(int i = 0; i < MAX_TAPS; i++)
        coefs[i] = 0x1;

    for(int N = 1; N < MAX_TAPS; N++){
        PRINTF("\ttap count: %d\n", N);

        memset(state, 0, sizeof(state));

        xs3_filter_fir_s16_init(&filter, state, N, coefs, 0);

        int16_t exp = 0;
        for(int i = 0; i < N; i++){
            xs3_filter_fir_s16_add_sample(&filter, i);
            exp += i;
        }

        for(int i = 0; i < 20; i++){
            // PRINTF("\t\tsamp.. %d\n", i);

            exp += N;  // old sample (i) leaves as new sample (N+i) comes in.

            int16_t res = xs3_filter_fir_s16(&filter, N+i);
            TEST_ASSERT_EQUAL(exp, res);
        }

    }
}
#undef MAX_TAPS


#define MAX_TAPS    100
void test_xs3_filter_fir_s16_case1()
{
    PRINTF("%s...\n", __func__);
    
    int16_t coefs[MAX_TAPS];
    int16_t state[MAX_TAPS];

    xs3_fir_filter_s16_t filter;

    for(int i = 0; i < MAX_TAPS; i++)
        coefs[i] = i;

    for(int N = 1; N < MAX_TAPS; N++){
        PRINTF("\ttap count: %d\n", N);

        memset(state, 0, sizeof(state));

        xs3_filter_fir_s16_init(&filter, state, N, coefs, 0);

        int16_t exp = 0;
        for(int i = 0; i < N; i++){
            xs3_filter_fir_s16_add_sample(&filter, 1);
        }

        for(int i = 0; i < 20; i++){
            
            int16_t exp = N*(N-1) / 2;

            int16_t res = xs3_filter_fir_s16(&filter, 1);
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
#define REPS        500
void test_xs3_filter_fir_s16_case2()
{
    PRINTF("%s...\n", __func__);
    
    int16_t coefs[MAX_TAPS];
    int16_t state[MAX_TAPS];

    xs3_fir_filter_s16_t filter;

    for(int v = 0; v < REPS; v++){

        const unsigned old_seed = seed;

        // Number of filter taps
        unsigned N = (pseudo_rand_uint32(&seed) % MAX_TAPS) + 1;
        N = (v % MAX_TAPS) + 1;
        // N = 75;

        unsigned log2_N = ceil_log2(N);

        PRINTF("\trep %d... (%u taps)\t(seed: 0x%08X)\n", v, N, old_seed);

        xs3_filter_fir_s16_init(&filter, state, N, coefs, 0);

        int32_t expected32 = 0;

        for(int i = 0; i < N; i++){
            coefs[i] = pseudo_rand_int16(&seed) >> log2_N;
            state[i] = pseudo_rand_int16(&seed) >> log2_N;
        }

        for(int i = 1; i < N; i++){
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
        int16_t res = xs3_filter_fir_s16(&filter, new_sample);
        
        if(expected16 != res){
            sprintf(msg_buff, "(rep %d;   %u taps;   seed 0x%08X)", v, N, old_seed);

            PRINTF("======================\n");
            PRINTF("N = %u\n", N);
            PRINTF("expected32 = %ld\t(0x%08lX)\n", expected32, (uint32_t)expected32);
            PRINTF("filter.shift = %d\n", filter.shift);
            PRINTF("expected16 = %d\t(0x%04X)\n", expected16, (unsigned)expected16);

            for(int i = 0; i < N; i++){
                PRINTF("\tstate[%d]: % 6d\tcoefs[%d]: % 6d\n", i, state[i], i, coefs[i]);
            }

            PRINTF("======================\n");
        }

        TEST_ASSERT_EQUAL_MESSAGE(expected16, res, msg_buff);
    }

}
#undef MAX_TAPS
#undef REPS




void test_xs3_filter_fir_s16()
{
    SET_TEST_FILE();

    RUN_TEST(test_xs3_filter_fir_s16_case0);
    RUN_TEST(test_xs3_filter_fir_s16_case1);
    RUN_TEST(test_xs3_filter_fir_s16_case2);

}