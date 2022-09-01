// Copyright 2021-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


/*  
void vect_s8_is_negative(
    int8_t a[],
    const int8_t b[],
    const unsigned len);
*/


#include "../asm_helper.h"

#define NSTACKWORDS     (6)

#define FUNCTION_NAME   vect_s8_is_negative

#define a           r0 
#define b           r1 
#define len         r2
#define _32         r3
#define vec_0xC1    r4
#define tail        r5

.text
.issue_mode dual
.align 4

.cc_top FUNCTION_NAME.function,FUNCTION_NAME

FUNCTION_NAME:
    dualentsp NSTACKWORDS
    ldc r11, 0x200
    std r4, r5, sp[1]
  { mov r11, len                              ; vsetc r11                                 }
  { zext r11, 5                               ; shr len, len, EPV_LOG2_S8                 }
  { ldc _32, 32                               ; mkmsk tail, r11                           }
    ldaw r11, cp[vpu_vec_0xC1]
  { mov vec_0xC1, r11                         ; mov r11, b                                }
  {                                           ; bf len, .L_loop_bot                       }

  .L_loop_top:
    { add r11, r11, _32                         ; vldr r11[0]                               }
    {                                           ; vsign                                     }
    {                                           ; vlsub vec_0xC1[0]                         }
    {                                           ; vpos                                      }
    { sub len, len, 1                           ; vstr a[0]                                 }
    { add a, a, _32                             ; bt len, .L_loop_top                       }
  .L_loop_bot:

  {                                           ; bf tail, .L_finish                        }
  {                                           ; vldr r11[0]                               }
  {                                           ; vsign                                     }
  {                                           ; vlsub vec_0xC1[0]                         }
  {                                           ; vpos                                      }
    vstrpv a[0], tail

    
.L_finish:
        ldd r4, r5, sp[1]
    {                                           ; retsp NSTACKWORDS                     } 

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



