// Copyright 2022-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
    
#if defined(__VX4B__)

.text

/*
  Complex multiply-accumulate

  a[k] = a[k] + b[k] (*c) c[k]

  Note: a[], b[] and c[] must all be 8-byte aligned

  void vect_complex_f32_macc(
      complex_float_t a[],
      const complex_float_t b[],
      const complex_float_t c[],
      const unsigned length);

*/

#define FUNC_NAME     vect_complex_f32_macc
#define NSTACKWORDS   12

.globl	FUNC_NAME
.type	FUNC_NAME,@function

#define a         x10
#define b         x11
#define c         x12
#define len       x13
#define B_re      x18
#define B_im      x19
#define C_re      x20
#define C_im      x21
#define A_re      x22
#define A_im      x23

.p2align 4
FUNC_NAME:
  xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
  xm.stdsp  s3,s2,0
  xm.stdsp  s5,s4,8
  xm.stdsp  s7,s6,16

{ li t3, 0                  ; addi len, len, -1             }
.L_loop_top:
    xm.ldd  A_re,A_im, len(a)
    xm.ldd  B_re,B_im, len(b)
    xm.ldd  C_re,C_im, len(c)
    xm.fmacc A_im, A_im, B_re, C_im   // A[k].im += B[k].re * C[k].im
    xm.fmacc A_im, A_im, C_re, B_im  // A[k].im  += C[k].re * B[k].im
    xm.fsub B_im, t3, B_im    // B_im <-- 0 - B[k].im = -B[k].im
    xm.fmacc A_re, A_re, B_re, C_re   // A[k].re += B[k].re * C[k].re
    xm.fmacc A_re, A_re, B_im, C_im  // A[k].re  -= B[k].im * C[k].im
    xm.std  A_re,A_im, len(a)
  { addi len, len, -1             ; xm.bt len, .L_loop_top         }
.L_loop_bot:

.L_done:
  xm.lddsp  s3,s2,0
  xm.lddsp  s5,s4,8
  xm.lddsp  s7,s6,16
  xm.retsp (NSTACKWORDS)*4/* Multiple XAT warnings: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in retsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate", 'RETSP operand may need scaling' */
    
	.set	FUNC_NAME.nstackwords,NSTACKWORDS;     .globl	FUNC_NAME.nstackwords /* Translation error on this line: unexpected token at position 39. */ 
	.set	FUNC_NAME.maxcores,1;                  .globl	FUNC_NAME.maxcores /* Translation error on this line: unexpected token at position 26. */ 
	.set	FUNC_NAME.maxtimers,0;                 .globl	FUNC_NAME.maxtimers /* Translation error on this line: unexpected token at position 27. */ 
	.set	FUNC_NAME.maxchanends,0;               .globl	FUNC_NAME.maxchanends /* Translation error on this line: unexpected token at position 29. */ 
.Ltmp1:
	.size	FUNC_NAME, .Ltmp1-FUNC_NAME

#undef NSTACKWORDS


#endif
