// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#if defined(__VX4B__)


/*  

void idct_adsb(
    int32_t sums[],
    int32_t diffs[],
    const int32_t head[],
    const int32_t tail[],
    const unsigned chunks);

*/

#define FUNCTION_NAME   idct_adsb
#define NSTACKWORDS 8

.text
.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.p2align 2

#define sums        a0
#define diffs       a1
#define s           a2
#define t_tilde     a3
#define chunks      s2
#define _32         s3


FUNCTION_NAME:
  xm.entsp (NSTACKWORDS)*4
  xm.stdsp  s3,s2,0
  xm.stdsp  s5,s4,8

  { li _32, 32                          ; nop                                   }
  mv chunks, a4


{ li t3, 0                              ; nop                                   }
{ mv t3, t_tilde                        ; xm.vsetc t3                           }

.L_loop_top:
  { addi chunks, chunks, -1             ; xm.vldr t3                            }
  { add t3, t3, _32                     ; xm.vladsb s                           }
  { add s, s, _32                       ; xm.vstr sums                          }
  { add sums, sums, _32                 ; xm.vstd diffs                         }
  { add diffs, diffs, _32               ; xm.bt chunks, .L_loop_top             }
.L_loop_bot:

  xm.lddsp  s5,s4,8
  xm.lddsp  s3,s2,0
  xm.retsp (NSTACKWORDS)*4

	
.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
.Ltmp0:
	.size	FUNCTION_NAME, .Ltmp0-FUNCTION_NAME    


#endif //defined(__VX4B__)
