// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once


#include "xmath/xmath_conf.h"
#include "xmath/api.h"
#include "xmath/types.h"

#include "xmath/vect/vect.h"
#include "xmath/scalar/scalar.h"
#include "xmath/bfp/bfp.h"

#include "xmath/dct.h"
#include "xmath/fft.h"
#include "xmath/filter.h"

#include "xmath/util.h"
#include "xmath/q_format.h"

#ifdef __XS3A__
#include "xmath/xs3/vpu_info.h"
#endif

#ifndef M_PI
#define M_PI ( 3.14159265358979323846 )
#endif
#ifndef M_E
#define M_E	( 2.71828182845904523536 )
#endif
