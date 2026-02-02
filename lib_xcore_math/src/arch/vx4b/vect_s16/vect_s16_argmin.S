// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


/*  

unsigned vect_s16_argmin(
    const int16_t b[],
    const unsigned length);
*/


#include "../asm_helper.h"

.text
.p2align 2

#define NSTACKVECS      (3)
#define NSTACKWORDS     (8 + 8*NSTACKVECS+4)

#define FUNCTION_NAME       vect_s16_argmin

#define STACK_VEC_MAX_DEX   (NSTACKWORDS-8-4)
#define STACK_VEC_CUR_MAX   (NSTACKWORDS-16-4)
#define STACK_VEC_CUR_DEX   (NSTACKWORDS-24-4)

#define STACK_N     6

#define b           a0      // ![0x%08X]
#define N           a1      // ![%d]
#define vec_16s     a2      // ![0x%X]
#define tmp         a3      // ![%d]
#define tmz         x18      // ![%d]
#define cur_min     x19      // ![0x%08X]
#define mask_0xF    s4      // ![0x%04X]
#define vec_ones    s5      // ![0x%08X]


FUNCTION_NAME:

    xm.entsp (NSTACKWORDS)*4
    xm.stdsp  s3,s2,8
    xm.stdsp  s5,s4,16

    li t3, 0x100
{ xm.mkmski mask_0xF, 4                 ; sw N, (STACK_N)*4                      (sp) }
{ srli N, N, 4                          ; xm.vsetc t3                           }

// cur_min[i] = 0x7FFF
    la t3, vpu_vec_0x7FFF
{ addi t3,sp, (STACK_VEC_CUR_MAX)*4     ; xm.vldr t3                            }
{ nop                                   ; xm.vstr t3                            }

// cur_dex[i] = i
{ addi tmp,sp, (STACK_VEC_CUR_DEX)*4    ; li t3, 15                             }
.L_setup_cur_dex:
        xm.st16 t3,  t3(tmp)
    { addi t3, t3, -1                   ; xm.bt t3, .L_setup_cur_dex            }

// min_dex[i] = -1
    la t3, vpu_vec_neg_1
{ addi t3,sp, (STACK_VEC_MAX_DEX)*4     ; xm.vldr t3                            }
{ addi cur_min,sp, (STACK_VEC_CUR_MAX)*4 ; xm.vstr t3                            }

    la t3, vpu_vec_0x0010
{ mv vec_16s, t3                        ; xm.vclrdr                             }
    la t3, vpu_vec_0x0001
{ mv vec_ones, t3                       ; nop                                   }
{ mv t3, b                              ; xm.brff N, .L_loop_bot                }


/*

    vR[k] = b[k]
    vR[k] = cur_min[k] - b[k]
    vR[k] = (cur_min[k] - b[k]) < 0 = cur_min[k] < b[k]

    vR[k] = !vR[k] = !(cur_min[k] < b[k]) = (cur_min[k] >= b[k])

    So, this will replace the indexes if b[k] is equal to cur_min[k]

    Instead, we want:
    
    !vR[k] = (cur_min[k] > b[k])
           = cur_min[k] >= b[k] + 1

    vR[k] = !(cur_min[k] >= b[k] + 1)
    vR[k] = cur_min[k] < b[k] + 1
    vR[k] = cur_min[k] - (b[k]+1) < 0
    vR[k] = cur_min[k] - (b[k] + 1)



*/


.L_loop_top:
    { mv b, t3                          ; xm.vldr t3                            }
    { nop                               ; xm.vladd vec_ones                     }
    { addi N, N, -1                     ; xm.vlsub cur_min                      }
    { addi t3,sp, 0                     ; xm.vdepth1                            }
        xm.vstrpv t3, mask_0xF
    { mv t3, b                          ; lw tmp, 0                          (sp) }
    { xm.not tmz, tmp                   ; xm.not tmp, tmp                       }
xm.zip tmz, tmp, 0                                                                
    { addi t3,sp, (STACK_VEC_CUR_DEX)*4 ; xm.vldr t3                            }
        xm.vstrpv cur_min, tmp                                                                
    { addi tmz,sp, (STACK_VEC_MAX_DEX)*4 ; xm.vldr t3                            } 
        xm.vstrpv tmz, tmp                                                                    
    { nop                               ; xm.vladd vec_16s                      }
    { li t3, 32                         ; xm.vstr t3                            } 
    { add t3, b, t3                     ; xm.bt N, .L_loop_top                  }
.L_loop_bot:

{ nop                                   ; lw N, (STACK_N)*4                      (sp) }
{ xm.zexti N, 4                         ; nop                                   }
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
{ addi t3,sp, (STACK_VEC_CUR_DEX)*4     ; xm.vldr t3                            }
    xm.vstrpv cur_min, tmp
{ addi tmz,sp, (STACK_VEC_MAX_DEX)*4    ; xm.vldr t3                            }
    xm.vstrpv tmz, tmp      

.L_no_tail:

#undef cur_min
#undef vec_16s
#undef mask_0xF

#define cur_min     x19  // ![%d]
#define min_dex     a2  // ![0x%08X]


{ addi t3,sp, (STACK_VEC_CUR_MAX)*4     ; li N, 15                              }
{ xm.ldawsp s4, (STACK_VEC_MAX_DEX)*4  ; nop                                   }
xm.ld16s cur_min, N(t3)
xm.ld16s min_dex, N(s4)
{ addi N, N, -1                         ; nop                                   }
.L_loop2_top:
xm.ld16s a0, N(t3)
xm.slt tmp, cur_min, a0
xm.ld16s s5, N(s4)
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
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME, .L_end - FUNCTION_NAME






#endif //defined(__VX4B__)
