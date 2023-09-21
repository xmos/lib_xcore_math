// Copyright 2022-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
    
#if defined(__XS3A__)

.text
.issue_mode  dual

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
#define NSTACKWORDS   4

.globl	FUNC_NAME
.type	FUNC_NAME,@function
.cc_top FUNC_NAME.function,FUNC_NAME

#define a         r0
#define b         r1
#define c         r2
#define len       r3
#define B0        r4
#define B1        r5
#define C0        r6
#define C1        r7

.align 16
FUNC_NAME:
  dualentsp NSTACKWORDS
  std r4, r5, sp[0]
  std r6, r7, sp[1]

{ mov r11, len                ; zext len, 1                 }
{ shr len, r11, 1             ; bf len, .L_even             }
.L_odd:
  { sub r11, r11, 1             ;                           }
  {                             ; ldw B0, b[r11]            }
  {                             ; ldw C0, c[r11]            }
    fadd B0, B0, C0
    stw B0, a[r11]
.L_even:

{ sub len, len, 1           ; bf len, .L_loop_bot           }
{                           ; bu .L_loop_top                }

.align 16
.L_loop_top:
    ldd B1, B0, b[len]
    ldd C1, C0, c[len]
    fadd B0, B0, C0
    fadd B1, B1, C1
    std B1, B0, a[len]
  { sub len, len, 1             ; bt len, .L_loop_top         }
.L_loop_bot:

.L_done:
  ldd r4, r5, sp[0]
  ldd r6, r7, sp[1]
  add r0, r11, 0
  retsp NSTACKWORDS
    
	
	// RETURN_REG_HOLDER
	.cc_bottom FUNC_NAME.function
	.set	FUNC_NAME.nstackwords,NSTACKWORDS;     .globl	FUNC_NAME.nstackwords
	.set	FUNC_NAME.maxcores,1;                  .globl	FUNC_NAME.maxcores
	.set	FUNC_NAME.maxtimers,0;                 .globl	FUNC_NAME.maxtimers
	.set	FUNC_NAME.maxchanends,0;               .globl	FUNC_NAME.maxchanends
.Ltmp1:
	.size	FUNC_NAME, .Ltmp1-FUNC_NAME

#undef NSTACKWORDS


#endif