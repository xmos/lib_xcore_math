// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdio.h>
#include <stdlib.h>
#include <xscope.h>

void vect_s32_example();
void vect_complex_s16_example();

#define RAND_SEED   0xDEFACED

int main(int argc, char** argv)
{
  xscope_config_io(XSCOPE_IO_BASIC);

  // Seed the random number generator, using a constant for reproducibility
  srand(RAND_SEED);

  vect_s32_example();
  vect_complex_s16_example();

  return 0;
}
