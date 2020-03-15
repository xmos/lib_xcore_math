
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "xs3_math.h"

#include "../tst_common.h"

#include "unity.h"


#if !defined(DEBUG_ON) || 0
#undef DEBUG_ON
#define DEBUG_ON    (1)
#endif


#define MAX_LEN     68
#define REPS        100
static void test_xs3_clip_vect_s16()
{
    PRINTF("%s...\n", __func__);
    unsigned seed = 12312344;
    
    headroom_t hr;
    int16_t A[MAX_LEN];
    int16_t B[MAX_LEN];

    struct {
        struct { unsigned inplace; unsigned notinplace; } placeness;
        struct { unsigned pos; unsigned neg; } shr;
        struct { unsigned above; unsigned below;  unsigned within; } vals;
        struct { unsigned small; unsigned medium; unsigned large;  } len;
        struct {
            struct { unsigned pos; unsigned neg; } lower;
            struct { unsigned pos; unsigned neg; } upper;
        } bound;
    } count;

    memset(&count, 0, sizeof(count));

    for(int v = 0; v < REPS; v++){

        PRINTF("\trepetition %d..\n", v);

        const unsigned len = pseudo_rand_uint32(&seed) % MAX_LEN;

        int16_t upper_bound = pseudo_rand_int16(&seed);
        int16_t lower_bound = pseudo_rand_int16(&seed);

        unsigned min_shr = -xs3_cls_array_s16(B, len);

        int16_t b_shr = min_shr + pseudo_rand_uint32(&seed) % 3;

        if(upper_bound < lower_bound){
            int16_t tmp = upper_bound;
            upper_bound = lower_bound;
            lower_bound = tmp;
        }

        

        for(int i = 0; i < len; i++){
            B[i] = pseudo_rand_int16(&seed);

            if(B[i] >> b_shr > upper_bound)
                count.vals.above++;
            else if(B[i] >> b_shr < lower_bound)
                count.vals.below++;
            else
                count.vals.within++;
        }


        if(lower_bound < 0)     count.bound.lower.neg++;
        else                    count.bound.lower.pos++;
        if(upper_bound < 0)     count.bound.upper.neg++;
        else                    count.bound.upper.pos++;
        if(len < 16)                count.len.small++;
        else if(len>=16&&len<48)    count.len.medium++;
        else                        count.len.large++;
        if(b_shr < 0)   count.shr.pos++;
        else            count.shr.neg++;

        if( pseudo_rand_uint32(&seed) % 2){
            count.placeness.notinplace++;
            //Not in-place

            memset(A, 0xCC, sizeof(A));
            hr = xs3_clip_vect_s16(A, B, len, lower_bound, upper_bound, b_shr);

            for(int i = 0; i < len; i++){
                int16_t bbb = B[i] >> b_shr;
                int16_t exp = bbb <= lower_bound? lower_bound
                            : bbb >= upper_bound? upper_bound
                            : bbb; 

                TEST_ASSERT_EQUAL(exp, A[i]);
            }
            for(int i = len; i < MAX_LEN; i++)
                TEST_ASSERT_EQUAL((int16_t)0xCCCC, A[i]);

        } else {
            count.placeness.inplace++;
            //In-place

            memcpy(A, B, sizeof(A));
            hr = xs3_clip_vect_s16(A, A, len, lower_bound, upper_bound, b_shr);

            for(int i = 0; i < len; i++)
            {
                int16_t bbb = B[i] >> b_shr;
                int16_t exp = bbb <= lower_bound? lower_bound
                            : bbb >= upper_bound? upper_bound
                            : bbb; 
                TEST_ASSERT_EQUAL(exp, A[i]);
            }
        }


    }

    TEST_ASSERT_GREATER_THAN(20, count.placeness.inplace);
    TEST_ASSERT_GREATER_THAN(20, count.placeness.notinplace);
    TEST_ASSERT_GREATER_THAN(20, count.shr.pos);
    TEST_ASSERT_GREATER_THAN(20, count.shr.neg);
    TEST_ASSERT_GREATER_THAN(20, count.len.small);
    TEST_ASSERT_GREATER_THAN(20, count.len.medium);
    TEST_ASSERT_GREATER_THAN(20, count.len.large);
    TEST_ASSERT_GREATER_THAN(20, count.bound.lower.neg);
    TEST_ASSERT_GREATER_THAN(20, count.bound.lower.pos);
    TEST_ASSERT_GREATER_THAN(20, count.bound.upper.neg);
    TEST_ASSERT_GREATER_THAN(20, count.bound.upper.pos);
    TEST_ASSERT_GREATER_THAN(1000, count.vals.above);
    TEST_ASSERT_GREATER_THAN(1000, count.vals.below);
    TEST_ASSERT_GREATER_THAN(1000, count.vals.within);
}







static void test_xs3_clip_vect_s32()
{
    PRINTF("%s...\n", __func__);
    unsigned seed = 12312344;
    
    headroom_t hr;
    int32_t A[MAX_LEN];
    int32_t B[MAX_LEN];

    struct {
        struct { unsigned inplace; unsigned notinplace; } placeness;
        struct { unsigned pos; unsigned neg; } shr;
        struct { unsigned above; unsigned below;  unsigned within; } vals;
        struct { unsigned small; unsigned medium; unsigned large;  } len;
        struct {
            struct { unsigned pos; unsigned neg; } lower;
            struct { unsigned pos; unsigned neg; } upper;
        } bound;
    } count;

    memset(&count, 0, sizeof(count));

    for(int v = 0; v < REPS; v++){

        PRINTF("\trepetition %d..\n", v);

        const unsigned len = pseudo_rand_uint32(&seed) % MAX_LEN;

        int32_t upper_bound = pseudo_rand_int32(&seed);
        int32_t lower_bound = pseudo_rand_int32(&seed);

        unsigned min_shr = -xs3_cls_array_s32(B, len);

        int32_t b_shr = min_shr + pseudo_rand_uint32(&seed) % 3;

        if(upper_bound < lower_bound){
            int32_t tmp = upper_bound;
            upper_bound = lower_bound;
            lower_bound = tmp;
        }

        

        for(int i = 0; i < len; i++){
            B[i] = pseudo_rand_int32(&seed);

            if(B[i] >> b_shr > upper_bound)
                count.vals.above++;
            else if(B[i] >> b_shr < lower_bound)
                count.vals.below++;
            else
                count.vals.within++;
        }


        if(lower_bound < 0)     count.bound.lower.neg++;
        else                    count.bound.lower.pos++;
        if(upper_bound < 0)     count.bound.upper.neg++;
        else                    count.bound.upper.pos++;
        if(len < 8)                count.len.small++;
        else if(len>=8&&len<48)    count.len.medium++;
        else                       count.len.large++;
        if(b_shr < 0)   count.shr.pos++;
        else            count.shr.neg++;

        if( pseudo_rand_uint32(&seed) % 2){
            count.placeness.notinplace++;
            //Not in-place

            memset(A, 0xCC, sizeof(A));
            hr = xs3_clip_vect_s32(A, B, len, lower_bound, upper_bound, b_shr);

            for(int i = 0; i < len; i++){
                int32_t bbb = B[i] >> b_shr;
                int32_t exp = bbb <= lower_bound? lower_bound
                            : bbb >= upper_bound? upper_bound
                            : bbb; 

                TEST_ASSERT_EQUAL(exp, A[i]);
            }
            for(int i = len; i < MAX_LEN; i++)
                TEST_ASSERT_EQUAL((int32_t)0xCCCCCCCC, A[i]);

        } else {
            count.placeness.inplace++;
            //In-place

            memcpy(A, B, sizeof(A));
            hr = xs3_clip_vect_s32(A, A, len, lower_bound, upper_bound, b_shr);

            for(int i = 0; i < len; i++)
            {
                int32_t bbb = B[i] >> b_shr;
                int32_t exp = bbb <= lower_bound? lower_bound
                            : bbb >= upper_bound? upper_bound
                            : bbb; 
                TEST_ASSERT_EQUAL(exp, A[i]);
            }
        }


    }

    TEST_ASSERT_GREATER_THAN(20, count.placeness.inplace);
    TEST_ASSERT_GREATER_THAN(20, count.placeness.notinplace);
    TEST_ASSERT_GREATER_THAN(20, count.shr.pos);
    TEST_ASSERT_GREATER_THAN(20, count.shr.neg);
    TEST_ASSERT_GREATER_THAN(20, count.len.small);
    TEST_ASSERT_GREATER_THAN(20, count.len.medium);
    TEST_ASSERT_GREATER_THAN(20, count.len.large);
    TEST_ASSERT_GREATER_THAN(20, count.bound.lower.neg);
    TEST_ASSERT_GREATER_THAN(20, count.bound.lower.pos);
    TEST_ASSERT_GREATER_THAN(20, count.bound.upper.neg);
    TEST_ASSERT_GREATER_THAN(20, count.bound.upper.pos);
    TEST_ASSERT_GREATER_THAN(1000, count.vals.above);
    TEST_ASSERT_GREATER_THAN(1000, count.vals.below);
    TEST_ASSERT_GREATER_THAN(1000, count.vals.within);
}



int test_xs3_clip_vect()
{
    int retval = 0;
    
    UnityBegin(__FILE__);

    RUN_TEST(test_xs3_clip_vect_s16);

    RUN_TEST(test_xs3_clip_vect_s32);
    
    retval = UnityEnd();
    
    return retval;
}