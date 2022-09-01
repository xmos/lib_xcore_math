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

int32_t vladd32(
    const int32_t x, 
    const int32_t y);

********************************************************/

#define FUNCTION_NAME   vladd32
#define NSTACKWORDS     (2)
FUNC_START
FUNCTION_NAME:
{   ldc r11, 0                              ;   dualentsp NSTACKWORDS                   }
{                                           ;   vsetc r11                               }
{   ldaw r11, sp[0]                         ;   stw r0, sp[0]                           }
{                                           ;   vldr r11[0]                             }
{                                           ;   stw r1, sp[0]                           }
{   mkmsk r1, 4                             ;   vladd r11[0]                            }
    vstrpv r11[0], r1
{                                           ;   ldw r0, sp[0]                           }
{                                           ;   retsp NSTACKWORDS                       }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME








/* *****************************************************

int32_t vlsub32(
    const int32_t x, 
    const int32_t y);

********************************************************/

#define FUNCTION_NAME   vlsub32
#define NSTACKWORDS     (2)
FUNC_START
FUNCTION_NAME:
{   ldc r11, 0                              ;   dualentsp NSTACKWORDS                   }
{                                           ;   vsetc r11                               }
{   ldaw r11, sp[0]                         ;   stw r1, sp[0]                           }
{                                           ;   vldr r11[0]                             }
{                                           ;   stw r0, sp[0]                           }
{   mkmsk r1, 4                             ;   vlsub r11[0]                            }
    vstrpv r11[0], r1
{                                           ;   ldw r0, sp[0]                           }
{                                           ;   retsp NSTACKWORDS                       }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME








/* *****************************************************

int32_t vlashr32(
    const int32_t x,
    const right_shift_t shr);

********************************************************/

#define FUNCTION_NAME   vlashr32
#define NSTACKWORDS     (2)
FUNC_START
FUNCTION_NAME:
{   ldc r11, 0                              ;   dualentsp NSTACKWORDS                   }
{   mkmsk r2, 4                             ;   vsetc r11                               }
{   ldaw r11, sp[0]                         ;   stw r0, sp[0]                           }
    vlashr r11[0], r1
    vstrpv r11[0], r2
{                                           ;   ldw r0, sp[0]                           }
{                                           ;   retsp NSTACKWORDS                       }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME








/* *****************************************************

int32_t vpos32(
    const int32_t x);

********************************************************/

#define FUNCTION_NAME   vpos32
#define NSTACKWORDS     (2)
FUNC_START
FUNCTION_NAME:
{   ldc r11, 0                              ;   dualentsp NSTACKWORDS                   }
{                                           ;   vsetc r11                               }
{   ldaw r11, sp[0]                         ;   stw r0, sp[0]                           }
{                                           ;   vldr r11[0]                             }
{   mkmsk r1, 4                             ;   vpos                                    }
    vstrpv r11[0], r1
{                                           ;   ldw r0, sp[0]                           }
{                                           ;   retsp NSTACKWORDS                       }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME








/* *****************************************************

int32_t vsign32(
    const int32_t x);

********************************************************/

#define FUNCTION_NAME   vsign32
#define NSTACKWORDS     (2)
FUNC_START
FUNCTION_NAME:
{   ldc r11, 0                              ;   dualentsp NSTACKWORDS                   }
{                                           ;   vsetc r11                               }
{   ldaw r11, sp[0]                         ;   stw r0, sp[0]                           }
{                                           ;   vldr r11[0]                             }
{   mkmsk r1, 4                             ;   vsign                                   }
    vstrpv r11[0], r1
{                                           ;   ldw r0, sp[0]                           }
{                                           ;   retsp NSTACKWORDS                       }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME








/* *****************************************************

unsigned vdepth1_32(
    const int32_t x);

********************************************************/

#define FUNCTION_NAME   vdepth1_32
#define NSTACKWORDS     (2)
FUNC_START
FUNCTION_NAME:
{   ldc r11, 0                              ;   dualentsp NSTACKWORDS                   }
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

int8_t vdepth8_32(
    const int32_t x);

********************************************************/

#define FUNCTION_NAME   vdepth8_32
#define NSTACKWORDS     (2)
FUNC_START
FUNCTION_NAME:
{   ldc r11, 0                              ;   dualentsp NSTACKWORDS                   }
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

int16_t vdepth16_32(
    const int32_t x);

********************************************************/

#define FUNCTION_NAME   vdepth16_32
#define NSTACKWORDS     (2)
FUNC_START
FUNCTION_NAME:
{   ldc r11, 0                              ;   dualentsp NSTACKWORDS                   }
{                                           ;   vsetc r11                               }
{   ldaw r11, sp[0]                         ;   stw r0, sp[0]                           }
{                                           ;   vldr r11[0]                             }
{   mkmsk r1, 4                             ;   vdepth16                                }
    vstrpv r11[0], r1
{                                           ;   ldw r0, sp[0]                           }
{   sext r0, 16                             ;   retsp NSTACKWORDS                       }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME









/* *****************************************************

int32_t vlmul32(
    const int32_t x,
    const int32_t y);

********************************************************/

#define FUNCTION_NAME   vlmul32
#define NSTACKWORDS     (2)
FUNC_START
FUNCTION_NAME:
{   ldc r11, 0                              ;   dualentsp NSTACKWORDS                   }
{                                           ;   vsetc r11                               }
{   ldaw r11, sp[0]                         ;   stw r0, sp[0]                           }
{                                           ;   vldr r11[0]                             }
{                                           ;   stw r1, sp[0]                           }
{   mkmsk r1, 4                             ;   vlmul r11[0]                            }
    vstrpv r11[0], r1
{                                           ;   ldw r0, sp[0]                           }
{                                           ;   retsp NSTACKWORDS                       }
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
#define NSTACKWORDS     (8)
FUNC_START
FUNCTION_NAME:
{   ldc r11, 0                              ;   dualentsp NSTACKWORDS                   }
{   ldaw r11, sp[0]                         ;   vsetc r11                               }
{                                           ;   stw r1, sp[0]                           }
{                                           ;   vldd r11[0]                             }
{                                           ;   stw r0, sp[0]                           }
{                                           ;   vldr r11[0]                             }
{                                           ;   stw r2, sp[0]                           }
{                                           ;   vldc r11[0]                             }
{                                           ;   stw r3, sp[0]                           }
{                                           ;   vlmacc r11[0]                           }
{                                           ;   vstd r11[0]                             }
{                                           ;   ldw r1, sp[0]                           }
{                                           ;   vstr r11[0]                             }
{                                           ;   ldw r0, sp[0]                           }
{                                           ;   retsp NSTACKWORDS                       }
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
{   ldc r11, 0                              ;   dualentsp NSTACKWORDS                   }
{   ldaw r11, sp[0]                         ;   vsetc r11                               }

// The *last* accumulator is the one that will be added to.
{                                           ;   stw r1, sp[7]                           }
{                                           ;   vldd r11[0]                             }
{                                           ;   stw r0, sp[7]                           }
{                                           ;   vldr r11[0]                             }
{                                           ;   vldc r2[0]                              }
{                                           ;   vlmaccr r3[0]                           }
{                                           ;   vstd r11[0]                             }
{                                           ;   ldw r1, sp[0]                           }
{                                           ;   vstr r11[0]                             }
{                                           ;   ldw r0, sp[0]                           }
{                                           ;   retsp NSTACKWORDS                       }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME









/* *****************************************************

int32_t vlsat32(
    const vpu_int32_acc_t acc,
    const unsigned sat);

********************************************************/

#define FUNCTION_NAME   vlsat32
#define NSTACKWORDS     (2)
FUNC_START
FUNCTION_NAME:
{   ldc r11, 0                              ;   dualentsp NSTACKWORDS                   }
{   ldaw r11, sp[0]                         ;   vsetc r11                               }
{                                           ;   stw r1, sp[0]                           }
{                                           ;   vldd r11[0]                             }
{                                           ;   stw r0, sp[0]                           }
{                                           ;   vldr r11[0]                             }
{                                           ;   stw r2, sp[0]                           }
{   mkmsk r1, 4                             ;   vlsat r11[0]                            }
    vstrpv r11[0], r1
{                                           ;   ldw r0, sp[0]                           }
{                                           ;   retsp NSTACKWORDS                       }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME









/* *****************************************************

int32_t vcmr32(
    const complex_s32_t x,
    const complex_s32_t y);

********************************************************/

#define FUNCTION_NAME   vcmr32
#define NSTACKWORDS     (2)
FUNC_START
FUNCTION_NAME:
{   ldc r11, 0                              ;   dualentsp NSTACKWORDS                   }
{   ldaw r11, sp[0]                         ;   vsetc r11                               }
{                                           ;   vldd r0[0]                              }
{                                           ;   vldc r1[0]                              }
{   mkmsk r1, 8                             ;   vcmr                                    }
    vstrpv r11[0], r1
{                                           ;   ldw r0, sp[0]                           }
{                                           ;   retsp NSTACKWORDS                       }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME









/* *****************************************************

int32_t vcmi32(
    const complex_s32_t x,
    const complex_s32_t y);

********************************************************/

#define FUNCTION_NAME   vcmi32
#define NSTACKWORDS     (2)
FUNC_START
FUNCTION_NAME:
{   ldc r11, 0                              ;   dualentsp NSTACKWORDS                   }
{   ldaw r11, sp[0]                         ;   vsetc r11                               }
{                                           ;   vldd r0[0]                              }
{                                           ;   vldc r1[0]                              }
{   mkmsk r1, 8                             ;   vcmi                                    }
    vstrpv r11[0], r1
{                                           ;   ldw r0, sp[1]                           }
{                                           ;   retsp NSTACKWORDS                       }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME









/* *****************************************************

int32_t vcmcr32(
    const complex_s32_t x,
    const complex_s32_t y);

********************************************************/

#define FUNCTION_NAME   vcmcr32
#define NSTACKWORDS     (2)
FUNC_START
FUNCTION_NAME:
{   ldc r11, 0                              ;   dualentsp NSTACKWORDS                   }
{   ldaw r11, sp[0]                         ;   vsetc r11                               }
{                                           ;   vldd r0[0]                              }
{                                           ;   vldc r1[0]                              }
{   mkmsk r1, 8                             ;   vcmcr                                   }
    vstrpv r11[0], r1
{                                           ;   ldw r0, sp[0]                           }
{                                           ;   retsp NSTACKWORDS                       }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME









/* *****************************************************

int32_t vcmci32(
    const complex_s32_t x,
    const complex_s32_t y);

********************************************************/

#define FUNCTION_NAME   vcmci32
#define NSTACKWORDS     (2)
FUNC_START
FUNCTION_NAME:
{   ldc r11, 0                              ;   dualentsp NSTACKWORDS                   }
{   ldaw r11, sp[0]                         ;   vsetc r11                               }
{                                           ;   vldd r0[0]                              }
{                                           ;   vldc r1[0]                              }
{   mkmsk r1, 8                             ;   vcmci                                   }
    vstrpv r11[0], r1
{                                           ;   ldw r0, sp[1]                           }
{                                           ;   retsp NSTACKWORDS                       }
FUNC_END
#undef NSTACKWORDS
#undef FUNCTION_NAME








#endif //defined(__XS3A__)



