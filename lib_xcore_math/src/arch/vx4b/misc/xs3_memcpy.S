// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


/*  
void xs3_memcpy(
    void* dst,
    const void* src,
    unsigned bytes);
*/

#define NSTACKWORDS     (0)
#define FUNCTION_NAME   xs3_memcpy

#define a           x10
#define b           x11
#define len         x12
#define tmp         x13

.text
.p2align 2


FUNCTION_NAME:
    xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
  { srli tmp, len, 5               ; xm.zexti len, 5                   }
  { li t3, 32                   ; xm.brff tmp, .L_loop_bot           } /* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
  .L_loop_top:
    { add b, b, t3                 ; xm.vldd b}
    { addi tmp, tmp, -1               ; xm.vstd a}
    { add a, a, t3                 ; xm.bt tmp, .L_loop_top           }
  .L_loop_bot:
  { xm.mkmsk len, len                ; xm.brff len, .L_finish             }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
  { mv t3, b                    ; nop                               }
  { nop                               ; xm.vldr t3}
    xm.vstrpv a, len
.L_finish:
    xm.retsp (NSTACKWORDS)*4/* Multiple XAT warnings: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in retsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate", 'RETSP operand may need scaling' */


.L_func_end:

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;  .global FUNCTION_NAME.nstackwords /* Translation error on this line: unexpected token at position 42. */ 
.set FUNCTION_NAME.maxcores,1;               .global FUNCTION_NAME.maxcores /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;              .global FUNCTION_NAME.maxtimers /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;            .global FUNCTION_NAME.maxchanends /* Translation error on this line: unexpected token at position 32. */ 


#endif //defined(__VX4B__)
