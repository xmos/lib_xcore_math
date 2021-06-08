// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include <stdio.h>

#include "unity.h"

#define CALL(F)     do { void F(); F(); } while(0)

int main(int argc, char** argv)
{
    UNITY_BEGIN();

    CALL(test_xs3_filter_fir_s16);
    CALL(test_xs3_filter_fir_s32);
    CALL(test_xs3_push_sample_s16);
    CALL(test_xs3_filter_biquad_s32);

    return UNITY_END();
}
