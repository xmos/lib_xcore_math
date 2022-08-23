// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#if defined(__XS3A__)


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
#define NSTACKWORDS 2

.text
.issue_mode dual
.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.align 4
.cc_top FUNCTION_NAME.function,FUNCTION_NAME

#define y        r0
#define x        r1
#define chunks   r2
#define _32      r3
#define tmp      r4


FUNCTION_NAME:
  dualentsp NSTACKWORDS
  std r4, r5, sp[0]

  // This has to start at the end or else values will get clobbered
  // early if it's done in-place

  ldc r11, 0x80  // 32-bit mode with SHR=1 on VLADSB
{ shl r5, chunks, 3           ; vsetc r11                   }
{ ldc _32, 32                 ; ldw tmp, x[0]               }
  ldaw x, x[r5]
  ldaw y, y[r5]
{ sub x, x, _32               ; sub y, y, _32               }
{ sub r11, x, 4               ;                             }

.L_loop_top:
  { sub chunks, chunks, 1       ; vldr r11[0]                 }
  { sub x, x, _32               ; vladsb x[0]                 }
  { sub y, y, _32               ; vstr y[0]                   }
  { sub r11, x, 4               ; bt chunks, .L_loop_top      }
.L_loop_bot:
  
  stw tmp, y[8] // y is pointing 8 words before where it started
  
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