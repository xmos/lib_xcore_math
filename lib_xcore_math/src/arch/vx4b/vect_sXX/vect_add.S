// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"



.text
.p2align 2


#define NSTACKWORDS     (32)
#define STACK_VEC_TMP   (NSTACKWORDS-8-1)
#define STACK_BYTEMASK  8

#define a           a0 
#define b           a1 
#define c           a2
#define len         a3

#define shr_b       x18
#define shr_c       x19
#define _32         s4
#define tmp_vec     s5
#define bytemask    len





/*  
headroom_t vect_s16_add(
    int16_t a[],
    const int16_t b[],
    const int16_t c[],
    const unsigned len,
    const int b_shr,
    const int c_shr);
*/
vect_s16_add:
FNAME_S16:
        xm.entsp (NSTACKWORDS)*4
        li t3, 0x100
    { slli t3, len, SIZEOF_LOG2_S16     ; xm.vsetc t3                           }
    { xm.zexti t3, 5                    ; srli len, len, EPV_LOG2_S16           }
        { nop                           ; xm.bu .L_apply_op                     }
.L_func_end_s16: 




/*
headroom_t vect_s32_add(
    int32_t a[],
    const int32_t b[],
    const int32_t c[],
    const unsigned len,
    const int b_shr,
    const int c_shr);
*/
vect_s32_add:
FNAME_S32:
        xm.entsp (NSTACKWORDS)*4
    { li t3, 0                          ; nop                                   }
    { slli t3, len, SIZEOF_LOG2_S32     ; xm.vsetc t3                           }
    { xm.zexti t3, 5                    ; srli len, len, EPV_LOG2_S32           }
    { nop                               ; xm.bu .L_apply_op                     }
.L_func_end_s32: 






/*
    Code shared by all functions above
*/
.type .L_apply_op,@function
.L_apply_op:

        xm.stdsp  s3,s2,0
        xm.stdsp  s5,s4,8
    { li _32, 32                        ; xm.vclrdr                             }
    mv shr_c, a5 
    mv shr_b, a4 
    { xm.mkmsk t3, t3                   ; nop                                   }
    { addi tmp_vec,sp, (NSTACKWORDS-8-1)*4 ; nop                                   }
    { xm.mkmski t3, 32                  ; sw t3, (STACK_BYTEMASK)*4         (sp) }
    { nop                               ; xm.brff len, .L_loop_bot              }
    { nop                               ; xm.bu .L_loop_top                     }

.p2align 4
.L_loop_top:
            xm.vlashr b, shr_b
            xm.vstrpv tmp_vec, t3
            xm.vlashr c, shr_c
        { add b, b, _32                 ; nop                                   } 
        { add c, c, _32                 ; xm.vladd tmp_vec                      }  
        { addi len, len, -1             ; xm.vstr a                             }
        { add a, a, _32                 ; xm.bt len, .L_loop_top                }
.L_loop_bot:

    { nop                               ; lw bytemask, (STACK_BYTEMASK)*4    (sp) }
    { nop                               ; xm.brff bytemask, .L_finish           }
        xm.vlashr b, shr_b
        xm.vstrpv tmp_vec, t3 
        xm.vlashr c, shr_c
    { mv t3, tmp_vec                    ; xm.vladd tmp_vec                      }
    { nop                               ; xm.vstd tmp_vec                       }
        xm.vstrpv tmp_vec, bytemask
        xm.vstrpv a, bytemask
    { nop                               ; xm.vldr t3                            }
    { nop                               ; xm.vstr t3                            }

.L_finish:
    xm.lddsp  s3,s2,0
    xm.lddsp  s5,s4,8

    // Should work for both 16 and 32 bit modes
    { li a0, 32                         ; xm.vgetc t3                           }
    { xm.zexti t3, 5                    ; srli a1, t3, 8                        }
    { xm.shr a0, a0, a1                 ; addi t3, t3, 1                        }
    { sub a0, a0, t3                    ; xm.retsp (NSTACKWORDS)*4              } 

.L_end_apply_op: 
    .size .L_apply_op, .L_end_apply_op - .L_apply_op



.global vect_s16_add
.type vect_s16_add,@function
.set vect_s16_add.nstackwords,NSTACKWORDS;  .global vect_s16_add.nstackwords
.set vect_s16_add.maxcores,1;               .global vect_s16_add.maxcores
.set vect_s16_add.maxtimers,0;              .global vect_s16_add.maxtimers
.set vect_s16_add.maxchanends,0;            .global vect_s16_add.maxchanends
.size vect_s16_add, .L_func_end_s16 - vect_s16_add

.global vect_s32_add
.type vect_s32_add,@function
.set vect_s32_add.nstackwords,NSTACKWORDS;  .global vect_s32_add.nstackwords
.set vect_s32_add.maxcores,1;               .global vect_s32_add.maxcores
.set vect_s32_add.maxtimers,0;              .global vect_s32_add.maxtimers
.set vect_s32_add.maxchanends,0;            .global vect_s32_add.maxchanends
.size vect_s32_add, .L_func_end_s32 - vect_s32_add





#endif //defined(__VX4B__)



