// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdio.h>
#include <stdlib.h>

#ifdef __XS3A__
# include <xscope.h>
#endif

void filter_32bit_fir();
void filter_16bit_fir();
void filter_32bit_biquad();

#define RAND_SEED   76465367

int main(int argc, char** argv)
{
#ifdef __XS3A__
  xscope_config_io(XSCOPE_IO_BASIC);
#endif

  // Seed the random number generator, using a constant for reproducibility
  srand(RAND_SEED);

  // filter_32bit_fir();
  // filter_16bit_fir();
  filter_32bit_biquad();

  return 0;
}
