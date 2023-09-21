// Copyright 2022-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
    
#if defined(__XS3A__)

.text
.issue_mode  dual

/*

  void vect_s32_to_vect_f32(
      float a[],
      const int32_t b[], 
      const unsigned length, 
      const exponent_t b_exp);

*/

#define NSTACKWORDS 4
#define FUNC_NAME vect_s32_to_vect_f32
    
.globl	FUNC_NAME
.type	FUNC_NAME,@function
.cc_top FUNC_NAME.function,FUNC_NAME

#define a       r0
#define b       r1
#define len     r2
#define b_exp   r3

#define _0      r4
#define tmp1    r5
#define tmp0    r6

.align 16
FUNC_NAME:
    dualentsp NSTACKWORDS
    std r4, r5, sp[0]
  { ldc _0, 0                   ; ldc r5, 23                  }
  { add b_exp, b_exp, r5        ;                             }
    std r6, r7, sp[1]

  // handle tail first
  { shr r11, len, 1             ; zext len, 1                 }
  { mov len, r11                ; bf len, .L_pre_loop         }
    
    ldd tmp1, tmp0, b[len]
    { lss r11, tmp0, _0           ; shl tmp1, len, 1            }
    bf r11, .L_posT
      neg tmp0, tmp0
    .L_posT:
    fmake tmp0, r11, b_exp, _0, tmp0
    stw tmp0, a[tmp1]

  .L_pre_loop:

  { sub len, len, 1               ; bf len, .L_loop_end           }
  
  .L_loop:
    ldd tmp1, tmp0, b[len]
  { lss r11, tmp1, _0             ;                               }
    bf r11, .L_pos1
      neg tmp1, tmp1
    .L_pos1:
    fmake tmp1, r11, b_exp, _0, tmp1
    lss r11, tmp0, _0
    bf r11, .L_pos0
      neg tmp0, tmp0
    .L_pos0:
    fmake tmp0, r11, b_exp, _0, tmp0
    std tmp1, tmp0, a[len]
    { sub len, len, 1             ; bt len, .L_loop               }
  .L_loop_end:
    
    ldd r4, r5, sp[0]
    ldd r6, r7, sp[1]
    retsp NSTACKWORDS

	
	// RETURN_REG_HOLDER
	.cc_bottom FUNC_NAME.function
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