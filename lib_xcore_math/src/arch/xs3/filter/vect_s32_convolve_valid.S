// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


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
#define NSTACKWORDS     (14 + 8*NSTACKVECTS)

#define FUNCTION_NAME   vect_s32_convolve_valid

#define STACK_TAPS      (NSTACKWORDS+1)

#define STACK_VEC_TMP   (NSTACKWORDS-8)


#define sig_out     r0
#define sig_in      r1
#define filter      r2
#define len         r3

#define tmpA        r4
#define _32         r5
#define vec_tmp     r6
#define tmpB        r7


#define P           filter    // P = (filter_taps >> 1)



.text; .issue_mode dual
.align 4

.cc_top FUNCTION_NAME.function,FUNCTION_NAME

FUNCTION_NAME:

    dualentsp NSTACKWORDS
    std r4, r5, sp[1]
    std r6, r7, sp[2]
    std r8, r9, sp[3]

  ////// Set mode to 32-bit
  { ldc r11, 0                                ; stw r10, sp[1]                            }
  { ldaw vec_tmp, sp[STACK_VEC_TMP]           ; vsetc r11                                 }
  
  ////// Move the filter coefficients into vC[]
  { mov r11, filter                           ; ldw tmpB, sp[STACK_TAPS]                  }
  { shl tmpA, tmpB, 2                         ; vclrdr                                    }
  { mkmsk tmpA, tmpA                          ; vstd vec_tmp[0]                           }
  { shr P, tmpB, 1                            ; vldr r11[0]                               }
    vstrpv vec_tmp[0], tmpA
  { sub len, len, P                           ; vldc vec_tmp[0]                           }
  { sub len, len, P                           ; ldc _32, 32                               }
  
  // Number of output elements is  sig_in_length - (2 * (filter_taps >> 1)) = sig_in_length - 2*P

  { shr r11, len, 3                           ; add sig_in, sig_in, _32                   }
  { sub sig_in, sig_in, 4                     ; bf r11, .L_loop_bot                       }

  .L_loop_top:
    { sub len, len, 8                           ; vclrdr                                    }
    { sub r11, sig_in, 4                        ; vlmaccr sig_in[0]                         }
    { sub r11, r11, 4                           ; vlmaccr r11[0]                            }
    { sub r11, r11, 4                           ; vlmaccr r11[0]                            }
    { sub r11, r11, 4                           ; vlmaccr r11[0]                            }
    { sub r11, r11, 4                           ; vlmaccr r11[0]                            }
    { sub r11, r11, 4                           ; vlmaccr r11[0]                            }
    { sub r11, r11, 4                           ; vlmaccr r11[0]                            }
    { shr r11, len, 3                           ; vlmaccr r11[0]                            }
    { add sig_in, sig_in, _32                   ; vstr sig_out[0]                           }
    { add sig_out, sig_out, _32                 ; bt r11, .L_loop_top                       }
  .L_loop_bot:

// If there is a tail, then len will be non-zero.
// In that case, there are len elements left to VLMACCR, but sig_in[] currently points to the last
// element of the group, assuming a full 8 elements are to be output. But of course the tail must,
// by definition, be fewer than 8 elements.  So sig_in[] needs to be offset:
//  sig_in <-- sig_in - 4*(8 - len) = sig_in - 32 + 4*len

  { shl len, len, 2                           ; bf len, .L_finish                         }

  { sub sig_in, sig_in, _32                   ; vclrdr                                    }
  { mkmsk tmpA, len                           ; add sig_in, sig_in, len                   }

  .L_tail_loop:
    { sub len, len, 4                         ; vlmaccr sig_in[0]                         }
    { sub sig_in, sig_in, 4                   ; bt len, .L_tail_loop                      }
  .L_tail_loop_bot:

    vstrpv sig_out[0], tmpA
  {                                           ; vstr vec_tmp[0]                           }

.L_finish:
        ldd r4, r5, sp[1]
        ldd r6, r7, sp[2]
        ldd r8, r9, sp[3]
    {   ldc r0, 31                              ;   vgetc r11                               }
    {   zext r11, 5                             ;   ldw r10, sp[1]                          }
    {   sub r0, r0, r11                         ;   retsp NSTACKWORDS                       } 
        retsp NSTACKWORDS

.L_func_end:
.cc_bottom FUNCTION_NAME.function


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS; .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;              .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;             .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;           .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME






#endif //defined(__XS3A__)