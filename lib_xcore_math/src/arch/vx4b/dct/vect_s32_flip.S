// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#if defined(__VX4B__)

/*  
void vect_s32_flip(
    int32_t y[],
    const int32_t x[],
    const unsigned length);
*/

#define FUNCTION_NAME   vect_s32_flip
#define NSTACKWORDS 0

.text
.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.p2align 4

#define y       a0
#define x       a1
#define len     a2
#define a       a3
#define b       t3


FUNCTION_NAME:
xm.entsp (NSTACKWORDS)*4
{ addi x, x, -4                         ; addi y, y, -8                         }
  .L_loop_top:
  { addi y, y, 4                        ; lw a,4                 ( x)           }
  { addi x, x, 4                        ; xm.ldw b, len          ( x)           }
  xm.stw a,len(y) 
  { addi len, len, -2                   ; sw b,4                 ( y)           }
  { nop                                 ; xm.bt len, .L_loop_top                }
.L_loop_bottom:
xm.retsp (NSTACKWORDS)*4  
	
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


#endif //defined(__VX4B__)
