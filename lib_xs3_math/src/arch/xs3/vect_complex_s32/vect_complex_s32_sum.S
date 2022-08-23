// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)

#include "../asm_helper.h"

/*  


void vect_complex_s32_sum(
    const complex_s64_t* res,
    const complex_s32_t* b,
    const unsigned length,
    const right_shift_t b_shr);


*/

.text
.issue_mode dual
.align 4

#define NSTACKVECS      (2)
#define NSTACKWORDS     (6+(8*NSTACKVECS))

#define b           r0
#define b_shr       r1
#define length      r2
#define _32         r3
#define tmp         r4
#define tail_bytes  r5

#define STACK_VEC_ZEROS     (NSTACKWORDS- 8)
#define STACK_VEC_TMP       (NSTACKWORDS-16)

#define STACK_RES   (1)

#define FUNCTION_NAME vect_complex_s32_sum
    


.cc_top FUNCTION_NAME.function,FUNCTION_NAME
FUNCTION_NAME:

        dualentsp NSTACKWORDS
        std r4, r5, sp[1]
        std r6, r7, sp[2]

    {   mov b, r1                               ;   stw r0, sp[STACK_RES]                   }


    {   mov b_shr, r3                           ;   shl tail_bytes, length, 3               }
    {                                           ;   zext tail_bytes, 5                      }

    {   ldaw r11, sp[STACK_VEC_ZEROS]           ;   vclrdr                                  }
    {   ldc r11, 0                              ;   vstd r11[0]                             }
    {   lss tmp, b_shr, r11                     ;   vsetc r11                               }
    {   ldaw tmp, sp[STACK_VEC_TMP]             ;   ecallt tmp /*Cannot be negative shift*/ }
        ldaw r11, cp[vpu_vec_0x40000000]
        vlashr r11[0], b_shr
    {   ldc r11, 0                              ;   vstr tmp[0]                             }
    {   shr length, length, 2                   ;   vldc tmp[0]                             }
    {   ldc _32, 32                             ;   vsetc r11                               }
    {                                           ;   vclrdr                                  }

    {                                           ;   bf length, .L_loop_bot                  }

    .L_loop_top:

        {   sub length, length, 1               ;   vlmacc b[0]                             }
        {   add b, b, _32                       ;   bt length, .L_loop_top                  }

    .L_loop_bot:

    {   ldaw r11, sp[STACK_VEC_ZEROS]           ;   bf tail_bytes, .L_get_res               }
    {   sub r11, r11, tail_bytes                ;                                           }
    {                                           ;   vldc r11[0]                             }
    {   ldaw r11, sp[STACK_VEC_ZEROS]           ;   vlmacc b[0]                             }
        

/*  We've got 8 40-bit accumulators. Lower 32 bits are in vR, upper 8 in vD.
    vD does appear to sign-extend the values up to 64 bits.

    (vD:vR)[k] ==  ((int32_t)vD[k])*(2^32) + ((uint32_t)vR[k]) */

#define real_hi     r0
#define real_lo     r1
#define imag_hi     r2
#define imag_lo     r3
#define num         r5
#define tmp_re      r6
#define tmp_im      r7

// astew [2020-10-16]: There's probably a faster way to do this. See the VPU-based solution I found for vect_s32_sum for
//                     non-complex values

.L_get_res:
    {   ldc real_hi, 0                          ;   ldc imag_hi, 0                          }
    {   ldc num, 1                              ;   vstr tmp[0]                             }
        ldd imag_lo, real_lo, tmp[0]
        ldd tmp_im, tmp_re, tmp[1]
        maccu real_hi, real_lo, num, tmp_re
        maccu imag_hi, imag_lo, num, tmp_im
        ldd tmp_im, tmp_re, tmp[2]
        maccu real_hi, real_lo, num, tmp_re
        maccu imag_hi, imag_lo, num, tmp_im
        ldd tmp_im, tmp_re, tmp[3]
        maccu real_hi, real_lo, num, tmp_re
        maccu imag_hi, imag_lo, num, tmp_im
    {                                           ;   vfttf                                   }
    {   ldc num, 2                              ;   vstd r11[0]                             }
        ldd tmp_im, tmp_re, r11[0]
    {   add real_hi, real_hi, tmp_re            ;   add imag_hi, imag_hi, tmp_im            }

    // astew [2021-09-28]: ... what was the purpose of these next 4 instructions..?
    //                     maybe at the time I was thinking the lower word should be
    //                     interpreted as signed?
    // {   shr tmp_re, real_lo, 1                  ;   zext real_lo, 1                         }
    // {   shr tmp_im, imag_lo, 1                  ;   zext imag_lo, 1                         }
    //     maccs real_hi, real_lo, num, tmp_re
    //     maccs imag_hi, imag_lo, num, tmp_im
    {                                           ;   ldw tmp, sp[STACK_RES]                  }
        std real_hi, real_lo, tmp[0]
        std imag_hi, imag_lo, tmp[1]

    

.L_done:
        ldd r4, r5, sp[1]
        ldd r6, r7, sp[2]
        retsp NSTACKWORDS

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



