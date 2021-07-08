// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include <stdio.h>

#include "unity.h"
#include "tst_common.h"

#define CALL(F)     do { void F(); F(); } while(0)

FILE* perf_file = NULL;

int main(int argc, char** argv)
{

#if WRITE_PERFORMANCE_INFO
    printf("Writing performance info to: %s\n", PERFORMANCE_INFO_FILENAME);
    perf_file = fopen(PERFORMANCE_INFO_FILENAME, "w");
    fprintf(perf_file, "Function, Input Size, Worst Observed Error, Worst Observed Timing, Misc\n");
#endif

    UNITY_BEGIN();

    CALL(test_xs3_fft_helpers);
    CALL(test_xs3_fft_mono_adjust);
    CALL(test_xs3_fft_dit);
    CALL(test_xs3_fft_dif);

    CALL(test_bfp_fft);
    CALL(test_bfp_fft_pack_unpack);

#if WRITE_PERFORMANCE_INFO
    fclose(perf_file);
#endif

    return UNITY_END();
}
