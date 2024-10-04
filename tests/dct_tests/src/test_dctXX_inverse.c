// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include "xmath/xmath.h"
#include "testing.h"
#include "floating_dct.h"
#include "tst_common.h"
#include "unity_fixture.h"
#include "lib_dsp/dsp_dct.h"

#include <string.h>

TEST_GROUP_RUNNER(dctXX_inverse) {
  RUN_TEST_CASE(dctXX_inverse, dct6_inverse);
  RUN_TEST_CASE(dctXX_inverse, dct8_inverse);
  RUN_TEST_CASE(dctXX_inverse, dct12_inverse);
  RUN_TEST_CASE(dctXX_inverse, dct16_inverse);
  RUN_TEST_CASE(dctXX_inverse, dct24_inverse);
  RUN_TEST_CASE(dctXX_inverse, dct32_inverse);
  RUN_TEST_CASE(dctXX_inverse, dct48_inverse);
  RUN_TEST_CASE(dctXX_inverse, dct64_inverse);
}

TEST_GROUP(dctXX_inverse);
TEST_SETUP(dctXX_inverse) { fflush(stdout); }
TEST_TEAR_DOWN(dctXX_inverse) {}


#define MAX_PROC_FRAME_LENGTH_LOG2 9
#define MAX_PROC_FRAME_LENGTH (1<<MAX_PROC_FRAME_LENGTH_LOG2)

#define MIN_DCT_N_LOG2  (3)
#define LOOPS_LOG2  (0)


TEST(dctXX_inverse, dct6_inverse)
{
#define FUNC_NAME "dct6_inverse"
#define DCT_N   6

#if PRINT_FUNC_NAMES
  printf("\n%s..\n", FUNC_NAME);
#endif

  unsigned r = 1;

  float worst_timing = 0.0f;
  
  for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
  
    DWORD_ALIGNED int32_t x[DCT_N];
    DWORD_ALIGNED int32_t y[DCT_N];

    double ref_in[DCT_N];
    double ref_out[DCT_N];
 
    for(unsigned i = 0; i < DCT_N; i++){
      ref_in[i] = x[i] = pseudo_rand_int32(&r);
    }

    // Compute the reference
    dbl_dct_inverse(&ref_out[0], &ref_in[0], DCT_N);
    
    // printf("x = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%d, ", x[k]);
    // printf("])\n");

    unsigned ts1 = getTimestamp();
    dct6_inverse(&y[0], &x[0]);
    unsigned ts2 = getTimestamp();
    
    // printf("y = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%d, ", y[k]);
    // printf("])\n");

    // printf("ref = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%0.03f, ", ref_out[k]);
    // printf("])\n");

    int32_t max_allowed_diff = DCT_N;
    for(unsigned int n = 0; n < DCT_N; n++){
      int32_t act_val = y[n];
      int32_t ref_val = lround(ref_out[n]);
      TEST_ASSERT_INT32_WITHIN(max_allowed_diff, ref_val, act_val);
    }

    float timing = (float) ((ts2-ts1)/100.0);
    if(timing > worst_timing) worst_timing = timing;
  }

#if TIME_FUNCS
    printf("    %s (%u-point): %f us\n", FUNC_NAME, DCT_N, worst_timing);
#endif

#undef DCT_N
#undef FUNC_NAME
}








TEST(dctXX_inverse, dct8_inverse)
{
#define FUNC_NAME "dct8_inverse"
#define DCT_N   8

#if PRINT_FUNC_NAMES
  printf("\n%s..\n", FUNC_NAME);
#endif

  unsigned r = 1;

  float worst_timing = 0.0f;
  
  for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
  
    DWORD_ALIGNED int32_t x[DCT_N];
    DWORD_ALIGNED int32_t y[DCT_N];

    double ref_in[DCT_N];
    double ref_out[DCT_N];
 
    for(unsigned i = 0; i < DCT_N; i++){
      ref_in[i] = x[i] = pseudo_rand_int32(&r);
    }

    // Compute the reference
    dbl_dct_inverse(&ref_out[0], &ref_in[0], DCT_N);
    
    // printf("x = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%d, ", x[k]);
    // printf("])\n");

    unsigned ts1 = getTimestamp();
    dct8_inverse(&y[0], &x[0]);
    unsigned ts2 = getTimestamp();
    
    // printf("y = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%d, ", y[k]);
    // printf("])\n");

    // printf("ref = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%0.03f, ", ref_out[k]);
    // printf("])\n");

    int32_t max_allowed_diff = DCT_N;
    for(unsigned int n = 0; n < DCT_N; n++){
      int32_t act_val = y[n];
      int32_t ref_val = lround(ref_out[n]);
      TEST_ASSERT_INT32_WITHIN(max_allowed_diff, ref_val, act_val);
    }

    float timing = (float) ((ts2-ts1)/100.0);
    if(timing > worst_timing) worst_timing = timing;
  }

#if TIME_FUNCS
    printf("    %s (%u-point): %f us\n", FUNC_NAME, DCT_N, worst_timing);
#endif

#undef DCT_N
#undef FUNC_NAME
}







TEST(dctXX_inverse, dct12_inverse)
{
#define FUNC_NAME "dct12_inverse"
#define DCT_N   12

#if PRINT_FUNC_NAMES
  printf("\n%s..\n", FUNC_NAME);
#endif

  unsigned r = 1;

  float worst_timing = 0.0f;
  
  for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
  
    DWORD_ALIGNED int32_t x[DCT_N];
    DWORD_ALIGNED int32_t y[DCT_N];

    double ref_in[DCT_N];
    double ref_out[DCT_N];
 
    for(unsigned i = 0; i < DCT_N; i++){
      ref_in[i] = x[i] = pseudo_rand_int32(&r);
    }

    // Compute the reference
    dbl_dct_inverse(&ref_out[0], &ref_in[0], DCT_N);
    
    // printf("\nx = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%d, ", x[k]);
    // printf("])\n");

    unsigned ts1 = getTimestamp();
    dct12_inverse(&y[0], &x[0]);
    unsigned ts2 = getTimestamp();
    
    // printf("y = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%d, ", y[k]);
    // printf("])\n");

    // printf("ref = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%0.03f, ", ref_out[k]);
    // printf("])\n");

    int32_t max_allowed_diff = 8;
    for(unsigned int n = 0; n < DCT_N; n++){
      int32_t act_val = y[n];
      int32_t ref_val = lround(ref_out[n]);
      TEST_ASSERT_INT32_WITHIN(max_allowed_diff, ref_val, act_val);
    }

    float timing = (float) ((ts2-ts1)/100.0);
    if(timing > worst_timing) worst_timing = timing;
  }

#if TIME_FUNCS
    printf("    %s (%u-point): %f us\n", FUNC_NAME, DCT_N, worst_timing);
#endif

#undef DCT_N
#undef FUNC_NAME
}








TEST(dctXX_inverse, dct16_inverse)
{
#define FUNC_NAME "dct16_inverse"
#define DCT_N   16

#if PRINT_FUNC_NAMES
  printf("\n%s..\n", FUNC_NAME);
#endif

  unsigned r = 1;

  float worst_timing = 0.0f;
  
  for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
  
    DWORD_ALIGNED int32_t x[DCT_N];
    DWORD_ALIGNED int32_t y[DCT_N];

    double ref_in[DCT_N];
    double ref_out[DCT_N];
 
    for(unsigned i = 0; i < DCT_N; i++){
      ref_in[i] = x[i] = pseudo_rand_int32(&r);
    }

    // Compute the reference
    dbl_dct_inverse(&ref_out[0], &ref_in[0], DCT_N);
    
    // printf("x = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%d, ", x[k]);
    // printf("])\n");

    unsigned ts1 = getTimestamp();
    dct16_inverse(&y[0], &x[0]);
    unsigned ts2 = getTimestamp();
    
    // printf("y = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%d, ", y[k]);
    // printf("])\n");

    // printf("ref = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%0.03f, ", ref_out[k]);
    // printf("])\n");

    int32_t max_allowed_diff = 2*DCT_N;
    for(unsigned int n = 0; n < DCT_N; n++){
      int32_t act_val = y[n];
      int32_t ref_val = lround(ref_out[n]);
      TEST_ASSERT_INT32_WITHIN(max_allowed_diff, ref_val, act_val);
    }

    float timing = (float) ((ts2-ts1)/100.0);
    if(timing > worst_timing) worst_timing = timing;
  }

#if TIME_FUNCS
    printf("    %s (%u-point): %f us\n", FUNC_NAME, DCT_N, worst_timing);
#endif

#undef DCT_N
#undef FUNC_NAME
}








TEST(dctXX_inverse, dct24_inverse)
{
#define FUNC_NAME "dct24_inverse"
#define DCT_N   24

#if PRINT_FUNC_NAMES
  printf("\n%s..\n", FUNC_NAME);
#endif

  unsigned r = 1;

  float worst_timing = 0.0f;
  
  for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
  
    DWORD_ALIGNED int32_t x[DCT_N];
    DWORD_ALIGNED int32_t y[DCT_N];

    double ref_in[DCT_N];
    double ref_out[DCT_N];
 
    for(unsigned i = 0; i < DCT_N; i++){
      ref_in[i] = x[i] = pseudo_rand_int32(&r);
    }

    // Compute the reference
    dbl_dct_inverse(&ref_out[0], &ref_in[0], DCT_N);
    
    // printf("x = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%d, ", x[k]);
    // printf("])\n");

    unsigned ts1 = getTimestamp();
    dct24_inverse(&y[0], &x[0]);
    unsigned ts2 = getTimestamp();
    
    // printf("y = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%d, ", y[k]);
    // printf("])\n");

    // printf("ref = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%0.03f, ", ref_out[k]);
    // printf("])\n");

    int32_t max_allowed_diff = 2*DCT_N;
    for(unsigned int n = 0; n < DCT_N; n++){
      int32_t act_val = y[n];
      int32_t ref_val = lround(ref_out[n]);
      TEST_ASSERT_INT32_WITHIN(max_allowed_diff, ref_val, act_val);
    }

    float timing = (float) ((ts2-ts1)/100.0);
    if(timing > worst_timing) worst_timing = timing;
  }

#if TIME_FUNCS
    printf("    %s (%u-point): %f us\n", FUNC_NAME, DCT_N, worst_timing);
#endif

#undef DCT_N
#undef FUNC_NAME
}








TEST(dctXX_inverse, dct32_inverse)
{
#define FUNC_NAME "dct32_inverse"
#define DCT_N   32

#if PRINT_FUNC_NAMES
  printf("\n%s..\n", FUNC_NAME);
#endif

  unsigned r = 1;

  float worst_timing = 0.0f;
  
  for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
  
    DWORD_ALIGNED int32_t x[DCT_N];
    DWORD_ALIGNED int32_t y[DCT_N];

    double ref_in[DCT_N];
    double ref_out[DCT_N];
 
    for(unsigned i = 0; i < DCT_N; i++){
      ref_in[i] = x[i] = pseudo_rand_int32(&r);
    }

    // Compute the reference
    dbl_dct_inverse(&ref_out[0], &ref_in[0], DCT_N);
    
    // printf("x = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%d, ", x[k]);
    // printf("])\n");

    unsigned ts1 = getTimestamp();
    dct32_inverse(&y[0], &x[0]);
    unsigned ts2 = getTimestamp();
    
    // printf("y = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%d, ", y[k]);
    // printf("])\n");

    // printf("ref = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%0.03f, ", ref_out[k]);
    // printf("])\n");

    int32_t max_allowed_diff = 2*DCT_N;
    for(unsigned int n = 0; n < DCT_N; n++){
      int32_t act_val = y[n];
      int32_t ref_val = lround(ref_out[n]);
      TEST_ASSERT_INT32_WITHIN(max_allowed_diff, ref_val, act_val);
    }

    float timing = (float) ((ts2-ts1)/100.0);
    if(timing > worst_timing) worst_timing = timing;
  }

#if TIME_FUNCS
    printf("    %s (%u-point): %f us\n", FUNC_NAME, DCT_N, worst_timing);
#endif

#undef DCT_N
#undef FUNC_NAME
}








TEST(dctXX_inverse, dct48_inverse)
{
#define FUNC_NAME "dct48_inverse"
#define DCT_N   48

#if PRINT_FUNC_NAMES
  printf("\n%s..\n", FUNC_NAME);
#endif

  unsigned r = 1;

  float worst_timing = 0.0f;
  
  for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
  
    DWORD_ALIGNED int32_t x[DCT_N];
    DWORD_ALIGNED int32_t y[DCT_N];

    double ref_in[DCT_N];
    double ref_out[DCT_N];
 
    for(unsigned i = 0; i < DCT_N; i++){
      ref_in[i] = x[i] = pseudo_rand_int32(&r);
    }

    // Compute the reference
    dbl_dct_inverse(&ref_out[0], &ref_in[0], DCT_N);
    
    // printf("x = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%d, ", x[k]);
    // printf("])\n");

    unsigned ts1 = getTimestamp();
    dct48_inverse(&y[0], &x[0]);
    unsigned ts2 = getTimestamp();
    
    // printf("y = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%d, ", y[k]);
    // printf("])\n");

    // printf("ref = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%0.03f, ", ref_out[k]);
    // printf("])\n");

    int32_t max_allowed_diff = 5*DCT_N;
    for(unsigned int n = 0; n < DCT_N; n++){
      int32_t act_val = y[n];
      int32_t ref_val = lround(ref_out[n]);
      TEST_ASSERT_INT32_WITHIN(max_allowed_diff, ref_val, act_val);
    }

    float timing = (float) ((ts2-ts1)/100.0);
    if(timing > worst_timing) worst_timing = timing;
  }

#if TIME_FUNCS
    printf("    %s (%u-point): %f us\n", FUNC_NAME, DCT_N, worst_timing);
#endif

#undef DCT_N
#undef FUNC_NAME
}








TEST(dctXX_inverse, dct64_inverse)
{
#define FUNC_NAME "dct64_inverse"
#define DCT_N   64

#if PRINT_FUNC_NAMES
  printf("\n%s..\n", FUNC_NAME);
#endif

  unsigned r = 1;

  float worst_timing = 0.0f;
  
  for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
  
    DWORD_ALIGNED int32_t x[DCT_N];
    DWORD_ALIGNED int32_t y[DCT_N];

    double ref_in[DCT_N];
    double ref_out[DCT_N];
 
    for(unsigned i = 0; i < DCT_N; i++){
      ref_in[i] = x[i] = pseudo_rand_int32(&r);
    }

    // Compute the reference
    dbl_dct_inverse(&ref_out[0], &ref_in[0], DCT_N);
    
    // printf("x = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%d, ", x[k]);
    // printf("])\n");

    unsigned ts1 = getTimestamp();
    dct64_inverse(&y[0], &x[0]);
    unsigned ts2 = getTimestamp();
    
    // printf("y = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%d, ", y[k]);
    // printf("])\n");

    // printf("ref = np.array([ ");
    // for(int k = 0; k < DCT_N; k++)  printf("%0.03f, ", ref_out[k]);
    // printf("])\n");

    int32_t max_allowed_diff = 6*DCT_N;
    for(unsigned int n = 0; n < DCT_N; n++){
      int32_t act_val = y[n];
      int32_t ref_val = lround(ref_out[n]);
      TEST_ASSERT_INT32_WITHIN(max_allowed_diff, ref_val, act_val);
    }

    float timing = (float) ((ts2-ts1)/100.0);
    if(timing > worst_timing) worst_timing = timing;
  }

#if TIME_FUNCS
    printf("    %s (%u-point): %f us\n", FUNC_NAME, DCT_N, worst_timing);
#endif

#undef DCT_N
#undef FUNC_NAME
}

