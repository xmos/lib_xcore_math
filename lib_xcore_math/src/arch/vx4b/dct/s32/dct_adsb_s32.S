// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#if defined(__VX4B__)


/*  

headroom_t dct_adsb_s32(
    int32_t sums[],
    int32_t diffs[],
    const int32_t head[],
    const int32_t tail[],
    const unsigned chunks,
    const int32_t dct_lut[]);

*/

#define FUNCTION_NAME   dct_adsb_s32
#define NSTACKWORDS 4

.text
.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.p2align 2

#define STK_CHUNKS  (NSTACKWORDS+1)
#define STK_LUT     (NSTACKWORDS+2)

#define sums        x10
#define diffs       x11
#define head        x12
#define tail        x13
#define chunks      x18
#define lut         x19
#define _32         x20


FUNCTION_NAME:
  xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
  xm.stdsp  s3,s2,0
  xm.stdsp  s5,s4,8

{ li _32, 32                 ; lw chunks, (STK_CHUNKS)*4  (sp)}
{ xm.shli t3, _32, 2             ; lw lut, (STK_LUT)*4        (sp)}
{ mv t3, tail               ; xm.vsetc t3}

.L_loop_top:
  { addi chunks, chunks, -1       ; xm.vldr t3}
  { add tail, tail, _32         ; xm.vladsb head}
  { add head, head, _32         ; xm.vstr sums}
  { mv t3, lut                ; xm.vstd diffs}
  { add lut, lut, _32           ; xm.vldr t3}
  { add sums, sums, _32         ; xm.vlmul0 diffs}
  { add diffs, diffs, _32       ; xm.vstr diffs}
  { mv t3, tail               ; xm.bt chunks, .L_loop_top      }
.L_loop_bot:

  xm.lddsp  s5,s4,8
  xm.lddsp  s3,s2,0
{ li a0, 31                ; xm.vgetc t3}
{ xm.zexti t3, 5               ; nop                             }
{ sub a0, a0, t3           ; xm.retsp (NSTACKWORDS)*4           }

	
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
