// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>

#include "xmath/xmath.h"


#define REP8(X)  X,X,X,X,X,X,X,X

const int32_t exp_small_coef[] = {
  REP8(0x40000000),
  REP8(0x40000000),
  REP8(0x20000000),
  REP8(0x0AAAAAAB),
  REP8(0x02AAAAAB),
  REP8(0x00888889),
  REP8(0x0016C16C),
  REP8(0x00034034),
  REP8(0x00006807),
  REP8(0x00000B8F),
};
const unsigned exp_small_term_count = 10;

void chunk_q30_exp_small(
    q2_30 a[VPU_INT32_EPV],
    const q2_30 b[VPU_INT32_EPV])
{
  chunk_q30_power_series(a, b, exp_small_coef, exp_small_term_count);
}