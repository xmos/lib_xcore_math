// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xmath/xmath.h"

#include "../../tst_common.h"

#include "unity_fixture.h"

TEST_GROUP_RUNNER(bfp_alloc) {
  RUN_TEST_CASE(bfp_alloc, bfp_s16_alloc);
  RUN_TEST_CASE(bfp_alloc, bfp_s32_alloc);
  RUN_TEST_CASE(bfp_alloc, bfp_complex_s16_alloc);
  RUN_TEST_CASE(bfp_alloc, bfp_complex_s32_alloc);
}

TEST_GROUP(bfp_alloc);
TEST_SETUP(bfp_alloc) { fflush(stdout); }
TEST_TEAR_DOWN(bfp_alloc) {}



#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (128)
#else
#  define REPS       (1000)
#  define MAX_LEN    (512)
#endif

TEST(bfp_alloc, bfp_s32_alloc) 
{

  unsigned seed = SEED_FROM_FUNC_NAME();

  for(int r = 0; r < REPS; r++){

      unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
      setExtraInfo_RSL(r, seed, length);

      bfp_s32_t vec = bfp_s32_alloc( length );

      // If a test assertion fails, it immediately returns, so this prevents a memory leak
      if(vec.data != NULL)
        free(vec.data);

      // Allocation succeeded
      TEST_ASSERT_NOT_NULL( vec.data );
      // Length is correct
      TEST_ASSERT_EQUAL( length, vec.length );

#if __xcore__
      // d-word-aligned (xcore only)
      TEST_ASSERT_EQUAL(0, ((unsigned)vec.data) % 8);
#endif

      // Flags are correct
      TEST_ASSERT_EQUAL( BFP_FLAG_DYNAMIC, vec.flags );

      //NOTE: we don't have a way to check that bfp_s32_alloc() allocates 2 extra elements
  }
}


TEST(bfp_alloc, bfp_s16_alloc) 
{

  unsigned seed = SEED_FROM_FUNC_NAME();

  for(int r = 0; r < REPS; r++){

      unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
      setExtraInfo_RSL(r, seed, length);

      bfp_s16_t vec = bfp_s16_alloc( length );

      // If a test assertion fails, it immediately returns, so this prevents a memory leak
      if(vec.data != NULL)
        free(vec.data);

      // Allocation succeeded
      TEST_ASSERT_NOT_NULL( vec.data );
      // Length is correct
      TEST_ASSERT_EQUAL( length, vec.length );

#if __xcore__
      // d-word-aligned (xcore only)
      TEST_ASSERT_EQUAL(0, ((unsigned)vec.data) % 8);
#endif

      // Flags are correct
      TEST_ASSERT_EQUAL( BFP_FLAG_DYNAMIC, vec.flags );
  }
}


TEST(bfp_alloc, bfp_complex_s32_alloc) 
{

  unsigned seed = SEED_FROM_FUNC_NAME();

  for(int r = 0; r < REPS; r++){

      unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
      setExtraInfo_RSL(r, seed, length);

      bfp_complex_s32_t vec = bfp_complex_s32_alloc( length );

      // If a test assertion fails, it immediately returns, so this prevents a memory leak
      if(vec.data != NULL)
        free(vec.data);

      // Allocation succeeded
      TEST_ASSERT_NOT_NULL( vec.data );
      // Length is correct
      TEST_ASSERT_EQUAL( length, vec.length );
      
#if __xcore__
      // d-word-aligned (xcore only)
      TEST_ASSERT_EQUAL(0, ((unsigned)vec.data) % 8);
#endif

      // Flags are correct
      TEST_ASSERT_EQUAL( BFP_FLAG_DYNAMIC, vec.flags );
  }
}


TEST(bfp_alloc, bfp_complex_s16_alloc) 
{

  unsigned seed = SEED_FROM_FUNC_NAME();

  for(int r = 0; r < REPS; r++){

      unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
      setExtraInfo_RSL(r, seed, length);

      bfp_complex_s16_t vec = bfp_complex_s16_alloc( length );

      // If a test assertion fails, it immediately returns, so this prevents a memory leak
      if(vec.real != NULL)
        free(vec.real);
        
      // Allocation succeeded
      TEST_ASSERT_NOT_NULL( vec.real );
      
#if __xcore__
      // imag should always be word-aligned. (xcore only)
      TEST_ASSERT_EQUAL_MESSAGE(0, ((unsigned)vec.imag) % 4, "imag not word-aligned");
      // imag should be real offset by length elements, rounded up to the nearest word-aligned address
      int16_t* exp_imag = &vec.real[length + ((length & 1)? 1 : 0)];
      TEST_ASSERT_POINTERS_EQUAL(exp_imag, vec.imag);
#endif

      // Length is correct
      TEST_ASSERT_EQUAL( length, vec.length );
      
#if __xcore__
      // d-word-aligned (xcore only)
      TEST_ASSERT_EQUAL(0, ((unsigned)vec.real) % 8);
#endif

      // Flags are correct
      TEST_ASSERT_EQUAL( BFP_FLAG_DYNAMIC, vec.flags );
  }
}