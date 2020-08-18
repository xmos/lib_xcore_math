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

#define MAX_PROC_FRAME_LENGTH_LOG2 10
#define MAX_PROC_FRAME_LENGTH (1<<MAX_PROC_FRAME_LENGTH_LOG2)


#define LOOPS_LOG2 4


#define WIGGLE 20
#define ENFORCED_HEADROOM 1
#define TIME_FUNCS 1
#define PRINT_ERRORS 0


void test_xs3_fft_dif_forward_complete()
{

    unsigned r = 0x6999B20C;

    for(unsigned k = 2; k <= MAX_PROC_FRAME_LENGTH_LOG2; k++){
        // printf("Frame size: %u\n", 1<<k);
        const unsigned proc_frame_length = (1<<k);
        unsigned worst_case = 0;

        double sine_table[(MAX_PROC_FRAME_LENGTH/2) + 1];

        flt_make_sine_table_double(sine_table, proc_frame_length);
        
        for(unsigned t = 0; t < (1 << LOOPS_LOG2); t++){
            
            // printf("Iter: %u  (0x%08X)\n", t, r);
            const unsigned seed = r;

            complex_s32_t ALIGNED a[MAX_PROC_FRAME_LENGTH];
            complex_double_t ALIGNED A[MAX_PROC_FRAME_LENGTH];
            double real[MAX_PROC_FRAME_LENGTH], imag[MAX_PROC_FRAME_LENGTH];

            conv_error_e error = 0;
            exponent_t exponent = 0;// sext(pseudo_rand_int32(&r), 5);
            headroom_t headroom;

            for(unsigned i = 0; i < proc_frame_length; i++){
                // complex_s32_t ttt[4] = {
                //     { 0x7FFFFFFF,  0x00000000},
                //     { 0x00000000,  0x7FFFFFFF},
                //     {-0x7FFFFFFF,  0x00000000},
                //     { 0x00000000, -0x7FFFFFFF},
                // };

                // a[i].re = ttt[i%4].re >> 1;
                // a[i].im = ttt[i%4].im >> 1;

                a[i].re = pseudo_rand_int32(&r) >> ENFORCED_HEADROOM;
                a[i].im = pseudo_rand_int32(&r) >> ENFORCED_HEADROOM;
                A[i].re = conv_s32_to_double(a[i].re, exponent, &error);
                A[i].im = conv_s32_to_double(a[i].im, exponent, &error);
                real[i] = A[i].re;
                imag[i] = A[i].im;
            }
            TEST_ASSERT_CONVERSION(error);

            headroom = xs3_headroom_vect_s32((int32_t*) a, 2*proc_frame_length);

            // printf("headroom = %u\n", headroom);
            
            // if(k == 3 && t==0){
            //     // printf("r = %u\n", r);
            //     printf("# exponent_in = %d\n", exponent);
            //     printf("# headroom_in = %u\n", headroom);
            //     PRINT_VAR("a", print_vect_complex_s32(a, exponent, proc_frame_length, &error));
            //     PRINT_VAR("A", print_vect_complex_double(A, proc_frame_length, &error));
            // }

            Fft_transform(real, imag, proc_frame_length);

            flt_bit_reverse_indexes_double(A, proc_frame_length);
            flt_fft_forward_double(A, proc_frame_length, sine_table);

            xs3_fft_dif_forward(a, proc_frame_length, &headroom, XS3_DIF_FFT_LUT(k), &exponent);
            xs3_fft_index_bit_reversal(a, proc_frame_length);

            unsigned diff = 0;

            diff = abs_diff_vect_complex_s32(a, exponent, A, proc_frame_length, &error);
            TEST_ASSERT_CONVERSION(error);

            if(diff > k+WIGGLE){
                printf("# k = %u\tt = %u\t\t(seed: 0x%08X)\n", k, t, seed);
                printf("# exponent_out = %d\n", exponent);
                printf("# headroom_out = %u\n", headroom);

                PRINT_VAR("a_fft", print_vect_complex_s32_fft(a, exponent, proc_frame_length, &error));

                // for(int n = 0; n < proc_frame_length; n++){
                //     a[n].re = conv_double_to_s32(A[n].re, exponent, &error);
                //     a[n].im = conv_double_to_s32(A[n].im, exponent, &error);
                // }
                // TEST_ASSERT_CONVERSION(error);

                PRINT_VAR("A_fft", print_vect_complex_double_fft(A, proc_frame_length, &error));
            }

            if(diff > worst_case) { worst_case = diff; /* printf("worst_case: %u\n", worst_case); */ }
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+WIGGLE, diff, "Output delta is too large");

            if(diff > worst_case) { worst_case = diff; /* printf("worst_case: %u\n", worst_case); */ }
            diff = abs_diff_vect_complex_s32(a, exponent, A, proc_frame_length, &error);
            TEST_ASSERT_CONVERSION(error);

            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+WIGGLE, diff, "Output delta is too large");
        }

#if PRINT_ERRORS
        printf("Worst case (%u-point): %d\n", proc_frame_length, worst_case);
#endif
    }
}









void test_xs3_fft_dif_inverse_complete()
{

    unsigned r = 1;

    for(unsigned k = 2; k <= MAX_PROC_FRAME_LENGTH_LOG2; k++){
        // printf("Frame size: %u\n", 1<<k);
        const unsigned proc_frame_length = (1<<k);
        unsigned worst_case = 0;

        double sine_table[(MAX_PROC_FRAME_LENGTH/2) + 1];

        flt_make_sine_table_double(sine_table, proc_frame_length);

        for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
            const unsigned seed = r;

            complex_s32_t ALIGNED a[MAX_PROC_FRAME_LENGTH];
            complex_double_t ALIGNED A[MAX_PROC_FRAME_LENGTH];
            double real[MAX_PROC_FRAME_LENGTH], imag[MAX_PROC_FRAME_LENGTH];

            conv_error_e error = 0;
            exponent_t exponent = sext(pseudo_rand_int32(&r), 5);
            headroom_t headroom;

            for(unsigned i = 0; i < proc_frame_length; i++){
                // complex_s32_t ttt[4] = {
                //     { 0x7FFFFFFF,  0x00000000},
                //     { 0x00000000,  0x7FFFFFFF},
                //     {-0x7FFFFFFF,  0x00000000},
                //     { 0x00000000, -0x7FFFFFFF},
                // };

                // a[i].re = ttt[i%4].re >> 1;
                // a[i].im = ttt[i%4].im >> 1;

                a[i].re = pseudo_rand_int32(&r) >> ENFORCED_HEADROOM;
                a[i].im = pseudo_rand_int32(&r) >> ENFORCED_HEADROOM;
                A[i].re = conv_s32_to_double(a[i].re, exponent, &error);
                A[i].im = conv_s32_to_double(a[i].im, exponent, &error);
                real[i] = A[i].re;
                imag[i] = A[i].im;
            }
            TEST_ASSERT_CONVERSION(error);
            
            headroom = xs3_headroom_vect_s32((int32_t*) a, 2*proc_frame_length);

            // printf("headroom = %u\n", headroom);
            
            // if(k == 3 && t==0){
            //     // printf("r = %u\n", r);
            //     printf("# exponent_in = %d\n", exponent);
            //     printf("# headroom_in = %u\n", headroom);
            //     PRINT_VAR("a_fft", print_vect_complex_s32(a, exponent, proc_frame_length, &error));
            //     PRINT_VAR("A_fft", print_vect_complex_double(A, proc_frame_length, &error));
            // }

            flt_bit_reverse_indexes_double(A, proc_frame_length);
            flt_fft_inverse_double(A, proc_frame_length, sine_table);

            xs3_fft_dif_inverse(a, proc_frame_length, &headroom, XS3_DIF_FFT_LUT(k), &exponent);
            xs3_fft_index_bit_reversal(a, proc_frame_length);

            Fft_inverseTransform(real, imag, proc_frame_length);

            unsigned diff = 0;

            diff = abs_diff_vect_complex_s32(a, exponent, A, proc_frame_length, &error);
            TEST_ASSERT_CONVERSION(error);

            if(diff > (k+WIGGLE)){
                printf("# k = %u\tt = %u\t\t(seed: 0x%08X)\n", k, t, seed);
                printf("# exponent_out = %d\n", exponent);
                printf("# headroom_out = %u\n", headroom);

                PRINT_VAR("a_fft", print_vect_complex_s32_fft(a, exponent, proc_frame_length, &error));

                // for(int n = 0; n < proc_frame_length; n++){
                //     a[n].re = conv_double_to_s32(A[n].re, exponent, &error);
                //     a[n].im = conv_double_to_s32(A[n].im, exponent, &error);
                // }
                // TEST_ASSERT_CONVERSION(error);

                PRINT_VAR("A_fft", print_vect_complex_double_fft(A, proc_frame_length, &error));
            }
            
            if(diff > worst_case) { worst_case = diff; /* printf("worst_case: %u\n", worst_case); */ }
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+WIGGLE, diff, "Output delta is too large");
            TEST_ASSERT_CONVERSION(error);

            for(unsigned i = 0; i < proc_frame_length; i++){
                A[i].re = real[i];
                A[i].im = imag[i];
            }

            diff = abs_diff_vect_complex_s32(a, exponent, A, proc_frame_length, &error);
            TEST_ASSERT_CONVERSION(error);

            if(diff > worst_case) { worst_case = diff; /* printf("worst_case: %u\n", worst_case); */ }
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+WIGGLE, diff, "Output delta is too large");
        }

#if PRINT_ERRORS
        printf("Worst case (%u-point): %d\n", proc_frame_length, worst_case);
#endif
    }
}









void test_xs3_fft_dif_forward(){

    unsigned r = 1;
    printf("%s..\n", __func__);

    for(unsigned k = 2; k <= MAX_PROC_FRAME_LENGTH_LOG2; k++){

        unsigned proc_frame_length = (1<<k);
        unsigned N = (1<<k);
        float worst_timing = 0.0f;
        double sine_table[(MAX_PROC_FRAME_LENGTH/2) + 1];

        flt_make_sine_table_double(sine_table, proc_frame_length);
        
        for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
            

            complex_s32_t ALIGNED a[MAX_PROC_FRAME_LENGTH];
            complex_double_t ALIGNED A[MAX_PROC_FRAME_LENGTH];

            conv_error_e error = 0;
            exponent_t exponent = 0; //sext(pseudo_rand_int32(&r), 5);
            headroom_t headroom;

            for(unsigned i=0;i<proc_frame_length;i++){
                a[i].re = pseudo_rand_int32(&r) >> ENFORCED_HEADROOM;
                a[i].im = pseudo_rand_int32(&r) >> ENFORCED_HEADROOM;
                A[i].re = conv_s32_to_double(a[i].re, exponent, &error);
                A[i].im = conv_s32_to_double(a[i].im, exponent, &error);
            }
            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
            
            headroom = xs3_headroom_vect_s32((int32_t*) a, 2*proc_frame_length);

            // printf("headroom = %u\n", headroom);
            
            // if(k == 3 && t==0){
            //     // printf("r = %u\n", r);
            //     printf("# exponent_in = %d\n", exponent);
            //     printf("# headroom_in = %u\n", headroom);
            //     PRINT_VAR("a_fft", print_vect_complex_s32(a, exponent, proc_frame_length, &error));
            //     PRINT_VAR("A_fft", print_vect_complex_double(A, proc_frame_length, &error));
            // }
            
            flt_bit_reverse_indexes_double(A, proc_frame_length);
            flt_fft_forward_double(A, proc_frame_length, sine_table);

            unsigned ts1 = getTimestamp();
            xs3_fft_dif_forward(a, proc_frame_length, &headroom, XS3_DIF_FFT_LUT(k), &exponent);
            unsigned ts2 = getTimestamp();
            xs3_fft_index_bit_reversal(a, proc_frame_length);

            float timing = (ts2-ts1)/100.0;
            if(timing > worst_timing) worst_timing = timing;
            

            unsigned diff = abs_diff_vect_complex_s32(a, exponent, A, proc_frame_length, &error);
            TEST_ASSERT_CONVERSION(error);
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+WIGGLE, diff, "Output delta is too large");
        }

#if TIME_FUNCS
        printf("\t%s (%u-point): %f us\n", __func__, N, worst_timing);
#endif
    }
}









void test_xs3_fft_dif_inverse(){

    unsigned r = 1;
    printf("%s..\n", __func__);

    for(unsigned k = 2; k <= MAX_PROC_FRAME_LENGTH_LOG2; k++){

        unsigned proc_frame_length = (1<<k);
        unsigned N = (1<<k);
        float worst_timing = 0.0f;
        double sine_table[(MAX_PROC_FRAME_LENGTH/2) + 1];

        flt_make_sine_table_double(sine_table, proc_frame_length);
        
        for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
            

            complex_s32_t ALIGNED a[MAX_PROC_FRAME_LENGTH];
            complex_double_t ALIGNED A[MAX_PROC_FRAME_LENGTH];

            conv_error_e error = 0;
            exponent_t exponent = 0; //sext(pseudo_rand_int32(&r), 5);
            headroom_t headroom;

            for(unsigned i=0;i<proc_frame_length;i++){
                a[i].re = pseudo_rand_int32(&r) >> ENFORCED_HEADROOM;
                a[i].im = pseudo_rand_int32(&r) >> ENFORCED_HEADROOM;
                A[i].re = conv_s32_to_double(a[i].re, exponent, &error);
                A[i].im = conv_s32_to_double(a[i].im, exponent, &error);
            }
            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");
            
            headroom = xs3_headroom_vect_s32((int32_t*) a, 2*proc_frame_length);

            // printf("headroom = %u\n", headroom);
            
            // if(k == 3 && t==0){
            //     // printf("r = %u\n", r);
            //     printf("# exponent_in = %d\n", exponent);
            //     printf("# headroom_in = %u\n", headroom);
            //     PRINT_VAR("a_fft", print_vect_complex_s32(a, exponent, proc_frame_length, &error));
            //     PRINT_VAR("A_fft", print_vect_complex_double(A, proc_frame_length, &error));
            // }
            
            flt_bit_reverse_indexes_double(A, proc_frame_length);
            flt_fft_inverse_double(A, proc_frame_length, sine_table);

            unsigned ts1 = getTimestamp();
            xs3_fft_dif_inverse(a, proc_frame_length, &headroom, XS3_DIF_FFT_LUT(k), &exponent);
            unsigned ts2 = getTimestamp();
            xs3_fft_index_bit_reversal(a, proc_frame_length);

            float timing = (ts2-ts1)/100.0;
            if(timing > worst_timing) worst_timing = timing;

            unsigned diff = abs_diff_vect_complex_s32(a, exponent, A, proc_frame_length, &error);
            TEST_ASSERT_CONVERSION(error);
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+WIGGLE, diff, "Output delta is too large");
        }

#if TIME_FUNCS
        printf("\t%s (%u-point): %f us\n", __func__, N, worst_timing);
#endif
    }
}





void test_xs3_fft_dif()
{
    SET_TEST_FILE();

    RUN_TEST(test_xs3_fft_dif_forward_complete);
    RUN_TEST(test_xs3_fft_dif_inverse_complete);
    RUN_TEST(test_xs3_fft_dif_forward);
    RUN_TEST(test_xs3_fft_dif_inverse);
}