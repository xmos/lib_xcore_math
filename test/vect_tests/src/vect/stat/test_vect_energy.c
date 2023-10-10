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

TEST_GROUP_RUNNER(vect_energy) {
  RUN_TEST_CASE(vect_energy, vect_s16_energy);
  RUN_TEST_CASE(vect_energy, vect_s32_energy);
}

TEST_GROUP(vect_energy);
TEST_SETUP(vect_energy) { fflush(stdout); }
TEST_TEAR_DOWN(vect_energy) {}



#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (64)
#else
#  define REPS       (1000)
#  define MAX_LEN    (256)
#endif


TEST(vect_energy, vect_s16_energy)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    int32_t result;
    int16_t B[MAX_LEN];

    for(unsigned int v = 0; v < REPS; v++){

        unsigned len = (pseudo_rand_uint32(&seed) % MAX_LEN) + 1;
        setExtraInfo_RSL(v, seed, len);

        // Max possible value is  

        //   (-0x8000 >> b_shr)**2 * len
        // = (-2**15 >> b_shr)**2 * len
        // = (-2**(15-b_shr))**2 * len
        // = 2**(2*(15-b_shr)) * len
        // = 2**(30-2*b_shr) * len

        // For simplicity, let's assume   len == 2**(u32_ceil_log2(len))

        // Maximum allowable result is 2**31, then solving for b_shr with this equality

        //   2**(30-2*b_shr) * len = 2**31
        // = 2**(30-2*b_shr) * 2**(u32_ceil_log2(len))
        // = 2**(30 - 2*b_shr + u32_ceil_log2(len)) = 2**31
        //   30 - 2*b_shr + u32_ceil_log2(len) = 31
        //   -1 + u32_ceil_log2(len) = 2*b_shr
        //   b_shr = (u32_ceil_log2(len)-1)/2

        // But that's the minimum value of b_shr. If (u32_ceil_log2(len)-1) happens to be odd, then dividing will
        //  leave b_shr too small by half. So add 1 before dividing.
        // b_shr = u32_ceil_log2(len)/2 will always give a valid result.

        right_shift_t b_shr = u32_ceil_log2(len)/2;

        int32_t exp = 0;
        
        for(unsigned int i = 0; i < len; i++){
            B[i] = pseudo_rand_int16(&seed);
            int32_t t = B[i];
            t = (b_shr>=0)? (t>>b_shr) : (t<<-b_shr);
            exp += t*t;
        }

        result = vect_s16_energy(B, len, b_shr);

        TEST_ASSERT_EQUAL(exp, result);
        
    }
}
#undef MAX_LEN
#undef REPS


#define MAX_LEN     200
#define REPS        (100)

TEST(vect_energy, vect_s32_energy)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    int64_t result;
    int32_t B[MAX_LEN];

    for(unsigned int v = 0; v < REPS; v++){

        unsigned len = (pseudo_rand_uint32(&seed) % MAX_LEN) + 1;
        // len = 1;

        setExtraInfo_RSL(v, seed, len);

        // Max possible value is  

        //   ((-0x80000000 >> b_shr)**2)>>30 * len
        // = ((-2**31 >> b_shr)**2)>>30 * len
        // = ((-2**(31-b_shr))**2)>>30 * len
        // = (2**(2*(31-b_shr))) * 2**(-30) * len
        // = 2**(62-2*b_shr) * 2**(-30) * len
        // = 2**(32-2*b_shr) * len

        // For simplicity, let's assume   len == 2**(u32_ceil_log2(len))
        // Maximum allowable result is 2**39, then solving for b_shr with this equality

        //   2**(32-2*b_shr) * len = 2**39
        // = 2**(32-2*b_shr) * 2**(u32_ceil_log2(len))
        // = 2**(32- 2*b_shr + u32_ceil_log2(len)) = 2**39
        //   32 - 2*b_shr + u32_ceil_log2(len) = 39
        //   -7 + u32_ceil_log2(len) = 2*b_shr
        //   b_shr = (u32_ceil_log2(len)-7)/2

        // But that's the minimum value of b_shr. If (u32_ceil_log2(len)-7) happens to be odd, then dividing will
        //  leave b_shr too small by half. So add 1 before dividing.
        // b_shr = (u32_ceil_log2(len)-6)/2 should always work, as long as it's not negative
        // (because I've assumed zero headroom)

        right_shift_t b_shr = (u32_ceil_log2(len)-6)/2;
        b_shr = MAX(b_shr, 0);

        int64_t exp = 0;

        for(unsigned int i = 0; i < len; i++){
            B[i] = pseudo_rand_int32(&seed);
            int64_t t = B[i];
            t = (b_shr>=0)? (t>>b_shr) : (t<<-b_shr);
            t = ((t*t)+(1<<29))>>30;
            exp += t;
        }

        result = vect_s32_energy(B, len, b_shr);

        TEST_ASSERT_EQUAL(exp, result);
        
    }
}
#undef MAX_LEN
#undef REPS

