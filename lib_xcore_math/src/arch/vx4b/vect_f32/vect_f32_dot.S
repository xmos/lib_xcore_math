// Copyright 2022-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
    
#if defined(__VX4B__)

.text

/*

  float vect_f32_dot(
      const float b[],
      const float c[],
      const unsigned length);

*/

#define FUNC_NAME     vect_f32_dot
#define NSTACKWORDS   8

.globl	FUNC_NAME
.type	FUNC_NAME,@function

.p2align 4
FUNC_NAME:
  xm.entsp (NSTACKWORDS)*4
  xm.stdsp  s3,s2,0
  xm.stdsp  s5,s4,8

{ mv a3, a2                             ; xm.zexti a2, 1                        }
{ xm.brff a2, .even                     ; li t3, 0                              }

.odd:
  // Deal with tail first
  addi a3, a3, -1
  xm.ldw s2, a3(a0)
  xm.ldw s3, a3(a1)
  xm.fmacc t3, t3, s2, s3

.even:

// 4 possibilities:
//    b[] and c[] are (both) DWORD aligned
//    c[] and c[] are (both) not DWORD aligned
//    b[] or c[] is DWORD aligned, and the other is not.
// Figure out which situation applies, because it will affect whether we can
// do load-doubles and whether the two vectors are aligned if we do.
{ srli s4, a0, 2                        ; srli s5, a1, 2                        }
{ xm.zexti s4, 1                        ; xm.zexti s5, 1                        }
{ slli s4, s4, 1                        ; mv a2, a3                             }
{ or s4, s4, s5                         ; xm.brff a2, .done                     }
{ addi a2, a2, -1                       ; xm.bru s4                             }
  tail .together
  tail .r1_odd
  tail .r0_odd

// b[] and c[] are both not DWORD aligned.
// deal with final element, and shift pointers to be DWORD aligned
.r0r1_odd:
  { addi a0, a0, -4                     ; xm.ldw s4, a2              (a0)       }
  { addi a1, a1, -4                     ; xm.ldw s5, a2              (a1)       }
    srli a2, a2, 1
  .r0r1_odd_loop:
      xm.fmacc t3, t3, s4, s5
      xm.ldd  s4,s2, a2(a0)
      xm.ldd  s5,s3, a2(a1)
      xm.fmacc t3, t3, s2, s3
    { addi a2, a2, -1                   ; xm.bt  a2, .r0r1_odd_loop             }
  .r0r1_odd_loop_done:
    tail .done

// c[] was odd and b[] even.
// Since the operands are symmetric (doesn't matter which is which), we can just
// swap pointers and pretend it was the other way around.
.r1_odd:
  { mv a0, a1                           ; mv a1, a0                             }
// b[] was odd and c[] even.
.r0_odd:
  { srli a2, a2, 1                      ; xm.ldw s4, a2              (a0)       }
    addi a0, a0, -4
  .r0_odd_loop:
      xm.ldd  s5,s3, a2(a1)
      xm.fmacc t3, t3, s4, s3
      xm.ldd  s4,s2, a2(a0)
      xm.fmacc t3, t3, s2, s5
    { addi a2, a2, -1                   ; xm.bt  a2, .r0_odd_loop               }
  .r0_odd_loop_done:
    tail .done

  nop

.together:
    srli a2, a2, 1
  .together_loop:
      xm.ldd  s4,s2, a2(a0)
      xm.ldd  s5,s3, a2(a1)
      xm.fmacc t3, t3, s2, s3
      xm.fmacc t3, t3, s4, s5
    { addi a2, a2, -1                   ; xm.bt  a2, .together_loop             }

.done:
  xm.lddsp  s3,s2,0
  xm.lddsp  s5,s4,8
  addi a0, t3, 0
  xm.retsp (NSTACKWORDS)*4
    
	
	// RETURN_REG_HOLDER
	.set	FUNC_NAME.nstackwords,NSTACKWORDS;     .globl	FUNC_NAME.nstackwords
	.set	FUNC_NAME.maxcores,1;                  .globl	FUNC_NAME.maxcores
	.set	FUNC_NAME.maxtimers,0;                 .globl	FUNC_NAME.maxtimers
	.set	FUNC_NAME.maxchanends,0;               .globl	FUNC_NAME.maxchanends
.Ltmp1:
	.size	FUNC_NAME, .Ltmp1-FUNC_NAME

#undef NSTACKWORDS


#endif
