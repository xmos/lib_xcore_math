// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#if defined(__XS3A__)


/*  

length must be a multiple of 8

void dct_deconvolve_s32(
    int32_t res[],
    const int32_t B[],
    const int32_t D[],
    const unsigned length);

*/

#define FUNCTION_NAME   dct_deconvolve_s32
#define NSTACKWORDS 4

.text
.issue_mode dual
.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.align 4
.cc_top FUNCTION_NAME.function,FUNCTION_NAME

#define res     r0
#define B       r1
#define D       r2
#define len     r3
#define even    r4
#define a       r5
#define b       r6
#define c       r7


FUNCTION_NAME:
  dualentsp NSTACKWORDS
  std r4, r5, sp[0]
  std r6, r7, sp[1]

// Just set it up so that c contains half D[0], so that when
// it's subtracted from D[0] we get (D[0] >> 1)
  { shr len, len, 3             ; ldw c, D[0]                 }
    ashr c, c, 1
  { ldc r11, 16                 ; bu .L_loop_top              }

.align 16
.L_loop_top:
    ldd b, a, D[0]
  { sub a, a, c                 ; ldw even, B[0]              }
    std a, even, res[0]
  { sub b, b, a                 ; ldw even, B[1]              }
    std b, even, res[1]
    ldd c, a, D[1]
  { sub a, a, b                 ; ldw even, B[2]              }
    std a, even, res[2]
  { sub c, c, a                 ; ldw even, B[3]              }
    std c, even, res[3]
  { add D, D, r11               ; sub len, len, 1             }
  { add res, res, r11           ; add B, B, r11               }
  { add res, res, r11           ; bt len, .L_loop_top         }
.L_loop_bot:

.L_finish:
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