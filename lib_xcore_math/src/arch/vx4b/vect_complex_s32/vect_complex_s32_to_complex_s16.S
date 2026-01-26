// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.



#if defined(__VX4B__)


/*  

headroom_t vect_complex_s32_to_vect_complex_s16(
    int16_t* a_real,
    int16_t* a_imag,
    const complex_s32_t* b,
    const unsigned length,
    const right_shift_t b_shr);

*/

#include "../asm_helper.h"

.text
.p2align 2

#define NSTACKVECS      (2)
#define NSTACKWORDS     (8 + (8*NSTACKVECS)+4)

#define FUNCTION_NAME   vect_complex_s32_to_vect_complex_s16

#define STACK_VEC_TMP  (NSTACKWORDS-16-4)

#define STACK_B_SHR     (NSTACKWORDS+1)

#define a_real      x10
#define a_imag      x11
#define b           x12
#define len         x13
#define b_shr       x18
#define tail        x19
#define _28         x20
#define mask        x21


FUNCTION_NAME:
        xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
        xm.stdsp  s3,s2,8
        xm.stdsp  s5,s4,16
    {   li t3, 0                              ;   li _28, 28                             }
    addi b,b,-4
    {   li t3, 16                             ;   xm.vsetc t3}
    {   srli len, len, 2                         ;   slli tail, len, 1                        }
    mv b_shr, a4
    {   xm.mkmski mask, 8                           ;  nop}
    {   sub b_shr, b_shr, t3                   ;   xm.zexti tail, 3                            }
lui t3, %hi(vpu_vec_complex_pos_j)
        addi t3,t3, %lo(vpu_vec_complex_pos_j)
    {   xm.mkmsk tail, tail                        ;   xm.brff len, .L_loop_bot                     }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */

    .L_loop_top:
            xm.vlashr b, b_shr
        {   addi b, b, 4                             ;   xm.vlmul0 t3}   
        {   addi len, len, -1                         ;   xm.vdepth16                                }
        { nop                                           ;   xm.vdepth16                                }
            xm.vstrpv a_real, mask
            xm.vlashr b, b_shr         
        {   add b, b, _28                           ;   xm.vlmul0 t3}
        {   addi a_real, a_real, 8                   ;   xm.vdepth16                                }
        { nop                                           ;   xm.vdepth16                                }
            xm.vstrpv a_imag, mask
        {   addi a_imag, a_imag, 8                   ;   xm.bt len, .L_loop_top                     }


.L_loop_bot:



    beqz tail, .L_finish
    xm.vlashr b, b_shr
    xm.vlmul0 t3
    xm.vdepth16
    xm.vdepth16
    xm.vstrpv a_real, tail
    addi b, b, 4
    xm.vlashr b, b_shr         
    xm.vlmul0 t3
    xm.vdepth16
    xm.vdepth16
    xm.vstrpv a_imag, tail


.L_finish:
        xm.lddsp  s3,s2,8/* XAT Warning: "Not correcting LDDSP offset because it's not in the local frame range" */
        xm.lddsp  s5,s4,16/* XAT Warning: "Not correcting LDDSP offset because it's not in the local frame range" */
        xm.retsp (NSTACKWORDS)*4/* Multiple XAT warnings: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in retsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate", 'RETSP operand may need scaling' */

.L_func_end:

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords /* Translation error on this line: unexpected token at position 42. */ 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends /* Translation error on this line: unexpected token at position 32. */ 
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME


#endif //defined(__VX4B__)
