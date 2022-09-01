// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#if defined(__XS3A__)


/*  

void vect_s32_flip(
    int32_t y[],
    const int32_t x[],
    const unsigned length);

*/

#define FUNCTION_NAME   vect_s32_flip
#define NSTACKWORDS 0

.text
.issue_mode dual
.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.align 16
.cc_top FUNCTION_NAME.function,FUNCTION_NAME

#define y       r0
#define x       r1
#define len     r2
#define a       r3
#define b       r11


FUNCTION_NAME:
  dualentsp NSTACKWORDS
{ sub x, x, 4                 ; sub y, y, 8                 }
.L_loop_top:
  { add y, y, 4                 ; ldw a, x[1]                 }
  { add x, x, 4                 ; ldw b, x[len]               }
    stw a, y[len]                       
  { sub len, len, 2             ; stw b, y[1]                 }
  {                             ; bt len, .L_loop_top         }
.L_loop_bottom:
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