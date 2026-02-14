// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#if defined(__VX4B__)


/*  

headroom_t dct_adsb_s32(
    int32_t sums[],
    int32_t diffs[],
    const int32_t head[],
    const int32_t tail[],
    const unsigned chunks,
    const int32_t dct_lut[]);

*/

#define FUNCTION_NAME   dct_adsb_s32
#define NSTACKWORDS 8

.text
.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.p2align 2

#define sums        a0
#define diffs       a1
#define head        a2
#define tail        a3
#define chunks      s2
#define lut         s3
#define _32         s4


FUNCTION_NAME:
xm.entsp (NSTACKWORDS)*4
xm.stdsp  s3,s2,0
xm.stdsp  s5,s4,8
mv chunks, a4
{ li _32, 32                            ; nop                                   }
{nop                                    ; xm.shli t3, _32, 2                    }

mv lut, a5
{ mv t3, tail                           ; xm.vsetc t3                           }

.L_loop_top:
  { addi chunks, chunks, -1             ; xm.vldr t3                            }
  { add tail, tail, _32                 ; xm.vladsb head                        }
  { add head, head, _32                 ; xm.vstr sums                          }
  { mv t3, lut                          ; xm.vstd diffs                         }
  { add lut, lut, _32                   ; xm.vldr t3                            }
  { add sums, sums, _32                 ; xm.vlmul0 diffs                       }
  { add diffs, diffs, _32               ; xm.vstr diffs                         }
  { mv t3, tail                         ; xm.bt chunks, .L_loop_top             }
.L_loop_bot:

xm.lddsp  s5,s4,8
xm.lddsp  s3,s2,0
{ li a0, 31                             ; xm.vgetc t3                           }
{ xm.zexti t3, 5                        ; nop                                   }
{ sub a0, a0, t3                        ; xm.retsp (NSTACKWORDS)*4              }

	
.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
.Ltmp0:
	.size	FUNCTION_NAME, .Ltmp0-FUNCTION_NAME    


#endif //defined(__VX4B__)
