// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include "xmath/xmath.h"
#include "testing.h"
#include "floating_dct.h"
#include "tst_common.h"
#include "unity_fixture.h"
#include "lib_dsp/dsp_dct.h"

#include <string.h>

TEST_GROUP_RUNNER(dct8x8) {
  RUN_TEST_CASE(dct8x8, dct8x8_stageA);
  RUN_TEST_CASE(dct8x8, dct8x8_stageB);
  RUN_TEST_CASE(dct8x8, dct8x8_forward);
  RUN_TEST_CASE(dct8x8, dct8x8_inverse);
}

TEST_GROUP(dct8x8);
TEST_SETUP(dct8x8) { fflush(stdout); }
TEST_TEAR_DOWN(dct8x8) {}



#define MAX_PROC_FRAME_LENGTH_LOG2 9
#define MAX_PROC_FRAME_LENGTH (1<<MAX_PROC_FRAME_LENGTH_LOG2)

#define MIN_DCT_N_LOG2  (3)
#define LOOPS_LOG2  (4)

static
void dbl_transpose_8x8(
    double x[8][8])
{
  // because it's square, transpose can happen in-place
  for(int r = 0; r < 8; r++){
    for(int c = 0; c < r; c++){
      double tmp = x[c][r];
      x[c][r] = x[r][c];
      x[r][c] = tmp;
    }
  }
}


static
void dbl_dct8x8_stageA(
    double y[8][8],
    double x[8][8])
{
  // Do row-by-row DCT8 and then transpose

  for(int r = 0; r < 8; r++){
    dbl_dct_forward(&y[r][0], &x[r][0], 8);
  }

  dbl_transpose_8x8(y);

  // dct8x8_stageA is formulated such that the output is effectively
  // the true DCT values with an implied exponent of -3. So, multiplying
  // everything by 8 here should give us the same result as dct8x8_stageA
  
  for(int r = 0; r < 8; r++)
    for(int c = 0; c < 8; c++)
      y[r][c] = 8*y[r][c];
}


static
void dbl_dct8x8_stageB(
    double y[8][8],
    double x[8][8],
    right_shift_t sat)
{
  // Do row-by-row DCT8 and then transpose

  for(int r = 0; r < 8; r++){
    dbl_dct_forward(&y[r][0], &x[r][0], 8);
  }

  dbl_transpose_8x8(y);

  // dct8x8_stageB is formulated such that the output is effectively
  // the true DCT values with an implied exponent of (sat+11). So, dividing
  // everything by 2**(sat+11) here should give us the same result as
  // dct8x8_stageB
  
  for(int r = 0; r < 8; r++)
    for(int c = 0; c < 8; c++)
      y[r][c] = y[r][c] / (1<<(sat+11));
}


static
void dbl_idct8x8_stageA(
    double y[8][8],
    double x[8][8])
{
  // Do row-by-row DCT8 and then transpose

  for(int r = 0; r < 8; r++){
    dbl_dct_inverse(&y[r][0], &x[r][0], 8);
  }

  dbl_transpose_8x8(y);

  // dct8_inversex8_stageA is formulated such that the output is effectively
  // the true DCT values with an implied exponent of -7. So, multiplying
  // everything by 128 here should give us the same result as dct8_inversex8_stageA
  
  for(int r = 0; r < 8; r++)
    for(int c = 0; c < 8; c++)
      y[r][c] = 128*y[r][c];
}

static
void dbl_idct8x8_stageB(
    double y[8][8],
    double x[8][8],
    right_shift_t sat)
{
  // Do row-by-row DCT8 and then transpose

  for(int r = 0; r < 8; r++){
    dbl_dct_inverse(&y[r][0], &x[r][0], 8);
  }

  dbl_transpose_8x8(y);

  // dct8_inversex8_stageB is formulated such that the output is effectively
  // the true DCT values with an implied exponent of (sat+7). So, dividing
  // everything by 2**(sat+7) here should give us the same result as
  // dct8_inversex8_stageB
  
  for(int r = 0; r < 8; r++)
    for(int c = 0; c < 8; c++)
      y[r][c] = y[r][c] / (1<<(sat+7));
}




static
void dbl_dct8x8(
    double y[8][8],
    double x[8][8],
    right_shift_t sat)
{
  double tmp[8][8];
  dbl_dct8x8_stageA(tmp, x);
  dbl_dct8x8_stageB(y, tmp, sat);
}



static
void dbl_idct8x8(
    double y[8][8],
    double x[8][8],
    right_shift_t sat)
{
  double tmp[8][8];
  dbl_idct8x8_stageA(tmp, x);
  dbl_idct8x8_stageB(y, tmp, sat);
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

TEST(dct8x8, dct8x8_stageA)
{
#define FUNC_NAME "dct8x8_stageA"

#if PRINT_FUNC_NAMES
  printf("\n%s..\n", FUNC_NAME);
#endif

  unsigned r = 1;

  float worst_timing = 0.0f;

  for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
  
    DWORD_ALIGNED int8_t x[8][8];
    DWORD_ALIGNED int16_t y[8][8];

    double ref_in[8][8];
    double ref_out[8][8];

    for(int row = 0; row < 8; row++){
      for(unsigned col = 0; col < 8; col++){
        ref_in[row][col] = x[row][col] = pseudo_rand_int8(&r);
      }
    }

    // Compute the reference
    dbl_dct8x8_stageA(ref_out, ref_in);
    
    // printf("x = np.array([ ");
    // for(int row = 0; row < 8; row++){
    //   printf("  [ ");
    //   for(int col = 0; col < 8; col++)  printf("%d, ", x[row][col]);
    //   printf(" ],\n");
    // }
    // printf("])\n");

    unsigned ts1 = getTimestamp();
    headroom_t reported_hr = dct8x8_stageA(y, x, dct8_matrix_16bit);
    unsigned ts2 = getTimestamp();
    
    // printf("y = np.array([ ");
    // for(int row = 0; row < 8; row++){
    //   printf("  [ ");
    //   for(int col = 0; col < 8; col++) printf("%d, ", y[row][col]);
    //   printf(" ],\n");
    // }
    // printf("])\n");
    
    // printf("ref = np.array([ ");
    // for(int row = 0; row < 8; row++){
    //   printf("  [ ");
    //   for(int col = 0; col < 8; col++) printf("%0.00f, ", ref_out[row][col]);
    //   printf(" ],\n");
    // }
    // printf("])\n");
    
    float max_allowed_diff = 2;

    for(int row = 0; row < 8; row++){
      for(unsigned col = 0; col < 8; col++){
        float ref_val = (float) ref_out[row][col];
        float act_val = y[row][col];

        TEST_ASSERT_FLOAT_WITHIN(max_allowed_diff, ref_val, act_val);
      }
    }

    headroom_t actual_hr = vect_s16_headroom(&y[0][0], 64);
    TEST_ASSERT_EQUAL(actual_hr, reported_hr);

    float timing = (float) ((ts2-ts1)/100.0);
    if(timing > worst_timing) worst_timing = timing;
  }

#if TIME_FUNCS
    printf("    %s (8x8-point): %f us\n", FUNC_NAME, worst_timing);
#endif

#undef FUNC_NAME
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

TEST(dct8x8, dct8x8_stageB)
{
#define FUNC_NAME "dct8x8_stageB"

#if PRINT_FUNC_NAMES
  printf("\n%s..\n", FUNC_NAME);
#endif

  unsigned r = 1;

  float worst_timing = 0.0f;
  
  for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
  
    DWORD_ALIGNED int16_t x[8][8];
    DWORD_ALIGNED int8_t y[8][8];

    double ref_in[8][8];
    double ref_out[8][8];

    for(int row = 0; row < 8; row++){
      for(unsigned col = 0; col < 8; col++){
        // the output of stageA should have 1 bit of headroom so that
        // part 2 doesn't overflow the accumulators
        ref_in[row][col] = x[row][col] = pseudo_rand_int16(&r) >> 1;
      }
    }

    // Compute the reference
    dbl_dct8x8_stageB(ref_out, ref_in, 0);
    
    // printf("x = np.array([ ");
    // for(int row = 0; row < 8; row++){
    //   printf("  [ ");
    //   for(int col = 0; col < 8; col++)  printf("%d, ", x[row][col]);
    //   printf(" ],\n");
    // }
    // printf("])\n");

    unsigned ts1 = getTimestamp();
    headroom_t reported_hr = dct8x8_stageB(y, x, dct8_matrix_16bit, 0);
    unsigned ts2 = getTimestamp();
    
    // printf("y = np.array([ ");
    // for(int row = 0; row < 8; row++){
    //   printf("  [ ");
    //   for(int col = 0; col < 8; col++) printf("%d, ", y[row][col]);
    //   printf(" ],\n");
    // }
    // printf("])\n");
    
    // printf("ref = np.array([ ");
    // for(int row = 0; row < 8; row++){
    //   printf("  [ ");
    //   for(int col = 0; col < 8; col++) printf("%0.00f, ", ref_out[row][col]);
    //   printf(" ],\n");
    // }
    // printf("])\n");
    
    float max_allowed_diff = 2;
    headroom_t actual_hr = 8;

    for(int row = 0; row < 8; row++){
      for(unsigned col = 0; col < 8; col++){
        float ref_val = (float) ref_out[row][col];
        float act_val = y[row][col];

        TEST_ASSERT_FLOAT_WITHIN(max_allowed_diff, ref_val, act_val);
        
        // NOTE: currently no vect_s8_headroom() function.
        headroom_t hr = HR_S8(y[row][col]);
        actual_hr = MIN(actual_hr, hr);
      }
    }

    TEST_ASSERT_EQUAL(actual_hr, reported_hr);

    float timing = (float) ((ts2-ts1)/100.0);
    if(timing > worst_timing) worst_timing = timing;
  }

#if TIME_FUNCS
    printf("    %s (8x8-point): %f us\n", FUNC_NAME, worst_timing);
#endif

#undef FUNC_NAME
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


TEST(dct8x8, dct8x8_forward)
{
#define FUNC_NAME "dct8x8_forward"

#if PRINT_FUNC_NAMES
  printf("\n%s..\n", FUNC_NAME);
#endif

  unsigned r = 1;

  float worst_timing = 0.0f;
  
  for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
  
    DWORD_ALIGNED int8_t x[8][8];
    DWORD_ALIGNED int8_t y[8][8];

    double ref_in[8][8];
    double ref_out[8][8];

    for(int row = 0; row < 8; row++){
      for(unsigned col = 0; col < 8; col++){
        ref_in[row][col] = x[row][col] = pseudo_rand_int8(&r);
      }
    }

    for(right_shift_t sat = 3; sat > -5; sat--){

      // Compute the reference
      dbl_dct8x8(ref_out, ref_in, sat);
      
      // printf("x = np.array([ ");
      // for(int row = 0; row < 8; row++){
      //   printf("  [ ");
      //   for(int col = 0; col < 8; col++)  printf("%d, ", x[row][col]);
      //   printf(" ],\n");
      // }
      // printf("])\n");

      unsigned ts1 = getTimestamp();
      headroom_t reported_hr = dct8x8_forward(y, x, sat);
      unsigned ts2 = getTimestamp();
      
      // printf("y = np.array([ ");
      // for(int row = 0; row < 8; row++){
      //   printf("  [ ");
      //   for(int col = 0; col < 8; col++) printf("%d, ", y[row][col]);
      //   printf(" ],\n");
      // }
      // printf("])\n");
      
      // printf("ref = np.array([ ");
      // for(int row = 0; row < 8; row++){
      //   printf("  [ ");
      //   for(int col = 0; col < 8; col++) printf("%0.00f, ", ref_out[row][col]);
      //   printf(" ],\n");
      // }
      // printf("])\n");
      
      float max_allowed_diff = 1;
      headroom_t actual_hr = 8;

      for(int row = 0; row < 8; row++){
        for(unsigned col = 0; col < 8; col++){
          float ref_val = (float) ref_out[row][col];
          float act_val = y[row][col];

          if(ref_val > 127.0f)  ref_val =  127.0f;
          if(ref_val < -128.0f) ref_val = -128.0f;

          TEST_ASSERT_FLOAT_WITHIN(max_allowed_diff, ref_val, act_val);
        
          // NOTE: currently no vect_s8_headroom() function.
          headroom_t hr = HR_S8(y[row][col]);
          actual_hr = MIN(actual_hr, hr);
        }
      }

      TEST_ASSERT_EQUAL(actual_hr, reported_hr);

      float timing = (float) ((ts2-ts1)/100.0);
      if(timing > worst_timing) worst_timing = timing;
    }
  }

#if TIME_FUNCS
    printf("    %s (8x8-point): %f us\n", FUNC_NAME, worst_timing);
#endif

#undef FUNC_NAME
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

TEST(dct8x8, dct8x8_inverse)
{
#define FUNC_NAME "dct8x8_inverse"

#if PRINT_FUNC_NAMES
  printf("\n%s..\n", FUNC_NAME);
#endif

  unsigned r = 1;

  float worst_timing = 0.0f;
  
  for(unsigned t = 0; t < (1<<LOOPS_LOG2); t++){
  
    DWORD_ALIGNED int8_t x[8][8];
    DWORD_ALIGNED int8_t y[8][8];

    double ref_in[8][8];
    double ref_out[8][8];

    for(int row = 0; row < 8; row++){
      for(unsigned col = 0; col < 8; col++){
        ref_in[row][col] = x[row][col] = pseudo_rand_int8(&r);
      }
    }

    for(right_shift_t sat = 3; sat > -5; sat--){

      // Compute the reference
      dbl_idct8x8(ref_out, ref_in, sat);
      
      // printf("x = np.array([ ");
      // for(int row = 0; row < 8; row++){
      //   printf("  [ ");
      //   for(int col = 0; col < 8; col++)  printf("%d, ", x[row][col]);
      //   printf(" ],\n");
      // }
      // printf("])\n");

      unsigned ts1 = getTimestamp();
      headroom_t reported_hr = dct8x8_inverse(y, x, sat);
      unsigned ts2 = getTimestamp();
      
      // printf("y = np.array([ ");
      // for(int row = 0; row < 8; row++){
      //   printf("  [ ");
      //   for(int col = 0; col < 8; col++) printf("%d, ", y[row][col]);
      //   printf(" ],\n");
      // }
      // printf("])\n");
      
      // printf("ref = np.array([ ");
      // for(int row = 0; row < 8; row++){
      //   printf("  [ ");
      //   for(int col = 0; col < 8; col++) printf("%0.00f, ", ref_out[row][col]);
      //   printf(" ],\n");
      // }
      // printf("])\n");
      
      float max_allowed_diff = 1;
      headroom_t actual_hr = 8;

      for(int row = 0; row < 8; row++){
        for(unsigned col = 0; col < 8; col++){
          float ref_val = (float) ref_out[row][col];
          float act_val = y[row][col];

          if(ref_val > 127.0f)  ref_val =  127.0f;
          if(ref_val < -128.0f) ref_val = -128.0f;

          TEST_ASSERT_FLOAT_WITHIN(max_allowed_diff, ref_val, act_val);
        
          // NOTE: currently no vect_s8_headroom() function.
          headroom_t hr = HR_S8(y[row][col]);
          actual_hr = MIN(actual_hr, hr);
        }
      }

      TEST_ASSERT_EQUAL(actual_hr, reported_hr);

      float timing = (float) ((ts2-ts1)/100.0);
      if(timing > worst_timing) worst_timing = timing;
    }
  }

#if TIME_FUNCS
    printf("    %s (8x8-point): %f us\n", FUNC_NAME, worst_timing);
#endif

#undef FUNC_NAME
}


