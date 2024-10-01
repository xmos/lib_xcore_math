// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdio.h>

#ifdef __XS3A__
# include <xscope.h>
#endif

#include "xmath/xmath.h"

int main() { 

  // Just reference a few symbols to make sure that they can be linked in
  asm volatile("ldap r11, vect_s32_add" ::: "r11");
  asm volatile("ldap r11, vect_complex_s32_conj_mul" ::: "r11");
  asm volatile("ldap r11, vect_f32_dot" ::: "r11");
  asm volatile("ldap r11, vect_s16_headroom" ::: "r11");
  asm volatile("ldap r11, bfp_fft_forward_mono" ::: "r11");
  asm volatile("ldap r11, fft_dif_inverse" ::: "r11");
  asm volatile("ldap r11, q24_logistic_fast" ::: "r11");
  asm volatile("ldap r11, float_s32_abs" ::: "r11");
  asm volatile("ldap r11, filter_biquad_s32" ::: "r11");
  asm volatile("ldap r11, dct32_forward" ::: "r11");
  
  return 0;
}
