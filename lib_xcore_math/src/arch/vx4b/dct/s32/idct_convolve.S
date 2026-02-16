// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#if defined(__VX4B__)


/*  

Applies the convolution needed for the recursive IDCT.

given x[], the result is

  y[0] = x[0]
  y[1:] = 0.5*(y[1:] + y[0:-1])

Each "chunk" is 8 elements, so if the data isn't a multiple of 8 elements
you'll need buffer space at the end of the data that can be safely clobbered.

void idct_convolve(
    int32_t y[],
    const int32_t x[],
    const unsigned chunks);

*/

#define FUNCTION_NAME   idct_convolve
#define NSTACKWORDS 4

.text
.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.p2align 2

#define y        a0
#define x        a1
#define chunks   a2
#define _32      a3
#define tmp      s2


FUNCTION_NAME:
  xm.entsp (NSTACKWORDS)*4
  xm.stdsp  s3,s2,0

  // This has to start at the end or else values will get clobbered
  // early if it's done in-place

  li t3, 0x80  // 32-bit mode with SHR=1 on VLADSB
{ xm.shli s3, chunks, 3                 ; xm.vsetc t3                           }
{ li _32, 32                            ; lw tmp,0               ( x)           }
  sh2add x, s3, x
  sh2add y, s3, y
{ sub x, x, _32                         ; sub y, y, _32                         }
{ addi t3, x, -4                        ; nop                                   }

.L_loop_top:
  { addi chunks, chunks, -1             ; xm.vldr t3                            }
  { sub x, x, _32                       ; xm.vladsb x                           }
  { sub y, y, _32                       ; xm.vstr y                             }
  { addi t3, x, -4                      ; xm.bt chunks, .L_loop_top             }
.L_loop_bot:
  
  sw tmp,32 ( y)// y is pointing 8 words before where it started
  
  xm.lddsp  s3,s2,0
  xm.retsp (NSTACKWORDS)*4

	
.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
.Ltmp0:
	.size	FUNCTION_NAME, .Ltmp0-FUNCTION_NAME    


#endif //defined(__VX4B__)
