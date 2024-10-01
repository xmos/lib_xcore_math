// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <math.h>

#include "xmath/xmath.h"
#include "xmath/xs3/vpu_scalar_ops.h"
#include "../../tst_common.h"
#include "unity_fixture.h"




TEST_GROUP_RUNNER(vect_complex_f32_macc) {
  RUN_TEST_CASE(vect_complex_f32_macc, vect_complex_f32_macc);
  RUN_TEST_CASE(vect_complex_f32_macc, vect_complex_f32_conj_macc);
}

TEST_GROUP(vect_complex_f32_macc);
TEST_SETUP(vect_complex_f32_macc) { fflush(stdout); }
TEST_TEAR_DOWN(vect_complex_f32_macc) {}


#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (64)
#else
#  define REPS       (1000)
#  define MAX_LEN    (256)
#endif

// Return the value of 1 LSb of the provided float.
// This can be used for setting error threshold in the test.
static float get_lsb_value(float x)
{
  // To do this, just add 1 LSb to x and subtract x
  unsigned tmp = ((unsigned*)&x)[0] + 1;
  float y = ((float*)&tmp)[0];
  return y - x;
}





TEST(vect_complex_f32_macc, vect_complex_f32_macc)
{
  unsigned seed = SEED_FROM_FUNC_NAME();
  // seed = 0x9F6BD8DD;

  complex_float_t expected[MAX_LEN];

  DWORD_ALIGNED
  complex_float_t a[MAX_LEN];
  DWORD_ALIGNED
  complex_float_t b[MAX_LEN];
  DWORD_ALIGNED
  complex_float_t c[MAX_LEN];

  for(unsigned int v = 0; v < REPS; v++){
    const unsigned old_seed = seed;

    unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN-1);
    setExtraInfo_RSL(v, old_seed, len);
    // printf("0x%08X\n", old_seed);

    for(unsigned int k = 0; k < len; k++){
      a[k].re = ldexpf((float) pseudo_rand_int32(&seed), pseudo_rand_int(&seed, -40, 10));
      a[k].im = ldexpf((float) pseudo_rand_int32(&seed), pseudo_rand_int(&seed, -40, 10));
      b[k].re = ldexpf((float) pseudo_rand_int32(&seed), pseudo_rand_int(&seed, -40, 10));
      b[k].im = ldexpf((float) pseudo_rand_int32(&seed), pseudo_rand_int(&seed, -40, 10));
      c[k].re = ldexpf((float) pseudo_rand_int32(&seed), pseudo_rand_int(&seed, -40, 10));
      c[k].im = ldexpf((float) pseudo_rand_int32(&seed), pseudo_rand_int(&seed, -40, 10));

      double RR = ((double)b[k].re) * ((double)c[k].re);
      double RI = ((double)b[k].re) * ((double)c[k].im);
      double IR = ((double)b[k].im) * ((double)c[k].re);
      double II = ((double)b[k].im) * ((double)c[k].im);

      double ZR = ((double)a[k].re) + RR - II;
      double ZI = ((double)a[k].im) + IR + RI;

      // This avoids pathological cases with large unavoidable errors
      unsigned path = (log2(fabs( II / ZR )) > 4);
      path = path ||  (log2(fabs( IR / ZI )) > 4);
      if( path ){
        k--;
        continue;
      }
      
      expected[k].re = (float) ZR;
      expected[k].im = (float) ZI;

    }
    
    vect_complex_f32_macc(a, b, c, len);

    for(unsigned int k = 0; k < len; k++){

      complex_float_t thresh;
      thresh.re = 16*get_lsb_value(expected[k].re);
      thresh.im = 16*get_lsb_value(expected[k].im);

      TEST_ASSERT_FLOAT_WITHIN_MESSAGE(thresh.re, expected[k].re, a[k].re, "(Real Part)");
      TEST_ASSERT_FLOAT_WITHIN_MESSAGE(thresh.im, expected[k].im, a[k].im, "(Imag Part)");
    }
  }
}





TEST(vect_complex_f32_macc, vect_complex_f32_conj_macc)
{
  unsigned seed = SEED_FROM_FUNC_NAME();
  // seed = 0x9F6BD8DD;

  complex_float_t expected[MAX_LEN];

  DWORD_ALIGNED
  complex_float_t a[MAX_LEN];
  DWORD_ALIGNED
  complex_float_t b[MAX_LEN];
  DWORD_ALIGNED
  complex_float_t c[MAX_LEN];

  for(unsigned int v = 0; v < REPS; v++){
    const unsigned old_seed = seed;

    unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN-1);
    setExtraInfo_RSL(v, old_seed, len);
    // printf("0x%08X\n", old_seed);

    for(unsigned int k = 0; k < len; k++){
      a[k].re = ldexpf((float) pseudo_rand_int32(&seed), pseudo_rand_int(&seed, -40, 10));
      a[k].im = ldexpf((float) pseudo_rand_int32(&seed), pseudo_rand_int(&seed, -40, 10));
      b[k].re = ldexpf((float) pseudo_rand_int32(&seed), pseudo_rand_int(&seed, -40, 10));
      b[k].im = ldexpf((float) pseudo_rand_int32(&seed), pseudo_rand_int(&seed, -40, 10));
      c[k].re = ldexpf((float) pseudo_rand_int32(&seed), pseudo_rand_int(&seed, -40, 10));
      c[k].im = ldexpf((float) pseudo_rand_int32(&seed), pseudo_rand_int(&seed, -40, 10));

      double RR = ((double)b[k].re) * ((double)c[k].re);
      double RI = ((double)b[k].re) * ((double)c[k].im);
      double IR = ((double)b[k].im) * ((double)c[k].re);
      double II = ((double)b[k].im) * ((double)c[k].im);

      double ZR = ((double)a[k].re) + RR + II;
      double ZI = ((double)a[k].im) + IR - RI;

      // This avoids pathological cases with large unavoidable errors
      unsigned path = (log2(fabs( II / ZR )) > 4);
      path = path ||  (log2(fabs( IR / ZI )) > 4);
      if( path ){
        k--;
        continue;
      }
      
      expected[k].re = (float) ZR;
      expected[k].im = (float) ZI;

    }
    
    vect_complex_f32_conj_macc(a, b, c, len);

    for(unsigned int k = 0; k < len; k++){

      complex_float_t thresh;
      thresh.re = 16*get_lsb_value(expected[k].re);
      thresh.im = 16*get_lsb_value(expected[k].im);

      TEST_ASSERT_FLOAT_WITHIN_MESSAGE(thresh.re, expected[k].re, a[k].re, "(Real Part)");
      TEST_ASSERT_FLOAT_WITHIN_MESSAGE(thresh.im, expected[k].im, a[k].im, "(Imag Part)");
    }
  }
}



