// Copyright 2022-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
    
#if defined(__VX4B__)

.text


/*

  exponent_t vect_f32_max_exponent(
      const float b[], 
      const unsigned length);
       
*/

#define NSTACKWORDS 0
#define FUNC_NAME vect_f32_max_exponent

.globl	FUNC_NAME
.type	FUNC_NAME,@function

#define b     a0
#define len   a1

.p2align 4
FUNC_NAME:
  { mv t3, len                          ; xm.mkmski a2, 32                      }
  { slli a2, a2, 16                     ; xm.zexti t3, 1                        }
  { srli len, len, 1                    ; xm.brff t3, .L_even_elms              }

// Handle the tail first
    xm.ldd  a3,t3, len(b)
    xm.fsexp t2, a2, a3

.L_even_elms:
  { addi len, len, -1                   ; xm.brff len, .loop_end                }

.loop:
      xm.ldd  a3,t3, len (b)
      xm.fsexp t2, t3, t3
      xm.fsexp t2, a3, a3
      xm.max3  a2, a2, t3, a3
      { addi len, len, -1               ; xm.bt  len, .loop                     }
.loop_end:

    li a0, 30
    { xm.retsp (NSTACKWORDS)*4          ; sub a0, a2, a0 }
	
	// RETURN_REG_HOLDER
.resource_const FUNC_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNC_NAME, "tail_callees"
.resource_list_empty FUNC_NAME, "callees"
.resource_list_empty FUNC_NAME, "parallel_callees"
.Ltmp0:
	.size	FUNC_NAME, .Ltmp0-FUNC_NAME

#undef NSTACKWORDS
        
#endif
