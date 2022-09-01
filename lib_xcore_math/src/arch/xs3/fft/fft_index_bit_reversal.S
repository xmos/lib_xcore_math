// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#if defined(__XS3A__)


/*  

void fft_index_bit_reversal(
    complex_s32_t* a,
    const unsigned length);

*/

#define FUNCTION_NAME   fft_index_bit_reversal
#define NSTACKWORDS 4

.text
.issue_mode dual
.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.align 16
.cc_top FUNCTION_NAME.function,FUNCTION_NAME

FUNCTION_NAME:
	{   clz r2, r1                              ;   dualentsp NSTACKWORDS                   }
    {   std r6, r7, sp[1]                                                                   }
    {   std r4, r5, sp[0]                                                                   }
    {   add r2, r2, 1                           ;   shr r1, r1, 1                           }
    {   shl r1, r1, 1                           ;   bu .L_loop                              }
    
.align 16
.L_loop:   
    {   bitrev r3, r1                           ;   shl r11, r1, r2                         }
    {   lsu r11, r3, r11                        ;   shr r3, r3, r2                          }
    {   sub r11, r1, 1                          ;   bf r11, .L_dontswap                     }
    {   ldd r4, r5, r0[r1]                                                                  }
    {   ldd r6, r7, r0[r3]                                                                  }
    {   std r4, r5, r0[r3]                                                                  }
    {   std r6, r7, r0[r1]                                                                  }
.L_dontswap:
    {   bitrev r3, r11                          ;   shl r1, r11, r2                         }
    {   lsu r1, r3, r1                          ;   shr r3, r3, r2                          }
    {   sub r1, r11, 1                          ;   bf r1, .L_dontswap2                     }
    {   ldd r4, r5, r0[r11]                                                                 }
    {   ldd r6, r7, r0[r3]                                                                  }
    {   std r4, r5, r0[r3]                                                                  }
    {   std r6, r7, r0[r11]                                                                 }
.L_dontswap2:
    {   bt r1, .L_loop                                                                      }
    
    {   ldd r6, r7, sp[1]                                                                   }
    {   ldd r4, r5, sp[0]                                                                   }
	{   retsp NSTACKWORDS                                                                   }
	
	// RETURN_REG_HOLDER
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