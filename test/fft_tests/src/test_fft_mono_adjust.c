// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include "xmath/xmath.h"
#include "testing.h"
#include "floating_fft.h"
#include "tst_common.h"
#include "fft.h"
#include "unity_fixture.h"
#include "xmath_fft_lut.h"


TEST_GROUP_RUNNER(fft_mono_adjust) {
  RUN_TEST_CASE(fft_mono_adjust, fft_mono_adjust_forward);
  RUN_TEST_CASE(fft_mono_adjust, fft_mono_adjust_inverse);
}

TEST_GROUP(fft_mono_adjust);
TEST_SETUP(fft_mono_adjust) { fflush(stdout); }
TEST_TEAR_DOWN(fft_mono_adjust) {}


#define MAX_PROC_FRAME_LENGTH_LOG2 (MAX_DIT_FFT_LOG2)
#define MAX_PROC_FRAME_LENGTH (1<<MAX_PROC_FRAME_LENGTH_LOG2)


#if SMOKE_TEST
#  define LOOPS_LOG2       (2)
#else
#  define LOOPS_LOG2       (8)
#endif

#define TESTING_FUNC_NAME "fft_mono_adjust"

#define MIN_N_LOG2  (4)

TEST(fft_mono_adjust, fft_mono_adjust_forward)
{
#define FUNC_NAME "fft_mono_adjust_forward"

#if PRINT_FUNC_NAMES
    printf("\n%s..\n", FUNC_NAME);
#endif

    unsigned r = 436554;

    for(unsigned k = MIN_N_LOG2; k <= MAX_PROC_FRAME_LENGTH_LOG2; k++){
        const unsigned N = (1<<k);
        unsigned worst_error = 0;
        float worst_timing = 0.0f;

        double sine_tableA[(MAX_PROC_FRAME_LENGTH/2) + 1];
        double sine_tableB[(MAX_PROC_FRAME_LENGTH/2) + 1];

        flt_make_sine_table_double(sine_tableA, N/2);
        flt_make_sine_table_double(sine_tableB, N);
        
        for(unsigned t = 0; t < (1 << LOOPS_LOG2); t++){
            unsigned diff = 0;

            int32_t DWORD_ALIGNED a[MAX_PROC_FRAME_LENGTH];
            complex_double_t DWORD_ALIGNED ref1[MAX_PROC_FRAME_LENGTH];
            complex_double_t DWORD_ALIGNED ref2[MAX_PROC_FRAME_LENGTH];

            conv_error_e error = 0;
            exponent_t exponent = 0;
            headroom_t headroom;

            //astew: why does this require 3 bits of headroom..???
            rand_vect_s32(a, N, 3, &r);

            for(unsigned i = 0; i < N; i++){
                ref2[i].re = conv_s32_to_double(a[i], exponent, &error);
                ref2[i].im = 0;
            }

            for(unsigned int i = 0; i < N/2; i++){
                ref1[i].re = conv_s32_to_double(a[2*i+0], exponent, &error);
                ref1[i].im = conv_s32_to_double(a[2*i+1], exponent, &error);
            }
            TEST_ASSERT_CONVERSION(error);

            headroom = vect_s32_headroom(a, N);

            diff = abs_diff_vect_complex_s32((complex_s32_t*)a, exponent, (complex_double_t*) ref1, N/2, &error);
            TEST_ASSERT_CONVERSION(error);
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+2, diff, "Test init failed.");

            flt_bit_reverse_indexes_double((complex_double_t*) ref1, N/2);
            flt_fft_forward_double((complex_double_t*) ref1, N/2, sine_tableA);
            
            flt_bit_reverse_indexes_double(ref2, N);
            flt_fft_forward_double(ref2, N, sine_tableB);
            ref2[0].im = ref2[N/2].re;

            fft_index_bit_reversal((complex_s32_t*) a, N/2);
            fft_dit_forward((complex_s32_t*) a, N/2, &headroom, &exponent);

            diff = abs_diff_vect_complex_s32((complex_s32_t*)a, exponent, (complex_double_t*) ref1, N/2, &error);
            TEST_ASSERT_CONVERSION(error);
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+2, diff, "Test setup failed.");

            unsigned ts1 = getTimestamp();
            fft_mono_adjust((complex_s32_t*) a, N, 0);
            unsigned ts2 = getTimestamp();

            float timing = (float) ((ts2-ts1)/100.0);
            if(timing > worst_timing) worst_timing = timing;

            diff = abs_diff_vect_complex_s32((complex_s32_t*) a, exponent, ref2, N/2, &error);
            TEST_ASSERT_CONVERSION(error);

            if(diff > worst_error) { worst_error = diff; }
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+4, diff, "Output delta is too large");
        }
        
#if PRINT_ERRORS
        printf("    %s worst error (%u-point): %u\n", FUNC_NAME, N, worst_error);
#endif

#if TIME_FUNCS
        printf("    %s (%u-point): %f us\n", FUNC_NAME, N, worst_timing);
#endif

#if WRITE_PERFORMANCE_INFO
        fprintf(perf_file, "%s, %u, %u, %0.02f, (forward)\n", TESTING_FUNC_NAME, N, worst_error, worst_timing);
#endif

#undef FUNC_NAME
    }
}


TEST(fft_mono_adjust, fft_mono_adjust_inverse)
{
#define FUNC_NAME "fft_mono_adjust_inverse"

#if PRINT_FUNC_NAMES
    printf("\n%s..\n", FUNC_NAME);
#endif

    unsigned r = 656556;
    conv_error_e error = 0;

    for(unsigned k = MIN_N_LOG2; k <= MAX_PROC_FRAME_LENGTH_LOG2; k++){
        const unsigned N = (1<<k);
        unsigned worst_error = 0;
        float worst_timing = 0.0f;

        double sine_tableA[(MAX_PROC_FRAME_LENGTH/2) + 1];

        flt_make_sine_table_double(sine_tableA, N/2);
        
        for(unsigned t = 0; t < (1 << LOOPS_LOG2); t++){
            unsigned diff = 0;
            complex_s32_t DWORD_ALIGNED a[MAX_PROC_FRAME_LENGTH/2];
            complex_double_t DWORD_ALIGNED ref[MAX_PROC_FRAME_LENGTH/2];

            exponent_t exponent = 0;

            rand_vect_complex_s32(a, N/2, 1, &r);
            conv_vect_complex_s32_to_complex_double(ref, a, N/2, exponent, &error);
            TEST_ASSERT_CONVERSION(error);

            diff = abs_diff_vect_complex_s32(a, exponent, (complex_double_t*) ref, N/2, &error);
            TEST_ASSERT_CONVERSION(error);
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+2, diff, "Test setup failed.");


            fft_mono_adjust(a, N, 0);

            unsigned ts1 = getTimestamp();
            fft_mono_adjust(a, N, 1);
            unsigned ts2 = getTimestamp();

            float timing = (float) ((ts2-ts1)/100.0);
            if(timing > worst_timing) worst_timing = timing;
            
            diff = abs_diff_vect_complex_s32(a, exponent, ref, N/2, &error);
            TEST_ASSERT_CONVERSION(error);
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+2, diff, "Output delta is too large.");

            if(diff > worst_error) { worst_error = diff; }
        }
        
#if PRINT_ERRORS
        printf("    %s worst error (%u-point): %u\n", FUNC_NAME, N, worst_error);
#endif

#if TIME_FUNCS
        printf("    %s (%u-point): %f us\n", FUNC_NAME, N, worst_timing);
#endif

#if WRITE_PERFORMANCE_INFO
        fprintf(perf_file, "%s, %u, %u, %0.02f, (inverse)\n", TESTING_FUNC_NAME, N, worst_error, worst_timing);
#endif

#undef FUNC_NAME
    }
}


