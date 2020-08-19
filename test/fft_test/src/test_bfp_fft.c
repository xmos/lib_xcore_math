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

#define MAX_PROC_FRAME_LENGTH_LOG2 10
#define MAX_PROC_FRAME_LENGTH (1<<MAX_PROC_FRAME_LENGTH_LOG2)


#define LOOPS_LOG2 4

#define WIGGLE 20

#define TIME_FUNCS 1
#define PRINT_ERRORS 0




void test_bfp_fft_forward_complex()
{
    printf("\n\n%s..\n", __func__);
    unsigned r = 1;

    for(unsigned k = 3; k <= MAX_PROC_FRAME_LENGTH_LOG2; k++){
        // printf("Frame size: %u\n", 1<<k);

        unsigned proc_frame_length = (1<<k);
        unsigned worst_case = 0;
        float worst_timing = 0.0f;

        double sine_table[(MAX_PROC_FRAME_LENGTH/2) + 1];

        flt_make_sine_table_double(sine_table, proc_frame_length);
        
        for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
            

            complex_s32_t ALIGNED a[MAX_PROC_FRAME_LENGTH];

            bfp_complex_s32_t A;

            complex_double_t ALIGNED ref[MAX_PROC_FRAME_LENGTH];

            conv_error_e error = 0;
            const exponent_t initial_exponent = sext(pseudo_rand_int32(&r), 5);
            right_shift_t shr = pseudo_rand_uint32(&r) % 4;

            for(unsigned i = 0; i < proc_frame_length; i++){
                a[i].re = pseudo_rand_int32(&r) >> shr;
                a[i].im = pseudo_rand_int32(&r) >> shr;
                ref[i].re = conv_s32_to_double(a[i].re, initial_exponent, &error);
                ref[i].im = conv_s32_to_double(a[i].im, initial_exponent, &error);
            }
            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            bfp_init_vect_complex_s32(&A, a, initial_exponent, proc_frame_length, 1);

            flt_bit_reverse_indexes_double(ref, proc_frame_length);
            flt_fft_forward_double(ref, proc_frame_length, sine_table);

            unsigned ts1 = getTimestamp();
            bfp_fft_forward_complex(&A);
            unsigned ts2 = getTimestamp();

            float timing = (ts2-ts1)/100.0;
            if(timing > worst_timing) worst_timing = timing;

            unsigned diff = abs_diff_vect_complex_s32(A.data, A.exp, ref, proc_frame_length, &error);
            if(diff > worst_case) { worst_case = diff; /* printf("worst_case: %u\n", worst_case); */ }
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+WIGGLE, diff, "Output delta is too large");
            TEST_ASSERT_CONVERSION(error);
        }
        
#if PRINT_ERRORS
        printf("\tWorst case error (%u-point): %d\n", proc_frame_length, worst_case);
#endif

#if TIME_FUNCS
        printf("\tbfp_fft_forward_complex (%u-point): %f us\n", proc_frame_length, worst_timing);
#endif
    }

}




void test_bfp_fft_inverse_complex()
{
    printf("\n\n%s..\n", __func__);
    unsigned r = 1;

    for(unsigned k = 3; k <= MAX_PROC_FRAME_LENGTH_LOG2; k++){
        // printf("Frame size: %u\n", 1<<k);

        unsigned proc_frame_length = (1<<k);
        unsigned worst_case = 0;
        float worst_timing = 0.0f;

        double sine_table[(MAX_PROC_FRAME_LENGTH/2) + 1];

        flt_make_sine_table_double(sine_table, proc_frame_length);
        
        for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
            

            complex_s32_t ALIGNED a[MAX_PROC_FRAME_LENGTH];

            bfp_complex_s32_t A;

            complex_double_t ALIGNED ref[MAX_PROC_FRAME_LENGTH];

            conv_error_e error = 0;
            const exponent_t initial_exponent = sext(pseudo_rand_int32(&r), 5);
            right_shift_t shr = pseudo_rand_uint32(&r) % 4;

            for(unsigned i = 0; i < proc_frame_length; i++){
                a[i].re = pseudo_rand_int32(&r) >> shr;
                a[i].im = pseudo_rand_int32(&r) >> shr;
                ref[i].re = conv_s32_to_double(a[i].re, initial_exponent, &error);
                ref[i].im = conv_s32_to_double(a[i].im, initial_exponent, &error);
            }
            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            bfp_init_vect_complex_s32(&A, a, initial_exponent, proc_frame_length, 1);

            flt_bit_reverse_indexes_double(ref, proc_frame_length);
            flt_fft_inverse_double(ref, proc_frame_length, sine_table);

            unsigned ts1 = getTimestamp();
            bfp_fft_inverse_complex(&A);
            unsigned ts2 = getTimestamp();
            
            float timing = (ts2-ts1)/100.0;
            if(timing > worst_timing) worst_timing = timing;

            unsigned diff = abs_diff_vect_complex_s32(A.data, A.exp, ref, proc_frame_length, &error);
            if(diff > worst_case) { worst_case = diff; /* printf("worst_case: %u\n", worst_case); */ }
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+WIGGLE, diff, "Output delta is too large");
            TEST_ASSERT_CONVERSION(error);
        }
        
#if PRINT_ERRORS
        printf("\tWorst case error (%u-point): %d\n", proc_frame_length, worst_case);
#endif

#if TIME_FUNCS
        printf("\tbfp_fft_inverse_complex (%u-point): %f us\n", proc_frame_length, worst_timing);
#endif
    }
}



void test_bfp_fft_forward_stereo()
{
    printf("\n\n%s..\n", __func__);
    unsigned r = 1;

    for(unsigned k = 3; k <= MAX_PROC_FRAME_LENGTH_LOG2; k++){
        // printf("Frame size: %u\n", 1<<k);

        unsigned proc_frame_length = (1<<k);
        unsigned worst_case = 0;
        float worst_timing = 0.0f;

        double sine_table[(MAX_PROC_FRAME_LENGTH/2) + 1];

        flt_make_sine_table_double(sine_table, proc_frame_length);
        
        for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){

            const unsigned seed = r;

            ch_pair_s32_t ALIGNED a[MAX_PROC_FRAME_LENGTH];

            bfp_ch_pair_s32_t A;

            complex_double_t ALIGNED ref[MAX_PROC_FRAME_LENGTH];

            conv_error_e error = 0;
            const exponent_t initial_exponent = sext(pseudo_rand_int32(&r), 5);
            right_shift_t shr = pseudo_rand_uint32(&r) % 4;
            

            for(unsigned i = 0; i < proc_frame_length; i++){
                a[i].ch_a = pseudo_rand_int32(&r) >> shr;
                a[i].ch_b = pseudo_rand_int32(&r) >> shr;
                ref[i].re = conv_s32_to_double(a[i].ch_a, initial_exponent, &error);
                ref[i].im = conv_s32_to_double(a[i].ch_b, initial_exponent, &error);
            }
            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            bfp_init_vect_ch_pair_s32(&A, a, initial_exponent, proc_frame_length, 1);

            flt_bit_reverse_indexes_double(ref, proc_frame_length);
            flt_fft_forward_double(ref, proc_frame_length, sine_table);
            flt_fft_split_spectrum_double(ref, proc_frame_length);

            bfp_complex_s32_t z1, z2;

            unsigned ts1 = getTimestamp();
            bfp_fft_forward_stereo(&z1, &z2, &A);
            unsigned ts2 = getTimestamp();
            
            float timing = (ts2-ts1)/100.0;
            if(timing > worst_timing) worst_timing = timing;

            unsigned diff = abs_diff_vect_complex_s32((complex_s32_t*) A.data, A.exp, ref, proc_frame_length, &error);
            if(diff > worst_case) { worst_case = diff; /* printf("worst_case: %u\n", worst_case); */ }

            if(diff > k+WIGGLE){
                printf("# k = %u\tt = %u\t\t(seed: 0x%08X)\n", k, t, seed);
                printf("# exponent_out = %d\n", A.exp);
                printf("# headroom_out = %u\n", A.hr);

                printf("#### %ld %ld\t%d\t%u\n", z1.data[1].re, z1.data[1].im, z1.exp, z1.length);

                PRINT_VAR("z1", print_vect_complex_s32_fft(z1.data, z1.exp, z1.length, &error));
                PRINT_VAR("z2", print_vect_complex_s32_fft(z2.data, z2.exp, z2.length, &error));

                PRINT_VAR("ref1", print_vect_complex_double_fft(&ref[0], proc_frame_length/2, &error));
                PRINT_VAR("ref2", print_vect_complex_double_fft(&ref[proc_frame_length/2], proc_frame_length/2, &error));
            }

            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+WIGGLE, diff, "Output delta is too large");
            TEST_ASSERT_CONVERSION(error);
        }
        
#if PRINT_ERRORS
        printf("\tWorst case error (%u-point): %d\n", proc_frame_length, worst_case);
#endif

#if TIME_FUNCS
        printf("\tbfp_fft_forward_stereo (%u-point): %f us\n", proc_frame_length, worst_timing);
#endif
    }
}




void test_bfp_fft_inverse_stereo()
{
    printf("\n\n%s..\n", __func__);
    unsigned r = 1;

    for(unsigned k = 3; k <= MAX_PROC_FRAME_LENGTH_LOG2; k++){
        // printf("Frame size: %u\n", 1<<k);

        unsigned proc_frame_length = (1<<k);
        unsigned worst_case = 0;
        float worst_timing = 0.0f;

        double sine_table[(MAX_PROC_FRAME_LENGTH/2) + 1];

        flt_make_sine_table_double(sine_table, proc_frame_length);
        
        for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
            

            complex_s32_t ALIGNED ab_data[MAX_PROC_FRAME_LENGTH];
            complex_double_t ALIGNED ref[MAX_PROC_FRAME_LENGTH];

            bfp_complex_s32_t A_fft;
            bfp_complex_s32_t B_fft;
            bfp_ch_pair_s32_t AB;


            conv_error_e error = 0;
            const exponent_t initial_exponent = sext(pseudo_rand_int32(&r), 5);
            right_shift_t shr = pseudo_rand_uint32(&r) % 4;

            for(unsigned i = 0; i < proc_frame_length; i++){
                ab_data[i].re = pseudo_rand_int32(&r) >> shr;
                ab_data[i].im = pseudo_rand_int32(&r) >> shr;
                ref[i].re = conv_s32_to_double(ab_data[i].re, initial_exponent, &error);
                ref[i].im = conv_s32_to_double(ab_data[i].im, initial_exponent, &error);
            }
            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            bfp_init_vect_complex_s32(&A_fft, &ab_data[0], initial_exponent, proc_frame_length/2, 1);
            bfp_init_vect_complex_s32(&B_fft, &ab_data[proc_frame_length/2], initial_exponent, proc_frame_length/2, 1);
            //For this function, the output BFP vector must already be initialized (exponent and hr don't matter)
            bfp_init_vect_ch_pair_s32(&AB, (ch_pair_s32_t*) ab_data, 0, proc_frame_length, 0);

            flt_fft_merge_spectra_double(ref, proc_frame_length);
            flt_bit_reverse_indexes_double(ref, proc_frame_length);
            flt_fft_inverse_double(ref, proc_frame_length, sine_table);

            unsigned ts1 = getTimestamp();
             bfp_fft_inverse_stereo(&AB, &A_fft, &B_fft);
            unsigned ts2 = getTimestamp();
            
            float timing = (ts2-ts1)/100.0;
            if(timing > worst_timing) worst_timing = timing;

            unsigned diff = abs_diff_vect_complex_s32((complex_s32_t*) AB.data, AB.exp, ref, proc_frame_length, &error);
            if(diff > worst_case) { worst_case = diff; /* printf("worst_case: %u\n", worst_case); */ }
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+WIGGLE, diff, "Output delta is too large");
            TEST_ASSERT_CONVERSION(error);
        }
        
#if PRINT_ERRORS
        printf("\tWorst case error (%u-point): %d\n", proc_frame_length, worst_case);
#endif

#if TIME_FUNCS
        printf("\t bfp_fft_inverse_stereo (%u-point): %f us\n", proc_frame_length, worst_timing);
#endif
    }
}








void test_bfp_fft_forward_mono()
{
    printf("\n\n%s..\n", __func__);
    unsigned r = 1;

    for(unsigned k = 4; k <= MAX_PROC_FRAME_LENGTH_LOG2; k++){
        // printf("Frame size: %u\n", 1<<k);

        unsigned proc_frame_length = (1<<k);
        unsigned worst_case = 0;
        float worst_timing = 0.0f;

        double sine_table[(MAX_PROC_FRAME_LENGTH/2) + 1];

        flt_make_sine_table_double(sine_table, proc_frame_length);
        
        for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
        
            int32_t ALIGNED a[MAX_PROC_FRAME_LENGTH];
            complex_double_t ALIGNED ref[MAX_PROC_FRAME_LENGTH];

            bfp_s32_t A;
            bfp_complex_s32_t* A_fft;

            conv_error_e error = 0;
            const exponent_t initial_exponent = sext(pseudo_rand_int32(&r), 5);
            right_shift_t shr = pseudo_rand_uint32(&r) % 4;

            for(unsigned i = 0; i < proc_frame_length; i++){
                a[i] = pseudo_rand_int32(&r) >> shr;
                
                ref[i].re = conv_s32_to_double(a[i], initial_exponent, &error);
                ref[i].im = 0;
            }
            TEST_ASSERT_FALSE_MESSAGE(error, "Conversion error");

            bfp_init_vect_s32(&A, a, initial_exponent, proc_frame_length, 1);

            flt_bit_reverse_indexes_double(ref, proc_frame_length);
            flt_fft_forward_double(ref, proc_frame_length, sine_table);
            ref[0].im = ref[proc_frame_length/2].re;

            unsigned ts1 = getTimestamp();
            A_fft = bfp_fft_forward_mono(&A);
            unsigned ts2 = getTimestamp();
            
            float timing = (ts2-ts1)/100.0;
            if(timing > worst_timing) worst_timing = timing;


            unsigned diff = abs_diff_vect_complex_s32(A_fft->data, A_fft->exp, ref, A_fft->length, &error);
            if(diff > worst_case) { worst_case = diff; /* printf("worst_case: %u\n", worst_case); */ }
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+WIGGLE, diff, "Output delta is too large");
            TEST_ASSERT_CONVERSION(error);
        }
        
#if PRINT_ERRORS
        printf("\tWorst case error (%u-point): %d\n", proc_frame_length, worst_case);
#endif

#if TIME_FUNCS
        printf("\tbfp_fft_forward_mono (%u-point): %f us\n", proc_frame_length, worst_timing);
#endif
    }
}




void test_bfp_fft_inverse_mono()
{
    printf("\n\n%s..\n", __func__);
    unsigned r = 1;

    for(unsigned k = 4; k <= MAX_PROC_FRAME_LENGTH_LOG2; k++){
        // printf("Frame size: %u\n", 1<<k);

        unsigned proc_frame_length = (1<<k);
        unsigned N = proc_frame_length;
        unsigned worst_case = 0;
        float worst_timing = 0.0f;

        double sine_table[(MAX_PROC_FRAME_LENGTH/2) + 1];

        flt_make_sine_table_double(sine_table, proc_frame_length);
        
        for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
            
            complex_s32_t ALIGNED a[MAX_PROC_FRAME_LENGTH/2];
            complex_double_t ALIGNED ref[MAX_PROC_FRAME_LENGTH];
            double ALIGNED ref_real[MAX_PROC_FRAME_LENGTH];

            bfp_complex_s32_t A_fft;
            bfp_s32_t* A;

            conv_error_e error = 0;
            const exponent_t initial_exponent = sext(pseudo_rand_int32(&r), 5);
            right_shift_t shr = pseudo_rand_uint32(&r) % 4;

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

            bfp_init_vect_complex_s32(&A_fft, a, initial_exponent, N/2, 1);

            flt_bit_reverse_indexes_double(ref, proc_frame_length);
            flt_fft_inverse_double(ref, proc_frame_length, sine_table);


            unsigned ts1 = getTimestamp();
            A = bfp_fft_inverse_mono(&A_fft);
            unsigned ts2 = getTimestamp();
            
            float timing = (ts2-ts1)/100.0;
            if(timing > worst_timing) worst_timing = timing;

            for(int i = 0; i < N; i++)
                ref_real[i] = ref[i].re;

            unsigned diff = abs_diff_vect_s32(A->data, A->exp, ref_real, N, &error);
            if(diff > worst_case) { worst_case = diff; /* printf("worst_case: %u\n", worst_case); */ }
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+WIGGLE, diff, "Output delta is too large");
            TEST_ASSERT_CONVERSION(error);
        }
        
#if PRINT_ERRORS
        printf("\tWorst case error (%u-point): %d\n", proc_frame_length, worst_case);
#endif

#if TIME_FUNCS
        printf("\tbfp_fft_inverse_mono (%u-point): %f us\n", proc_frame_length, worst_timing);
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