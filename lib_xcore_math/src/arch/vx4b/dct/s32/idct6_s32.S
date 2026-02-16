// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#if defined(__VX4B__)


/*  

Perform an 6-point inverse DCT.

Computed directly by multiplying by the DCT matrix.

headroom_t dct6_inverse(
    int32_t y[6],
    const int32_t x[6]);

*/

#define FUNCTION_NAME   dct6_inverse
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
la t3, idct6_matrix
{ nop                                   ; xm.vclrdr                             }
{ add t3, t3, _32                       ; xm.vlmaccr0 t3                        }
{ add t3, t3, _32                       ; xm.vlmaccr0 t3                        }
{ add t3, t3, _32                       ; xm.vlmaccr0 t3                        }
{ add t3, t3, _32                       ; xm.vlmaccr0 t3                        }
{ add t3, t3, _32                       ; xm.vlmaccr0 t3                        }
{ add t3, t3, _32                       ; xm.vlmaccr0 t3                        }
xm.vlsat t3 
xm.vstrpv y, mask
{ nop                                   ; xm.retsp (NSTACKWORDS)*4              }


	
.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
.Ltmp0:
	.size	FUNCTION_NAME, .Ltmp0-FUNCTION_NAME    


#endif //defined(__VX4B__)
