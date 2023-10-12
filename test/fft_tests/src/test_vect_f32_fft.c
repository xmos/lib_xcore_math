// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include "xmath/xmath.h"
#include "testing.h"
#include "floating_fft.h"
#include "tst_common.h"
#include "fft.h"
#include "unity_fixture.h"

#include <math.h>


TEST_GROUP_RUNNER(vect_f32_fft) {
  RUN_TEST_CASE(vect_f32_fft, fft_f32_forward);
  RUN_TEST_CASE(vect_f32_fft, fft_f32_inverse);
}

TEST_GROUP(vect_f32_fft);
TEST_SETUP(vect_f32_fft) { fflush(stdout); }
TEST_TEAR_DOWN(vect_f32_fft) {}


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


TEST(vect_f32_fft, fft_f32_forward)
{
#define FUNC_NAME "fft_f32_forward"

#if PRINT_FUNC_NAMES
  printf("\n%s..\n", FUNC_NAME);
#endif

  unsigned r = 1;

  for(unsigned k = MAX(MIN_FFT_N_LOG2, 4); k <= MAX_PROC_FRAME_LENGTH_LOG2; k++){

    unsigned FFT_N = (1<<k);
    float worst_timing = 0.0f;

    double sine_table[(MAX_PROC_FRAME_LENGTH/2) + 1];

    flt_make_sine_table_double(sine_table, FFT_N);
    
    for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
    
      DWORD_ALIGNED
      float a[MAX_PROC_FRAME_LENGTH];
      DWORD_ALIGNED
      complex_double_t ref[MAX_PROC_FRAME_LENGTH];

      const exponent_t initial_exponent = sext(pseudo_rand_int32(&r), EXPONENT_SIZE);

      for(unsigned i = 0; i < FFT_N; i++){
        a[i] = ldexpf((float) pseudo_rand_int32(&r), initial_exponent);
        ref[i].re = a[i];
        ref[i].im = 0;
      }

      flt_bit_reverse_indexes_double(ref, FFT_N);
      flt_fft_forward_double(ref, FFT_N, sine_table);
      ref[0].im = ref[FFT_N/2].re;

      unsigned ts1 = getTimestamp();
      complex_float_t* a_fft = fft_f32_forward(&a[0], FFT_N);
      unsigned ts2 = getTimestamp();
      
      float timing = (float) ((ts2-ts1)/100.0);
      if(timing > worst_timing) worst_timing = timing;

      // Finding the max element of each and making sure they're similar
      // establishes a scale for the spectrum.
      double a_max = 0.0f;
      double ref_max = 0.0;

      for(unsigned int k = 0; k < FFT_N/2; k++){
        double a_mag = a_fft[k].re * a_fft[k].re + a_fft[k].im * a_fft[k].im;
        double ref_mag = ref[k].re * ref[k].re + ref[k].im * ref[k].im;

        a_max = (a_mag > a_max)? a_mag : a_max;
        ref_max = (ref_mag > ref_max)? ref_mag : ref_max;
      }

      double ppo = fabs(ref_max - a_max)/ref_max;

      TEST_ASSERT( ppo < ldexp(1, -14));

      double max_diff = ldexp(1, -17) * ref_max;

      for(unsigned int k = 0; k < FFT_N/2; k++){
        TEST_ASSERT( fabs(ref[k].re - a_fft[k].re) <= max_diff );
        TEST_ASSERT( fabs(ref[k].im - a_fft[k].im) <= max_diff );
      }
    }

#if TIME_FUNCS
    printf("    %s (%u-point): %f us\n", FUNC_NAME, FFT_N, worst_timing);
#endif
  }

#undef FUNC_NAME
}





TEST(vect_f32_fft, fft_f32_inverse)
{
#define FUNC_NAME "fft_f32_inverse"

#if PRINT_FUNC_NAMES
  printf("\n%s..\n", FUNC_NAME);
#endif

  unsigned r = 1;

  for(unsigned k = MAX(MIN_FFT_N_LOG2, 4); k <= MAX_PROC_FRAME_LENGTH_LOG2; k++){

    unsigned FFT_N = (1<<k);
    unsigned N = FFT_N;
    float worst_timing = 0.0f;

    double sine_table[(MAX_PROC_FRAME_LENGTH/2) + 1];

    flt_make_sine_table_double(sine_table, FFT_N);
    
    for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
      
      DWORD_ALIGNED
      complex_float_t a_fft[MAX_PROC_FRAME_LENGTH/2];
      DWORD_ALIGNED
      complex_double_t ref[MAX_PROC_FRAME_LENGTH];
      DWORD_ALIGNED
      double ref_real[MAX_PROC_FRAME_LENGTH];
      
      const exponent_t initial_exponent = sext(pseudo_rand_int32(&r), EXPONENT_SIZE);
      
      for(unsigned i = 0; i < N/2; i++){
        ref[i].re = a_fft[i].re = ldexpf((float) pseudo_rand_int32(&r), initial_exponent);
        ref[i].im = a_fft[i].im = ldexpf((float) pseudo_rand_int32(&r), initial_exponent);
        
        if(i){
          ref[N-i].re =  ref[i].re;
          ref[N-i].im = -ref[i].im;
        }
      }
      
      ref[N/2].re = ref[0].im;
      ref[N/2].im = ref[0].im = 0;

      flt_bit_reverse_indexes_double(ref, FFT_N);
      flt_fft_inverse_double(ref, FFT_N, sine_table);

      unsigned ts1 = getTimestamp();
      float* a = fft_f32_inverse(&a_fft[0], FFT_N);
      unsigned ts2 = getTimestamp();
      
      float timing = (float) ((ts2-ts1)/100.0);
      if(timing > worst_timing) worst_timing = timing;

      for(unsigned int i = 0; i < N; i++)
        ref_real[i] = ref[i].re;

      
      double a_max = 0.0f;
      double ref_max = 0.0;

      for(unsigned int k = 0; k < FFT_N; k++){
        double AA = fabs(a[k]);
        double BB = fabs(ref_real[k]);

        a_max = (AA > a_max)? AA : a_max;
        ref_max = (BB > ref_max)? BB : ref_max;
      }

      double ppo = fabs(ref_max - a_max)/ref_max;

      TEST_ASSERT( ppo < ldexp(1, -14));

      double max_diff = ldexp(1, -17) * ref_max;

      for(unsigned int k = 0; k < FFT_N; k++){
        TEST_ASSERT( fabs(ref_real[k] - a[k]) <= max_diff );
      }
    }

#if TIME_FUNCS
    printf("    %s (%u-point): %f us\n", FUNC_NAME, FFT_N, worst_timing);
#endif
  }

#undef FUNC_NAME
}
