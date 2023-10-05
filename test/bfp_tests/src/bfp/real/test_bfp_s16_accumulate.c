// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xmath/xmath.h"

#include "../../tst_common.h"

#include "unity_fixture.h"


TEST_GROUP_RUNNER(bfp_s16_accumulate) {
  RUN_TEST_CASE(bfp_s16_accumulate, bfp_s16_accumulate);
}

TEST_GROUP(bfp_s16_accumulate);
TEST_SETUP(bfp_s16_accumulate) { fflush(stdout); }
TEST_TEAR_DOWN(bfp_s16_accumulate) {}



#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (128)
#else
#  define REPS       (1000)
#  define MAX_LEN    (1024)
#endif



TEST(bfp_s16_accumulate, bfp_s16_accumulate)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  int16_t DWORD_ALIGNED b_data[MAX_LEN];
  split_acc_s32_t DWORD_ALIGNED acc[(MAX_LEN+VPU_INT16_EPV-1) / VPU_INT16_EPV];

  bfp_s16_t B;

  double expected_f[MAX_LEN];
  int32_t DWORD_ALIGNED expected[MAX_LEN];
  int32_t DWORD_ALIGNED actual[MAX_LEN+1];

  bfp_s16_init(&B, b_data, 0, 0, 0);

  for(int v = 0; v < REPS; v++){
    test_random_bfp_s16(&B, MAX_LEN, &seed, NULL, 0);
    B.length = pseudo_rand_int(&seed, 1, MAX_LEN+1);

    exponent_t acc_exp = pseudo_rand_int(&seed, -30, 30);

    B.exp = acc_exp + pseudo_rand_int(&seed, -14, 15);
    printf("\n\n\nREP %d\n\n\n", v);
    printf("    B.length = %d;\n", B.length);
    printf("    acc_exp = %d;\n", acc_exp);
    printf("    B.exp = %d;\n", B.exp);
    for(int k = 0; k < B.length; k++) {
      B.data[k] = pseudo_rand_int16(&seed);
      //printf("    B.data[%d] = %d;\n", k, B.data[k]);
    }

    // Generate random accumulator vector as int32_t's with 1 bit of headroom
    for(int k = 0; k < B.length; k++){
      expected[k] = pseudo_rand_int32(&seed) >> 1;
      //printf("    expected[%d] = %d;\n", k, expected[k]);
    }
    #if 0
     B.length = 128;

     acc_exp = 26;

     B.exp = 30;

     B.data[0] = 18393;

     B.data[1] = 29982;

     B.data[2] = 30917;

     B.data[3] = 26400;

     B.data[4] = 13165;

     B.data[5] = 9203;

     B.data[6] = 15846;

     B.data[7] = 18637;

     B.data[8] = 1999;

     B.data[9] = 15680;

     B.data[10] = 11622;

     B.data[11] = 14955;

     B.data[12] = 17309;

     B.data[13] = 28484;

     B.data[14] = 24716;

     B.data[15] = 20467;

     B.data[16] = 25975;

     B.data[17] = 15552;

     B.data[18] = 12186;

     B.data[19] = 31382;

     B.data[20] = 11131;

     B.data[21] = 19077;

     B.data[22] = 17341;

     B.data[23] = 25111;

     B.data[24] = 22468;

     B.data[25] = 678;

     B.data[26] = 30144;

     B.data[27] = 27350;

     B.data[28] = 22727;

     B.data[29] = 8896;

     B.data[30] = 22250;

     B.data[31] = 8078;

     B.data[32] = 20300;

     B.data[33] = 9804;

     B.data[34] = 29975;

     B.data[35] = 7319;

     B.data[36] = 20587;

     B.data[37] = 7102;

     B.data[38] = 15761;

     B.data[39] = 22082;

     B.data[40] = 3306;

     B.data[41] = 1541;

     B.data[42] = 10389;

     B.data[43] = 13919;

     B.data[44] = 2726;

     B.data[45] = 19857;

     B.data[46] = 22187;

     B.data[47] = 3577;

     B.data[48] = 8878;

     B.data[49] = 16207;

     B.data[50] = 2019;

     B.data[51] = 12305;

     B.data[52] = 28616;

     B.data[53] = 32101;

     B.data[54] = 5459;

     B.data[55] = 6871;

     B.data[56] = 8657;

     B.data[57] = 627;

     B.data[58] = 792;

     B.data[59] = 1317;

     B.data[60] = 25494;

     B.data[61] = 782;

     B.data[62] = 5899;

     B.data[63] = 17630;

     B.data[64] = 16048;

     B.data[65] = 8751;

     B.data[66] = 5409;

     B.data[67] = 5826;

     B.data[68] = 21274;

     B.data[69] = 16771;

     B.data[70] = 1655;

     B.data[71] = 2677;

     B.data[72] = 3411;

     B.data[73] = 30897;

     B.data[74] = 4636;

     B.data[75] = 19083;

     B.data[76] = 32111;

     B.data[77] = 26133;

     B.data[78] = 794;

     B.data[79] = 10035;

     B.data[80] = 14133;

     B.data[81] = 9063;

     B.data[82] = 14313;

     B.data[83] = 1053;

     B.data[84] = 13126;

     B.data[85] = 6765;

     B.data[86] = 27614;

     B.data[87] = 14461;

     B.data[88] = 29398;

     B.data[89] = 10750;

     B.data[90] = 3612;

     B.data[91] = 3678;

     B.data[92] = 27078;

     B.data[93] = 29668;

     B.data[94] = 17964;

     B.data[95] = 8168;

     B.data[96] = 13676;

     B.data[97] = 25458;

     B.data[98] = 3172;

     B.data[99] = 6876;

     B.data[100] = 14194;

     B.data[101] = 24624;

     B.data[102] = 22409;

     B.data[103] = 3271;

     B.data[104] = 6825;

     B.data[105] = 22514;

     B.data[106] = 7223;

     B.data[107] = 19072;

     B.data[108] = 19909;

     B.data[109] = 27024;

     B.data[110] = 20268;

     B.data[111] = 2142;

     B.data[112] = 26657;

     B.data[113] = 10516;

     B.data[114] = 27362;

     B.data[115] = 15783;

     B.data[116] = 17016;

     B.data[117] = 21760;

     B.data[118] = 3693;

     B.data[119] = 32416;

     B.data[120] = 14719;

     B.data[121] = 10562;

     B.data[122] = 20960;

     B.data[123] = 13791;

     B.data[124] = 12391;

     B.data[125] = 14676;

     B.data[126] = 8882;

     B.data[127] = 10248;

     expected[0] = 109775829;

     expected[1] = 1058376049;

     expected[2] = 396967909;

     expected[3] = 767626598;

     expected[4] = 647044790;

     expected[5] = 883659981;

     expected[6] = 872382809;

     expected[7] = 108725303;

     expected[8] = 644200513;

     expected[9] = 982910077;

     expected[10] = 922424630;

     expected[11] = 208149163;

     expected[12] = 301138404;

     expected[13] = 476482003;

     expected[14] = 83144032;

     expected[15] = 929213596;

     expected[16] = 362418745;

     expected[17] = 583481857;

     expected[18] = 824872607;

     expected[19] = 642952967;

     expected[20] = 279849210;

     expected[21] = 198875886;

     expected[22] = 1047936506;

     expected[23] = 655660673;

     expected[24] = 106180843;

     expected[25] = 483892027;

     expected[26] = 7940812;

     expected[27] = 778672521;

     expected[28] = 238660357;

     expected[29] = 669267839;

     expected[30] = 228664489;

     expected[31] = 317171710;

     expected[32] = 133641854;

     expected[33] = 908138223;

     expected[34] = 748033744;

     expected[35] = 483504989;

     expected[36] = 928626652;

     expected[37] = 376570234;

     expected[38] = 160513221;

     expected[39] = 768907721;

     expected[40] = 1019845858;

     expected[41] = 772411228;

     expected[42] = 270376507;

     expected[43] = 141865592;

     expected[44] = 1032240845;

     expected[45] = 244620575;

     expected[46] = 628107428;

     expected[47] = 343553527;

     expected[48] = 450932846;

     expected[49] = 798495470;

     expected[50] = 253597795;

     expected[51] = 945666426;

     expected[52] = 682408912;

     expected[53] = 459386673;

     expected[54] = 772407786;

     expected[55] = 607596427;

     expected[56] = 179277864;

     expected[57] = 852010542;

     expected[58] = 644321004;

     expected[59] = 999765783;

     expected[60] = 490083432;

     expected[61] = 51678372;

     expected[62] = 303992413;

     expected[63] = 312523142;

     expected[64] = 638332158;

     expected[65] = 282961102;

     expected[66] = 464306072;

     expected[67] = 247015324;

     expected[68] = 960562493;

     expected[69] = 695025287;

     expected[70] = 66987572;

     expected[71] = 685416888;

     expected[72] = 1033910678;

     expected[73] = 54527682;

     expected[74] = 208026889;

     expected[75] = 976325971;

     expected[76] = 587413997;

     expected[77] = 540354819;

     expected[78] = 962700175;

     expected[79] = 772613560;

     expected[80] = 622473979;

     expected[81] = 524297161;

     expected[82] = 389068472;

     expected[83] = 255038499;

     expected[84] = 37403014;

     expected[85] = 1060934762;

     expected[86] = 386468822;

     expected[87] = 94320604;

     expected[88] = 564144366;

     expected[89] = 551302813;

     expected[90] = 1071196555;

     expected[91] = 741540712;

     expected[92] = 312192415;

     expected[93] = 725324953;

     expected[94] = 262410278;

     expected[95] = 367214031;

     expected[96] = 339949807;

     expected[97] = 154543829;

     expected[98] = 994184555;

     expected[99] = 35661611;

     expected[100] = 966572144;

     expected[101] = 495752129;

     expected[102] = 972104965;

     expected[103] = 254124674;

     expected[104] = 961719139;

     expected[105] = 197835024;

     expected[106] = 554373154;

     expected[107] = 1034067835;

     expected[108] = 402209449;

     expected[109] = 465024519;

     expected[110] = 458963040;

     expected[111] = 303772335;

     expected[112] = 457621217;

     expected[113] = 899089814;

     expected[114] = 227283279;

     expected[115] = 1073133002;

     expected[116] = 702032496;

     expected[117] = 757108522;

     expected[118] = 205003704;

     expected[119] = 368019823;

     expected[120] = 991201686;

     expected[121] = 539297111;

     expected[122] = 85295150;

     expected[123] = 661297454;

     expected[124] = 638429013;

     expected[125] = 610114355;

     expected[126] = 30215838;

     expected[127] = 510243775;
     #endif
    // Turn the int32_t accs into split_s32 accs
    vect_s32_split_accs(&acc[0], expected, B.length);

    for(int k = 0; k < B.length; k++){
      expected_f[k] = ldexp(expected[k], acc_exp)
                    + ldexp(B.data[k], B.exp);
    }

    unsigned chunks = B.length >> VPU_INT16_EPV_LOG2;
    unsigned tail = B.length & (VPU_INT16_EPV - 1);
    for(int k = tail; k < VPU_INT16_EPV; k++){
      acc[chunks].vD[k] = 0;
      acc[chunks].vR[k] = 0;
    }

    headroom_t res_hr = bfp_s16_accumulate(acc, acc_exp, &B);

    vect_s32_merge_accs(actual, &acc[0], B.length);

    headroom_t exp_hr = vect_s32_headroom(actual, B.length);

    exp_hr = MIN(15, exp_hr);

    TEST_ASSERT_EQUAL(exp_hr, res_hr);

    test_s32_from_double(expected, expected_f, B.length, acc_exp);

    TEST_ASSERT_INT32_ARRAY_WITHIN(1, expected, actual, B.length);
  }
}

