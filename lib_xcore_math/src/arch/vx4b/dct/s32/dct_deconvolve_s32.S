// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#if defined(__VX4B__)


/*  

length must be a multiple of 8

void dct_deconvolve_s32(
    int32_t res[],
    const int32_t B[],
    const int32_t D[],
    const unsigned length);

*/

#define FUNCTION_NAME   dct_deconvolve_s32
#define NSTACKWORDS 4

.text
.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.p2align 2

#define res     x10
#define B       x11
#define D       x12
#define len     x13
#define even    x18
#define a       x19
#define b       x20
#define c       x21


FUNCTION_NAME:
  xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
  xm.stdsp  s3,s2,0
  xm.stdsp  s5,s4,8

// Just set it up so that c contains half D[0], so that when
// it's subtracted from D[0] we get (D[0] >> 1)
  { srli len, len, 3             ; lw c,0                 ( D)}
    srai c, c, 1
  { li t3, 16                 ; xm.bu .L_loop_top              }

.p2align 4
.L_loop_top:
    xm.lddi  a,b, 0(D)
  { sub a, a, c                 ; lw even,0              ( B)}
    xm.stdi  even,a, 0(res)
  { sub b, b, a                 ; lw even,4              ( B)}
    xm.stdi  even,b, 8(res)
    xm.lddi  a,c, 8(D)
  { sub a, a, b                 ; lw even,8              ( B)}
    xm.stdi  even,a, 16(res)
  { sub c, c, a                 ; lw even,12              ( B)}
    xm.stdi  even,c, 24(res)
  { add D, D, t3               ; addi len, len, -1             }
  { add res, res, t3           ; add B, B, t3               }
  { add res, res, t3           ; xm.bt len, .L_loop_top         }
.L_loop_bot:

.L_finish:
  xm.lddsp  s5,s4,8
  xm.lddsp  s3,s2,0  
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
