// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once


/**
 * @page page_xs3_vpu_info_h  xs3_vpu_info.h
 * 
 * This header contains macros and types which may be useful for interacting with the
 * XS3 VPU at a relatively low level.
 * 
 * @note This header is included automatically through `xs3_math.h` or `bfp_math.h`.
 * 
 * @ingroup xs3_math_header_file
 */


/**
 * @defgroup vpu_info XS3 VPU Info
 */

/**
 * Width of the VPU vector registers in bits.
 * 
 * @ingroup vpu_info
 */
#define XS3_VPU_VREG_WIDTH_BITS     (256)

/**
 * Width of the VPU vector registers in bytes.
 * 
 * @ingroup vpu_info
 */
#define XS3_VPU_VREG_WIDTH_BYTES    (XS3_VPU_VREG_WIDTH_BITS  >> 3)

/**
 * Width of the VPU vector registers in words.
 * 
 * @ingroup vpu_info
 */
#define XS3_VPU_VREG_WIDTH_WORDS    (XS3_VPU_VREG_WIDTH_BYTES >> 2)

/**
 * Data type (bits 11..8) values of vCTRL, the control register for the VPU.
 * 
 * @ingroup vpu_info
 */
enum {
    /** Signed 32-bit Integers */
    VSETCTRL_TYPE_INT32 = 0,
    /** Signed 16-bit Integers */
    VSETCTRL_TYPE_INT16 = 1,
    /** Signed 8-bit Integers */
    VSETCTRL_TYPE_INT8  = 2,
};

/**
 * Shift type (bits 7..6) values of vCTRL, the control register for the VPU.
 * 
 * @ingroup vpu_info
 */
enum {
    /** Do not shift on VLADSB and VFT* */
    VEC_SH0 = 0,
    /** Shift left on VLADSB and VFT* */
    VEC_SHL = 1,
    /** Shift right on VLADSB and VFT* */
    VEC_SHR = 2,
};

/**
 * The saturation bounds for signed integers in each VPU operating mode.
 * 
 * @ingroup vpu_info
 */
enum {
    /** The upper saturation bound for 8-bit elements */
    VPU_INT8_MAX =  0x7F,
    /** The lower saturation bound for 8-bit elements */
    VPU_INT8_MIN = -0x7F,

    /** The upper saturation bound for 16-bit elements */
    VPU_INT16_MAX =  0x7FFF,
    /** The lower saturation bound for 16-bit elements */
    VPU_INT16_MIN = -0x7FFF,

    /** The upper saturation bound for 32-bit elements and 32-bit accumulators */
    VPU_INT32_MAX =  0x7FFFFFFF,
    /** The lower saturation bound for 32-bit elements and 32-bit accumulators */
    VPU_INT32_MIN = -0x7FFFFFFF,

    /** The upper saturation bound for 40-bit accumulators */
    VPU_INT40_MAX =  0x7FFFFFFFFFLL,
    /** The lower saturation bound for 40-bit accumulators */
    VPU_INT40_MIN = -0x7FFFFFFFFFLL,
};

/**
 * Number of accumulator bits in each operating mode.
 * 
 * In each operating mode, the VLMACC, VLMACCR and VLSAT instructions operate on
 * an array of accumulators in the vector registers vR and vD. In each case, the
 * most significant bits are stored in vD, and the least significant bits are stored
 * in vR.
 * 
 * @ingroup vpu_info
 */
enum {
    VPU_INT8_ACC_SIZE = 32,
    VPU_INT16_ACC_SIZE = 32,
    VPU_INT32_ACC_SIZE = 40,
};

/**
 * When vD and vR contain accumulators, the values in this enum indicate how many least significant 
 * bits are stored in vR, with the remaining bits stored in vD.
 * 
 * @ingroup vpu_info
 */
enum {
    VPU_INT8_ACC_VR_BITS = 16,
    VPU_INT16_ACC_VR_BITS = 16,
    VPU_INT32_ACC_VR_BITS = 32,
};
/**
 * When vD and vR contain accumulators, the values in this enum can be used to mask off the bits of
 * the accumulator value which correspond to the portion in vR.
 * 
 * @ingroup vpu_info
 */
enum {
    VPU_INT8_ACC_VR_MASK = 0xFFFF,
    VPU_INT16_ACC_VR_MASK = 0xFFFF,
    VPU_INT32_ACC_VR_MASK = 0xFFFFFFFF,
};

/**
 * Integer type which fits a single accumulator (32-bits) corresponding to the 8-bit VPU mode.
 * 
 * @ingroup vpu_info
 */
typedef int32_t vpu_int8_acc_t;

/**
 * Integer type which fits a single accumulator (32-bits) corresponding to the 16-bit VPU mode.
 * 
 * @ingroup vpu_info
 */
typedef int32_t vpu_int16_acc_t;

/**
 * Integer type which fits a single accumulator (40-bits) corresponding to the 32-bit VPU mode.
 * 
 * @ingroup vpu_info
 */
typedef int64_t vpu_int32_acc_t;

/**
 * The number of elements which fit into a vector register for each operating mode.
 * 
 * This is also the number of elements which are operated on in the following 
 * instructions: VDEPTH1, VDEPTH16, VDEPTH8, VLADD, VLADDD, VLASHR, VLMACCR, VLMUL, 
 *               VLSUB, VPOS, VSIGN
 * 
 * @ingroup vpu_info
 */
enum {
    VPU_INT8_EPV    = 32,
    VPU_INT16_EPV   = 16,
    VPU_INT32_EPV   =  8,
};

/**
 * log-base-2 of the corresponding VPU_INT*_EPV values.
 * 
 * @ingroup vpu_info
 */
enum {
    VPU_INT8_EPV_LOG2    = 5,
    VPU_INT16_EPV_LOG2   = 4,
    VPU_INT32_EPV_LOG2   = 3,
};

/**
 * The number of accumulators, spread across vR and vD, in each operating mode.
 * 
 * This is also the number of elements consumed (number of multiplies) by the
 * VLMACC instruction.
 * 
 * @ingroup vpu_info
 */
enum {
    VPU_INT8_ACC_PERIOD  = 16,
    VPU_INT16_ACC_PERIOD = 16,
    VPU_INT32_ACC_PERIOD =  8,
};

/**
 * log-base-2 of the corresponding VPU_INT*_ACC_PERIOD values.
 * 
 * @ingroup vpu_info
 */
enum {
    VPU_INT8_ACC_PERIOD_LOG2  = 4,
    VPU_INT16_ACC_PERIOD_LOG2 = 4,
    VPU_INT32_ACC_PERIOD_LOG2 = 3,
};

/**
 * The number of elements consumed by a VLMACC instruction in each operating mode.
 * In other words, the number of simultaneous multiply-accumulates performed by the VLMACC
 * instruction.
 * 
 * @ingroup vpu_info
 */
enum {
    VPU_INT8_VLMACC_ELMS = 16,
    VPU_INT16_VLMACC_ELMS = 16,
    VPU_INT32_VLMACC_ELMS = 8,
};

/**
 * log-base-2 of the corresponding VPU_INT*_VLMACC_ELMS values.
 * 
 * @ingroup vpu_info
 */
enum {
    VPU_INT8_VLMACC_ELMS_LOG2 = 4,
    VPU_INT16_VLMACC_ELMS_LOG2 = 4,
    VPU_INT32_VLMACC_ELMS_LOG2 = 3,
};