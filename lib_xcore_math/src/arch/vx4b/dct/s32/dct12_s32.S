// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#if defined(__VX4B__)


/*  

Perform an 12-point forward DCT.

void dct12_forward(
    int32_t y[12],
    const int32_t x[12]);

*/

#define FUNCTION_NAME   dct12_forward
#define NSTACKWORDS 12

.text
.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.p2align 2

#define VEC_TMP       (NSTACKWORDS - 8 - 2)

#define y         a0
#define x         a1

#define a         a2
#define b         a3
#define c         s2
#define d         s3


FUNCTION_NAME:
xm.entsp (NSTACKWORDS)*4
xm.stdsp  s3,s2,0

// Reverse the tail half of x[], placing it in y[]
// leave the head half where it is
xm.lddi  a,b, 24(x)
xm.lddi  c,d, 40 (x)// Load these in case x and y are the same address
xm.stdi  b,a, 40(y)
xm.lddi  a,b, 32(x)
li t3, 0x80
xm.stdi  b,a, 32(y)
xm.stdi  d,c, 24(y)

// Take the sum and difference of the head and (flipped) tail
// also dividing by 2 so that we don't saturate.
{ li a, 24                              ; nop                                   }
{ add t3, y, a                          ; xm.vsetc t3                           }
{ nop                                   ; xm.vldr t3                            }
{ addi t3,sp, (VEC_TMP)*4               ; xm.vladsb x                           }
{ add x, y, a                           ; xm.vstd t3                            }

#undef x  //no longer needed
#undef y
// now a0 points at the first half of y and a1 at the second half
#define left    a0
#define right   a1
{ nop                                   ; xm.vstr left                          }
{ xm.mkmski a, 24                       ; xm.vldr t3                            }
la t3, dct12_lut
{ addi t3,sp, (VEC_TMP)*4               ; xm.vlmul0 t3                          }
xm.vstrpv t3, a

// DCT the the sum of the head and tail, placing the result in
// the second half of y[] (for now)
{ li b , 32                             ; xm.vldc left                          }
la t3, dct6_matrix
{ mv a, t3                              ; xm.vclrdr                             }
{ add t3, t3, b                         ; xm.vlmaccr0 t3                        }
{ add t3, t3, b                         ; xm.vlmaccr0 t3                        }
{ add t3, t3, b                         ; xm.vlmaccr0 t3                        }
{ add t3, t3, b                         ; xm.vlmaccr0 t3                        }
{ add t3, t3, b                         ; xm.vlmaccr0 t3                        }
{ nop                                   ; xm.vlmaccr0 t3                        }
la t3, vpu_vec_0x10000000 // ashr vR[] right 2 bits
{ xm.mkmski t3, 24                      ; xm.vlmul0 t3                          }
  xm.vstrpv right, t3 // put in right half so left half is clear
                       // when we start interleaving them

// DCT the difference of head and tail, placing the result 
// on the stack
{ addi t3,sp, (VEC_TMP)*4               ; xm.vclrdr                             }
{ mv t3, a                              ; xm.vldc t3                            } // DCT right half (from stack vec)
{ add t3, t3, b                         ; xm.vlmaccr0 t3                        }
{ add t3, t3, b                         ; xm.vlmaccr0 t3                        }
{ add t3, t3, b                         ; xm.vlmaccr0 t3                        }
{ add t3, t3, b                         ; xm.vlmaccr0 t3                        }
{ add t3, t3, b                         ; xm.vlmaccr0 t3                        }
{ nop                                   ; xm.vlmaccr0 t3                        }
la t3, vpu_vec_0x20000000 // shr vR[] right 1 bit (to simplify deconvolution)
{ addi t3,sp, (VEC_TMP)*4               ; xm.vlmul0 t3                          }
{ nop                                   ; xm.vstr t3                            } // store on stack so we don't clobber
                                                              // anything when we interleave

// Now simultaneously rearrange stuff in memory while deconvolving the
// second DCT that we did
xm.lddi  b,d, 0(t3)
srai b, b, 1
{ nop                                   ; lw a,0             ( right)           }
xm.stdi  a,b, 0(left)
{ sub d, d, b                           ; lw a,4             ( right)           }
xm.stdi  a,d, 8(left)
xm.lddi  b,c, 8(t3)
{ sub b, b, d                           ; lw a,8             ( right)           }
xm.stdi  a,b, 16(left)
{ sub c, c, b                           ; lw a,12             ( right)          }
xm.stdi  a,c, 24(left)
xm.lddi  b,d, 16   (t3)
{ sub b, b, c                           ; lw a,16             ( right)          }
xm.stdi  a,b, 32(left)
{ sub d, d, b                           ; lw a,20             ( right)          }
xm.stdi  a,d, 40(left)

xm.lddsp  s3,s2,0
{ nop                                   ; xm.retsp (NSTACKWORDS)*4              }

.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
.Ltmp0:
	.size	FUNCTION_NAME, .Ltmp0-FUNCTION_NAME    

#endif //defined(__VX4B__)
