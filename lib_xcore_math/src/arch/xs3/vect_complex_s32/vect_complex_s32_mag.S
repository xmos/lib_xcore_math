// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)

#include "../asm_helper.h"

/*  

headroom_t vect_complex_s32_mag(
    int32_t a[],
    const complex_s32_t* b,
    const unsigned length,
    const right_shift_t b_shr,
    const complex_s32_t* rot_table
    const unsigned table_rows);

*/

.text
.issue_mode dual
.align 4

#define NSTACKVECS      (2)
#define NSTACKWORDS     (8+(8*NSTACKVECS))

#define STACK_ROT_TABLE     (NSTACKWORDS+1)
#define STACK_TABLE_ROWS    (NSTACKWORDS+2)

#define STACK_VEC_TMP   (NSTACKWORDS-8)
#define STACK_VEC_TMP2  (NSTACKWORDS-16)

#define FUNCTION_NAME vect_complex_s32_mag

#define Q(R)    R

#define a           r0 
#define b           r1 
#define length      r2
#define b_shr       r3
#define _32         r4
#define vec_tmp     r5
#define mask32      r6
#define tmp         r7
#define iter        r8
#define tail_bytes  r9


.cc_top FUNCTION_NAME.function,FUNCTION_NAME
FUNCTION_NAME:
    dualentsp NSTACKWORDS
    std r4, r5, sp[1]
    std r6, r7, sp[2]
    std r8, r9, sp[3]
    {   ldc r10, 0                              ;   stw r10, sp[1]                          }

    {   ldc _32, 32                             ;   ldc r11, 0                              }
    {   ldaw vec_tmp, sp[STACK_VEC_TMP]         ;   vsetc r11                               }
    
    {   shr length, length, 2                   ;   mov tail_bytes, length                  }
    {   mkmsk mask32, 32                        ;   zext tail_bytes, 2                      }
    {   shl tail_bytes, tail_bytes, 2           ;   bf length, .L_outer_loop_bot            }

    .L_outer_loop_top:
        //     vlashr b[0], b_shr
        //     vstrpv vec_tmp[0], mask32
        // {   add b, b, _32                           ;   vsign                                   }
        // {                                           ;   vlmul vec_tmp[0]                        }
        //     vstrpv vec_tmp[0], mask32
        // {                                           ;   ldw r11, sp[STACK_ROT_TABLE]            }
        // {                                           ;   ldw iter, sp[STACK_TABLE_ROWS]          }        

            vlashr b[0], b_shr
        {                                           ;   vsign                                   }
        {                                           ;   ldw r11, sp[STACK_ROT_TABLE]            }
            vstrpv vec_tmp[0], mask32
            vlashr b[0], b_shr
        {   add b, b, _32                           ;   vlmul vec_tmp[0]                        }
        {                                           ;   ldw iter, sp[STACK_TABLE_ROWS]          }
            vstrpv vec_tmp[0], mask32

        .L_inner_loop_top:
            {   sub iter, iter, 1                       ;   vldd vec_tmp[0]                         }
            {   add r11, r11, _32                       ;   vldc r11[0]                             }
            {                                           ;   vcmr                                    }
            {                                           ;   vcmi                                    }
                vstrpv vec_tmp[0], mask32
            {                                           ;   vsign                                   }
            {                                           ;   vlmul vec_tmp[0]                        }
                vstrpv vec_tmp[0], mask32
            {                                           ;   bt iter, .L_inner_loop_top              }
        
        {                                           ;   vstr vec_tmp[0]                         }
        {   sub length, length, 1                   ;   ldw r11, vec_tmp[0]                     }
        {                                           ;   stw r11, a[0]                           }
        {                                           ;   ldw r11, vec_tmp[2]                     }
        {                                           ;   stw r11, a[1]                           }
        {   add a, a, 8                             ;   ldw r11, vec_tmp[4]                     }
        {                                           ;   stw r11, a[0]                           }
        {                                           ;   ldw r11, vec_tmp[6]                     }
        {   add a, a, 8                             ;   stw r11, a[1]                           }
        {                                           ;   bt length, .L_outer_loop_top            }

    .L_outer_loop_bot:  

    {   mkmsk tail_bytes, tail_bytes            ;   bf tail_bytes, .L_done                  }
        vlashr b[0], b_shr
    {                                           ;   ldw r11, sp[STACK_ROT_TABLE]            }
    {                                           ;   vsign                                   }
        vstrpv vec_tmp[0], mask32
        vlashr b[0], b_shr
    {                                           ;   vlmul vec_tmp[0]                        }
    {                                           ;   ldw iter, sp[STACK_TABLE_ROWS]          }
        vstrpv vec_tmp[0], mask32
              
    .L_inner_loop2_top:
        {   sub iter, iter, 1                       ;   vldd vec_tmp[0]                         }
        {   add r11, r11, _32                       ;   vldc r11[0]                             }
        {                                           ;   vcmr                                    }
        {                                           ;   vcmi                                    }
            vstrpv vec_tmp[0], mask32
        {                                           ;   vsign                                   }
        {                                           ;   vlmul vec_tmp[0]                        }
            vstrpv vec_tmp[0], mask32
        {                                           ;   bt iter, .L_inner_loop2_top             }
        
    {                                           ;   ldw r11, vec_tmp[0]                     }
    {                                           ;   stw r11, vec_tmp[0]                     }
    {                                           ;   ldw r11, vec_tmp[2]                     }
    {                                           ;   stw r11, vec_tmp[1]                     }
    {                                           ;   ldw r11, vec_tmp[4]                     }
    {                                           ;   stw r11, vec_tmp[2]                     }
    {                                           ;   ldw r11, vec_tmp[6]                     }
    {   ldaw r11, sp[STACK_VEC_TMP]             ;   stw r11, vec_tmp[3]                     }
    {                                           ;   vclrdr                                  }
    {                                           ;   vldr r11[0]                             }
    {                                           ;   vstd r11[0]                             }
        vstrpv r11[0], tail_bytes
        vstrpv a[0], tail_bytes
    {                                           ;   vldd r11[0]                             }
    {                                           ;   vstd r11[0]                             }

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



