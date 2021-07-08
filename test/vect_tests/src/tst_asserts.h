// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include <stdint.h>

#define XTEST_ASSERT_VECT_S16_EQUAL(EXPECTED, ACTUAL, LEN, FMT, ...)        \
  do {                                                                      \
    for(int i = 0; i < (LEN); i++) {                                        \
      if(((int16_t)(EXPECTED)[i]) != ((int16_t)(ACTUAL)[i])) {              \
        printf("\n*** %s[%d] != %s[%d] ***\n", #EXPECTED, i, #ACTUAL, i);   \
        printf(FMT, __VA_ARGS__ );                                          \
      }                                                                     \
      TEST_ASSERT_EQUAL_INT16_MESSAGE((EXPECTED)[i], (ACTUAL)[i],"");       \
    }                                                                       \
  } while(0)


#define XTEST_ASSERT_VECT_S32_EQUAL(EXPECTED, ACTUAL, LEN, FMT, ...)        \
  do {                                                                      \
    for(int i = 0; i < (LEN); i++) {                                        \
      if(((int32_t)(EXPECTED)[i]) != ((int32_t)(ACTUAL)[i])) {              \
        printf("\n*** %s[%d] != %s[%d] ***\n", #EXPECTED, i, #ACTUAL, i);   \
        printf(FMT, __VA_ARGS__ );                                          \
      }                                                                     \
      TEST_ASSERT_EQUAL_INT32_MESSAGE((EXPECTED)[i], (ACTUAL)[i],"");       \
    }                                                                       \
  } while(0)
    
