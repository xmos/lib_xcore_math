// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


#include "../asm_helper.h"

.text
.issue_mode dual
.align 4


#define CAT_(A, B)    A##B
#define CAT(A, B)     CAT_(A,B)

#define FUNC_START                                  \
    .globl FUNCTION_NAME;                           \
    .type FUNCTION_NAME,@function;                  \
    .cc_top FUNCTION_NAME.function,FUNCTION_NAME;                   


#define FUNC_END                                                                            \
    .cc_bottom FUNCTION_NAME.function;                                                      \
    .set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords;      \
    .set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores;         \
    .set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers;        \
    .set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends;      \
    CAT(.L_size_end_, FUNCTION_NAME):                                                       \
    .size FUNCTION_NAME, CAT(.L_size_end_, FUNCTION_NAME) - FUNCTION_NAME










/* *****************************************************

int16_t vladd16(
    const int16_t x, 
    const int16_t y);

********************************************************/

#define FUNCTION_NAME   vladd16
#define NSTACKWORDS     (2)

// .global FUNCTION_NAME; 
// .type FUNCTION_NAME,@function;
// .cc_top FUNCTION_NAME.function, FUNCTION_NAME

FUNC_START
FUNCTION_NAME:
{                                           ;   dualentsp NSTACKWORDS                   }
    ldc r11, 0x100    
{                                           ;   vsetc r11                               }
{   ldaw r11, sp[0]                         ;   stw r0, sp[0]                           }
{                                           ;   vldr r11[0]                             }
{                                           ;   stw r1, sp[0]                           }
{   mkmsk r1, 4                             ;   vladd r11[0]                            }
    vstrpv r11[0], r1
{                                           ;   ldw r0, sp[0]                           }
{   sext r0, 16                             ;   retsp NSTACKWORDS                       }
FUNC_END

// .cc_bottom FUNCTION_NAME.function; 
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
#define NSTACKWORDS     (2)
FUNC_START
FUNCTION_NAME:
{                                           ;   dualentsp NSTACKWORDS                   }
    ldc r11, 0x100    
{                                           ;   vsetc r11                               }
{   ldaw r11, sp[0]                         ;   stw r1, sp[0]                           }
{                                           ;   vldr r11[0]                             }
{                                           ;   stw r0, sp[0]                           }
{   mkmsk r1, 4                             ;   vlsub r11[0]                            }
    vstrpv r11[0], r1
{                                           ;   ldw r0, sp[0]                           }
{   sext r0, 16                             ;   retsp NSTACKWORDS                       }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME








/* *****************************************************

int16_t vlashr16(
    const int16_t x,
    const right_shift_t shr);

********************************************************/

#define FUNCTION_NAME   vlashr16
#define NSTACKWORDS     (2)
FUNC_START
FUNCTION_NAME:
{                                           ;   dualentsp NSTACKWORDS                   }
    ldc r11, 0x100    
{   mkmsk r2, 4                             ;   vsetc r11                               }
{   ldaw r11, sp[0]                         ;   stw r0, sp[0]                           }
    vlashr r11[0], r1
    vstrpv r11[0], r2
{                                           ;   ldw r0, sp[0]                           }
{   sext r0, 16                             ;   retsp NSTACKWORDS                       }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME








/* *****************************************************

int16_t vpos16(
    const int16_t x);

********************************************************/

#define FUNCTION_NAME   vpos16
#define NSTACKWORDS     (2)
FUNC_START
FUNCTION_NAME:
{                                           ;   dualentsp NSTACKWORDS                   }
    ldc r11, 0x100    
{                                           ;   vsetc r11                               }
{   ldaw r11, sp[0]                         ;   stw r0, sp[0]                           }
{                                           ;   vldr r11[0]                             }
{   mkmsk r1, 4                             ;   vpos                                    }
    vstrpv r11[0], r1
{                                           ;   ldw r0, sp[0]                           }
{   sext r0, 16                             ;   retsp NSTACKWORDS                       }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME








/* *****************************************************

int16_t vsign16(
    const int16_t x);

********************************************************/

#define FUNCTION_NAME   vsign16
#define NSTACKWORDS     (2)
FUNC_START
FUNCTION_NAME:
{                                           ;   dualentsp NSTACKWORDS                   }
    ldc r11, 0x100    
{                                           ;   vsetc r11                               }
{   ldaw r11, sp[0]                         ;   stw r0, sp[0]                           }
{                                           ;   vldr r11[0]                             }
{   mkmsk r1, 4                             ;   vsign                                   }
    vstrpv r11[0], r1
{                                           ;   ldw r0, sp[0]                           }
{   sext r0, 16                             ;   retsp NSTACKWORDS                       }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME








/* *****************************************************

unsigned vdepth1_16(
    const int16_t x);

********************************************************/

#define FUNCTION_NAME   vdepth1_16
#define NSTACKWORDS     (2)
FUNC_START
FUNCTION_NAME:
{                                           ;   dualentsp NSTACKWORDS                   }
    ldc r11, 0x100    
{                                           ;   vsetc r11                               }
{   ldaw r11, sp[0]                         ;   stw r0, sp[0]                           }
{                                           ;   vldr r11[0]                             }
{   mkmsk r1, 4                             ;   vdepth1                                 }
    vstrpv r11[0], r1
{                                           ;   ldw r0, sp[0]                           }
{   zext r0, 1                              ;   retsp NSTACKWORDS                       }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME









/* *****************************************************

int8_t vdepth8_16(
    const int16_t x);

********************************************************/

#define FUNCTION_NAME   vdepth8_16
#define NSTACKWORDS     (2)
FUNC_START
FUNCTION_NAME:
{                                           ;   dualentsp NSTACKWORDS                   }
    ldc r11, 0x100    
{                                           ;   vsetc r11                               }
{   ldaw r11, sp[0]                         ;   stw r0, sp[0]                           }
{                                           ;   vldr r11[0]                             }
{   mkmsk r1, 4                             ;   vdepth8                                 }
    vstrpv r11[0], r1
{                                           ;   ldw r0, sp[0]                           }
{   sext r0, 8                              ;   retsp NSTACKWORDS                       }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME









/* *****************************************************

int16_t vlmul16(
    const int16_t x,
    const int16_t y);

********************************************************/

#define FUNCTION_NAME   vlmul16
#define NSTACKWORDS     (2)
FUNC_START
FUNCTION_NAME:
{                                           ;   dualentsp NSTACKWORDS                   }
    ldc r11, 0x100    
{                                           ;   vsetc r11                               }
{   ldaw r11, sp[0]                         ;   stw r0, sp[0]                           }
{                                           ;   vldr r11[0]                             }
{                                           ;   stw r1, sp[0]                           }
{   mkmsk r1, 4                             ;   vlmul r11[0]                            }
    vstrpv r11[0], r1
{                                           ;   ldw r0, sp[0]                           }
{   sext r0, 16                             ;   retsp NSTACKWORDS                       }
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
{                                           ;   dualentsp NSTACKWORDS                   }
    ldc r11, 0x100    
{                                           ;   vsetc r11                               }
{   shr r3, r0, 16                          ;   zext r0, 16                             }
{   ldaw r11, sp[0]                         ;   stw r3, sp[0]                           }
{                                           ;   vldd r11[0]                             }
{                                           ;   stw r0, sp[0]                           }
{                                           ;   vldr r11[0]                             }
{                                           ;   stw r1, sp[0]                           }
{                                           ;   vldc r11[0]                             }
{                                           ;   stw r2, sp[0]                           }
{                                           ;   vlmacc r11[0]                           }
{                                           ;   vstd r11[0]                             }
{                                           ;   ldw r1, sp[0]                           }
{   shl r1, r1, 16                          ;   vstr r11[0]                             }
{                                           ;   ldw r0, sp[0]                           }
{   zext r0, 16                             ;                                           }
{   or r0, r0, r1                           ;   retsp NSTACKWORDS                       }
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
#define NSTACKWORDS     (10)
FUNC_START
FUNCTION_NAME:
{                                           ;   dualentsp NSTACKWORDS                   }
{   mkmsk r10, 16                           ;   stw r10, sp[8]                          }
    ldc r11, 0x100    
{   ldaw r11, sp[0]                         ;   vsetc r11                               }
{                                           ;   vclrdr                                  }
{                                           ;   vstd r11[0]                             }

// The *last* accumulator is the one that will be added to.

{   shl r3, r0, 16                          ;   andnot r0, r10                          }
{   ldaw r11, sp[0]                         ;   stw r0, sp[7]                           }
{                                           ;   vldd r11[0]                             }
{                                           ;   stw r3, sp[7]                           }
{   ldc r3, 15                              ;   vldr r11[0]                             }

.L_vlmaccr16_loop_top1:
    {                                           ;   ld16s r10, r1[r3]                       }
        st16 r10, r11[r3]
    {   sub r3, r3, 1                           ;   bt r3, .L_vlmaccr16_loop_top1           }

{   ldc r3, 15                              ;   vldc r11[0]                             }

.L_vlmaccr16_loop_top2:
    {                                           ;   ld16s r10, r2[r3]                       }
        st16 r10, r11[r3]
    {   sub r3, r3, 1                           ;   bt r3, .L_vlmaccr16_loop_top2           }

{                                           ;   vlmaccr r11[0]                          }
{                                           ;   vstd r11[0]                             }
{                                           ;   ldw r1, sp[0]                           }
{                                           ;   vstr r11[0]                             }
{   shl r1, r1, 16                          ;   ldw r0, sp[0]                           }
{   zext r0, 16                             ;   ldw r10, sp[8]                          }
{   or r0, r0, r1                           ;   retsp NSTACKWORDS                       }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME









/* *****************************************************

int16_t vlsat16(
    const vpu_int16_acc_t acc,
    const unsigned sat);

********************************************************/

#define FUNCTION_NAME   vlsat16
#define NSTACKWORDS     (2)
FUNC_START
FUNCTION_NAME:
{                                           ;   dualentsp NSTACKWORDS                   }
    ldc r11, 0x100    
{                                           ;   vsetc r11                               }
{   shr r3, r0, 16                          ;   zext r0, 16                             }
{   ldaw r11, sp[0]                         ;   stw r3, sp[0]                           }
{                                           ;   vldd r11[0]                             }
{                                           ;   stw r0, sp[0]                           }
{                                           ;   vldr r11[0]                             }
{                                           ;   stw r1, sp[0]                           }
{   mkmsk r1, 4                             ;   vlsat r11[0]                            }
    vstrpv r11[0], r1
{                                           ;   ldw r0, sp[0]                           }
{   sext r0, 16                             ;   retsp NSTACKWORDS                       }
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
{   ldc r1, 8                               ;   dualentsp NSTACKWORDS                   }
    std r4, r5, sp[1]
    ldc r11, 0x100    
{   ldaw r2, sp[4]                          ;   vsetc r11                               }
{   ldaw r3, sp[12]                         ;   ldc r11, 0                              }

.L_split_loop_top:
    {                                           ;   ldw r4, r0[0]                           }
    {   add r0, r0, 8                           ;   ldw r5, r0[1]                           }
        zip r4, r5, 4
    {   sub r1, r1, 1                           ;   stw r4, r3[0]                           }
    {   add r3, r3, 4                           ;   stw r5, r2[0]                           }
    {   add r2, r2, 4                           ;   bt r1, .L_split_loop_top                }

{   ldaw r3, sp[12]                         ;   ldaw r11, sp[4]                         }
{                                           ;   vldd r3[0]                              }
{                                           ;   vldr r11[0]                             }
{                                           ;   vadddr                                  }
{                                           ;   vstd r2[0]                              }
{                                           ;   ldw r4, r2[0]                           }
{   shl r4, r4, 16                          ;   vstr r2[0]                              }
{                                           ;   ldw r0, r2[0]                           }
{   or r0, r0, r4                           ;                                           }

    ldd r4, r5, sp[1]
{                                           ;   retsp NSTACKWORDS                       }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME








#endif //defined(__XS3A__)



