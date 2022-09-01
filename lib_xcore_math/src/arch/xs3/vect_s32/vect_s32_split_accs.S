// Copyright 2021-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// XMOS Public License: Version 1

#if defined(__XS3A__)


#include "../asm_helper.h"

/*  
    void vect_s32_split_accs(
        split_acc_s32_t a[],
        const int32_t b[],
        const unsigned length);
*/


#define NSTACKWORDS     (20)

#define FUNCTION_NAME   vect_s32_split_accs

#define split     r0
#define merged    r1
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
  { add len, len, r11                         ;                                           }
  { shr len, len, 4                           ; ldaw r11, sp[0]                           }

  .L_loop_top:

      ldd tmpR, tmpD, merged[0]
      zip tmpR, tmpD, 4
    {                                           ; stw tmpR, sp[0]                           }
    {                                           ; stw tmpD, sp[8]                           }
    
      ldd tmpR, tmpD, merged[1]
      zip tmpR, tmpD, 4
    {                                           ; stw tmpR, sp[1]                           }
    {                                           ; stw tmpD, sp[9]                           }
    
      ldd tmpR, tmpD, merged[2]
      zip tmpR, tmpD, 4
    {                                           ; stw tmpR, sp[2]                           }
    {                                           ; stw tmpD, sp[10]                          }
    
      ldd tmpR, tmpD, merged[3]
      zip tmpR, tmpD, 4
    { add merged, merged, _32                   ; stw tmpR, sp[3]                           }
    {                                           ; stw tmpD, sp[11]                          }
    
      ldd tmpR, tmpD, merged[0]
      zip tmpR, tmpD, 4
    {                                           ; stw tmpR, sp[4]                           }
    {                                           ; stw tmpD, sp[12]                          }
    
      ldd tmpR, tmpD, merged[1]
      zip tmpR, tmpD, 4
    {                                           ; stw tmpR, sp[5]                           }
    {                                           ; stw tmpD, sp[13]                          }
    
      ldd tmpR, tmpD, merged[2]
      zip tmpR, tmpD, 4
    {                                           ; stw tmpR, sp[6]                           }
    {                                           ; stw tmpD, sp[14]                          }
    
      ldd tmpR, tmpD, merged[3]
      zip tmpR, tmpD, 4
    {                                           ; stw tmpR, sp[7]                           }
    { sub len, len, 1                           ; stw tmpD, sp[15]                          }


    { ldaw r11, sp[8]                           ; vldd r11[0]                               }
    { add split, split, _32                     ; vstd split[0]                             }
    { ldaw r11, sp[0]                           ; vldd r11[0]                               }
    { add split, split, _32                     ; vstd split[0]                             }

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



