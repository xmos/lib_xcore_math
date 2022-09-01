// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)

#include "../asm_helper.h"

/*  

headroom_t vect_complex_s16_mag(
    int16_t a[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const unsigned length,
    const right_shift_t b_shr,
    const int16_t* rot_table,
    const unsigned table_rows)

*/

.text
.issue_mode dual
.align 4

#define NSTACKVECS      (5)
#define NSTACKWORDS     (8+(8*NSTACKVECS))

#define STACK_B_SHR         (NSTACKWORDS+1)
#define STACK_ROT_TABLE     (NSTACKWORDS+2)
#define STACK_TABLE_ROWS    (NSTACKWORDS+3)

#define STACK_VEC_TMP_IMAG  (NSTACKWORDS-8)
#define STACK_VEC_TMP_REAL  (NSTACKWORDS-16)
#define STACK_VEC_TMP2      (NSTACKWORDS-24)
#define STACK_VEC_NEG_ONES  (NSTACKWORDS-40)
#define STACK_VEC_SAT       (NSTACKWORDS-32)

#define FUNCTION_NAME vect_complex_s16_mag

#define Q(R)    R

#define a           r0 
#define b_real      r1 
#define b_imag      r2
#define length      r3
#define b_shr       r4
#define _32         r5
#define vec_neg_one r6
#define mask32      r7
#define tail_bytes  r8
#define iter        r9
#define table       r10

.cc_top FUNCTION_NAME.function,FUNCTION_NAME
FUNCTION_NAME:
    dualentsp NSTACKWORDS
    std r4, r5, sp[1]
    std r6, r7, sp[2]
    std r8, r9, sp[3]

    {   ldc _32, 32                             ;   stw r10, sp[1]                          }

    {   shl r11, _32, 3                         ;   mov tail_bytes, length                  }
    {   shr length, length, 4                   ;   vsetc r11                               }
    {                                           ;   ldc r11, 15                             }

    {   shl r10, r11, 16                        ;   zext tail_bytes, 4                      }
    {   or r11, r11, r10                        ;   mkmsk mask32, 32                        }
    std r11, r11, sp[STACK_VEC_SAT/2 + 0]
    std r11, r11, sp[STACK_VEC_SAT/2 + 1]
    std r11, r11, sp[STACK_VEC_SAT/2 + 2]
    std r11, r11, sp[STACK_VEC_SAT/2 + 3]
    ldc r10, 0xC000
    {   shl r9, r10, 16                         ;   shl tail_bytes, tail_bytes, 1           }
    {   or r10, r10, r9                         ;   ldaw vec_neg_one, sp[STACK_VEC_NEG_ONES]}
    std r10, r10, vec_neg_one[0]
    std r10, r10, vec_neg_one[1]
    std r10, r10, vec_neg_one[2]
    std r10, r10, vec_neg_one[3]

    {   mkmsk tail_bytes, tail_bytes            ;   ldw b_shr, sp[STACK_B_SHR]              }
    {   ldaw r11, sp[STACK_VEC_TMP_REAL]        ;   bf length, .L_outer_loop_bot            }

    .L_outer_loop_top:
            vlashr b_real[0], b_shr
            vstrpv r11[0], mask32
        {   add b_real, b_real, _32                 ;   vsign                                   }
        {   ldaw Q(iter), sp[STACK_VEC_TMP_IMAG]    ;   vlmul r11[0]                            }
            vstrpv r11[0], mask32
            vlashr b_imag[0], b_shr
            vstrpv Q(iter)[0], mask32
        {   add b_imag, b_imag, _32                 ;   vsign                                   }
        {                                           ;   vlmul Q(iter)[0]                        }
            vstrpv Q(iter)[0], mask32

        {                                           ;   ldw table, sp[STACK_ROT_TABLE]          }
        {   ldaw r11, sp[STACK_VEC_TMP_IMAG]        ;   ldw iter, sp[STACK_TABLE_ROWS]          }


        .L_inner_loop_top:
            // {   ldaw r11, sp[STACK_VEC_NEG_ONES]        ;   vclrdr                                  }


            {                                           ;   vldr r11[0]                             }
            {   ldaw r11, sp[STACK_VEC_TMP2]            ;   vlmul vec_neg_one[0]                    }
                vstrpv r11[0], mask32
            {                                           ;   vclrdr                                  }
            {   add table, table, _32                   ;   vldc r11[0]     /*-B.imag */            }
            {   ldaw r11, sp[STACK_VEC_TMP_REAL]        ;   vlmacc table[0] /* T.imag */            }
            {   sub table, table, _32                   ;   vldc r11[0]     /* B.real */            }
            {   ldaw r11, sp[STACK_VEC_SAT]             ;   vlmacc table[0] /* T.real */            }
            {   ldaw r11, sp[STACK_VEC_TMP_REAL]        ;   vlsat r11[0]                            }
                vstrpv r11[0], mask32
            {   add table, table, _32                   ;   vclrdr                                  }
            {   ldaw r11, sp[STACK_VEC_TMP_IMAG]        ;   vlmacc table[0] /* T.imag */            }
            {   sub table, table, _32                   ;   vldc r11[0]     /* B.imag */            }
            {   ldaw r11, sp[STACK_VEC_SAT]             ;   vlmacc table[0]                         }
            {   ldaw r11, sp[STACK_VEC_TMP_IMAG]        ;   vlsat r11[0]                            }
                vstrpv r11[0], mask32   
            {   add table, table, _32                   ;   vsign                                   }
            {   sub iter, iter, 1                       ;   vlmul r11[0]                            } // imag = |imag|
                vstrpv r11[0], mask32
            {   add table, table, _32                   ;   bt iter, .L_inner_loop_top              }

        {   ldaw r11, sp[STACK_VEC_TMP_REAL]        ;                                           }
        {   sub length, length, 1                   ;   vldr r11[0]                             }
        {   add a, a, _32                           ;   vstr a[0]                               }
        {                                           ;   bt length, .L_outer_loop_top            }

    .L_outer_loop_bot:  

    {   ldaw r11, sp[STACK_VEC_TMP_REAL]        ;   bf tail_bytes, .L_done                  }

        vlashr b_real[0], b_shr
        vstrpv r11[0], tail_bytes
    {   add b_real, b_real, _32                 ;   vsign                                   }
    {   ldaw Q(iter), sp[STACK_VEC_TMP_IMAG]    ;   vlmul r11[0]                            }
        vstrpv r11[0], mask32
        vlashr b_imag[0], b_shr
        vstrpv Q(iter)[0], tail_bytes
    {   add b_imag, b_imag, _32                 ;   vsign                                   }
    {                                           ;   vlmul Q(iter)[0]                        }
        vstrpv Q(iter)[0], mask32

    {                                           ;   ldw table, sp[STACK_ROT_TABLE]          }
    {   ldaw r11, sp[STACK_VEC_TMP_IMAG]        ;   ldw iter, sp[STACK_TABLE_ROWS]          }

    .L_inner_loop2_top:
        // {   ldaw r11, sp[STACK_VEC_NEG_ONES]        ;   vclrdr                                  }
        {   ldaw r11, sp[STACK_VEC_TMP_IMAG]        ;   vldr r11[0]                             }
        {   ldaw r11, sp[STACK_VEC_TMP2]            ;   vlmul vec_neg_one[0]                    }
            vstrpv r11[0], mask32
        {                                           ;   vclrdr                                  }
        {   add table, table, _32                   ;   vldc r11[0]     /*-B.imag */            }
        {   ldaw r11, sp[STACK_VEC_TMP_REAL]        ;   vlmacc table[0] /* T.imag */            }
        {   sub table, table, _32                   ;   vldc r11[0]     /* B.real */            }
        {   ldaw r11, sp[STACK_VEC_SAT]             ;   vlmacc table[0] /* T.real */            }
        {   ldaw r11, sp[STACK_VEC_TMP_REAL]        ;   vlsat r11[0]                            }
            vstrpv r11[0], mask32
        {   add table, table, _32                   ;   vclrdr                                  }
        {   ldaw r11, sp[STACK_VEC_TMP_IMAG]        ;   vlmacc table[0] /* T.imag */            }
        {   sub table, table, _32                   ;   vldc r11[0]     /* B.imag */            }
        {   ldaw r11, sp[STACK_VEC_SAT]             ;   vlmacc table[0]                         }
        {   ldaw r11, sp[STACK_VEC_TMP_IMAG]        ;   vlsat r11[0]                            }
            vstrpv r11[0], mask32   
        {   add table, table, _32                   ;   vsign                                   }
        {   sub iter, iter, 1                       ;   vlmul r11[0]                            } // imag = |imag|
            vstrpv r11[0], mask32
        {   add table, table, _32                   ;   bt iter, .L_inner_loop2_top             }

    {   ldaw r11, sp[STACK_VEC_TMP_REAL]        ;   vclrdr                                  }
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

    {   ldc r0, 15                              ;   vgetc r11                               }
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



