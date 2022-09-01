// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#if defined(__XS3A__)


/*  

Perform an 6-point forward DCT.

Computed directly by multiplying by the DCT matrix. The output has elements ordered 
so that when used in recursive DCT computation the bit-reversed indexing can be used
to deconvolve those that need it.

void dct6_forward(
    int32_t y[6],
    const int32_t x[6]);

*/

#define FUNCTION_NAME   dct6_forward
#define NSTACKWORDS 0

.text
.issue_mode dual
.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.align 4
.cc_top FUNCTION_NAME.function,FUNCTION_NAME

#define y         r0
#define x         r1
#define mask      r2
#define _32       r3


FUNCTION_NAME:
  dualentsp NSTACKWORDS
{ ldc r11, 0                  ; ldc _32, 32                 }
{ mkmsk mask, 24              ; vsetc r11                   }
{                             ; vldc x[0]                   }
  ldaw r11, cp[dct6_matrix]
{                             ; vclrdr                      }
{ add r11, r11, _32           ; vlmaccr r11[0]              }
{ add r11, r11, _32           ; vlmaccr r11[0]              }
{ add r11, r11, _32           ; vlmaccr r11[0]              }
{ add r11, r11, _32           ; vlmaccr r11[0]              }
{ add r11, r11, _32           ; vlmaccr r11[0]              }
{                             ; vlmaccr r11[0]              }
  vstrpv y[0], mask
{                             ; retsp NSTACKWORDS           }


	
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