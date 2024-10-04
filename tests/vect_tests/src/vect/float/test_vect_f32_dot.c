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




TEST_GROUP_RUNNER(vect_f32_dot) {
  RUN_TEST_CASE(vect_f32_dot, vect_f32_dot);
}

TEST_GROUP(vect_f32_dot);
TEST_SETUP(vect_f32_dot) { fflush(stdout); }
TEST_TEAR_DOWN(vect_f32_dot) {}


#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (64)
#else
#  define REPS       (1000)
#  define MAX_LEN    (256)
#endif


TEST(vect_f32_dot, vect_f32_dot)
{
  unsigned seed = SEED_FROM_FUNC_NAME();
  // seed = 0x571AC451;

  DWORD_ALIGNED
  float b[MAX_LEN];
  DWORD_ALIGNED
  float c[MAX_LEN];

  // vect_f32_dot() traces different code paths depending upon the
  // alignment of the two input vectors, so we want to try each combination
  // of WORD and DWORD alignment for each vector.
  for(unsigned B_OFF = 0; B_OFF <= 1; B_OFF++){
    for(unsigned C_OFF = 0; C_OFF <= 1; C_OFF++){

      for(int v = 0; v < REPS/4; v++){
        const unsigned old_seed = seed;

        unsigned len = pseudo_rand_uint(&seed, 1, MAX_LEN-1);
        setExtraInfo_RSL(v, old_seed, len);
        // printf("0x%08X\n", old_seed);

        for(unsigned int k = 0; k < len; k++){
          // The mask being applied is to avoid a divergence between the
          // implementation of this function using the FMACC instruction, and the
          // implementation using FMUL+FADD.  The former keeps full precision
          // of the product to be added.  But I want this test to be agnostic to
          // the implementation. Masking off LSbs helps mitigate that divergence
          // without compromising the integrity of the test.

          b[k+B_OFF] = ldexpf((float) (pseudo_rand_int32(&seed) & 0xFFFFFF00),
                              pseudo_rand_int(&seed, -2, 2));
          c[k+C_OFF] = ldexpf((float) (pseudo_rand_int32(&seed) & 0xFFFFFF00),
                              pseudo_rand_int(&seed, -2, 2));
        }
        
        float result = vect_f32_dot(&b[B_OFF], &c[C_OFF], len);

        float expected = 0.0f;
        for(int k = len-1; k >= 0; k--){
          const float BB = b[k+B_OFF];
          const float CC = c[k+C_OFF];
          expected += BB * CC;
        }

        float ppo = (expected - result)/expected;

        TEST_ASSERT_FLOAT_WITHIN_MESSAGE(ldexpf(1,-14), 0.0f, ppo, "");
      }
    }
  }
}



