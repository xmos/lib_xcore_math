// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#if defined(__XS3A__)


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
#define NSTACKWORDS 4

.text
.issue_mode dual
.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.align 4
.cc_top FUNCTION_NAME.function,FUNCTION_NAME

#define STK_CHUNKS  (NSTACKWORDS+1)
#define STK_LUT     (NSTACKWORDS+2)

#define sums        r0
#define diffs       r1
#define head        r2
#define tail        r3
#define chunks      r4
#define lut         r5
#define _32         r6


FUNCTION_NAME:
  dualentsp NSTACKWORDS
  std r4, r5, sp[0]
  std r6, r7, sp[1]

{ ldc _32, 32                 ; ldw chunks, sp[STK_CHUNKS]  }
{ shl r11, _32, 2             ; ldw lut, sp[STK_LUT]        }
{ mov r11, tail               ; vsetc r11                   }

.L_loop_top:
  { sub chunks, chunks, 1       ; vldr r11[0]                 }
  { add tail, tail, _32         ; vladsb head[0]              }
  { add head, head, _32         ; vstr sums[0]                }
  { mov r11, lut                ; vstd diffs[0]               }
  { add lut, lut, _32           ; vldr r11[0]                 }
  { add sums, sums, _32         ; vlmul diffs[0]              }
  { add diffs, diffs, _32       ; vstr diffs[0]               }
  { mov r11, tail               ; bt chunks, .L_loop_top      }
.L_loop_bot:

  ldd r6, r7, sp[1]
  ldd r4, r5, sp[0]
{ ldc r0, 31                ; vgetc r11                   }
{ zext r11, 5               ;                             }
{ sub r0, r0, r11           ; retsp NSTACKWORDS           }

	
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