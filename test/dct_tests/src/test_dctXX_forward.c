// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include "xmath/xmath.h"
#include "testing.h"
#include "floating_dct.h"
#include "tst_common.h"
#include "unity_fixture.h"
#include "lib_dsp/dsp_dct.h"

#include <string.h>

TEST_GROUP_RUNNER(dctXX_forward) {
  RUN_TEST_CASE(dctXX_forward, dct6_forward);
  RUN_TEST_CASE(dctXX_forward, dct8_forward);
  RUN_TEST_CASE(dctXX_forward, dct12_forward);
  RUN_TEST_CASE(dctXX_forward, dct16_forward);
  RUN_TEST_CASE(dctXX_forward, dct24_forward);
  RUN_TEST_CASE(dctXX_forward, dct32_forward);
  RUN_TEST_CASE(dctXX_forward, dct48_forward);
  RUN_TEST_CASE(dctXX_forward, dct64_forward);
}

TEST_GROUP(dctXX_forward);
TEST_SETUP(dctXX_forward) { fflush(stdout); }
TEST_TEAR_DOWN(dctXX_forward) {}


#define MAX_PROC_FRAME_LENGTH_LOG2 9
#define MAX_PROC_FRAME_LENGTH (1<<MAX_PROC_FRAME_LENGTH_LOG2)

#define MIN_DCT_N_LOG2  (3)

#if SMOKE_TEST
#  define LOOPS_LOG2       (2)
#else
#  define LOOPS_LOG2       (8)
#endif


TEST(dctXX_forward, dct6_forward)
{
#define FUNC_NAME "dct6_forward"
#define DCT_N   6

#if PRINT_FUNC_NAMES
  printf("\n%s..\n", FUNC_NAME);
#endif

  unsigned r = 1;

  float worst_timing = 0.0f;
  float worst_timing_dsp = 0.0f;
  
  for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
  
    DWORD_ALIGNED int32_t x[DCT_N];
    DWORD_ALIGNED int32_t y[DCT_N];

    exponent_t x_exp = -31;

    double ref_in[DCT_N];
    double ref_out[DCT_N];
    int32_t dsp_in[DCT_N];
    int32_t dsp_out[DCT_N];

    int32_t ref_out_s32[DCT_N];

    for(unsigned i = 0; i < DCT_N; i++){
      // Generate random signal with elements in [-1,1)
      x[i] = pseudo_rand_int32(&r);
      ref_in[i] = ldexp(x[i], x_exp);
    }

    // Compute the reference
    dbl_dct_forward(&ref_out[0], &ref_in[0], DCT_N);
    
    // printf("x = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%d, ", x[k]);
    // printf("])\n");

    unsigned ts1 = getTimestamp();
    dct6_forward(&y[0], &x[0]);
    unsigned ts2 = getTimestamp();
    
    // printf("y = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%d, ", y[k]);
    // printf("])\n");

    x_exp += dct6_exp;

    float dsp_timing = 0;
    unsigned dsp_ts1 = getTimestamp();
    dsp_dct_forward6(dsp_out, dsp_in);
    unsigned dsp_ts2 = getTimestamp();
    dsp_timing = (float) ((dsp_ts2 - dsp_ts1)/100.0);
    if(dsp_timing > worst_timing_dsp) worst_timing_dsp = dsp_timing;



    
    float timing = (float) ((ts2-ts1)/100.0);
    if(timing > worst_timing) worst_timing = timing;
    test_s32_from_double(ref_out_s32, ref_out, DCT_N, x_exp);
    int32_t max_allowed_diff = DCT_N;
    TEST_ASSERT_INT32_ARRAY_WITHIN(max_allowed_diff, ref_out_s32, y, DCT_N);

  }

#if TIME_FUNCS
    printf("    %s (%u-point): %f us\n", FUNC_NAME, DCT_N, worst_timing);
    printf("                   lib_dsp: %f us\n", worst_timing_dsp);
#endif

#undef DCT_N
#undef FUNC_NAME
}








TEST(dctXX_forward, dct8_forward)
{
#define FUNC_NAME "dct8_forward"
#define DCT_N   8

#if PRINT_FUNC_NAMES
  printf("\n%s..\n", FUNC_NAME);
#endif

  unsigned r = 1;

  float worst_timing = 0.0f;
  float worst_timing_dsp = 0.0f;
  
  for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
  
    DWORD_ALIGNED int32_t x[DCT_N];
    DWORD_ALIGNED int32_t y[DCT_N];

    exponent_t x_exp = -31;

    double ref_in[DCT_N];
    double ref_out[DCT_N];
    int32_t dsp_in[DCT_N];
    int32_t dsp_out[DCT_N];

    int32_t ref_out_s32[DCT_N];

    for(unsigned i = 0; i < DCT_N; i++){
      // Generate random signal with elements in [-1,1)
      x[i] = pseudo_rand_int32(&r);
      ref_in[i] = ldexp(x[i], x_exp);
    }

    // Compute the reference
    dbl_dct_forward(&ref_out[0], &ref_in[0], DCT_N);
    
    // printf("x = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%d, ", x[k]);
    // printf("])\n");

    unsigned ts1 = getTimestamp();
    dct8_forward(&y[0], &x[0]);
    unsigned ts2 = getTimestamp();
    
    // printf("y = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%d, ", y[k]);
    // printf("])\n");

    x_exp += dct8_exp;

    float dsp_timing = 0;
    unsigned dsp_ts1 = getTimestamp();
    dsp_dct_forward8(dsp_out, dsp_in);
    unsigned dsp_ts2 = getTimestamp();
    dsp_timing = (float) ((dsp_ts2 - dsp_ts1)/100.0);
    if(dsp_timing > worst_timing_dsp) worst_timing_dsp = dsp_timing;
    
    float timing = (float) ((ts2-ts1)/100.0);
    if(timing > worst_timing) worst_timing = timing;
    test_s32_from_double(ref_out_s32, ref_out, DCT_N, x_exp);
    int32_t max_allowed_diff = DCT_N;
    TEST_ASSERT_INT32_ARRAY_WITHIN(max_allowed_diff, ref_out_s32, y, DCT_N);

  }

#if TIME_FUNCS
    printf("    %s (%u-point): %f us\n", FUNC_NAME, DCT_N, worst_timing);
    printf("                   lib_dsp: %f us\n", worst_timing_dsp);
#endif

#undef DCT_N
#undef FUNC_NAME
}









TEST(dctXX_forward, dct12_forward)
{
#define FUNC_NAME "dct12_forward"
#define DCT_N  12

#if PRINT_FUNC_NAMES
  printf("\n%s..\n", FUNC_NAME);
#endif

  unsigned r = 1;

  float worst_timing = 0.0f;
  float worst_timing_dsp = 0.0f;
  
  for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
  
    DWORD_ALIGNED int32_t x[DCT_N];
    DWORD_ALIGNED int32_t y[DCT_N];

    exponent_t x_exp = -31;

    double ref_in[DCT_N];
    double ref_out[DCT_N];
    int32_t dsp_in[DCT_N];
    int32_t dsp_out[DCT_N];

    int32_t ref_out_s32[DCT_N];

    for(unsigned i = 0; i < DCT_N; i++){
      // Generate random signal with elements in [-1,1)
      x[i] = pseudo_rand_int32(&r);
      ref_in[i] = ldexp(x[i], x_exp);
    }

    // Compute the reference
    dbl_dct_forward(&ref_out[0], &ref_in[0], DCT_N);

    // printf("x = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%d, ", x[k]);
    // printf("])\n");

    unsigned ts1 = getTimestamp();
    dct12_forward(&y[0], &x[0]);
    unsigned ts2 = getTimestamp();
    x_exp += dct12_exp;
    // memcpy(y, x, sizeof(x));
    
    // printf("y = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%d, ", y[k]);
    // printf("])\n");

    float dsp_timing = 0;
    unsigned dsp_ts1 = getTimestamp();
    dsp_dct_forward12(dsp_out, dsp_in);
    unsigned dsp_ts2 = getTimestamp();
    dsp_timing = (float) ((dsp_ts2 - dsp_ts1)/100.0);
    if(dsp_timing > worst_timing_dsp) worst_timing_dsp = dsp_timing;

    float timing = (float) ((ts2-ts1)/100.0);
    if(timing > worst_timing) worst_timing = timing;
    test_s32_from_double(ref_out_s32, ref_out, DCT_N, x_exp);
    int32_t max_allowed_diff = 1<<3;
    TEST_ASSERT_INT32_ARRAY_WITHIN(max_allowed_diff, ref_out_s32, y, DCT_N);
  }

#if TIME_FUNCS
    printf("    %s (%u-point): %f us\n", FUNC_NAME, DCT_N, worst_timing);
    printf("                   lib_dsp: %f us\n", worst_timing_dsp);
#endif

#undef DCT_N
#undef FUNC_NAME
}








TEST(dctXX_forward, dct16_forward)
{
#define FUNC_NAME "dct16_forward"
#define DCT_N   16

#if PRINT_FUNC_NAMES
  printf("\n%s..\n", FUNC_NAME);
#endif

  unsigned r = 1;

  float worst_timing = 0.0f;
  float worst_timing_dsp = 0.0f;
  
  for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
  
    DWORD_ALIGNED int32_t x[DCT_N];
    DWORD_ALIGNED int32_t y[DCT_N];

    exponent_t x_exp = -31;

    double ref_in[DCT_N];
    double ref_out[DCT_N];
    int32_t dsp_in[DCT_N];
    int32_t dsp_out[DCT_N];

    int32_t ref_out_s32[DCT_N];

    for(unsigned i = 0; i < DCT_N; i++){
      // Generate random signal with elements in [-1,1)
      x[i] = pseudo_rand_int32(&r);
      ref_in[i] = ldexp(x[i], x_exp);
    }

    // Compute the reference
    dbl_dct_forward(&ref_out[0], &ref_in[0], DCT_N);
    
    // printf("x = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%d, ", x[k]);
    // printf("])\n");

    unsigned ts1 = getTimestamp();
    dct16_forward(&y[0], &x[0]);
    // dct16_forward_v2(&y[0], &x[0]);
    unsigned ts2 = getTimestamp();
    
    // printf("y = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%d, ", y[k]);
    // printf("])\n");

    x_exp += dct16_exp;

    float dsp_timing = 0;
    unsigned dsp_ts1 = getTimestamp();
    dsp_dct_forward16(dsp_out, dsp_in);
    unsigned dsp_ts2 = getTimestamp();
    dsp_timing = (float) ((dsp_ts2 - dsp_ts1)/100.0);
    if(dsp_timing > worst_timing_dsp) worst_timing_dsp = dsp_timing;
    
    float timing = (float) ((ts2-ts1)/100.0);
    if(timing > worst_timing) worst_timing = timing;
    test_s32_from_double(ref_out_s32, ref_out, DCT_N, x_exp);
    int32_t max_allowed_diff = DCT_N;
    TEST_ASSERT_INT32_ARRAY_WITHIN(max_allowed_diff, ref_out_s32, y, DCT_N);

  }

#if TIME_FUNCS
    printf("    %s (%u-point): %f us\n", FUNC_NAME, DCT_N, worst_timing);
    printf("                   lib_dsp: %f us\n", worst_timing_dsp);
#endif

#undef DCT_N
#undef FUNC_NAME
}









TEST(dctXX_forward, dct24_forward)
{
#define FUNC_NAME "dct24_forward"
#define DCT_N  24

#if PRINT_FUNC_NAMES
  printf("\n%s..\n", FUNC_NAME);
#endif

  unsigned r = 1;

  float worst_timing = 0.0f;
  float worst_timing_dsp = 0.0f;
  
  for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
  
    DWORD_ALIGNED int32_t x[DCT_N];
    DWORD_ALIGNED int32_t y[DCT_N];

    exponent_t x_exp = -31;

    double ref_in[DCT_N];
    double ref_out[DCT_N];
    int32_t dsp_in[DCT_N];
    int32_t dsp_out[DCT_N];

    int32_t ref_out_s32[DCT_N];

    for(unsigned i = 0; i < DCT_N; i++){
      // Generate random signal with elements in [-1,1)
      x[i] = pseudo_rand_int32(&r);
      ref_in[i] = ldexp(x[i], x_exp);
    }

    // Compute the reference
    dbl_dct_forward(&ref_out[0], &ref_in[0], DCT_N);

    // printf("x = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%d, ", x[k]);
    // printf("])\n");

    unsigned ts1 = getTimestamp();
    dct24_forward(&y[0], &x[0]);
    // dct24_forward_v2(&y[0], &x[0]);
    unsigned ts2 = getTimestamp();
    x_exp += dct24_exp;
    // memcpy(y, x, sizeof(x));
    
    // printf("y = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%d, ", y[k]);
    // printf("])\n");

    float dsp_timing = 0;
    unsigned dsp_ts1 = getTimestamp();
    dsp_dct_forward24(dsp_out, dsp_in);
    unsigned dsp_ts2 = getTimestamp();
    dsp_timing = (float) ((dsp_ts2 - dsp_ts1)/100.0);
    if(dsp_timing > worst_timing_dsp) worst_timing_dsp = dsp_timing;

    float timing = (float) ((ts2-ts1)/100.0);
    if(timing > worst_timing) worst_timing = timing;
    test_s32_from_double(ref_out_s32, ref_out, DCT_N, x_exp);
    int32_t max_allowed_diff = DCT_N;
    TEST_ASSERT_INT32_ARRAY_WITHIN(max_allowed_diff, ref_out_s32, y, DCT_N);

  }

#if TIME_FUNCS
    printf("    %s (%u-point): %f us\n", FUNC_NAME, DCT_N, worst_timing);
    printf("                   lib_dsp: %f us\n", worst_timing_dsp);
#endif

#undef DCT_N
#undef FUNC_NAME
}








TEST(dctXX_forward, dct32_forward)
{
#define FUNC_NAME "dct32_forward"
#define DCT_N   32

#if PRINT_FUNC_NAMES
  printf("\n%s..\n", FUNC_NAME);
#endif

  unsigned r = 1;

  float worst_timing = 0.0f;
  float worst_timing_dsp = 0.0f;
  
  for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
  
    DWORD_ALIGNED int32_t x[DCT_N];
    DWORD_ALIGNED int32_t y[DCT_N];

    exponent_t x_exp = -31;

    double ref_in[DCT_N];
    double ref_out[DCT_N];
    int32_t dsp_in[DCT_N];
    int32_t dsp_out[DCT_N];

    int32_t ref_out_s32[DCT_N];

    for(unsigned i = 0; i < DCT_N; i++){
      // Generate random signal with elements in [-1,1)
      x[i] = pseudo_rand_int32(&r);
      ref_in[i] = ldexp(x[i], x_exp);
    }

    // Compute the reference
    dbl_dct_forward(&ref_out[0], &ref_in[0], DCT_N);
    
    // printf("x = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%d, ", x[k]);
    // printf("])\n");

    unsigned ts1 = getTimestamp();
    dct32_forward(&y[0], &x[0]);
    unsigned ts2 = getTimestamp();
    
    // printf("y = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%d, ", y[k]);
    // printf("])\n");

    x_exp += dct32_exp;

    float dsp_timing = 0;
    unsigned dsp_ts1 = getTimestamp();
    dsp_dct_forward32(dsp_out, dsp_in);
    unsigned dsp_ts2 = getTimestamp();
    dsp_timing = (float) ((dsp_ts2 - dsp_ts1)/100.0);
    if(dsp_timing > worst_timing_dsp) worst_timing_dsp = dsp_timing;
    
    float timing = (float) ((ts2-ts1)/100.0);
    if(timing > worst_timing) worst_timing = timing;
    test_s32_from_double(ref_out_s32, ref_out, DCT_N, x_exp);
    int32_t max_allowed_diff = DCT_N;
    TEST_ASSERT_INT32_ARRAY_WITHIN(max_allowed_diff, ref_out_s32, y, DCT_N);

  }

#if TIME_FUNCS
    printf("    %s (%u-point): %f us\n", FUNC_NAME, DCT_N, worst_timing);
    printf("                   lib_dsp: %f us\n", worst_timing_dsp);
#endif

#undef DCT_N
#undef FUNC_NAME
}









TEST(dctXX_forward, dct48_forward)
{
#define FUNC_NAME "dct48_forward"
#define DCT_N  48

#if PRINT_FUNC_NAMES
  printf("\n%s..\n", FUNC_NAME);
#endif

  unsigned r = 1;

  float worst_timing = 0.0f;
  float worst_timing_dsp = 0.0f;
  
  for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
  
    DWORD_ALIGNED int32_t x[DCT_N];
    DWORD_ALIGNED int32_t y[DCT_N];

    exponent_t x_exp = -31;

    double ref_in[DCT_N];
    double ref_out[DCT_N];
    int32_t dsp_in[DCT_N];
    int32_t dsp_out[DCT_N];

    int32_t ref_out_s32[DCT_N];

    for(unsigned i = 0; i < DCT_N; i++){
      // Generate random signal with elements in [-1,1)
      x[i] = pseudo_rand_int32(&r);
      ref_in[i] = ldexp(x[i], x_exp);
    }

    // Compute the reference
    dbl_dct_forward(&ref_out[0], &ref_in[0], DCT_N);

    // printf("x = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%d, ", x[k]);
    // printf("])\n");

    unsigned ts1 = getTimestamp();
    dct48_forward(&y[0], &x[0]);
    unsigned ts2 = getTimestamp();
    x_exp += dct48_exp;
    // memcpy(y, x, sizeof(x));
    
    // printf("y = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%d, ", y[k]);
    // printf("])\n");

    float dsp_timing = 0;
    unsigned dsp_ts1 = getTimestamp();
    dsp_dct_forward48(dsp_out, dsp_in);
    unsigned dsp_ts2 = getTimestamp();
    dsp_timing = (float) ((dsp_ts2 - dsp_ts1)/100.0);
    if(dsp_timing > worst_timing_dsp) worst_timing_dsp = dsp_timing;

    float timing = (float) ((ts2-ts1)/100.0);
    if(timing > worst_timing) worst_timing = timing;
    test_s32_from_double(ref_out_s32, ref_out, DCT_N, x_exp);
    int32_t max_allowed_diff = DCT_N;
    TEST_ASSERT_INT32_ARRAY_WITHIN(max_allowed_diff, ref_out_s32, y, DCT_N);

  }

#if TIME_FUNCS
    printf("    %s (%u-point): %f us\n", FUNC_NAME, DCT_N, worst_timing);
    printf("                   lib_dsp: %f us\n", worst_timing_dsp);
#endif

#undef DCT_N
#undef FUNC_NAME
}








TEST(dctXX_forward, dct64_forward)
{
#define FUNC_NAME "dct64_forward"
#define DCT_N   64

#if PRINT_FUNC_NAMES
  printf("\n%s..\n", FUNC_NAME);
#endif

  unsigned r = 1;

  float worst_timing = 0.0f;
  
  for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
  
    DWORD_ALIGNED int32_t x[DCT_N];
    DWORD_ALIGNED int32_t y[DCT_N];

    exponent_t x_exp = -31;

    double ref_in[DCT_N];
    double ref_out[DCT_N];

    int32_t ref_out_s32[DCT_N];

    for(unsigned i = 0; i < DCT_N; i++){
      // Generate random signal with elements in [-1,1)
      x[i] = pseudo_rand_int32(&r);
      ref_in[i] = ldexp(x[i], x_exp);
    }

    // Compute the reference
    dbl_dct_forward(&ref_out[0], &ref_in[0], DCT_N);
    
    // printf("x = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%d, ", x[k]);
    // printf("])\n");

    unsigned ts1 = getTimestamp();
    dct64_forward(&y[0], &x[0]);
    unsigned ts2 = getTimestamp();
    
    // printf("y = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%d, ", y[k]);
    // printf("])\n");

    x_exp += dct64_exp;
    
    float timing = (float) ((ts2-ts1)/100.0);
    if(timing > worst_timing) worst_timing = timing;
    test_s32_from_double(ref_out_s32, ref_out, DCT_N, x_exp);
    int32_t max_allowed_diff = 8;
    TEST_ASSERT_INT32_ARRAY_WITHIN(max_allowed_diff, ref_out_s32, y, DCT_N);

  }

#if TIME_FUNCS
    printf("    %s (%u-point): %f us\n", FUNC_NAME, DCT_N, worst_timing);
#endif

#undef DCT_N
#undef FUNC_NAME
}






