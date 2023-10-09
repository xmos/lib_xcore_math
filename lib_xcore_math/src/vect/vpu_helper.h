// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include "xmath/xmath.h"
#include "xmath/xs3/vpu_info.h"


/**
 * VPU saturation logic for 8-, 16- or 32-bit values.
 *
 * Note that the bit width of the argument VAL must be larger than the target type for this to be effective.
 */
#define SAT8(VAL)      (int8_t) (((VAL) >= VPU_INT8_MAX )? VPU_INT8_MAX  : (((VAL) <= VPU_INT8_MIN )? VPU_INT8_MIN  : (VAL)))
#define SAT16(VAL)     (int16_t) (((VAL) >= VPU_INT16_MAX)? VPU_INT16_MAX : (((VAL) <= VPU_INT16_MIN)? VPU_INT16_MIN : (VAL)))
#define SAT32(VAL)     (int32_t) (((VAL) >= VPU_INT32_MAX)? VPU_INT32_MAX : (((VAL) <= VPU_INT32_MIN)? VPU_INT32_MIN : (VAL)))

/**
 * e.g.   SAT(8)(512)  -->  SAT8(512)  --> VPU_INT8_MAX --> 127
 */
#define SAT(BITS)   SAT##BITS

/**
 * Apply a rounding right-shift to VAL. Adds 2**SHR to VAL prior to right-shifting, effectively rounding.
 *
 * Note that SHR must be non-negative for this to work correctly.
 */
#define ROUND_SHR(VAL, SHR)    (((SHR)<=0)?(VAL) : (((VAL)>>((SHR)-1))+1)>>1)

/**
 * Apply a non-rounding right-shift to VAL.
 *
 * Note that SHR must be non-negative for this to work correctly.
 */
#define SHR(VAL, SHR)   ((VAL)>>SHR)

/**
 * Rounding, saturating arithmetic right shift for 8-, 16- or 32-bit values.
 *
 * SHR may be negative or non-negative. If negative, VAL is cast to larger bit depth,
 * left shift is applied, and saturation logic is applied. If non-negative, rounding
 * right shfit is applied.
 */
#define ASHR8(VAL, SHR_BITS)    ( SAT8(((SHR_BITS) >= 0)? SHR((VAL),(SHR_BITS)) : (SAT8(  ((int32_t)(VAL))<<(-(SHR_BITS)) ) )))
#define ASHR16(VAL, SHR_BITS)   (SAT16(((SHR_BITS) >= 0)? SHR((VAL),(SHR_BITS)) : (SAT16( ((int32_t)(VAL))<<(-(SHR_BITS)) ) )))
#define ASHR32(VAL, SHR_BITS)   (SAT32(((SHR_BITS) >= 0)? SHR((VAL),(SHR_BITS)) : (SAT32( ((int64_t)(VAL))<<(-(SHR_BITS)) ) )))
#define ASHR(BITS)  ASHR##BITS

/**
 * Accumulate a value into a headroom mask.
 */
#define HRMASK_ADD(MASK, V)      (MASK|(((V) >= 0)? (V) : (-V)))



static inline headroom_t hr_from_mask(unsigned hr_mask){
    (void) hr_mask;
    return 0;
}

