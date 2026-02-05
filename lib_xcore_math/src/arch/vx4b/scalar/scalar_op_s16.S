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
    .set FUNCTION_NAME.nstackwords,NSTACKWORDS;   \  
    .global FUNCTION_NAME.nstackwords;      \
    .set FUNCTION_NAME.maxcores,1;                 \ 
    .global FUNCTION_NAME.maxcores;         \
    .set FUNCTION_NAME.maxtimers,0;                \ 
    .global FUNCTION_NAME.maxtimers;        \
    .set FUNCTION_NAME.maxchanends,0;              \ 
    .global FUNCTION_NAME.maxchanends;      \
    CAT(.L_size_end_, FUNCTION_NAME):              \
    .size FUNCTION_NAME, CAT(.L_size_end_, FUNCTION_NAME) - FUNCTION_NAME










/* *****************************************************

int16_t vladd16(
    const int16_t x, 
    const int16_t y);

********************************************************/

#define FUNCTION_NAME   vladd16
#define NSTACKWORDS     (4)

FUNC_START
FUNCTION_NAME:
{ nop                                   ; xm.entsp (NSTACKWORDS)*4              }
    li t3, 0x100    
{ nop                                   ; xm.vsetc t3                           }
{ addi t3,sp, 0                         ; sw a0, 0                           (sp) }
{ nop                                   ; xm.vldr t3                            }
{ nop                                   ; sw a1, 0                           (sp) }
{ xm.mkmski a1, 4                       ; xm.vladd t3                           }
    xm.vstrpv t3, a1
{ nop                                   ; lw a0, 0                           (sp) }
{ xm.sext a0, 16                        ; xm.retsp (NSTACKWORDS) *4             }
FUNC_END

// //.cc_bottom FUNCTION_NAME.function; 
// .set FUNCTION_NAME.nstackwords,NSTACKWORDS; .global FUNCTION_NAME.nstackwords; 
// .set FUNCTION_NAME.maxcores,1;              .global FUNCTION_NAME.maxcores; 
// .set FUNCTION_NAME.maxtimers,0;             .global FUNCTION_NAME.maxtimers; 
// .set FUNCTION_NAME.maxchanends,0;           .global FUNCTION_NAME.maxchanends; 

// CAT(.L_size_end_, FUNCTION_NAME): 
//     .size FUNCTION_NAME, CAT(.L_size_end_, FUNCTION_NAME) - FUNCTION_NAME

#undef NSTACKWORDS
#undef FUNCTION_NAME








/* *****************************************************

int16_t vlsub16(
    const int16_t x, 
    const int16_t y);

********************************************************/

#define FUNCTION_NAME   vlsub16
#define NSTACKWORDS     (4)
FUNC_START
FUNCTION_NAME:
{ nop                                   ; xm.entsp (NSTACKWORDS)*4              }
    li t3, 0x100    
{ nop                                   ; xm.vsetc t3                           }
{ addi t3,sp, 0                         ; sw a1, 0                           (sp) }
{ nop                                   ; xm.vldr t3                            }
{ nop                                   ; sw a0, 0                           (sp) }
{ xm.mkmski a1, 4                       ; xm.vlsub t3                           }
    xm.vstrpv t3, a1
{ nop                                   ; lw a0, 0                           (sp) }
{ xm.sexti a0, 16                       ; nop                                   }
{ nop                                   ; xm.retsp (NSTACKWORDS)*4              }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME








/* *****************************************************

int16_t vlashr16(
    const int16_t x,
    const right_shift_t shr);

********************************************************/

#define FUNCTION_NAME   vlashr16
#define NSTACKWORDS     (4)
FUNC_START
FUNCTION_NAME:
{ nop                                   ; xm.entsp (NSTACKWORDS)*4              }
    li t3, 0x100    
{ xm.mkmski a2, 4                       ; xm.vsetc t3                           }
{ addi t3,sp, 0                         ; sw a0, 0                           (sp) }
    xm.vlashr t3, a1
    xm.vstrpv t3, a2
{ nop                                   ; lw a0, 0                           (sp) }
{ xm.sext a0, 16                        ; xm.retsp (NSTACKWORDS)*4              }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME








/* *****************************************************

int16_t vpos16(
    const int16_t x);

********************************************************/

#define FUNCTION_NAME   vpos16
#define NSTACKWORDS     (4)
FUNC_START
FUNCTION_NAME:
{ nop                                   ; xm.entsp (NSTACKWORDS)*4              }
    li t3, 0x100    
{ nop                                   ; xm.vsetc t3                           }
{ addi t3,sp, 0                         ; sw a0, 0                           (sp) }
{ nop                                   ; xm.vldr t3                            }
{ xm.mkmski a1, 4                       ; xm.vpos                               }
    xm.vstrpv t3, a1
{ nop                                   ; lw a0, 0                           (sp) }
{ xm.sext a0, 16                        ; nop                                   }
{ nop                                   ; xm.retsp (NSTACKWORDS)*4              }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME








/* *****************************************************

int16_t vsign16(
    const int16_t x);

********************************************************/

#define FUNCTION_NAME   vsign16
#define NSTACKWORDS     (4)
FUNC_START
FUNCTION_NAME:
{ nop                                   ; xm.entsp (NSTACKWORDS)*4              }
    li t3, 0x100    
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

unsigned vdepth1_16(
    const int16_t x);

********************************************************/

#define FUNCTION_NAME   vdepth1_16
#define NSTACKWORDS     (4)
FUNC_START
FUNCTION_NAME:
{ nop                                   ; xm.entsp (NSTACKWORDS)*4              }
    li t3, 0x100    
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

int8_t vdepth8_16(
    const int16_t x);

********************************************************/

#define FUNCTION_NAME   vdepth8_16
#define NSTACKWORDS     (4)
FUNC_START
FUNCTION_NAME:
{ nop                                   ; xm.entsp (NSTACKWORDS)*4              }
    li t3, 0x100    
{ nop                                   ; xm.vsetc t3                           }
{ addi t3,sp, 0                         ; sw a0, 0                           (sp) }
{ nop                                   ; xm.vldr t3                            }
{ xm.mkmski a1, 4                       ; xm.vdepth8                            }
    xm.vstrpv t3, a1
{ nop                                   ; lw a0, 0                           (sp) }
{ xm.sext a0, 8                         ; nop                                   }
{ nop                                   ; xm.retsp (NSTACKWORDS)*4              }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME









/* *****************************************************

int16_t vlmul16(
    const int16_t x,
    const int16_t y);

********************************************************/

#define FUNCTION_NAME   vlmful16
#define NSTACKWORDS     (4)
FUNC_START
FUNCTION_NAME:
{ nop                                   ; xm.entsp (NSTACKWORDS)*4              }
    li t3, 0x100    
{ nop                                   ; xm.vsetc t3                           }
{ addi t3,sp, 0                         ; sw a0, 0                           (sp) }
{ nop                                   ; xm.vldr t3                            }
{ nop                                   ; sw a1, 0                           (sp) }
{ xm.mkmski a1, 4                       ; xm.vlmul0 t3                          }
xm.vlmul1 t3
    xm.vstrpv t3, a1
{ nop                                   ; lw a0, 0                           (sp) }
{ xm.sext a0, 16                        ; xm.retsp (NSTACKWORDS)*4              }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME









/* *****************************************************

vpu_int16_acc_t vlmacc16(
    const vpu_int16_acc_t acc,
    const int16_t x,
    const int16_t y);

********************************************************/

#define FUNCTION_NAME   vlmacc16
#define NSTACKWORDS     (8)
FUNC_START
FUNCTION_NAME:
{ nop                                   ; xm.entsp (NSTACKWORDS)*4              }
    li t3, 0x100    
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
xm.vlmacc1 t3
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

vpu_int16_acc_t vlmaccr16(
    const vpu_int16_acc_t acc,
    const int16_t x[VPU_INT16_EPV],
    const int16_t y[VPU_INT16_EPV]);

********************************************************/

#define FUNCTION_NAME   vlmaccr16
#define NSTACKWORDS     (12)
FUNC_START
FUNCTION_NAME:
{ nop                                   ; xm.entsp (NSTACKWORDS)*4              }
{ xm.mkmski s8, 16                      ; sw s8, 32                          (sp) }
    li t3, 0x100    
{ addi t3,sp, 0                         ; xm.vsetc t3                           }
{ nop                                   ; xm.vclrdr                             }
{ nop                                   ; xm.vstd t3                            }

// The *last* accumulator is the one that will be added to.

{ slli a3, a0, 16                       ; xm.andnot a0, s8                      }
{ addi t3,sp, 0                         ; sw a0, 28                           (sp) }
{ nop                                   ; xm.vldd t3                            }
{ nop                                   ; sw a3, 28                           (sp) }
{ li a3, 15                             ; xm.vldr t3                            }

.L_vlmaccr16_loop_top1:
xm.ld16s s8, a3(a1)                       
        xm.st16 s8,  a3(t3)
    { addi a3, a3, -1                   ; xm.bt a3, .L_vlmaccr16_loop_top1      }

{ li a3, 15                             ; xm.vldc t3                            }

.L_vlmaccr16_loop_top2:
xm.ld16s s8, a3(a2)                       
        xm.st16 s8,  a3(t3)
    { addi a3, a3, -1                   ; xm.bt a3, .L_vlmaccr16_loop_top2      }

{ nop                                   ; xm.vlmaccr0 t3                        }
xm.vlmaccr1 t3
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

int16_t vlsat16(
    const vpu_int16_acc_t acc,
    const unsigned sat);

********************************************************/

#define FUNCTION_NAME   vlsat16
#define NSTACKWORDS     (4)
FUNC_START
FUNCTION_NAME:
{ nop                                   ; xm.entsp (NSTACKWORDS)*4              }
    li t3, 0x100    
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
{ xm.sext a0, 16                        ; nop                                   }
{ nop                                   ; xm.retsp (NSTACKWORDS)*4              }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME









/* *****************************************************

int16_t vadddr16(
    const vpu_int16_acc_t acc[VPU_INT16_ACC_PERIOD]);

********************************************************/

#define FUNCTION_NAME   vadddr16
#define NSTACKWORDS     (12 + 8*2)
FUNC_START
FUNCTION_NAME:
{ li a1, 8                              ; xm.entsp (NSTACKWORDS)*4              }
    xm.stdsp  s3,s2,8
    li t3, 0x100    
{ addi a2,sp, 16                        ; xm.vsetc t3                           }
{ addi a3,sp, 48                        ; li t3, 0                              }

.L_split_loop_top:
    { nop                               ; lw s2,0                           ( a0) }
    { addi a0, a0, 8                    ; lw s3,4                           ( a0) }
xm.zip s2, s3, 4
    { addi a1, a1, -1                   ; sw s2,0                           ( a3) }
    { addi a3, a3, 4                    ; sw s3,0                           ( a2) }
    { addi a2, a2, 4                    ; xm.bt a1, .L_split_loop_top           }

{ xm.ldawsp a3, 12                      ; nop                                   }
{ nop                                   ; xm.ldawsp t3, 4                       }
{ nop                                   ; xm.vldd a3                            }
{ nop                                   ; xm.vldr t3                            }
//{ nop                                           ;   xm.vadddr                                  }
{ nop                                   ; xm.vstd a2                            }
{ nop                                   ; lw s2,0                           ( a2) }
{ slli s2, s2, 16                       ; xm.vstr a2                            }
{ nop                                   ; lw a0,0                           ( a2) }
{ or a0, a0, s2                         ; nop                                   }

    xm.lddsp  s3,s2,8
{ nop                                   ; xm.retsp (NSTACKWORDS)*4              }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME








#endif //defined(__VX4B__)



