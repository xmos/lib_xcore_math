// Copyright 2020 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public License: Version 1.


#include <xs1.h>
#include <xclib.h>

#include "xs3_math.h"
#include "testing.h"
#include "floating_fft.h"
#include "tst_common.h"
#include "fft.h"
#include "unity.h"
#include "../src/vect/xs3_fft_lut.h"


#define MAX_PROC_FRAME_LENGTH_LOG2 (MAX_DIT_FFT_LOG2)
#define MAX_PROC_FRAME_LENGTH (1<<MAX_PROC_FRAME_LENGTH_LOG2)

#define LOOPS_LOG2  (8)

#define MIN_N_LOG2  (2)


void test_xs3_fft_index_bit_reversal()
{
#if PRINT_FUNC_NAMES
    printf("%s..\n", __func__);
#endif

    unsigned r = 1;
    conv_error_e error = 0;

    for(unsigned k = MIN_N_LOG2; k <= MAX_PROC_FRAME_LENGTH_LOG2; k++){

        unsigned N = (1<<k);
        float worst_timing = 0.0f;
        unsigned worst_error = 0;
        
        for(unsigned t = 0; t <= (1<<LOOPS_LOG2); t++){

            complex_s32_t DWORD_ALIGNED a[MAX_PROC_FRAME_LENGTH];
            complex_double_t DWORD_ALIGNED A[MAX_PROC_FRAME_LENGTH];

            conv_error_e error = 0;
            exponent_t exponent = sext(pseudo_rand_int32(&r), 5);

            rand_vect_complex_s32(a, N, 0, &r);
            conv_vect_complex_s32_to_complex_double(A, a, N, exponent, &error);
            TEST_ASSERT_CONVERSION(error);

            flt_bit_reverse_indexes_double(A, N);

            unsigned ts1 = getTimestamp();
            xs3_fft_index_bit_reversal(a, N);
            unsigned ts2 = getTimestamp();

            float timing = (ts2-ts1)/100.0;
            if(timing > worst_timing) worst_timing = timing;

            unsigned diff = abs_diff_vect_complex_s32(a, exponent, A, N, &error);
            TEST_ASSERT_CONVERSION(error);
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k, diff, "Output delta is too large");

            if(diff > worst_error) worst_error = diff;
        }
        
#if PRINT_ERRORS
        printf("    %s worst error (%u-point): %u\n", __func__, N, worst_error);
#endif

#if TIME_FUNCS
        printf("    %s (%u-point): %f us\n", __func__, N, worst_timing);
#endif

#if WRITE_PERFORMANCE_INFO
        fprintf(perf_file, "%s, %u, %u, %0.02f,\n", &(__func__[5]), N, worst_error, worst_timing);
#endif
    }
}


void test_xs3_vect_complex_s32_tail_reverse()
{
#if PRINT_FUNC_NAMES
    printf("%s..\n", __func__);
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
            xs3_vect_complex_s32_tail_reverse(a, N);
            unsigned ts2 = getTimestamp();

            float timing = (ts2-ts1)/100.0;
            if(timing > worst_timing) worst_timing = timing;

            TEST_ASSERT_EQUAL_INT32_ARRAY((int32_t*) ref, (int32_t*) a, 2*N);
        }

#if TIME_FUNCS
        printf("    %s (N=%u): %f us\n", __func__, N, worst_timing);
#endif

#if WRITE_PERFORMANCE_INFO
        fprintf(perf_file, "%s, %u, 0, %0.02f,\n", &(__func__[5]), N, worst_timing);
#endif
    }
}



void test_xs3_fft_spectra_split()
{
#if PRINT_FUNC_NAMES
    printf("%s..\n", __func__);
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
            headroom_t got_hr = xs3_fft_spectra_split(a, N);
            unsigned ts2 = getTimestamp();

            float timing = (ts2-ts1)/100.0;
            if(timing > worst_timing) worst_timing = timing;

            unsigned diff = abs_diff_vect_complex_s32(a, 0, A, N, &error);
            TEST_ASSERT_CONVERSION(error);
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(1, diff, "Output delta is too large");

            headroom_t exp_hr = xs3_vect_s32_headroom((int32_t*)a,2*N);
            TEST_ASSERT_EQUAL_MESSAGE(exp_hr, got_hr, "Headroom error");

            if(diff > worst_error) worst_error = diff;
        }
        
#if PRINT_ERRORS
        printf("    %s worst error (%u-point): %u\n", __func__, N, worst_case);
#endif

#if TIME_FUNCS
        printf("    %s (%u-point): %f us\n", __func__, N, worst_timing);
#endif

#if WRITE_PERFORMANCE_INFO
        fprintf(perf_file, "%s, %u, %u, %0.02f,\n", &(__func__[5]), N, worst_error, worst_timing);
#endif
    }
}






void test_xs3_fft_spectra_merge()
{
#if PRINT_FUNC_NAMES
    printf("%s..\n", __func__);
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
            headroom_t got_hr = xs3_fft_spectra_merge(a, N);
            unsigned ts2 = getTimestamp();

            float timing = (ts2-ts1)/100.0;
            if(timing > worst_timing) worst_timing = timing;

            unsigned diff = abs_diff_vect_complex_s32(a, 0, A, N, &error);
            TEST_ASSERT_CONVERSION(error);
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k, diff, "Output delta is too large");

            headroom_t exp_hr = xs3_vect_s32_headroom((int32_t*)a,2*N);
            TEST_ASSERT_EQUAL_MESSAGE(exp_hr, got_hr, "Headroom error");

            if(diff > worst_error) worst_error = diff;
        }
        
#if PRINT_ERRORS
        printf("    %s worst error (%u-point): %u\n", __func__, N, worst_case);
#endif

#if TIME_FUNCS
        printf("    %s (%u-point): %f us\n", __func__, N, worst_timing);
#endif

#if WRITE_PERFORMANCE_INFO
        fprintf(perf_file, "%s, %u, %u, %0.02f,\n", &(__func__[5]), N, worst_error, worst_timing);
#endif
    }
}




void test_xs3_fft_helpers()
{
    SET_TEST_FILE();

    RUN_TEST(test_xs3_fft_index_bit_reversal);
    RUN_TEST(test_xs3_vect_complex_s32_tail_reverse);
    RUN_TEST(test_xs3_fft_spectra_split);
    RUN_TEST(test_xs3_fft_spectra_merge);
}
