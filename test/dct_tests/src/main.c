// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include <stdio.h>

#ifdef __XS3A__
# include <xscope.h>
#endif

#include "unity_fixture.h"
#include "tst_common.h"

FILE* perf_file = NULL;

int main(int argc, const char* argv[])
{
#ifdef __XS3A__
  xscope_config_io(XSCOPE_IO_BASIC);
#endif
    
  UnityGetCommandLineOptions(argc, argv);
  UnityBegin(argv[0]);

#if WRITE_PERFORMANCE_INFO
  printf("Writing performance info to: %s\n", PERFORMANCE_INFO_FILENAME);
  perf_file = fopen(PERFORMANCE_INFO_FILENAME, "w");
  fprintf(perf_file, "Function, Input Size, Worst Observed Error, Worst Observed Timing, Misc\n");
#endif

  RUN_TEST_GROUP(dctXX_forward);
  RUN_TEST_GROUP(dctXX_inverse);
  RUN_TEST_GROUP(dct8x8);

#if WRITE_PERFORMANCE_INFO
  fclose(perf_file);
#endif

  return UNITY_END();
}
