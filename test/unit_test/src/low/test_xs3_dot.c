
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

#if DEBUG_ON || 0
#undef DEBUG_ON
#define DEBUG_ON    (1)
#endif


static void test_xs3_dot_s16_basic()
{
    PRINTF("%s...\n", __func__);

    typedef struct {
        struct{ int16_t b;  int16_t c;  } input;
        struct{ int b;      int c;      } shr;
        int sat;
        unsigned len;

        int16_t expected;

        unsigned line;
    } test_case_t;

    test_case_t casses[] = {
        //  input{       b,       c },    shr{    b,      c },    sat,      len,    expected,       line
        {        {  0x0000,  0x0000 },       {    0,      0 },      0,        1,      0x0000,     __LINE__ },
        {        {  0x0000,  0x0000 },       {    0,      0 },      0,        2,      0x0000,     __LINE__ },
        {        {  0x0000,  0x0000 },       {    0,      0 },      0,       16,      0x0000,     __LINE__ },
        {        {  0x0000,  0x0000 },       {    0,      0 },      0,       32,      0x0000,     __LINE__ },
        {        {  0x0000,  0x0000 },       {    0,      0 },      0,       40,      0x0000,     __LINE__ },
        
        {        {  0x0001,  0x0000 },       {    0,      0 },      0,        1,      0x0000,     __LINE__ },
        {        {  0x0000,  0x0010 },       {    0,      0 },      0,        2,      0x0000,     __LINE__ },
        {        {  0x2200,  0x0000 },       {    0,      0 },      0,       16,      0x0000,     __LINE__ },

        {        {  0x0001,  0x0001 },       {    0,      0 },      0,        1,      0x0001,     __LINE__ },
        {        {  0x0001,  0x0001 },       {    0,      0 },      0,        2,      0x0002,     __LINE__ },
        {        {  0x0001,  0x0001 },       {    0,      0 },      0,        8,      0x0008,     __LINE__ },
        {        {  0x0001,  0x0001 },       {    0,      0 },      0,       16,      0x0010,     __LINE__ },
        {        {  0x0001,  0x0001 },       {    0,      0 },      0,       32,      0x0020,     __LINE__ },
        {        {  0x0001,  0x0001 },       {    0,      0 },      0,       40,      0x0028,     __LINE__ },

        {        {  0x0010,  0x0010 },       {    0,      0 },      0,        1,      0x0100,     __LINE__ },
        {        {  0x0010,  0x0010 },       {    0,      0 },      0,       25,      0x1900,     __LINE__ },
        {        {  0x0010,  0x0010 },       {    0,      0 },      8,       25,      0x0019,     __LINE__ },
        {        {  0x0010,  0x0010 },       {    4,      4 },      0,       25,      0x0019,     __LINE__ },
        {        {  0x0010,  0x0010 },       {    2,      2 },      4,       25,      0x0019,     __LINE__ },
        {        {  0x0010,  0x0010 },       {   -1,      0 },      4,       25,      0x0320,     __LINE__ },
        {        {  0x0010,  0x0010 },       {   -1,     -1 },      0,       25,      0x6400,     __LINE__ },

    };

    const unsigned start_case = 0;

    const unsigned N_cases = sizeof(casses)/sizeof(test_case_t);

    for(int v = start_case; v < N_cases; v++){
        PRINTF("\ttest vector %d..\n", v);
        test_case_t* casse = &casses[v];
        
        unsigned len = casse->len;

        int16_t WORD_ALIGNED B[40];
        int16_t WORD_ALIGNED C[40];
        int16_t result;


        for(int i = 0; i < len; i++){
            B[i] = casse->input.b;
            C[i] = casse->input.c;
        }

        result = xs3_dot_s16(B, C, len, casse->shr.b, casse->shr.c, casse->sat);

        TEST_ASSERT_EQUAL_MSG(casse->expected, result, casse->line);
    }
}


// #define MAX_LEN     200
// #define REPS        100
// static void test_xs3_dot_s16_random()
// {
//     PRINTF("%s...\n", __func__);
//     seed = 343446;

//     int32_t result;
//     int16_t B[MAX_LEN];

//     for(int v = 0; v < REPS; v++){

//         PRINTF("\trepetition %d.. (seed: 0x%08X)\n", v, seed);

//         unsigned len = (pseudo_rand_uint32(&seed) % MAX_LEN) + 1;
        
//         for(int i = 0; i < len; i++){
//             B[i] = pseudo_rand_int16(&seed);
//         }

//         result = xs3_sum_s16(B, len);

//         int32_t exp = 0;
//         for(int i = 0; i < len; i++){
//             exp += B[i];
//         }

//         TEST_ASSERT_EQUAL(exp, result);
        
//     }
// }
// #undef MAX_LEN
// #undef REPS




static void test_xs3_dot_s32_basic()
{
    PRINTF("%s...\n", __func__);

    typedef struct {
        struct{ int32_t b;  int32_t c;  } input;
        struct{ int b;      int c;      } shr;
        int sat;
        unsigned len;

        int32_t expected;

        unsigned line;
    } test_case_t;

    test_case_t casses[] = {
        //  input{           b,           c },  shr{   b,    c },    sat,      len,        expected,       line
        {        {  0x00000000,  0x00000000 },     {   0,    0 },      0,        1,      0x00000000,     __LINE__ },
        {        {  0x00000000,  0x00000000 },     {    0,   0 },      0,        2,      0x00000000,     __LINE__ },
        {        {  0x00000000,  0x00000000 },     {    0,   0 },      0,       16,      0x00000000,     __LINE__ },
        {        {  0x00000000,  0x00000000 },     {    0,   0 },      0,       32,      0x00000000,     __LINE__ },
        {        {  0x00000000,  0x00000000 },     {    0,   0 },      0,       40,      0x00000000,     __LINE__ },
        {        {  0x00010000,  0x00000000 },     {    0,   0 },      0,        1,      0x00000000,     __LINE__ },
        {        {  0x00000000,  0x00100000 },     {    0,   0 },      0,        2,      0x00000000,     __LINE__ },
        {        {  0x22000000,  0x00000000 },     {    0,   0 },      0,       16,      0x00000000,     __LINE__ },
        {        {  0x40000000,  0x00010000 },     {    0,   0 },      0,        1,      0x00010000,     __LINE__ },
        {        {  0x40000000,  0x00010000 },     {    0,   0 },      0,        2,      0x00020000,     __LINE__ },
        {        {  0x40000000,  0x00010000 },     {    0,   0 },      0,        8,      0x00080000,     __LINE__ },
        {        {  0x40000000,  0x00010000 },     {    0,   0 },      0,       16,      0x00100000,     __LINE__ },
        {        {  0x40000000,  0x00010000 },     {    0,   0 },      0,       32,      0x00200000,     __LINE__ },
        {        {  0x40000000,  0x00010000 },     {    0,   0 },      0,       40,      0x00280000,     __LINE__ },
        {        {  0x04000000,  0x01000000 },     {    0,   0 },      0,        1,      0x00100000,     __LINE__ },
        {        {  0x04000000,  0x00100000 },     {    0,   0 },      0,       25,      0x00190000,     __LINE__ },
        {        {  0x04000000,  0x00100000 },     {    0,   0 },      8,       25,      0x00001900,     __LINE__ },
        {        {  0x04000000,  0x00100000 },     {    4,   4 },      0,       25,      0x00001900,     __LINE__ },
        {        {  0x04000000,  0x00100000 },     {    2,   2 },      4,       25,      0x00001900,     __LINE__ },
        {        {  0x04000000,  0x00100000 },     {   -1,   0 },      4,       25,      0x00032000,     __LINE__ },
        {        {  0x04000000,  0x00100000 },     {   -1,  -1 },      0,       25,      0x00640000,     __LINE__ },
    };

    const unsigned start_case = 0;

    const unsigned N_cases = sizeof(casses)/sizeof(test_case_t);
    for(int v = start_case; v < N_cases; v++){
        PRINTF("\ttest vector %d..\n", v);
        
        test_case_t* casse = &casses[v];

        unsigned len = casse->len;

        int32_t WORD_ALIGNED B[40];
        int32_t WORD_ALIGNED C[40];
        int32_t result;

        for(int i = 0; i < len; i++){
            B[i] = casse->input.b;
            C[i] = casse->input.c;
        }

        result = xs3_dot_s32(B, C, len, casse->shr.b, casse->shr.c, casse->sat);

        TEST_ASSERT_EQUAL_MSG(casse->expected, result, casse->line);
    }
}



#define RUN(TEST) do{UnityBegin(__FILE__); RUN_TEST(TEST); retval = UnityEnd(); printf("\n\n");}while(0);


int test_xs3_dot()
{
    int retval = 0;
    
    UnityBegin(__FILE__);

    RUN_TEST(test_xs3_dot_s16_basic);

    RUN_TEST(test_xs3_dot_s32_basic);

    retval = UnityEnd();
    

    return retval;
}