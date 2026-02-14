// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


/*  

unsigned vect_s32_argmin(
    const int32_t b[],
    const unsigned length);


*/


#include "../asm_helper.h"

.text
.p2align 2

#define NSTACKVECS      (3)
#define NSTACKWORDS     (8 + 8*NSTACKVECS)

#define FUNCTION_NAME       vect_s32_argmin

#define STACK_VEC_MAX_DEX   (NSTACKWORDS-8-1)
#define STACK_VEC_CUR_MAX   (NSTACKWORDS-16-1)
#define STACK_VEC_CUR_DEX   (NSTACKWORDS-24-1)

#define STACK_N     6

#define b           a0      // ![0x%08X]
#define N           a1      // ![%d]
#define vec_8s      a2      // ![0x%X]
#define tmp         a3      // ![%d]
#define tmz         s2      // ![%d]
#define cur_min     s3      // ![0x%08X]
#define mask_0xF    s4      // ![0x%04X]
#define vec_ones    s5      // ![0x%08X]



FUNCTION_NAME:

    xm.entsp (NSTACKWORDS)*4
    xm.stdsp  s3,s2,8
    xm.stdsp  s5,s4,16

{ li t3, 0                              ; sw N, (STACK_N)*4                      (sp) }
{ srli N, N, 3                          ; xm.vsetc t3                           }

// cur_min[i] = 0x7FFFFFFF
    la t3, vpu_vec_0x7FFFFFFF
{ addi t3,sp, (STACK_VEC_CUR_MAX)*4     ; xm.vldr t3                            }
{ xm.mkmski mask_0xF, 4                 ; xm.vstr t3                            }

// cur_dex[i] = i
{ addi tmp,sp, (STACK_VEC_CUR_DEX)*4    ; li t3, 7                              }
.L_setup_cur_dex:
xm.stw t3, t3(tmp)
    { addi t3, t3, -1                   ; xm.bt t3, .L_setup_cur_dex            }

// max_dex[i] = -1
la t3, vpu_vec_neg_1
{ addi t3,sp, (STACK_VEC_MAX_DEX)*4     ; xm.vldr t3                            }
{ addi cur_min,sp, (STACK_VEC_CUR_MAX)*4 ; xm.vstr t3                            }

    la t3, vpu_vec_0x00000008
{ mv vec_8s, t3                         ; xm.vclrdr                             }
    la t3, vpu_vec_0x00000001
{ mv vec_ones, t3                       ; nop                                   }
{ mv t3, b                              ; xm.brff N, .L_loop_bot                }

.L_loop_top:
    { mv b, t3                          ; xm.vldr t3                            }
    { nop                               ; xm.vladd vec_ones                     }
    { addi N, N, -1                     ; xm.vlsub cur_min                      }
    { addi t3,sp, 0                     ; xm.vdepth1                            }
        xm.vstrpv t3, mask_0xF
    { mv t3, b                          ; lw tmp, 0                          (sp) }
    { xm.not tmp, tmp                   ; xm.not tmz, tmp                       }
xm.zip tmz, tmp, 0
    { mv tmz, tmp                       ; nop                                   }
xm.zip tmz, tmp, 0
    { addi t3,sp, (STACK_VEC_CUR_DEX)*4 ; xm.vldr t3                            }
        xm.vstrpv cur_min, tmp
    { addi tmz,sp, (STACK_VEC_MAX_DEX)*4 ; xm.vldr t3                            }
        xm.vstrpv tmz, tmp      
    { nop                               ; xm.vladd vec_8s                       }
    { li t3, 32                         ; xm.vstr t3                            }
    { add t3, b, t3                     ; xm.bt N, .L_loop_top                  }
.L_loop_bot:

{ nop                                   ; lw N, (STACK_N)*4                      (sp) }
{ xm.zexti N, 3                         ; nop                                   }
{ xm.mkmsk N, N                         ; xm.brff N, .L_no_tail                 }
{ mv b, t3                              ; xm.vldr t3                            }
{ nop                                   ; xm.vladd vec_ones                     }
{ addi t3,sp, 0                         ; xm.vlsub cur_min                      }
{ nop                                   ; xm.vdepth1                            }
    xm.vstrpv t3, mask_0xF
{ mv t3, b                              ; lw tmp, 0                          (sp) }
{ xm.not tmp, tmp                       ; nop                                   }
{ and tmp, tmp, N                       ; and tmz, tmp, N                       }
xm.zip tmz, tmp, 0
{ mv tmz, tmp                           ; nop                                   }
xm.zip tmz, tmp, 0
{ addi t3,sp, (STACK_VEC_CUR_DEX)*4     ; xm.vldr t3                            }
    xm.vstrpv cur_min, tmp
{ addi tmz,sp, (STACK_VEC_MAX_DEX)*4    ; xm.vldr t3                            }
    xm.vstrpv tmz, tmp      

.L_no_tail:

#undef cur_min
#undef vec_16s
#undef mask_0xF

#define cur_min     s3  // ![%d]
#define min_dex     a2  // ![0x%08X]

{ addi t3,sp, (STACK_VEC_CUR_MAX)*4     ; li N, 7                               }
{ addi s4,sp, (STACK_VEC_MAX_DEX)*4     ; xm.ldw cur_min,N                     ( t3) }
{ addi N, N, -1                         ; xm.ldw min_dex,N                      ( s4) }
.L_loop2_top:
    { nop                               ; xm.ldw a0,N                          ( t3) }
    { xm.slt tmp, cur_min, a0           ; xm.ldw s5,N                           ( s4) }
    xm.eq tmz, cur_min, a0
{ nop                                   ; xm.bt tmp, .L_greater_than            }
    .L_less_or_equal:
        { xm.slt tmp, s5, min_dex       ; xm.brff tmz, .L_less                  }
        .L_equal:
            { nop                       ; xm.brff tmp, .L_greater_than          }
        .L_less:
            { mv cur_min, a0            ; mv min_dex, s5                        }

    .L_greater_than:
    { addi N, N, -1                     ; xm.bt N, .L_loop2_top                 }

    xm.lddsp  s5,s4,16
    xm.lddsp  s3,s2,8
{ mv a0, min_dex                        ; xm.retsp (NSTACKWORDS)*4              }

.L_end: 

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
.size FUNCTION_NAME, .L_end - FUNCTION_NAME






#endif //defined(__VX4B__)
