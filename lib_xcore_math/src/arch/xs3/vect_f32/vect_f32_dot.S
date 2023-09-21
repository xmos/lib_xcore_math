// Copyright 2022-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
    
#if defined(__XS3A__)

.text
.issue_mode  dual

/*

  float vect_f32_dot(
      const float b[],
      const float c[],
      const unsigned length);

*/

#define FUNC_NAME     vect_f32_dot
#define NSTACKWORDS   4

.globl	FUNC_NAME
.type	FUNC_NAME,@function
.cc_top FUNC_NAME.function,FUNC_NAME

.align 16
FUNC_NAME:
  dualentsp NSTACKWORDS
  std r4, r5, sp[0]
  std r6, r7, sp[1]

{ mov r3, r2                  ; zext r2, 1                  }
{ bf r2, .even                ; ldc r11, 0                  }

.odd:
  // Deal with tail first
  sub r3, r3, 1
  ldw r4, r0[r3]
  ldw r5, r1[r3]
  fmacc r11, r11, r4, r5

.even:

// 4 possibilities:
//    b[] and c[] are (both) DWORD aligned
//    c[] and c[] are (both) not DWORD aligned
//    b[] or c[] is DWORD aligned, and the other is not.
// Figure out which situation applies, because it will affect whether we can
// do load-doubles and whether the two vectors are aligned if we do.
{ shr r6, r0, 2               ; shr r7, r1, 2               }
{ zext r6, 1                  ; zext r7, 1                  }
{ shl r6, r6, 1               ; mov r2, r3                  }
{ or r6, r6, r7               ; bf r2, .done                }
{ sub r2, r2, 1               ; bru r6                      }
  bu .together
  bu .r1_odd
  bu .r0_odd

// b[] and c[] are both not DWORD aligned.
// deal with final element, and shift pointers to be DWORD aligned
.r0r1_odd:
  { sub r0, r0, 4               ; ldw r6, r0[r2]              }
  { sub r1, r1, 4               ; ldw r7, r1[r2]              }
    shr r2, r2, 1
  .r0r1_odd_loop:
      fmacc r11, r11, r6, r7
      ldd r4, r6, r0[r2]
      ldd r5, r7, r1[r2]
      fmacc r11, r11, r4, r5
    { sub r2, r2, 1               ; bt  r2, .r0r1_odd_loop      }
  .r0r1_odd_loop_done:
    bu .done

// c[] was odd and b[] even.
// Since the operands are symmetric (doesn't matter which is which), we can just
// swap pointers and pretend it was the other way around.
.r1_odd:
  { mov r0, r1                  ; mov r1, r0                  }
// b[] was odd and c[] even.
.r0_odd:
  { shr r2, r2, 1               ; ldw r6, r0[r2]              }
    sub r0, r0, 4
  .r0_odd_loop:
      ldd r5, r7, r1[r2]
      fmacc r11, r11, r6, r5
      ldd r4, r6, r0[r2]
      fmacc r11, r11, r4, r7
    { sub r2, r2, 1               ; bt  r2, .r0_odd_loop        }
  .r0_odd_loop_done:
    bu .done

  nop

.together:
    shr r2, r2, 1
  .together_loop:
      ldd r4, r6, r0[r2]
      ldd r5, r7, r1[r2]
      fmacc r11, r11, r4, r5
      fmacc r11, r11, r6, r7
    { sub r2, r2, 1               ; bt  r2, .together_loop      }

.done:
  ldd r4, r5, sp[0]
  ldd r6, r7, sp[1]
  add r0, r11, 0
  retsp NSTACKWORDS
    
	
	// RETURN_REG_HOLDER
	.cc_bottom FUNC_NAME.function
	.set	FUNC_NAME.nstackwords,NSTACKWORDS;     .globl	FUNC_NAME.nstackwords
	.set	FUNC_NAME.maxcores,1;                  .globl	FUNC_NAME.maxcores
	.set	FUNC_NAME.maxtimers,0;                 .globl	FUNC_NAME.maxtimers
	.set	FUNC_NAME.maxchanends,0;               .globl	FUNC_NAME.maxchanends
.Ltmp1:
	.size	FUNC_NAME, .Ltmp1-FUNC_NAME

#undef NSTACKWORDS


#endif