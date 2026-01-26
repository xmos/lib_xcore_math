// Copyright 2020-2022 XMOS LIMITED.
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


#define NSTACKWORDS     (8)

#define FUNCTION_NAME   vect_s32_to_vect_s16

#define a           x10
#define b           x11
#define len         x12
#define b_shr       x13
#define tail        x18
#define _16         x19



FUNCTION_NAME:
        xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
        xm.stdsp  s3,s2,0
    {   li t3, 0                              ;   li _16, 16                         }
    {   sub b_shr, b_shr, _16                   ;   xm.vsetc t3}
    {   srli len, len, EPV_LOG2_S32              ;   slli tail, len, SIZEOF_LOG2_S16      }/* Multiple XAT warnings: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shri len, len, EPV_LOG2_S32              \nMessage: The shift amount is not 32", "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shli tail, len, SIZEOF_LOG2_S16      \nMessage: The shift amount is not 32" */
    { nop                                           ;   xm.zexti tail, 4                        }
    {   xm.mkmsk tail, tail                        ;   xm.brff len, .L_loop_bot                 }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
    {   xm.mkmski t3, 16                           ;   xm.bu .L_loop_top                      }

.p2align 4
.L_loop_top:
        xm.vlashr b, b_shr
    {   add b, b, _16                           ;   xm.vdepth16                            }
    {   addi len, len, -1                         ;   add b, b, _16                       }
        xm.vstrpv a, t3
    {   add a, a, _16                           ;   xm.bt len, .L_loop_top                 }
.L_loop_bot:

    { nop                                           ;   xm.brff tail, .L_finish                  }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
    { nop                                           ;   xm.vclrdr                              }
        xm.vlashr b, b_shr
    { nop                                           ;   xm.vdepth16                            }
        xm.vstrpv a, tail

.L_finish:
        xm.lddsp  s3,s2,0
    { nop                                           ;   xm.retsp (NSTACKWORDS)*4                   } 

.L_func_end:


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS; .global FUNCTION_NAME.nstackwords /* Translation error on this line: unexpected token at position 42. */ 
.set FUNCTION_NAME.maxcores,1;              .global FUNCTION_NAME.maxcores /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;             .global FUNCTION_NAME.maxtimers /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;           .global FUNCTION_NAME.maxchanends /* Translation error on this line: unexpected token at position 32. */ 
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME


#endif //defined(__VX4B__)
