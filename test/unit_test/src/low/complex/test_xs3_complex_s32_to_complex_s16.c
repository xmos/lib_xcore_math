
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
static void test_xs3_complex_s32_to_complex_s16_basic()
{
    PRINTF("%s...\n", __func__);

    typedef struct {
        complex_s32_t b;
        int b_shr;
        complex_s16_t expected;
        unsigned line;
    } test_case_t;

    test_case_t casses[] = {
        // b{          re,          im }, b_shr, exp{      re,    im},       line #
        {   {  0x00000000,  0x00000000 },     0,    {  0x0000,  0x0000 },    __LINE__},
        {   {  0x00000001,  0x00000000 },     0,    {  0x0001,  0x0000 },    __LINE__},
        {   {  0x00000000,  0x00000001 },     0,    {  0x0000,  0x0001 },    __LINE__},
        {   { -0x00000001,  0x00000000 },     0,    { -0x0001,  0x0000 },    __LINE__},
        {   {  0x00000000, -0x00000001 },     0,    {  0x0000, -0x0001 },    __LINE__},
        {   {  0x00000010,  0x00000020 },     0,    {  0x0010,  0x0020 },    __LINE__},
        {   {  0x00000100,  0x00000301 },     0,    {  0x0100,  0x0301 },    __LINE__},
        {   { -0x00000100,  0x00000020 },     0,    { -0x0100,  0x0020 },    __LINE__},
        {   {  0x00008000,  0x00000000 },     0,    {  0x7FFF,  0x0000 },    __LINE__},
        {   {  0x00000000,  0x00008000 },     0,    {  0x0000,  0x7FFF },    __LINE__},
        {   { -0x00008000,  0x00000000 },     0,    { -0x7FFF,  0x0000 },    __LINE__},
        {   {  0x00000000, -0x00008000 },     0,    {  0x0000, -0x7FFF },    __LINE__},

            //ties round towards positive infinity.
        {   {  0x00000001,  0x00000000 },     1,    {  0x0001,  0x0000 },    __LINE__},
        {   {  0x00000000,  0x00000001 },     1,    {  0x0000,  0x0001 },    __LINE__},
        {   { -0x00000001,  0x00000000 },     1,    {  0x0000,  0x0000 },    __LINE__},
        {   {  0x00000000, -0x00000001 },     1,    {  0x0000,  0x0000 },    __LINE__},
        {   {  0x00018000, -0x00018000 },    16,    {  0x0002, -0x0001 },    __LINE__},
        {   { -0x00018000,  0x00018000 },    16,    { -0x0001,  0x0002 },    __LINE__},

        {   {  0x00000001, -0x00000002 },   -14,    {  0x4000, -0x7FFF },    __LINE__},

    };

    const unsigned N_cases = sizeof(casses)/sizeof(test_case_t);

    const unsigned start_case = 0;
    char buff[100];
    for(int v = start_case; v < N_cases; v++){
        PRINTF("\ttest vector %d..\n", v);
        
        test_case_t* casse = &casses[v];
        sprintf(buff, "(line %u)", casse->line);

        unsigned lengths[] = {1, 4, 16, 32, 40 };
        complex_s32_t B[MAX_LEN];
        struct {
            int16_t real[MAX_LEN];
            int16_t imag[MAX_LEN];
        } A;

        for(int i = 0; i < sizeof(lengths)/sizeof(lengths[0]); i++){
            unsigned len = lengths[i];
            PRINTF("\tlength %u..\n", len);

            for(int i = 0; i < MAX_LEN; i++){
                B[i] = casse->b;
                A.real[i] = 0xCCCC;
                A.imag[i] = 0xCCCC;
            }

            xs3_complex_s32_to_complex_s16(A.real, A.imag, B, len, casse->b_shr);

            for(int k = 0; k < len; k++) {
                TEST_ASSERT_EQUAL_MESSAGE(casse->expected.re, A.real[k], buff);
                TEST_ASSERT_EQUAL_MESSAGE(casse->expected.im, A.imag[k], buff);
            }
            for(int k = len; k < MAX_LEN; k++){
                TEST_ASSERT_EQUAL_MESSAGE((int16_t) 0xCCCC, A.real[k], buff);
                TEST_ASSERT_EQUAL_MESSAGE((int16_t) 0xCCCC, A.imag[k], buff);
            }
        }
    }
}
#undef MAC_LEN


void test_xs3_complex_s32_to_complex_s16()
{
    SET_TEST_FILE();

    RUN_TEST(test_xs3_complex_s32_to_complex_s16_basic);

}