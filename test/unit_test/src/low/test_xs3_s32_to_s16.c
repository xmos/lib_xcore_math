
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

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


#if !defined(DEBUG_ON) || (0)
#undef DEBUG_ON
#define DEBUG_ON    (1)
#endif

#define MAX_LEN 50
static void test_xs3_s32_to_s16_basic()
{
    PRINTF("%s...\n", __func__);

    typedef struct {
        int32_t input;
        int b_shr;
        int16_t expected;
        unsigned line;
    } test_case_t;

    test_case_t casses[] = {
        //       input     shr     expected       line #
        {   0x00000000,      0,      0x0000,    __LINE__},
        {   0x00000001,      0,      0x0001,    __LINE__},
        {   0x00000010,      0,      0x0010,    __LINE__},
        {   0x00000100,      0,      0x0100,    __LINE__},
        {  -0x00000100,      0,     -0x0100,    __LINE__},
        {   0x00008000,      0,      0x7FFF,    __LINE__},
        {  -0x00008000,      0,     -0x7FFF,    __LINE__},
        {   0x00000001,      1,      0x0001,    __LINE__},
        {  -0x00000001,      1,      0x0000,    __LINE__}, //ties round towards positive infty
        {   0x00018000,     16,      0x0002,    __LINE__},
        {  -0x00018000,     16,     -0x0001,    __LINE__},
    };

    const unsigned N_cases = sizeof(casses)/sizeof(test_case_t);

    const unsigned start_case = 0;

    for(int v = start_case; v < N_cases; v++){
        PRINTF("\ttest vector %d..\n", v);
        
        test_case_t* casse = &casses[v];

        unsigned lengths[] = {1, 4, 16, 32, 40 };
        int32_t WORD_ALIGNED B[MAX_LEN];
        int16_t WORD_ALIGNED A[MAX_LEN];

        for(int i = 0; i < sizeof(lengths)/sizeof(lengths[0]); i++){
            unsigned len = lengths[i];
            PRINTF("\tlength %u..\n", len);

            for(int i = 0; i < MAX_LEN; i++){
                B[i] = casse->input;
                A[i] = 0xCCCC;
            }

            xs3_s32_to_s16(A, B, len, casse->b_shr);

            for(int k = 0; k < len; k++)
                TEST_ASSERT_EQUAL(casse->expected, A[k]);
            for(int k = len; k < MAX_LEN; k++)
                TEST_ASSERT_EQUAL((int16_t)0xCCCC, A[k]);

            xs3_s32_to_s16((int16_t*)B, B, len, casse->b_shr);

            for(int k = 0; k < len; k++)
                TEST_ASSERT_EQUAL(casse->expected, ((int16_t*)B)[k]);
        }
    }
}
#undef MAC_LEN


int test_xs3_s32_to_s16()
{
    int retval = 0;
    
    UnityBegin(__FILE__);

    RUN_TEST(test_xs3_s32_to_s16_basic);

    retval = UnityEnd();
    

    return retval;
}