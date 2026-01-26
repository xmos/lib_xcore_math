// Copyright 2021-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


/*  
void vect_s8_is_negative(
    int8_t a[],
    const int8_t b[],
    const unsigned len);
*/


#include "../asm_helper.h"

#define NSTACKWORDS     (8)

#define FUNCTION_NAME   vect_s8_is_negative

#define a           x10 
#define b           x11 
#define len         x12
#define _32         x13
#define vec_0xC1    x18
#define tail        x19

.text
.p2align 2


FUNCTION_NAME:
    xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
    li t3, 0x200
    xm.stdsp  s3,s2,8
  { mv t3, len                              ; xm.vsetc t3}
  { xm.zexti t3, 5                               ; srli len, len, EPV_LOG2_S8                 }/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in  shri len, len, EPV_LOG2_S8                 \nMessage: The shift amount is not 32" */
  { li _32, 32                               ; xm.mkmsk tail, t3                           }
lui t3, %hi(vpu_vec_0xC1)
    addi t3,t3, %lo(vpu_vec_0xC1)
  { mv vec_0xC1, t3                         ; mv t3, b                                }
  { nop                                           ; xm.brff len, .L_loop_bot                       }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */

  .L_loop_top:
    { add t3, t3, _32                         ; xm.vldr t3}
    { nop                                           ; xm.vsign                                     }
    { nop                                           ; xm.vlsub vec_0xC1}
    { nop                                           ; xm.vpos                                      }
    { addi len, len, -1                           ; xm.vstr a}
    { add a, a, _32                             ; xm.bt len, .L_loop_top                       }
  .L_loop_bot:

  { nop                                           ; xm.brff tail, .L_finish                        }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
  { nop                                           ; xm.vldr t3}
  { nop                                           ; xm.vsign                                     }
  { nop                                           ; xm.vlsub vec_0xC1}
  { nop                                           ; xm.vpos                                      }
    xm.vstrpv a, tail

    
.L_finish:
        xm.lddsp  s3,s2,8
    { nop                                           ; xm.retsp (NSTACKWORDS)*4                     } 

.L_func_end:


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS; .global FUNCTION_NAME.nstackwords /* Translation error on this line: unexpected token at position 42. */ 
.set FUNCTION_NAME.maxcores,1;              .global FUNCTION_NAME.maxcores /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;             .global FUNCTION_NAME.maxtimers /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;           .global FUNCTION_NAME.maxchanends /* Translation error on this line: unexpected token at position 32. */ 
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME



#endif //defined(__VX4B__)



