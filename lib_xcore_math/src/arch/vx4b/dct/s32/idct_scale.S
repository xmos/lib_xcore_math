// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#if defined(__VX4B__)


/*  



void idct_scale(
    int32_t x[],
    const int32_t idct_lut[],
    const unsigned chunks,
    const right_shift_t shr);

*/

#define FUNCTION_NAME   idct_scale
#define NSTACKWORDS 4

.text
.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.p2align 2


#define x        x10
#define lut      x11
#define chunks   x12
#define shr      x13
#define _32      x18


FUNCTION_NAME:
  xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
  xm.stdsp  s3,s2,0

{ li t3, 0                  ; li _32, 32                 }
{ mv t3, lut                ; xm.vsetc t3}

.L_loop_top:
  { add t3, t3, _32           ; xm.vldr t3}
  { nop                             ; xm.vlmul0 x}
  { nop                             ; xm.vstr x}
    xm.vlashr x, shr
  { addi chunks, chunks, -1       ; xm.vstr x}
  { add x, x, _32               ; xm.bt chunks, .L_loop_top      }
.L_loop_bot:
  
  xm.lddsp  s3,s2,0
  xm.retsp (NSTACKWORDS)*4/* Multiple XAT warnings: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in retsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate", 'RETSP operand may need scaling' */

	
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
