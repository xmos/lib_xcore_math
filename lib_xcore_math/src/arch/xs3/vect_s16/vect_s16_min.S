// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


/*  

int32_t vect_s16_min(
    const int32_t b[],
    const unsigned length);


*/


#include "../asm_helper.h"

.text
.issue_mode dual
.align 4

#define NSTACKVECS      (2)
#define NSTACKWORDS     (8 + 8*NSTACKVECS)

#define FUNCTION_NAME       vect_s16_min

#define STACK_VEC_TMP       (NSTACKWORDS-8)
#define STACK_VEC_CUR_MIN   (NSTACKWORDS-16)

#define b           r0      // ![0x%08X]
#define N           r1      // ![%d]
#define tail        r2      // ![0x%X]
#define tmp         r3      // ![%d]
#define tmz         r4      // ![%d]



.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.align 16;
.cc_top FUNCTION_NAME.function,FUNCTION_NAME

FUNCTION_NAME:

        dualentsp NSTACKWORDS
        std r4, r5, sp[1]
        std r6, r7, sp[2]
        ldc r11, 0x100
    {   shl tail, N, SIZEOF_LOG2_S16            ;   vsetc r11                               }
    {   shr N, N, EPV_LOG2_S16                  ;                                           }
    {   shl tmp, N, 5                           ;                                           }
        ldaw r11, cp[vpu_vec_0x7FFF]
    {   ldaw r11, sp[STACK_VEC_CUR_MIN]         ;   vldr r11[0]                             }
    {   add r11, b, tmp                         ;   vstr r11[0]                             }
    {   zext tail, 5                            ;   vldr r11[0]                             }
    {   mkmsk tail, tail                        ;   ldaw r11, sp[STACK_VEC_CUR_MIN]         }
        vstrpv r11[0], tail
    
    // Tail is fully accounted for in cur_min now.

#undef tail
#define cur_min     r2      // ![0x%08X]
    
    {   ldaw tmp, sp[STACK_VEC_TMP]             ;   mov cur_min, r11                        }
    {                                           ;   vclrdr                                  }
    {   mov r11, b                              ;   bf N, .L_loop_bot                       }

    .L_loop_top:
        // cur_min[] saved in stack

        {   mov b, r11                              ;   vldr r11[0]                             } //  vR[i] = b[i]
        {   sub N, N, 1                             ;   vlsub cur_min[0]                        } //  vR[i] = cur_min[i] - b[i]
        {   ldaw r11, sp[0]                         ;   vdepth1                                 } //  vR[0] = [bitmask -- 1 where vR[i] < 0]  b[i] > cur_min[i]
        {   mkmsk tmp, 2                            ;                                           }
            vstrpv r11[0], tmp
        {   mov r11, b                              ;   ldw tmp, sp[0]                          }
        {   mov tmz, tmp                            ;   vldr r11[0]                             }
            zip tmz, tmp, 0
        {   not tmp, tmp                            ;   ldc r11, 32                             }
            vstrpv cur_min[0], tmp
        {   add r11, b, r11                         ;   bt N, .L_loop_top                       }
.L_loop_bot:

    {   ldaw r11, sp[STACK_VEC_CUR_MIN]         ;   ldc N, 15                               }
    {   ldaw r11, sp[STACK_VEC_CUR_MIN]         ;   ld16s cur_min, r11[N]                   }
    {   sub r11, r11, 2                         ;                                           }
    .L_loop2_top:
        {   sub N, N, 1                             ;   ld16s r0, r11[N]                        }
        {   lss tmp, cur_min, r0                    ;                                           }
        {                                           ;   bru tmp                                 }
            {   mov cur_min, r0                         ;                                           }
        {   mov r0, cur_min                         ;   bt N, .L_loop2_top                      }

        ldd r6, r7, sp[2]
        ldd r4, r5, sp[1]
        retsp NSTACKWORDS


.cc_bottom FUNCTION_NAME.function; 
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords; 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores; 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers; 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends; 
.L_end: 
    .size FUNCTION_NAME, .L_end - FUNCTION_NAME






#endif //defined(__XS3A__)