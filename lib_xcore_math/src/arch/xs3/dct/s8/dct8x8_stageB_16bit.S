// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#if defined(__XS3A__)


/*  

Perform the final step of a 2D 8-by-8 forward or inverse DCT on 8-bit data.

The first step takes an 8-bit tensor x[8][8] as input and populates a 16-bit
tensor y[8][8] as output. The first step is implemented as dct8x8_stageA().

The final step takes a 16-bit tensor x[8][8] as input and populates an 16-bit
tensor y[8][8] as output.

The operation is to perform an 8-point DCT on each row of x[][] to get
an intermediate tensor tmp[][], and then populate y[][] with the TRANSPOSE of
tmp[][].

Whether the forward or inverse DCT is performed depends on whether the
matrix[][] argument points to dct8_matrix_16bit[][] or 
idct8_matrix_16bit[][].

void dct8x8_stageB(
    int16_t y[8][8],
    const int16_t x[8][8],
    const int16_t matrix[8][16]);

*/

#define FUNCTION_NAME   dct8x8_stageB_16bit
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
#define buff    r3
#define count   r4
#define _32     r5
#define _16     r6
#define sat     r7

.L_vpu_vec_0x0010:
.short 20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20

FUNCTION_NAME:
  dualentsp NSTACKWORDS
  std r4, r5, sp[0]
  std r6, r7, sp[1]
  
  ldc r11, 0x100 // 16-bit mode
{ ldc _16, 16                 ; vsetc r11                   }
  ldap r11, .L_vpu_vec_0x0010
{ mov sat, r11                ;                             }

////// Perform eight 8-point, 16-bit DCTs

{ ldc count, 4                ; ldc r11, 28                 }
// We need to traverse the rows of x[] backwards to get elements
// in the right output order.
  ldaw x, x[r11]
{ ldc _32, 32                 ;                             }

// Each loop handles 2 rows of the matrix, in the order: (1,0),(3,2),(5,4),(7,6)
// so that later matrix rows end up in higher accumulator indices, and thus higher memory
// when stored. 
.L_loop_top:
  { add mat, mat, _32           ; vclrdr                      }
  { mov r11, x                  ; vldc mat[0]                 }
  { sub r11, r11, _16           ; vlmaccr r11[0]              }
  { sub r11, r11, _16           ; vlmaccr r11[0]              }
  { sub r11, r11, _16           ; vlmaccr r11[0]              }
  { sub r11, r11, _16           ; vlmaccr r11[0]              }
  { sub r11, r11, _16           ; vlmaccr r11[0]              }
  { sub r11, r11, _16           ; vlmaccr r11[0]              }
  { sub r11, r11, _16           ; vlmaccr r11[0]              }
  { sub mat, mat, _32           ; vlmaccr r11[0]              }
  { mov r11, x                  ; vldc mat[0]                 }
  { sub r11, r11, _16           ; vlmaccr r11[0]              }
  { sub r11, r11, _16           ; vlmaccr r11[0]              }
  { sub r11, r11, _16           ; vlmaccr r11[0]              }
  { sub r11, r11, _16           ; vlmaccr r11[0]              }
  { sub r11, r11, _16           ; vlmaccr r11[0]              }
  { sub r11, r11, _16           ; vlmaccr r11[0]              }
  { sub r11, r11, _16           ; vlmaccr r11[0]              }
  { add mat, mat, _32           ; vlmaccr r11[0]              }
  { add mat, mat, _32           ; vlsat sat[0]                }
  { sub count, count, 1         ; vstr y[0]                   }
  { add y, y, _32               ; bt count, .L_loop_top       }
.L_loop_bot:

  ldd r4, r5, sp[0]
  ldd r6, r7, sp[1]

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