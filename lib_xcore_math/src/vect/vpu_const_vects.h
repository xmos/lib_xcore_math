// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#pragma once

#include "xmath/types.h"

typedef union {
    int8_t s8[32];
    int16_t s16[16];
    int32_t s32[8];
    complex_s32_t c32[4];
} xs3_vpu_vect_t;

extern const xs3_vpu_vect_t vpu_vec_zero;
extern const xs3_vpu_vect_t vpu_vec_neg_1;

extern const complex_s32_t vpu_vec_complex_pos_j[4];
extern const complex_s32_t vpu_vec_complex_ones[4];
extern const complex_s32_t vpu_vec_complex_conj_op[4];
extern const complex_s32_t vpu_vec_complex_neg_j[4];
extern const complex_s32_t vpu_vec_complex_neg_ones[4];

extern const int32_t vpu_vec_0x00000001[8];
extern const int32_t vpu_vec_0x00000002[8];
extern const int32_t vpu_vec_0x00000008[8];
extern const int32_t vpu_vec_0x40000000[8];
extern const int32_t vpu_vec_neg_0x40000000[8];
extern const int32_t vpu_vec_0x20000000[8];
extern const int32_t vpu_vec_0x10000000[8];
extern const int32_t vpu_vec_0x08000000[8];
extern const int32_t vpu_vec_0x80000000[8];
extern const int32_t vpu_vec_0x7FFFFFFF[8];

extern const int16_t vpu_vec_0x0001[16];
extern const int16_t vpu_vec_0x0002[16];
extern const int16_t vpu_vec_0x0010[16];
extern const int16_t vpu_vec_0x4000[16];
extern const int16_t vpu_vec_neg_0x4000[16];
extern const int16_t vpu_vec_0x8000[16];
extern const int16_t vpu_vec_0x7FFF[16];
extern const int16_t vpu_vec_0x007F[16];
extern const int16_t vpu_vec_0x0100[16];

extern const int8_t vpu_vec_0x01[32];
extern const int8_t vpu_vec_0x40[32];
extern const int8_t vpu_vec_0x80[32];
extern const int8_t vpu_vec_0xC1[32];