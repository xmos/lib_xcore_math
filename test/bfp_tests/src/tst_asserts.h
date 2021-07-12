// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include <stdint.h>


#define XTEST_ASSERT(CONDITION, FMT, ...)         \
  do {                                            \
    if(!(CONDITION)) {                            \
      printf( FMT, __VA_ARGS__ );                 \
    }                                             \
    TEST_ASSERT(CONDITION);                       \
  } while(0)                            


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
    


#define XTEST_ASSERT_VECT_S16_WITHIN(THRESH, EXPECTED, ACTUAL, LEN, FMT, ...)           \
  do {                                                                                  \
    for(int i = 0; i < (LEN); i++) {                                                    \
      int16_t d = abs(((int16_t)(EXPECTED)[i]) - ((int16_t)(ACTUAL)[i]));               \
      if(d > (THRESH)) {                                                                \
        printf("\n*** abs(%s[%d] - %s[%d]) > %d ***\n",                                 \
            #EXPECTED, i, #ACTUAL, i, (THRESH));                                        \
        printf("Actual delta: %d\n", d);                                                \
        printf(FMT, __VA_ARGS__ );                                                      \
      }                                                                                 \
      TEST_ASSERT_INT16_WITHIN_MESSAGE((THRESH),                                        \
          (EXPECTED)[i], (ACTUAL)[i],"");                                               \
    }                                                                                   \
  } while(0)


#define XTEST_ASSERT_VECT_S32_WITHIN(THRESH, EXPECTED, ACTUAL, LEN, FMT, ...)           \
  do {                                                                                  \
    for(int i = 0; i < (LEN); i++) {                                                    \
      int32_t d = abs(((int32_t)(EXPECTED)[i]) - ((int32_t)(ACTUAL)[i]));               \
      if(d > (THRESH)) {                                                                \
        printf("\n*** abs(%s[%d] - %s[%d]) > %d ***\n",                                 \
            #EXPECTED, i, #ACTUAL, i, (THRESH));                                        \
        printf("Actual delta: %d\n", d);                                                \
        printf(FMT, __VA_ARGS__ );                                                      \
      }                                                                                 \
      TEST_ASSERT_INT32_WITHIN_MESSAGE((THRESH),                                        \
          (EXPECTED)[i], (ACTUAL)[i],"");                                               \
    }                                                                                   \
  } while(0)
    
