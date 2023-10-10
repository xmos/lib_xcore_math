// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include "floating_dct.h"

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

 
void flt_dct_forward(
    float output[],
    float input[],
    const unsigned DCT_N)
{
  for(unsigned int k = 0; k < DCT_N; k++){
    output[k] = 0.0;
    for(unsigned int n = 0; n < DCT_N; n++){
      float c = cosf( k * ((float) M_PI) * (2*n + 1) / (2.0f * DCT_N));
      output[k] += input[n] * c;
    }
    output[k] *= 2.0f;
  }
}

void flt_dct_inverse(
    float output[],
    float input[],
    const unsigned DCT_N)
{
  for(unsigned int k = 0; k < DCT_N; k++){
    output[k] = input[0]/2.0f;
    for(unsigned int n = 1; n < DCT_N; n++){
      float c = cosf( n * ((float) M_PI) * (2*k + 1) / (2.0f * DCT_N));
      output[k] += input[n] * c;
    }
    output[k] /= DCT_N;
  }
}

void dbl_dct_forward(
    double output[],
    double input[],
    const unsigned DCT_N)
{
  for(unsigned int k = 0; k < DCT_N; k++){
    output[k] = 0.0;
    for(unsigned int n = 0; n < DCT_N; n++){
      double c = cos( k * M_PI * (2*n + 1) / (2.0 * DCT_N));
      output[k] += input[n] * c;
    }
    output[k] *= 2;
  }
}

void dbl_dct_inverse(
    double output[],
    double input[],
    const unsigned DCT_N)
{
  for(unsigned int k = 0; k < DCT_N; k++){
    output[k] = input[0]/2.0;
    for(unsigned int n = 1; n < DCT_N; n++){
      double c = cos( n * (M_PI) * (2*k + 1) / (2.0 * DCT_N));
      output[k] += input[n] * c;
    }
    output[k] /= DCT_N;
  }
}