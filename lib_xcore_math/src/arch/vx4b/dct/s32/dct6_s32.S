// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#if defined(__VX4B__)


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
.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.p2align 2

#define y         a0
#define x         a1
#define mask      a2
#define _32       a3


FUNCTION_NAME:
xm.entsp (NSTACKWORDS)*4
{ li t3, 0                              ; li _32, 32                            }
{ xm.mkmski mask, 24                    ; xm.vsetc t3                           }
{ nop                                   ; xm.vldc x                             }
la t3, dct6_matrix
{ nop                                   ; xm.vclrdr                             }
{ add t3, t3, _32                       ; xm.vlmaccr0 t3                        }
{ add t3, t3, _32                       ; xm.vlmaccr0 t3                        }
{ add t3, t3, _32                       ; xm.vlmaccr0 t3                        }
{ add t3, t3, _32                       ; xm.vlmaccr0 t3                        }
{ add t3, t3, _32                       ; xm.vlmaccr0 t3                        }
{ nop                                   ; xm.vlmaccr0 t3                        }
xm.vstrpv y, mask
{ nop                                   ; xm.retsp (NSTACKWORDS)*4              }

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

#endif //defined(__VX4B__)
