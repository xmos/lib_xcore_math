// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include <stdint.h>
#include <stdio.h>

#include "xmath/xmath.h"
#include "vpu_helper.h"



void vect_s8_is_negative(
    int8_t a[],
    const int8_t b[],
    const unsigned len)
{
  for(unsigned k = 0; k < len; k++){
    a[k] = (b[k] < 0)? 1 : 0;
  }
}

