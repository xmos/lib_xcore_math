
#if defined(__XS3A__)
#ifndef XS3_MATH_NO_ASM

#include "asm_helper.h"

/*  
headroom_t xs3_vect_s32_scale(
    int32_t a[],
    const int32_t b[],
    const unsigned len,
    const int32_t alpha,
    const int b_shr);
*/


#define NSTACKWORDS     (8+8)

#define FUNCTION_NAME   xs3_vect_s32_scale

#define STACK_B_SHR     (NSTACKWORDS+1)
#define STACK_VEC_TMP   (NSTACKWORDS-8)
#define STACK_BYTEMASK  6

#define a           r0 
#define b           r1 
#define len         r2
#define alpha       r3
#define shr_b       r4
#define _32         r5
#define tmp_vec     r6
#define bytemask    len

.text
.issue_mode dual
.align 4


.cc_top FUNCTION_NAME.function,FUNCTION_NAME

FUNCTION_NAME:
        dualentsp NSTACKWORDS
    {   ldc r11, 0                              ;                                           }
    {   shl r11, len, SIZEOF_LOG2_S32           ;   vsetc r11                               }
    {   zext r11, 5                             ;   shr len, len, EPV_LOG2_S32              }
        std r4, r5, sp[1]
        std r6, r7, sp[2]
    {   ldc _32, 32                             ;   vclrdr                                  }
    {   mkmsk r11, r11                          ;   ldw shr_b, sp[STACK_B_SHR]              }
    {   mkmsk r11, 32                           ;   stw r11, sp[STACK_BYTEMASK]             }
        std alpha, alpha, sp[(STACK_VEC_TMP/2)+0]
        std alpha, alpha, sp[(STACK_VEC_TMP/2)+1]
        std alpha, alpha, sp[(STACK_VEC_TMP/2)+2]
        std alpha, alpha, sp[(STACK_VEC_TMP/2)+3]
    {   ldaw alpha, sp[STACK_VEC_TMP]           ;   bf len, .L_loop_bot                     }
    {                                           ;   bu .L_loop_top                          }

.align 16
.L_loop_top:
            vlashr b[0], shr_b
        {   add b, b, _32                           ;   vlmul alpha[0]                          }  
        {   sub len, len, 1                         ;   vstr a[0]                               }
        {   add a, a, _32                           ;   bt len, .L_loop_top                     }
.L_loop_bot:

    {                                           ;   ldw bytemask, sp[STACK_BYTEMASK]        }
    {                                           ;   bf bytemask, .L_finish                  }
        vlashr b[0], shr_b
    {   mov r11, alpha                          ;   vlmul alpha[0]                          }
    {                                           ;   vstd r11[0]                             }
        vstrpv r11[0], bytemask
        vstrpv a[0], bytemask
    {                                           ;   vldr r11[0]                             }
    {                                           ;   vstr r11[0]                             }

.L_finish:
    ldd r4, r5, sp[1]
    ldd r6, r7, sp[2]

    {   ldc r0, 32                              ;   vgetc r11                               }
    {   zext r11, 5                             ;   shr r1, r11, 8                          }
    {   shr r0, r0, r1                          ;   add r11, r11, 1                         }
    {   sub r0, r0, r11                         ;   retsp NSTACKWORDS                       } 

.L_func_end:
.cc_bottom FUNCTION_NAME.function




.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS; .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;              .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;             .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;           .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME


#endif //!defined(XS3_MATH_NO_ASM)
#endif //defined(__XS3A__)



