// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.



#if defined(__VX4B__)

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
#define NSTACKWORDS     (12+8*NSTACKVECS)



#define STACK_VEC_TMP   (NSTACKWORDS-8)


#define buff        x10
#define length      x11
#define value       x12
#define _60         x13
#define mask        x18
#define tail_start  x19
#define buff_end    x20
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
    {   li a3, 32                              ;   sw s8, 4                          (sp)}

    {   slli t3, a3, 3                          ;   li mask, 28 /*28 samples at a time*/   }
    {   xm.mkmsk mask, mask                        ;   xm.vsetc t3}

    // We're going to be moving 28 samples per loop iteration. The last address at which we 
    // can move 28 samples is  56 bytes before the end of the buffer. The end of the buffer is
    // at  buff + 2*length. 
    
    {   slli tail_start, length, 1               ;   li t3, 56                             }
    {   add buff_end, buff, tail_start          ;   slli mask, mask, 4                       }
    {   sub tail_start, buff_end, t3           ;   addi _60, t3, 4                         }

    {   mv t3, buff                           ;   xm.sltu tmp, tail_start, buff               }
    {   li tmp, 28                             ;   xm.bt tmp, .L_loop_end                     }
    {   add buffD, buff, tmp                    ;   xm.bu .L_loop_top                          }

    .p2align 4 // Does this loop have an FNOP after the first iteration? It all fits in the instruction buffer..
    .L_loop_top:
        {   addi buff, t3, -4                        ;   xm.vldr t3}
        {   add t3, buff, _60                      ;   xm.vldd buffD}
        {   addi buffD, buffD, -4                     ;   xm.vlmaccr0 buff}
        xm.vlmaccr1 buff
        {   xm.sltu tmp, tail_start, t3                ;   xm.vstd buffD}
            xm.vstrpv buff , mask
        {   add buffD, buffD, _60                   ;   xm.bt tmp, .L_loop_end   } 
        {nop ;   xm.bu .L_loop_top }             /* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
    .L_loop_end:

#undef _60

    // x28 holds the address of the next sample to be moved.
    {   sub length, buff_end, t3               ;   li tmp, 29                         }
    {   xm.sltu tmp, length, tmp                    ;   li a3, 28                          }
    { nop                                           ;   xm.bt tmp, .L_skippp                   }
    {   addi buff, t3, -4                        ;   xm.vldr t3}
    {   nop                        ;   xm.vlmaccr0 t3}
    xm.vlmaccr1 t3
    {   add t3, t3, a3                        ;   nop}
        xm.vstrpv buff, mask

    
    .L_skippp:
    {   sub length, buff_end, t3               ; nop                                       }
    {   li a3, 0                               ;   xm.vldr t3}
    {   xm.mkmsk tmp, length                       ;   addi buff, t3, -4                    }
    {   nop                   ;   xm.vlmaccr0 t3}   
     xm.vlmaccr1 t3            
    {   add t3, t3, length                    ;   nop}
    {   addi t3, t3, -2                         ;   slli tmp, tmp, 4                     }
        xm.vstrpv buff, tmp

        xm.st16 value,  a3(t3)
        //xm.st16 value,  t3(a3)

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



