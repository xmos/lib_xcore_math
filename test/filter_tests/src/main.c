// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdio.h>

#ifdef __XS3A__
# include <xscope.h>
#endif

#include "unity_fixture.h"

int main(int argc, const char* argv[])
{
#ifdef __XS3A__
  xscope_config_io(XSCOPE_IO_BASIC);
#endif
    
  UnityGetCommandLineOptions(argc, argv);
  UnityBegin(argv[0]);

  RUN_TEST_GROUP(filter_fir_s16);
  RUN_TEST_GROUP(filter_fir_s32);
  RUN_TEST_GROUP(filter_fir_s16_push_sample);
  RUN_TEST_GROUP(filter_biquad_s32);

  return UNITY_END();
}
