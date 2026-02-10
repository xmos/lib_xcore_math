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

int32_t vladd32(
    const int32_t x, 
    const int32_t y);

********************************************************/

#define FUNCTION_NAME   vladd32
#define NSTACKWORDS     (4)
FUNC_START
FUNCTION_NAME:
{ li t3, 0                              ; xm.entsp (NSTACKWORDS)*4              }
{ nop                                   ; xm.vsetc t3                           }
{ addi t3,sp, 0                         ; sw a0, 0                           (sp) }
{ nop                                   ; xm.vldr t3                            }
{ nop                                   ; sw a1, 0                           (sp) }
{ xm.mkmski a1, 4                       ; xm.vladd t3                           }
    xm.vstrpv t3, a1
{ nop                                   ; lw a0, 0                           (sp) }
{ nop                                   ; xm.retsp (NSTACKWORDS)*4              }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME








/* *****************************************************

int32_t vlsub32(
    const int32_t x, 
    const int32_t y);

********************************************************/

#define FUNCTION_NAME   vlsub32
#define NSTACKWORDS     (4)
FUNC_START
FUNCTION_NAME:
{ li t3, 0                              ; xm.entsp (NSTACKWORDS)*4              }
{ nop                                   ; xm.vsetc t3                           }
{ addi t3,sp, 0                         ; sw a1, 0                           (sp) }
{ nop                                   ; xm.vldr t3                            }
{ nop                                   ; sw a0, 0                           (sp) }
{ xm.mkmski a1, 4                       ; xm.vlsub t3                           }
    xm.vstrpv t3, a1
{ nop                                   ; lw a0, 0                           (sp) }
{ nop                                   ; xm.retsp (NSTACKWORDS)*4              }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME








/* *****************************************************

int32_t vlashr32(
    const int32_t x,
    const right_shift_t shr);

********************************************************/

#define FUNCTION_NAME   vlashr32
#define NSTACKWORDS     (4)
FUNC_START
FUNCTION_NAME:
{ li t3, 0                              ; xm.entsp (NSTACKWORDS)*4              }
{ xm.mkmski a2, 4                       ; xm.vsetc t3                           }
{ addi t3,sp, 0                         ; sw a0, 0                           (sp) }
    xm.vlashr t3, a1
    xm.vstrpv t3, a2
{ nop                                   ; lw a0, 0                           (sp) }
{ nop                                   ; xm.retsp (NSTACKWORDS)*4              }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME








/* *****************************************************

int32_t vpos32(
    const int32_t x);

********************************************************/

#define FUNCTION_NAME   vpos32
#define NSTACKWORDS     (4)
FUNC_START
FUNCTION_NAME:
{ li t3, 0                              ; xm.entsp (NSTACKWORDS)*4              }
{ nop                                   ; xm.vsetc t3                           }
{ addi t3,sp, 0                         ; sw a0, 0                           (sp) }
{ nop                                   ; xm.vldr t3                            }
{ xm.mkmski a1, 4                       ; xm.vpos                               }
    xm.vstrpv t3, a1
{ nop                                   ; lw a0, 0                           (sp) }
{ nop                                   ; xm.retsp (NSTACKWORDS)*4              }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME








/* *****************************************************

int32_t vsign32(
    const int32_t x);

********************************************************/

#define FUNCTION_NAME   vsign32
#define NSTACKWORDS     (4)
FUNC_START
FUNCTION_NAME:
{ li t3, 0                              ; xm.entsp (NSTACKWORDS)*4              }
{ nop                                   ; xm.vsetc t3                           }
{ addi t3,sp, 0                         ; sw a0, 0                           (sp) }
{ nop                                   ; xm.vldr t3                            }
{ xm.mkmski a1, 4                       ; xm.vsign                              }
    xm.vstrpv t3, a1
{ nop                                   ; lw a0, 0                           (sp) }
{ nop                                   ; xm.retsp (NSTACKWORDS)*4              }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME








/* *****************************************************

unsigned vdepth1_32(
    const int32_t x);

********************************************************/

#define FUNCTION_NAME   vdepth1_32
#define NSTACKWORDS     (4)
FUNC_START
FUNCTION_NAME:
{ li t3, 0                              ; xm.entsp (NSTACKWORDS)*4              }
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

int8_t vdepth8_32(
    const int32_t x);

********************************************************/

#define FUNCTION_NAME   vdepth8_32
#define NSTACKWORDS     (4)
FUNC_START
FUNCTION_NAME:
{ li t3, 0                              ; xm.entsp (NSTACKWORDS)*4              }
{ nop                                   ; xm.vsetc t3                           }
{ addi t3,sp, 0                         ; sw a0, 0                           (sp) }
{ nop                                   ; xm.vldr t3                            }
{ xm.mkmski a1, 4                       ; xm.vdepth8                            }
    xm.vstrpv t3, a1
{ nop                                   ; lw a0, 0                           (sp) }
{ xm.sext a0, 8                         ; xm.retsp (NSTACKWORDS)*4              }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME









/* *****************************************************

int16_t vdepth16_32(
    const int32_t x);

********************************************************/

#define FUNCTION_NAME   vdepth16_32
#define NSTACKWORDS     (4)
FUNC_START
FUNCTION_NAME:
{ li t3, 0                              ; xm.entsp (NSTACKWORDS)*4              }
{ nop                                   ; xm.vsetc t3                           }
{ addi t3,sp, 0                         ; sw a0, 0                           (sp) }
{ nop                                   ; xm.vldr t3                            }
{ xm.mkmski a1, 4                       ; xm.vdepth16                           }
    xm.vstrpv t3, a1
{ nop                                   ; lw a0, 0                           (sp) }
{ xm.sext a0, 16                        ; xm.retsp (NSTACKWORDS)*4              }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME









/* *****************************************************

int32_t vlmul32(
    const int32_t x,
    const int32_t y);

********************************************************/

#define FUNCTION_NAME   vlmul32
#define NSTACKWORDS     (4)
FUNC_START
FUNCTION_NAME:
{ li t3, 0                              ; xm.entsp (NSTACKWORDS)*4              }
{ nop                                   ; xm.vsetc t3                           }
{ addi t3,sp, 0                         ; sw a0, 0                           (sp) }
{ nop                                   ; xm.vldr t3                            }
{ nop                                   ; sw a1, 0                           (sp) }
{ xm.mkmski a1, 4                       ; xm.vlmul0 t3                          }
    xm.vstrpv t3, a1
{ nop                                   ; lw a0, 0                           (sp) }
{ nop                                   ; xm.retsp (NSTACKWORDS)*4              }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME









/* *****************************************************

vpu_int32_acc_t vlmacc32(
    const vpu_int32_acc_t acc,
    const int32_t x,
    const int32_t y);

********************************************************/

#define FUNCTION_NAME   vlmacc32
#define NSTACKWORDS     (12)
FUNC_START
FUNCTION_NAME:
{ li t3, 0                              ; xm.entsp (NSTACKWORDS)*4              }
{ addi t3,sp, 0                         ; xm.vsetc t3                           }
{ nop                                   ; sw a1, 0                           (sp) }
{ nop                                   ; xm.vldd t3                            }
{ nop                                   ; sw a0, 0                           (sp) }
{ nop                                   ; xm.vldr t3                            }
{ nop                                   ; sw a2, 0                           (sp) }
{ nop                                   ; xm.vldc t3                            }
{ nop                                   ; sw a3, 0                           (sp) }
{ nop                                   ; xm.vlmacc0 t3                         }
{ nop                                   ; xm.vstd t3                            }
{ nop                                   ; lw a1, 0                           (sp) }
{ nop                                   ; xm.vstr t3                            }
{ nop                                   ; lw a0, 0                           (sp) }
{ nop                                   ; xm.retsp (NSTACKWORDS)*4              }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME









/* *****************************************************

vpu_int32_acc_t vlmaccr32(
    const vpu_int32_acc_t acc,
    const int32_t x[VPU_INT32_EPV],
    const int32_t y[VPU_INT32_EPV]);

********************************************************/

#define FUNCTION_NAME   vlmaccr32
#define NSTACKWORDS     (8)
FUNC_START
FUNCTION_NAME:
{ li t3, 0                              ; xm.entsp (NSTACKWORDS)*4              }
{ addi t3,sp, 0                         ; xm.vsetc t3                           }

// The *last* accumulator is the one that will be added to.
{ nop                                   ; sw a1, 28                           (sp) }
{ nop                                   ; xm.vldd t3                            }
{ nop                                   ; sw a0, 28                           (sp) }
{ nop                                   ; xm.vldr t3                            }
{ nop                                   ; xm.vldc a2                            }
{ nop                                   ; xm.vlmaccr0 a3                        }
{ nop                                   ; xm.vstd t3                            }
{ nop                                   ; lw a1, 0                           (sp) }
{ nop                                   ; xm.vstr t3                            }
{ nop                                   ; lw a0, 0                           (sp) }
{ nop                                   ; xm.retsp (NSTACKWORDS)*4              }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME









/* *****************************************************

int32_t vlsat32(
    const vpu_int32_acc_t acc,
    const unsigned sat);

********************************************************/

#define FUNCTION_NAME   vlsat32
#define NSTACKWORDS     (4)
FUNC_START
FUNCTION_NAME:
{ li t3, 0                              ; xm.entsp (NSTACKWORDS)*4              }
{ addi t3,sp, 0                         ; xm.vsetc t3                           }
{ nop                                   ; sw a1, 0                           (sp) }
{ nop                                   ; xm.vldd t3                            }
{ nop                                   ; sw a0, 0                           (sp) }
{ nop                                   ; xm.vldr t3                            }
{ nop                                   ; sw a2, 0                           (sp) }
{ xm.mkmski a1, 4                       ; nop                                   }
xm.vlsat t3
    xm.vstrpv t3, a1
{ nop                                   ; lw a0, 0                           (sp) }
{ nop                                   ; xm.retsp (NSTACKWORDS)*4              }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME









/* *****************************************************

int32_t vcmr32(
    const complex_s32_t x,
    const complex_s32_t y);

********************************************************/

#define FUNCTION_NAME   vcmr32
#define NSTACKWORDS     (4)
FUNC_START
FUNCTION_NAME:
{ li t3, 0                              ; xm.entsp (NSTACKWORDS)*4              }
{ addi t3,sp, 0                         ; xm.vsetc t3                           }
{ nop                                   ; xm.vldd a0                            }
{ nop                                   ; xm.vldc a1                            }
{ xm.mkmski a1, 8                       ; xm.vcmr0                              }
    xm.vstrpv t3, a1
{ nop                                   ; lw a0, 0                           (sp) }
{ nop                                   ; xm.retsp (NSTACKWORDS)*4              }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME









/* *****************************************************

int32_t vcmi32(
    const complex_s32_t x,
    const complex_s32_t y);

********************************************************/

#define FUNCTION_NAME   vcmi32
#define NSTACKWORDS     (4)
FUNC_START
FUNCTION_NAME:
{ li t3, 0                              ; xm.entsp (NSTACKWORDS)*4              }
{ addi t3,sp, 0                         ; xm.vsetc t3                           }
{ nop                                   ; xm.vldd a0                            }
{ nop                                   ; xm.vldc a1                            }
{ xm.mkmski a1, 8                       ; xm.vcmi0                              }
    xm.vstrpv t3, a1
{ nop                                   ; lw a0, 4                           (sp) }
{ nop                                   ; xm.retsp (NSTACKWORDS)*4              }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME









/* *****************************************************

int32_t vcmcr32(
    const complex_s32_t x,
    const complex_s32_t y);

********************************************************/

#define FUNCTION_NAME   vcmcr32
#define NSTACKWORDS     (4)
FUNC_START
FUNCTION_NAME:
{ li t3, 0                              ; xm.entsp (NSTACKWORDS)*4              }
{ addi t3,sp, 0                         ; xm.vsetc t3                           }
{ nop                                   ; xm.vldd a0                            }
{ nop                                   ; xm.vldc a1                            }
{ xm.mkmski a1, 8                       ; xm.vcmcr0                             }
    xm.vstrpv t3, a1
{ nop                                   ; lw a0, 0                           (sp) }
{ nop                                   ; xm.retsp (NSTACKWORDS)*4              }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME









/* *****************************************************

int32_t vcmci32(
    const complex_s32_t x,
    const complex_s32_t y);

********************************************************/

#define FUNCTION_NAME   vcmci32
#define NSTACKWORDS     (4)
FUNC_START
FUNCTION_NAME:
{ li t3, 0                              ; xm.entsp (NSTACKWORDS)*4              }
{ addi t3,sp, 0                         ; xm.vsetc t3                           }
{ nop                                   ; xm.vldd a0                            }
{ nop                                   ; xm.vldc a1                            }
{ xm.mkmski a1, 8                       ; xm.vcmci0                             }
    xm.vstrpv t3, a1
{ nop                                   ; lw a0, 4                           (sp) }
{ nop                                   ; xm.retsp (NSTACKWORDS)*4              }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME








#endif //defined(__VX4B__)



