// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)



/*  
headroom_t vect_s16_headroom(
    const int16_t* v, 
    const unsigned length);
*/

#include "../asm_helper.h"

.text
.issue_mode dual
.align 4

#define NSTACKWORDS     8

#define STACK_TMP_VEC   0

#define arg_v       r0
#define arg_len     r1


#define FUNCTION_NAME vect_s16_headroom

.cc_top FUNCTION_NAME.function,FUNCTION_NAME
FUNCTION_NAME:
    {   ldc r11, 32                             ;   dualentsp NSTACKWORDS               }
    {   shl r11, r11, 3                         ;   vclrdr                              }
    {   shl r2, arg_len, 1                      ;   vsetc r11                           }
    {   shr arg_len, arg_len, 4                 ;   mov r11, arg_v                      }

    {   ldaw r0, sp[STACK_TMP_VEC]              ;                                       }
    {   ldc r3, 32                              ;                                       }
    {   zext r2, 5                              ;   bf arg_len, .L_loop_bot_s16         }
    {                                           ;   bu .L_loop_top_s16                  }
     
    .align 16
    .L_loop_top_s16:
        {   add r11, r11, r3                        ;   vldr r11[0]                         }
        {   sub arg_len, arg_len, 1                 ;   vstr r0[0]                          }
        {                                           ;   bt arg_len, .L_loop_top_s16         }

.L_loop_bot_s16:
    {                                           ;   bf r2, .L_finish16                  }
    {   mkmsk r2, r2                            ;   vstd r0[0]                          }
    {   mov r11, r0                             ;   vldr r11[0]                         }
    vstrpv r0[0], r2
    {                                           ;   vldr r11[0]                         }
    {                                           ;   vstr r11[0]                         }

.L_finish16:
    {   ldc r0, 15                              ;   vgetc r11                           }
    {   zext r11, 5                             ;                                       }
    {   sub r0, r0, r11                         ;   retsp NSTACKWORDS                   }


.L_func_end_s16:
.cc_bottom FUNCTION_NAME.function

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME, .L_func_end_s16 - FUNCTION_NAME


#undef FUNCTION_NAME







/*  
headroom_t vect_s32_headroom(
    const int32_t* v,
    const unsigned length);
*/

#define FUNCTION_NAME vect_s32_headroom

.cc_top FUNCTION_NAME.function,FUNCTION_NAME
FUNCTION_NAME:
    dualentsp NSTACKWORDS
    {   ldc r11, 0                              ;   shl r2, arg_len, 2                  }
    {   shr arg_len, arg_len, 3                 ;   vsetc r11                           }
    {   ldaw r0, sp[STACK_TMP_VEC]              ;   mov r11, arg_v                      }
    {   ldc r3, 32                              ;   vclrdr                              }
    {   zext r2, 5                              ;   bf arg_len, .L_loop_bot_s32         }
    {                                           ;   bu .L_loop_top_s32                  }

    .align 16
    .L_loop_top_s32:
        {   add r11, r11, r3                        ;   vldr r11[0]                         }
        {   sub arg_len, arg_len, 1                 ;   vstr r0[0]                          }
        {                                           ;   bt arg_len, .L_loop_top_s32         }

    .L_loop_bot_s32:
    {                                           ;   bf r2, .L_finish32                  }
    {   mkmsk r2, r2                            ;   vstd r0[0]                          }
    {   mov r11, r0                             ;   vldr r11[0]                         }
    vstrpv r0[0], r2
    {                                           ;   vldr r11[0]                         }
    {                                           ;   vstr r11[0]                         }

    .L_finish32:
    {   ldc r0, 31                              ;   vgetc r11                           }
    {   zext r11, 5                             ;                                       }
    {   sub r0, r0, r11                         ;   retsp NSTACKWORDS                   }

.L_func_end_s32:
.cc_bottom FUNCTION_NAME.function

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME, .L_func_end_s32 - FUNCTION_NAME

#undef FUNCTION_NAME





#endif //defined(__XS3A__)
