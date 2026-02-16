// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#if defined(__VX4B__)


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
#define NSTACKWORDS 44

.text
.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.p2align 2

#define STK_VEC_HEAD       (NSTACKWORDS - 16-2)
#define STK_VEC_TAIL       (NSTACKWORDS - 8-2)

#define y         a0
#define x         a1

#define a         a2
#define b         a3
#define c         s2
#define d         s3


FUNCTION_NAME:
  xm.entsp (NSTACKWORDS)*4
  xm.stdsp  s3,s2,8

  // Reverse the tail half of x[], placing it in y[]
  // leave the head half where it is
  xm.lddi  a,b, 48(x)
  xm.lddi  c,d, 88(x)
  xm.stdi  b,a, 88(y)
  xm.stdi  d,c, 48(y)
  
  xm.lddi  a,b, 56(x)
  xm.lddi  c,d, 80(x)
  xm.stdi  b,a, 80(y)
  xm.stdi  d,c, 56(y)

  li t3, 0x80

  xm.lddi  a,b, 64(x)
  xm.lddi  c,d, 72(x)
  xm.stdi  b,a, 72(y)
  xm.stdi  d,c, 64(y)

// Take the sum and difference between the head and (flipped) tail
// the sum goes into y[0:12], the difference goes into tmp[0:12]
{ li a, 48                              ; addi d,sp, (STK_VEC_HEAD)*4           }
{ add t3, y, a                          ; xm.vsetc t3                           } // t3 <-- &y[12]
{ li b, 32                              ; xm.vldr t3                            } // vR[] <-- y[12:20]
{ nop                                   ; xm.vladsb x                           } // vR[] <-- sum; vD[] <-- diff
{ nop                                   ; xm.vstd d                             } // tmp[0:8] <-- diff[0:8]
{ add t3, t3, b                         ; xm.vstr y                             } // y[0:8] <-- sum[0:8]
{ add x, x, b                           ; xm.vldr t3                            } // vR[] <-- y[20:24]
{ addi t3,sp, (STK_VEC_TAIL)*4          ; xm.vladsb x                           } // sum/diff; orig x no longer needed
{ add t3, y, b                          ; xm.vstd t3                            } // tmp[8:12] <-- diff[8:12]
{ add x, y, a                           ; xm.vstr t3                            } // y[8:12] <-- sum[8:12]

// multiply tail component by DCT LUT
la t3, dct24_lut
{ nop                                   ; xm.vldr t3                            }
{ add a, d, b                           ; xm.vlmul0 d                           }
{ add t3, t3, b                         ; xm.vstr d                             }
{ nop                                   ; xm.vldr t3                            }
{ nop                                   ; xm.vlmul0 a                           }
{ nop                                   ; xm.vstr a                             }

#define left    a0  // Contains &y[0]
#define right   a1  // Contains &y[12]

// perform 12-point DCTs on the head and tail sub-sequences.
//  y[0:12] (head) --> DCT12 --> y[12:24]
//  tmp[0:12] (tail) --> DCT12 --> tmp[0:12]
// The head is being moved to the end of y so that it isn't in
// the way when we need to do deconvolution
  xm.stdsp  a1,a0,16
  xm.stdsp  a3,a2,24

// DCT12(head[])
{ mv a0, right                          ; mv a1, left                           }
la t3, dct12_forward
{ nop                                   ; jalr t3                               }
// DCT12(tail[])
{ xm.ldawsp a0, STK_VEC_HEAD*4          ; nop                                   }
{ xm.ldawsp a1, STK_VEC_HEAD*4          ; nop                                   }
la t3, dct12_forward
{ nop                                   ; jalr t3                               }
  xm.lddsp  a1,a0,16
  xm.lddsp  a3,a2,24

// Before deconvolution, right-shift the head vector 2 bits, and 
// right-shift the tail vector 1 bit
{ li a, 1                               ; xm.mkmski c, 16                       }
  xm.vlashr d, a
{ addi t3,sp, (STK_VEC_TAIL)*4          ; xm.vstr d                             }
  xm.vlashr t3, a
{ li a, 2                               ; xm.vstr t3                            }
  xm.vlashr right, a
{ add t3, right, b                      ; xm.vstr right                         }
  xm.vlashr t3, a
  xm.vstrpv t3, c

// Finally, begin deconvolving and interleaving

{ mv t3, d                              ; nop                                   }
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
  
  xm.lddi  b,c, 24(t3)
{ sub b, b, d                           ; lw a,24             ( right)          }
  xm.stdi  a,b, 48(left)
{ sub c, c, b                           ; lw a,28             ( right)          }
  xm.stdi  a,c, 56(left)
  
  xm.lddi  b,d, 32(t3)
{ sub b, b, c                           ; lw a,32             ( right)          }
  xm.stdi  a,b, 64(left)
{ sub d, d, b                           ; lw a,36             ( right)          }
  xm.stdi  a,d, 72(left)
  
  xm.lddi  b,c, 40(t3)
{ sub b, b, d                           ; lw a,40             ( right)          }
  xm.stdi  a,b, 80(left)
{ sub c, c, b                           ; lw a,44             ( right)          }
  xm.stdi  a,c, 88(left)
  
  xm.lddsp  s3,s2,8
{ nop                                   ; xm.retsp (NSTACKWORDS)*4              }


	
.resource_const FUNCTION_NAME, "stack_frame_bytes", ((NSTACKWORDS+12))*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
.Ltmp0:
	.size	FUNCTION_NAME, .Ltmp0-FUNCTION_NAME    


#endif //defined(__VX4B__)
