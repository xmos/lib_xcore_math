// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>

#include "xmath/xmath.h"
#include "vpu_helper.h"
#include "xmath/xs3/vpu_scalar_ops.h"






unsigned chunk_s16_accumulate(
    split_acc_s32_t* acc,
    const int16_t b[VPU_INT16_EPV],
    const right_shift_t b_shr,
    const unsigned vpu_ctrl)
{
  unsigned vc = vpu_ctrl & 0x1F;

  for(int k = 0; k < VPU_INT16_EPV; k++){
    int32_t hi = acc->vD[k];
    uint32_t lo = acc->vR[k];
    int32_t acc32 = (hi << 16) | lo;

    int32_t b_mod = b[k];
    if(b_shr >= 0)
      b_mod = b_mod >> b_shr;
    else
      b_mod = b_mod << (-b_shr);

     acc32 += b_mod;

     acc->vD[k] = (acc32 >> 16) & 0xFFFF;
     acc->vR[k] = acc32 & 0xFFFF;

     unsigned tmp = 15 - HR_S16(acc->vD[k]);
     vc = MAX(vc, tmp);
  }
  return vc;
}


