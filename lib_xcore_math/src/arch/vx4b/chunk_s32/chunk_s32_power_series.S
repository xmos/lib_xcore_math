// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

.text
.align 4; /* Translation error on this line: unexpected token at position 8. */ 

/*  
    void chunk_q30_power_series(
        int32_t a[VPU_INT32_EPV],
        const q2_30 b[VPU_INT32_EPV],
        const int32_t coef[],
        const unsigned term_count);
*/
#define FUNCTION_NAME   chunk_q30_power_series
#define NSTACKWORDS     (8 + 2 * 8 + 4)

#define VEC_POW   (NSTACKWORDS - 8-4)
#define VEC_ACC   (NSTACKWORDS - 16-4)

#define a           x10
#define b           x11
#define coef        x12
#define len         x13
#define vec_pow     x18
#define vec_acc     x19
#define tmp         x20
#define _32         x21

FUNCTION_NAME:
  xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
  xm.stdsp  s3,s2,0
  xm.stdsp  s5,s4,8
{ addi len, len, -1             ; nop                             }
  xm.stdsp  s7,s6,16

{ li t3, 0                  ; xm.vldc coef}
{ nop                             ; xm.vsetc t3}
lui t3, %hi(vpu_vec_0x40000000)
  addi t3,t3, %lo(vpu_vec_0x40000000)
{ addi vec_pow,sp, (VEC_POW)*4   ; xm.vldr t3}
{ addi vec_acc,sp, (VEC_ACC)*4   ; xm.vstr vec_pow}
{ li _32, 32                 ; xm.vclrdr                      }
{ add coef, coef, _32         ; xm.vlmacc0 vec_pow}
{ mv t3, vec_pow            ; xm.vstr vec_acc}

  .L_loop_top:
  { nop                             ; xm.vldr t3}
  { nop                             ; xm.vlmul0 b}
  { mv t3, vec_acc            ; xm.vstr vec_pow}
  { nop                             ; xm.vldr t3}
  { add coef, coef, _32         ; xm.vldc coef}
  { addi len, len, -1             ; xm.vlmacc0 vec_pow}
  { nop                             ; xm.vstr vec_acc}
  { mv t3, vec_pow            ; xm.bt len, .L_loop_top         }

{ nop                             ; xm.vstr a}

.L_finish:
  xm.lddsp  s3,s2,0
  xm.lddsp  s5,s4,8
  xm.lddsp  s7,s6,16
  xm.retsp (NSTACKWORDS)*4/* Multiple XAT warnings: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in retsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate", 'RETSP operand may need scaling' */

.L_func_end_unpack:

.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords /* Translation error on this line: unexpected token at position 42. */ 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends /* Translation error on this line: unexpected token at position 32. */ 
.size FUNCTION_NAME,.L_func_end_unpack - FUNCTION_NAME

#undef NSTACKWORDS
#undef FUNCTION_NAME



#endif //defined(__VX4B__)



