// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#if defined(__XS3A__)


/*  

Perform an 8-point forward DCT.

Computed directly by multiplying by the DCT matrix.

headroom_t dct8_forward(
    int32_t y[8],
    const int32_t x[8]);

*/

#define FUNCTION_NAME   dct8_forward
#define NSTACKWORDS 0

.text
.issue_mode dual
.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.align 16
.cc_top FUNCTION_NAME.function,FUNCTION_NAME

#define y       r0
#define x       r1
#define tmp     r2
#define _32     r3


FUNCTION_NAME:
  dualentsp NSTACKWORDS
{ ldc r11, 0                  ; ldc _32, 32                 }
{                             ; vsetc r11                   }
  ldaw r11, cp[dct8_matrix]
{                             ; vclrdr                      }
{                             ; vldc x[0]                   }
{ add r11, r11, _32           ; vlmaccr r11[0]              }
{ add r11, r11, _32           ; vlmaccr r11[0]              }
{ add r11, r11, _32           ; vlmaccr r11[0]              }
{ add r11, r11, _32           ; vlmaccr r11[0]              }
{ add r11, r11, _32           ; vlmaccr r11[0]              }
{ add r11, r11, _32           ; vlmaccr r11[0]              }
{ add r11, r11, _32           ; vlmaccr r11[0]              }
{ add r11, r11, _32           ; vlmaccr r11[0]              }
{                             ; vlsat r11[0]                }
{                             ; vstr y[0]                   }
{ ldc r0, 31                  ; vgetc r11                   }
{ zext r11, 5                 ;                             }
{ sub r0, r0, r11             ; retsp NSTACKWORDS           }

	
.cc_bottom FUNCTION_NAME.function
.set	FUNCTION_NAME.nstackwords,NSTACKWORDS
.globl	FUNCTION_NAME.nstackwords
.set	FUNCTION_NAME.maxcores,1
.globl	FUNCTION_NAME.maxcores
.set	FUNCTION_NAME.maxtimers,0
.globl	FUNCTION_NAME.maxtimers
.set	FUNCTION_NAME.maxchanends,0
.globl	FUNCTION_NAME.maxchanends
.Ltmp0:
	.size	FUNCTION_NAME, .Ltmp0-FUNCTION_NAME    


#endif //defined(__XS3A__)