// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

/*  
C_API
sbrad_t radians_to_sbrads(
    const radian_q24_t theta);
*/

#define FUNCTION_NAME   radians_to_sbrads
#define NSTACKWORDS     (0)

#define theta       x10
#define tmp         x11
#define accA        x12
#define accB        x13


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
  xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
{ li tmp, 25                 ; xm.ldap t3, .L_vals           }
{ li accA, 0                 ; li accB, 0                 }
{ nop                             ; lw t3,0             ( t3)}
  xm.maccs accA, accB, t3, theta
  xm.lextract theta, accA, accB, tmp, 32
{ li t3, 30-1                 ; nop                             }
{ xm.shr tmp, theta, t3         ; slli theta, theta, 1         }
{ nop                             ; xm.bru tmp                     }
{ mv a0, theta                   ; nop           }
{xm.retsp (NSTACKWORDS)*4  ; nop           }
{ xm.neg a0, theta               ; nop           }
{xm.retsp (NSTACKWORDS)*4  ; nop           }
{ xm.neg a0, theta               ; nop           }
{xm.retsp (NSTACKWORDS)*4  ; nop           }
{ mv a0, theta                   ; nop           }
{xm.retsp (NSTACKWORDS)*4  ; nop           }
.L_func_end:

.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords /* Translation error on this line: unexpected token at position 42. */ 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends /* Translation error on this line: unexpected token at position 32. */ 
.size FUNCTION_NAME,.L_func_end - FUNCTION_NAME













#endif //defined(__VX4B__)

