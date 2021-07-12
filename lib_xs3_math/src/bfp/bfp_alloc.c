// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include <stdio.h>

#include "bfp_math.h"

#include "vect/xs3_vect_s32.h"
#include "vect/xs3_vect_s16.h"

////////////
bfp_s32_t bfp_s32_alloc(
    const unsigned length)
{
#if (DEBUG)
  assert( length > 0 );
#endif // DEBUG

  bfp_s32_t bfp_vec;
  bfp_vec.data = XS3_MALLOC(sizeof(int32_t) * (length+2));
  bfp_vec.length = (bfp_vec.data == NULL)? 0 : length;
  bfp_vec.flags  = (bfp_vec.data == NULL)? 0 : BFP_FLAG_DYNAMIC;
  return bfp_vec;
}


////////////
bfp_s16_t bfp_s16_alloc(
    const unsigned length)
{
#if (DEBUG)
  assert( length > 0 );
#endif // DEBUG

  bfp_s16_t bfp_vec;
  bfp_vec.data = XS3_MALLOC(sizeof(int16_t) * (length));
  bfp_vec.length = (bfp_vec.data == NULL)? 0 : length;
  bfp_vec.flags  = (bfp_vec.data == NULL)? 0 : BFP_FLAG_DYNAMIC;
  return bfp_vec;
}


////////////
bfp_complex_s32_t bfp_complex_s32_alloc(
    const unsigned length)
{
#if (DEBUG)
  assert( length > 0 );
#endif // DEBUG

  bfp_complex_s32_t bfp_vec;
  bfp_vec.data = XS3_MALLOC(sizeof(complex_s32_t) * (length));
  bfp_vec.length = (bfp_vec.data == NULL)? 0 : length;
  bfp_vec.flags  = (bfp_vec.data == NULL)? 0 : BFP_FLAG_DYNAMIC;
  return bfp_vec;
}


////////////
bfp_complex_s16_t bfp_complex_s16_alloc(
    const unsigned length)
{
#if (DEBUG)
  assert( length > 0 );
#endif // DEBUG

  // We only want to malloc one buffer for both the real and imaginary parts.
  // To word-align the imaginary part, we may need to allocate an extra element
  const unsigned alloc_elms = 2*length + (length & 1);

  bfp_complex_s16_t bfp_vec;
  bfp_vec.real = XS3_MALLOC(sizeof(int16_t) * alloc_elms);
  bfp_vec.imag = (bfp_vec.real == NULL)? NULL : &bfp_vec.real[length + (length & 1)];
  bfp_vec.length = (bfp_vec.real == NULL)? 0 : length;
  bfp_vec.flags  = (bfp_vec.real == NULL)? 0 : BFP_FLAG_DYNAMIC;
  return bfp_vec;
}


////////////
bfp_ch_pair_s32_t bfp_ch_pair_s32_alloc(
    const unsigned length)
{
#if (DEBUG)
  assert( length > 0 );
#endif // DEBUG

  bfp_ch_pair_s32_t bfp_vec;
  bfp_vec.data = XS3_MALLOC(sizeof(ch_pair_s32_t) * (length+2));
  bfp_vec.length = (bfp_vec.data == NULL)? 0 : length;
  bfp_vec.flags  = (bfp_vec.data == NULL)? 0 : BFP_FLAG_DYNAMIC;
  return bfp_vec;
}


////////////
bfp_ch_pair_s16_t bfp_ch_pair_s16_alloc(
    const unsigned length)
{
#if (DEBUG)
  assert( length > 0 );
#endif // DEBUG

  bfp_ch_pair_s16_t bfp_vec;
  bfp_vec.data = XS3_MALLOC(sizeof(ch_pair_s32_t) * (length));
  bfp_vec.length = (bfp_vec.data == NULL)? 0 : length;
  bfp_vec.flags  = (bfp_vec.data == NULL)? 0 : BFP_FLAG_DYNAMIC;
  return bfp_vec;
}


////////////
void bfp_s32_dealloc(
    bfp_s32_t* vector)
{
  if(!(vector->flags & BFP_FLAG_DYNAMIC)) return;

  XS3_FREE(vector->data);
  vector->data = NULL;
  vector->length = 0;
  vector->flags = 0;
}


////////////
void bfp_s16_dealloc(
    bfp_s16_t* vector)
{
  if(!(vector->flags & BFP_FLAG_DYNAMIC)) return;

  XS3_FREE(vector->data);
  vector->data = NULL;
  vector->length = 0;
  vector->flags = 0;
}


////////////
void bfp_complex_s32_dealloc(
    bfp_complex_s32_t* vector)
{
  if(!(vector->flags & BFP_FLAG_DYNAMIC)) return;
  if(vector->flags & BFP_FLAG_CHAN_B) return;

  XS3_FREE(vector->data);
  vector->data = NULL;
  vector->length = 0;
  vector->flags = 0;
}


////////////
void bfp_complex_s16_dealloc(
    bfp_complex_s16_t* vector)
{
  if(!(vector->flags & BFP_FLAG_DYNAMIC)) return;

  XS3_FREE(vector->real);
  vector->real = NULL;
  vector->imag = NULL;
  vector->length = 0;
  vector->flags = 0;
}


////////////
void bfp_ch_pair_s32_dealloc(
    bfp_ch_pair_s32_t* vector)
{
  if(!(vector->flags & BFP_FLAG_DYNAMIC)) return;

  XS3_FREE(vector->data);
  vector->data = NULL;
  vector->length = 0;
  vector->flags = 0;
}


////////////
void bfp_ch_pair_s16_dealloc(
    bfp_ch_pair_s16_t* vector)
{
  if(!(vector->flags & BFP_FLAG_DYNAMIC)) return;

  XS3_FREE(vector->data);
  vector->data = NULL;
  vector->length = 0;
  vector->flags  = 0;
}
