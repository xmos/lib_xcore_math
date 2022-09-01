// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.



#if defined(__XS3A__)

#include "../asm_helper.h"

/*  

Push a sample into the buffer, moving everything 1 index up.

void filter_fir_s16_push_sample_up(
    int16_t* buffer,
    const unsigned length,
    const int16_t new_value);
*/

#define FUNCTION_NAME filter_fir_s16_push_sample_up

#define NSTACKVECS      (1)
#define NSTACKWORDS     (10+8*NSTACKVECS)



#define STACK_VEC_TMP   (NSTACKWORDS-8)


#define buff_start  r0
#define length      r1
#define value       r2
#define tmpB        r3
#define mask        r4
#define buffR       r5
#define tmpC        r6
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
    {   ldc tmpB, 32                            ;   stw r10, sp[1]                          }

    {   shl r11, tmpB, 3                        ;   mkmsk mask, 32                          }
    {   mov tmp, length                         ;   vsetc r11                               }

// If the number of samples is odd, pretend it was one larger. If it's even, move the
// final sample without the VPU.

    {   zext tmp, 1                             ;   eq buffR, length, 1                     }
    {   add length, length, tmp                 ;   bt buffR, .L_write_new_sample           }
    {   sub tmp, length, 2                      ;   bt tmp, .L_odd_samps                    }
.L_even_samps:
        {   add tmp, tmp, 1                         ;   ld16s buffD, buff_start[tmp]            }
            st16 buffD, buff_start[tmp] 
.L_odd_samps:

    {   shl mask, mask, 4                       ;   shl length, length, 1                   }

// buffR <-- first byte after buff[]
// mask <-- 0xFFFFFFF0
    {   add buffR, buff_start, length           ;                                           }

// Move buffD and buffR to point to:
    {   sub buffR, buffR, tmpB                  ;   ldc tmpB, 28                            }
    {   sub buffD, buffR, tmpB                  ;   shr mask, mask, 2                       }

// If (buffD < buff_start) then skip the loop.
    {   mov r11, buffR                          ;   lsu tmp, buffD, buff_start              }
    {   ldc tmpB, 56                            ;   bt tmp, .L_loop_end                     }
    {                                           ;   bu .L_loop_top                          }

// Do the loop. Align to 16 bytes so that we hopefully don't have FNOPs after the first
// iteration.
    .align 16
    .L_loop_top:
        {   mov buffR, buffD                        ;   vldr r11[0]                             }
        {   sub buffD, buffD, tmpB                  ;   vldd buffD[0]                           }
        {   lsu tmp, buffD, buff_start              ;   vlmaccr r11[0]                          }
            vstrpv r11[0], mask
        {                                           ;   vstd buffR[0]                           }
        {   sub r11, r11, tmpB                      ;   bf tmp, .L_loop_top                     }


    .L_loop_end:


    // If (r11 < buff_start ) we CANNOT do another vector (just vR[]) using the same
    // mask. Otherwise, we can.

    {   lsu tmp, r11, buff_start                ;                                           }
    {   mov buffR, r11                          ;   bt tmp, .L_skippp                       }
    {   ldc tmpB, 28                            ;   vldr r11[0]                             }
    {   sub r11, r11, tmpB                      ;   vlmaccr buffR[0]                        }
        vstrpv buffR[0], mask

.L_skippp:
    // Now we have less than 1 vector (14 samples) to shift. They'll be at the end of
    // the vector when we load r11. Everything after buff_start.

    {   sub length, buff_start, r11             ;   mkmsk tmpC, 2                           }
    {   mkmsk mask, 32                          ;   bitrev tmpC, tmpC                       }
    {   shl mask, mask, length                  ;   vldr r11[0]                             }
    {   andnot mask, tmpC                       ;   vlmaccr r11[0]                          }
        vstrpv r11[0], mask

.L_write_new_sample:
    {   ldc tmpC, 0                             ;                                           }
        st16 value, buff_start[tmpC]

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



