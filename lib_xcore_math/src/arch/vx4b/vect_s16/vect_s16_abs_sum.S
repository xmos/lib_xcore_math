// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


/*  
int32_t vect_s16_abs_sum(
    const int16_t b[],
    const unsigned length);
*/


#include "../asm_helper.h"

.text
.p2align 2

#define NSTACKVECS      (4)
#define NSTACKWORDS     (8+8*NSTACKVECS+4)

#define FUNCTION_NAME       vect_s16_abs_sum

#define STACK_VEC_TMP       (NSTACKWORDS-24-4)
#define STACK_VEC_VR        (NSTACKWORDS-32-4)
#define STACK_VEC_TMP2      (NSTACKWORDS-8-2)

#define b           a0 
#define N           a1 
#define tail        a2 
#define tmp         a3
#define neg_1       s2
#define pos_2       s3

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function

FUNCTION_NAME:
        xm.entsp (NSTACKWORDS)*4
        xm.stdsp  s3,s2,0
        xm.stdsp  s5,s4,8
        li t3, 0x0100

     { addi s4, sp, (STACK_VEC_TMP2)*4  ; nop                                   }
         addi s5, s4, (-30)

    { slli tail, N, SIZEOF_LOG2_S16     ; xm.vsetc t3                           }
    { xm.zexti tail, 5                  ; xm.vclrdr                             }
    { srli N, N, EPV_LOG2_S16           ; xm.mkmsk tail, tail                   }
        la t3, vpu_vec_0x0002
    { mv pos_2, t3                      ; nop                                   }
        la t3, vpu_vec_neg_1
    { mv neg_1, t3                      ; xm.vldc t3                            }
    { slli tmp, N, 5                    ; nop                                   }
    { add t3, b, tmp                    ; xm.brff tail, .L_tail_dealt_with      }

    { addi tmp,sp, (STACK_VEC_TMP)*4    ; xm.vldr t3                            }
    { nop                               ; xm.vstd tmp                           }
        xm.vstrpv tmp, tail
    { addi t3,sp, (STACK_VEC_VR)*4      ; xm.vclrdr                             }
    { nop                               ; xm.vlmaccr0 tmp                       }
    xm.vlmaccr1 tmp

    { nop                               ; xm.vstd s4                            }    
    { nop                               ; xm.vldd s5                            }
    { nop                               ; xm.vstr s4                            }
    { nop                               ; xm.vldr s5                            }


    { mv t3, tmp                        ; xm.vstr t3                            }
    { nop                               ; xm.vldr t3                            }
    { nop                               ; xm.vpos                               }
    { addi t3,sp, (STACK_VEC_VR)*4      ; xm.vstr t3                            }
    { nop                               ; xm.vldr t3                            }
    { nop                               ; xm.vldc pos_2                         }
    { nop                               ; xm.vlmaccr0 tmp                       }
    xm.vlmaccr1 tmp

    { nop                               ; xm.vstd s4                            }    
    { nop                               ; xm.vldd s5                            }
    { nop                               ; xm.vstr s4                            }
    { nop                               ; xm.vldr s5                            }




.L_tail_dealt_with:
    { addi tmp,sp, (STACK_VEC_TMP)*4    ; nop                                   }
    { addi t3,sp, (STACK_VEC_VR)*4      ; xm.brff N, .L_loop_bot                }
.L_loop_top:
        { nop                           ; xm.vldc neg_1                         }
        { nop                           ; xm.vlmaccr0 b                         }
        xm.vlmaccr1 b

        { nop                           ; xm.vstd s4                            }    
        { nop                           ; xm.vldd s5                            }
        { nop                           ; xm.vstr s4                            }
        { nop                           ; xm.vldr s5                            }


        { mv t3, b                      ; xm.vstr t3                            }
        { li t3, 32                     ; xm.vldr t3                            }
        { add b, b, t3                  ; xm.vpos                               }
        { addi t3,sp, (STACK_VEC_VR)*4  ; xm.vstr tmp                           }
        { nop                           ; xm.vldr t3                            }
        { nop                           ; xm.vldc pos_2                         }
        { addi N, N, -1                 ; xm.vlmaccr0 tmp                       }
        xm.vlmaccr1 tmp

        { nop                           ; xm.vstd s4                            }    
        { nop                           ; xm.vldd s5                            }
        { nop                           ; xm.vstr s4                            }
        { nop                           ; xm.vldr s5                            }


        { nop                           ; xm.bt N, .L_loop_top                  }
.L_loop_bot:


.L_finish:


   


    { addi a1,sp, (STACK_VEC_TMP)*4     ; nop/*  xm.vadddr */                   }
      addi s4, a1, 32-2                           
    { nop                               ; xm.vstd a1                            }
    { nop                               ; lw a0, 0(s4)                          }
    { slli a0, a0, 16                   ; xm.vstr a1                            }
    { nop                               ; lw a1, 0(s4)                          }
        xm.lddsp  s3,s2,0                               
        xm.lddsp  s5,s4,8
    { xm.zexti a1, 16                   ; nop                                   }
    { or a0, a0, a1                     ; xm.retsp (NSTACKWORDS)*4              }

//.cc_bottom FUNCTION_NAME.function; 
.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
.L_end: 
    .size FUNCTION_NAME, .L_end - FUNCTION_NAME





#endif //defined(__VX4B__)
