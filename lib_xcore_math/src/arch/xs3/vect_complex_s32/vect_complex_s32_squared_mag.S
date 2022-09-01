// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)

#include "../asm_helper.h"

/*  

headroom_t vect_complex_s32_squared_mag(
    int32_t a[],
    const complex_s32_t* b,
    const unsigned length,
    const right_shift_t b_shr);

*/

.text
.issue_mode dual
.align 4


#define NSTACKWORDS     (8+8)

#define a               r0 
#define b               r1 
#define length          r2
#define b_shr           r3
#define vec_count       r4
#define _16             r5
#define vec_tmp         r6
#define tail_mask       r7
#define vec_ones        r8
#define tmpA            r9
#define tmpB            r10


#define STACK_VEC_TMP   (NSTACKWORDS-8)

#define FUNCTION_NAME vect_complex_s32_squared_mag
    


.cc_top FUNCTION_NAME.function,FUNCTION_NAME
FUNCTION_NAME:
    dualentsp NSTACKWORDS
    std r4, r5, sp[1]
    std r6, r7, sp[2]
    std r8, r9, sp[3]
    {                                           ;   stw r10, sp[1]                          }

    {   ldc _16, 16                             ;   shr vec_count, length, 2                }
    {   ldc r11, 0                              ;   shl tail_mask, length, 3                }
    {   zext tail_mask, 5                       ;   vsetc r11                               }

        ldaw r11, cp[vpu_vec_complex_ones]                                                  
    {   ldaw vec_tmp, sp[STACK_VEC_TMP]         ;   mov vec_ones, r11                       }
    {   mkmsk r11, 32                           ;   bf vec_count, .L_loop_bot               }

    .L_loop_top:
            vlashr b[0], b_shr
            vstrpv vec_tmp[0], r11
        {   add b, b, _16                           ;   vldc vec_tmp[0]                         }
        {   add b, b, _16                           ;   vldd vec_tmp[0]                         }
        {   sub vec_count, vec_count, 1             ;   vcmcr                                   }
        { /* zero out imag part so that we don't */ ;   vlmul vec_ones[0]                       }
        { /* clobber the headroom counter        */ ;   vstr vec_tmp[0]                         }
        {                                           ;   ldw tmpA, vec_tmp[0]                    }
        {                                           ;   ldw tmpB, vec_tmp[2]                    }
            std tmpB, tmpA, a[0]
        {                                           ;   ldw tmpA, vec_tmp[4]                    }
        {                                           ;   ldw tmpB, vec_tmp[6]                    }
            std tmpB, tmpA, a[1]
        {   add a, a, _16                           ;   bt vec_count, .L_loop_top               }

    .L_loop_bot:
    {   zext length, 2                          ;   bf tail_mask, .L_done                   }
    {   shl length, length,1                    ;   vclrdr                                  }
    {   mkmsk tail_mask, tail_mask              ;   vstd vec_tmp[0]                         }
        vlashr b[0], b_shr
        vstrpv vec_tmp[0], tail_mask
    {   ldc tmpA, 6                             ;   vldd vec_tmp[0]                         }
    {   sub length, tmpA, length                ;   vldc vec_tmp[0]                         }
    {                                           ;   vcmcr                                   }
    {                                           ;   vlmul vec_ones[0]                       }
    {                                           ;   vstr vec_tmp[0]                         }
    {                                           ;   bru length                              }
    {                                           ;   ldw r11, vec_tmp[4]                     }
    {                                           ;   stw r11, a[2]                           }
    {                                           ;   ldw r11, vec_tmp[2]                     }
    {                                           ;   stw r11, a[1]                           }
    {                                           ;   ldw r11, vec_tmp[0]                     }
    {                                           ;   stw r11, a[0]                           }
.L_done:
        ldd r4, r5, sp[1]
        ldd r6, r7, sp[2]
        ldd r8, r9, sp[3]

    {   ldc r0, 31                              ;   vgetc r11                               }
    {   zext r11, 5                             ;   ldw r10, sp[1]                          }
    {   sub r0, r0, r11                         ;   retsp NSTACKWORDS                       }


.L_func_end:
.cc_bottom FUNCTION_NAME.function

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME

#undef FUNCTION_NAME



#endif //defined(__XS3A__)



