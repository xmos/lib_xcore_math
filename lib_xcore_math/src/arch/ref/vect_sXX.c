// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>
#include <string.h>


#include "xmath/xmath.h"
#include "vpu_helper.h"
#include "xmath/xs3/vpu_scalar_ops.h"
#include "vpu_const_vects.h"

typedef union {
  int32_t s32[VPU_INT32_EPV];
  int16_t s16[VPU_INT16_EPV];
  int8_t s8[VPU_INT8_EPV];
  complex_s32_t complex_s32[VPU_INT32_EPV/2];
} vpu_vector_t;


static inline void add_scalar_s32(
    vpu_vector_t* a,
    const vpu_vector_t* b,
    const vpu_vector_t* c,
    const right_shift_t b_shr)
{
  for(int k = 0; k < VPU_INT32_EPV; k++){
    int32_t T = vlashr32(b->s32[k], b_shr);
    a->s32[k] = vladd32(T, c->s32[k]);
  }
}

static inline void add_scalar_s16(
    vpu_vector_t* a,
    const vpu_vector_t* b,
    const vpu_vector_t* c,
    const right_shift_t b_shr)
{
  for(int k = 0; k < VPU_INT16_EPV; k++){
    int16_t T = vlashr16(b->s16[k], b_shr);
    a->s16[k] = vladd16(T, c->s16[k]);
  }
}

static inline void add_scalar_s8(
    vpu_vector_t* a,
    const vpu_vector_t* b,
    const vpu_vector_t* c,
    const right_shift_t b_shr)
{
  for(int k = 0; k < VPU_INT8_EPV; k++){
    int8_t T = vlashr8(b->s8[k], b_shr);
    a->s8[k] = vladd8(T, c->s8[k]);
  }
}


unsigned vect_sXX_add_scalar(
    int32_t a[],
    const int32_t b[],
    const unsigned length_bytes,
    const int32_t c,
    const int32_t d,
    const right_shift_t b_shr,
    const unsigned mode_bits)
{

  vpu_vector_t vec_c;

  for(int k = 0; k < VPU_INT32_EPV; k+=2)
    vec_c.s32[k] = c;
  for(int k = 1; k < VPU_INT32_EPV; k+=2)
    vec_c.s32[k] = d;

  vpu_vector_t* vec_a = (vpu_vector_t*) &a[0];
  vpu_vector_t* vec_b = (vpu_vector_t*) &b[0];
  vpu_vector_t vec_tmp;

  unsigned bytes_left = length_bytes;

  while( bytes_left > 0 ){

    switch(mode_bits){
      case 0x0000: // 32-bit
        add_scalar_s32(&vec_tmp, vec_b, &vec_c, b_shr);
        break;
      case 0x0100: // 16-bit
        add_scalar_s16(&vec_tmp, vec_b, &vec_c, b_shr);
        break;
      case 0x0200: // 8-bit
        assert(0); // Not supported on x86 at the moment.
        add_scalar_s8(&vec_tmp, vec_b, &vec_c, b_shr);
        break;
      default:
        assert(0); // shouldn't happen
    }

    const unsigned bytes = MIN(VPU_INT8_EPV, bytes_left);

    memcpy(vec_a, &vec_tmp, bytes);
    bytes_left -= bytes;
    vec_a = &vec_a[1];
    vec_b = &vec_b[1];
  }

  switch(mode_bits){
    case 0x0000: // 32-bit
      return 31 - vect_s32_headroom(a, length_bytes >> 2);
      break;
    case 0x0100: // 16-bit
      return 15 - vect_s16_headroom((void*) a, length_bytes >> 1);
      break;
    case 0x0200: // 8-bit
      // @todo: There is currently no vect_s8_headroom()
      assert(0); // Not supported on x86 at the moment.
      return 0; // add return value to avoid warning C4715: 'vect_sXX_add_scalar': not all control paths return a value
      // return 7 - vect_s8_headroom((void*) a, length_bytes);
      break;
    default:
      assert(0); // shouldn't happen
      return 0; // add return value to avoid warning C4715: 'vect_sXX_add_scalar': not all control paths return a value
  }
}
