// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

.text
.align 4; /* Translation error on this line: unexpected token at position 8. */ 

/*  
void float_s32_exp(
    float_s32_t* a,
    const float_s32_t b);

    OR
    
float_s32_t float_s32_exp(
    const float_s32_t b);

  (The ABI means these are equivalent)
*/
#define FUNCTION_NAME   float_s32_exp
#define NSTACKWORDS     (8)


#define STACK_RHO       (7)
#define STACK_A         (6)


#define a       x10
#define b       x11
#define tmp1    x12
#define tmp2    x13
#define tmp3    x18
#define consts  x19

.L_consts: 
.L_none:   .word 0x00000000 /* Translation error on this line: unexpected token at position 11. */ 
.L_one:    .word 0x40000000 /* Translation error on this line: unexpected token at position 11. */ 
.L_sqrt_2: .word 0x5a82799a /* Translation error on this line: unexpected token at position 11. */ 
.L_log2_e: .word 0x5c551d95 /* Translation error on this line: unexpected token at position 11. */ 
.L_ln_2:   .word 0x2c5c85fe /* Translation error on this line: unexpected token at position 11. */ 


FUNCTION_NAME:
  xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
  xm.stdsp  s3,s2,8

lui t3, %hi(.L_consts)
  addi t3,t3, %lo(.L_consts)
// Load input, reformat to have 1 bit of headroom and store in
// output.
{ mv consts, t3             ; lw tmp1,0              ( b)}
{ xm.cls t3, tmp1               ; nop}
{nop;xm.ldwi tmp2, 4(b)              }
{ addi t3, t3, -1             ; nop                             }
{ xm.shl tmp1, tmp1, t3         ; addi t3, t3, -1             }
  srai tmp1, tmp1, 1
{ sub tmp2, tmp2, t3         ; sw tmp1,0              ( a)}
{ nop                             ; sw tmp2,4              ( a)}

#undef b
#define y     x11

//// Compute y = x * log2(e)
{ mv y, tmp1                 ; nop                             }
{ li tmp1, 0                 ; lw t3,12          ( consts)}
  xm.maccs tmp1, y, t3, y /// astew: Is this correct... isn't this doing  acc = y + log2(e) * y ??
{ li t3, 30                 ; nop                             }
  xm.lextract y, tmp1, y, t3, 32

//// Deal with fractional bit count
{ xm.clz tmp3, tmp2               ;  nop}
{nop; xm.neg t3, tmp2                            }
{  nop              ; xm.brff tmp3, .L_neg_exp          }

//// If the exponent is non-negative, then the best estimate we can give is 2^(y<<exp)
{ li tmp1, 30                ; lw y,4            ( consts)}
{ sub tmp2, tmp2, tmp1        ; sw y,0                 ( a)}
{ nop                             ; sw tmp2,4              ( a)}
{ nop                             ; xm.bu .L_finish                }

.L_neg_exp:

  //// alpha = floor(y)
  sra tmp1, y, t3

  //// output exponent = alpha - 30
{ li tmp2, 30                ; nop                             }
{ sub tmp2, tmp1, tmp2        ; xm.zext y, t3                             }
{ nop                 ; xm.stwi tmp2, 4(a)              }

  // Put it in Q30 format
{ li tmp2, 30                ; nop                             }
{ xm.neg tmp2, tmp2              ; nop                             }
{ add tmp2, tmp2, t3         ; sw a, (STACK_A)*4          (sp)}
  {xm.shr y, y, tmp2 ; nop} /* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in ashri y, y, tmp2\nMessage: Shift amount may need adjustment but I don't know its value" */

  //// If y is negative, we need to add 1 to it
  srai tmp1, y, 31
{ xm.neg tmp1, tmp1              ; nop                             }
xm.ldw tmp1, tmp1      ( consts)
{ add y, y, tmp1              ; addi consts, consts, 4       }


  //// Get 2^rho
{ li tmp1, 29                ; li tmp3, 29                }
{ xm.shr tmp2, y, tmp1           ; xm.zext y, tmp3                }/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in  shri tmp2, y, tmp1           \nMessage: The shift amount is not 32" */
{ addi consts, consts, -4       ; xm.ldw t3,tmp2       ( consts)}
{ li tmp1, 0                 ; sw t3, (STACK_RHO)*4      (sp)}

  //// Need to compute z = beta * ln(2)

{ li tmp3, 30                ; lw t3,16          ( consts)}
  xm.lmul tmp1, y, y, t3, tmp1, tmp1
  xm.lextract y, tmp1, y, tmp3, 32

  //// Now we need to actually compute the power series
  //   on y, given that it is a Q30
lui t3, %hi(q30_exp_small)
  addi t3,t3, %lo(q30_exp_small)
{ mv a0, y                   ; jalr t3                     }

  //// Now we just need to multiply our rho and beta factors
{ li a2, 0                   ; lw a1, (STACK_RHO)*4       (sp)}
  xm.lmul a1, a0, a1, a0, a2, a2
  xm.lextract a0, a1, a0, tmp3, 32

  //// That's our result.
{ nop                             ; lw a1, (STACK_A)*4         (sp)}
{ nop                             ; sw a0,0               ( a1)}

.L_finish:
  xm.lddsp  s3,s2,8
  xm.retsp (NSTACKWORDS)*4/* Multiple XAT warnings: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in retsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate", 'RETSP operand may need scaling' */

.L_func_end_unpack:

.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords, NSTACKWORDS + q30_exp_small.nstackwords; /* Translation error on this line: unexpected token at position 71. */ 
.global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends /* Translation error on this line: unexpected token at position 32. */ 
.size FUNCTION_NAME,.L_func_end_unpack - FUNCTION_NAME

#undef NSTACKWORDS
#undef FUNCTION_NAME



#endif //defined(__VX4B__)



