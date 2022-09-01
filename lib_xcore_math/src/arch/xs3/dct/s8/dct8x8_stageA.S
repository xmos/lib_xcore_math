// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#if defined(__XS3A__)


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
#define NSTACKWORDS 34

.text
.issue_mode dual
.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.align 16
.cc_top FUNCTION_NAME.function,FUNCTION_NAME

#define STK_BUFF      (NSTACKWORDS - 32)
#define STK_LAST_ROW  (NSTACKWORDS - 4) // will point to last row of 16-bit buffered input matrix

#define y       r0
#define x       r1
#define mat     r2
#  define _16     mat
#define buff    r3
#define count   r4
#define _32     r5

// Because a 16-bit DCT matrix is used and 8-bit inputs, the maximum accumulator value is
// 2^24, and we don't want to output anything larger than 2^14 (otherwise dct8x8_part2()
// could saturate the accumulators) so we down-shift the accumulators 10 bits.
.L_sat_vec: .short 10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10

FUNCTION_NAME:
  dualentsp NSTACKWORDS
  std r4, r5, sp[0]
  
////// Expand to 16-bits

  ldc r11, 0x200 // 8-bit mode
{ ldaw buff, sp[STK_BUFF]     ; vsetc r11                   }
  ldaw r11, cp[vpu_vec_0x01]
{ ldc r11, 16                 ; vldc r11[0]                 }

{ ldc _32, 32                 ; vclrdr                      }
{ add count, x, r11           ; vlmacc x[0]                 }
{ add buff, buff, _32         ; vstr buff[0]                }

{                             ; vclrdr                      }
{ add count, count, r11       ; vlmacc count[0]             }
{ add buff, buff, _32         ; vstr buff[0]                }

{                             ; vclrdr                      }
{ add count, count, r11       ; vlmacc count[0]             }
{ add buff, buff, _32         ; vstr buff[0]                }

{                             ; vclrdr                      }
{                             ; vlmacc count[0]             }
{                             ; vstr buff[0]                }
  
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
#define sat  r1

  ldc r11, 0x100 // 16-bit mode
{                             ; vsetc r11                   }
  ldap r11, .L_sat_vec
{ ldc count, 4                ; mov sat, r11                }
{ ldc _16, 16                 ; mov r11, mat                } // NOTE: _16 and mat are the same register!
.L_loop_top:
  { add r11, r11, _32           ; vclrdr                      }
  { ldaw buff, sp[STK_LAST_ROW] ; vldc r11[0]                 }
  { sub buff, buff, _16         ; vlmaccr buff[0]             }
  { sub buff, buff, _16         ; vlmaccr buff[0]             }
  { sub buff, buff, _16         ; vlmaccr buff[0]             }
  { sub buff, buff, _16         ; vlmaccr buff[0]             }
  { sub buff, buff, _16         ; vlmaccr buff[0]             }
  { sub buff, buff, _16         ; vlmaccr buff[0]             }
  { sub buff, buff, _16         ; vlmaccr buff[0]             }
  { sub r11, r11, _32           ; vlmaccr buff[0]             }
  { ldaw buff, sp[STK_LAST_ROW] ; vldc r11[0]                 }
  { sub buff, buff, _16         ; vlmaccr buff[0]             }
  { sub buff, buff, _16         ; vlmaccr buff[0]             }
  { sub buff, buff, _16         ; vlmaccr buff[0]             }
  { sub buff, buff, _16         ; vlmaccr buff[0]             }
  { sub buff, buff, _16         ; vlmaccr buff[0]             }
  { sub buff, buff, _16         ; vlmaccr buff[0]             }
  { sub buff, buff, _16         ; vlmaccr buff[0]             }
  { sub count, count, 1         ; vlmaccr buff[0]             }
  { add r11, r11, _32           ; vlsat sat[0]                }
  { add r11, r11, _32           ; vstr y[0]                   }
  { add y, y, _32               ; bt count, .L_loop_top       }
.L_loop_bot:

  ldd r4, r5, sp[0]
  
{ ldc r0, 15                  ; vgetc r11                   }
{ zext r11, 5                 ;                             }
{ sub r0, r0, r11             ; retsp NSTACKWORDS           }

	
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