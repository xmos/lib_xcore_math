// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "../../../vect/vpu_helper.h"



void xs3_vect_s8_is_negative(
    int8_t a[],
    const int8_t b[],
    const unsigned len)
{
  for(int k = 0; k < len; k++){
    a[k] = (b[k] < 0)? 1 : 0;
  }
}