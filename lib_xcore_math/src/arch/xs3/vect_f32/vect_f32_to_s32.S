// Copyright 2022-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
    
#if defined(__XS3A__)

.text
.issue_mode  dual


/*

  void vect_f32_to_vect_s32(
      int32_t a[],
      const float b[], 
      const unsigned length,
      const exponent_t exp);
       
*/

#define NSTACKWORDS   4
#define FUNC_NAME     vect_f32_to_vect_s32

.globl	FUNC_NAME
.type	FUNC_NAME,@function
.cc_top FUNC_NAME.function,FUNC_NAME

#define a         r0
#define b         r1
#define len       r2
#define exp       r3

#define mant1     r4
#define mant0     r5
#define tmp       r6


.align 16
FUNC_NAME:
    dualentsp NSTACKWORDS
    std r4, r5, sp[0]
    std r6, r7, sp[1]

    //handle tail first
  { shr r11, len, 1             ; ldc tmp, 23                 }
  { zext len, 1                 ; add exp, exp, tmp           }
  { mov len, r11                ; bf len, .L_pre_loop         }
    ldd mant1, mant0, b[len]
    fsexp r11, tmp, mant0
    fmant mant0, mant0
  { sub tmp, tmp, exp           ; bf r11, .L_tail_pos         }
  { neg mant0, mant0            ;                             }
.L_tail_pos:
  { shl mant0, mant0, tmp       ; shl r11, len, 1             }
    stw mant0, a[r11]

.L_pre_loop:
  { sub len, len, 1             ; bf len, .L_loop_end         }

  .L_loop:
      ldd mant1, mant0, b[len]
      fsexp r11, tmp, mant1
      fmant mant1, mant1
    { sub tmp, tmp, exp           ; bf r11, .L_not3             }
      neg mant1, mant1
    .L_not3:
        shl mant1, mant1, tmp
        fsexp r11, tmp, mant0
        fmant mant0, mant0
      { sub tmp, tmp, exp           ; bf r11, .L_not4             }
        neg mant0, mant0
    .L_not4:
        shl mant0, mant0, tmp
        std mant1, mant0, a[len]
      { sub len, len, 1             ; bt  len, .L_loop            }
  .L_loop_end:


      ldd r4, r5, sp[0]
      ldd r6, r7, sp[1]
      retsp NSTACKWORDS
	
	// RETURN_REG_HOLDER
	.cc_bottom FUNC_NAME.function
	.set	FUNC_NAME.nstackwords,NSTACKWORDS;  .globl	FUNC_NAME.nstackwords
	.set	FUNC_NAME.maxcores,1;              	.globl	FUNC_NAME.maxcores
	.set	FUNC_NAME.maxtimers,0;              .globl	FUNC_NAME.maxtimers
	.set	FUNC_NAME.maxchanends,0;            .globl	FUNC_NAME.maxchanends
.Ltmp0:
	.size	FUNC_NAME, .Ltmp0-FUNC_NAME

#undef NSTACKWORDS
        
#endif