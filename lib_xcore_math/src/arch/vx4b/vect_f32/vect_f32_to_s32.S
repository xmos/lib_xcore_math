// Copyright 2022-2026 XMOS LIMITED.
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

#define a         a0
#define b         a1
#define len       a2
#define exp       a3

#define mant1     s2
#define mant0     s3


.p2align 4
FUNC_NAME:
    xm.entsp (NSTACKWORDS)*4
    xm.stdsp  s3,s2,0
    xm.stdsp  s5,s4,8

    //handle tail first
  { srli mant1, len, 1                  ; li t3, 23                            }
  { xm.zexti len, 1                     ; add exp, exp, t3                     }
  { mv len, mant1                       ; xm.brff len, .L_pre_loop              }
    xm.ldd  mant0,mant1, len(b)
    xm.fsexp t2, t3, mant0
    xm.fmants mant0, mant0
    sub t3, t3, exp
  { xm.shl mant0, mant0, t3             ; xm.shl mant1, len, 1                     }
    xm.stw mant0, mant1(a)

.L_pre_loop:
  { addi len, len, -1                   ; xm.brff len, .L_loop_end              }

.L_loop:
      xm.ldd  mant0,mant1, len(b)
      xm.fsexp t2, t3, mant1
      xm.fmants mant1, mant1
      xm.fsexp t2, s5, mant0
      xm.fmants mant0, mant0
      { sub t3, t3, exp                 ; sub s5, s5, exp                       }
      { xm.shl mant1, mant1, t3         ; xm.shl mant0, mant0, t3              }
      xm.std  mant0,mant1, len(a)
      { addi len, len, -1               ; xm.bt  len, .L_loop                   }
.L_loop_end:


      xm.lddsp  s3,s2,0
      xm.lddsp  s5,s4,8
      xm.retsp (NSTACKWORDS)*4
	
	// RETURN_REG_HOLDER
.resource_const FUNC_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNC_NAME, "tail_callees"
.resource_list_empty FUNC_NAME, "callees"
.resource_list_empty FUNC_NAME, "parallel_callees"
.Ltmp0:
	.size	FUNC_NAME, .Ltmp0-FUNC_NAME

#undef NSTACKWORDS
        
#endif
