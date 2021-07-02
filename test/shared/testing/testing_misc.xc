// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// XMOS Public License: Version 1

extern "C" {
  #include "testing.h"
}

#include <math.h>
#include <stdio.h>

timer tmr;

unsigned getTimestamp()
{
    unsigned res = 0;
    tmr :> res;
    return res;
}