// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


#include "../asm_helper.h"

.text
.align 4;
.issue_mode dual

/*  
  The difference between this and chunk_q30_power_series() is that this one doesn't require
  the coefficient vector to contain redundant elements for each vector index. Instead, this version
  will broadcast the coefficient to a chunk. For this reason it is significantly slower, but it is
  also less wasteful of memory.

  NOTE: This hasn't (yet) been officially added to the API

    void chunk_q30_power_series_v2(
        int32_t a[],
        const q2_30 b[],
        const int32_t coef[],
        const unsigned term_count);
*/
#define FUNCTION_NAME   chunk_q20_power_series
#define NSTACKWORDS     (6 + 4 * 8)

#define VEC_POW   (NSTACKWORDS - 8)
#define VEC_ACC   (NSTACKWORDS - 16)
#define VEC_TMP   (NSTACKWORDS - 24) // -->  [coef, coef, 0, 0, 0, 0, 0, 0]
                                     // (last six elements must stay zeros)
#define VEC_COEF  (NSTACKWORDS - 30) // -->  [coef, coef, coef, coef, coef, coef, coef, coef]
                                     // (overlaps VEC_TMP and that's fine)

#define a           r0
#define b           r1
#define coef        r2
#define len         r3
#define vec_pow     r4
#define vec_acc     r5
#define vec_coef    r6
#define tmp         r7

.cc_top FUNCTION_NAME.function,FUNCTION_NAME
FUNCTION_NAME:
  dualentsp NSTACKWORDS
  std r4, r5, sp[0]
  std r6, r7, sp[1]
{ ldaw tmp, sp[VEC_TMP]       ; vclrdr                      }
  std r8, r9, sp[2]
{ ldc r11, 0                  ; vstd tmp[0]                 }
{ sub len, len, 1             ; vsetc r11                   }
{ ldaw vec_pow, sp[VEC_POW]   ; ldw r11, coef[0]            }
  std r11, r11, tmp[0]
{ ldaw vec_acc, sp[VEC_ACC]   ; vldd tmp[0]                 }
{ ldaw tmp, sp[VEC_COEF]      ; vfttf                       }
{ add coef, coef, 4           ; vstd vec_coef[0]            }
{                             ; vldc vec_coef[0]            }
  ldaw r11, cp[vpu_vec_0x40000000]
{                             ; vldr r11[0]                 }
{                             ; vstr vec_pow[0]             }
{                             ; vclrdr                      }
{                             ; vlmacc vec_pow[0]           }
{ mov r11, vec_pow            ; vstr vec_acc[0]             }

  .L_loop_top:
  {                             ; vldr r11[0]                 }
  {                             ; vlmul b[0]                  }
  { mov r11, vec_acc            ; vstr vec_pow[0]             }
  {                             ; vldr r11[0]                 }
  { add coef, coef, 4           ; ldw r11, coef[0]            }
    std r11, r11, tmp[0]
  {                             ; vldd tmp[0]                 }
  {                             ; vfttf                       }
  {                             ; vstd vec_coef[0]            }
  {                             ; vldc vec_coef[0]            }
  { sub len, len, 1             ; vlmacc vec_pow[0]           }
  {                             ; vstr vec_acc[0]             }
  { mov r11, vec_pow            ; bt len, .L_loop_top         }

{                             ; vstr a[0]                   }

.L_finish:
  ldd r4, r5, sp[0]
  ldd r6, r7, sp[1]
  ldd r8, r9, sp[2]
  retsp NSTACKWORDS

.L_func_end_unpack:
.cc_bottom FUNCTION_NAME.function

.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME,.L_func_end_unpack - FUNCTION_NAME

#undef NSTACKWORDS
#undef FUNCTION_NAME



#endif //defined(__XS3A__)



