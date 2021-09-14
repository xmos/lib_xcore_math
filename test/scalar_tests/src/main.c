// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include <stdio.h>

#include "unity_fixture.h"

int main(int argc, const char* argv[])
{
    UnityGetCommandLineOptions(argc, argv);
    UnityBegin(argv[0]);

    // macro
    RUN_TEST_GROUP(CLS_macro);
    RUN_TEST_GROUP(HR_macro);

    // scalar utils
    RUN_TEST_GROUP(xs3_vpu_scalar_ops_s8);
    RUN_TEST_GROUP(xs3_vpu_scalar_ops_s16);
    RUN_TEST_GROUP(xs3_vpu_scalar_ops_s32);
    RUN_TEST_GROUP(xs3_s32_sqrt);
    RUN_TEST_GROUP(xs3_sXX_inverse);
    RUN_TEST_GROUP(xs3_sXX_mul);

    // scalar ops
    RUN_TEST_GROUP(float_convert);

    RUN_TEST_GROUP(float_sXX_add);
    RUN_TEST_GROUP(float_sXX_sub);
    RUN_TEST_GROUP(float_sXX_mul);
    RUN_TEST_GROUP(float_sXX_abs);
    RUN_TEST_GROUP(float_sXX_gt);
    RUN_TEST_GROUP(float_sXX_div);
    RUN_TEST_GROUP(float_sXX_ema);
    
    RUN_TEST_GROUP(float_sXX_sqrt);
    

    return UNITY_END();
}
