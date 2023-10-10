// Copyright 2020-2022 XMOS LIMITED.
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

TEST_GROUP_RUNNER(filter_biquad_s32) {
  RUN_TEST_CASE(filter_biquad_s32, case0);
  RUN_TEST_CASE(filter_biquad_s32, case1);
  RUN_TEST_CASE(filter_biquad_s32, case2);
  RUN_TEST_CASE(filter_biquad_s32, case3);
}

TEST_GROUP(filter_biquad_s32);
TEST_SETUP(filter_biquad_s32) { fflush(stdout); }
TEST_TEAR_DOWN(filter_biquad_s32) {}

static char msg_buff[200];

#define MUL32(X, Y)     (((((int64_t)(X)) * (Y)) + (1<<29)) >> 30)

TEST(filter_biquad_s32, case0)
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

    int32_t res = filter_biquad_s32(&filter, 100);
    TEST_ASSERT_EQUAL(100, res);

    res = filter_biquad_s32(&filter, 50);
    TEST_ASSERT_EQUAL(250, res);

    res = filter_biquad_s32(&filter, 4000);
    TEST_ASSERT_EQUAL(4500, res);

}

TEST(filter_biquad_s32, case1)
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
    int32_t res = filter_biquad_s32(&filter, 100);
    TEST_ASSERT_EQUAL(100, res);

    // x[n  ] = 50
    // y[0][n] = 1.0 * (  50 + 100 +   0 + 100 +   0  ) = 250
    // y[1][n] = 1.0 * ( 250 + 100 +   0 + 100 +   0  ) = 450
    res = filter_biquad_s32(&filter, 50);
    TEST_ASSERT_EQUAL(450, res);

    // x[n  ] = 4000
    // y[0][n] = 1.0 * ( 4000 +   50 +  100 +  250 +  100  ) = 4500
    // y[1][n] = 1.0 * ( 4500 +  250 +  100 +  450 +  100  ) = 5400
    res = filter_biquad_s32(&filter, 4000);
    TEST_ASSERT_EQUAL(5400, res);

}

TEST(filter_biquad_s32, case2)
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
    int32_t res = filter_biquad_s32(&filter, 100);
    TEST_ASSERT_EQUAL(50, res);

    // x[n] = 50
    // y[0][n] = 1.0 * (  50 + 100 +   0 + 100 +   0  ) = 250
    // y[1][n] = 1.0 * ( 250 + 100 +   0 + 100 +   0  ) = 450
    // y[2][n] = 0.5 * ( 450 + 100 +   0 +  50 +   0  ) = 300
    res = filter_biquad_s32(&filter, 50);
    TEST_ASSERT_EQUAL(300, res);

    // x[n] = 4000
    // y[0][n] = 1.0 * ( 4000 +   50 +  100 +  250 +  100  ) = 4500
    // y[1][n] = 1.0 * ( 4500 +  250 +  100 +  450 +  100  ) = 5400
    // y[2][n] = 0.5 * ( 5400 +  450 +  100 +  300 +   50  ) = 3150
    res = filter_biquad_s32(&filter, 4000);
    TEST_ASSERT_EQUAL(3150, res);

}


// 8 random biquad filters generated via a python script with 178 samples
TEST(filter_biquad_s32, case3)
{
    int32_t samples[178] = {467355140, 971723598, 79768692, 652849784, -118844908, -520103702, 374884846, -739198562, -414013243, 345525677, -673298012, 410683536, -299712938, 666935681, 321601895, -187155238, 344889499, 84931353, -885232369, 928451560, -467729332, 132105374, 365574900, 456665658, -362311973, -649123253, 98029624, 572539415, 829228406, -349545957, -461191696, -611451899, -135178949, -750658420, -709401475, -1017559348, -473432490, -371696656, 494715916, -543691177, -1027757324, 6765789, 555023241, 77212252, -584321436, 147595474, 285683530, -970382736, -1018707986, -724007596, 29694353, -828911888, 281791397, -75658141, -658620194, -297350857, -396206587, 609759735, -1034881270, -334217876, -226183923, -339027215, 211164694, 723677393, 253866943, -110891175, 762147688, 117905672, -720436127, 972721314, 666144365, -308416287, -536237728, 377128654, -592756650, 974628290, -591699333, 839150681, -875511139, 818945114, -758792987, 991647796, -201556583, -119439615, 60968928, 827513344, 532324579, -1041054888, -819655938, 832849431, 46062955, -209929365, -962867972, -1014364259, -769960394, 420341716, 923823820, -577015617, -435401422, -616684968, -367061420, 125604388, 840098588, 849847137, 937682217, 732137027, -287381689, 749432045, 540025126, 491244761, -629457675, -698629749, -540641972, 193327989, -783612367, 844708340, 169052394, 1024559084, 196122706, -743427061, 127928704, 492961145, 385129395, 307296909, -709077560, 1048453431, 977816127, 160566685, 900290964, -504865295, -574641136, -851558428, -424875007, -30590096, -515012070, 879329697, -805401821, -155272211, 296083325, -180086418, 628663269, 95853821, 961575690, -82549741, 1040124329, 238205025, -211321146, -207618890, 1020630513, -242362949, -91896424, 1005647090, 157633513, 235397546, 714501800, 909708920, -1051283590, 279052694, 357339547, -20184101, -274981120, -792556804, 268851821, -511896176, -481448027, -57034235, -33337537, -162871606, -213237059, 995953783, -647345663, 584026311, 384212829, -940323367, 351803584, -681075411, 381091822, 115462249};
    int32_t coef[0x5][0x8] = {{0xffd2c7, 0x4beef46, 0x5d6f096, 0x6a59c51, 0x23efe0f, 0x65dca2c, 0x81f6cc9, 0x470261c},{0x10b11f1, 0xcb554f4, 0x35ad5b6, 0xcb0581d, 0x48cd430, 0xa483402, 0x82f39fe, 0x7553711},{0x70b10d2, 0x62592d8, 0x5277b9, 0xb06c9f5, 0x94d5273, 0x7b38864, 0x93feab8, 0x7a755bb},{-0x630e2ca, -0x4d08143, -0x4a1e3d5, -0x3f079ec, -0x4fcbd42, -0xa03eada, -0xabe890a, -0x25a53b8},{-0xc40f9f8, -0x12aa0ae, -0xa997bf2, -0x5a4b3b9, -0x75b38f, -0x724005f, -0x2f0f64f, -0x939bdf2}};
    int32_t Y_exp[178] = {0, 2, 15, 77, 282, 801, 1792, 3187, 4475, 4840, 3754, 1528, -830, -2407, -2969, -2803, -2209, -1302, -174, 981, 1923, 2386, 2135, 1214, 109, -529, -399, 292, 1070, 1470, 1203, 335, -528, -508, 679, 1956, 1560, -1046, -4352, -6403, -6859, -6788, -6811, -6348, -4858, -3025, -2097, -2297, -2482, -1600, -147, 346, -833, -2916, -4853, -6260, -6888, -6216, -4316, -2418, -1772, -2326, -3062, -3236, -2955, -2786, -2982, -2998, -1949, 267, 2602, 3831, 3666, 2764, 1956, 1683, 1871, 2043, 1599, 392, -814, -925, 200, 1394, 1541, 827, 273, 436, 1035, 1507, 1648, 1643, 1621, 1197, -101, -1831, -2726, -2399, -2269, -3771, -5955, -6216, -3581, -204, 920, -832, -3172, -3259, -285, 4106, 7514, 8601, 7573, 5574, 3816, 2721, 1615, -396, -3028, -4681, -4004, -1352, 1683, 3742, 4342, 3524, 1818, 434, 524, 1900, 3193, 3598, 3797, 4589, 5227, 4044, 642, -3366, -5844, -5882, -4100, -1867, -405, -108, -392, -248, 861, 2597, 4164, 5012, 5114, 4638, 3675, 2439, 1478, 1360, 2070, 3017, 3700, 4133, 4443, 4359, 3487, 1928, 356, -596, -960, -1404, -2377, -3419, -3678, -3055, -2184, -1431, -594, 427, 1322, 1728};
    filter_biquad_s32_t filter;
    filter.biquad_count = 8;
    memset(&filter.state, 0, sizeof(filter.state));
    memcpy(&filter.coef, coef, sizeof(coef));

    const unsigned N_samples = sizeof(samples) >> 2;

    for(unsigned int i = 0; i < N_samples; i++){
        int32_t y = filter_biquad_s32(&filter, samples[i]);

        if(Y_exp[i] != y)
            sprintf(msg_buff, "(i=%d)", i);

        TEST_ASSERT_EQUAL_MESSAGE(Y_exp[i], y, msg_buff);
    }


}