// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


#include "../asm_helper.h"

/*  
C_API
sbrad_t radians_to_sbrads(
    const radian_q24_t theta);
*/

#define FUNCTION_NAME   radians_to_sbrads
#define NSTACKWORDS     (0)

#define theta       r0
#define tmp         r1
#define accA        r2
#define accB        r3


.text
.issue_mode dual
.align 16


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
.cc_top FUNCTION_NAME.function,FUNCTION_NAME
FUNCTION_NAME:
  dualentsp NSTACKWORDS
{ ldc tmp, 25                 ; ldap r11, .L_vals           }
{ ldc accA, 0                 ; ldc accB, 0                 }
{                             ; ldw r11, r11[0]             }
  maccs accA, accB, r11, theta
  lextract theta, accA, accB, tmp, 32
{ ldc r11, 30                 ;                             }
{ shr tmp, theta, r11         ; shl theta, theta, 1         }
{                             ; bru tmp                     }
{ mov r0, theta               ; retsp NSTACKWORDS           }
{ neg r0, theta               ; retsp NSTACKWORDS           }
{ neg r0, theta               ; retsp NSTACKWORDS           }
{ mov r0, theta               ; retsp NSTACKWORDS           }
.L_func_end:
.cc_bottom FUNCTION_NAME.function

.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME,.L_func_end - FUNCTION_NAME













#endif //defined(__XS3A__)

