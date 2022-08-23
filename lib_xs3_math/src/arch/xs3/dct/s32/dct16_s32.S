// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#if defined(__XS3A__)


/*  

Perform an 16-point forward DCT.

void dct16_forward(
    int32_t y[16],
    const int32_t x[16]);

*/

#define FUNCTION_NAME   dct16_forward
#define NSTACKWORDS 12

.text
.issue_mode dual
.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.align 4
.cc_top FUNCTION_NAME.function,FUNCTION_NAME

#define VEC_TMP       (NSTACKWORDS - 8)

#define y         r0
#define x         r1

#define a         r2
#define b         r3
#define c         r4
#define d         r5


FUNCTION_NAME:
  dualentsp NSTACKWORDS
  std r4, r5, sp[0]

  // Reverse the tail half of x[], placing it in y[]
  // leave the head half where it is
  ldd b, a, x[4]
  ldd d, c, x[7]
  std a, b, y[7]
  std c, d, y[4]

  ldd b, a, x[5]
  ldd d, c, x[6]
  std a, b, y[6]
  std c, d, y[5]

  ldc r11, 0x80
  
// Take the sum and difference of the head and (flipped) tail
// also dividing by 2 so that we don't saturate.
{ ldc a, 32                   ;                             }
{ add r11, y, a               ; vsetc r11                   }
{                             ; vldr r11[0]                 }
{ ldaw r11, sp[VEC_TMP]       ; vladsb x[0]                 }
{ add x, y, a                 ; vstd r11[0]                 }

#undef x  //no longer needed
#undef y
// now r0 points at the first half of y and r1 at the second half
#define left    r0
#define right   r1
{                             ; vstr left[0]                }
{                             ; vldr r11[0]                 }
  ldaw r11, cp[dct16_lut]
{ ldaw r11, sp[VEC_TMP]       ; vlmul r11[0]                }
{                             ; vstr r11[0]                 }


// DCT the the sum of the head and tail, placing the result in
// the second half of y[] (for now)
{ ldc b, 32                   ; vldc left[0]                }
  ldaw r11, cp[dct8_matrix]
{ mov a, r11                  ; vclrdr                      }
{ add r11, r11, b             ; vlmaccr r11[0]              }
{ add r11, r11, b             ; vlmaccr r11[0]              }
{ add r11, r11, b             ; vlmaccr r11[0]              }
{ add r11, r11, b             ; vlmaccr r11[0]              }
{ add r11, r11, b             ; vlmaccr r11[0]              }
{ add r11, r11, b             ; vlmaccr r11[0]              }
{ add r11, r11, b             ; vlmaccr r11[0]              }
{ add r11, r11, b             ; vlmaccr r11[0]              }
{                             ; vlsat r11[0]                } 
  ldaw r11, cp[vpu_vec_0x10000000] // ashr vR[] right 2 bits
{ mkmsk r11, 24               ; vlmul r11[0]                }
{                             ; vstr right[0]               }

// DCT the difference of head and tail, placing the result 
// on the stack
{ ldaw r11, sp[VEC_TMP]       ; vclrdr                      }
{ mov r11, a                  ; vldc r11[0]                 } // DCT right half (from stack vec)
{ add r11, r11, b             ; vlmaccr r11[0]              }
{ add r11, r11, b             ; vlmaccr r11[0]              }
{ add r11, r11, b             ; vlmaccr r11[0]              }
{ add r11, r11, b             ; vlmaccr r11[0]              }
{ add r11, r11, b             ; vlmaccr r11[0]              }
{ add r11, r11, b             ; vlmaccr r11[0]              }
{ add r11, r11, b             ; vlmaccr r11[0]              }
{ add r11, r11, b             ; vlmaccr r11[0]              }
{                             ; vlsat r11[0]                } 
  ldaw r11, cp[vpu_vec_0x20000000] // shr vR[] right 1 bit (to simplify deconvolution)
{ ldaw r11, sp[VEC_TMP]       ; vlmul r11[0]                }
{                             ; vstr r11[0]                 } // store on stack so we don't clobber
                                                              // anything when we interleave

// Now simultaneously rearrange stuff in memory while deconvolving the
// second DCT that we did
  ldd d, b, r11[0]
  ashr c, b, 1
{                             ; ldw a, right[0]             }
  std c, a, left[0]
{ sub c, d, c                 ; ldw a, right[1]             }
  std c, a, left[1]

  ldd d, b, r11[1]
{ sub c, b, c                 ; ldw a, right[2]             }
  std c, a, left[2]
{ sub c, d, c                 ; ldw a, right[3]             }
  std c, a, left[3]

  ldd d, b, r11[2]
{ sub c, b, c                 ; ldw a, right[4]             }
  std c, a, left[4]
{ sub c, d, c                 ; ldw a, right[5]             }
  std c, a, left[5]

  ldd d, b, r11[3]
{ sub c, b, c                 ; ldw a, right[6]             }
  std c, a, left[6]
{ sub c, d, c                 ; ldw a, right[7]             }
  std c, a, left[7]

  ldd r4, r5, sp[0]
{                             ; retsp NSTACKWORDS           }


	
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