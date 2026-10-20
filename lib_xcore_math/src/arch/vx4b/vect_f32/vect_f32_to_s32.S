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

#define NSTACKWORDS   4
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
  { li t3, 23 ; xm.entsp (NSTACKWORDS)*4 }
    xm.stdsp s2, s3, 0
  { add exp, exp, t3 ; xm.stwsp s4, 8 }

    //handle tail first
  { srl s2, len, 1                      ; xm.zext len, 1                        }
  { mv len, s2                          ; xm.brff len, .L_pre_loop              }

    xm.ldd mant0, mant1, len(b)
    xm.fsexp x0, t3, mant0
    xm.fmants mant0, mant0
    sub t3, t3, exp
  { xm.shl mant0, mant0, t3             ; xm.shl s2, len, 1                     }
    xm.stw mant0, s2(a)

.L_pre_loop:
  { addi len, len, -1                   ; xm.brff len, .L_loop_end              }

  .L_loop:
      xm.ldd mant0, mant1, len(b)
      xm.fsexp x0, t3, mant1
      xm.fmants mant1, mant1
      xm.fsexp x0, s4, mant0
      xm.fmants mant0, mant0

    { sub t3, t3, exp                 ; sub s4, s4, exp                       }
    { xm.shl mant1, mant1, t3         ; xm.shl mant0, mant0, s4               }
      xm.std mant0, mant1, len(a)

    { addi len, len, -1               ; xm.bt  len, .L_loop                   }
  .L_loop_end:

    xm.stwsp s4, 8
    xm.lddsp s2, s3, 0
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
