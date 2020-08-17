// Copyright (c) 2018-2020, XMOS Ltd, All rights reserved

#include <xs1.h>
#include <xclib.h>

#include "xs3_math.h"
#include "testing.h"
#include "floating_fft.h"
#include "tst_common.h"
#include "../src/low/c/twiddle_factors.h"
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


#define LOOPS_LOG2 4

#define TIME_FUNCS 1
#define PRINT_ERRORS 1



void test_xs3_real_ifft_first_pass_s32()
{

    unsigned r = 436554;

    printf("%s..\n", __func__);

    for(unsigned k = 4; k <= MAX_PROC_FRAME_LENGTH_LOG2; k++){
        printf("Frame size: %u\n", 1<<k);
        const unsigned N = (1<<k);
        unsigned worst_case = 0;
        float worst_timing = 0.0f;

        double sine_tableA[(MAX_PROC_FRAME_LENGTH/2) + 1];

        flt_make_sine_table_double(sine_tableA, N/2);
        
        for(unsigned t = 0; t < (1 << LOOPS_LOG2); t++){
            
            // printf("Iter: %u  (0x%08X)\n", t, r);
            const unsigned seed = r;
            unsigned diff = 0;

            complex_s32_t ALIGNED a[MAX_PROC_FRAME_LENGTH/2];
            complex_double_t ALIGNED ref[MAX_PROC_FRAME_LENGTH/2];

            conv_error_e error = 0;
            exponent_t exponent = 0;
            headroom_t headroom;

            for(unsigned i = 0; i < N/2; i++){
                a[i].re = pseudo_rand_int32(&r) >> 1;
                a[i].im = pseudo_rand_int32(&r) >> 1;
                ref[i].re = conv_s32_to_double(a[i].re, exponent, &error);
                ref[i].im = conv_s32_to_double(a[i].im, exponent, &error);
            }

            diff = abs_diff_vect_complex_s32(a, exponent, (complex_double_t*) ref, N/2, &error);
            TEST_ASSERT_CONVERSION(error);
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+2, diff, "Test setup failed.");


            xs3_real_fft_final_pass_s32(a, N, XS3_DIT_REAL_FFT_LUT(N));

            unsigned ts1 = getTimestamp();
            xs3_real_ifft_first_pass_s32(a, N, XS3_DIT_REAL_FFT_LUT(N));
            unsigned ts2 = getTimestamp();

            float timing = (ts2-ts1)/100.0;
            if(timing > worst_timing) worst_timing = timing;

            // for(int i = 0; i < N/2; i++){
            //     printf("\t{ %0.04f, %0.04f }\n", (double)a[i].re, (double)a[i].im);
            //     printf("\t{ %0.04f, %0.04f }\n\n", ref[i].re, ref[i].im);
            // }
            
            diff = abs_diff_vect_complex_s32(a, exponent, ref, N/2, &error);
            TEST_ASSERT_CONVERSION(error);
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+2, diff, "Output delta is too large.");

            if(diff > worst_case) { worst_case = diff; }
        }
        
#if PRINT_ERRORS
        printf("\tWorst case error (%u-point): %d\n", N, worst_case);
#endif

#if TIME_FUNCS
        printf("\txs3_real_ifft_first_pass_s32 (%u-point): %f us\n", N, worst_timing);
#endif

    }
}








void test_xs3_real_ifft_first_pass()
{
    SET_TEST_FILE();

    RUN_TEST(test_xs3_real_ifft_first_pass_s32);
}