// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)

/*  

void fft_index_bit_reversal(
    complex_s32_t* a,
    const unsigned length);
*/

#define FUNCTION_NAME   fft_index_bit_reversal
#define NSTACKWORDS 8

.text
.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.p2align 4

FUNCTION_NAME:
xm.entsp (NSTACKWORDS)*4
xm.stdsp  s4, s5, 0
xm.stdsp  s2, s3, 8
xm.clz a2, a1
{ addi a2, a2, 1                    ; srli a1, a1, 1                        }
{ slli a1, a1, 1                    ; xm.bu .L_loop                         }

.p2align 4
.L_loop:   
    { xm.bitrev a3, a1                  ; xm.shl t3, a1, a2                     }
    { xm.sltu t3, a3, t3                ; xm.shr a3, a3, a2                     }
    { addi t3, a1, -1                   ; xm.brff t3, .L_dontswap               }
    xm.ldd s2, s3, a1(a0)                                                                  
    xm.ldd s4, s5, a3(a0)                                                                  
    xm.std s2, s3, a3(a0)                                                                  
    xm.std s4, s5, a1(a0)                                                                
.L_dontswap:
    { xm.bitrev a3, t3                  ; xm.shl a1, t3, a2                     }
    { xm.sltu a1, a3, a1                ; xm.shr a3, a3, a2                     }
    { addi a1, t3, -1                   ; xm.brff a1, .L_dontswap2              }
    xm.ldd s2, s3, t3(a0)                                                                 
    xm.ldd s4, s5, a3(a0)                                                                 
    xm.std s2, s3, a3(a0)                                                                  
    xm.std s4, s5, t3(a0)                                                                 
.L_dontswap2:
    { xm.bt a1, .L_loop                 ; nop                                   }
    
xm.lddsp  s4, s5, 0
xm.lddsp  s2, s3, 8
xm.retsp (NSTACKWORDS)*4

	// RETURN_REG_HOLDER
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
