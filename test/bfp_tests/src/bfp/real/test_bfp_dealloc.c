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


TEST_GROUP_RUNNER(bfp_dealloc) {
  RUN_TEST_CASE(bfp_dealloc, bfp_s32_dealloc);
  RUN_TEST_CASE(bfp_dealloc, bfp_s16_dealloc);
  RUN_TEST_CASE(bfp_dealloc, bfp_complex_s32_dealloc);
  RUN_TEST_CASE(bfp_dealloc, bfp_complex_s16_dealloc);

  RUN_TEST_CASE(bfp_dealloc, bfp_s32_dealloc_static);
  RUN_TEST_CASE(bfp_dealloc, bfp_s16_dealloc_static);
  RUN_TEST_CASE(bfp_dealloc, bfp_complex_s32_dealloc_static);
  RUN_TEST_CASE(bfp_dealloc, bfp_complex_s16_dealloc_static);
}


TEST_GROUP(bfp_dealloc);
TEST_SETUP(bfp_dealloc) { fflush(stdout); }
TEST_TEAR_DOWN(bfp_dealloc) {}


#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (128)
#else
#  define REPS       (1000)
#  define MAX_LEN    (512)
#endif


TEST(bfp_dealloc, bfp_s32_dealloc) 
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  for(int r = 0; r < REPS; r++){

      unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
      setExtraInfo_RSL(r, seed, length);

      bfp_s32_t vec = bfp_s32_alloc( length );

      // If NULL it failed to allocate (and so memory won't leak if this fails)
      TEST_ASSERT_NOT_NULL(vec.data);

      bfp_s32_dealloc(&vec);

      // data, length and flags must be zeroed if it succeeded
      TEST_ASSERT_NULL(vec.data);
      TEST_ASSERT_EQUAL_INT(0, vec.length);
      TEST_ASSERT_EQUAL_INT(0, vec.flags);
  }
}

TEST(bfp_dealloc, bfp_s16_dealloc) 
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  for(int r = 0; r < REPS; r++){

      unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
      setExtraInfo_RSL(r, seed, length);

      bfp_s16_t vec = bfp_s16_alloc( length );

      // If NULL it failed to allocate (and so memory won't leak if this fails)
      TEST_ASSERT_NOT_NULL(vec.data);

      bfp_s16_dealloc(&vec);

      // data, length and flags must be zeroed if it succeeded
      TEST_ASSERT_NULL(vec.data);
      TEST_ASSERT_EQUAL_INT(0, vec.length);
      TEST_ASSERT_EQUAL_INT(0, vec.flags);
  }
}


TEST(bfp_dealloc, bfp_complex_s32_dealloc) 
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  for(int r = 0; r < REPS; r++){

      unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
      setExtraInfo_RSL(r, seed, length);

      bfp_complex_s32_t vec = bfp_complex_s32_alloc( length );

      // If NULL it failed to allocate (and so memory won't leak if this fails)
      TEST_ASSERT_NOT_NULL(vec.data);

      bfp_complex_s32_dealloc(&vec);

      // data, length and flags must be zeroed if it succeeded
      TEST_ASSERT_NULL(vec.data);
      TEST_ASSERT_EQUAL_INT(0, vec.length);
      TEST_ASSERT_EQUAL_INT(0, vec.flags);
  }
}


TEST(bfp_dealloc, bfp_complex_s16_dealloc) 
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  for(int r = 0; r < REPS; r++){

      unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
      setExtraInfo_RSL(r, seed, length);

      bfp_complex_s16_t vec = bfp_complex_s16_alloc( length );

      // If NULL it failed to allocate (and so memory won't leak if this fails)
      TEST_ASSERT_NOT_NULL(vec.real);

      bfp_complex_s16_dealloc(&vec);

      // real, imag, length and flags must be zeroed if it succeeded
      TEST_ASSERT_NULL(vec.real);
      TEST_ASSERT_NULL(vec.imag);
      TEST_ASSERT_EQUAL_INT(0, vec.length);
      TEST_ASSERT_EQUAL_INT(0, vec.flags);
  }
}


TEST(bfp_dealloc, bfp_s32_dealloc_static) 
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  int32_t WORD_ALIGNED buff[MAX_LEN];
  bfp_s32_t vec;

  for(int r = 0; r < REPS; r++){

      unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
      setExtraInfo_RSL(r, seed, length);
      
      bfp_s32_init(&vec, buff, 0, length, 0);

      bfp_s32_t expect = vec;

      // This call should not do anything to vec
      bfp_s32_dealloc(&vec);

      // expect and vec should be identical.
      TEST_ASSERT_POINTERS_EQUAL(expect.data, vec.data);
      TEST_ASSERT_EQUAL_INT(expect.length, vec.length);
      TEST_ASSERT_EQUAL_INT(expect.flags, vec.flags);
      TEST_ASSERT_EQUAL_INT(expect.exp, vec.exp);
      TEST_ASSERT_EQUAL_INT(expect.hr, vec.hr);

  }
}


TEST(bfp_dealloc, bfp_s16_dealloc_static) 
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  int16_t WORD_ALIGNED buff[MAX_LEN];
  bfp_s16_t vec;

  for(int r = 0; r < REPS; r++){

      unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
      setExtraInfo_RSL(r, seed, length);
      
      bfp_s16_init(&vec, buff, 0, length, 0);

      bfp_s16_t expect = vec;

      // This call should not do anything to vec
      bfp_s16_dealloc(&vec);

      // expect and vec should be identical.
      TEST_ASSERT_POINTERS_EQUAL(expect.data, vec.data);
      TEST_ASSERT_EQUAL_INT(expect.length, vec.length);
      TEST_ASSERT_EQUAL_INT(expect.flags, vec.flags);
      TEST_ASSERT_EQUAL_INT(expect.exp, vec.exp);
      TEST_ASSERT_EQUAL_INT(expect.hr, vec.hr);

  }
}


TEST(bfp_dealloc, bfp_complex_s32_dealloc_static) 
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  complex_s32_t WORD_ALIGNED buff[MAX_LEN];
  bfp_complex_s32_t vec;

  for(int r = 0; r < REPS; r++){

      unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
      setExtraInfo_RSL(r, seed, length);
      
      bfp_complex_s32_init(&vec, buff, 0, length, 0);

      bfp_complex_s32_t expect = vec;

      // This call should not do anything to vec
      bfp_complex_s32_dealloc(&vec);

      // expect and vec should be identical.
      TEST_ASSERT_POINTERS_EQUAL(expect.data, vec.data);
      TEST_ASSERT_EQUAL_INT(expect.length, vec.length);
      TEST_ASSERT_EQUAL_INT(expect.flags, vec.flags);
      TEST_ASSERT_EQUAL_INT(expect.exp, vec.exp);
      TEST_ASSERT_EQUAL_INT(expect.hr, vec.hr);

  }
}


TEST(bfp_dealloc, bfp_complex_s16_dealloc_static) 
{
  unsigned seed = SEED_FROM_FUNC_NAME();

  int16_t WORD_ALIGNED real_buff[MAX_LEN];
  int16_t WORD_ALIGNED imag_buff[MAX_LEN];
  bfp_complex_s16_t vec;

  for(int r = 0; r < REPS; r++){

      unsigned length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
      setExtraInfo_RSL(r, seed, length);
      
      bfp_complex_s16_init(&vec, real_buff, imag_buff, 0, length, 0);

      bfp_complex_s16_t expect = vec;

      // This call should not do anything to vec
      bfp_complex_s16_dealloc(&vec);

      // expect and vec should be identical.
      TEST_ASSERT_POINTERS_EQUAL(expect.real, vec.real);
      TEST_ASSERT_POINTERS_EQUAL(expect.imag, vec.imag);
      TEST_ASSERT_EQUAL_INT(expect.length, vec.length);
      TEST_ASSERT_EQUAL_INT(expect.flags, vec.flags);
      TEST_ASSERT_EQUAL_INT(expect.exp, vec.exp);
      TEST_ASSERT_EQUAL_INT(expect.hr, vec.hr);

  }
}