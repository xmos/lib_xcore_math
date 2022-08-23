// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


#include "../asm_helper.h"

/*  

headroom_t vect_s32_inverse(
    int32_t a[],
    const int32_t b[],
    const unsigned length,
    const unsigned scale);

*/


#define NSTACKVECTS     (1)
#define NSTACKWORDS     (10+8*(NSTACKVECTS))

#define FUNCTION_NAME   vect_s32_inverse


#define STACK_VEC_TMP       (NSTACKWORDS-8)


#define a               r0
#define b               r1
#define length          r2
#define scale           r3
#define div_hi          r3
#define div_lo          r4
#define v_mask          r5
#define _32             r6
#define val1            r7
#define val2            r8
#define vec_tmp         r9

.text
.issue_mode dual
.align 4

.cc_top FUNCTION_NAME.function,FUNCTION_NAME

FUNCTION_NAME:
    dualentsp NSTACKWORDS
    std r4, r5, sp[1]
    std r6, r7, sp[2]
    std r8, r9, sp[3]

{   ldc r11, 0                              ;   stw r10, sp[1]                          }
{   shl length, length, 2                   ;   vsetc r11                               }

{   ldc _32, 32                             ;                                           }
{   sub val2, scale, _32                    ;   ldc val1, 1                             }
{   shl div_hi, val1, val2                  ;   shl div_lo, val1, scale                 }

{   ldaw vec_tmp, sp[STACK_VEC_TMP]         ;   bf length, .L_loop_bot                  }

.align 16
.L_loop_top:
    // The masked out elements will  
    {   mkmsk v_mask, length                    ;   vstd vec_tmp[0]                         }
        vlashr b[0], v_mask
    {   sub length, length, _32                 ;   vsign                                   }
    {                                           ;   vlmul b[0]                              }
        vstrpv vec_tmp[0], v_mask
        vlashr b[0], v_mask
    {   add b, b, _32                           ;   vsign                                   }


#if 1

    {   mov val2, v_mask                        ;                                           }
    .L_div_loop_top:

        {   shr val2, val2, 4                       ;   ldw val1, vec_tmp[0]                    }
            ldivu val1, r10, div_hi, div_lo, val1
        {   add vec_tmp, vec_tmp, 4                 ;   stw val1, vec_tmp[0]                    }
        {                                           ;   bt val2, .L_div_loop_top                }
    .L_div_loop_bot:
    {   ldaw vec_tmp, sp[STACK_VEC_TMP]         ;                                           }

#else 


        ldd val1, val2, sp[((STACK_VEC_TMP) / 2) + 0]
        ldivu val1, r10, div_hi, _0, val1
        ldivu val2, r10, div_hi, _0, val2
        std val1, val2, sp[((STACK_VEC_TMP) / 2) + 0]

        ldd val1, val2, sp[((STACK_VEC_TMP) / 2) + 1]
        ldivu val1, r10, div_hi, _0, val1
        ldivu val2, r10, div_hi, _0, val2
        std val1, val2, sp[((STACK_VEC_TMP) / 2) + 1]

        ldd val1, val2, sp[((STACK_VEC_TMP) / 2) + 2]
        ldivu val1, r10, div_hi, _0, val1
        ldivu val2, r10, div_hi, _0, val2
        std val1, val2, sp[((STACK_VEC_TMP) / 2) + 2]

        ldd val1, val2, sp[((STACK_VEC_TMP) / 2) + 3]
        ldivu val1, r10, div_hi, _0, val1
        ldivu val2, r10, div_hi, _0, val2
        std val1, val2, sp[((STACK_VEC_TMP) / 2) + 3]

#endif

    {   ldc val1, 1                             ;   vlmul vec_tmp[0]                        }
        vstrpv a[0], v_mask
    {   lss val1, length, val1                  ;   vstr vec_tmp[0]                         } // Headroom update
    {   add a, a, _32                           ;   bf val1, .L_loop_top                    }

.L_loop_bot:

.L_finish:
    ldd r4, r5, sp[1]
    ldd r6, r7, sp[2]
    ldd r8, r9, sp[3]
{   ldc r0, 31                              ;   vgetc r11                           }
{   zext r11, 5                             ;   ldw r10, sp[1]                      }
{   sub r0, r0, r11                         ;   retsp NSTACKWORDS                   } 


.L_func_end:
.cc_bottom FUNCTION_NAME.function


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME


#endif //defined(__XS3A__)



