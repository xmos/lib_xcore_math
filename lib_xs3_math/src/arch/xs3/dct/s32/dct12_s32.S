// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#if defined(__XS3A__)


/*  

Perform an 12-point forward DCT.

void dct12_forward(
    int32_t y[12],
    const int32_t x[12]);

*/

#define FUNCTION_NAME   dct12_forward
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
  ldd b, a, x[3]
  ldd d, c, x[5] // Load these in case x and y are the same address
  std a, b, y[5]
  ldd b, a, x[4]
  ldc r11, 0x80
  std a, b, y[4]
  std c, d, y[3]

// Take the sum and difference of the head and (flipped) tail
// also dividing by 2 so that we don't saturate.
{ ldc a, 24                   ;                             }
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
{ mkmsk a, 24                 ; vldr r11[0]                 }
  ldaw r11, cp[dct12_lut]
{ ldaw r11, sp[VEC_TMP]       ; vlmul r11[0]                }
  vstrpv r11[0], a


// DCT the the sum of the head and tail, placing the result in
// the second half of y[] (for now)
{ ldc b , 32                  ; vldc left[0]                }
  ldaw r11, cp[dct6_matrix]
{ mov a, r11                  ; vclrdr                      }
{ add r11, r11, b             ; vlmaccr r11[0]              }
{ add r11, r11, b             ; vlmaccr r11[0]              }
{ add r11, r11, b             ; vlmaccr r11[0]              }
{ add r11, r11, b             ; vlmaccr r11[0]              }
{ add r11, r11, b             ; vlmaccr r11[0]              }
{                             ; vlmaccr r11[0]              }
  ldaw r11, cp[vpu_vec_0x10000000] // ashr vR[] right 2 bits
{ mkmsk r11, 24               ; vlmul r11[0]                }
  vstrpv right[0], r11 // put in right half so left half is clear
                       // when we start interleaving them

// DCT the difference of head and tail, placing the result 
// on the stack
{ ldaw r11, sp[VEC_TMP]       ; vclrdr                      }
{ mov r11, a                  ; vldc r11[0]                 } // DCT right half (from stack vec)
{ add r11, r11, b             ; vlmaccr r11[0]              }
{ add r11, r11, b             ; vlmaccr r11[0]              }
{ add r11, r11, b             ; vlmaccr r11[0]              }
{ add r11, r11, b             ; vlmaccr r11[0]              }
{ add r11, r11, b             ; vlmaccr r11[0]              }
{                             ; vlmaccr r11[0]              }
  ldaw r11, cp[vpu_vec_0x20000000] // shr vR[] right 1 bit (to simplify deconvolution)
{ ldaw r11, sp[VEC_TMP]       ; vlmul r11[0]                }
{                             ; vstr r11[0]                 } // store on stack so we don't clobber
                                                              // anything when we interleave

// Now simultaneously rearrange stuff in memory while deconvolving the
// second DCT that we did
  ldd d, b, r11[0]
  ashr b, b, 1
{                             ; ldw a, right[0]             }
  std b, a, left[0]
{ sub d, d, b                 ; ldw a, right[1]             }
  std d, a, left[1]
  ldd c, b, r11[1]
{ sub b, b, d                 ; ldw a, right[2]             }
  std b, a, left[2]
{ sub c, c, b                 ; ldw a, right[3]             }
  std c, a, left[3]
  ldd d, b, r11[2]   
{ sub b, b, c                 ; ldw a, right[4]             }
  std b, a, left[4]
{ sub d, d, b                 ; ldw a, right[5]             }
  std d, a, left[5]
  
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