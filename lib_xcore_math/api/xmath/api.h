// Copyright 2021-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once


#ifndef EXTERN_C
#  if defined(__cplusplus)
#    define EXTERN_C extern "C"
#  else
#    define EXTERN_C 
#  endif //__cplusplus
#endif

/// Used to indicate a function uses the C function ABI
#ifndef C_API
#  define C_API EXTERN_C
#endif
/// Used to indicate a struct is a C type
#ifndef C_TYPE
#  define C_TYPE EXTERN_C
#endif

/// Many API functions require word or double word alignment
#ifndef ALIGNMENT
#  ifdef __xcore__
#    define ALIGNMENT(N)  __attribute__((aligned (N)))
#  else
#    define ALIGNMENT(N)
#  endif
#endif

/// (on xcore) Force variable to word alignment
#ifndef WORD_ALIGNED
#  define WORD_ALIGNED   ALIGNMENT(4)
#endif

/// (on xcore) Force variable to double word alignment
#ifndef DWORD_ALIGNED
#  define DWORD_ALIGNED  ALIGNMENT(8)
#endif
