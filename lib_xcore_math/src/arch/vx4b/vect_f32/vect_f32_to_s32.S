// Copyright 2022-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
    
#if defined(__VX4B__)

.text


/*

  void vect_f32_to_vect_s32(
      int32_t a[],
      const float b[], 
      const unsigned length,
      const exponent_t exp);
       
*/

#define NSTACKWORDS   8
#define FUNC_NAME     vect_f32_to_vect_s32

.globl	FUNC_NAME
.type	FUNC_NAME,@function

#define a         x10
#define b         x11
#define len       x12
#define exp       x13

#define mant1     x18
#define mant0     x19
#define tmp       x20


.p2align 4
FUNC_NAME:
    xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
    xm.stdsp  s3,s2,0
    xm.stdsp  s5,s4,8

    //handle tail first
  { srli t3, len, 1             ; li tmp, 23                 }
  { xm.zexti len, 1                 ; add exp, exp, tmp           }
  { mv len, t3                ; xm.brff len, .L_pre_loop         }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
    xm.ldd  mant0,mant1, len(b)
    xm.fsexp t3, tmp, mant0
    xm.fmant mant0, mant0
  { sub tmp, tmp, exp           ; xm.brff t3, .L_tail_pos         }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
  { xm.neg mant0, mant0            ; nop                             }
.L_tail_pos:
  { xm.shl mant0, mant0, tmp       ; xm.shl t3, len, 1             }/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in  shli mant0, mant0, tmp       \nMessage: The shift amount is not 32" */
    xm.stw mant0, t3(a)

.L_pre_loop:
  { addi len, len, -1             ; xm.brff len, .L_loop_end         }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */

  .L_loop:
      xm.ldd  mant0,mant1, len(b)
      xm.fsexp t3, tmp, mant1
      xm.fmant mant1, mant1
    { sub tmp, tmp, exp           ; xm.brff t3, .L_not3             }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
      xm.neg mant1, mant1
    .L_not3:
        xm.shl mant1, mant1, tmp/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in shli mant1, mant1, tmp\nMessage: The shift amount is not 32" */
        xm.fsexp t3, tmp, mant0
        xm.fmant mant0, mant0
      { sub tmp, tmp, exp           ; xm.brff t3, .L_not4             }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
        xm.neg mant0, mant0
    .L_not4:
        xm.shl mant0, mant0, tmp/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in shli mant0, mant0, tmp\nMessage: The shift amount is not 32" */
        xm.std  mant0,mant1, len(a)
      { addi len, len, -1             ; xm.bt  len, .L_loop            }
  .L_loop_end:


      xm.lddsp  s3,s2,0
      xm.lddsp  s5,s4,8
      xm.retsp (NSTACKWORDS)*4/* Multiple XAT warnings: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in retsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate", 'RETSP operand may need scaling' */
	
	// RETURN_REG_HOLDER
	.set	FUNC_NAME.nstackwords,NSTACKWORDS;  .globl	FUNC_NAME.nstackwords /* Translation error on this line: unexpected token at position 39. */ 
	.set	FUNC_NAME.maxcores,1;              	.globl	FUNC_NAME.maxcores /* Translation error on this line: unexpected token at position 26. */ 
	.set	FUNC_NAME.maxtimers,0;              .globl	FUNC_NAME.maxtimers /* Translation error on this line: unexpected token at position 27. */ 
	.set	FUNC_NAME.maxchanends,0;            .globl	FUNC_NAME.maxchanends /* Translation error on this line: unexpected token at position 29. */ 
.Ltmp0:
	.size	FUNC_NAME, .Ltmp0-FUNC_NAME

#undef NSTACKWORDS
        
#endif
