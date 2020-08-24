

#include <stdio.h>

#include "unity.h"
#include "tst_common.h"
#include "test_cases.h"

FILE* perf_file = NULL;

int main(int argc, char** argv)
{

#if WRITE_PERFORMANCE_INFO
    perf_file = fopen(PERFORMANCE_INFO_FILENAME, "w");
    fprintf(perf_file, "Function, Input Size, Worst Observed Error, Worst Observed Timing, Misc\n");
#endif

    UNITY_BEGIN();

    test_xs3_fft_helpers();
    test_xs3_fft_mono_adjust();
    test_xs3_fft_dit();
    test_xs3_fft_dif();

    test_bfp_fft();

#if WRITE_PERFORMANCE_INFO
    fclose(perf_file);
#endif

    return UNITY_END();
}
