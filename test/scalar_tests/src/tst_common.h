// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include <stdint.h>

#include "bfp_math.h"
#include "pseudo_rand.h"
#include "rand_frame.h"
#include "testing.h"
#include "tst_asserts.h"


#define INT32_MAX_POS(HEADROOM)    (((int32_t)0x7FFFFFFF) >> ((int)(HEADROOM)))
#define INT32_MIN_POS(HEADROOM)    (((int32_t)0x40000000) >> ((int)(HEADROOM)))
#define INT32_MAX_NEG(HEADROOM)    (((int32_t)0xBFFFFFFF) >> ((int)(HEADROOM)))
#define INT32_MIN_NEG(HEADROOM)    (((int32_t)0x80000000) >> ((int)(HEADROOM)))

#define INT16_MAX_POS(HEADROOM)    (((int16_t)0x7FFF) >> ((int)(HEADROOM)))
#define INT16_MIN_POS(HEADROOM)    (((int16_t)0x4000) >> ((int)(HEADROOM)))
#define INT16_MAX_NEG(HEADROOM)    (((int16_t)0xBFFF) >> ((int)(HEADROOM)))
#define INT16_MIN_NEG(HEADROOM)    (((int16_t)0x8000) >> ((int)(HEADROOM)))


EXTERN_C
void setExtraInfo_R(
    int rep);

EXTERN_C
void setExtraInfo_RS(
    int rep, 
    unsigned seed);

EXTERN_C
void setExtraInfo_RSL(
    int rep, 
    unsigned seed, 
    unsigned length);


