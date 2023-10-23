// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "xmath/xmath.h"


void vect_complex_f32_add(
  complex_float_t a[],
  const complex_float_t b[],
  const complex_float_t c[],
  const unsigned length)
{
  vect_f32_add((float*) &a[0],(float*) &b[0], (float*) &c[0], 2*length);
}


void vect_packed_complex_f32_mul(
  complex_float_t a[],
  const complex_float_t b[],
  const complex_float_t c[],
  const unsigned length)
{
  a[0].re = b[0].re * c[0].re;
  a[0].im = b[0].im * c[0].im;

  vect_complex_f32_mul(&a[1], &b[1], &c[1], length-1);
}

void vect_packed_complex_f32_conj_mul(
  complex_float_t a[],
  const complex_float_t b[],
  const complex_float_t c[],
  const unsigned length)
{
  a[0].re = b[0].re * c[0].re;
  a[0].im = b[0].im * c[0].im;

  vect_complex_f32_conj_mul(&a[1], &b[1], &c[1], length-1);
}


void vect_packed_complex_f32_macc(
  complex_float_t a[],
  const complex_float_t b[],
  const complex_float_t c[],
  const unsigned length)
{
  a[0].re += b[0].re * c[0].re;
  a[0].im += b[0].im * c[0].im;

  vect_complex_f32_macc(&a[1], &b[1], &c[1], length-1);
}

void vect_packed_complex_f32_conj_macc(
  complex_float_t a[],
  const complex_float_t b[],
  const complex_float_t c[],
  const unsigned length)
{
  a[0].re += b[0].re * c[0].re;
  a[0].im += b[0].im * c[0].im;

  vect_complex_f32_conj_macc(&a[1], &b[1], &c[1], length-1);
}
