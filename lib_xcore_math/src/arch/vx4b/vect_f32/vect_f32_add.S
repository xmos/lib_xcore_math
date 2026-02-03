// Copyright 2022-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
    
#if defined(__VX4B__)

.text

/*
  Note: This works for real or complex floats, just use double the length for complex.

  a[k] = b[k] + c[k]

  It is safe to use the same argument twice, so
    vect_f32_add(x[], x[], y[])  -->   x += y

  Note: a[], b[] and c[] must all be 8-byte aligned

  void vect_f32_add(
      float a[],
      const float b[],
      const float c[],
      const unsigned length);

*/

#define FUNC_NAME     vect_f32_add
#define NSTACKWORDS   8

.globl	FUNC_NAME
.type	FUNC_NAME,@function

#define a         a0
#define b         a1
#define c         a2
#define len       a3
#define B0        s2
#define B1        s3
#define C0        s4
#define C1        s5

.p2align 4
FUNC_NAME:
  xm.entsp (NSTACKWORDS)*4
  xm.stdsp  s3,s2,0
  xm.stdsp  s5,s4,8

{ mv t3, len                            ; xm.zexti len, 1                       }
{ srli len, t3, 1                       ; xm.brff len, .L_even                  }
.L_odd:
  { addi t3, t3, -1                     ; nop                                   }
  { nop                                 ; xm.ldw B0, t3            (b)          }
  { nop                                 ; xm.ldw C0, t3            (c)          }
    xm.fadd B0, B0, C0
    xm.stw B0, t3(a)
.L_even:

{ addi len, len, -1                     ; xm.brff len, .L_loop_bot              }
{ nop                                   ; xm.bu .L_loop_top                     }

.p2align 4
.L_loop_top:
    xm.ldd  B0,B1, len(b)
    xm.ldd  C0,C1, len(c)
    xm.fadd B0, B0, C0
    xm.fadd B1, B1, C1
    xm.std  B0,B1, len(a)
  { addi len, len, -1                   ; xm.bt len, .L_loop_top                }
.L_loop_bot:

.L_done:
  xm.lddsp  s3,s2,0
  xm.lddsp  s5,s4,8
  addi a0, t3, 0
  xm.retsp (NSTACKWORDS)*4
    
	
	// RETURN_REG_HOLDER
	.set	FUNC_NAME.nstackwords,NSTACKWORDS;     .globl	FUNC_NAME.nstackwords
	.set	FUNC_NAME.maxcores,1;                  .globl	FUNC_NAME.maxcores
	.set	FUNC_NAME.maxtimers,0;                 .globl	FUNC_NAME.maxtimers
	.set	FUNC_NAME.maxchanends,0;               .globl	FUNC_NAME.maxchanends
.Ltmp1:
	.size	FUNC_NAME, .Ltmp1-FUNC_NAME

#undef NSTACKWORDS


#endif
