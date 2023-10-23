// Copyright 2022-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
    
#if defined(__XS3A__)

.text
.issue_mode  dual


/*

  exponent_t vect_f32_max_exponent(
      const float b[], 
      const unsigned length);
       
*/

#define NSTACKWORDS 2
#define FUNC_NAME vect_f32_max_exponent

.globl	FUNC_NAME
.type	FUNC_NAME,@function
.cc_top FUNC_NAME.function,FUNC_NAME

#define b     r0
#define len   r1

.align 16
FUNC_NAME:
    dualentsp NSTACKWORDS
    std r4, r5, sp[0]

  { mov r11, len                ; mkmsk r2, 32                }
  { shl r2, r2, 16              ; zext r11, 1                 }
  { shr len, len, 1             ; bf r11, .L_even_elms        }

// Handle the tail first
    ldd r4, r5, b[len]
    fsexp r11, r5, r5
  { mov r2, r5                  ;                             }

.L_even_elms:
  { sub len, len, 1             ; bf len, .loop_end           }

  .loop:
      ldd r4, r5, b[len] 
      fsexp r11, r4, r4
      fsexp r3, r5, r5
    { lss r11, r4, r2             ;                             }
    { lss r11, r5, r2             ; bt r11, .not                }
    { lss r11, r5, r4             ; mov r2, r4                  }
    .not:   
        bt r11, .not2
        mov r2, r5
    .not2:
      { sub len, len, 1             ; bt  len, .loop              }
  .loop_end:

    ldd r4, r5, sp[0]
    ldc r0, 30
    sub r0, r2, r0
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