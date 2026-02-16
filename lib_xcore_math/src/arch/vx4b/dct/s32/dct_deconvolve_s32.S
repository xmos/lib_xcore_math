// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#if defined(__VX4B__)


/*  

length must be a multiple of 8

void dct_deconvolve_s32(
    int32_t res[],
    const int32_t B[],
    const int32_t D[],
    const unsigned length);

*/

#define FUNCTION_NAME   dct_deconvolve_s32
#define NSTACKWORDS 8

.text
.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.p2align 2

#define res     a0
#define B       a1
#define D       a2
#define len     a3
#define even    s2
#define a       s3
#define b       s4
#define c       s5


FUNCTION_NAME:
xm.entsp (NSTACKWORDS)*4
xm.stdsp  s3,s2,0
xm.stdsp  s5,s4,8

// Just set it up so that c contains half D[0], so that when
// it's subtracted from D[0] we get (D[0] >> 1)
{ srli len, len, 3                    ; lw c,0                 ( D)           }
  srai c, c, 1
{ li t3, 16                           ; xm.bu .L_loop_top                     }

.p2align 4
  .L_loop_top:
  xm.lddi  a,b, 0(D)
  { sub a, a, c                         ; lw even,0              ( B)           }
  xm.stdi  even,a, 0(res)
  { sub b, b, a                         ; lw even,4              ( B)           }
  xm.stdi  even,b, 8(res)
  xm.lddi  a,c, 8(D)
  { sub a, a, b                         ; lw even,8              ( B)           }
  xm.stdi  even,a, 16(res)
  { sub c, c, a                         ; lw even,12              ( B)          }
  xm.stdi  even,c, 24(res)
  { add D, D, t3                        ; addi len, len, -1                     }
  { add res, res, t3                    ; add B, B, t3                          }
  { add res, res, t3                    ; xm.bt len, .L_loop_top                }
.L_loop_bot:

.L_finish:
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
