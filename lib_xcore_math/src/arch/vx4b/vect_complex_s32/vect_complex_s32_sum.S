// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)

#include "../asm_helper.h"

/*  


void vect_complex_s32_sum(
    const complex_s64_t* res,
    const complex_s32_t* b,
    const unsigned length,
    const right_shift_t b_shr);


*/

.text
.p2align 2

#define NSTACKVECS      (2)
#define NSTACKWORDS     (8+(8*NSTACKVECS))

#define b           a0
#define b_shr       a1
#define length      a2
#define _32         a3
#define tmp         s2
#define tail_bytes  s3

#define STACK_VEC_ZEROS     (NSTACKWORDS- 8-1)
#define STACK_VEC_TMP       (NSTACKWORDS-16-1)

#define STACK_RES   (4)

#define FUNCTION_NAME vect_complex_s32_sum
    


FUNCTION_NAME:

        xm.entsp (NSTACKWORDS)*4
        xm.stdsp  s3,s2,8
        xm.stdsp  s5,s4,0

    { mv b, a1                          ; sw a0, (STACK_RES)*4                   (sp) }


    { mv b_shr, a3                      ; slli tail_bytes, length, 3            }
    { nop                               ; xm.zexti tail_bytes, 5                }

    { addi t3,sp, (STACK_VEC_ZEROS)*4   ; xm.vclrdr                             }
    { li t3, 0                          ; xm.vstd t3                            }
    { xm.slt tmp, b_shr, t3             ; xm.vsetc t3                           }
    { addi tmp,sp, (STACK_VEC_TMP)*4    ; xm.assertn tmp /*Cannot be negative shift*/ }
la t3, vpu_vec_0x40000000
        xm.vlashr t3, b_shr
    { li t3, 0                          ; xm.vstr tmp                           }
    { srli length, length, 2            ; xm.vldc tmp                           }
    { li _32, 32                        ; xm.vsetc t3                           }
    { nop                               ; xm.vclrdr                             }

    { nop                               ; xm.brff length, .L_loop_bot           }

    .L_loop_top:

        { addi length, length, -1       ; xm.vlmacc0 b                          }
        { add b, b, _32                 ; xm.bt length, .L_loop_top             }

    .L_loop_bot:

    { addi t3,sp, (STACK_VEC_ZEROS)*4   ; xm.brff tail_bytes, .L_get_res        }
    { sub t3, t3, tail_bytes            ; nop                                   }
    { nop                               ; xm.vldc t3                            }
    { addi t3,sp, (STACK_VEC_ZEROS)*4   ; xm.vlmacc0 b                          }
        

/*  We've got 8 40-bit accumulators. Lower 32 bits are in vR, upper 8 in vD.
    vD does appear to sign-extend the values up to 64 bits.

    (vD:vR)[k] ==  ((int32_t)vD[k])*(2^32) + ((uint32_t)vR[k]) */

#define real_hi     a0
#define real_lo     a1
#define imag_hi     a2
#define imag_lo     a3
#define num         s3
#define tmp_re      s4
#define tmp_im      s5

// astew [2020-10-16]: There's probably a faster way to do this. See the VPU-based solution I found for vect_s32_sum for
//                     non-complex values

.L_get_res:
    { li real_hi, 0                     ; li imag_hi, 0                         }
    { li num, 1                         ; xm.vstr tmp                           }
        xm.lddi  real_lo,imag_lo, 0(tmp)
        xm.lddi  tmp_re,tmp_im, 8(tmp)
        xm.maccu real_hi, real_lo, num, tmp_re
        xm.maccu imag_hi, imag_lo, num, tmp_im
        xm.lddi  tmp_re,tmp_im, 16(tmp)
        xm.maccu real_hi, real_lo, num, tmp_re
        xm.maccu imag_hi, imag_lo, num, tmp_im
        xm.lddi  tmp_re,tmp_im, 24(tmp)
        xm.maccu real_hi, real_lo, num, tmp_re
        xm.maccu imag_hi, imag_lo, num, tmp_im
    { nop                               ; xm.vfttf                              }
    { li num, 2                         ; xm.vstd t3                            }
        xm.lddi  tmp_re,tmp_im, 0(t3)
    { add real_hi, real_hi, tmp_re      ; add imag_hi, imag_hi, tmp_im          }

    // astew [2021-09-28]: ... what was the purpose of these next 4 instructions..?
    //                     maybe at the time I was thinking the lower word should be
    //                     interpreted as signed?
    // {   shr tmp_re, real_lo, 1                  ;   zext real_lo, 1                         }
    // {   shr tmp_im, imag_lo, 1                  ;   zext imag_lo, 1                         }
    //     maccs real_hi, real_lo, num, tmp_re
    //     maccs imag_hi, imag_lo, num, tmp_im
    { nop                               ; lw tmp, (STACK_RES)*4                  (sp) }
        xm.stdi  real_lo,real_hi, 0(tmp)
        xm.stdi  imag_lo,imag_hi, 8(tmp)

    

.L_done:
        xm.lddsp  s3,s2,8
        xm.lddsp  s5,s4,0
        xm.retsp (NSTACKWORDS)*4

.L_func_end:

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME

#undef FUNCTION_NAME



#endif //defined(__VX4B__)



