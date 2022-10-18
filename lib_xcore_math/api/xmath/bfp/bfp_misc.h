// Copyright 2021-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include "xmath/types.h"



/**
 * 
 */
C_API
void bfp_complex_s32_gradient_constraint_mono(
    bfp_complex_s32_t* x,
    const unsigned frame_advance);

C_API
void bfp_complex_s32_gradient_constraint_stereo(
    bfp_complex_s32_t* X1,
    bfp_complex_s32_t* X2,
    const unsigned frame_advance);