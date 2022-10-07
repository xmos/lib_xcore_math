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

    // scalar utils
    RUN_TEST_GROUP(vpu_scalar_ops_s8);
    RUN_TEST_GROUP(vpu_scalar_ops_s16);
    RUN_TEST_GROUP(vpu_scalar_ops_s32);
    

    return UNITY_END();
}
