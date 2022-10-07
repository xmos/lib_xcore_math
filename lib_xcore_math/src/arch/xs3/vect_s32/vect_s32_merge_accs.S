// Copyright 2021-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// XMOS Public License: Version 1

#if defined(__XS3A__)


#include "../asm_helper.h"

/*  
    void vect_s32_merge_accs(
        int32_t a[],
        split_acc_s32_t b[],
        const unsigned length);
*/


#define NSTACKWORDS     (20)

#define FUNCTION_NAME   vect_s32_merge_accs

#define merged    r0
#define split     r1
#define len       r2
#define _32       r3

#define tmpR      r4
#define tmpD      r5


.text
.issue_mode dual
.align 4

.cc_top FUNCTION_NAME.function,FUNCTION_NAME

FUNCTION_NAME:
    dualentsp NSTACKWORDS
    std r4, r5, sp[8]

  { ldc r11, 15                               ; ldc _32, 32                               }
  { add len, len, r11                         ; sub merged, merged, _32                   }
  { shr len, len, 4                           ; ldaw r11, sp[0]                           }

  .L_loop_top:

    { add split, split, _32                     ; vldd split[0]                             }
    { ldaw r11, sp[8]                           ; vstd r11[0]                               }
    { add split, split, _32                     ; vldd split[0]                             }
    { ldaw r11, sp[0]                           ; vstd r11[0]                               }
    { sub len, len, 1                           ; ldw tmpD, sp[0]                           }
    { add merged, merged, _32                   ; ldw tmpR, sp[8]                           }
      unzip tmpD, tmpR, 4
      std tmpD, tmpR, merged[0]
    {                                           ; ldw tmpD, sp[1]                           }
    {                                           ; ldw tmpR, sp[9]                           }
      unzip tmpD, tmpR, 4
      std tmpD, tmpR, merged[1]
    {                                           ; ldw tmpD, sp[2]                           }
    {                                           ; ldw tmpR, sp[10]                          }
      unzip tmpD, tmpR, 4
      std tmpD, tmpR, merged[2]
    {                                           ; ldw tmpD, sp[3]                           }
    {                                           ; ldw tmpR, sp[11]                          }
      unzip tmpD, tmpR, 4
      std tmpD, tmpR, merged[3]
    {                                           ; ldw tmpD, sp[4]                           }
    {                                           ; ldw tmpR, sp[12]                          }
      unzip tmpD, tmpR, 4
      std tmpD, tmpR, merged[4]
    {                                           ; ldw tmpD, sp[5]                           }
    {                                           ; ldw tmpR, sp[13]                          }
      unzip tmpD, tmpR, 4
      std tmpD, tmpR, merged[5]
    {                                           ; ldw tmpD, sp[6]                           }
    {                                           ; ldw tmpR, sp[14]                          }
      unzip tmpD, tmpR, 4
      std tmpD, tmpR, merged[6]
    {                                           ; ldw tmpD, sp[7]                           }
    {                                           ; ldw tmpR, sp[15]                          }
      unzip tmpD, tmpR, 4
      std tmpD, tmpR, merged[7]
    { add merged, merged, _32                   ; bt len, .L_loop_top                       }

  .L_finish:
      ldd r4, r5, sp[8]
    {                                           ; retsp NSTACKWORDS                         } 


.L_func_end:
.cc_bottom FUNCTION_NAME.function


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS; .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;              .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;             .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;           .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME


#endif //defined(__XS3A__)



