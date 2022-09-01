// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)

#include "../asm_helper.h"

/*  

headroom_t vect_complex_s32_scale(
    complex_s32_t* a,
    const complex_s32_t* b,
    const int32_t c_real,
    const int32_t c_imag,
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);

*/

.text
.issue_mode dual
.align 4


#define NSTACKWORDS     (6+8)

#define a           r0 
#define b           r1 
#define c_real      r2
#define c_imag      r3
#define length      r4
#define b_shr       r5

#define _32         r6
#define tmp_vec     r7

#define STACK_LENGTH    (NSTACKWORDS+1)
#define STACK_B_SHR     (NSTACKWORDS+2)
#define STACK_C_SHR     (NSTACKWORDS+3)

#define STACK_VEC_TMP   (NSTACKWORDS-8)

#define FUNCTION_NAME vect_complex_s32_scale
    
    
.cc_top FUNCTION_NAME.function,FUNCTION_NAME
FUNCTION_NAME:
    dualentsp NSTACKWORDS
    std r4, r5, sp[1]
    std r6, r7, sp[2]
    
    {   ldaw tmp_vec, sp[STACK_VEC_TMP]         ;   ldc r11, 0                              }
    {   ldc _32, 32                             ;   vsetc r11                               }
    
        std c_imag, c_real, tmp_vec[0]
        std c_imag, c_real, tmp_vec[1]
        std c_imag, c_real, tmp_vec[2]
        std c_imag, c_real, tmp_vec[3]

    {   mkmsk c_imag, 32                        ;   ldw c_real, sp[STACK_C_SHR]             }
        vlashr tmp_vec[0], c_real
        vstrpv tmp_vec[0], c_imag

    {                                           ;   vldc tmp_vec[0]                         }

    #undef c_real
    #undef c_imag
    #define vec_count   r2
    #define tail_bytes  r3

    {                                           ;   ldw length, sp[STACK_LENGTH]            }
    {   shl tail_bytes, length, 3               ;   ldw b_shr, sp[STACK_B_SHR]              }
    {   zext tail_bytes, 5                      ;   shr vec_count, length, 2                }

    #undef length

    {   mkmsk tail_bytes, tail_bytes            ;   mkmsk r11, 32                           }
    {                                           ;   bf vec_count, .L_loop_bot               }
    

    .L_loop_top:
            vlashr b[0], b_shr
            vstrpv tmp_vec[0], r11
        {   sub vec_count, vec_count, 1             ;   vldd tmp_vec[0]                         }
        {   add b, b, _32                           ;   vcmr                                    }
        {                                           ;   vcmi                                    }
        {   add a, a, _32                           ;   vstr a[0]                               }
        {                                           ;   bt vec_count, .L_loop_top               }

.L_loop_bot:

    {                                           ;   bf tail_bytes, .L_done                  }

    {;entsp 0}

.issue_mode single

    vclrdr
    vstd tmp_vec[0]
    vlashr b[0], b_shr
    vstrpv tmp_vec[0], tail_bytes
    vldd tmp_vec[0]
    vcmr
    vcmi
    vstrpv tmp_vec[0], tail_bytes
    vstrpv a[0], tail_bytes
    vldd tmp_vec[0]
    vstd tmp_vec[0]
    dualentsp 0

.issue_mode dual
.align 4


    // {                                           ;   bf tail_bytes, .L_done                  }
    // {                                           ;   vclrdr                                  }
    // {                                           ;   vstd tmp_vec[0]                         }
    //     vlashr b[0], b_shr
    //     vstrpv tmp_vec[0], tail_bytes   
    // {                                           ;   vldd tmp_vec[0]                         }
    // {                                           ;   vcmr                                    }
    // {                                           ;   vcmi                                    }
    //     vstrpv tmp_vec[0], tail_bytes
    //     vstrpv a[0], tail_bytes 
    // {                                           ;   vldd tmp_vec[0]                         }
    // {                                           ;   vstd tmp_vec[0]                         }

.align 4
.L_done:
        ldd r4, r5, sp[1]
        ldd r6, r7, sp[2]

    {   ldc r0, 31                              ;   vgetc r11                               }
    {   zext r11, 5                             ;                                           }
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



