// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#if defined(__XS3A__)


/*  

Perform an 24-point forward DCT.

Computed directly by multiplying by the DCT matrix. The output has elements ordered 
so that when used in recursive DCT computation the bit-reversed indexing can be used
to deconvolve those that need it.

void dct24_forward(
    int32_t y[24],
    const int32_t x[24]);

*/

#define FUNCTION_NAME   dct24_forward
#define NSTACKWORDS 40

.text
.issue_mode dual
.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.align 4
.cc_top FUNCTION_NAME.function,FUNCTION_NAME

#define STK_VEC_HEAD       (NSTACKWORDS - 16)
#define STK_VEC_TAIL       (NSTACKWORDS - 8)

#define y         r0
#define x         r1

#define a         r2
#define b         r3
#define c         r4
#define d         r5


FUNCTION_NAME:
  dualentsp NSTACKWORDS
  std r4, r5, sp[1]

  // Reverse the tail half of x[], placing it in y[]
  // leave the head half where it is
  ldd b, a, x[6]
  ldd d, c, x[11]
  std a, b, y[11]
  std c, d, y[6]
  
  ldd b, a, x[7]
  ldd d, c, x[10]
  std a, b, y[10]
  std c, d, y[7]

  ldc r11, 0x80

  ldd b, a, x[8]
  ldd d, c, x[9]
  std a, b, y[9]
  std c, d, y[8]

// Take the sum and difference between the head and (flipped) tail
// the sum goes into y[0:12], the difference goes into tmp[0:12]
{ ldc a, 48                   ; ldaw d, sp[STK_VEC_HEAD]    }
{ add r11, y, a               ; vsetc r11                   } // r11 <-- &y[12]
{ ldc b, 32                   ; vldr r11[0]                 } // vR[] <-- y[12:20]
{                             ; vladsb x[0]                 } // vR[] <-- sum; vD[] <-- diff
{                             ; vstd d[0]                   } // tmp[0:8] <-- diff[0:8]
{ add r11, r11, b             ; vstr y[0]                   } // y[0:8] <-- sum[0:8]
{ add x, x, b                 ; vldr r11[0]                 } // vR[] <-- y[20:24]
{ ldaw r11, sp[STK_VEC_TAIL]  ; vladsb x[0]                 } // sum/diff; orig x no longer needed
{ add r11, y, b               ; vstd r11[0]                 } // tmp[8:12] <-- diff[8:12]
{ add x, y, a                 ; vstr r11[0]                 } // y[8:12] <-- sum[8:12]

// multiply tail component by DCT LUT
  ldaw r11, cp[dct24_lut]
{                             ; vldr r11[0]                 }
{ add a, d, b                 ; vlmul d[0]                  }
{ add r11, r11, b             ; vstr d[0]                   }
{                             ; vldr r11[0]                 }
{                             ; vlmul a[0]                  }
{                             ; vstr a[0]                   }

#define left    r0  // Contains &y[0]
#define right   r1  // Contains &y[12]

// perform 12-point DCTs on the head and tail sub-sequences.
//  y[0:12] (head) --> DCT12 --> y[12:24]
//  tmp[0:12] (tail) --> DCT12 --> tmp[0:12]
// The head is being moved to the end of y so that it isn't in
// the way when we need to do deconvolution
  std r0, r1, sp[2]
  std r2, r3, sp[3]

// DCT12(head[])
{ mov r0, right               ; mov r1, left                }
  ldap r11, dct12_forward
{                             ; bla r11                     }
// DCT12(tail[])
{ ldaw r0, sp[STK_VEC_HEAD]   ; ldaw r1, sp[STK_VEC_HEAD]   }
  ldap r11, dct12_forward   
{                             ; bla r11                     }
  ldd r0, r1, sp[2]
  ldd r2, r3, sp[3]

// Before deconvolution, right-shift the head vector 2 bits, and 
// right-shift the tail vector 1 bit
{ ldc a, 1                    ; mkmsk c, 16                 }
  vlashr d[0], a
{ ldaw r11, sp[STK_VEC_TAIL]  ; vstr d[0]                   }
  vlashr r11[0], a
{ ldc a, 2                    ; vstr r11[0]                 }
  vlashr right[0], a
{ add r11, right, b           ; vstr right[0]               }
  vlashr r11[0], a
  vstrpv r11[0], c

// Finally, begin deconvolving and interleaving

{ mov r11, d                  ;                             }
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
  
  ldd c, b, r11[3]
{ sub b, b, d                 ; ldw a, right[6]             }
  std b, a, left[6]
{ sub c, c, b                 ; ldw a, right[7]             }
  std c, a, left[7]
  
  ldd d, b, r11[4]
{ sub b, b, c                 ; ldw a, right[8]             }
  std b, a, left[8]
{ sub d, d, b                 ; ldw a, right[9]             }
  std d, a, left[9]
  
  ldd c, b, r11[5]
{ sub b, b, d                 ; ldw a, right[10]             }
  std b, a, left[10]
{ sub c, c, b                 ; ldw a, right[11]             }
  std c, a, left[11]
  
  ldd r4, r5, sp[1]
{                             ; retsp NSTACKWORDS           }


	
.cc_bottom FUNCTION_NAME.function
.set	FUNCTION_NAME.nstackwords,(NSTACKWORDS+12)
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