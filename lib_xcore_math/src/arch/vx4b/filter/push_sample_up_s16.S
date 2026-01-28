// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.



#if defined(__VX4B__)

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
#define NSTACKWORDS     (12+8*NSTACKVECS)



#define STACK_VEC_TMP   (NSTACKWORDS-8)


#define buff_start  x10
#define length      x11
#define value       x12
#define tmpB        x13
#define mask        x18
#define buffR       x19
#define tmpC        x20
#define buffD       x21
#define tmp         x24
    
.text
.globl FUNCTION_NAME; /* Translation error on this line: unexpected token at position 20. */ 
.type FUNCTION_NAME,@function
.p2align 4

FUNCTION_NAME:
        xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
        xm.stdsp  s3,s2,8
        xm.stdsp  s5,s4,16
        xm.stdsp  s7,s6,24
    {   li tmpB, 32                            ;   sw s8, 4                          (sp)}

    {   slli t3, tmpB, 3                        ;   xm.mkmski mask, 32                          }
    {   mv tmp, length                         ;   xm.vsetc t3}

// If the number of samples is odd, pretend it was one larger. If it's even, move the
// final sample without the VPU.

    xm.zexti tmp, 1
    xm.eq buffR, length, 1 
    {   add length, length, tmp                 ;   xm.bt buffR, .L_write_new_sample           }
    {   addi tmp, length, -2                      ;   xm.bt tmp, .L_odd_samps                    }
.L_even_samps:
   // xm.ld16s buffD, buff_start(tmp)
    xm.ld16s buffD, tmp(buff_start)
    addi tmp, tmp, 1   
    //xm.st16 buffD, buff_start(tmp) 
    xm.st16 buffD, tmp(buff_start)
.L_odd_samps:

    {   slli mask, mask, 4                       ;   slli length, length, 1                   }

// buffR <-- first byte after buff[]
// mask <-- 0xFFFFFFF0
    {   add buffR, buff_start, length           ; nop                                           }

// Move buffD and buffR to point to:
    {   sub buffR, buffR, tmpB                  ;   li tmpB, 28                            }
    {   sub buffD, buffR, tmpB                  ;   srli mask, mask, 2                       }

// If (buffD < buff_start) then skip the loop.
    {   mv t3, buffR                          ;   xm.sltu tmp, buffD, buff_start              }
    {   li tmpB, 56                            ;   xm.bt tmp, .L_loop_end                     }
    { nop                                           ;   xm.bu .L_loop_top                          }

// Do the loop. Align to 16 bytes so that we hopefully don't have FNOPs after the first
// iteration.
    .p2align 4
    .L_loop_top:
        {   mv buffR, buffD                        ;   xm.vldr t3}
        {   sub buffD, buffD, tmpB                  ;   xm.vldd buffD}
        {   xm.sltu tmp, buffD, buff_start              ;   xm.vlmaccr0 t3}
        xm.vlmaccr1 t3
        xm.vstrpv t3, mask
        { nop                                           ;   xm.vstd buffR}
      //  {   sub t3, t3, tmpB                      ;   xm.brff tmp, .L_loop_top                     }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
         {   sub t3, t3, tmpB                      ;   xm.bt tmp, .L_loop_end        }
         {xm.bu .L_loop_top ;nop}
    .L_loop_end:


    // If (x28 < buff_start ) we CANNOT do another vector (just vR[]) using the same
    // mask. Otherwise, we can.

    {   xm.sltu tmp, t3, buff_start                ; nop                                           }
    {   mv buffR, t3                          ;   xm.bt tmp, .L_skippp                       }
    {   li tmpB, 28                            ;   xm.vldr t3}
    {   sub t3, t3, tmpB                     ;   xm.vlmaccr0 buffR}
    xm.vlmaccr1 buffR
    xm.vstrpv buffR, mask

.L_skippp:
    // Now we have less than 1 vector (14 samples) to shift. They'll be at the end of
    // the vector when we load x28. Everything after buff_start.

    {   sub length, buff_start, t3             ;   xm.mkmski tmpC, 2                           }
    {   xm.mkmski mask, 32                          ;   xm.bitrev tmpC, tmpC                       }
    
    { xm.shl mask, mask, length; xm.vldr t3}

    xm.andnot mask, tmpC 
    {nop; xm.vlmaccr0 x28                          }
    xm.vlmaccr1 x28   

    xm.vstrpv t3, mask

.L_write_new_sample:
    {   li tmpC, 0                             ; nop                                           }
   // xm.st16 value, buff_start(tmpC)
     xm.st16 value, tmpC(buff_start)
.L_done:
        xm.lddsp  s7,s6,24
        xm.lddsp  s5,s4,16
        xm.lddsp  s3,s2,8
    { nop                                           ;   lw s8, 4                          (sp)}
        xm.retsp (NSTACKWORDS)*4/* Multiple XAT warnings: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in retsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate", 'RETSP operand may need scaling' */

//.cc_bottom FUNCTION_NAME.function;  /* Translation error on this line: unexpected token at position 33. */ 
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords;  /* Translation error on this line: unexpected token at position 42. */ 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores;  /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers;  /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends;  /* Translation error on this line: unexpected token at position 32. */ 
.L_size_end:
    .size FUNCTION_NAME, .L_size_end - FUNCTION_NAME

#undef FUNCTION_NAME



#endif //defined(__VX4B__)



