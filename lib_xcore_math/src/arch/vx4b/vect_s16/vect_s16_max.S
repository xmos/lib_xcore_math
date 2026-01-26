// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


/*  

int32_t vect_s16_max(
    const int32_t b[],
    const unsigned length);


*/


#include "../asm_helper.h"

.text
.p2align 2

#define NSTACKVECS      (2)
#define NSTACKWORDS     (8 + 8*NSTACKVECS+4)

#define FUNCTION_NAME       vect_s16_max

#define STACK_VEC_TMP       (NSTACKWORDS-8-4)
#define STACK_VEC_CUR_MAX   (NSTACKWORDS-16-4)

#define b           x10      // ![0x%08X]
#define N           x11      // ![%d]
#define tail        x12      // ![0x%X]
#define tmp         x13      // ![%d]
#define tmz         x18      // ![%d]



.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.align 16; /* Translation error on this line: unexpected token at position 9. */ 

FUNCTION_NAME:

        xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
        xm.stdsp  s3,s2,0
        xm.stdsp  s5,s4,8
        li t3, 0x100
    {   slli tail, N, SIZEOF_LOG2_S16            ;   xm.vsetc t3}/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shli tail, N, SIZEOF_LOG2_S16            \nMessage: The shift amount is not 32" */
    {   srli N, N, EPV_LOG2_S16                  ; nop                                           }/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shri N, N, EPV_LOG2_S16                  \nMessage: The shift amount is not 32" */
    {   slli tmp, N, 5                           ; nop                                           }
        la t3, vpu_vec_0x8000
    {   addi t3,sp, (STACK_VEC_CUR_MAX)*4         ;   xm.vldr t3}
    {   add t3, b, tmp                         ;   xm.vstr t3}
    {   xm.zexti tail, 5                            ;   xm.vldr t3}
    {   xm.mkmsk tail, tail                        ;   addi t3,sp, (STACK_VEC_CUR_MAX)*4         }
        xm.vstrpv t3, tail
    
    // Tail is fully accounted for in cur_max now.

#undef tail
#define cur_max     x12      // ![0x%08X]
    
    {   addi tmp,sp, (STACK_VEC_TMP)*4             ;   mv cur_max, t3                        }
    { nop                                           ;   xm.vclrdr                                  }
    {   mv t3, b                              ;   xm.brff N, .L_loop_bot                       }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */

    .L_loop_top:
        // cur_max[] saved in stack

        {   mv b, t3                              ;   xm.vldr t3} //  vR[i] = b[i]
        {   addi N, N, -1                             ;   xm.vlsub cur_max} //  vR[i] = cur_max[i] - b[i]
        {   addi t3,sp, 0                         ;   xm.vdepth1                                 } //  vR[0] = [bitmask -- 1 where vR[i] < 0]  b[i] > cur_max[i]
        {   xm.mkmski tmp, 2                            ; nop                                           }
            xm.vstrpv t3, tmp
        {   mv t3, b                              ;   lw tmp, 0                          (sp)}
        {   mv tmz, tmp                            ;   xm.vldr t3}
xm.zip tmz, tmp, 0
        { nop                                           ;   li t3, 32                             }
            xm.vstrpv cur_max, tmp
        {   add t3, b, t3                         ;   xm.bt N, .L_loop_top                       }
.L_loop_bot:

    {   addi t3,sp, (STACK_VEC_CUR_MAX)*4         ;   li N, 15                               }
    xm.ld16s cur_max, N(x28)
    addi t3,sp, (STACK_VEC_CUR_MAX)*4 
    {   addi t3, t3, -2                         ; nop                                           }
    .L_loop2_top:
         xm.ld16s a0, N(x28)
        {   addi N, N, -1                            ;   nop}
        {   xm.slt tmp, a0, cur_max                    ; nop                                           }
        {   xm.shli tmp, tmp, 1; nop}
        {   nop                                           ;   xm.bru tmp                                 }
            {   mv cur_max, a0                         ; nop                                           }
        {   mv a0, cur_max                         ;   xm.bt N, .L_loop2_top                      }

        xm.lddsp  s3,s2,0
        xm.lddsp  s5,s4,8
        xm.retsp (NSTACKWORDS)*4/* Multiple XAT warnings: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in retsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate", 'RETSP operand may need scaling' */


//.cc_bottom FUNCTION_NAME.function;  /* Translation error on this line: unexpected token at position 33. */ 
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords;  /* Translation error on this line: unexpected token at position 42. */ 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores;  /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers;  /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends;  /* Translation error on this line: unexpected token at position 32. */ 
.L_end: 
    .size FUNCTION_NAME, .L_end - FUNCTION_NAME






#endif //defined(__VX4B__)
