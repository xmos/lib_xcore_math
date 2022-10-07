// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include <stdint.h>

#include "xmath/xmath.h"
#include "pseudo_rand.h"
#include "rand_frame.h"
#include "testing.h"

#include "tst_asserts.h"


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