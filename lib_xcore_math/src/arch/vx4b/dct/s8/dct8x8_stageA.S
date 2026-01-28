// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#if defined(__VX4B__)


/*  

Perform the first step of a 2D 8-by-8 forward or inverse DCT on 8-bit data.

The first step takes an 8-bit tensor x[8][8] as input and populates a 16-bit
tensor y[8][8] as output.

The operation is to perform an 8-point DCT on each row of x[][] to get
an intermediate tensor tmp[][], and then populate y[][] with the TRANSPOSE of
tmp[][].

Whether the forward or inverse DCT is performed depends on whether the
dct_matrix[][] argument points to dct8_matrix_16bit[][] or 
idct8_matrix_16bit[][].

headroom_t dct8x8_stageA(
    int16_t y[8][8],
    const int8_t x[8][8],
    const int16_t matrix[8][16]);

*/

#define FUNCTION_NAME   dct8x8_stageA
#define NSTACKWORDS 36

.text
.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.p2align 4

#define STK_BUFF      (NSTACKWORDS - 32-1)
#define STK_LAST_ROW  (NSTACKWORDS - 4-1) // will point to last row of 16-bit buffered input matrix

#define y       x10
#define x       x11
#define mat     x12
#  define _16     mat
#define buff    x13
#define count   x18
#define _32     x19

// Because a 16-bit DCT matrix is used and 8-bit inputs, the maximum accumulator value is
// 2^24, and we don't want to output anything larger than 2^14 (otherwise dct8x8_part2()
// could saturate the accumulators) so we down-shift the accumulators 10 bits.
.L_sat_vec: .short 10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10 /* Translation error on this line: unexpected token at position 12. */ 

FUNCTION_NAME:
  xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
  xm.stdsp  s3,s2,0
  
////// Expand to 16-bits

  li t3, 0x200 // 8-bit mode
{ addi buff,sp, (STK_BUFF)*4     ; xm.vsetc t3}
lui t3, %hi(vpu_vec_0x01)
  addi t3,t3, %lo(vpu_vec_0x01)
{ li t3, 16                 ; xm.vldc t3}

{ li _32, 32                 ; xm.vclrdr                      }
{ add count, x, t3           ; xm.vlmacc0 x}
{ add buff, buff, _32         ; xm.vstr buff}

{ nop                             ; xm.vclrdr                      }
{ add count, count, t3       ; xm.vlmacc0 count}
{ add buff, buff, _32         ; xm.vstr buff}

{ nop                             ; xm.vclrdr                      }
{ add count, count, t3       ; xm.vlmacc0 count}
{ add buff, buff, _32         ; xm.vstr buff}

{ nop                             ; xm.vclrdr                      }
{ nop                             ; xm.vlmacc0 count}
{ nop                             ; xm.vstr buff}
  
////// Perform eight 8-point, 16-bit DCTs

// The trick here is that we'll transpose while computing the
// output. Instead of loading the row from x[] into vC[], we'll
// load a row from the DCT matrix, and each vlmaccr will apply
// to a different row of x[].
// Then when we saturate and store that in y[], we'll have
// what would have been the first COLUMN of output as the first
// ROW of output.

// The other catch is that the data needs to be masked to avoid
// including the wrong stuff in the accumulators. This is easily
// handled by just padding the matrix with 0's (then it will be
// the same size as the 32-bit DCT8 matrix).

// Finally, we'll compute two rows of output per loop iteration,
// since we have enough accumulators to do so.

// (also, we don't need the original x[] pointer anymore, so we'll
//  put something else in there)
#undef x
#define sat  x11

  li t3, 0x100 // 16-bit mode
{ nop                             ; xm.vsetc t3}
lui t3, %hi(.L_sat_vec)
  addi t3,t3, %lo(.L_sat_vec)
{ li count, 4                ; mv sat, t3                }
{ li _16, 16                 ; mv t3, mat                } // NOTE: _16 and mat are the same register!
.L_loop_top:
  { add t3, t3, _32           ; xm.vclrdr                      }
  { addi buff,sp, (STK_LAST_ROW)*4 ; xm.vldc t3}
  
  { nop                         ; xm.vlmaccr0 buff}
  xm.vlmaccr1 buff
  { sub buff, buff, _16         ; nop}
  { nop                         ; xm.vlmaccr0 buff}
  xm.vlmaccr1 buff
  { sub buff, buff, _16         ; nop}
  { nop                         ; xm.vlmaccr0 buff}
  xm.vlmaccr1 buff
  { sub buff, buff, _16         ; nop}
  { nop                         ; xm.vlmaccr0 buff}
  xm.vlmaccr1 buff
  { sub buff, buff, _16         ; nop}
  { nop                         ; xm.vlmaccr0 buff}
  xm.vlmaccr1 buff
  { sub buff, buff, _16         ; nop}
  { nop                         ; xm.vlmaccr0 buff}
  xm.vlmaccr1 buff
  { sub buff, buff, _16         ; nop}
  { nop                         ; xm.vlmaccr0 buff}
  xm.vlmaccr1 buff
  { sub buff, buff, _16         ; nop}
  { sub t3, t3, _32           ; xm.vlmaccr0 buff}
  xm.vlmaccr1 buff
  { addi buff,sp, (STK_LAST_ROW)*4 ; xm.vldc t3}
  { nop                         ; xm.vlmaccr0 buff}
  xm.vlmaccr1 buff
  { sub buff, buff, _16         ; nop}
  { nop                         ; xm.vlmaccr0 buff}
  xm.vlmaccr1 buff
  { sub buff, buff, _16         ; nop}
  { nop                         ; xm.vlmaccr0 buff}
  xm.vlmaccr1 buff
  { sub buff, buff, _16         ; nop}
  { nop                         ; xm.vlmaccr0 buff}
  xm.vlmaccr1 buff
  { sub buff, buff, _16         ; nop}
  { nop                         ; xm.vlmaccr0 buff}
  xm.vlmaccr1 buff
  { sub buff, buff, _16         ; nop}
  { nop                         ; xm.vlmaccr0 buff}
  xm.vlmaccr1 buff
  { sub buff, buff, _16         ; nop}
  { nop                         ; xm.vlmaccr0 buff}
  xm.vlmaccr1 buff
  { sub buff, buff, _16         ; nop}
  
  { addi count, count, -1         ; xm.vlmaccr0 buff}
  xm.vlmaccr1 buff
  { add t3, t3, _32           ; nop}
   xm.vlsat sat
  { add t3, t3, _32           ; xm.vstr y}
  { add y, y, _32               ; nop}
  bnez count, .L_loop_top   
.L_loop_bot:

  xm.lddsp  s3,s2,0
  
{ li a0, 15                  ; xm.vgetc t3}
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
