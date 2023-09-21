// Copyright 2022-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
    
#if defined(__XS3A__)

.text
.issue_mode  dual

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
#define NSTACKWORDS   6

.globl	FUNC_NAME
.type	FUNC_NAME,@function
.cc_top FUNC_NAME.function,FUNC_NAME

#define a         r0
#define b         r1
#define c         r2
#define len       r3
#define B_re      r4
#define B_im      r5
#define C_re      r6
#define C_im      r7
#define A_re      r8
#define A_im      r9

.align 16
FUNC_NAME:
  dualentsp NSTACKWORDS
  std r4, r5, sp[0]
  std r6, r7, sp[1]
  std r8, r9, sp[2]

{ ldc r11, 0                  ; sub len, len, 1             }
.L_loop_top:
    ldd A_im, A_re, a[len]
    ldd B_im, B_re, b[len]
    ldd C_im, C_re, c[len]
    fmacc A_im, A_im, B_re, C_im   // A[k].im += B[k].re * C[k].im
    fmacc A_im, A_im, C_re, B_im  // A[k].im  += C[k].re * B[k].im
    fsub B_im, r11, B_im    // B_im <-- 0 - B[k].im = -B[k].im
    fmacc A_re, A_re, B_re, C_re   // A[k].re += B[k].re * C[k].re
    fmacc A_re, A_re, B_im, C_im  // A[k].re  -= B[k].im * C[k].im
    std A_im, A_re, a[len]
  { sub len, len, 1             ; bt len, .L_loop_top         }
.L_loop_bot:

.L_done:
  ldd r4, r5, sp[0]
  ldd r6, r7, sp[1]
  ldd r8, r9, sp[2]
  retsp NSTACKWORDS
    
		.cc_bottom FUNC_NAME.function
	.set	FUNC_NAME.nstackwords,NSTACKWORDS;     .globl	FUNC_NAME.nstackwords
	.set	FUNC_NAME.maxcores,1;                  .globl	FUNC_NAME.maxcores
	.set	FUNC_NAME.maxtimers,0;                 .globl	FUNC_NAME.maxtimers
	.set	FUNC_NAME.maxchanends,0;               .globl	FUNC_NAME.maxchanends
.Ltmp1:
	.size	FUNC_NAME, .Ltmp1-FUNC_NAME

#undef NSTACKWORDS


#endif