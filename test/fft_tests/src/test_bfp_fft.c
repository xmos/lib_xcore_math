// Copyright 2021 XMOS LIMITED. This Software is subject to the terms of the 
// XMOS Public License: Version 1


#include <xs1.h>
#include <xclib.h>

#include "bfp_math.h"
#include "testing.h"
#include "floating_fft.h"
#include "tst_common.h"
#include "fft.h"
#include "unity.h"


#define MAX_PROC_FRAME_LENGTH_LOG2 10
#define MAX_PROC_FRAME_LENGTH (1<<MAX_PROC_FRAME_LENGTH_LOG2)

#define EXPONENT_SIZE 5
#define MAX_HEADROOM 5
#define WIGGLE 20

#define MIN_FFT_N_LOG2  (2)

#define LOOPS_LOG2  (8)


void test_bfp_fft_forward_complex()
{
#if PRINT_FUNC_NAMES
    printf("%s..\n", __func__);
#endif

    unsigned r = 0x0429BBDF;

    for(unsigned k = MIN_FFT_N_LOG2; k <= MAX_PROC_FRAME_LENGTH_LOG2; k++){
        unsigned FFT_N = (1<<k);
        unsigned worst_error = 0;
        float worst_timing = 0.0f;

        double sine_table[(MAX_PROC_FRAME_LENGTH/2) + 1];

        flt_make_sine_table_double(sine_table, FFT_N);
        
        for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){ 

            complex_s32_t DWORD_ALIGNED a[MAX_PROC_FRAME_LENGTH];

            bfp_complex_s32_t A;

            complex_double_t DWORD_ALIGNED ref[MAX_PROC_FRAME_LENGTH];

            conv_error_e error = 0;
            const exponent_t initial_exponent = sext(pseudo_rand_int32(&r), EXPONENT_SIZE);
            right_shift_t shr = pseudo_rand_uint32(&r) % MAX_HEADROOM;

            for(unsigned i = 0; i < FFT_N; i++){
                a[i].re = pseudo_rand_int32(&r) >> (shr);
                a[i].im = pseudo_rand_int32(&r) >> (shr);
                ref[i].re = conv_s32_to_double(a[i].re, initial_exponent, &error);
                ref[i].im = conv_s32_to_double(a[i].im, initial_exponent, &error);
            }
            TEST_ASSERT_CONVERSION(error);

            bfp_complex_s32_init(&A, a, initial_exponent, FFT_N, 1);

            flt_bit_reverse_indexes_double(ref, FFT_N);
            flt_fft_forward_double(ref, FFT_N, sine_table);

            unsigned ts1 = getTimestamp();
            bfp_fft_forward_complex(&A);
            unsigned ts2 = getTimestamp();

            float timing = (ts2-ts1)/100.0;
            if(timing > worst_timing) worst_timing = timing;

            unsigned diff = abs_diff_vect_complex_s32(A.data, A.exp, ref, FFT_N, &error);
            if(diff > worst_error) worst_error = diff;
            TEST_ASSERT_CONVERSION(error);
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+WIGGLE, diff, "Output delta is too large");
        }
        
#if PRINT_ERRORS
        printf("    %s worst error (%u-point): %u\n", __func__, FFT_N, worst_error);
#endif

#if TIME_FUNCS
        printf("    %s (%u-point): %f us\n", __func__, FFT_N, worst_timing);
#endif

#if WRITE_PERFORMANCE_INFO
        fprintf(perf_file, "%s, %u, %u, %0.02f,\n", &(__func__[5]), FFT_N, worst_error, worst_timing);
#endif
    }

}




void test_bfp_fft_inverse_complex()
{
#if PRINT_FUNC_NAMES
    printf("%s..\n", __func__);
#endif

    unsigned r = 1;

    for(unsigned k = MIN_FFT_N_LOG2; k <= MAX_PROC_FRAME_LENGTH_LOG2; k++){
        unsigned FFT_N = (1<<k);
        unsigned worst_error = 0;
        float worst_timing = 0.0f;

        double sine_table[(MAX_PROC_FRAME_LENGTH/2) + 1];

        flt_make_sine_table_double(sine_table, FFT_N);
        
        for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
            

            complex_s32_t DWORD_ALIGNED a[MAX_PROC_FRAME_LENGTH];

            bfp_complex_s32_t A;

            complex_double_t DWORD_ALIGNED ref[MAX_PROC_FRAME_LENGTH];

            conv_error_e error = 0;
            const exponent_t initial_exponent = sext(pseudo_rand_int32(&r), EXPONENT_SIZE);
            right_shift_t shr = pseudo_rand_uint32(&r) % MAX_HEADROOM;

            for(unsigned i = 0; i < FFT_N; i++){
                a[i].re = pseudo_rand_int32(&r) >> shr;
                a[i].im = pseudo_rand_int32(&r) >> shr;
                ref[i].re = conv_s32_to_double(a[i].re, initial_exponent, &error);
                ref[i].im = conv_s32_to_double(a[i].im, initial_exponent, &error);
            }
            TEST_ASSERT_CONVERSION(error);

            bfp_complex_s32_init(&A, a, initial_exponent, FFT_N, 1);

            flt_bit_reverse_indexes_double(ref, FFT_N);
            flt_fft_inverse_double(ref, FFT_N, sine_table);

            unsigned ts1 = getTimestamp();
            bfp_fft_inverse_complex(&A);
            unsigned ts2 = getTimestamp();
            
            float timing = (ts2-ts1)/100.0;
            if(timing > worst_timing) worst_timing = timing;

            unsigned diff = abs_diff_vect_complex_s32(A.data, A.exp, ref, FFT_N, &error);
            if(diff > worst_error) worst_error = diff;
            TEST_ASSERT_CONVERSION(error);
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+WIGGLE, diff, "Output delta is too large");
        }
        
#if PRINT_ERRORS
        printf("    %s worst error (%u-point): %u\n", __func__, FFT_N, worst_error);
#endif

#if TIME_FUNCS
        printf("    %s (%u-point): %f us\n", __func__, FFT_N, worst_timing);
#endif

#if WRITE_PERFORMANCE_INFO
        fprintf(perf_file, "%s, %u, %u, %0.02f,\n", &(__func__[5]), FFT_N, worst_error, worst_timing);
#endif
    }
}



void test_bfp_fft_forward_stereo()
{
#if PRINT_FUNC_NAMES
    printf("%s..\n", __func__);
#endif

    unsigned r = 1;

    for(unsigned k = MIN_FFT_N_LOG2; k <= MAX_PROC_FRAME_LENGTH_LOG2; k++){
        unsigned FFT_N = (1<<k);
        unsigned worst_error = 0;
        float worst_timing = 0.0f;

        double sine_table[(MAX_PROC_FRAME_LENGTH/2) + 1];

        flt_make_sine_table_double(sine_table, FFT_N);
        
        for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){

            const unsigned seed = r;

            ch_pair_s32_t DWORD_ALIGNED a[MAX_PROC_FRAME_LENGTH];

            bfp_ch_pair_s32_t A;

            complex_double_t DWORD_ALIGNED ref[MAX_PROC_FRAME_LENGTH];

            conv_error_e error = 0;
            const exponent_t initial_exponent = sext(pseudo_rand_int32(&r), EXPONENT_SIZE);
            right_shift_t shr = pseudo_rand_uint32(&r) % MAX_HEADROOM;
            

            for(unsigned i = 0; i < FFT_N; i++){
                a[i].ch_a = pseudo_rand_int32(&r) >> shr;
                a[i].ch_b = pseudo_rand_int32(&r) >> shr;
                ref[i].re = conv_s32_to_double(a[i].ch_a, initial_exponent, &error);
                ref[i].im = conv_s32_to_double(a[i].ch_b, initial_exponent, &error);
            }
            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            bfp_ch_pair_s32_init(&A, a, initial_exponent, FFT_N, 1);

            flt_bit_reverse_indexes_double(ref, FFT_N);
            flt_fft_forward_double(ref, FFT_N, sine_table);
            flt_fft_split_spectrum_double(ref, FFT_N);

            bfp_complex_s32_t z1, z2;

            unsigned ts1 = getTimestamp();
            bfp_fft_forward_stereo(&z1, &z2, &A);
            unsigned ts2 = getTimestamp();
            
            float timing = (ts2-ts1)/100.0;
            if(timing > worst_timing) worst_timing = timing;

            unsigned diff = abs_diff_vect_complex_s32((complex_s32_t*) A.data, A.exp, ref, FFT_N, &error);
            if(diff > worst_error) worst_error = diff;

            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+WIGGLE, diff, "Output delta is too large");
            TEST_ASSERT_CONVERSION(error);
        }
        
#if PRINT_ERRORS
        printf("    %s worst error (%u-point): %u\n", __func__, FFT_N, worst_error);
#endif

#if TIME_FUNCS
        printf("    %s (%u-point): %f us\n", __func__, FFT_N, worst_timing);
#endif

#if WRITE_PERFORMANCE_INFO
        fprintf(perf_file, "%s, %u, %u, %0.02f,\n", &(__func__[5]), FFT_N, worst_error, worst_timing);
#endif
    }
}




void test_bfp_fft_inverse_stereo()
{
#if PRINT_FUNC_NAMES
    printf("%s..\n", __func__);
#endif

    unsigned r = 1;

    for(unsigned k = MIN_FFT_N_LOG2; k <= MAX_PROC_FRAME_LENGTH_LOG2; k++){
        unsigned FFT_N = (1<<k);
        unsigned worst_error = 0;
        float worst_timing = 0.0f;

        double sine_table[(MAX_PROC_FRAME_LENGTH/2) + 1];

        flt_make_sine_table_double(sine_table, FFT_N);
        
        for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
            

            complex_s32_t DWORD_ALIGNED ab_data[MAX_PROC_FRAME_LENGTH];
            complex_double_t DWORD_ALIGNED ref[MAX_PROC_FRAME_LENGTH];

            bfp_complex_s32_t A_fft;
            bfp_complex_s32_t B_fft;
            bfp_ch_pair_s32_t AB;


            conv_error_e error = 0;
            const exponent_t initial_exponent = sext(pseudo_rand_int32(&r), EXPONENT_SIZE);
            right_shift_t shr = pseudo_rand_uint32(&r) % MAX_HEADROOM;

            for(unsigned i = 0; i < FFT_N; i++){
                ab_data[i].re = pseudo_rand_int32(&r) >> shr;
                ab_data[i].im = pseudo_rand_int32(&r) >> shr;
                ref[i].re = conv_s32_to_double(ab_data[i].re, initial_exponent, &error);
                ref[i].im = conv_s32_to_double(ab_data[i].im, initial_exponent, &error);
            }
            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            bfp_complex_s32_init(&A_fft, &ab_data[0], initial_exponent, FFT_N/2, 1);
            bfp_complex_s32_init(&B_fft, &ab_data[FFT_N/2], initial_exponent, FFT_N/2, 1);

            bfp_ch_pair_s32_init(&AB, (ch_pair_s32_t*) ab_data, 0, FFT_N, 0);

            flt_fft_merge_spectra_double(ref, FFT_N);
            flt_bit_reverse_indexes_double(ref, FFT_N);
            flt_fft_inverse_double(ref, FFT_N, sine_table);

            unsigned ts1 = getTimestamp();
             bfp_fft_inverse_stereo(&AB, &A_fft, &B_fft);
            unsigned ts2 = getTimestamp();
            
            float timing = (ts2-ts1)/100.0;
            if(timing > worst_timing) worst_timing = timing;

            unsigned diff = abs_diff_vect_complex_s32((complex_s32_t*) AB.data, AB.exp, ref, FFT_N, &error);
            if(diff > worst_error) worst_error = diff;
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+WIGGLE, diff, "Output delta is too large");
            TEST_ASSERT_CONVERSION(error);
        }
        
#if PRINT_ERRORS
        printf("    %s worst error (%u-point): %u\n", __func__, FFT_N, worst_error);
#endif

#if TIME_FUNCS
        printf("    %s (%u-point): %f us\n", __func__, FFT_N, worst_timing);
#endif

#if WRITE_PERFORMANCE_INFO
        fprintf(perf_file, "%s, %u, %u, %0.02f,\n", &(__func__[5]), FFT_N, worst_error, worst_timing);
#endif
    }
}








void test_bfp_fft_forward_mono()
{
#if PRINT_FUNC_NAMES
    printf("%s..\n", __func__);
#endif

    unsigned r = 1;

    for(unsigned k = MAX(MIN_FFT_N_LOG2, 4); k <= MAX_PROC_FRAME_LENGTH_LOG2; k++){

        unsigned FFT_N = (1<<k);
        unsigned worst_error = 0;
        float worst_timing = 0.0f;

        double sine_table[(MAX_PROC_FRAME_LENGTH/2) + 1];

        flt_make_sine_table_double(sine_table, FFT_N);
        
        for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
        
            int32_t DWORD_ALIGNED a[MAX_PROC_FRAME_LENGTH];
            complex_double_t DWORD_ALIGNED ref[MAX_PROC_FRAME_LENGTH];

            bfp_s32_t A;
            bfp_complex_s32_t* A_fft;

            conv_error_e error = 0;
            const exponent_t initial_exponent = sext(pseudo_rand_int32(&r), EXPONENT_SIZE);
            right_shift_t shr = pseudo_rand_uint32(&r) % MAX_HEADROOM;

            for(unsigned i = 0; i < FFT_N; i++){
                a[i] = pseudo_rand_int32(&r) >> shr;
                
                ref[i].re = conv_s32_to_double(a[i], initial_exponent, &error);
                ref[i].im = 0;
            }
            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            bfp_s32_init(&A, a, initial_exponent, FFT_N, 1);

            flt_bit_reverse_indexes_double(ref, FFT_N);
            flt_fft_forward_double(ref, FFT_N, sine_table);
            ref[0].im = ref[FFT_N/2].re;

            unsigned ts1 = getTimestamp();
            A_fft = bfp_fft_forward_mono(&A);
            unsigned ts2 = getTimestamp();
            
            float timing = (ts2-ts1)/100.0;
            if(timing > worst_timing) worst_timing = timing;


            unsigned diff = abs_diff_vect_complex_s32(A_fft->data, A_fft->exp, ref, A_fft->length, &error);
            if(diff > worst_error) worst_error = diff;
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+WIGGLE, diff, "Output delta is too large");
            TEST_ASSERT_CONVERSION(error);
        }
        
#if PRINT_ERRORS
        printf("    %s worst error (%u-point): %u\n", __func__, FFT_N, worst_error);
#endif

#if TIME_FUNCS
        printf("    %s (%u-point): %f us\n", __func__, FFT_N, worst_timing);
#endif

#if WRITE_PERFORMANCE_INFO
        fprintf(perf_file, "%s, %u, %u, %0.02f,\n", &(__func__[5]), FFT_N, worst_error, worst_timing);
#endif
    }
}




void test_bfp_fft_inverse_mono()
{
#if PRINT_FUNC_NAMES
    printf("%s..\n", __func__);
#endif

    unsigned r = 1;

    for(unsigned k = MAX(MIN_FFT_N_LOG2, 4); k <= MAX_PROC_FRAME_LENGTH_LOG2; k++){

        unsigned FFT_N = (1<<k);
        unsigned N = FFT_N;
        unsigned worst_error = 0;
        float worst_timing = 0.0f;

        double sine_table[(MAX_PROC_FRAME_LENGTH/2) + 1];

        flt_make_sine_table_double(sine_table, FFT_N);
        
        for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
            
            complex_s32_t DWORD_ALIGNED a[MAX_PROC_FRAME_LENGTH/2];
            complex_double_t DWORD_ALIGNED ref[MAX_PROC_FRAME_LENGTH];
            double DWORD_ALIGNED ref_real[MAX_PROC_FRAME_LENGTH];

            bfp_complex_s32_t A_fft;
            bfp_s32_t* A;

            conv_error_e error = 0;
            const exponent_t initial_exponent = sext(pseudo_rand_int32(&r), EXPONENT_SIZE);
            right_shift_t shr = pseudo_rand_uint32(&r) % MAX_HEADROOM;

            for(unsigned i = 0; i < N/2; i++){
                a[i].re = pseudo_rand_int32(&r) >> shr;
                a[i].im = pseudo_rand_int32(&r) >> shr;
                
                ref[i].re = conv_s32_to_double(a[i].re, initial_exponent, &error);
                ref[i].im = conv_s32_to_double(a[i].im, initial_exponent, &error);

                if(i){
                    ref[N-i].re =  ref[i].re;
                    ref[N-i].im = -ref[i].im;
                }
            }
            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
            ref[N/2].re = ref[0].im;
            ref[N/2].im = ref[0].im = 0;

            bfp_complex_s32_init(&A_fft, a, initial_exponent, N/2, 1);

            flt_bit_reverse_indexes_double(ref, FFT_N);
            flt_fft_inverse_double(ref, FFT_N, sine_table);

            unsigned ts1 = getTimestamp();
            A = bfp_fft_inverse_mono(&A_fft);
            unsigned ts2 = getTimestamp();
            
            float timing = (ts2-ts1)/100.0;
            if(timing > worst_timing) worst_timing = timing;

            for(int i = 0; i < N; i++)
                ref_real[i] = ref[i].re;

            unsigned diff = abs_diff_vect_s32(A->data, A->exp, ref_real, N, &error);
            if(diff > worst_error) worst_error = diff;
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+WIGGLE, diff, "Output delta is too large");
            TEST_ASSERT_CONVERSION(error);
        }
        
#if PRINT_ERRORS
        printf("    %s worst error (%u-point): %u\n", __func__, FFT_N, worst_error);
#endif

#if TIME_FUNCS
        printf("    %s (%u-point): %f us\n", __func__, FFT_N, worst_timing);
#endif

#if WRITE_PERFORMANCE_INFO
        fprintf(perf_file, "%s, %u, %u, %0.02f,\n", &(__func__[5]), FFT_N, worst_error, worst_timing);
#endif
    }
}

void test_bfp_fft()
{
    SET_TEST_FILE();

    RUN_TEST(test_bfp_fft_forward_complex);
    RUN_TEST(test_bfp_fft_inverse_complex);
    
    RUN_TEST(test_bfp_fft_forward_stereo);
    RUN_TEST(test_bfp_fft_inverse_stereo);
    
    RUN_TEST(test_bfp_fft_forward_mono);
    RUN_TEST(test_bfp_fft_inverse_mono);

}
