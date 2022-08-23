// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include <stdio.h>

#ifdef __XS3A__
# include <xscope.h>
#endif

#include "unity_fixture.h"

int main(int argc, const char* argv[])
{
#ifdef __XS3A__
  xscope_config_io(XSCOPE_IO_BASIC);
#endif
    
    UnityGetCommandLineOptions(argc, argv);
    UnityBegin(argv[0]);

    // macro
    RUN_TEST_GROUP(CLS_macro);
    RUN_TEST_GROUP(HR_macro);

    // scalar utils
    RUN_TEST_GROUP(s32_sqrt);
    RUN_TEST_GROUP(sXX_inverse);
    RUN_TEST_GROUP(sXX_mul);
    RUN_TEST_GROUP(q30_powers);

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

    RUN_TEST_GROUP(float_trig);
    RUN_TEST_GROUP(fixed_trig);
    RUN_TEST_GROUP(float_log);
    RUN_TEST_GROUP(float_exp);
    RUN_TEST_GROUP(float_logistic);
    

    return UNITY_END();
}
