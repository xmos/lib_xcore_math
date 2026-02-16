// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)

#include "../asm_helper.h"

/*  

headroom_t vect_complex_s32_scale(
    complex_s32_t* a,
    const complex_s32_t* b,
    const int32_t c_real,
    const int32_t c_imag,
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);

*/

.text
.p2align 2


#define NSTACKWORDS     (8+8)

#define a           a0 
#define b           a1 
#define c_real      a2
#define c_imag      a3
#define length      s2
#define b_shr       s3

#define _32         s4
#define tmp_vec     s5


#define STACK_VEC_TMP   (NSTACKWORDS-8-1)

#define FUNCTION_NAME vect_complex_s32_scale
    
    
FUNCTION_NAME:
    xm.entsp (NSTACKWORDS)*4
    xm.stdsp  s3,s2,8
    xm.stdsp  s5,s4,0
    
    { addi tmp_vec,sp, (STACK_VEC_TMP)*4 ; li t3, 0                              }
    { li _32, 32                        ; xm.vsetc t3                           }
    
        xm.stdi  c_real,c_imag, 0(tmp_vec)
        xm.stdi  c_real,c_imag, 8(tmp_vec)
        xm.stdi  c_real,c_imag, 16(tmp_vec)
        xm.stdi  c_real,c_imag, 24(tmp_vec)

    mv c_real, a6
    { xm.mkmski c_imag, 32              ; nop                                   }
        xm.vlashr tmp_vec, c_real
        xm.vstrpv tmp_vec, c_imag

    { nop                               ; xm.vldc tmp_vec                       }

    #undef c_real
    #undef c_imag
    #define vec_count   a2
    #define tail_bytes  a3

    mv length, a4
 //   { nop                                           ;   lw length, (STACK_LENGTH)*4            (sp)}
 mv b_shr, a5
    { slli tail_bytes, length, 3        ; nop                                   }
    { xm.zexti tail_bytes, 5            ; srli vec_count, length, 2             }

    #undef length

    { xm.mkmsk tail_bytes, tail_bytes   ; xm.mkmski t3, 32                      }
    { nop                               ; xm.brff vec_count, .L_loop_bot        }
    

    .L_loop_top:
            xm.vlashr b, b_shr
            xm.vstrpv tmp_vec, t3
        { addi vec_count, vec_count, -1 ; xm.vldd tmp_vec                       }
        { add b, b, _32                 ; xm.vcmr0                              }
        { nop                           ; xm.vcmi0                              }
        { add a, a, _32                 ; xm.vstr a                             }
        { nop                           ; xm.bt vec_count, .L_loop_top          }

.L_loop_bot:

    { nop                               ; xm.brff tail_bytes, .L_done           }

     nop


    xm.vclrdr
    xm.vstd tmp_vec
    xm.vlashr b, b_shr
    xm.vstrpv tmp_vec, tail_bytes
    xm.vldd tmp_vec
    xm.vcmr0
    xm.vcmi0
    xm.vstrpv tmp_vec, tail_bytes
    xm.vstrpv a, tail_bytes
    xm.vldd tmp_vec
    xm.vstd tmp_vec

.p2align 2


    // {                                           ;   bf tail_bytes, .L_done                  }
    // {                                           ;   vclrdr                                  }
    // {                                           ;   vstd tmp_vec[0]                         }
    //     vlashr b[0], b_shr
    //     vstrpv tmp_vec[0], tail_bytes   
    // {                                           ;   vldd tmp_vec[0]                         }
    // {                                           ;   vcmr                                    }
    // {                                           ;   vcmi                                    }
    //     vstrpv tmp_vec[0], tail_bytes
    //     vstrpv a[0], tail_bytes 
    // {                                           ;   vldd tmp_vec[0]                         }
    // {                                           ;   vstd tmp_vec[0]                         }

.p2align 2
.L_done:
        xm.lddsp  s3,s2,8
        xm.lddsp  s5,s4,0

    { li a0, 31                         ; xm.vgetc t3                           }
    { xm.zexti t3, 5                    ; nop                                   }
    { sub a0, a0, t3                    ; xm.retsp (NSTACKWORDS)*4              }


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



