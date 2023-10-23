// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include <stdint.h>

#define XTEST_ASSERT_S16_EQUAL(EXPECTED, ACTUAL, FMT, ...)                  \
  do {                                                                      \
    if(((int16_t)(EXPECTED)) != ((int16_t)(ACTUAL))) {                      \
      printf("\n" FMT, __VA_ARGS__ );                                       \
    }                                                                       \
    TEST_ASSERT_EQUAL_INT16_MESSAGE((EXPECTED), (ACTUAL),"");               \
  } while(0)



#define XTEST_ASSERT_S32_EQUAL(EXPECTED, ACTUAL, FMT, ...)                  \
  do {                                                                      \
    if(((int32_t)(EXPECTED)) != ((int32_t)(ACTUAL))) {                      \
      printf(FMT, __VA_ARGS__ );                                            \
    }                                                                       \
    TEST_ASSERT_EQUAL_INT32_MESSAGE((EXPECTED), (ACTUAL), "");              \
  } while(0)



#define XTEST_ASSERT_S64_EQUAL(EXPECTED, ACTUAL, FMT, ...)                  \
  do {                                                                      \
    if(((int64_t)(EXPECTED)) != ((int64_t)(ACTUAL))) {                      \
      printf(FMT, __VA_ARGS__ );                                            \
    }                                                                       \
    TEST_ASSERT_EQUAL_INT64_MESSAGE((EXPECTED), (ACTUAL), "");              \
  } while(0)



#define XTEST_ASSERT_VECT_S16_EQUAL(EXPECTED, ACTUAL, LEN, FMT, ...)        \
  do {                                                                      \
    for(unsigned int i = 0; i < (LEN); i++) {                                        \
      if(((int16_t)(EXPECTED)[i]) != ((int16_t)(ACTUAL)[i])) {              \
        printf("\n*** %s[%d] != %s[%d] ***\n", #EXPECTED, i, #ACTUAL, i);   \
        printf(FMT, __VA_ARGS__ );                                          \
      }                                                                     \
      TEST_ASSERT_EQUAL_INT16_MESSAGE((EXPECTED)[i], (ACTUAL)[i],"");       \
    }                                                                       \
  } while(0)


#define XTEST_ASSERT_VECT_S32_EQUAL(EXPECTED, ACTUAL, LEN, FMT, ...)        \
  do {                                                                      \
    for(unsigned int i = 0; i < (LEN); i++) {                                        \
      if(((int32_t)(EXPECTED)[i]) != ((int32_t)(ACTUAL)[i])) {              \
        printf("\n*** %s[%d] != %s[%d] ***\n", #EXPECTED, i, #ACTUAL, i);   \
        printf(FMT, __VA_ARGS__ );                                          \
      }                                                                     \
      TEST_ASSERT_EQUAL_INT32_MESSAGE((EXPECTED)[i], (ACTUAL)[i],"");       \
    }                                                                       \
  } while(0)
    
