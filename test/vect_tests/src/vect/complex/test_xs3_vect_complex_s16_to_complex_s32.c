
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "xs3_math.h"

#include "../../tst_common.h"

#include "unity.h"

static unsigned seed = 2314567;
static char msg_buff[200];

#define TEST_ASSERT_EQUAL_MSG(EXPECTED, ACTUAL, LINE_NUM)   do{       \
    if((EXPECTED)!=(ACTUAL)) {                                        \
      sprintf(msg_buff, "(test vector @ line %u)", (LINE_NUM));       \
      TEST_ASSERT_EQUAL_MESSAGE((EXPECTED), (ACTUAL), msg_buff);      \
    }} while(0)


#if !defined(DEBUG_ON) || 0
#undef DEBUG_ON
#define DEBUG_ON    (1)
#endif

#define MAX_LEN 50
static void test_xs3_vect_complex_s16_to_complex_s32_basic()
{
    PRINTF("%s...\n", __func__);

    typedef struct {
        complex_s16_t b;
        unsigned line;
    } test_case_t;

    test_case_t casses[] = {
        // b{      re,      im },       line #
        {   {  0x0000,  0x0000 },     __LINE__},
        {   {  0x0001,  0x0000 },     __LINE__},
        {   {  0x0100,  0x0000 },     __LINE__},
        {   {  0x0080,  0x0000 },     __LINE__},
        {   {  0x8000,  0x0000 },     __LINE__},
        {   {  0x7FFF,  0x0000 },     __LINE__},
        {   {  0x8000,  0x0000 },     __LINE__},
        {   {  0x0080,  0x0000 },     __LINE__},
        {   {  0x8080,  0x0000 },     __LINE__},
        {   {  0x00FF,  0x0000 },     __LINE__},
        {   {  0xFF00,  0x0000 },     __LINE__},
    };

    const unsigned N_cases = sizeof(casses)/sizeof(test_case_t);

    const unsigned start_case = 0;

    for(int v = start_case; v < N_cases; v++){
        PRINTF("\ttest vector %d..\n", v);
        
        test_case_t* casse = &casses[v];

        unsigned lengths[] = {1, 4, 16, 32, 40 };
        complex_s32_t DWORD_ALIGNED A[MAX_LEN];
        struct {
            int16_t real[MAX_LEN];
            int16_t imag[MAX_LEN];
        } B;

        for(int i = 0; i < sizeof(lengths)/sizeof(lengths[0]); i++){
            unsigned len = lengths[i];
            PRINTF("\tlength %u..\n", len);

            for(int i = 0; i < MAX_LEN; i++){
                B.real[i] = (i < len)? casse->b.re : 0xBBBB;
                B.imag[i] = (i < len)? casse->b.im : 0xBBBB;
                A[i].re = 0xCCCCCCCC;
                A[i].im = 0xCCCCCCCC;
            }

            xs3_vect_complex_s16_to_complex_s32(A, B.real, B.imag, len);

            for(int k = 0; k < MAX_LEN; k++){
                complex_s32_t exp = {
                    (k < len)? ((int32_t)casse->b.re) : 0xCCCCCCCC,
                    (k < len)? ((int32_t)casse->b.im) : 0xCCCCCCCC  };
                TEST_ASSERT_EQUAL_MSG(exp.re, A[k].re, casse->line);
                TEST_ASSERT_EQUAL_MSG(exp.im, A[k].im, casse->line);
            }

        }
    }
}
#undef MAX_LEN


#define MAX_LEN     68
#define REPS        (100)
static void test_xs3_vect_complex_s16_to_complex_s32_random()
{
    PRINTF("%s...\n", __func__);
    unsigned seed = 778;
    
    complex_s32_t DWORD_ALIGNED A[MAX_LEN];
    struct {
        int16_t real[MAX_LEN];
        int16_t imag[MAX_LEN];
     } B;

    for(int v = 0; v < REPS; v++){

        PRINTF("\trepetition %d..\n", v);

        const unsigned len = pseudo_rand_uint32(&seed) % MAX_LEN + 1;

        for(int i = 0; i < len; i++){
            unsigned shr = pseudo_rand_uint32(&seed) % 10;
            B.real[i] = pseudo_rand_int16(&seed) >> shr;
            B.imag[i] = pseudo_rand_int16(&seed) >> shr;
        }

        memset(A, 0xCC, sizeof(A));
        xs3_vect_complex_s16_to_complex_s32(A, B.real, B.imag, len);

        for(int k = 0; k < MAX_LEN; k++){
                complex_s32_t exp = {
                    (k < len)? ((int32_t)B.real[k]) : 0xCCCCCCCC,
                    (k < len)? ((int32_t)B.imag[k]) : 0xCCCCCCCC  };

            TEST_ASSERT_EQUAL_MSG(exp.re, A[k].re, v);
            TEST_ASSERT_EQUAL_MSG(exp.im, A[k].im, v);
        }
    }
}
#undef MAX_LEN
#undef REPS

void test_xs3_vect_complex_s16_to_complex_s32()
{
    SET_TEST_FILE();

    RUN_TEST(test_xs3_vect_complex_s16_to_complex_s32_basic);
    RUN_TEST(test_xs3_vect_complex_s16_to_complex_s32_random);
}