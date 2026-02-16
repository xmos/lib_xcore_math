// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

/*  

void vect_s16_inverse(
    int16_t a[],
    const int16_t b[],
    const unsigned length,
    const unsigned scale);

*/

#define NSTACKWORDS     (4)

#define FUNCTION_NAME   vect_s16_inverse

#define a               a0
#define b               a1
#define length          a2
#define scale           a3

.text
.p2align 2


FUNCTION_NAME:

{ xm.mkmsk scale, scale                 ; xm.entsp (NSTACKWORDS)*4              }
{ sw s2, 0(sp)                          ; li s2, 15                             }
xm.mkmsk s2, s2
{ addi scale, scale, 1                  ; nop                                   }
{ addi length, length, -1               ; xm.brff length, .L_finish             }

.L_loop_top:
    xm.ld16s t3, length(b)
    div t3, scale, t3
    xm.min t3, t3, s2
    xm.st16 t3, length(a)
    { addi length, length, -1           ; xm.bt length, .L_loop_top             }
    
.L_finish:
    lw s2, 0(sp)
    xm.retsp (NSTACKWORDS)*4

.L_func_end:


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME


#endif //defined(__VX4B__)



