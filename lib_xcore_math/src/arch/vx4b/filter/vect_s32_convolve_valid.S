// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


/*  

headroom_t vect_s32_convolve_valid(
    int32_t signal_out[],
    const int32_t signal_in[],
    const int32_t filter_q30[],
    const unsigned signal_in_length,
    const unsigned filter_taps);
    
*/

// #include "../asm_helper.h"

#define NSTACKVECTS     (2)
#define NSTACKWORDS     (16 + 8*NSTACKVECTS+4)

#define FUNCTION_NAME   vect_s32_convolve_valid

#define STACK_VEC_TMP   (NSTACKWORDS-8-4)


#define sig_out     x10
#define sig_in      x11
#define filter      x12
#define len         x13

#define tmpA        x18
#define _32         x19
#define vec_tmp     x20
#define tmpB        x21


#define P           filter    // P = (filter_taps >> 1)



.text; .issue_mode dual /* Translation error on this line: unexpected token at position 5. */ 
.p2align 2


FUNCTION_NAME:

    xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
    xm.stdsp  s3,s2,8
    xm.stdsp  s5,s4,16
    xm.stdsp  s7,s6,24

  ////// Set mode to 32-bit
  { li t3, 0                                ; sw s8, 4                            (sp)}
  { addi vec_tmp,sp, (STACK_VEC_TMP)*4           ; xm.vsetc t3}
  
  ////// Move the filter coefficients into vC[]
  mv tmpB, a4
  { mv t3, filter                           ; nop}
  { slli tmpA, tmpB, 2                         ; xm.vclrdr                                    }
  { xm.mkmsk tmpA, tmpA                          ; xm.vstd vec_tmp}
  { srli P, tmpB, 1                            ; xm.vldr t3}
    xm.vstrpv vec_tmp, tmpA
  { sub len, len, P                          ; xm.vldc vec_tmp}
  { sub len, len, P                          ; li _32, 32                               }
  
  // Number of output elements is  sig_in_length - (2 * (filter_taps >> 1)) = sig_in_length - 2*P

  { srli t3, len, 3                           ; add sig_in, sig_in, _32                   }
  { addi sig_in, sig_in, -4                     ; xm.brff t3, .L_loop_bot                       }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */

  .L_loop_top:
    { addi len, len, -8                           ; xm.vclrdr                                    }
    { addi t3, sig_in, -4                        ; xm.vlmaccr0 sig_in}
    { addi t3, t3, -4                           ; xm.vlmaccr0 t3}
    { addi t3, t3, -4                           ; xm.vlmaccr0 t3}
    { addi t3, t3, -4                           ; xm.vlmaccr0 t3}
    { addi t3, t3, -4                           ; xm.vlmaccr0 t3}
    { addi t3, t3, -4                           ; xm.vlmaccr0 t3}
    { addi t3, t3, -4                           ; xm.vlmaccr0 t3}
    { srli t3, len, 3                           ; xm.vlmaccr0 t3}
    { add sig_in, sig_in, _32                   ; xm.vstr sig_out}
    { add sig_out, sig_out, _32                 ; xm.bt t3, .L_loop_top                       }
  .L_loop_bot:

// If there is a tail, then len will be non-zero.
// In that case, there are len elements left to VLMACCR, but sig_in[] currently points to the last
// element of the group, assuming a full 8 elements are to be output. But of course the tail must,
// by definition, be fewer than 8 elements.  So sig_in[] needs to be offset:
//  sig_in <-- sig_in - 4*(8 - len) = sig_in - 32 + 4*len

  { slli len, len, 2                           ; xm.brff len, .L_finish                         }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */

  { sub sig_in, sig_in, _32                   ; xm.vclrdr                                    }
  { xm.mkmsk tmpA, len                           ; add sig_in, sig_in, len                   }

  .L_tail_loop:
    { addi len, len, -4                         ; xm.vlmaccr0 sig_in}
    { addi sig_in, sig_in, -4                   ; xm.bt len, .L_tail_loop                      }
  .L_tail_loop_bot:

    xm.vstrpv sig_out, tmpA
  { nop                                           ; xm.vstr vec_tmp}

.L_finish:
        xm.lddsp  s3,s2,8
        xm.lddsp  s5,s4,16
        xm.lddsp  s7,s6,24
    {   li a0, 31                              ;   xm.vgetc t3}
    {   xm.zexti t3, 5                             ;   lw s8, 4                          (sp)}
    {   sub a0, a0, t3                         ;   xm.retsp (NSTACKWORDS)*4                       } 
    
.L_func_end:


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS; .global FUNCTION_NAME.nstackwords /* Translation error on this line: unexpected token at position 42. */ 
.set FUNCTION_NAME.maxcores,1;              .global FUNCTION_NAME.maxcores /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;             .global FUNCTION_NAME.maxtimers /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;           .global FUNCTION_NAME.maxchanends /* Translation error on this line: unexpected token at position 32. */ 
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME






#endif //defined(__VX4B__)
