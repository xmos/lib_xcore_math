// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.



#if defined(__XS3A__)

#include "../asm_helper.h"

/*  

Push a sample into the buffer, moving everything 1 index up.

void filter_fir_s16_push_sample_down(
    int16_t* buffer,
    const unsigned length,
    const int16_t new_value);
*/

#define FUNCTION_NAME filter_fir_s16_push_sample_down

#define NSTACKVECS      (1)
#define NSTACKWORDS     (10+8*NSTACKVECS)



#define STACK_VEC_TMP   (NSTACKWORDS-8)


#define buff        r0
#define length      r1
#define value       r2
#define _60         r3
#define mask        r4
#define tail_start  r5
#define buff_end    r6
#define buffD       r7
#define tmp         r10
    
.text
.issue_mode dual
.globl FUNCTION_NAME;
.type FUNCTION_NAME,@function
.align 16
.cc_top FUNCTION_NAME.function,FUNCTION_NAME

FUNCTION_NAME:
        dualentsp NSTACKWORDS
        std r4, r5, sp[1]
        std r6, r7, sp[2]
        std r8, r9, sp[3]
    {   ldc r3, 32                              ;   stw r10, sp[1]                          }

    {   shl r11, r3, 3                          ;   ldc mask, 28 /*28 samples at a time*/   }
    {   mkmsk mask, mask                        ;   vsetc r11                               }

    // We're going to be moving 28 samples per loop iteration. The last address at which we 
    // can move 28 samples is  56 bytes before the end of the buffer. The end of the buffer is
    // at  buff + 2*length. 
    
    {   shl tail_start, length, 1               ;   ldc r11, 56                             }
    {   add buff_end, buff, tail_start          ;   shl mask, mask, 4                       }
    {   sub tail_start, buff_end, r11           ;   add _60, r11, 4                         }

    {   mov r11, buff                           ;   lsu tmp, tail_start, buff               }
    {   ldc tmp, 28                             ;   bt tmp, .L_loop_end                     }
    {   add buffD, buff, tmp                    ;   bu .L_loop_top                          }

    .align 16 // Does this loop have an FNOP after the first iteration? It all fits in the instruction buffer..
    .L_loop_top:
        {   sub buff, r11, 4                        ;   vldr r11[0]                         }
        {   add r11, buff, _60                      ;   vldd buffD[0]                       }
        {   sub buffD, buffD, 4                     ;   vlmaccr buff[0]                     }
        {   lsu tmp, tail_start, r11                ;   vstd buffD[0]                       }
            vstrpv buff [0], mask
        {   add buffD, buffD, _60                   ;   bf tmp, .L_loop_top                 }
    .L_loop_end:

#undef _60

    // r11 holds the address of the next sample to be moved.
    {   sub length, buff_end, r11               ;   ldc tmp, 29                         }
    {   lsu tmp, length, tmp                    ;   ldc r3, 28                          }
    {                                           ;   bt tmp, .L_skippp                   }
    {   sub buff, r11, 4                        ;   vldr r11[0]                         }
    {   add r11, r11, r3                        ;   vlmaccr r11[0]                      }
        vstrpv buff[0], mask

    
    .L_skippp:
    {   sub length, buff_end, r11               ;                                       }
    {   ldc r3, 0                               ;   vldr r11[0]                         }
    {   mkmsk tmp, length                       ;   sub buff, r11, 4                    }
    {   add r11, r11, length                    ;   vlmaccr r11[0]                      }
    {   sub r11, r11, 2                         ;   shl tmp, tmp, 4                     }
        vstrpv buff[0], tmp

        st16 value, r11[r3]

.L_done:
        ldd r8, r9, sp[3]
        ldd r6, r7, sp[2]
        ldd r4, r5, sp[1]
    {                                           ;   ldw r10, sp[1]                          }
        retsp NSTACKWORDS

.cc_bottom FUNCTION_NAME.function; 
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords; 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores; 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers; 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends; 
.L_size_end:
    .size FUNCTION_NAME, .L_size_end - FUNCTION_NAME

#undef FUNCTION_NAME



#endif //defined(__XS3A__)



