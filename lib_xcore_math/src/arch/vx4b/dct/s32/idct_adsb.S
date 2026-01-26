// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#if defined(__VX4B__)


/*  

void idct_adsb(
    int32_t sums[],
    int32_t diffs[],
    const int32_t head[],
    const int32_t tail[],
    const unsigned chunks);

*/

#define FUNCTION_NAME   idct_adsb
#define NSTACKWORDS 4

.text
.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.p2align 2

#define STK_CHUNKS  (NSTACKWORDS+1)

#define sums        x10
#define diffs       x11
#define s           x12
#define t_tilde     x13
#define chunks      x18
#define _32         x19


FUNCTION_NAME:
  xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
  xm.stdsp  s3,s2,0
  xm.stdsp  s5,s4,8

{ li _32, 32                 ; lw chunks, (STK_CHUNKS)*4  (sp)}
{ li t3, 0                  ; nop                             }
{ mv t3, t_tilde            ; xm.vsetc t3}

.L_loop_top:
  { addi chunks, chunks, -1       ; xm.vldr t3}
  { add t3, t3, _32           ; xm.vladsb s}
  { add s, s, _32               ; xm.vstr sums}
  { add sums, sums, _32         ; xm.vstd diffs}
  { add diffs, diffs, _32       ; xm.bt chunks, .L_loop_top      }
.L_loop_bot:

  xm.lddsp  s5,s4,8
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
