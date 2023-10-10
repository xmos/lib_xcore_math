// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdarg.h>
#include "xmath/xmath.h"
#include "../tst_common.h"
#include "xmath/xs3/vpu_scalar_ops.h"
#include "unity_fixture.h"


TEST_GROUP_RUNNER(vect_log) {
  RUN_TEST_CASE(vect_log, chunk_s32_log_correctness);
  RUN_TEST_CASE(vect_log, chunk_s32_log);
  RUN_TEST_CASE(vect_log, chunk_float_s32_log_CORRECTNESS);
  RUN_TEST_CASE(vect_log, chunk_float_s32_log_RANDOM);
  RUN_TEST_CASE(vect_log, vect_float_s32_log);
  RUN_TEST_CASE(vect_log, vect_s32_log);
}

TEST_GROUP(vect_log);
TEST_SETUP(vect_log) { fflush(stdout); }
TEST_TEAR_DOWN(vect_log) {}

#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (64)
#else
#  define REPS       (1000)
#  define MAX_LEN    (256)
#endif




#define COUNT       (4)

TEST(vect_log, chunk_float_s32_log_CORRECTNESS)
{

  // If it gets these correct it will probably get everything else correct.
  float_s32_t DWORD_ALIGNED B[VPU_INT32_EPV] = {
    { Q28(0.0), -28  },
    { Q28(1.0), -28  },
    { Q28(M_E), -28  },
    { Q28((M_E * M_E)), -28  },
    { Q24(0.0), -24  },
    { Q24(1.0), -24  },
    { Q24(M_E), -24  },
    { Q24((M_E * M_E)), -24  },
  };

  q8_24 DWORD_ALIGNED A[VPU_INT32_EPV];

  q8_24 expected[VPU_INT32_EPV] = {
    INT32_MIN,           // ln(0.0) is defined here to yield INT32_MIN
    Q24(0.0),             // ln(1.0) = 0.0
    Q24(1.0),             // ln(e) = 1.0
    Q24(2.0),             // ln(e^2) = 2.0
    INT32_MIN,
    Q24(0.0),
    Q24(1.0),
    Q24(2.0),
  };

  chunk_float_s32_log(A, B);

  TEST_ASSERT_INT32_WITHIN(0,     expected[0], A[0]);
  TEST_ASSERT_INT32_WITHIN(35000, expected[1], A[1]);
  TEST_ASSERT_INT32_WITHIN(2000,  expected[2], A[2]);
  TEST_ASSERT_INT32_WITHIN(10,    expected[3], A[3]);

  TEST_ASSERT_INT32_WITHIN(0,     expected[4], A[4]);
  TEST_ASSERT_INT32_WITHIN(35000, expected[5], A[5]);
  TEST_ASSERT_INT32_WITHIN(2000,  expected[6], A[6]);
  TEST_ASSERT_INT32_WITHIN(10,    expected[7], A[7]);
}

#undef COUNT







TEST(vect_log, chunk_float_s32_log_RANDOM)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  float_s32_t DWORD_ALIGNED B[VPU_INT32_EPV];
  q8_24 DWORD_ALIGNED A[VPU_INT32_EPV];

  q8_24 expected[VPU_INT32_EPV];

  int max_max_error = 0;

  for(unsigned int v = 0; v < REPS; v++){
    setExtraInfo_RS(v, seed);

    unsigned length = 8;

    for(unsigned int i = 0; i < length; i++){
      B[i].exp = pseudo_rand_int(&seed, -30, 30);
      B[i].mant = pseudo_rand_uint(&seed, 0, INT32_MAX);
    }

    for(unsigned int i = 0; i < length; i++){
      double bi = ldexp(B[i].mant, B[i].exp);
      double exp_dbl = log(bi);
      expected[i] = lround(ldexp(exp_dbl, 24));
    }

    // volatile uint32_t t0 = get_reference_time();
    chunk_float_s32_log(A, B);
    // volatile uint32_t t1 = get_reference_time();

    // uint32_t delta_ticks = t1 - t0;
    // float delta_us = (float) (delta_ticks / 100.0);
    // float delta_us_per_elm = delta_us / length;

    int max_error = 0;
    for(unsigned int i = 0; i < length; i++){
      int32_t error = abs(expected[i] - A[i]);
      max_error = (error > max_error)? error : max_error;

      TEST_ASSERT_INT32_WITHIN(70000, expected[i], A[i]);
    }

    // printf("Len: % 3u;  time: % 7.02f us;      time/elm:  % 7.02f us;   max error: %u\n",
    //     length, delta_us, delta_us_per_elm, max_error);

    max_max_error = (max_error > max_max_error)? max_error : max_max_error;
  }

  // printf("Max max error: %u\n", max_max_error);
}
 






#define COUNT       (4)

TEST(vect_log, chunk_s32_log_correctness)
{
  // If it gets these correct it will probably get everything else correct.

  exponent_t b_exp = -28;
  int32_t DWORD_ALIGNED B[VPU_INT32_EPV] = {
    Q28(0.0),
    Q28(1.0),
    Q28(M_E),
    Q28((M_E * M_E)),
    0,0,0,0,
  };

  q8_24 DWORD_ALIGNED A[VPU_INT32_EPV];

  q8_24 expected[VPU_INT32_EPV] = {
    INT32_MIN,           // ln(0.0) is defined here to yield INT32_MIN
    Q24(0.0),             // ln(1.0) = 0.0
    Q24(1.0),             // ln(e) = 1.0
    Q24(2.0),             // ln(e^2) = 2.0
    0,0,0,0               // don't care.
  };

  chunk_s32_log(A, B, b_exp);

  TEST_ASSERT_INT32_WITHIN(0,     expected[0], A[0]);
  TEST_ASSERT_INT32_WITHIN(35000, expected[1], A[1]); //powers of 2 are basically a worst case scenario
  TEST_ASSERT_INT32_WITHIN(2000,  expected[2], A[2]);
  TEST_ASSERT_INT32_WITHIN(10,    expected[3], A[3]);
}

#undef COUNT





TEST(vect_log, chunk_s32_log)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  // printf("\n");


  int32_t DWORD_ALIGNED B[VPU_INT32_EPV];
  q8_24 DWORD_ALIGNED A[VPU_INT32_EPV];

  q8_24 expected[VPU_INT32_EPV];

  int max_max_error = 0;

  for(unsigned int v = 0; v < REPS; v++){
    setExtraInfo_RS(v, seed);

    unsigned length = 8;

    const exponent_t b_exp = pseudo_rand_int(&seed, -30, 30);
    headroom_t b_hr = pseudo_rand_uint(&seed, 0, 5);

    for(unsigned int i = 0; i < length; i++){
      B[i] = pseudo_rand_uint(&seed, 0, INT32_MAX) >> b_hr;
    }

    for(unsigned int i = 0; i < length; i++){
      double bi = ldexp(B[i], b_exp);
      double exp_dbl = log(bi);

      if( B[i] == 0 ) expected[i] = -INT32_MAX;
      else            expected[i] = lround(ldexp(exp_dbl, 24));
    }

    // volatile uint32_t t0 = get_reference_time();
    chunk_s32_log(A, B, b_exp);
    // volatile uint32_t t1 = get_reference_time();

    // uint32_t delta_ticks = t1 - t0;
    // float delta_us = (float) (delta_ticks / 100.0);
    // float delta_us_per_elm = delta_us / length;

    int max_error = 0;
    for(unsigned int i = 0; i < length; i++){
      int32_t error = abs(expected[i] - A[i]);
      max_error = (error > max_error)? error : max_error;

      TEST_ASSERT_INT32_WITHIN(70000, expected[i], A[i]);
    }

    // printf("Len: % 3u;  time: % 7.02f us;      time/elm:  % 7.02f us;   max error: %u\n",
    //     length, delta_us, delta_us_per_elm, max_error);

    max_max_error = (max_error > max_max_error)? max_error : max_max_error;
  }

  // printf("Max max error: %u\n", max_max_error);
}







TEST(vect_log, vect_s32_log)
{
  unsigned seed = SEED_FROM_FUNC_NAME();
  seed = 0x1DA79E33;
  // printf("\n");


  int32_t DWORD_ALIGNED B[MAX_LEN];
  q8_24 DWORD_ALIGNED A[MAX_LEN];

  q8_24 expected[MAX_LEN];

  int max_max_error = 0;

  for(unsigned int v = 0; v < REPS; v++){
    setExtraInfo_RS(v, seed);

    unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);

    const unsigned output_base = pseudo_rand_uint(&seed, 2, 11);

    const double inv_ln_output_base  = 1.0 / log(output_base);
    const q2_30 inv_ln_output_base_q30 = Q30( inv_ln_output_base );

    const exponent_t b_exp = pseudo_rand_int(&seed, -30, 30);
    headroom_t b_hr = pseudo_rand_uint(&seed, 0, 5);

    for(unsigned int i = 0; i < length; i++){
      B[i] = pseudo_rand_uint(&seed, 0, INT32_MAX) >> b_hr;
    }

    for(unsigned int i = 0; i < length; i++){
      double bi = ldexp(B[i], b_exp);
      double exp_dbl;
      
      switch(output_base){
        case 1:   exp_dbl = log(bi);    break;
        case 2:   exp_dbl = log2(bi);   break;
        case 10:  exp_dbl = log10(bi);  break;
        default:  exp_dbl = log(bi) * inv_ln_output_base;
      }

      expected[i] = lround(ldexp(exp_dbl, 24));
    }

    // volatile uint32_t t0;
    // volatile uint32_t t1;
    switch(output_base){
      case 1:
        // t0 = get_reference_time();
        vect_s32_log(A, B, b_exp, length);
        // t1 = get_reference_time();
        break;
      case 2:
        // t0 = get_reference_time();
        vect_s32_log2(A, B, b_exp, length);
        // t1 = get_reference_time();
        break;
      case 10:
        // t0 = get_reference_time();
        vect_s32_log10(A, B, b_exp, length);
        // t1 = get_reference_time();
        break;
      default:
        // t0 = get_reference_time();
        vect_s32_log_base(A, B, b_exp, inv_ln_output_base_q30, length);
        // t1 = get_reference_time();
    }

    // uint32_t delta_ticks = t1 - t0;
    // float delta_us = (float) (delta_ticks / 100.0);
    // float delta_us_per_elm = delta_us / length;

    int max_error = 0;
    for(unsigned int i = 0; i < length; i++){
      int32_t error = abs(expected[i] - A[i]);
      max_error = (error > max_error)? error : max_error;

      TEST_ASSERT_INT32_WITHIN_MESSAGE(700000, expected[i], A[i], "");
    }

    // printf("Len: % 3u;  time: % 7.02f us;      time/elm:  % 7.02f us;   max error: %u\n",
    //     length, delta_us, delta_us_per_elm, max_error);

    max_max_error = (max_error > max_max_error)? max_error : max_max_error;
  }

  // printf("Max max error: %u\n", max_max_error);
}





TEST(vect_log, vect_float_s32_log)
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  // printf("\n");


  float_s32_t DWORD_ALIGNED B[MAX_LEN];
  q8_24 DWORD_ALIGNED A[MAX_LEN];

  q8_24 expected[MAX_LEN];

  int max_max_error = 0;

  for(unsigned int v = 0; v < REPS; v++){
    setExtraInfo_RS(v, seed);

    unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);

    const unsigned output_base = pseudo_rand_uint(&seed, 2, 11);

    const double inv_ln_output_base  = 1.0 / log(output_base);
    const q2_30 inv_ln_output_base_q30 = Q30( inv_ln_output_base );

    for(unsigned int i = 0; i < length; i++){
      B[i].exp = pseudo_rand_int(&seed, -30, 30);
      B[i].mant = pseudo_rand_uint(&seed, 0, INT32_MAX);
    }

    for(unsigned int i = 0; i < length; i++){
      double bi = ldexp(B[i].mant, B[i].exp);
      double exp_dbl;
      
      switch(output_base){
        case 1:   exp_dbl = log(bi);   break;
        case 2:   exp_dbl = log2(bi);   break;
        case 10:  exp_dbl = log10(bi);  break;
        default:  exp_dbl = log(bi) * inv_ln_output_base;
      }

      expected[i] = lround(ldexp(exp_dbl, 24));
    }

    // volatile uint32_t t0;
    // volatile uint32_t t1;
    switch(output_base){
      case 1:
        // t0 = get_reference_time();
        vect_float_s32_log(A, B, length);
        // t1 = get_reference_time();
        break;
      case 2:
        // t0 = get_reference_time();
        vect_float_s32_log2(A, B, length);
        // t1 = get_reference_time();
        break;
      case 10:
        // t0 = get_reference_time();
        vect_float_s32_log10(A, B, length);
        // t1 = get_reference_time();
        break;
      default:
        // t0 = get_reference_time();
        vect_float_s32_log_base(A, B, inv_ln_output_base_q30, length);
        // t1 = get_reference_time();
    }

    // uint32_t delta_ticks = t1 - t0;
    // float delta_us = (float) (delta_ticks / 100.0);
    // float delta_us_per_elm = delta_us / length;

    int max_error = 0;
    for(unsigned int i = 0; i < length; i++){
      int32_t error = abs(expected[i] - A[i]);
      max_error = (error > max_error)? error : max_error;

      TEST_ASSERT_INT32_WITHIN(70000, expected[i], A[i]);
    }

    // printf("Len: % 3u;  time: % 7.02f us;      time/elm:  % 7.02f us;   max error: %u\n",
    //     length, delta_us, delta_us_per_elm, max_error);

    max_max_error = (max_error > max_max_error)? max_error : max_max_error;
  }

  // printf("Max max error: %u\n", max_max_error);
}

