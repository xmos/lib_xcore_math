// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include "xs3_math.h"
#include "testing.h"
#include "floating_fft.h"
#include "tst_common.h"
#include "fft.h"
#include "unity_fixture.h"
#include "../src/vect/xs3_fft_lut.h"


TEST_GROUP_RUNNER(xs3_fft_dif) {
  RUN_TEST_CASE(xs3_fft_dif, xs3_fft_dif_forward);
  RUN_TEST_CASE(xs3_fft_dif, xs3_fft_dif_inverse);
  RUN_TEST_CASE(xs3_fft_dif, xs3_fft_dif_forward_complete);
  RUN_TEST_CASE(xs3_fft_dif, xs3_fft_dif_inverse_complete);
}

TEST_GROUP(xs3_fft_dif);
TEST_SETUP(xs3_fft_dif) {}
TEST_TEAR_DOWN(xs3_fft_dif) {}


#define MAX_PROC_FRAME_LENGTH_LOG2 (MAX_DIF_FFT_LOG2)
#define MAX_PROC_FRAME_LENGTH (1<<MAX_PROC_FRAME_LENGTH_LOG2)


#define EXPONENT_SIZE 5
#define BASIC_HEADROOM 2
#define EXTRA_HEADROOM_MAX 3
#define WIGGLE 20

#define MIN_FFT_N_LOG2  (2)

#define LOOPS_LOG2 8


TEST(xs3_fft_dif, xs3_fft_dif_forward_complete)
{
#define FUNC_NAME "xs3_fft_dif_forward_complete"

#if PRINT_FUNC_NAMES
    printf("\n%s..\n", FUNC_NAME);
#endif

    unsigned r = 0x6999B20C;
    conv_error_e error = 0;

    for(unsigned k = MIN_FFT_N_LOG2; k <= MAX_PROC_FRAME_LENGTH_LOG2; k++){
        const unsigned FFT_N = (1<<k);
        unsigned worst_case = 0;

        double sine_table[(MAX_PROC_FRAME_LENGTH/2) + 1];

        flt_make_sine_table_double(sine_table, FFT_N);
        
        for(unsigned t = 0; t < (1 << LOOPS_LOG2); t++){
            const unsigned seed = r;

            complex_s32_t DWORD_ALIGNED a[MAX_PROC_FRAME_LENGTH];
            complex_double_t DWORD_ALIGNED A[MAX_PROC_FRAME_LENGTH];
            double real[MAX_PROC_FRAME_LENGTH], imag[MAX_PROC_FRAME_LENGTH];

            exponent_t exponent = sext(pseudo_rand_int32(&r), EXPONENT_SIZE);
            right_shift_t extra_hr = BASIC_HEADROOM + (pseudo_rand_uint32(&r) % (EXTRA_HEADROOM_MAX+1));
            
            rand_vect_complex_s32(a, FFT_N, extra_hr, &r);
            conv_vect_complex_s32_to_complex_double(A, a, FFT_N, exponent, &error);
            conv_vect_complex_s32_to_complex_double_v2(real, imag, a, FFT_N, exponent, &error);
            TEST_ASSERT_CONVERSION(error);

            headroom_t headroom = xs3_vect_complex_s32_headroom(a, FFT_N);

            Fft_transform(real, imag, FFT_N);

            flt_bit_reverse_indexes_double(A, FFT_N);
            flt_fft_forward_double(A, FFT_N, sine_table);

            xs3_fft_dif_forward(a, FFT_N, &headroom, &exponent);
            xs3_fft_index_bit_reversal(a, FFT_N);

            unsigned diff = 0;

            diff = abs_diff_vect_complex_s32(a, exponent, A, FFT_N, &error);
            TEST_ASSERT_CONVERSION(error);

            if(diff > worst_case) { worst_case = diff;  }
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+WIGGLE, diff, "Output delta is too large");

            if(diff > worst_case) { worst_case = diff;  }
            diff = abs_diff_vect_complex_s32(a, exponent, A, FFT_N, &error);
            TEST_ASSERT_CONVERSION(error);

            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+WIGGLE, diff, "Output delta is too large");

            TEST_ASSERT_EQUAL_MESSAGE(xs3_vect_complex_s32_headroom(a, FFT_N), headroom, "Reported headroom was incorrect.");
        }

#if PRINT_ERRORS
        printf("    %s worst error (%u-point): %u\n", FUNC_NAME, FFT_N, worst_case);
#endif

#if WRITE_PERFORMANCE_INFO
        fprintf(perf_file, "%s, %u, %u, -,\n", "xs3_fft_dif_forward", FFT_N, worst_case);
#endif

#undef FUNC_NAME
    }
}


TEST(xs3_fft_dif, xs3_fft_dif_inverse_complete)
{
#define FUNC_NAME "xs3_fft_dif_inverse_complete"

#if PRINT_FUNC_NAMES
    printf("\n%s..\n", FUNC_NAME);
#endif

    unsigned r = 1;
    conv_error_e error = 0;

    for(unsigned k = MIN_FFT_N_LOG2; k <= MAX_PROC_FRAME_LENGTH_LOG2; k++){
        const unsigned FFT_N = (1<<k);
        unsigned worst_case = 0;

        double sine_table[(MAX_PROC_FRAME_LENGTH/2) + 1];

        flt_make_sine_table_double(sine_table, FFT_N);

        for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
            const unsigned seed = r;

            complex_s32_t DWORD_ALIGNED a[MAX_PROC_FRAME_LENGTH];
            complex_double_t DWORD_ALIGNED A[MAX_PROC_FRAME_LENGTH];
            double real[MAX_PROC_FRAME_LENGTH], imag[MAX_PROC_FRAME_LENGTH];

            exponent_t exponent = sext(pseudo_rand_int32(&r), EXPONENT_SIZE);
            right_shift_t extra_hr = BASIC_HEADROOM + (pseudo_rand_uint32(&r) % (EXTRA_HEADROOM_MAX+1));
            
            rand_vect_complex_s32(a, FFT_N, extra_hr, &r);
            conv_vect_complex_s32_to_complex_double(A, a, FFT_N, exponent, &error);
            conv_vect_complex_s32_to_complex_double_v2(real, imag, a, FFT_N, exponent, &error);
            TEST_ASSERT_CONVERSION(error);
            
            headroom_t headroom = xs3_vect_complex_s32_headroom(a, FFT_N);

            flt_bit_reverse_indexes_double(A, FFT_N);
            flt_fft_inverse_double(A, FFT_N, sine_table);

            xs3_fft_dif_inverse(a, FFT_N, &headroom, &exponent);
            xs3_fft_index_bit_reversal(a, FFT_N);

            Fft_inverseTransform(real, imag, FFT_N);

            unsigned diff = 0;

            diff = abs_diff_vect_complex_s32(a, exponent, A, FFT_N, &error);
            TEST_ASSERT_CONVERSION(error);
            
            if(diff > worst_case) { worst_case = diff;  }
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+WIGGLE, diff, "Output delta is too large");
            TEST_ASSERT_CONVERSION(error);

            for(unsigned i = 0; i < FFT_N; i++){
                A[i].re = real[i];
                A[i].im = imag[i];
            }

            diff = abs_diff_vect_complex_s32(a, exponent, A, FFT_N, &error);
            TEST_ASSERT_CONVERSION(error);

            if(diff > worst_case) { worst_case = diff;  }
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+WIGGLE, diff, "Output delta is too large");

            TEST_ASSERT_EQUAL_MESSAGE(xs3_vect_complex_s32_headroom(a, FFT_N), headroom, "Reported headroom was incorrect.");
        }


#if PRINT_ERRORS
        printf("    %s worst error (%u-point): %u\n", FUNC_NAME, FFT_N, worst_case);
#endif

#if WRITE_PERFORMANCE_INFO
        fprintf(perf_file, "%s, %u, %u, -,\n", "xs3_fft_dif_inverse", FFT_N, worst_case);
#endif

#undef FUNC_NAME
    }
}


TEST(xs3_fft_dif, xs3_fft_dif_forward)
{
#define FUNC_NAME "xs3_fft_dif_forward"

#if PRINT_FUNC_NAMES
    printf("\n%s..\n", FUNC_NAME);
#endif

    unsigned r = 1;
    conv_error_e error = 0;

    for(unsigned k = MIN_FFT_N_LOG2; k <= MAX_PROC_FRAME_LENGTH_LOG2; k++){

        unsigned FFT_N = (1<<k);
        float worst_timing = 0.0f;
        double sine_table[(MAX_PROC_FRAME_LENGTH/2) + 1];

        flt_make_sine_table_double(sine_table, FFT_N);
        
        for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){

            complex_s32_t DWORD_ALIGNED a[MAX_PROC_FRAME_LENGTH];
            complex_double_t DWORD_ALIGNED A[MAX_PROC_FRAME_LENGTH];

            exponent_t exponent = sext(pseudo_rand_int32(&r), EXPONENT_SIZE);
            right_shift_t extra_hr = BASIC_HEADROOM + (pseudo_rand_uint32(&r) % (EXTRA_HEADROOM_MAX+1));
            
            rand_vect_complex_s32(a, FFT_N, extra_hr, &r);
            conv_vect_complex_s32_to_complex_double(A, a, FFT_N, exponent, &error);
            TEST_ASSERT_CONVERSION(error);
            
            headroom_t headroom = xs3_vect_complex_s32_headroom(a, FFT_N);

            flt_bit_reverse_indexes_double(A, FFT_N);
            flt_fft_forward_double(A, FFT_N, sine_table);

            unsigned ts1 = getTimestamp();
            xs3_fft_dif_forward(a, FFT_N, &headroom, &exponent);
            unsigned ts2 = getTimestamp();
            xs3_fft_index_bit_reversal(a, FFT_N);

            float timing = (ts2-ts1)/100.0;
            if(timing > worst_timing) worst_timing = timing;

            unsigned diff = abs_diff_vect_complex_s32(a, exponent, A, FFT_N, &error);
            TEST_ASSERT_CONVERSION(error);
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+WIGGLE, diff, "Output delta is too large");

            TEST_ASSERT_EQUAL_MESSAGE(xs3_vect_complex_s32_headroom(a, FFT_N), headroom, "Reported headroom was incorrect.");
        }

#if TIME_FUNCS
        printf("    %s (%u-point): %f us\n", FUNC_NAME, FFT_N, worst_timing);
#endif

#if WRITE_PERFORMANCE_INFO
        fprintf(perf_file, "%s, %u,, %0.02f,\n", &(__func__[5]), FFT_N, worst_timing);
#endif

#undef FUNC_NAME
    }
}


TEST(xs3_fft_dif, xs3_fft_dif_inverse)
{
#define FUNC_NAME "xs3_fft_dif_inverse"

#if PRINT_FUNC_NAMES
    printf("\n%s..\n", FUNC_NAME);
#endif

    unsigned r = 1;
    conv_error_e error = 0;

    for(unsigned k = MIN_FFT_N_LOG2; k <= MAX_PROC_FRAME_LENGTH_LOG2; k++){

        unsigned FFT_N = (1<<k);
        float worst_timing = 0.0f;
        double sine_table[(MAX_PROC_FRAME_LENGTH/2) + 1];

        flt_make_sine_table_double(sine_table, FFT_N);
        
        for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
            
            complex_s32_t DWORD_ALIGNED a[MAX_PROC_FRAME_LENGTH];
            complex_double_t DWORD_ALIGNED A[MAX_PROC_FRAME_LENGTH];

            exponent_t exponent = sext(pseudo_rand_int32(&r), EXPONENT_SIZE);
            right_shift_t extra_hr = BASIC_HEADROOM + (pseudo_rand_uint32(&r) % (EXTRA_HEADROOM_MAX+1));
            
            rand_vect_complex_s32(a, FFT_N, extra_hr, &r);
            conv_vect_complex_s32_to_complex_double(A, a, FFT_N, exponent, &error);
            TEST_ASSERT_CONVERSION(error);
            
            headroom_t headroom = xs3_vect_complex_s32_headroom(a, FFT_N);
            
            flt_bit_reverse_indexes_double(A, FFT_N);
            flt_fft_inverse_double(A, FFT_N, sine_table);

            unsigned ts1 = getTimestamp();
            xs3_fft_dif_inverse(a, FFT_N, &headroom, &exponent);
            unsigned ts2 = getTimestamp();
            xs3_fft_index_bit_reversal(a, FFT_N);

            float timing = (ts2-ts1)/100.0;
            if(timing > worst_timing) worst_timing = timing;

            unsigned diff = abs_diff_vect_complex_s32(a, exponent, A, FFT_N, &error);
            TEST_ASSERT_CONVERSION(error);
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+WIGGLE, diff, "Output delta is too large");

            TEST_ASSERT_EQUAL_MESSAGE(xs3_vect_complex_s32_headroom(a, FFT_N), headroom, "Reported headroom was incorrect.");
        }
#if TIME_FUNCS
        printf("    %s (%u-point): %f us\n", FUNC_NAME, FFT_N, worst_timing);
#endif

#if WRITE_PERFORMANCE_INFO
        fprintf(perf_file, "%s, %u,, %0.02f,\n", &(__func__[5]), FFT_N, worst_timing);
#endif
    }

#undef FUNC_NAME
}
