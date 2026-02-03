// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#if defined(__VX4B__)

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
.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.p2align 4

#define y       a0
#define x       a1
#define tmp     a2
#define _32     a3


FUNCTION_NAME:
xm.entsp (NSTACKWORDS)*4
{ li t3, 0                              ; li _32, 32                            }
{ nop                                   ; xm.vsetc t3                           }
la t3, dct8_matrix
{ nop                                   ; xm.vclrdr                             }
{ nop                                   ; xm.vldc x                             }
{ add t3, t3, _32                       ; xm.vlmaccr0 t3                        }
{ add t3, t3, _32                       ; xm.vlmaccr0 t3                        }
{ add t3, t3, _32                       ; xm.vlmaccr0 t3                        }
{ add t3, t3, _32                       ; xm.vlmaccr0 t3                        }
{ add t3, t3, _32                       ; xm.vlmaccr0 t3                        }
{ add t3, t3, _32                       ; xm.vlmaccr0 t3                        }
{ add t3, t3, _32                       ; xm.vlmaccr0 t3                        }
{ add t3, t3, _32                       ; xm.vlmaccr0 t3                        }
xm.vlsat t3 
{ nop                                   ; xm.vstr y                             }
{ li a0, 31                             ; xm.vgetc t3                           }
{ xm.zexti t3, 5                        ; nop                                   }
{ sub a0, a0, t3                        ; xm.retsp (NSTACKWORDS)*4              }

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
