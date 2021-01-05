

#ifndef XS3_VPU_SCALAR_OPS_H_
#define XS3_VPU_SCALAR_OPS_H_

#include <stdint.h>

#include "xs3_math_types.h"
#include "xs3_vpu.h"

/**
 * @file xs3_vpu_scalar_ops.h
 * 
 * These functions implement the behavior of various XS3 VPU instructions in 8-, 16- or 32-bit modes.
 * 
 * @warning This is not considered to be part of the public API.
 */


/**
 * Implements the logic of the VLADD instruction in 8-bit mode.
 */
int8_t vladd8(
    const int8_t x, 
    const int8_t y);

/**
 * Implements the logic of hte VLSUB instruction in 8-bit mode.
 */
int8_t vlsub8(
    const int8_t x, 
    const int8_t y);

/**
 * Implements the logic of the VLASHR instruction in 8-bit mode.
 */
int8_t vlashr8(
    const int8_t x,
    const right_shift_t shr);

/**
 * Implements the logic of the VPOS instruction in 8-bit mode.
 */
int8_t vpos8(
    const int8_t x);

/**
 * Implements the logic of the VISGN instruction in 8-bit mode.
 */
int8_t vsign8(
    const int8_t x);

/**
 * Implements the logic of the VDEPTH1 instruction in 8-bit mode.
 */
unsigned vdepth1_8(
    const int8_t x);

/**
 * Implements the logic of the VLMUL instruction in 8-bit mode.
 */
int8_t vlmul8(
    const int8_t x,
    const int8_t y);

/**
 * Implements the logic of the VLMACC instruction in 8-bit mode.
 */
vpu_int8_acc_t vlmacc8(
    const vpu_int8_acc_t acc,
    const int8_t x,
    const int8_t y);

/**
 * Implements the logic of the VLMACCR instruction in 8-bit mode.
 */
vpu_int8_acc_t vlmaccr8(
    const vpu_int8_acc_t acc,
    const int8_t x[VPU_INT8_EPV],
    const int8_t y[VPU_INT8_EPV]);

/**
 * Implements the logic of the VLSAT instruction in 8-bit mode.
 */
int8_t vlsat8(
    const vpu_int8_acc_t acc,
    const unsigned sat);






/**
 * Implements the logic of the VLADD instruction in 16-bit mode.
 */
int16_t vladd16(
    const int16_t x, 
    const int16_t y);

/**
 * Implements the logic of the VLSUB instruction in 16-bit mode.
 */
int16_t vlsub16(
    const int16_t x, 
    const int16_t y);

/**
 * Implements the logic of the VLASHR instruction in 16-bit mode.
 */
int16_t vlashr16(
    const int16_t x,
    const right_shift_t shr);

/**
 * Implements the logic of the VPOS instruction in 16-bit mode.
 */
int16_t vpos16(
    const int16_t x);

/**
 * Implements the logic of the VSIGN instruction in 16-bit mode.
 */
int16_t vsign16(
    const int16_t x);

/**
 * Implements the logic of the VDEPTH1 instruction in 16-bit mode.
 */
unsigned vdepth1_16(
    const int16_t x);

/**
 * Implements the logic of the VDEPTH8 instruction in 16-bit mode.
 */
int8_t vdepth8_16(
    const int16_t x);

/**
 * Implements the logic of the VLMUL instruction in 16-bit mode.
 */
int16_t vlmul16(
    const int16_t x,
    const int16_t y);

/**
 * Implements the logic of the VLMACC instruction in 16-bit mode.
 */
vpu_int16_acc_t vlmacc16(
    const vpu_int16_acc_t acc,
    const int16_t x,
    const int16_t y);

/**
 * Implements the logic of the VLMACCR instruction in 16-bit mode.
 */
vpu_int16_acc_t vlmaccr16(
    const vpu_int16_acc_t acc,
    const int16_t x[VPU_INT16_EPV],
    const int16_t y[VPU_INT16_EPV]);

/**
 * Implements the logic of the VLSAT instruction in 16-bit mode.
 */
int16_t vlsat16(
    const vpu_int16_acc_t acc,
    const unsigned sat);








/**
 * Implements the logic of the VLADD instruction in 32-bit mode.
 */
int32_t vladd32(
    const int32_t x, 
    const int32_t y);

/**
 * Implements the logic of the VLSUB instruction in 32-bit mode.
 */
int32_t vlsub32(
    const int32_t x, 
    const int32_t y);

/**
 * Implements the logic of the VLASHR instruction in 32-bit mode.
 */
int32_t vlashr32(
    const int32_t x,
    const right_shift_t shr);

/**
 * Implements the logic of the VPOS instruction in 32-bit mode.
 */
int32_t vpos32(
    const int32_t x);

/**
 * Implements the logic of the VSIGN instruction in 32-bit mode.
 */
int32_t vsign32(
    const int32_t x);

/**
 * Implements the logic of the VDEPTH1 instruction in 32-bit mode.
 */
unsigned vdepth1_32(
    const int32_t x);

/**
 * Implements the logic of the VDEPTH8 instruction in 32-bit mode.
 */
int8_t vdepth8_32(
    const int32_t x);

/**
 * Implements the logic of the VDEPTH16 instruction in 32-bit mode.
 */
int16_t vdepth16_32(
    const int32_t x);

/**
 * Implements the logic of the VLMUL instruction in 32-bit mode.
 */
int32_t vlmul32(
    const int32_t x,
    const int32_t y);

/**
 * Implements the logic of the VLMACC instruction in 32-bit mode.
 */
vpu_int32_acc_t vlmacc32(
    const vpu_int32_acc_t acc,
    const int32_t x,
    const int32_t y);

/**
 * Implements the logic of the VLMACCR instruction in 32-bit mode.
 */
vpu_int32_acc_t vlmaccr32(
    const vpu_int32_acc_t acc,
    const int32_t x[VPU_INT32_EPV],
    const int32_t y[VPU_INT32_EPV]);

/**
 * Implements the logic of the VLSAT instruction in 32-bit mode.
 */
int32_t vlsat32(
    const vpu_int32_acc_t acc,
    const unsigned sat);

/**
 * Implements the logic of the VCMR instruction in 32-bit mode.
 */
int32_t vcmr32(
    const complex_s32_t vD,
    const complex_s32_t vC);

/**
 * Implements the logic of the VCMI instruction in 32-bit mode.
 */
int32_t vcmi32(
    const complex_s32_t vD,
    const complex_s32_t vC);

/**
 * Implements the logic of the VCMCR instruction in 32-bit mode.
 */
int32_t vcmcr32(
    const complex_s32_t vD,
    const complex_s32_t vC);

/**
 * Implements the logic of the VCMCI instruction in 32-bit mode.
 */
int32_t vcmci32(
    const complex_s32_t vD,
    const complex_s32_t vC);





#endif //XS3_VPU_SCALAR_OPS_H_