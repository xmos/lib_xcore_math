// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include "tst_common.h"

#include "unity_fixture.h"

char detail_buff[200];


void setExtraInfo_R(
    int rep)
{
  sprintf(detail_buff, "( rep: %d )", rep);
  UNITY_SET_DETAIL(detail_buff);
}

void setExtraInfo_RS(
    int rep, 
    unsigned seed)
{
  sprintf(detail_buff, "( rep: %d; seed: 0x%08X )", rep, seed);
  UNITY_SET_DETAIL(detail_buff);
}

void setExtraInfo_RSL(
    int rep, 
    unsigned seed, 
    unsigned length)
{
  sprintf(detail_buff, "( rep: %d; seed: 0x%08X; length: %u )", rep, seed, length);
  UNITY_SET_DETAIL(detail_buff);
}