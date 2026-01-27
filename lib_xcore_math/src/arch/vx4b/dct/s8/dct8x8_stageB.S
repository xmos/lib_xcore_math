// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#if defined(__VX4B__)


/*  

Perform the final step of a 2D 8-by-8 forward or inverse DCT on 8-bit data.

The first step takes an 8-bit tensor x[8][8] as input and populates a 16-bit
tensor y[8][8] as output. The first step is implemented as dct8x8_stageA().

The final step takes a 16-bit tensor x[8][8] as input and populates an 8-bit
tensor y[8][8] as output.

The operation is to perform an 8-point DCT on each row of x[][] to get
an intermediate tensor tmp[][], and then populate y[][] with the TRANSPOSE of
tmp[][].

Whether the forward or inverse DCT is performed depends on whether the
matrix[][] argument points to dct8_matrix_16bit[][] or 
idct8_matrix_16bit[][].

headroom_t dct8_inversex8_stageB(
    int8_t y[8][8],
    const int16_t x[8][8],
    const int16_t matrix[8][16],
    const right_shift_t sat);

*/

#define FUNCTION_NAME   dct8x8_stageB
#define NSTACKWORDS 40

.text
.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.p2align 4

#define STK_BUFF      (NSTACKWORDS - 32)

#define y       x10
#define x       x11
#define mat     x12
#  define _32     mat
#define buff    x13
#define count   x18
#define A       x19
#define mask    x20
#define _16     x21
#define sat     x22

FUNCTION_NAME:
  xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
  xm.stdsp  s3,s2,0
  xm.stdsp  s5,s4,8
  xm.stdsp  s7,s6,16
  
  li t3, 0x100 // 16-bit mode
{ li _16, 16                 ; xm.vsetc t3}
{ add a3, a3, _16             ; add t3, a3, _16            }
xm.zip t3, a3, 4

// Store VLSAT argument vector in y[] (which won't be needed
// until after all VLSATs are done).
  xm.stdi  a3,a3, 0(y)
  xm.stdi  a3,a3, 8(y)
  xm.stdi  a3,a3, 16(y)
  xm.stdi  a3,a3, 24(y)

////// Perform eight 8-point, 16-bit DCTs

// We'll place the result on the stack as 16-bit values because it
// will be faster than switching between modes while DCTing.
// We'll again do the transpose in-flight.
// The stack space doesn't matter because stageA uses the same amount

{ li count, 4                ; addi buff,sp, (STK_BUFF)*4     }
{ nop                             ; li t3, 28                 }
// We need to traverse the rows of x[] backwards to get elements
// in the right output order.
  sh2add x, t3, x
{ li _32, 32                 ; mv t3, mat                } // NOTE: _32 and mat are the same register!

.L_loop_top:
  { add t3, t3, _32           ; xm.vclrdr                      }
  { mv A, x                    ; xm.vldc t3}
  /*
  { sub A, A, _16               ; xm.vlmaccr A}
  { sub A, A, _16               ; xm.vlmaccr A}
  { sub A, A, _16               ; xm.vlmaccr A}
  { sub A, A, _16               ; xm.vlmaccr A}
  { sub A, A, _16               ; xm.vlmaccr A}
  { sub A, A, _16               ; xm.vlmaccr A}
  { sub A, A, _16               ; xm.vlmaccr A}
  { sub t3, t3, _32           ; xm.vlmaccr A}
  { mv A, x                    ; xm.vldc t3}
  { sub A, A, _16               ; xm.vlmaccr A}
  { sub A, A, _16               ; xm.vlmaccr A}
  { sub A, A, _16               ; xm.vlmaccr A}
  { sub A, A, _16               ; xm.vlmaccr A}
  { sub A, A, _16               ; xm.vlmaccr A}
  { sub A, A, _16               ; xm.vlmaccr A}
  { sub A, A, _16               ; xm.vlmaccr A}
    */
  { add t3, t3, _32           ; xm.vlmaccr0 A}
  xm.vlmaccr1 A
  { add t3, t3, _32           ; nop}
  xm.vlsat y
  { addi count, count, -1         ; xm.vstr buff}
  { add buff, buff, _32         ; xm.bt count, .L_loop_top       }
.L_loop_bot:

// We could get the headroom right now on the 16-bit values, but
// there's a chance that VDEPTH8 causes a value to round away from
// zero in a way that decreases headroom.

// Reduce depth to 8 bits, moving to y[].
{ addi t3,sp, (STK_BUFF)*4        ; nop                             }
{ add t3, t3, _32             ; xm.vldr t3}
{ nop                               ; xm.vdepth8                     }
{ add y, y, _16                 ; xm.vstr y}
{ add t3, t3, _32             ; xm.vldr t3}
{ nop                               ; xm.vdepth8                     }
{ add y, y, _16                 ; xm.vstr y}
{ add t3, t3, _32             ; xm.vldr t3}
{ xm.mkmski mask, 16                ; xm.vdepth8                     }
{ add y, y, _16                 ; xm.vstr y}
{ xm.shli t3, _32, 4 /*8-bit mode*/; xm.vldr t3}
{ add _16, _32, _16             ; xm.vdepth8                     }
  xm.vstrpv y, mask

// Load/store one last time to get headroom
{ sub y, y, _16                 ; xm.vsetc t3}
{ nop                               ; xm.vldd y}
{ add y, y, _32                 ; xm.vstd y}
{ nop                               ; xm.vldd y}
{ nop                               ; xm.vstd y}

  xm.lddsp  s3,s2,0
  xm.lddsp  s5,s4,8
  xm.lddsp  s7,s6,16

{ li a0, 7                   ; xm.vgetc t3}
{ xm.zexti t3, 5                 ; nop                             }
{ sub a0, a0, t3             ; xm.retsp (NSTACKWORDS)*4           }

	
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
