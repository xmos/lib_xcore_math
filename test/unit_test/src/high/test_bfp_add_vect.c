
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xs3_math.h"

#include "../tst_common.h"

#include "unity.h"

#if DEBUG_ON || 1
#undef DEBUG_ON
#define DEBUG_ON    (1)
#endif


#define REPS        IF_QUICK_TEST(100, 100)
#define MAX_LEN     16  //Smaller lengths mean larger variance w.r.t. individual element headroom


static unsigned seed = 666;
static char msg_buff[200];

#define TEST_ASSERT_EQUAL_MSG(EXPECTED, ACTUAL, EXTRA, LINE_NUM)   do{          \
    if((EXPECTED)!=(ACTUAL)) {                                                  \
      sprintf(msg_buff, "%s (test vector @ line %u)", (EXTRA), (LINE_NUM));     \
      TEST_ASSERT_EQUAL_MESSAGE((EXPECTED), (ACTUAL), msg_buff);                \
    }} while(0)

static void test_bfp_add_vect_s16_0()
{
    PRINTF("%s...\n", __func__);

    int16_t dataA[MAX_LEN];
    int16_t dataB[MAX_LEN];
    int16_t dataC[MAX_LEN];
    bfp_s16_t A, B, C;

    bfp_init_vect_s16(&A, dataA, 0, MAX_LEN, 0);
    bfp_init_vect_s16(&B, dataB, 0, MAX_LEN, 0);
    bfp_init_vect_s16(&C, dataC, 0, MAX_LEN, 0);

    typedef struct {

        struct {    int16_t value;  exponent_t exp;     } b;
        struct {    int16_t value;  exponent_t exp;     } c;
        struct {    int16_t value;  exponent_t exp;     } expected;

        unsigned line;
    } test_case_t;

    //
    //  X + Y
    //  where X.exp == Y.exp
    //  Worst case is  X.data[k] == Y.data[k]   (which means X.hr == Y.hr)
    //            and  X.data[k] == -(1 << (16-X.hr+1))
    //  in which case result if -2*(1 << (15-X.hr)) = -(1 << (16-X.hr)) = -2^(16-X.hr)
    //  -2^N has headroom  15-N, so    -2^(16-X.hr) has (worst-case) headroom  15-(16-X.hr) = (X.hr-1),
    //  which means it can only be left-shifted X.hr-2 bits. So, the exponent of the result is going
    //  to be  X.exp - (X.hr-2) ==   X.exp - X.hr + 2 ==  X_min_exp + 2
    //
    //  If X_min_exp != Y_min_exp, then we can actually decrease the output exponent by 1, because the sum
    //  can't actually reach saturation. So it becomes X.exp - X.hr + 1
    //


    test_case_t casses[] = {
        //    b{   value,  exp },      c{   value,  exp },   expected{   value,  exp }
        {      { -0x0100,    0 },       { -0x0100,    0 },           { -0x4000,  -5  },        __LINE__ },
        {      {  0x00FF,    0 },       {  0x00FF,    0 },           {  0x3FC0,  -5  },        __LINE__ },
        {      {  0x0001,    0 },       {  0x0000,    0 },           {  0x2000,  -13 },        __LINE__ },
        {      {  0x0002,    0 },       {  0x0000,    0 },           {  0x2000,  -12 },        __LINE__ },
        {      {  0x0000,    0 },       {  0x1111,    0 },           {  0x2222,  -1  },        __LINE__ },
        {      {  0x0001,    0 },       {  0x0001,    0 },           {  0x2000,  -12 },        __LINE__ },

        {      { -0x0100,    1 },       { -0x0100,    1 },           { -0x4000,  -4  },        __LINE__ },
        {      {  0x00FF,    1 },       {  0x00FF,    1 },           {  0x3FC0,  -4  },        __LINE__ },
        {      {  0x0001,    1 },       {  0x0000,    1 },           {  0x2000,  -12 },        __LINE__ },
        {      {  0x0002,    1 },       {  0x0000,    1 },           {  0x2000,  -11 },        __LINE__ },
        {      {  0x0000,    1 },       {  0x1111,    1 },           {  0x2222,   0  },        __LINE__ },
        {      {  0x0001,    1 },       {  0x0001,    1 },           {  0x2000,  -11 },        __LINE__ },
        
        {      {  0x0001,    1 },       {  0x0001,    0 },           {  0x3000,  -12 },        __LINE__ },
        {      {  0x0001,    0 },       {  0x0001,    1 },           {  0x3000,  -12 },        __LINE__ },
        {      {  0x0002,    0 },       {  0x0001,    0 },           {  0x3000,  -12 },        __LINE__ },
        {      {  0x0002,    0 },       {  0x0010,   -4 },           {  0x3000,  -12 },        __LINE__ },

    };

    const unsigned N_cases = sizeof(casses)/sizeof(test_case_t);

    const unsigned start_case = 0;

    for(int v = start_case; v < N_cases; v++){
        PRINTF("\ttest vector %d..\n", v);
        
        test_case_t* casse = &casses[v];

        bfp_set_vect_s16(&B, casse->b.value, casse->b.exp);
        bfp_set_vect_s16(&C, casse->c.value, casse->c.exp);

        bfp_add_vect_s16(&A, &B, &C);

        headroom_t exp_hr = HR_S16(casse->expected.value);

        TEST_ASSERT_EQUAL_MSG(casse->expected.exp, A.exp, "A.exp is wrong.", casse->line);
        TEST_ASSERT_EQUAL_MSG(exp_hr, A.hr, "A.hr is wrong.", casse->line);
        TEST_ASSERT_EQUAL_MSG(B.length, A.length, "A.length is wrong.", casse->line);

        for(int i = 0; i < A.length; i++){
            TEST_ASSERT_EQUAL_MSG(casse->expected.value, A.data[i], "A.data[] is wrong.", casse->line);
        }
    }
}




static void test_bfp_add_vect_s16_1()
{
    PRINTF("%s...\t(random vectors)\n", __func__);

    seed = 8788764;

    int16_t dataA[MAX_LEN];
    int16_t dataB[MAX_LEN];
    int16_t dataC[MAX_LEN];
    int16_t expA[MAX_LEN];
    bfp_s16_t A, B, C;

    A.data = dataA;
    B.data = dataB;
    C.data = dataC;

    double Af[MAX_LEN];
    double Bf[MAX_LEN];
    double Cf[MAX_LEN];

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        test_random_bfp_s16(&B, MAX_LEN, &seed, &A, 0);
        test_random_bfp_s16(&C, MAX_LEN, &seed, &A, B.length);

        test_double_from_s16(Bf, &B);
        test_double_from_s16(Cf, &C);

        for(int i = 0; i < B.length; i++){
            Af[i] = Bf[i] + Cf[i];
        }

        bfp_add_vect_s16(&A, &B, &C);

        test_s16_from_double(expA, Af, MAX_LEN, A.exp);

        for(int i = 0; i < A.length; i++){
            // PRINTF("! %08X\t%08X\n", (unsigned) expA[i], (unsigned) A.data[i]);
            TEST_ASSERT_INT16_WITHIN(1, expA[i], A.data[i]);
        }
    }
}






int test_bfp_add_vect()
{
    int retval = 0;
    UnityBegin(__FILE__);

    RUN_TEST(test_bfp_add_vect_s16_0);
    RUN_TEST(test_bfp_add_vect_s16_1);
    // RUN_TEST(test_bfp_add_vect_s32);

    retval = UnityEnd();
    return retval;
}