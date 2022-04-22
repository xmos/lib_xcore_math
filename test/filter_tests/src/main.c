// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdio.h>
#include <xscope.h>

#include "unity_fixture.h"

int main(int argc, const char* argv[])
{
    xscope_config_io(XSCOPE_IO_BASIC);
    
    UnityGetCommandLineOptions(argc, argv);
    UnityBegin(argv[0]);

    RUN_TEST_GROUP(xs3_filter_fir_s16);
    RUN_TEST_GROUP(xs3_filter_fir_s32);
    RUN_TEST_GROUP(xs3_push_sample);
    RUN_TEST_GROUP(xs3_filter_biquad_s32);

    return UNITY_END();
}
