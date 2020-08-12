// Copyright (c) 2019, XMOS Ltd, All rights reserved
#include "testing.h"

#include <math.h>

#include <stdio.h>

timer tmr;

unsigned getTimestamp()
{
    unsigned res = 0;
    tmr :> res;
    return res;
}