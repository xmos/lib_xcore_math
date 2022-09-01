// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#if defined(__XS3A__)


/*  



void idct_scale(
    int32_t x[],
    const int32_t idct_lut[],
    const unsigned chunks,
    const right_shift_t shr);

*/

#define FUNCTION_NAME   idct_scale
#define NSTACKWORDS 2

.text
.issue_mode dual
.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.align 4
.cc_top FUNCTION_NAME.function,FUNCTION_NAME


#define x        r0
#define lut      r1
#define chunks   r2
#define shr      r3
#define _32      r4


FUNCTION_NAME:
  dualentsp NSTACKWORDS
  std r4, r5, sp[0]

{ ldc r11, 0                  ; ldc _32, 32                 }
{ mov r11, lut                ; vsetc r11                   }

.L_loop_top:
  { add r11, r11, _32           ; vldr r11[0]                 }
  {                             ; vlmul x[0]                  }
  {                             ; vstr x[0]                   }
    vlashr x[0], shr
  { sub chunks, chunks, 1       ; vstr x[0]                   }
  { add x, x, _32               ; bt chunks, .L_loop_top      }
.L_loop_bot:
  
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
