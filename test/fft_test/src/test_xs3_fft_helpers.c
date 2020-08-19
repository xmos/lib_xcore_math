// Copyright (c) 2018-2020, XMOS Ltd, All rights reserved

#include <xs1.h>
#include <xclib.h>

#include "xs3_math.h"
#include "testing.h"
#include "floating_fft.h"
#include "tst_common.h"
#include "../src/low/c/xs3_fft_lut.h"
#include "fft.h"
#include "unity.h"

#ifdef __XC__
#define UNSAFE unsafe
#define ALIGNED [[aligned(8)]]
#else
#define UNSAFE
#define ALIGNED __attribute__ ((aligned (8)))
#endif //__XC_


static signed sext(int a, unsigned b){
#ifdef __XS3A__
    asm("sext %0, %1": "=r"(a): "r"(b));
#else
    unsigned mask = ~((1<<b)-1);
    unsigned p = a >= 0;
    a = p? (a & ~mask) : (a | mask);
#endif
    return a;
}

#define TEST_ASSERT_CONVERSION(V) do{ \
    char qwe[100];  if((V)){ sprintf(qwe, "Conversion failure (0x%X)", (V)); TEST_ASSERT_FALSE_MESSAGE(V, qwe); }} while(0)

#define MAX_PROC_FRAME_LENGTH_LOG2 (XS3_MAX_DIT_FFT_LOG2)
#define MAX_PROC_FRAME_LENGTH (1<<MAX_PROC_FRAME_LENGTH_LOG2)

#define TIME_FUNCS 1
#define LOOPS_LOG2 4


void test_bit_reverse()
{
    unsigned r = 1;

    printf("%s..\n", __func__);

    for(unsigned k = 2; k <= MAX_PROC_FRAME_LENGTH_LOG2; k++){

        unsigned N = (1<<k);
        float worst_timing = 0.0f;
        
        for(unsigned t = 0; t <= (1<<LOOPS_LOG2); t++){

            complex_s32_t ALIGNED a[MAX_PROC_FRAME_LENGTH];
            complex_double_t ALIGNED A[MAX_PROC_FRAME_LENGTH];

            conv_error_e error = 0;
            exponent_t exponent = sext(pseudo_rand_int32(&r), 5);

            for(unsigned i = 0 ; i < N; i++){
                a[i].re = pseudo_rand_int32(&r)>>1;
                a[i].im = pseudo_rand_int32(&r)>>1;
                A[i].re = conv_s32_to_double(a[i].re, exponent, &error);
                A[i].im = conv_s32_to_double(a[i].im, exponent, &error);
            }
            TEST_ASSERT_CONVERSION(error);

            flt_bit_reverse_indexes_double(A, N);

            unsigned ts1 = getTimestamp();
            xs3_fft_index_bit_reversal(a, N);
            unsigned ts2 = getTimestamp();

            float timing = (ts2-ts1)/100.0;
            if(timing > worst_timing) worst_timing = timing;

            unsigned diff = abs_diff_vect_complex_s32(a, exponent, A, N, &error);
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k, diff, "Output delta is too large");
            TEST_ASSERT_CONVERSION(error);

        }

#if TIME_FUNCS
        printf("\t%s (%u-point): %f us\n", __func__, N, worst_timing);
#endif
    }
}


void test_xs3_tail_reverse_complex_s32()
{
    unsigned r = 1;

    printf("%s..\n", __func__);

    for(unsigned k = 3; k <= MAX_PROC_FRAME_LENGTH_LOG2; k++){

        unsigned N = (1<<k);
        float worst_timing = 0.0f;

        // printf("  N = %u\n", N);
        
        for(unsigned t = 0; t <= (1<<LOOPS_LOG2); t++){

            complex_s32_t ALIGNED a[MAX_PROC_FRAME_LENGTH];
            complex_s32_t ALIGNED ref[MAX_PROC_FRAME_LENGTH];

            for(unsigned i = 0 ; i < N; i++){
                a[i].re = pseudo_rand_int32(&r);
                a[i].im = pseudo_rand_int32(&r);
                ref[(N-i) % N] = a[i];
            }

            unsigned ts1 = getTimestamp();
            xs3_tail_reverse_complex_s32(a, N);
            unsigned ts2 = getTimestamp();

            // for(int i = 0; i < N; i++){
            //     printf("%ld\t%ld\n", a[i].re, a[i].im);
            //     printf("%ld\t%ld\n\n", ref[i].re, ref[i].im);
            // }

            float timing = (ts2-ts1)/100.0;
            if(timing > worst_timing) worst_timing = timing;

            TEST_ASSERT_EQUAL_INT32_ARRAY((int32_t*) ref, (int32_t*) a, 2*N);
        }

#if TIME_FUNCS
        printf("    %s (N=%u): %f us\n", __func__, N, worst_timing);
#endif
    }
}



void test_xs3_fft_spectra_split(){

    unsigned r = 1;

    for(unsigned k = 2; k <= MAX_PROC_FRAME_LENGTH_LOG2; k++){
        
        const unsigned N = 1<<k;
        float worst_timing = 0.0f;

        for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
            

            complex_s32_t ALIGNED a[MAX_PROC_FRAME_LENGTH];
            complex_double_t ALIGNED A[MAX_PROC_FRAME_LENGTH];

            conv_error_e error = 0;

            for(unsigned i = 0; i < N; i++){
                a[i].re = pseudo_rand_int32(&r)>>1;
                a[i].im = pseudo_rand_int32(&r)>>1;
                A[i].re = conv_s32_to_double(a[i].re, 0, &error);
                A[i].im = conv_s32_to_double(a[i].im, 0, &error);
            }

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            flt_fft_split_spectrum_double(A, N);

            unsigned ts1 = getTimestamp();
            headroom_t got_hr = xs3_fft_spectra_split(a, N);
            unsigned ts2 = getTimestamp();

            float timing = (ts2-ts1)/100.0;
            if(timing > worst_timing) worst_timing = timing;

            unsigned diff = abs_diff_vect_complex_s32(a, 0, A, N, &error);
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(1, diff, "Output delta is too large");
            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            headroom_t exp_hr = xs3_headroom_vect_s32((int32_t*)a,2*N);
            TEST_ASSERT_EQUAL_MESSAGE(exp_hr, got_hr, "Headroom error");

        }

#if TIME_FUNCS
        printf("\t%s (%u-point): %f us\n", __func__, N, worst_timing);
#endif
    }
}






void test_xs3_fft_spectra_merge(){

    unsigned r = 1;

    for(unsigned k = 2; k <= MAX_PROC_FRAME_LENGTH_LOG2; k++){
        
        unsigned N = (1<<k);
        float worst_timing = 0.0f;

        for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
            
            complex_s32_t ALIGNED a[MAX_PROC_FRAME_LENGTH];
            complex_double_t ALIGNED A[MAX_PROC_FRAME_LENGTH];

            conv_error_e error = 0;

            for(unsigned i = 0; i < N; i++){
                a[i].re = pseudo_rand_int32(&r) >> 1;
                a[i].im = pseudo_rand_int32(&r) >> 1;
                A[i].re = conv_s32_to_double(a[i].re, 0, &error);
                A[i].im = conv_s32_to_double(a[i].im, 0, &error);
            }

            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            flt_fft_merge_spectra_double(A, N);

            unsigned ts1 = getTimestamp();
            headroom_t got_hr = xs3_fft_spectra_merge(a, N);
            unsigned ts2 = getTimestamp();

            float timing = (ts2-ts1)/100.0;
            if(timing > worst_timing) worst_timing = timing;

            unsigned diff = abs_diff_vect_complex_s32(a, 0, A, N, &error);
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k, diff, "Output delta is too large");
            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            headroom_t exp_hr = xs3_headroom_vect_s32((int32_t*)a,2*N);
            TEST_ASSERT_EQUAL_MESSAGE(exp_hr, got_hr, "Headroom error");
        }

#if TIME_FUNCS
        printf("\t%s (%u-point): %f us\n", __func__, N, worst_timing);
#endif
    }
}




void test_xs3_fft_helpers()
{
    SET_TEST_FILE();

    RUN_TEST(test_bit_reverse);
    RUN_TEST(test_xs3_tail_reverse_complex_s32);
    RUN_TEST(test_xs3_fft_spectra_split);
    RUN_TEST(test_xs3_fft_spectra_merge);
}