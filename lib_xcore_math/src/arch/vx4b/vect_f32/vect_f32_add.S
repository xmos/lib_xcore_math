// Copyright 2022-2023 XMOS LIMITED.
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

#define a         x10
#define b         x11
#define c         x12
#define len       x13
#define B0        x18
#define B1        x19
#define C0        x20
#define C1        x21

.p2align 4
FUNC_NAME:
  xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
  xm.stdsp  s3,s2,0
  xm.stdsp  s5,s4,8

{ mv t3, len                ; xm.zexti len, 1                 }
{ srli len, t3, 1             ; xm.brff len, .L_even             }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
.L_odd:
  { addi t3, t3, -1             ; nop                           }
  { nop                             ; xm.ldw B0, t3            (b)}
  { nop                             ; xm.ldw C0, t3            (c)}
    xm.fadd B0, B0, C0
    xm.stw B0, t3(a)
.L_even:

{ addi len, len, -1           ; xm.brff len, .L_loop_bot           }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
{ nop                           ; xm.bu .L_loop_top                }

.p2align 4
.L_loop_top:
    xm.ldd  B0,B1, len(b)
    xm.ldd  C0,C1, len(c)
    xm.fadd B0, B0, C0
    xm.fadd B1, B1, C1
    xm.std  B0,B1, len(a)
  { addi len, len, -1             ; xm.bt len, .L_loop_top         }
.L_loop_bot:

.L_done:
  xm.lddsp  s3,s2,0
  xm.lddsp  s5,s4,8
  addi a0, t3, 0
  xm.retsp (NSTACKWORDS)*4/* Multiple XAT warnings: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in retsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate", 'RETSP operand may need scaling' */
    
	
	// RETURN_REG_HOLDER
	.set	FUNC_NAME.nstackwords,NSTACKWORDS;     .globl	FUNC_NAME.nstackwords /* Translation error on this line: unexpected token at position 39. */ 
	.set	FUNC_NAME.maxcores,1;                  .globl	FUNC_NAME.maxcores /* Translation error on this line: unexpected token at position 26. */ 
	.set	FUNC_NAME.maxtimers,0;                 .globl	FUNC_NAME.maxtimers /* Translation error on this line: unexpected token at position 27. */ 
	.set	FUNC_NAME.maxchanends,0;               .globl	FUNC_NAME.maxchanends /* Translation error on this line: unexpected token at position 29. */ 
.Ltmp1:
	.size	FUNC_NAME, .Ltmp1-FUNC_NAME

#undef NSTACKWORDS


#endif
