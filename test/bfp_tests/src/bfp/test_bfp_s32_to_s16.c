
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "bfp_math.h"

#include "../tst_common.h"

#include "testing.h"

#include "unity.h"

#if DEBUG_ON || 0
#undef DEBUG_ON
#define DEBUG_ON    (1)
#endif


#define REPS        (1000)
#define MAX_LEN     256  //Smaller lengths mean larger variance w.r.t. individual element headroom


static unsigned seed = 666;





static void test_bfp_s32_to_s16_()
{
    PRINTF("%s...\t(random vectors)\n", __func__);

    seed = 54516;

    int16_t WORD_ALIGNED dataA[MAX_LEN];
    int32_t WORD_ALIGNED dataB[MAX_LEN];
    bfp_s16_t A;
    bfp_s32_t B;

    A.data = dataA;
    B.data = dataB;

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        bfp_s32_init(&B, dataB, 
            pseudo_rand_int(&seed, -50, 50),
            pseudo_rand_uint(&seed, 1, MAX_LEN+1), 0);
        
        bfp_s16_init(&A, dataA, 0, B.length, 0);

        B.hr = pseudo_rand_uint(&seed, 0, 28);

        for(int i = 0; i < B.length; i++){
            B.data[i] = pseudo_rand_int32(&seed) >> B.hr;
        }

        bfp_s32_headroom(&B);

        bfp_s32_to_s16(&A, &B);

        TEST_ASSERT_EQUAL(xs3_vect_s16_headroom(A.data, A.length), A.hr);

        conv_error_e err = 0;
        double Bf[MAX_LEN];

        conv_vect_s32_to_double(Bf, B.data, B.length, B.exp, &err);

        TEST_ASSERT_EQUAL_MESSAGE(0, err, "Conversion to doubles failed.");

        int16_t expected[MAX_LEN];
        
        test_s16_from_double(expected, Bf, B.length, A.exp);

        for(int i = 0; i < A.length; i++){
            TEST_ASSERT_INT16_WITHIN(1, expected[i], A.data[i]);
        }
    }
}




void test_bfp_s32_to_s16()
{
    SET_TEST_FILE();

    RUN_TEST(test_bfp_s32_to_s16_);
}