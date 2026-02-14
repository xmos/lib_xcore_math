// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"


.text
.p2align 2

#define NSTACKWORDS     32

#define STACK_TMP_VEC       (NSTACKWORDS-16)
#define STACK_TMP_VEC_DBL   ((STACK_TMP_VEC)/2)

#define data    a0
#define value   a1
#define length  a2


/*  
void vect_s16_set(
    int16_t data[],
    const int16_t value,
    const unsigned length);
*/
vect_s16_set:
        xm.entsp (NSTACKWORDS)*4
    { slli t3, value, 16                ; slli a3, length, SIZEOF_LOG2_S16      }
    { xm.zexti value, 16                ; xm.zexti a3, 5                        }
    { or t3, t3, value                  ; srli length, length, EPV_LOG2_S16     }
    { mv value, t3                      ; xm.bu .L_set_bytes                    }
.L_size_end_vect_s16_set: 
    .size vect_s16_set, .L_size_end_vect_s16_set - vect_s16_set


/*  
void vect_s32_set(
    int32_t data[],
    const int32_t value,
    const unsigned length);
*/
vect_s32_set:
        xm.entsp (NSTACKWORDS)*4
    { mv t3, value                      ; slli a3, length, SIZEOF_LOG2_S32      }
    { xm.zexti a3, 5                    ; srli length, length, EPV_LOG2_S32     }
    { nop                               ; xm.bu .L_set_bytes                    }
.L_size_end_vect_s32_set: 
    .size vect_s32_set, .L_size_end_vect_s32_set - vect_s32_set

#undef value
#undef length
#define real a1
#define imag a2
#define length a3

/*  
void vect_complex_s32_set(
    complex_s32_t data[],
    const int32_t real_part,
    const int32_t imag_part,
    const unsigned length);
*/
vect_complex_s32_set:
        xm.entsp (NSTACKWORDS)*4
    { mv t3, imag                       ; slli a3, length, SIZEOF_LOG2_C32      }
    { srli a2, length, 5                ; xm.zexti a3, 5                        }
    { nop                               ; xm.bu .L_set_bytes                    }
.L_size_end_vect_complex_s32_set: 
    .size vect_complex_s32_set, .L_size_end_vect_complex_s32_set - vect_complex_s32_set




#undef real 
#undef imag 
#undef length 
#define value   a1
#define length  a2

/*
    Code shared by all functions above.
*/
.type .L_set_bytes,@function
.L_set_bytes:
        xm.stdsp  value,t3,(STACK_TMP_VEC_DBL+0)*8
        xm.stdsp  value,t3,(STACK_TMP_VEC_DBL+1)*8
        xm.stdsp  value,t3,(STACK_TMP_VEC_DBL+2)*8
        xm.stdsp  value,t3,(STACK_TMP_VEC_DBL+3)*8
    { nop                               ; addi t3,sp, (STACK_TMP_VEC)*4         }
    { xm.mkmsk t3, a3                   ; xm.vldr t3                            }  
    { li a3, 32                         ; xm.brff length, .L_loop_bot           }
    { nop                               ; xm.bu .L_loop_top                     }
.p2align 3
    .L_loop_top:
        { addi length, length, -1       ; xm.vstr data                          }
        { add data, data, a3            ; xm.bt length, .L_loop_top             }
.L_loop_bot:
    xm.vstrpv data, t3
    xm.retsp (NSTACKWORDS)*4

.L_end_set_bytes: 
    .size .L_set_bytes, .L_end_set_bytes - .L_set_bytes





.globl vect_s16_set
.type vect_s16_set,@function
.resource_const vect_s16_set, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty vect_s16_set, "tail_callees"
.resource_list_empty vect_s16_set, "callees"
.resource_list_empty vect_s16_set, "parallel_callees"

.globl vect_s32_set
.type vect_s32_set,@function
.resource_const vect_s32_set, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty vect_s32_set, "tail_callees"
.resource_list_empty vect_s32_set, "callees"
.resource_list_empty vect_s32_set, "parallel_callees"

.globl vect_complex_s32_set
.type vect_complex_s32_set,@function
.resource_const vect_complex_s32_set, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty vect_complex_s32_set, "tail_callees"
.resource_list_empty vect_complex_s32_set, "callees"
.resource_list_empty vect_complex_s32_set, "parallel_callees"


#endif //defined(__VX4B__)
