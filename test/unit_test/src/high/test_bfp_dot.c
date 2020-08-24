
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xs3_math.h"
#include "../src/low/vpu_helper.h"

#include "../tst_common.h"

#include "unity.h"

#if DEBUG_ON || 0
#undef DEBUG_ON
#define DEBUG_ON    (1)
#endif


#define REPS        IF_QUICK_TEST(10, 100)
#define MAX_LEN     256 

static char msg_buff[200];

#define TEST_ASSERT_EQUAL_MSG(EXPECTED, ACTUAL, EXTRA, LINE_NUM)   do{          \
    if((EXPECTED)!=(ACTUAL)) {                                                  \
      sprintf(msg_buff, "%s (line %u)", (EXTRA), (LINE_NUM));                   \
      TEST_ASSERT_EQUAL_MESSAGE((EXPECTED), (ACTUAL), msg_buff);                \
    }} while(0)




void bfp_dot_s16_calc_params(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    right_shift_t* c_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr,
    const unsigned length);
    
void bfp_dot_s32_calc_params(
    exponent_t* a_exp,
    right_shift_t* b_shr,
    right_shift_t* c_shr,
    const exponent_t b_exp,
    const exponent_t c_exp,
    const headroom_t b_hr,
    const headroom_t c_hr,
    const unsigned length);


static void test_bfp_dot_s16()
{
    PRINTF("%s...\t(random vectors)\n", __func__);

    unsigned seed = 6745;

    int16_t dataB[MAX_LEN];
    int16_t dataC[MAX_LEN];
    bfp_s16_t B, C;

    B.data = dataB;
    C.data = dataC;

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        test_random_bfp_s16(&B, MAX_LEN, &seed, NULL, 0);
        test_random_bfp_s16(&C, MAX_LEN, &seed, NULL, B.length);

        int64_t total = 0;

        exponent_t a_exp;
        right_shift_t b_shr, c_shr;

        bfp_dot_s16_calc_params(&a_exp, &b_shr, &c_shr, B.exp, C.exp, B.hr, C.hr, B.length);
        
        // printf("! %d \n", B.length);

        for(int k = 0; k < B.length; k++){
            int64_t b = (b_shr >= 0)? (B.data[k] >> b_shr) : (B.data[k] << -b_shr);
            int64_t c = (c_shr >= 0)? (C.data[k] >> c_shr) : (C.data[k] << -c_shr);

            // printf("@ %lld \t %d >> %d \n", b,  B.data[k], b_shr);
            // printf("# %lld \t %d >> %d \n\n", c,  C.data[k], c_shr);

            total += b * c;
        }

        int64_t result = bfp_dot_s16(&a_exp, &B, &C);
        
        TEST_ASSERT_EQUAL(total, result);
    }
}

static void test_bfp_dot_s32_A()
{
    PRINTF("%s...\t(random vectors)\n", __func__);


    int32_t dataB[MAX_LEN];
    int32_t dataC[MAX_LEN];
    bfp_s32_t B, C;

    B.data = dataB;
    C.data = dataC;
    
    typedef struct {

        struct {    int32_t value;  exponent_t exp;     } b;
        struct {    int32_t value;  exponent_t exp;     } c;
        struct {    int64_t value;  exponent_t exp;     } expected;

        unsigned length;
        unsigned line;
    } test_case_t;

    test_case_t casses[] = {
        //    b{       value,  exp },      c{      value,  exp },   expected{       value,  exp },  length,  line
        {      {  0x00000000,    0 },       { 0x00000000,    0 },           {  0x00000000,    0 },       1,  __LINE__ },
        {      {  0x00000000,    0 },       { 0x00000000,    0 },           {  0x00000000,    0 },      16,  __LINE__ },
        {      {  0x00000000,    0 },       { 0x00000000,    0 },           {  0x00000000,    0 },      40,  __LINE__ },

        {      {  0x00000001,    0 },       { 0x00000001,    0 },           {  0x00000001,    0 },       1,  __LINE__ },
        {      {  0x00000001,    0 },       { 0x00000001,    0 },           {  0x00000010,    0 },      16,  __LINE__ },
        {      {  0x00000001,    0 },       { 0x00000001,    0 },           {  0x00000028,    0 },      40,  __LINE__ },
        
        {      {  0x00000002,    0 },       { 0x00000001,    0 },           {  0x00000002,    0 },       1,  __LINE__ },
        {      {  0x00000002,    0 },       { 0x00000001,    0 },           {  0x00000020,    0 },      16,  __LINE__ },
        {      {  0x00000002,    0 },       { 0x00000001,    0 },           {  0x00000050,    0 },      40,  __LINE__ },

        {      {  0x00000002,    0 },       { 0x00000003,    0 },           {  0x00000006,    0 },       1,  __LINE__ },
        {      {  0x00000002,    0 },       { 0x00000003,    0 },           {  0x00000060,    0 },      16,  __LINE__ },
        {      {  0x00000002,    0 },       { 0x00000003,    0 },           {  0x00000600,    0 },     256,  __LINE__ },
        
        {      {  0x00000001,    1 },       { 0x00000001,    2 },           {  0x00000001,    3 },       1,  __LINE__ },
        {      {  0x00000001,    2 },       { 0x00000001,    1 },           {  0x00000010,    3 },      16,  __LINE__ },
        {      {  0x00000001,    1 },       { 0x00000001,    2 },           {  0x00000028,    3 },      40,  __LINE__ },
        
        {      {  0x01000000,  -20 },       { 0x000F0000,  -15 },           {  0x0000001e,    4 },       1,  __LINE__ },
        {      {  0x01000000,  -20 },       { 0x000F0000,  -15 },           {  0x0000001e,    8 },      16,  __LINE__ },
        {      {  0x01000000,  -20 },       { 0x000F0000,  -15 },           {  0x00005dc0,    0 },      50,  __LINE__ },
    };


    const unsigned N_cases = sizeof(casses)/sizeof(test_case_t);
    const unsigned start_case = 0;

    for(int v = start_case; v < N_cases; v++){
        PRINTF("\ttest vector %d..\n", v);

        test_case_t* casse = &casses[v];

        C.length = B.length = casse->length;
        B.exp = casse->b.exp;
        C.exp = casse->c.exp;

        for(int k = 0; k < casse->length; k++){
            B.data[k] = casse->b.value;
            C.data[k] = casse->c.value;
        }

        B.hr = HR_S32(casse->b.value);
        C.hr = HR_S32(casse->c.value);

        exponent_t a_exp;

        int64_t result = bfp_dot_s32(&a_exp, &B, &C);

        right_shift_t shr = a_exp - casse->expected.exp;

        int64_t exp_mod = (shr >= 0)? (casse->expected.value >> shr) : (casse->expected.value << -shr);

        // printf("!! %lld\n", result);
        // printf("@@ %lld \t %lld\n", casse->expected.value, exp_mod);

        // printf("!!\t%f\n", ldexp(result, a_exp));

        TEST_ASSERT_EQUAL_MSG(exp_mod, result, "", casse->line);
    } 
}

static void test_bfp_dot_s32_B()
{
    PRINTF("%s...\t(random vectors)\n", __func__);

    unsigned seed = 34563;

    int32_t dataB[MAX_LEN];
    int32_t dataC[MAX_LEN];
    bfp_s32_t B, C;

    B.data = dataB;
    C.data = dataC;

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        test_random_bfp_s32(&B, MAX_LEN, &seed, NULL, 0);
        test_random_bfp_s32(&C, MAX_LEN, &seed, NULL, B.length);

        int64_t total = 0;

        exponent_t a_exp;
        right_shift_t b_shr, c_shr;

        bfp_dot_s32_calc_params(&a_exp, &b_shr, &c_shr, B.exp, C.exp, B.hr, C.hr, B.length);
        
        for(int k = 0; k < B.length; k++){
            int64_t b = ASHR(32)(B.data[k], b_shr);
            int64_t c = ASHR(32)(C.data[k], c_shr);

            total += ((b * c) + (1<<29)) >> 30;
        }

        int64_t result = bfp_dot_s32(&a_exp, &B, &C);
        
        TEST_ASSERT_EQUAL(total, result);
    }
}




void test_bfp_dot()
{
    SET_TEST_FILE();
    
    RUN_TEST(test_bfp_dot_s16);
    RUN_TEST(test_bfp_dot_s32_A);
    RUN_TEST(test_bfp_dot_s32_B);
}