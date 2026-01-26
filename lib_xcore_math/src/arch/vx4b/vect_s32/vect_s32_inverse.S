// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

/*  

headroom_t vect_s32_inverse(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const unsigned scale);

*/


#define NSTACKVECTS     (1)
#define NSTACKWORDS     (8+8*(NSTACKVECTS)+4)

#define FUNCTION_NAME   vect_s32_inverse


#define STACK_VEC_TMP       (NSTACKWORDS-8-4)


#define a               x10
#define b               x11
#define length          x12
#define scale           x13
#define div_hi          x13
#define div_lo          x18
#define v_mask          x19
#define _32             x20
#define val1            x21
#define val2            x22
#define vec_tmp         x23

.text
.p2align 2


FUNCTION_NAME:
    xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
    xm.stdsp  s3,s2,0
    xm.stdsp  s5,s4,8
    xm.stdsp  s7,s6,16

{   li t3, 0                              ;   sw s8, 24                          (sp)}
{   slli length, length, 2                   ;   xm.vsetc t3}

{   li _32, 32                             ; nop                                           }
{   sub val2, scale, _32                    ;   li val1, 1                             }
{   xm.shl div_hi, val1, val2                  ;   xm.shl div_lo, val1, scale                 }/* Multiple XAT warnings: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shli div_hi, val1, val2                  \nMessage: The shift amount is not 32", "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shli div_lo, val1, scale                 \nMessage: The shift amount is not 32" */
{   xm.vclrdr;    nop}
{   addi vec_tmp,sp, (STACK_VEC_TMP)*4         ;   xm.brff length, .L_loop_bot                  }/* Multiple XAT warnings: 'LDAWSP outside of known frame - offset may need correction', 'Instruction xm.brff can only branch forwards; this branch may need revising' */
{xm.vldr b; nop}
.p2align 4
.L_loop_top:
    // The masked out elements will  
    {   xm.mkmsk v_mask, length                 ;   xm.vstd vec_tmp                            }
        xm.vlashr b, v_mask
    {   sub length, length, _32                 ;   xm.vsign                                   }

    {   nop                                     ;   xm.vlmul0 b                                }
        xm.vstrpv vec_tmp, v_mask
        xm.vlashr b, v_mask
    {   add b, b, _32                           ;   xm.vsign                                   }
    {   mv val2, v_mask                         ;   nop                                        }
    .L_div_loop_top:

        {   srli val2, val2, 4                       ;   lw val1,0                    ( vec_tmp)}
            xm.ldivu val1, s8, div_hi, div_lo, val1
        {   addi vec_tmp, vec_tmp, 4                 ;   sw val1,0                    ( vec_tmp)}
        {   nop                                      ;   xm.bt val2, .L_div_loop_top                }
    .L_div_loop_bot:
    {   addi vec_tmp,sp, (STACK_VEC_TMP)*4         ; nop                                           }


    {   li val1, 1                             ;   xm.vlmul0 vec_tmp}
        xm.vstrpv a, v_mask
    {   xm.slt val1, length, val1                  ;   xm.vstr vec_tmp} // Headroom update
    {   add a, a, _32                           ;   nop                  }
    beqz val1, .L_loop_top 
.L_loop_bot:

.L_finish:
    xm.lddsp  s3,s2,0
    xm.lddsp  s5,s4,8
    xm.lddsp  s7,s6,16
{   li a0, 31                              ;   xm.vgetc t3}
{   xm.zexti t3, 5                         ;   lw s8, 24                      (sp)}
{   sub a0, a0, t3                         ;   xm.retsp (NSTACKWORDS)*4                   } 


.L_func_end:


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords /* Translation error on this line: unexpected token at position 42. */ 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends /* Translation error on this line: unexpected token at position 32. */ 
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME


#endif //defined(__VX4B__)



