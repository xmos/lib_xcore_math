// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

.text
.p2align 2

#define CAT_(A, B)    A##B
#define CAT(A, B)     CAT_(A,B)

#define FUNC_START  \
    .text ; \
    .globl FUNCTION_NAME ; \
    .type FUNCTION_NAME,@function ; \
    .p2align 4


#define FUNC_END                                \
.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4 ;\
.resource_list_empty FUNCTION_NAME, "tail_callees" ;\
.resource_list_empty FUNCTION_NAME, "callees" ;\
.resource_list_empty FUNCTION_NAME, "parallel_callees" ;\
.size FUNCTION_NAME, . - FUNCTION_NAME









/* *****************************************************

int8_t vladd8(
    const int8_t x, 
    const int8_t y);

********************************************************/

#define FUNCTION_NAME   vladd8
#define NSTACKWORDS     (4)

// .global FUNCTION_NAME; 
// .type FUNCTION_NAME,@function;
// .cc_top FUNCTION_NAME.function, FUNCTION_NAME

FUNC_START
FUNCTION_NAME:
{ nop                                   ; xm.entsp (NSTACKWORDS)*4              }
    li t3, 0x200    
{ nop                                   ; xm.vsetc t3                           }
{ addi t3,sp, 0                         ; sw a0, 0                           (sp) }
{ nop                                   ; xm.vldr t3                            }
{ nop                                   ; sw a1, 0                           (sp) }
{ xm.mkmski a1, 4                       ; xm.vladd t3                           }
    xm.vstrpv t3, a1
{ nop                                   ; lw a0, 0                           (sp) }
{ xm.sext a0, 8                         ; xm.retsp (NSTACKWORDS)*4              }
FUNC_END

// //.cc_bottom FUNCTION_NAME.function; 
//.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
//.resource_list_empty FUNCTION_NAME, "tail_callees"
//.resource_list_empty FUNCTION_NAME, "callees"
//.resource_list_empty FUNCTION_NAME, "parallel_callees"

// CAT(.L_size_end_, FUNCTION_NAME): 
//     .size FUNCTION_NAME, CAT(.L_size_end_, FUNCTION_NAME) - FUNCTION_NAME

#undef NSTACKWORDS
#undef FUNCTION_NAME








/* *****************************************************

int8_t vlsub8(
    const int8_t x, 
    const int8_t y);

********************************************************/

#define FUNCTION_NAME   vlsub8
#define NSTACKWORDS     (4)
FUNC_START
FUNCTION_NAME:
{ nop                                   ; xm.entsp (NSTACKWORDS)*4              }
    li t3, 0x200    
{ nop                                   ; xm.vsetc t3                           }
{ addi t3,sp, 0                         ; sw a1, 0                           (sp) }
{ nop                                   ; xm.vldr t3                            }
{ nop                                   ; sw a0, 0                           (sp) }
{ xm.mkmski a1, 4                       ; xm.vlsub t3                           }
    xm.vstrpv t3, a1
{ nop                                   ; lw a0, 0                           (sp) }
{ xm.sext a0, 16                        ; xm.retsp (NSTACKWORDS)*4              }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME








/* *****************************************************

int8_t vlashs6(
    const int8_t x,
    const right_shift_t shr);

********************************************************/

#define FUNCTION_NAME   vlashs6
#define NSTACKWORDS     (4)
FUNC_START
FUNCTION_NAME:
{ nop                                   ; xm.entsp (NSTACKWORDS)*4              }
    li t3, 0x200    
{ xm.mkmski a2, 4                       ; xm.vsetc t3                           }
{ addi t3,sp, 0                         ; sw a0, 0                           (sp) }
    xm.vlashr t3, a1
    xm.vstrpv t3, a2
{ nop                                   ; lw a0, 0                           (sp) }
{ xm.sext a0, 8                         ; xm.retsp (NSTACKWORDS)*4              }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME








/* *****************************************************

int8_t vpos8(
    const int8_t x);

********************************************************/

#define FUNCTION_NAME   vpos8
#define NSTACKWORDS     (4)
FUNC_START
FUNCTION_NAME:
{ nop                                   ; xm.entsp (NSTACKWORDS)*4              }
    li t3, 0x200    
{ nop                                   ; xm.vsetc t3                           }
{ addi t3,sp, 0                         ; sw a0, 0                           (sp) }
{ nop                                   ; xm.vldr t3                            }
{ xm.mkmski a1, 4                       ; xm.vpos                               }
    xm.vstrpv t3, a1
{ nop                                   ; lw a0, 0                           (sp) }
{ xm.sext a0, 8                         ; xm.retsp (NSTACKWORDS)*4              }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME








/* *****************************************************

int8_t vsign8(
    const int8_t x);

********************************************************/

#define FUNCTION_NAME   vsign8
#define NSTACKWORDS     (4)
FUNC_START
FUNCTION_NAME:
{ nop                                   ; xm.entsp (NSTACKWORDS)*4              }
    li t3, 0x200    
{ nop                                   ; xm.vsetc t3                           }
{ addi t3,sp, 0                         ; sw a0, 0                           (sp) }
{ nop                                   ; xm.vldr t3                            }
{ xm.mkmski a1, 4                       ; xm.vsign                              }
    xm.vstrpv t3, a1
{ nop                                   ; lw a0, 0                           (sp) }
{ xm.sext a0, 16                        ; xm.retsp (NSTACKWORDS)*4              }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME








/* *****************************************************

unsigned vdepth1_8(
    const int8_t x);

********************************************************/

#define FUNCTION_NAME   vdepth1_8
#define NSTACKWORDS     (4)
FUNC_START
FUNCTION_NAME:
{ nop                                   ; xm.entsp (NSTACKWORDS)*4              }
    li t3, 0x200    
{ nop                                   ; xm.vsetc t3                           }
{ addi t3,sp, 0                         ; sw a0, 0                           (sp) }
{ nop                                   ; xm.vldr t3                            }
{ xm.mkmski a1, 4                       ; xm.vdepth1                            }
    xm.vstrpv t3, a1
{ nop                                   ; lw a0, 0                           (sp) }
{ xm.zexti a0, 1                        ; xm.retsp (NSTACKWORDS)*4              }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME









/* *****************************************************

int8_t vlmul8(
    const int8_t x,
    const int8_t y);

********************************************************/

#define FUNCTION_NAME   vlmul8
#define NSTACKWORDS     (4)
FUNC_START
FUNCTION_NAME:
{ nop                                   ; xm.entsp (NSTACKWORDS)*4              }
    li t3, 0x200    
{ nop                                   ; xm.vsetc t3                           }
{ addi t3,sp, 0                         ; sw a0, 0                           (sp) }
{ nop                                   ; xm.vldr t3                            }
{ nop                                   ; sw a1, 0                           (sp) }
{ xm.mkmski a1, 4                       ; xm.vlmul0 t3                          }
    xm.vstrpv t3, a1
{ nop                                   ; lw a0, 0                           (sp) }
{ xm.sext a0, 8                         ; xm.retsp (NSTACKWORDS)*4              }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME









/* *****************************************************

vpu_int8_acc_t vlmacc8(
    const vpu_int8_acc_t acc,
    const int8_t x,
    const int8_t y);

********************************************************/

#define FUNCTION_NAME   vlmacc8
#define NSTACKWORDS     (8)
FUNC_START
FUNCTION_NAME:
{ nop                                   ; xm.entsp (NSTACKWORDS)*4              }
    li t3, 0x200    
{ nop                                   ; xm.vsetc t3                           }
{ srli a3, a0, 16                       ; xm.zexti a0, 16                       }
{ addi t3,sp, 0                         ; sw a3, 0                           (sp) }
{ nop                                   ; xm.vldd t3                            }
{ nop                                   ; sw a0, 0                           (sp) }
{ nop                                   ; xm.vldr t3                            }
{ nop                                   ; sw a1, 0                           (sp) }
{ nop                                   ; xm.vldc t3                            }
{ nop                                   ; sw a2, 0                           (sp) }
{ nop                                   ; xm.vlmacc0 t3                         }
{ nop                                   ; xm.vstd t3                            }
{ nop                                   ; lw a1, 0                           (sp) }
{ slli a1, a1, 16                       ; xm.vstr t3                            }
{ nop                                   ; lw a0, 0                           (sp) }
{ xm.zexti a0, 16                       ; nop                                   }
{ or a0, a0, a1                         ; xm.retsp (NSTACKWORDS)*4              }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME









/* *****************************************************

vpu_int8_acc_t vlmaccs6(
    const vpu_int8_acc_t acc,
    const int8_t x[VPU_INT8_EPV],
    const int8_t y[VPU_INT8_EPV]);

********************************************************/

#define FUNCTION_NAME   vlmaccs6
#define NSTACKWORDS     (12)
FUNC_START
FUNCTION_NAME:
{ nop                                   ; xm.entsp (NSTACKWORDS)*4              }
{ xm.mkmski s8, 16                      ; sw s8, 32                          (sp) }
    li t3, 0x200    
{ addi t3,sp, 0                         ; xm.vsetc t3                           }
{ nop                                   ; xm.vclrdr                             }
{ nop                                   ; xm.vstd t3                            }

// The *last* accumulator is the one that will be added to.

{ slli a3, a0, 16                       ; xm.andnot a0, s8                      }
{ addi t3,sp, 0                         ; sw a0, 28                           (sp) }
{ nop                                   ; xm.vldd t3                            }
{ nop                                   ; sw a3, 28                           (sp) }
{ li a3, 31                             ; xm.vldr t3                            }

.L_vlmaccs6_loop_top1:
    { nop                               ; xm.ld8u s8,  a3                        (a1) }
        xm.st8 s8,  a3(t3)
    { addi a3, a3, -1                   ; xm.bt a3, .L_vlmaccs6_loop_top1      }

{ li a3, 31                             ; xm.vldc t3                            }

.L_vlmaccs6_loop_top2:
    { nop                               ; xm.ld8u s8,  a3                        (a2) }
        xm.st8 s8,  a3(t3)
    { addi a3, a3, -1                   ; xm.bt a3, .L_vlmaccs6_loop_top2      }

{ nop                                   ; xm.vlmaccr0 t3                        }
{ nop                                   ; xm.vstd t3                            }
{ nop                                   ; lw a1, 0                           (sp) }
{ nop                                   ; xm.vstr t3                            }
{ slli a1, a1, 16                       ; lw a0, 0                           (sp) }
{ xm.zexti a0, 16                       ; lw s8, 32                          (sp) }
{ or a0, a0, a1                         ; xm.retsp (NSTACKWORDS)*4              }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME









/* *****************************************************

int8_t vlsat8(
    const vpu_int8_acc_t acc,
    const unsigned sat);

********************************************************/

#define FUNCTION_NAME   vlsat8
#define NSTACKWORDS     (4)
FUNC_START
FUNCTION_NAME:
{ nop                                   ; xm.entsp (NSTACKWORDS)*4              }
    li t3, 0x200    
{ nop                                   ; xm.vsetc t3                           }
{ srli a3, a0, 16                       ; xm.zexti a0, 16                       }
{ addi t3,sp, 0                         ; sw a3, 0                           (sp) }
{ nop                                   ; xm.vldd t3                            }
{ nop                                   ; sw a0, 0                           (sp) }
{ nop                                   ; xm.vldr t3                            }
{ nop                                   ; sw a1, 0                           (sp) }
{ xm.mkmski a1, 4                       ; nop                                   }
xm.vlsat t3
    xm.vstrpv t3, a1
{ nop                                   ; lw a0, 0                           (sp) }
{ xm.sext a0, 8                         ; xm.retsp (NSTACKWORDS)*4              }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME








#endif //defined(__VX4B__)



