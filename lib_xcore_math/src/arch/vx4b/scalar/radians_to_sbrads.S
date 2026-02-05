// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

/*  
C_API
sbrad_t radians_to_sbrads(
    const radian_q24_t theta);
*/

#define FUNCTION_NAME   radians_to_sbrads
#define NSTACKWORDS     (4)

#define theta       a0
#define tmp         a1
#define accA        a2
#define accB        a3


.text
.p2align 4


/*

  The implementation of this function relies on controlled integer
  overflows to handle the symmetries of of the argument to sin().

  The MACCS instruction multiplies theta by a (Q31) scalar to change
  the period from 2.0*pi to 4.0.

  The LEXTRACT instruction chooses to extract so that the bit 
  corresponding to one period (of 4.0) is just above the MSb, so that
  extra periods are ignored. This number should be thought of as an 
  unsigned 32-bit integer.

  theta is then in a UQ30 format, where the output is specified to be
  in Q31 format.

  A left-shift takes care of subtracting 2.0 (if needed) and converting
  to Q31.

  The only thing that remains is deciding in which cases the result will
  be multiplied by -1, which correspond to the top 2 bits of theta.

*/
.L_vals:
  .word 0x517cc1b7
FUNCTION_NAME:
  xm.entsp (NSTACKWORDS)*4
{ li tmp, 25                            ; xm.ldap t3, .L_vals                   }
{ li accA, 0                            ; li accB, 0                            }
{ nop                                   ; lw t3,0             ( t3)             }
  xm.maccs accA, accB, t3, theta
  xm.lextract theta, accA, accB, tmp, 32
{ li t3, 30                             ; nop                                   }
{ xm.shr tmp, theta, t3                 ; slli theta, theta, 1                  }
{ xm.shli tmp, tmp, 2                   ; nop                                   }
{ nop                                   ; xm.bru tmp                            }
{ mv a0, theta                          ; nop                                   }
{ xm.retsp (NSTACKWORDS)*4              ; nop                                   }
{ xm.neg a0, theta                      ; nop                                   }
{ xm.retsp (NSTACKWORDS)*4              ; nop                                   }
{ xm.neg a0, theta                      ; nop                                   }
{ xm.retsp (NSTACKWORDS)*4              ; nop                                   }
{ mv a0, theta                          ; nop                                   }
{ xm.retsp (NSTACKWORDS)*4              ; nop                                   }
.L_func_end:

.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME,.L_func_end - FUNCTION_NAME













#endif //defined(__VX4B__)

