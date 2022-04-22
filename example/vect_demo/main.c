// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdio.h>
#include <stdlib.h>

#ifdef __XS3A__
# include <xscope.h>
#endif

void vect_s32_example();
void vect_complex_s16_example();

#define RAND_SEED   0xDEFACED

int main(int argc, char** argv)
{
#ifdef __XS3A__
  xscope_config_io(XSCOPE_IO_BASIC);
#endif

  // Seed the random number generator, using a constant for reproducibility
  srand(RAND_SEED);

  vect_s32_example();
  vect_complex_s16_example();

  return 0;
}
