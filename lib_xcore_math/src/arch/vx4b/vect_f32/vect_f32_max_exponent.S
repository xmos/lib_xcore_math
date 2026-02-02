// Copyright 2022-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
    
#if defined(__VX4B__)

.text


/*

  exponent_t vect_f32_max_exponent(
      const float b[], 
      const unsigned length);
       
*/

#define NSTACKWORDS 4
#define FUNC_NAME vect_f32_max_exponent

.globl	FUNC_NAME
.type	FUNC_NAME,@function

#define b     a0
#define len   a1

.p2align 4
FUNC_NAME:
    xm.entsp (NSTACKWORDS)*4
    xm.stdsp  s3,s2,0

  { mv t3, len                          ; xm.mkmski a2, 32                      }
  { slli a2, a2, 16                     ; xm.zexti t3, 1                        }
  { srli len, len, 1                    ; xm.brff t3, .L_even_elms              }

// Handle the tail first
    xm.ldd  s3,s2, len(b)
    xm.fsexp t3, s3, s3
  { mv a2, s3                           ; nop                                   }

.L_even_elms:
  { addi len, len, -1                   ; xm.brff len, .loop_end                }

  .loop:
      xm.ldd  s3,s2, len (b)
      xm.fsexp t3, s2, s2
      xm.fsexp a3, s3, s3
    { xm.slt t3, s2, a2                 ; nop                                   }
    { xm.slt t3, s3, a2                 ; xm.bt t3, .not                        }
    { xm.slt t3, s3, s2                 ; mv a2, s2                             }
    .not:   
        bnez t3, .not2
        mv a2, s3
    .not2:
      { addi len, len, -1               ; xm.bt  len, .loop                     }
  .loop_end:

    xm.lddsp  s3,s2,0
    li a0, 30
    sub a0, a2, a0
    xm.retsp (NSTACKWORDS)*4
	
	// RETURN_REG_HOLDER
	.set	FUNC_NAME.nstackwords,NSTACKWORDS;  .globl	FUNC_NAME.nstackwords
	.set	FUNC_NAME.maxcores,1;              	.globl	FUNC_NAME.maxcores
	.set	FUNC_NAME.maxtimers,0;              .globl	FUNC_NAME.maxtimers
	.set	FUNC_NAME.maxchanends,0;            .globl	FUNC_NAME.maxchanends
.Ltmp0:
	.size	FUNC_NAME, .Ltmp0-FUNC_NAME

#undef NSTACKWORDS
        
#endif
