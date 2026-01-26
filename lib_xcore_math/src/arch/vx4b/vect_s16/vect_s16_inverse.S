// Copyright 2020-2022 XMOS LIMITED.
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

#define a               x10
#define b               x11
#define length          x12
#define scale           x13

.text
.p2align 2


FUNCTION_NAME:

{   xm.mkmsk scale, scale                      ;  xm.entsp (NSTACKWORDS)*4                    }
{ sw s2, 0(sp); li s2, 15}
xm.mkmsk s2, s2
{   addi scale, scale, 1                     ; nop                                           }
{   addi length, length, -1                   ;   xm.brff length, .L_finish                    }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */

.L_loop_top:
    xm.ld16s t3, length(b)
    div t3, scale, t3
    xm.min t3, t3, s2
    xm.st16 t3, length(a)
    {   addi length, length, -1                   ;   xm.bt length, .L_loop_top                  }
    
.L_finish:
    lw s2, 0(sp)
    xm.retsp (NSTACKWORDS)*4

.L_func_end:


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords /* Translation error on this line: unexpected token at position 42. */ 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends /* Translation error on this line: unexpected token at position 32. */ 
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME


#endif //defined(__VX4B__)



