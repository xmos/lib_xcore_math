// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

/*  

void vect_float_s32_ln_prepare(
    q2_30 a[8],
    q8_24 exp_mod[8],
    const float_s32_t b[]);
*/


#define NSTACKWORDS     (8)

#define FUNCTION_NAME   vect_float_s32_ln_prepare


#define a           x10 
#define exp_mod     x11 
#define b           x12
#define mantA       x13
#define expA        x18
#define mantB       x19
#define expB        x20
#define tmpA        x21
#define tmpB        x22
#define tmpC        x23

.text
.p2align 2



.L_32_Q24:
.word 0x20000000,0x20000000,0x20000000,0x20000000,0x20000000,0x20000000,0x20000000,0x20000000

FUNCTION_NAME:
  xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
  xm.stdsp  s3,s2,0
  xm.stdsp  s5,s4,8
  xm.stdsp  s7,s6,16

{ li t3, 0                  ; li tmpC, 24                }
{ li t3, 1                  ; xm.vsetc t3}

  xm.lddi  mantA,expA, 0(b)
  xm.lddi  mantB,expB, 8(b)
{ xm.cls tmpA, mantA             ; nop                               }
{ nop                                  ; xm.cls tmpB, mantB             }
{ sub expA, expA, tmpA        ; sub expB, expB, tmpB        }
{ addi tmpA, tmpA, -1           ; addi tmpB, tmpB, -1           }
{ xm.shl mantA, mantA, tmpA      ; xm.shl mantB, mantB, tmpB      }/* Multiple XAT warnings: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in  shli mantA, mantA, tmpA      \nMessage: The shift amount is not 32", "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in  shli mantB, mantB, tmpB      \nMessage: The shift amount is not 32" */
  xm.stdi  expA,expB, 0(exp_mod)
  xm.stdi  mantA,mantB, 0(a)

  xm.lddi  mantA,expA, 16(b)
  xm.lddi  mantB,expB, 24(b)
{ xm.cls tmpA, mantA             ; nop                               }
{ nop                                  ; xm.cls tmpB, mantB             }
{ sub expA, expA, tmpA        ; sub expB, expB, tmpB        }
{ addi tmpA, tmpA, -1           ; addi tmpB, tmpB, -1           }
{ xm.shl mantA, mantA, tmpA      ; xm.shl mantB, mantB, tmpB      }/* Multiple XAT warnings: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in  shli mantA, mantA, tmpA      \nMessage: The shift amount is not 32", "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in  shli mantB, mantB, tmpB      \nMessage: The shift amount is not 32" */
  xm.stdi  expA,expB, 8(exp_mod)
  xm.stdi  mantA,mantB, 8(a)

  xm.lddi  mantA,expA, 32(b)
  xm.lddi  mantB,expB, 40(b)
{ xm.cls tmpA, mantA             ; nop                               }
{ nop                                  ; xm.cls tmpB, mantB             }
{ sub expA, expA, tmpA        ; sub expB, expB, tmpB        }
{ addi tmpA, tmpA, -1           ; addi tmpB, tmpB, -1           }
{ xm.shl mantA, mantA, tmpA      ; xm.shl mantB, mantB, tmpB      }/* Multiple XAT warnings: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in  shli mantA, mantA, tmpA      \nMessage: The shift amount is not 32", "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in  shli mantB, mantB, tmpB      \nMessage: The shift amount is not 32" */
  xm.stdi  expA,expB, 16(exp_mod)
  xm.stdi  mantA,mantB, 16(a)

  xm.lddi  mantA,expA, 48(b)
  xm.lddi  mantB,expB, 56(b)
{ xm.cls tmpA, mantA             ; nop                               }
{ nop                                  ; xm.cls tmpB, mantB             }
{ sub expA, expA, tmpA        ; sub expB, expB, tmpB        }
{ addi tmpA, tmpA, -1           ; addi tmpB, tmpB, -1           }
{ xm.shl mantA, mantA, tmpA      ; xm.shl mantB, mantB, tmpB      }/* Multiple XAT warnings: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in  shli mantA, mantA, tmpA      \nMessage: The shift amount is not 32", "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in  shli mantB, mantB, tmpB      \nMessage: The shift amount is not 32" */
  xm.stdi  expA,expB, 24(exp_mod)
  xm.stdi  mantA,mantB, 24(a)

  xm.vlashr a, t3
{ xm.neg tmpC, tmpC              ; nop}
{nop ; xm.vstr a}

lui t3, %hi(.L_32_Q24)
  addi t3,t3, %lo(.L_32_Q24)
  xm.vlashr exp_mod, tmpC
{ nop                             ; xm.vladd t3}
{ nop                             ; xm.vstr exp_mod}


.L_finish:
  xm.lddsp  s3,s2,0
  xm.lddsp  s5,s4,8
  xm.lddsp  s7,s6,16
  xm.retsp (NSTACKWORDS)*4/* Multiple XAT warnings: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in retsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate", 'RETSP operand may need scaling' */
.L_func_end:


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS; .global FUNCTION_NAME.nstackwords /* Translation error on this line: unexpected token at position 42. */ 
.set FUNCTION_NAME.maxcores,1;              .global FUNCTION_NAME.maxcores /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;             .global FUNCTION_NAME.maxtimers /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;           .global FUNCTION_NAME.maxchanends /* Translation error on this line: unexpected token at position 32. */ 
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME


#endif //defined(__VX4B__)



