// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#if defined(__XS3A__)


/*  

void idct_adsb(
    int32_t sums[],
    int32_t diffs[],
    const int32_t head[],
    const int32_t tail[],
    const unsigned chunks);

*/

#define FUNCTION_NAME   idct_adsb
#define NSTACKWORDS 4

.text
.issue_mode dual
.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.align 4
.cc_top FUNCTION_NAME.function,FUNCTION_NAME

#define STK_CHUNKS  (NSTACKWORDS+1)

#define sums        r0
#define diffs       r1
#define s           r2
#define t_tilde     r3
#define chunks      r4
#define _32         r5


FUNCTION_NAME:
  dualentsp NSTACKWORDS
  std r4, r5, sp[0]
  std r6, r7, sp[1]

{ ldc _32, 32                 ; ldw chunks, sp[STK_CHUNKS]  }
{ ldc r11, 0                  ;                             }
{ mov r11, t_tilde            ; vsetc r11                   }

.L_loop_top:
  { sub chunks, chunks, 1       ; vldr r11[0]                 }
  { add r11, r11, _32           ; vladsb s[0]                 }
  { add s, s, _32               ; vstr sums[0]                }
  { add sums, sums, _32         ; vstd diffs[0]               }
  { add diffs, diffs, _32       ; bt chunks, .L_loop_top      }
.L_loop_bot:

  ldd r6, r7, sp[1]
  ldd r4, r5, sp[0]
  retsp NSTACKWORDS

	
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