// Copyright 2022-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
    
#if defined(__VX4B__)

.text

/*
  Complex multiply

  a[k] = b[k] (*c) ( conjugate(c[k]) )

  It is safe to use the same argument twice, so
    vect_complex_f32_conj_mul(x[], x[], y[])  -->   x *= y

  Note: a[], b[] and c[] must all be 8-byte aligned

  void vect_complex_f32_conj_mul(
      complex_float_t a[],
      const complex_float_t b[],
      const complex_float_t c[],
      const unsigned length);

*/

#define FUNC_NAME     vect_complex_f32_conj_mul
#define NSTACKWORDS   12

.globl	FUNC_NAME
.type	FUNC_NAME,@function

#define a         a0
#define b         a1
#define c         a2
#define len       a3
#define B_re      x18
#define B_im      x19
#define C_re      s4
#define C_im      s5
#define A_re      x22
#define A_im      s7

.p2align 4
FUNC_NAME:
  xm.entsp (NSTACKWORDS)*4
  xm.stdsp  s3,s2,0
  xm.stdsp  s5,s4,8
  xm.stdsp  s7,s6,16

{ li t3, 0                              ; addi len, len, -1                     }
.L_loop_top:
    xm.ldd  B_re,B_im, len(b)
    xm.ldd  C_re,C_im, len(c)

    xm.fmul A_re, B_re, C_re         // A[k].re  = B[k].re * C[k].re
    xm.fmacc A_re, A_re, B_im, C_im  // A[k].re += B[k].im * C[k].im
    xm.fsub C_im, t3, C_im          // C_im <--  -C[k].im
    xm.fmul A_im, B_re, C_im         // A[k].im  = B_re * -C[k].im
    xm.fmacc A_im, A_im, C_re, B_im  // A[k].im += C_re * B[k].im

    xm.std  A_re,A_im, len(a)
  { addi len, len, -1                   ; xm.bt len, .L_loop_top                }
.L_loop_bot:

.L_done:
  xm.lddsp  s3,s2,0
  xm.lddsp  s5,s4,8
  xm.lddsp  s7,s6,16
  xm.retsp (NSTACKWORDS)*4
    
	.set	FUNC_NAME.nstackwords,NSTACKWORDS;     .globl	FUNC_NAME.nstackwords
	.set	FUNC_NAME.maxcores,1;                  .globl	FUNC_NAME.maxcores
	.set	FUNC_NAME.maxtimers,0;                 .globl	FUNC_NAME.maxtimers
	.set	FUNC_NAME.maxchanends,0;               .globl	FUNC_NAME.maxchanends
.Ltmp1:
	.size	FUNC_NAME, .Ltmp1-FUNC_NAME

#undef NSTACKWORDS


#endif
