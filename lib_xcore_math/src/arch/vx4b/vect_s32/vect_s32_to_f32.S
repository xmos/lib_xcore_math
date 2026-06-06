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

#define NSTACKWORDS 4
#define FUNC_NAME vect_s32_to_vect_f32
    
.globl	FUNC_NAME
.type	FUNC_NAME,@function

#define a       a0
#define b       a1
#define len     a2
#define b_exp   a3

#define tmp1    s2
#define tmp0    s3

#define _0      x28

.p2align 4
FUNC_NAME:
  { xm.entsp (NSTACKWORDS)*4          ; li _0, 0 }
    xm.stdsp  s3,s2,0
    xm.subi b_exp, b_exp, 9

  // handle tail first
  { srli tmp0, len, 1                     ; xm.zexti len, 1                       }
  { mv len, tmp0                          ; xm.brff len, .L_pre_loop              }
    
    xm.ldd  tmp0,tmp1, len(b)
    slli tmp1, len, 1
    xm.fmakes tmp0, b_exp, tmp0, _0
    xm.stw tmp0,tmp1( a)

  .L_pre_loop:

  { addi len, len, -1                   ; xm.brff len, .L_loop_end              }
  
  .L_loop:
    xm.ldd    tmp0,tmp1, len(b)
    xm.fmakes tmp1, b_exp, tmp1, _0
    xm.fmakes tmp0, b_exp, tmp0, _0
    xm.std  tmp0,tmp1, len(a)
    { addi len, len, -1                 ; xm.bt len, .L_loop                    }
  .L_loop_end:
    
    xm.lddsp  s3,s2,0
    xm.retsp (NSTACKWORDS)*4

	
	// RETURN_REG_HOLDER
.resource_const FUNC_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNC_NAME, "tail_callees"
.resource_list_empty FUNC_NAME, "callees"
.resource_list_empty FUNC_NAME, "parallel_callees"
.Ltmp1:
	.size	FUNC_NAME, .Ltmp1-FUNC_NAME

        
#endif
