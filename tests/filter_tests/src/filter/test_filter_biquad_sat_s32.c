// Copyright 2020-2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xmath/xmath.h"
#include "../src/vect/vpu_helper.h"
#include "../tst_common.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(filter_biquad_sat_s32) {
  RUN_TEST_CASE(filter_biquad_sat_s32, case0);
  RUN_TEST_CASE(filter_biquad_sat_s32, case1);
  RUN_TEST_CASE(filter_biquad_sat_s32, case2);
  RUN_TEST_CASE(filter_biquad_sat_s32, case3);
  RUN_TEST_CASE(filter_biquad_sat_s32, case4);
  RUN_TEST_CASE(filter_biquad_sat_s32, case5);
}

TEST_GROUP(filter_biquad_sat_s32);
TEST_SETUP(filter_biquad_sat_s32) { fflush(stdout); }
TEST_TEAR_DOWN(filter_biquad_sat_s32) {}

static char msg_buff[200];

#define MUL32(X, Y)     (((((int64_t)(X)) * (Y)) + (1<<29)) >> 30)

TEST(filter_biquad_sat_s32, case0)
{
    filter_biquad_s32_t filter;

    filter.biquad_count = 1;

    memset(filter.state, 0, sizeof(filter.state));

    for(int i = 0; i < 5; i++){
        for(unsigned int j = 0; j < filter.biquad_count; j++)
            filter.coef[i][j] = 0x40000000;
        for(int j = filter.biquad_count; j < 8; j++)
            filter.coef[i][j] = 0;
    }

    int32_t res = filter_biquad_sat_s32(&filter, 100);
    TEST_ASSERT_EQUAL(100, res);

    res = filter_biquad_sat_s32(&filter, 50);
    TEST_ASSERT_EQUAL(250, res);

    res = filter_biquad_sat_s32(&filter, 4000);
    TEST_ASSERT_EQUAL(4500, res);
}

TEST(filter_biquad_sat_s32, case1)
{
    filter_biquad_s32_t filter;

    filter.biquad_count = 2;

    memset(filter.state, 0, sizeof(filter.state));

    for(int i = 0; i < 5; i++){
        for(unsigned int j = 0; j < filter.biquad_count; j++)
            filter.coef[i][j] = 0x40000000;
        for(int j = filter.biquad_count; j < 8; j++)
            filter.coef[i][j] = 0;
    }

    // x[n  ] = 100
    // y[0][n] = 1.0 * (  100 + 0 + 0 + 0 + 0  ) = 100
    // y[1][n] = 1.0 * (  100 + 0 + 0 + 0 + 0  ) = 100
    int32_t res = filter_biquad_sat_s32(&filter, 100);
    TEST_ASSERT_EQUAL(100, res);

    // x[n  ] = 50
    // y[0][n] = 1.0 * (  50 + 100 +   0 + 100 +   0  ) = 250
    // y[1][n] = 1.0 * ( 250 + 100 +   0 + 100 +   0  ) = 450
    res = filter_biquad_sat_s32(&filter, 50);
    TEST_ASSERT_EQUAL(450, res);

    // x[n  ] = 4000
    // y[0][n] = 1.0 * ( 4000 +   50 +  100 +  250 +  100  ) = 4500
    // y[1][n] = 1.0 * ( 4500 +  250 +  100 +  450 +  100  ) = 5400
    res = filter_biquad_sat_s32(&filter, 4000);
    TEST_ASSERT_EQUAL(5400, res);
}

TEST(filter_biquad_sat_s32, case2)
{
    filter_biquad_s32_t filter;

    filter.biquad_count = 3;

    memset(filter.state, 0, sizeof(filter.state));

    for(int i = 0; i < 5; i++){
        for(unsigned int j = 0; j < filter.biquad_count; j++)
            filter.coef[i][j] = (j==(filter.biquad_count-1))? 0x20000000 : 0x40000000;
        for(int j = filter.biquad_count; j < 8; j++)
            filter.coef[i][j] = 0;
    }

    // x[n] = 100
    // y[0][n] = 1.0 * (  100 + 0 + 0 + 0 + 0  ) = 100
    // y[1][n] = 1.0 * (  100 + 0 + 0 + 0 + 0  ) = 100
    // y[2][n] = 0.5 * (  100 + 0 + 0 + 0 + 0  ) =  50
    int32_t res = filter_biquad_sat_s32(&filter, 100);
    TEST_ASSERT_EQUAL(50, res);

    // x[n] = 50
    // y[0][n] = 1.0 * (  50 + 100 +   0 + 100 +   0  ) = 250
    // y[1][n] = 1.0 * ( 250 + 100 +   0 + 100 +   0  ) = 450
    // y[2][n] = 0.5 * ( 450 + 100 +   0 +  50 +   0  ) = 300
    res = filter_biquad_sat_s32(&filter, 50);
    TEST_ASSERT_EQUAL(300, res);

    // x[n] = 4000
    // y[0][n] = 1.0 * ( 4000 +   50 +  100 +  250 +  100  ) = 4500
    // y[1][n] = 1.0 * ( 4500 +  250 +  100 +  450 +  100  ) = 5400
    // y[2][n] = 0.5 * ( 5400 +  450 +  100 +  300 +   50  ) = 3150
    res = filter_biquad_sat_s32(&filter, 4000);
    TEST_ASSERT_EQUAL(3150, res);
}

// 8 chosen biquad filters generated via a python script with 126 samples
// for details see test_filter_biquad_sat_s32_case3.py
TEST(filter_biquad_sat_s32, case3)
{
    int32_t samples[126] = {0, 0, 0, 0, -1, 1, -2, 2, -3, 3, -5, 5, -7, 7, -10, 10, -15, 15, -22, 22, -31, 31, -44, 44, -63, 63, -90, 90, -127, 127, -180, 180, -255, 255, -361, 361, -511, 511, -723, 723, -1023, 1023, -1447, 1447, -2047, 2047, -2895, 2895, -4095, 4095, -5792, 5792, -8191, 8191, -11584, 11584, -16383, 16383, -23169, 23169, -32767, 32767, -46340, 46340, -65535, 65535, -92681, 92681, -131071, 131071, -185363, 185363, -262143, 262143, -370727, 370727, -524287, 524287, -741454, 741454, -1048575, 1048575, -1482909, 1482909, -2097151, 2097151, -2965820, 2965820, -4194303, 4194303, -5931641, 5931641, -8388607, 8388607, -11863282, 11863282, -16777215, 16777215, -23726565, 23726565, -33554431, 33554431, -47453132, 47453132, -67108863, 67108863, -94906265, 94906265, -134217727, 134217727, -189812530, 189812530, -268435455, 268435455, -379625061, 379625061, -536870911, 536870911, -759250124, 759250124, -1073741823, 1073741823, -1518500249, 1518500249, -2147483647, 2147483647};
    int32_t coef[0x5][0x8] = {{0x376f5206, 0x317690be, 0x2d914a70, 0x2b5fd1e3, 0x2aaaaaab, 0x2b5fd1e3, 0x2d914a70, 0x40c585a6},{0x968e8127, 0xaff789ef, 0xca6ea0d5, 0xe5317640, 0x0, 0x1ace89c0, 0x35915f2b, 0x8848a545},{0x376f5206, 0x317690be, 0x2d914a70, 0x2b5fd1e3, 0x2aaaaaab, 0x2b5fd1e3, 0x2d914a70, 0x382c64b0},{0x69717ed9, 0x50087611, 0x35915f2b, 0x1ace89c0, 0x0, 0xe5317640, 0xca6ea0d5, 0x77e549f2},{0xd1215bf3, 0xdd12de84, 0xe4dd6b21, 0xe9405c39, 0xeaaaaaab, 0xe9405c39, 0xe4dd6b21, 0xc73c04e0}};
    int32_t Y_exp[126] = {0, 0, 0, 0, -1, 1, -2, 2, -1, 1, -5, 2, -7, 3, -8, 7, -10, 8, -13, 10, -16, 19, -22, 32, -31, 47, -43, 65, -68, 87, -102, 118, -152, 158, -221, 221, -316, 312, -443, 444, -622, 635, -874, 906, -1232, 1283, -1736, 1817, -2454, 2573, -3467, 3648, -4898, 5167, -6924, 7314, -9784, 10345, -13829, 14630, -19561, 20693, -27664, 29264, -39124, 41383, -55331, 58525, -78252, 82767, -110663, 117054, -156499, 165539, -221319, 234117, -312985, 331099, -442620, 468250, -625957, 662203, -885241, 936493, -1251921, 1324404, -1770484, 1872989, -2503845, 2648804, -3540974, 3745973, -5007695, 5297603, -7081955, 7491936, -10015405, 10595195, -14163927, 14983862, -20030823, 21190374, -28327871, 29967707, -40061671, 42380729, -56655770, 59935396, -80123365, 84761445, -113311550, 119870785, -160246735, 169522890, -226623109, 239741572, -320493471, 339045785, -453246207, 479483154, -640986925, 678091588, -906492392, 958966333, -1281973829, 1352734966};
    filter_biquad_s32_t filter;
    filter.biquad_count = 8;
    memset(&filter.state, 0, sizeof(filter.state));
    memcpy(&filter.coef, coef, sizeof(coef));

    const unsigned N_samples = sizeof(samples) >> 2;

    for(unsigned int i = 0; i < N_samples; i++){
        int32_t y = filter_biquad_sat_s32(&filter, samples[i]);

        if(Y_exp[i] != y)
            sprintf(msg_buff, "(i=%d)", i);

        TEST_ASSERT_EQUAL_MESSAGE(Y_exp[i], y, msg_buff);
    }
}

// Test saturation by using a biquad that just amplifies the input by 1.5
TEST(filter_biquad_sat_s32, case4)
{
    filter_biquad_s32_t filter;

    filter.biquad_count = 1;

    memset(filter.state, 0, sizeof(filter.state));

    for(int i = 0; i < 5; i++){
        for(unsigned int j = 0; j < filter.biquad_count; j++)
            if (i == 0){
                filter.coef[i][j] = 0x60000000;
            }
            else {
                // y coeffs are zero so no feedback, keeps the maths easy
                filter.coef[i][j] = 0;
            }
        for(int j = filter.biquad_count; j < 8; j++)
            filter.coef[i][j] = 0;
    }

    int32_t res = filter_biquad_sat_s32(&filter, 100);
    TEST_ASSERT_EQUAL(150, res);

    res = filter_biquad_sat_s32(&filter, 50);
    TEST_ASSERT_EQUAL(75, res);

    // this should saturate as it's already 2**31-1
    res = filter_biquad_sat_s32(&filter, INT32_MAX);
    TEST_ASSERT_EQUAL(INT32_MAX, res);

    res = filter_biquad_sat_s32(&filter, INT32_MIN);
    TEST_ASSERT_EQUAL(INT32_MIN + 1, res);
}

// Test a biquad that overflows halfway through the accumulator, 
// but ends up back in the safe zone
TEST(filter_biquad_sat_s32, case5)
{
    filter_biquad_s32_t filter;

    filter.biquad_count = 1;

    memset(filter.state, 0, sizeof(filter.state));

    // with these coeffs, if b0 = 0 the output would overflow
    // [1.0, -1.5, 0.5625, 1.5, -0.5625]
    filter.coef[0][0] = 0x40000000;
    filter.coef[1][0] = 0xa0000000;
    filter.coef[2][0] = 0x24000000;
    filter.coef[3][0] = 0x60000000;
    filter.coef[4][0] = 0xdc000000;

    int32_t res = filter_biquad_sat_s32(&filter, 1073741823);
    TEST_ASSERT_EQUAL(1073741823, res);

    res = filter_biquad_sat_s32(&filter, 1073741823);
    TEST_ASSERT_EQUAL(1073741824, res);

    // this should saturate as it's already 2**31-1
    res = filter_biquad_sat_s32(&filter, 1073741823);
    TEST_ASSERT_EQUAL(1073741825, res);

    res = filter_biquad_sat_s32(&filter, 1073741823);
    TEST_ASSERT_EQUAL(1073741826, res);
}
