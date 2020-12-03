

#ifndef XS3_VPU_SCALAR_OPS_H_
#define XS3_VPU_SCALAR_OPS_H_

#include <stdint.h>

#include "xs3_math_types.h"
#include "xs3_vpu.h"


/**
 * @todo
 */
int8_t vladd8(
    const int8_t x, 
    const int8_t y);

/**
 * @todo
 */
int8_t vlsub8(
    const int8_t x, 
    const int8_t y);

/**
 * @todo
 */
int8_t vlashr8(
    const int8_t x,
    const right_shift_t shr);

/**
 * @todo
 */
int8_t vpos8(
    const int8_t x);

/**
 * @todo
 */
int8_t vsign8(
    const int8_t x);

/**
 * @todo
 */
unsigned vdepth1_8(
    const int8_t x);

/**
 * @todo
 */
int8_t vlmul8(
    const int8_t x,
    const int8_t y);

/**
 * @todo
 */
vpu_int8_acc_t vlmacc8(
    const vpu_int8_acc_t acc,
    const int8_t x,
    const int8_t y);

/**
 * @todo
 */
vpu_int8_acc_t vlmaccr8(
    const vpu_int8_acc_t acc,
    const int8_t x[VPU_INT8_EPV],
    const int8_t y[VPU_INT8_EPV]);

/**
 * @todo
 */
int8_t vlsat8(
    const vpu_int8_acc_t acc,
    const unsigned sat);





/**
 * @todo
 */
int16_t vladd16(
    const int16_t x, 
    const int16_t y);

/**
 * @todo
 */
int16_t vlsub16(
    const int16_t x, 
    const int16_t y);

/**
 * @todo
 */
int16_t vlashr16(
    const int16_t x,
    const right_shift_t shr);

/**
 * @todo
 */
int16_t vpos16(
    const int16_t x);

/**
 * @todo
 */
int16_t vsign16(
    const int16_t x);

/**
 * @todo
 */
unsigned vdepth1_16(
    const int16_t x);

/**
 * @todo
 */
int8_t vdepth8_16(
    const int16_t x);

/**
 * @todo
 */
int16_t vlmul16(
    const int16_t x,
    const int16_t y);

/**
 * @todo
 */
vpu_int16_acc_t vlmacc16(
    const vpu_int16_acc_t acc,
    const int16_t x,
    const int16_t y);

/**
 * @todo
 */
vpu_int16_acc_t vlmaccr16(
    const vpu_int16_acc_t acc,
    const int16_t x[VPU_INT16_EPV],
    const int16_t y[VPU_INT16_EPV]);

/**
 * @todo
 */
int16_t vlsat16(
    const vpu_int16_acc_t acc,
    const unsigned sat);






/**
 * @todo
 */
int32_t vladd32(
    const int32_t x, 
    const int32_t y);

/**
 * @todo
 */
int32_t vlsub32(
    const int32_t x, 
    const int32_t y);

/**
 * @todo
 */
int32_t vlashr32(
    const int32_t x,
    const right_shift_t shr);

/**
 * @todo
 */
int32_t vpos32(
    const int32_t x);

/**
 * @todo
 */
int32_t vsign32(
    const int32_t x);

/**
 * @todo
 */
unsigned vdepth1_32(
    const int32_t x);

/**
 * @todo
 */
int8_t vdepth8_32(
    const int32_t x);

/**
 * @todo
 */
int16_t vdepth16_32(
    const int32_t x);

/**
 * @todo
 */
int32_t vlmul32(
    const int32_t x,
    const int32_t y);

/**
 * @todo
 */
vpu_int32_acc_t vlmacc32(
    const vpu_int32_acc_t acc,
    const int32_t x,
    const int32_t y);

/**
 * @todo
 */
vpu_int32_acc_t vlmaccr32(
    const vpu_int32_acc_t acc,
    const int32_t x[VPU_INT32_EPV],
    const int32_t y[VPU_INT32_EPV]);

/**
 * @todo
 */
int32_t vlsat32(
    const vpu_int32_acc_t acc,
    const unsigned sat);

/**
 * @todo
 */
int32_t vcmr32(
    const complex_s32_t vD,
    const complex_s32_t vC);

/**
 * @todo
 */
int32_t vcmi32(
    const complex_s32_t vD,
    const complex_s32_t vC);

/**
 * @todo
 */
int32_t vcmcr32(
    const complex_s32_t vD,
    const complex_s32_t vC);

/**
 * @todo
 */
int32_t vcmci32(
    const complex_s32_t vD,
    const complex_s32_t vC);





#endif //XS3_VPU_SCALAR_OPS_H_