// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#if defined(__XS3A__)


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
.issue_mode dual
.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.align 16
.cc_top FUNCTION_NAME.function,FUNCTION_NAME

#define STK_BUFF      (NSTACKWORDS - 32)

#define y       r0
#define x       r1
#define mat     r2
#  define _32     mat
#define buff    r3
#define count   r4
#define A       r5
#define mask    r6
#define _16     r7
#define sat     r8

FUNCTION_NAME:
  dualentsp NSTACKWORDS
  std r4, r5, sp[0]
  std r6, r7, sp[1]
  std r8, r9, sp[2]
  
  ldc r11, 0x100 // 16-bit mode
{ ldc _16, 16                 ; vsetc r11                   }
{ add r3, r3, _16             ; add r11, r3, _16            }
  zip r11, r3, 4

// Store VLSAT argument vector in y[] (which won't be needed
// until after all VLSATs are done).
  std r3, r3, y[0]
  std r3, r3, y[1]
  std r3, r3, y[2]
  std r3, r3, y[3]

////// Perform eight 8-point, 16-bit DCTs

// We'll place the result on the stack as 16-bit values because it
// will be faster than switching between modes while DCTing.
// We'll again do the transpose in-flight.
// The stack space doesn't matter because stageA uses the same amount

{ ldc count, 4                ; ldaw buff, sp[STK_BUFF]     }
{                             ; ldc r11, 28                 }
// We need to traverse the rows of x[] backwards to get elements
// in the right output order.
  ldaw x, x[r11]
{ ldc _32, 32                 ; mov r11, mat                } // NOTE: _32 and mat are the same register!

.L_loop_top:
  { add r11, r11, _32           ; vclrdr                      }
  { mov A, x                    ; vldc r11[0]                 }
  { sub A, A, _16               ; vlmaccr A[0]                }
  { sub A, A, _16               ; vlmaccr A[0]                }
  { sub A, A, _16               ; vlmaccr A[0]                }
  { sub A, A, _16               ; vlmaccr A[0]                }
  { sub A, A, _16               ; vlmaccr A[0]                }
  { sub A, A, _16               ; vlmaccr A[0]                }
  { sub A, A, _16               ; vlmaccr A[0]                }
  { sub r11, r11, _32           ; vlmaccr A[0]                }
  { mov A, x                    ; vldc r11[0]                 }
  { sub A, A, _16               ; vlmaccr A[0]                }
  { sub A, A, _16               ; vlmaccr A[0]                }
  { sub A, A, _16               ; vlmaccr A[0]                }
  { sub A, A, _16               ; vlmaccr A[0]                }
  { sub A, A, _16               ; vlmaccr A[0]                }
  { sub A, A, _16               ; vlmaccr A[0]                }
  { sub A, A, _16               ; vlmaccr A[0]                }
  { add r11, r11, _32           ; vlmaccr A[0]                }
  { add r11, r11, _32           ; vlsat y[0]                  }
  { sub count, count, 1         ; vstr buff[0]                }
  { add buff, buff, _32         ; bt count, .L_loop_top       }
.L_loop_bot:

// We could get the headroom right now on the 16-bit values, but
// there's a chance that VDEPTH8 causes a value to round away from
// zero in a way that decreases headroom.

// Reduce depth to 8 bits, moving to y[].
{ ldaw r11, sp[STK_BUFF]        ;                             }
{ add r11, r11, _32             ; vldr r11[0]                 }
{                               ; vdepth8                     }
{ add y, y, _16                 ; vstr y[0]                   }
{ add r11, r11, _32             ; vldr r11[0]                 }
{                               ; vdepth8                     }
{ add y, y, _16                 ; vstr y[0]                   }
{ add r11, r11, _32             ; vldr r11[0]                 }
{ mkmsk mask, 16                ; vdepth8                     }
{ add y, y, _16                 ; vstr y[0]                   }
{ shl r11, _32, 4 /*8-bit mode*/; vldr r11[0]                 }
{ add _16, _32, _16             ; vdepth8                     }
  vstrpv y[0], mask

// Load/store one last time to get headroom
{ sub y, y, _16                 ; vsetc r11                   }
{                               ; vldd y[0]                   }
{ add y, y, _32                 ; vstd y[0]                   }
{                               ; vldd y[0]                   }
{                               ; vstd y[0]                   }

  ldd r4, r5, sp[0]
  ldd r6, r7, sp[1]
  ldd r8, r9, sp[2]

{ ldc r0, 7                   ; vgetc r11                   }
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