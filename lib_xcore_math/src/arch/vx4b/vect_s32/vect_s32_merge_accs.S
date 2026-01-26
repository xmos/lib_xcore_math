// Copyright 2021-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// XMOS Public License: Version 1

#if defined(__VX4B__)


#include "../asm_helper.h"

/*  
    void vect_s32_merge_accs(
        int32_t a[],
        split_acc_s32_t b[],
        const unsigned length);
*/


#define NSTACKWORDS     (20)

#define FUNCTION_NAME   vect_s32_merge_accs

#define merged    x10
#define split     x11
#define len       x12
#define _32       x13

#define tmpR      x18
#define tmpD      x19


.text
.p2align 2


FUNCTION_NAME:
    xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
    xm.stdsp  s3,s2,64

  { li t3, 15                               ; li _32, 32                               }
  { add len, len, t3                         ; sub merged, merged, _32                   }
  { srli len, len, 4                           ; addi t3,sp, 0                           }

  .L_loop_top:

    { add split, split, _32                     ; xm.vldd split}
    { addi t3,sp, 32                           ; xm.vstd t3}
    { add split, split, _32                     ; xm.vldd split}
    { addi t3,sp, 0                           ; xm.vstd t3}
    { addi len, len, -1                           ; lw tmpD, 0                           (sp)}
    { add merged, merged, _32                   ; lw tmpR, 32                           (sp)}
xm.unzip  tmpD, tmpR, 4
      xm.stdi  tmpR,tmpD, 0(merged)
    { nop                                           ; lw tmpD, 4                           (sp)}
    { nop                                           ; lw tmpR, 36                           (sp)}
xm.unzip  tmpD, tmpR, 4
      xm.stdi  tmpR,tmpD, 8(merged)
    { nop                                           ; lw tmpD, 8                           (sp)}
    { nop                                           ; lw tmpR, 40                          (sp)}
xm.unzip  tmpD, tmpR, 4
      xm.stdi  tmpR,tmpD, 16(merged)
    { nop                                           ; lw tmpD, 12                           (sp)}
    { nop                                           ; lw tmpR, 44                          (sp)}
xm.unzip  tmpD, tmpR, 4
      xm.stdi  tmpR,tmpD, 24(merged)
    { nop                                           ; lw tmpD, 16                           (sp)}
    { nop                                           ; lw tmpR, 48                          (sp)}
xm.unzip  tmpD, tmpR, 4
      xm.stdi  tmpR,tmpD, 32(merged)
    { nop                                           ; lw tmpD, 20                           (sp)}
    { nop                                           ; lw tmpR, 52                          (sp)}
xm.unzip  tmpD, tmpR, 4
      xm.stdi  tmpR,tmpD, 40(merged)
    { nop                                           ; lw tmpD, 24                           (sp)}
    { nop                                           ; lw tmpR, 56                          (sp)}
xm.unzip  tmpD, tmpR, 4
      xm.stdi  tmpR,tmpD, 48(merged)
    { nop                                           ; lw tmpD, 28                           (sp)}
    { nop                                           ; lw tmpR, 60                          (sp)}
xm.unzip  tmpD, tmpR, 4
      xm.stdi  tmpR,tmpD, 56(merged)
    { add merged, merged, _32                   ;nop                     }
    bnez  len, .L_loop_top 

  .L_finish:
      xm.lddsp  s3,s2,64
    { nop                                           ; xm.retsp (NSTACKWORDS)*4                         } 


.L_func_end:


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS; .global FUNCTION_NAME.nstackwords /* Translation error on this line: unexpected token at position 42. */ 
.set FUNCTION_NAME.maxcores,1;              .global FUNCTION_NAME.maxcores /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;             .global FUNCTION_NAME.maxtimers /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;           .global FUNCTION_NAME.maxchanends /* Translation error on this line: unexpected token at position 32. */ 
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME


#endif //defined(__VX4B__)



