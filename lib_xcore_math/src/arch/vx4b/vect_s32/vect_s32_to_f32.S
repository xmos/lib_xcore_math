// Copyright 2022-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
    
#if defined(__VX4B__)

.text

/*

  void vect_s32_to_vect_f32(
      float a[],
      const int32_t b[], 
      const unsigned length, 
      const exponent_t b_exp);

*/

#define NSTACKWORDS 8
#define FUNC_NAME vect_s32_to_vect_f32
    
.globl	FUNC_NAME
.type	FUNC_NAME,@function

#define a       x10
#define b       x11
#define len     x12
#define b_exp   x13

#define _0      x18
#define tmp1    x19
#define tmp0    x20

.p2align 4
FUNC_NAME:
    xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
    xm.stdsp  s3,s2,0
  { li _0, 0                   ; li s3, 23                  }
  { add b_exp, b_exp, s3        ; nop                             }
    xm.stdsp  s5,s4,8

  // handle tail first
  { srli t3, len, 1             ; xm.zexti len, 1                 }
  { mv len, t3                ; xm.brff len, .L_pre_loop         }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
    
    xm.ldd  tmp0,tmp1, len(b)
    { xm.slt t3, tmp0, _0           ; slli tmp1, len, 1            }
    beqz t3, .L_posT
      xm.neg tmp0, tmp0
    .L_posT:
    xm.fmake tmp0, t3, b_exp, _0, tmp0
    xm.stw tmp0,tmp1( a)/* XAT Warning: "Falling back on assumption: the int < 12 for the integer value of the item at position 2 in the instruction's operands in stwi tmp0, a,tmp1\nMessage: The offset can be encoded in s2rus immediate" */

  .L_pre_loop:

  { addi len, len, -1               ; xm.brff len, .L_loop_end           }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
  
  .L_loop:
    xm.ldd  tmp0,tmp1, len(b)
  { xm.slt t3, tmp1, _0             ; nop                               }
    beqz t3, .L_pos1
      xm.neg tmp1, tmp1
    .L_pos1:
    xm.fmake tmp1, t3, b_exp, _0, tmp1
    slt t3, tmp0, _0
    beqz t3, .L_pos0
      xm.neg tmp0, tmp0
    .L_pos0:
    xm.fmake tmp0, t3, b_exp, _0, tmp0
    xm.std  tmp0,tmp1, len(a)
    { addi len, len, -1             ; xm.bt len, .L_loop               }
  .L_loop_end:
    
    xm.lddsp  s3,s2,0
    xm.lddsp  s5,s4,8
    xm.retsp (NSTACKWORDS)*4/* Multiple XAT warnings: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in retsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate", 'RETSP operand may need scaling' */

	
	// RETURN_REG_HOLDER
	.set	FUNC_NAME.nstackwords,NSTACKWORDS
	.globl	FUNC_NAME.nstackwords
	.set	FUNC_NAME.maxcores,1
	.globl	FUNC_NAME.maxcores
	.set	FUNC_NAME.maxtimers,0
	.globl	FUNC_NAME.maxtimers
	.set	FUNC_NAME.maxchanends,0
	.globl	FUNC_NAME.maxchanends
.Ltmp1:
	.size	FUNC_NAME, .Ltmp1-FUNC_NAME

        
#endif
