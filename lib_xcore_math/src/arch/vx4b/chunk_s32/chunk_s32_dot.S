// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)

#include "../asm_helper.h"

.text
.align 16;

/*  
int32_t chunk_s32_dot(
    const int32_t b[VPU_INT32_EPV],
    const q2_30 c[VPU_INT32_EPV]);
*/
#define FUNCTION_NAME   chunk_s32_dot
#define NSTACKWORDS     (4)

#define b         a0
#define c         a1


FUNCTION_NAME:
  xm.entsp (NSTACKWORDS)*4
{ li t3, 0                              ; xm.vldc b                             }
{ xm.mkmski t3, 4                       ; xm.vsetc t3                           }
{ nop                                   ; xm.vclrdr                             }
{ addi a2,sp, 0                         ; xm.vlmaccr0 c                         }
  xm.vstrpv a2, t3
{ nop                                   ; lw a0, 0               (sp)           }
  xm.retsp (NSTACKWORDS)*4

.L_func_end_unpack:

.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME,.L_func_end_unpack - FUNCTION_NAME

#undef NSTACKWORDS
#undef FUNCTION_NAME


#endif //defined(__VX4B__)



