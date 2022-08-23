// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


#include "../asm_helper.h"

.text
.align 4;
.issue_mode dual

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


#define a       r0
#define b       r1
#define tmp1    r2
#define tmp2    r3
#define tmp3    r4
#define consts  r5

.L_consts: 
.L_none:   .word 0x00000000
.L_one:    .word 0x40000000
.L_sqrt_2: .word 0x5a82799a
.L_log2_e: .word 0x5c551d95
.L_ln_2:   .word 0x2c5c85fe


.cc_top FUNCTION_NAME.function,FUNCTION_NAME
FUNCTION_NAME:
  dualentsp NSTACKWORDS
  std r4, r5, sp[1]

  ldap r11, .L_consts
// Load input, reformat to have 1 bit of headroom and store in
// output.
{ mov consts, r11             ; ldw tmp1, b[0]              }
{ cls r11, tmp1               ; ldw tmp2, b[1]              }
{ sub r11, r11, 1             ;                             }
{ shl tmp1, tmp1, r11         ; sub r11, r11, 1             }
  ashr tmp1, tmp1, 1
{ sub tmp2, tmp2, r11         ; stw tmp1, a[0]              }
{                             ; stw tmp2, a[1]              }

#undef b
#define y     r1

//// Compute y = x * log2(e)
{ mov y, tmp1                 ;                             }
{ ldc tmp1, 0                 ; ldw r11, consts[3]          }
  maccs tmp1, y, r11, y /// astew: Is this correct... isn't this doing  acc = y + log2(e) * y ??
{ ldc r11, 30                 ;                             }
  lextract y, tmp1, y, r11, 32

//// Deal with fractional bit count
{ clz r11, tmp2               ;                             }
{ neg r11, tmp2               ; bf r11, .L_neg_exp          }

//// If the exponent is non-negative, then the best estimate we can give is 2^(y<<exp)
{ ldc tmp1, 30                ; ldw y, consts[1]            }
{ sub tmp2, tmp2, tmp1        ; stw y, a[0]                 }
{                             ; stw tmp2, a[1]              }
{                             ; bu .L_finish                }

.L_neg_exp:

  //// alpha = floor(y)
  ashr tmp1, y, r11

  //// output exponent = alpha - 30
{ ldc tmp2, 30                ;                             }
{ sub tmp2, tmp1, tmp2        ;                             }
{ zext y, r11                 ; stw tmp2, a[1]              }

  // Put it in Q30 format
{ ldc tmp2, 30                ;                             }
{ neg tmp2, tmp2              ;                             }
{ add tmp2, tmp2, r11         ; stw a, sp[STACK_A]          }
  ashr y, y, tmp2

  //// If y is negative, we need to add 1 to it
  ashr tmp1, y, 32
{ neg tmp1, tmp1              ;                             }
{                             ; ldw tmp1, consts[tmp1]      }
{ add y, y, tmp1              ; add consts, consts, 4       }


  //// Get 2^rho
{ ldc tmp1, 29                ; ldc tmp3, 29                }
{ shr tmp2, y, tmp1           ; zext y, tmp3                }
{ sub consts, consts, 4       ; ldw r11, consts[tmp2]       }
{ ldc tmp1, 0                 ; stw r11, sp[STACK_RHO]      }

  //// Need to compute z = beta * ln(2)

{ ldc tmp3, 30                ; ldw r11, consts[4]          }
  lmul tmp1, y, y, r11, tmp1, tmp1
  lextract y, tmp1, y, tmp3, 32

  //// Now we need to actually compute the power series
  //   on y, given that it is a Q30
  ldap r11, q30_exp_small
{ mov r0, y                   ; bla r11                     }

  //// Now we just need to multiply our rho and beta factors
{ ldc r2, 0                   ; ldw r1, sp[STACK_RHO]       }
  lmul r1, r0, r1, r0, r2, r2
  lextract r0, r1, r0, tmp3, 32

  //// That's our result.
{                             ; ldw r1, sp[STACK_A]         }
{                             ; stw r0, r1[0]               }

.L_finish:
  ldd r4, r5, sp[1]
  retsp NSTACKWORDS

.L_func_end_unpack:
.cc_bottom FUNCTION_NAME.function

.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords, NSTACKWORDS + q30_exp_small.nstackwords;
.global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME,.L_func_end_unpack - FUNCTION_NAME

#undef NSTACKWORDS
#undef FUNCTION_NAME



#endif //defined(__XS3A__)



