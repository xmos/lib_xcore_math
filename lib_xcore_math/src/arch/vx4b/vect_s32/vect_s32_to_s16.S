// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.



#if defined(__VX4B__)


/*  
void vect_s32_to_vect_s16(
    int16_t a[],
    const int32_t b[],
    const unsigned length,
    const int v_shr);
*/

#include "../asm_helper.h"

.text
.p2align 2


#define NSTACKWORDS     (4)

#define FUNCTION_NAME   vect_s32_to_vect_s16

#define a           a0
#define b           a1
#define len         a2
#define b_shr       a3
#define tail        s2
#define _16         s3



FUNCTION_NAME:
        xm.entsp (NSTACKWORDS)*4
        xm.stdsp  s3,s2,0
    { li t3, 0                          ; li _16, 16                            }
    { sub b_shr, b_shr, _16             ; xm.vsetc t3                           }
    { srli len, len, EPV_LOG2_S32       ; slli tail, len, SIZEOF_LOG2_S16       }
    { nop                               ; xm.zexti tail, 4                      }
    { xm.mkmsk tail, tail               ; xm.brff len, .L_loop_bot              }
    { xm.mkmski t3, 16                  ; xm.bu .L_loop_top                     }

.p2align 4
.L_loop_top:
        xm.vlashr b, b_shr
    { add b, b, _16                     ; xm.vdepth16                           }
    { addi len, len, -1                 ; add b, b, _16                         }
        xm.vstrpv a, t3
    { add a, a, _16                     ; xm.bt len, .L_loop_top                }
.L_loop_bot:

    { nop                               ; xm.brff tail, .L_finish               }
    { nop                               ; xm.vclrdr                             }
        xm.vlashr b, b_shr
    { nop                               ; xm.vdepth16                           }
        xm.vstrpv a, tail

.L_finish:
        xm.lddsp  s3,s2,0
    { nop                               ; xm.retsp (NSTACKWORDS)*4              } 

.L_func_end:


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS; .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;              .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;             .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;           .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME


#endif //defined(__VX4B__)
