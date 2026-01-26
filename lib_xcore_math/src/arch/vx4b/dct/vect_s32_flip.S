// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#if defined(__VX4B__)


/*  

void vect_s32_flip(
    int32_t y[],
    const int32_t x[],
    const unsigned length);

*/

#define FUNCTION_NAME   vect_s32_flip
#define NSTACKWORDS 0

.text
.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.p2align 4

#define y       x10
#define x       x11
#define len     x12
#define a       x13
#define b       x28


FUNCTION_NAME:
  xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
{ addi x, x, -4                 ; addi y, y, -8                 }
.L_loop_top:
  { addi y, y, 4                 ; lw a,4                 ( x)}
  { addi x, x, 4                 ; xm.ldw b, len               ( x)}
    xm.stw a,len(y) /* XAT Warning: "Falling back on assumption: the int < 12 for the integer value of the item at position 2 in the instruction's operands in stwi a, y,len                       \nMessage: The offset can be encoded in s2rus immediate" */
  { addi len, len, -2             ; sw b,4                 ( y)}
  { nop                             ; xm.bt len, .L_loop_top         }
.L_loop_bottom:
  xm.retsp (NSTACKWORDS)*4  /* Multiple XAT warnings: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in retsp NSTACKWORDS  \nMessage: 0th operand fits in 6 bit unsigned immediate", 'RETSP operand may need scaling' */
	
.set	FUNCTION_NAME.nstackwords,NSTACKWORDS
.globl	FUNCTION_NAME.nstackwords
.set	FUNCTION_NAME.maxcores,1
.globl	FUNCTION_NAME.maxcores
.set	FUNCTION_NAME.maxtimers,0
.globl	FUNCTION_NAME.maxtimers
.set	FUNCTION_NAME.maxchanends,0
.globl	FUNCTION_NAME.maxchanends
.Ltmp0:
	.size	FUNCTION_NAME, .Ltmp0-FUNCTION_NAME    


#endif //defined(__VX4B__)
