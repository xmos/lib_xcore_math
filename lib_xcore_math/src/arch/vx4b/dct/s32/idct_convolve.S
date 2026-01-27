// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#if defined(__VX4B__)


/*  

Applies the convolution needed for the recursive IDCT.

given x[], the result is

  y[0] = x[0]
  y[1:] = 0.5*(y[1:] + y[0:-1])

Each "chunk" is 8 elements, so if the data isn't a multiple of 8 elements
you'll need buffer space at the end of the data that can be safely clobbered.

void idct_convolve(
    int32_t y[],
    const int32_t x[],
    const unsigned chunks);

*/

#define FUNCTION_NAME   idct_convolve
#define NSTACKWORDS 4

.text
.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.p2align 2

#define y        x10
#define x        x11
#define chunks   x12
#define _32      x13
#define tmp      x18


FUNCTION_NAME:
  xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
  xm.stdsp  s3,s2,0

  // This has to start at the end or else values will get clobbered
  // early if it's done in-place

  li t3, 0x80  // 32-bit mode with SHR=1 on VLADSB
{ xm.shli s3, chunks, 3           ; xm.vsetc t3}
{ li _32, 32                 ; lw tmp,0               ( x)}
  sh2add x, s3, x
  sh2add y, s3, y
{ sub x, x, _32               ; sub y, y, _32               }
{ addi t3, x, -4               ; nop                             }

.L_loop_top:
  { addi chunks, chunks, -1       ; xm.vldr t3}
  { sub x, x, _32               ; xm.vladsb x}
  { sub y, y, _32               ; xm.vstr y}
  { addi t3, x, -4               ; xm.bt chunks, .L_loop_top      }
.L_loop_bot:
  
  sw tmp,32 ( y)// y is pointing 8 words before where it started
  
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
