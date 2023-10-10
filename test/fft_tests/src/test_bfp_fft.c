// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include "xmath/xmath.h"
#include "testing.h"
#include "floating_fft.h"
#include "tst_common.h"
#include "fft.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(bfp_fft) {
  RUN_TEST_CASE(bfp_fft, bfp_fft_forward_complex);
  RUN_TEST_CASE(bfp_fft, bfp_fft_inverse_complex);
  RUN_TEST_CASE(bfp_fft, bfp_fft_forward_stereo);
  RUN_TEST_CASE(bfp_fft, bfp_fft_inverse_stereo);
  RUN_TEST_CASE(bfp_fft, bfp_fft_forward_mono);
  RUN_TEST_CASE(bfp_fft, bfp_fft_inverse_mono);
}

TEST_GROUP(bfp_fft);
TEST_SETUP(bfp_fft) { fflush(stdout); }
TEST_TEAR_DOWN(bfp_fft) {}


#define MAX_PROC_FRAME_LENGTH_LOG2 10
#define MAX_PROC_FRAME_LENGTH (1<<MAX_PROC_FRAME_LENGTH_LOG2)

#define EXPONENT_SIZE   3
#define EXP_BOUND_LOW   (1-(1<<EXPONENT_SIZE))
#define EXP_BOUND_HIGH  (1<<EXPONENT_SIZE)
#define MAX_HEADROOM 5
#define WIGGLE 20

#define MIN_FFT_N_LOG2  (4)

#if SMOKE_TEST
#  define LOOPS_LOG2       (2)
#else
#  define LOOPS_LOG2       (8)
#endif

TEST(bfp_fft, bfp_fft_forward_complex)
{
#define FUNC_NAME "bfp_fft_forward_complex"

#if PRINT_FUNC_NAMES
    printf("\n%s..\n", FUNC_NAME);
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

            float timing = (float) ((ts2-ts1)/100.0);
            if(timing > worst_timing) worst_timing = timing;

            unsigned diff = abs_diff_vect_complex_s32(A.data, A.exp, ref, FFT_N, &error);
            if(diff > worst_error) worst_error = diff;
            TEST_ASSERT_CONVERSION(error);
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+WIGGLE, diff, "Output delta is too large");
        }

#if PRINT_ERRORS
        printf("    %s worst error (%u-point): %u\n", FUNC_NAME, FFT_N, worst_error);
#endif

#if TIME_FUNCS
        printf("    %s (%u-point): %f us\n", FUNC_NAME, FFT_N, worst_timing);
#endif

#if WRITE_PERFORMANCE_INFO
        fprintf(perf_file, "%s, %u, %u, %0.02f,\n", &(FUNC_NAME[5]), FFT_N, worst_error, worst_timing);
#endif
    }

#undef FUNC_NAME
}


TEST(bfp_fft, bfp_fft_inverse_complex)
{
#define FUNC_NAME "bfp_fft_inverse_complex"

#if PRINT_FUNC_NAMES
    printf("\n%s..\n", FUNC_NAME);
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

            float timing = (float) ((ts2-ts1)/100.0);
            if(timing > worst_timing) worst_timing = timing;

            unsigned diff = abs_diff_vect_complex_s32(A.data, A.exp, ref, FFT_N, &error);
            if(diff > worst_error) worst_error = diff;
            TEST_ASSERT_CONVERSION(error);
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+WIGGLE, diff, "Output delta is too large");
        }

#if PRINT_ERRORS
        printf("    %s worst error (%u-point): %u\n", FUNC_NAME, FFT_N, worst_error);
#endif

#if TIME_FUNCS
        printf("    %s (%u-point): %f us\n", FUNC_NAME, FFT_N, worst_timing);
#endif

#if WRITE_PERFORMANCE_INFO
        fprintf(perf_file, "%s, %u, %u, %0.02f,\n", &(FUNC_NAME[5]), FFT_N, worst_error, worst_timing);
#endif
    }

#undef FUNC_NAME
}


TEST(bfp_fft, bfp_fft_forward_stereo)
{
#define FUNC_NAME "bfp_fft_forward_stereo"

#if PRINT_FUNC_NAMES
    printf("\n%s..\n", FUNC_NAME);
#endif

    unsigned r = 1;

    conv_error_e error = 0;

    for(unsigned k = MIN_FFT_N_LOG2; k <= MAX_PROC_FRAME_LENGTH_LOG2; k++){
        unsigned FFT_N = (1<<k);
        unsigned worst_error = 0;
        float worst_timing = 0.0f;

        double sine_table[(MAX_PROC_FRAME_LENGTH/2) + 1];

        flt_make_sine_table_double(sine_table, FFT_N);

        for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){

            // Buffers for the BFP vectors (both time and freq domain)
            int32_t DWORD_ALIGNED chanA_buff[MAX_PROC_FRAME_LENGTH];
            int32_t DWORD_ALIGNED chanB_buff[MAX_PROC_FRAME_LENGTH];

            // Scratch buffer required for stereo FFT
            complex_s32_t DWORD_ALIGNED scratch[MAX_PROC_FRAME_LENGTH];

            // Floating point reference vector
            complex_double_t DWORD_ALIGNED ref[MAX_PROC_FRAME_LENGTH];

            // Choose random time-domain exponents for each channel
            exponent_t initial_exponentA = pseudo_rand_int(&r, EXP_BOUND_LOW, EXP_BOUND_HIGH);
            exponent_t initial_exponentB = pseudo_rand_int(&r, EXP_BOUND_LOW, EXP_BOUND_HIGH);

            // Choose random headroom for each channel (not guaranteed -- will be recomputed after)
            right_shift_t a_shr = pseudo_rand_uint(&r, 0, MAX_HEADROOM+1);
            right_shift_t b_shr = pseudo_rand_uint(&r, 0, MAX_HEADROOM+1);

            // Fill buffers with random data
            for(unsigned i = 0; i < FFT_N; i++){
                chanA_buff[i] = pseudo_rand_int32(&r) >> a_shr;
                chanB_buff[i] = pseudo_rand_int32(&r) >> b_shr;

                ref[i].re = ldexp(chanA_buff[i], initial_exponentA);
                ref[i].im = ldexp(chanB_buff[i], initial_exponentB);
            }

            // time-domain BFP vectors
            bfp_s32_t chanA_td;
            bfp_s32_t chanB_td;

            // Initialize BFP vectors (and recalc HR)
            bfp_s32_init(&chanA_td, chanA_buff, initial_exponentA, FFT_N, 1);
            bfp_s32_init(&chanB_td, chanB_buff, initial_exponentB, FFT_N, 1);

            // FFT using float reference implementation
            flt_bit_reverse_indexes_double(ref, FFT_N);
            flt_fft_forward_double(ref, FFT_N, sine_table);
            flt_fft_split_spectrum_double(ref, FFT_N);

            // Perform forward FFT and timestamp it
            unsigned ts1 = getTimestamp();
            bfp_fft_forward_stereo(&chanA_td, &chanB_td, scratch);
            unsigned ts2 = getTimestamp();

            // freq domain BFP vectors (aliased from time-domain vectors)
            bfp_complex_s32_t* chanA_fd = (bfp_complex_s32_t*) &chanA_td;
            bfp_complex_s32_t* chanB_fd = (bfp_complex_s32_t*) &chanB_td;

            // Check for accuracy/correctness
            TEST_ASSERT_EQUAL(FFT_N/2, chanA_fd->length);
            TEST_ASSERT_EQUAL(FFT_N/2, chanB_fd->length);

            // headroom is expected to be the minimum of the two channels
            headroom_t exp_hr = MIN(vect_complex_s32_headroom(chanA_fd->data, chanA_fd->length),
                                    vect_complex_s32_headroom(chanB_fd->data, chanB_fd->length));

            TEST_ASSERT_EQUAL(exp_hr, chanA_fd->hr);
            TEST_ASSERT_EQUAL(exp_hr, chanB_fd->hr);

            unsigned diffA = abs_diff_vect_complex_s32( chanA_fd->data, chanA_fd->exp, &ref[0], FFT_N/2, &error);
            unsigned diffB = abs_diff_vect_complex_s32( chanB_fd->data, chanB_fd->exp, &ref[FFT_N/2], FFT_N/2, &error);

            TEST_ASSERT_CONVERSION(error);
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+WIGGLE, diffA, "Output delta is too large (chanA)");
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+WIGGLE, diffB, "Output delta is too large (chanB)");

            // Update worst-case timing and error info
            float timing = (float) ((ts2-ts1)/100.0);
            worst_timing = MAX(worst_timing, timing);

            worst_error = MAX(worst_error, diffA);
            worst_error = MAX(worst_error, diffB);
        }

#if PRINT_ERRORS
        printf("    %s worst error (%u-point): %u\n", FUNC_NAME, FFT_N, worst_error);
#endif

#if TIME_FUNCS
        printf("    %s (%u-point): %f us\n", FUNC_NAME, FFT_N, worst_timing);
#endif

#if WRITE_PERFORMANCE_INFO
        fprintf(perf_file, "%s, %u, %u, %0.02f,\n", &(FUNC_NAME[5]), FFT_N, worst_error, worst_timing);
#endif
    }

#undef FUNC_NAME
}


TEST(bfp_fft, bfp_fft_inverse_stereo)
{
#define FUNC_NAME "bfp_fft_inverse_stereo"

#if PRINT_FUNC_NAMES
    printf("\n%s..\n", FUNC_NAME);
#endif

    unsigned r = 1;
    conv_error_e error = 0;

    for(unsigned k = MIN_FFT_N_LOG2; k <= MAX_PROC_FRAME_LENGTH_LOG2; k++){
        unsigned FFT_N = (1<<k);
        unsigned worst_error = 0;
        float worst_timing = 0.0f;

        double sine_table[(MAX_PROC_FRAME_LENGTH/2) + 1];

        flt_make_sine_table_double(sine_table, FFT_N);

        for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){

            // Buffers for the BFP vectors (both time and freq domain)
            complex_s32_t DWORD_ALIGNED a_data[MAX_PROC_FRAME_LENGTH/2];
            complex_s32_t DWORD_ALIGNED b_data[MAX_PROC_FRAME_LENGTH/2];

            // Floating point reference vector
            complex_double_t DWORD_ALIGNED ref[MAX_PROC_FRAME_LENGTH];

            // Scratch buffer required for stereo FFT
            complex_s32_t DWORD_ALIGNED scratch[MAX_PROC_FRAME_LENGTH];

            // Choose random time-domain exponents for each channel
            exponent_t initial_exponentA = pseudo_rand_int(&r, EXP_BOUND_LOW, EXP_BOUND_HIGH);
            exponent_t initial_exponentB = pseudo_rand_int(&r, EXP_BOUND_LOW, EXP_BOUND_HIGH);

            // Choose random headroom for each channel (not guaranteed -- will be recomputed after)
            right_shift_t a_shr = pseudo_rand_uint(&r, 0, MAX_HEADROOM+1);
            right_shift_t b_shr = pseudo_rand_uint(&r, 0, MAX_HEADROOM+1);

            // Fill buffers with random data
            for(unsigned i = 0; i < FFT_N/2; i++){
                a_data[i].re = pseudo_rand_int32(&r) >> a_shr;
                a_data[i].im = pseudo_rand_int32(&r) >> a_shr;

                b_data[i].re = pseudo_rand_int32(&r) >> b_shr;
                b_data[i].im = pseudo_rand_int32(&r) >> b_shr;

                ref[i].re = ldexp(a_data[i].re, initial_exponentA);
                ref[i].im = ldexp(a_data[i].im, initial_exponentA);

                ref[(FFT_N/2)+i].re = ldexp(b_data[i].re, initial_exponentB);
                ref[(FFT_N/2)+i].im = ldexp(b_data[i].im, initial_exponentB);
            }

            // Freq domain BFP vectors
            bfp_complex_s32_t A_fft;
            bfp_complex_s32_t B_fft;

            // Initialize BFP vectors (and recalc HR)
            bfp_complex_s32_init(&A_fft, a_data, initial_exponentA, FFT_N/2, 1);
            bfp_complex_s32_init(&B_fft, b_data, initial_exponentB, FFT_N/2, 1);

            // IFFT using float reference implementation
            flt_fft_merge_spectra_double(ref, FFT_N);
            flt_bit_reverse_indexes_double(ref, FFT_N);
            flt_fft_inverse_double(ref, FFT_N, sine_table);

            // Perform inverse FFT and timestamp it
            unsigned ts1 = getTimestamp();
            bfp_fft_inverse_stereo(&A_fft, &B_fft, scratch);
            unsigned ts2 = getTimestamp();

            // Time domain BFP vectors (aliased from freq domain)
            bfp_s32_t* A = (bfp_s32_t*) &A_fft;
            bfp_s32_t* B = (bfp_s32_t*) &B_fft;

            //Split ref into two channels
            #ifndef _WIN32
              double refA[FFT_N];
              double refB[FFT_N];
            #else
              // On Windows all the arrays sizes must be known at compile time,
              // otherwise they must be dynamically allocated at runtime.
              double * refA = (double *) XMATH_MALLOC(FFT_N * sizeof(double));
              double * refB  = (double *) XMATH_MALLOC(FFT_N * sizeof(double));
            #endif
            for(unsigned int i = 0; i < FFT_N; i++){
              refA[i] = ref[i].re;
              refB[i] = ref[i].im;
            }

            // Check for accuracy/correctness
            TEST_ASSERT_EQUAL(FFT_N, A->length);
            TEST_ASSERT_EQUAL(FFT_N, B->length);

            // headroom is expected to be the minimum of the two channels
            headroom_t exp_hr = MIN(vect_s32_headroom(A->data, A->length),
                                    vect_s32_headroom(B->data, B->length));

            TEST_ASSERT_EQUAL(exp_hr, A->hr);
            TEST_ASSERT_EQUAL(exp_hr, B->hr);

            unsigned diffA = abs_diff_vect_s32(A->data, A->exp, refA, FFT_N, &error);
            unsigned diffB = abs_diff_vect_s32(B->data, B->exp, refB, FFT_N, &error);

            TEST_ASSERT_CONVERSION(error);
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+WIGGLE, diffA, "Output delta is too large (chanA)");
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+WIGGLE, diffB, "Output delta is too large (chanB)");

            // Update worst-case timing and error info
            float timing = (float) ((ts2-ts1)/100.0);
            worst_timing = MAX(worst_timing, timing);

            worst_error = MAX(worst_error, diffA);
            worst_error = MAX(worst_error, diffB);

            #ifdef _WIN32
              XMATH_FREE(refA);
              XMATH_FREE(refB);
            #endif
        }

#if PRINT_ERRORS
        printf("    %s worst error (%u-point): %u\n", FUNC_NAME, FFT_N, worst_error);
#endif

#if TIME_FUNCS
        printf("    %s (%u-point): %f us\n", FUNC_NAME, FFT_N, worst_timing);
#endif

#if WRITE_PERFORMANCE_INFO
        fprintf(perf_file, "%s, %u, %u, %0.02f,\n", &(FUNC_NAME[5]), FFT_N, worst_error, worst_timing);
#endif
    }

#undef FUNC_NAME
}


TEST(bfp_fft, bfp_fft_forward_mono)
{
#define FUNC_NAME "bfp_fft_forward_mono"

#if PRINT_FUNC_NAMES
    printf("\n%s..\n", FUNC_NAME);
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

            float timing = (float) ((ts2-ts1)/100.0);
            if(timing > worst_timing) worst_timing = timing;


            unsigned diff = abs_diff_vect_complex_s32(A_fft->data, A_fft->exp, ref, A_fft->length, &error);
            if(diff > worst_error) worst_error = diff;
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+WIGGLE, diff, "Output delta is too large");
            TEST_ASSERT_CONVERSION(error);
            TEST_ASSERT_EQUAL(FFT_N/2, A_fft->length);
        }

#if PRINT_ERRORS
        printf("    %s worst error (%u-point): %u\n", FUNC_NAME, FFT_N, worst_error);
#endif

#if TIME_FUNCS
        printf("    %s (%u-point): %f us\n", FUNC_NAME, FFT_N, worst_timing);
#endif

#if WRITE_PERFORMANCE_INFO
        fprintf(perf_file, "%s, %u, %u, %0.02f,\n", &(FUNC_NAME[5]), FFT_N, worst_error, worst_timing);
#endif
    }

#undef FUNC_NAME
}


TEST(bfp_fft, bfp_fft_inverse_mono)
{
#define FUNC_NAME "bfp_fft_inverse_mono"

#if PRINT_FUNC_NAMES
    printf("\n%s..\n", FUNC_NAME);
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

            float timing = (float) ((ts2-ts1)/100.0);
            if(timing > worst_timing) worst_timing = timing;

            for(unsigned int i = 0; i < N; i++)
                ref_real[i] = ref[i].re;

            unsigned diff = abs_diff_vect_s32(A->data, A->exp, ref_real, N, &error);
            if(diff > worst_error) worst_error = diff;
            TEST_ASSERT_LESS_OR_EQUAL_UINT32_MESSAGE(k+WIGGLE, diff, "Output delta is too large");
            TEST_ASSERT_CONVERSION(error);
            TEST_ASSERT_EQUAL(FFT_N, A->length);
        }

#if PRINT_ERRORS
        printf("    %s worst error (%u-point): %u\n", FUNC_NAME, FFT_N, worst_error);
#endif

#if TIME_FUNCS
        printf("    %s (%u-point): %f us\n", FUNC_NAME, FFT_N, worst_timing);
#endif

#if WRITE_PERFORMANCE_INFO
        fprintf(perf_file, "%s, %u, %u, %0.02f,\n", &(FUNC_NAME[5]), FFT_N, worst_error, worst_timing);
#endif
    }

#undef FUNC_NAME
}
