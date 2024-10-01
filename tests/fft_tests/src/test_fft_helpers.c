// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include "xmath/xmath.h"
#include "testing.h"
#include "floating_fft.h"
#include "tst_common.h"
#include "fft.h"
#include "unity_fixture.h"
#include "xmath_fft_lut.h"

#include <string.h>

TEST_GROUP_RUNNER(fft_helpers) {
  RUN_TEST_CASE(fft_helpers, fft_index_bit_reversal);
  RUN_TEST_CASE(fft_helpers, vect_complex_s32_tail_reverse);
  RUN_TEST_CASE(fft_helpers, fft_spectra_split);
  RUN_TEST_CASE(fft_helpers, fft_spectra_merge);
}

TEST_GROUP(fft_helpers);
TEST_SETUP(fft_helpers) { fflush(stdout); }
TEST_TEAR_DOWN(fft_helpers) {}


#define MAX_PROC_FRAME_LENGTH_LOG2 (MAX_DIT_FFT_LOG2)
#define MAX_PROC_FRAME_LENGTH (1<<MAX_PROC_FRAME_LENGTH_LOG2)


#if SMOKE_TEST
#  define LOOPS_LOG2       (2)
#else
#  define LOOPS_LOG2       (8)
#endif

#define MIN_N_LOG2  (2)


TEST(fft_helpers, fft_index_bit_reversal)
{
#define FUNC_NAME "fft_index_bit_reversal"

#if PRINT_FUNC_NAMES
    printf("\n%s..\n", FUNC_NAME);
#endif

    unsigned r = 1;

    for(unsigned k = MIN_N_LOG2; k <= MAX_PROC_FRAME_LENGTH_LOG2; k++){

        unsigned N = (1<<k);
        float worst_timing = 0.0f;
        unsigned worst_error = 0;
        
        for(unsigned t = 0; t <= (1<<LOOPS_LOG2); t++){

            complex_s32_t DWORD_ALIGNED a[MAX_PROC_FRAME_LENGTH];
            complex_s32_t a_copy[MAX_PROC_FRAME_LENGTH];

            rand_vect_complex_s32(a, N, 0, &r);

            memcpy(a_copy, a, sizeof(a));

            unsigned ts1 = getTimestamp();
            fft_index_bit_reversal(a, N);
            unsigned ts2 = getTimestamp();

            float timing = (float) ((ts2-ts1)/100.0);
            if(timing > worst_timing) worst_timing = timing;

            for(unsigned int i = 0; i < N; i++){
              int dex = n_bitrev(i, k);
              TEST_ASSERT_EQUAL_INT32(a_copy[dex].re, a[i].re);
              TEST_ASSERT_EQUAL_INT32(a_copy[dex].re, a[i].re);
            }
        }

#if TIME_FUNCS
        printf("    %s (%u-point): %f us\n", FUNC_NAME, N, worst_timing);
#endif

#if WRITE_PERFORMANCE_INFO
        fprintf(perf_file, "%s, %u, %u, %0.02f,\n", &(__func__[5]), N, worst_error, worst_timing);
#endif
    }

#undef FUNC_NAME
}


TEST(fft_helpers, vect_complex_s32_tail_reverse)
{
#define FUNC_NAME "vect_complex_s32_tail_reverse"

#if PRINT_FUNC_NAMES
    printf("\n%s..\n", FUNC_NAME);
#endif

    unsigned r = 1;

    for(unsigned k = MAX(MIN_N_LOG2, 3); k <= MAX_PROC_FRAME_LENGTH_LOG2; k++){

        unsigned N = (1<<k);
        float worst_timing = 0.0f;
        
        for(unsigned t = 0; t <= (1<<LOOPS_LOG2); t++){

            complex_s32_t DWORD_ALIGNED a[MAX_PROC_FRAME_LENGTH];
            complex_s32_t DWORD_ALIGNED ref[MAX_PROC_FRAME_LENGTH];

            rand_vect_complex_s32(a, N, 0, &r);
            
            for(unsigned i = 0 ; i < N; i++)
                ref[(N-i) % N] = a[i];

            unsigned ts1 = getTimestamp();
            vect_complex_s32_tail_reverse(a, N);
            unsigned ts2 = getTimestamp();

            float timing = (float) ((ts2-ts1)/100.0);
            if(timing > worst_timing) worst_timing = timing;

            TEST_ASSERT_EQUAL_INT32_ARRAY((int32_t*) ref, (int32_t*) a, 2*N);
        }

#if TIME_FUNCS
        printf("    %s (N=%u): %f us\n", FUNC_NAME, N, worst_timing);
#endif

#if WRITE_PERFORMANCE_INFO
        fprintf(perf_file, "%s, %u, 0, %0.02f,\n", &(__func__[5]), N, worst_timing);
#endif
    }

#undef FUNC_NAME
}


#undef LOOPS_LOG2


#if SMOKE_TEST
#  define LOOPS_LOG2       (0)
#else
#  define LOOPS_LOG2       (8)
#endif

TEST(fft_helpers, fft_spectra_split)
{
#define FUNC_NAME "fft_spectra_split"

#if PRINT_FUNC_NAMES
    printf("\n%s..\n", FUNC_NAME);
#endif


    unsigned r = 1;
    conv_error_e error = 0;

    for(unsigned k = MIN_N_LOG2; k <= MAX_PROC_FRAME_LENGTH_LOG2; k++){
        
        const unsigned N = 1<<k;
        float worst_timing = 0.0f;
        unsigned worst_error = 0;

        for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
            

            complex_s32_t DWORD_ALIGNED a[MAX_PROC_FRAME_LENGTH];
            complex_double_t DWORD_ALIGNED A[MAX_PROC_FRAME_LENGTH];

            rand_vect_complex_s32(a, N, 1, &r);
            conv_vect_complex_s32_to_complex_double(A, a, N, 0, &error);
            TEST_ASSERT_CONVERSION(error);

            flt_fft_split_spectrum_double(A, N);

            unsigned ts1 = getTimestamp();
            headroom_t got_hr = fft_spectra_split(a, N);
            unsigned ts2 = getTimestamp();

            float timing = (float) ((ts2-ts1)/100.0);
            if(timing > worst_timing) worst_timing = timing;

            unsigned diff = abs_diff_vect_complex_s32(a, 0, A, N, &error);
            TEST_ASSERT_CONVERSION(error);
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(1, diff, "Output delta is too large");

            headroom_t exp_hr = vect_s32_headroom((int32_t*)a,2*N);
            TEST_ASSERT_EQUAL_MESSAGE(exp_hr, got_hr, "Headroom error");

            if(diff > worst_error) worst_error = diff;
        }
        
#if PRINT_ERRORS
        printf("    %s worst error (%u-point): %u\n", FUNC_NAME, N, worst_case);
#endif

#if TIME_FUNCS
        printf("    %s (%u-point): %f us\n", FUNC_NAME, N, worst_timing);
#endif

#if WRITE_PERFORMANCE_INFO
        fprintf(perf_file, "%s, %u, %u, %0.02f,\n", &(__func__[5]), N, worst_error, worst_timing);
#endif
    }

#undef FUNC_NAME
}


TEST(fft_helpers, fft_spectra_merge)
{
#define FUNC_NAME "fft_spectra_merge"

#if PRINT_FUNC_NAMES
    printf("\n%s..\n", FUNC_NAME);
#endif


    unsigned r = 1;
    conv_error_e error = 0;

    for(unsigned k = MIN_N_LOG2; k <= MAX_PROC_FRAME_LENGTH_LOG2; k++){
        
        unsigned N = (1<<k);
        float worst_timing = 0.0f;
        unsigned worst_error = 0;

        for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
            
            complex_s32_t DWORD_ALIGNED a[MAX_PROC_FRAME_LENGTH];
            complex_double_t DWORD_ALIGNED A[MAX_PROC_FRAME_LENGTH];

            rand_vect_complex_s32(a, N, 1, &r);
            conv_vect_complex_s32_to_complex_double(A, a, N, 0, &error);
            TEST_ASSERT_CONVERSION(error);

            flt_fft_merge_spectra_double(A, N);

            unsigned ts1 = getTimestamp();
            headroom_t got_hr = fft_spectra_merge(a, N);
            unsigned ts2 = getTimestamp();

            float timing = (float) ((ts2-ts1)/100.0);
            if(timing > worst_timing) worst_timing = timing;

            unsigned diff = abs_diff_vect_complex_s32(a, 0, A, N, &error);
            TEST_ASSERT_CONVERSION(error);
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k, diff, "Output delta is too large");

            headroom_t exp_hr = vect_s32_headroom((int32_t*)a,2*N);
            TEST_ASSERT_EQUAL_MESSAGE(exp_hr, got_hr, "Headroom error");

            if(diff > worst_error) worst_error = diff;
        }
        
#if PRINT_ERRORS
        printf("    %s worst error (%u-point): %u\n", FUNC_NAME, N, worst_case);
#endif

#if TIME_FUNCS
        printf("    %s (%u-point): %f us\n", FUNC_NAME, N, worst_timing);
#endif

#if WRITE_PERFORMANCE_INFO
        fprintf(perf_file, "%s, %u, %u, %0.02f,\n", &(__func__[5]), N, worst_error, worst_timing);
#endif
    }

#undef FUNC_NAME
}
