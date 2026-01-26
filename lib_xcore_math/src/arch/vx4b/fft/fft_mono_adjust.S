// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


/*  
void fft_mono_adjust(
    complex_s32_t* X,
    const unsigned N,
    const unsigned inverse);
*/


#define FUNCTION_NAME   fft_mono_adjust

#define NSTACKVECTS     (4)
#define NSTACKWORDS     (32 + 8*(NSTACKVECTS))

#define STACK_VEC_TMP_A         (NSTACKWORDS-(8*2))
#define STACK_VEC_TMP_B         (NSTACKWORDS-(8*3))
#define STACK_VEC_TMP_B_CONJ    (NSTACKWORDS-(8*4))
#define STACK_VEC_TMP           (NSTACKWORDS-(8*5))

#define STACK_X0    (4)
#define STACK_XQ    (5)
#define STACK_X     (12)
#define STACK_N     (13)
#define STACK_W     (14)
#define STACK_INV   (15)

#define X           x10
#define N           x11
#define W           x12
#define X_lo        x13        
#define X_hi        x18  
#define _32         x19
#define i           x20
#define pos_j_vect  x21      
#define ones_vect   x22     
#define conj_vect   x23     

.text
.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
//.call FUNCTION_NAME, vect_complex_s32_tail_reverse

.p2align 4

FUNCTION_NAME:
    xm.entsp (NSTACKWORDS)*4
    xm.stdsp  x18,x19,8
    xm.stdsp  x20,x21,16
    xm.stdsp  x22,x23,24
    
    {   li t3, 0                              ;   sw s8, 4                          (sp)}
    {   addi s2, N, -8                            ;   sw a2, (STACK_INV)*4                   (sp)}
    {   slli s2, s2, 3                           ;   xm.vsetc t3}

    // W <-- &xmath_dit_fft_lut[N - 8]
    // W <-- xmath_dit_fft_lut + ((N-8)<<3)
lui t3, %hi(xmath_dit_fft_lut)
        addi t3,t3, %lo(xmath_dit_fft_lut)
    {   srli a3, N, 4                            ;   add W, t3, s2                          }
    
    {   srli N, N, 1                             ;   sw X, (STACK_X)*4                      (sp)}
    // exception if N < 16. Don't bother using this with really short FFTs.
    {   xm.assert a3                               ;   sw N, (STACK_N)*4                      (sp)}

        sh2add X, N, X
    {   srli N, N, 1                             ;   sw W, (STACK_W)*4                      (sp)}

    call vect_complex_s32_tail_reverse
    lw X, (STACK_X)*4(sp)/* Multiple XAT warnings: 'LDWSP outside of known frame - offset may need correction', "Falling back on assumption: the int < 64 for the integer value of the item at position 1 in the instruction's operands in ldwsp X, STACK_X\nMessage: The offset can be encoded in sru6 immediate" */
    lw N, (STACK_N)*4(sp)/* Multiple XAT warnings: 'LDWSP outside of known frame - offset may need correction', "Falling back on assumption: the int < 64 for the integer value of the item at position 1 in the instruction's operands in ldwsp N, STACK_N\nMessage: The offset can be encoded in sru6 immediate" */
    lw W, (STACK_W)*4(sp)/* Multiple XAT warnings: 'LDWSP outside of known frame - offset may need correction', "Falling back on assumption: the int < 64 for the integer value of the item at position 1 in the instruction's operands in ldwsp W, STACK_W\nMessage: The offset can be encoded in sru6 immediate" */


.p2align 4
.L_body:
    // the elements at indexes 0 and N/4 will come out of the loop wrong, but we can just store
    // X[0] and X[N/2] and fix them after the loop.
    {   srli i, N, 1                             ; nop                                           }
        xm.lddi  s5,s6, 0(X)
        xm.ldd  s7,s8, i(X)
        xm.stdsp  s5,s6,(STACK_X0)*8
        xm.stdsp  s7,s8,(STACK_XQ)*8
lui t3, %hi(vpu_vec_complex_pos_j)
        addi t3,t3, %lo(vpu_vec_complex_pos_j)
    {   mv pos_j_vect, t3                     ; nop                                           }
lui t3, %hi(vpu_vec_complex_ones)
        addi t3,t3, %lo(vpu_vec_complex_ones)
    {   mv ones_vect, t3                      ; nop                                           }
lui t3, %hi(vpu_vec_complex_conj_op)
        addi t3,t3, %lo(vpu_vec_complex_conj_op)
    {   mv conj_vect, t3                      ;   li _32, 32                             }

        li t3, 0x0080
    {   slli t3, N, 2                           ;   xm.vsetc t3}
    {   add X_hi, X, t3                        ;   mv X_lo, X                             }
    {   srli i, N, 3                             ;   lw t3, (STACK_INV)*4                  (sp)}
    { nop                                           ;   xm.brff t3, .L_main_loop                    }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
    {   mv X_hi, X_lo                          ;   mv X_lo, X_hi                          }

.L_main_loop://I want this loop to have 1 mod 4 alignment to eliminate all FNOPs
    {   addi i, i, -1                             ;   xm.vldd pos_j_vect}
    {   sub W, W, _32                          ;   xm.vldc W}
    { nop                                           ;   xm.vcmr0                                   }

    { nop                                           ;   xm.vcmi0                                    }
    {   addi t3,sp, (STACK_VEC_TMP_A)*4           ;   xm.vladsb ones_vect}
    {   addi t3,sp, (STACK_VEC_TMP_B)*4           ;   xm.vstd t3}
    { nop                                           ;   xm.vstr t3}

    {   addi t3,sp, (STACK_VEC_TMP_B_CONJ)*4      ;   xm.vlmul0 conj_vect}
    { nop                                           ;   xm.vstr t3}
    { nop                                           ;   xm.vldc X_lo}
    { nop                                           ;   xm.vcmr0                                   }

    {   addi t3,sp, (STACK_VEC_TMP_B)*4           ;   xm.vcmi0                                    }
    {   addi t3,sp, (STACK_VEC_TMP)*4             ;   xm.vldd t3}
    { nop                                           ;   xm.vstr t3}
    { nop                                           ;   xm.vldc X_hi}

    { nop                                           ;   xm.vcmcr0                                   }
    { nop                                           ;   xm.vcmci0                                    }
    { nop                                           ;   xm.vladd t3}
    {   addi t3,sp, (STACK_VEC_TMP_B_CONJ)*4      ;   xm.vldc X_lo}

    { nop                                           ;   xm.vldd t3}
    {   add X_lo, X_lo, _32                     ;   xm.vstr X_lo}
    { nop                                           ;   xm.vcmcr0                                   }
    {   addi t3,sp, (STACK_VEC_TMP)*4             ;   xm.vcmci0                                    }

    {   addi t3,sp, (STACK_VEC_TMP_A)*4           ;   xm.vstr t3}
    { nop                                           ;   xm.vldc t3}
    { nop                                           ;   xm.vldd X_hi}
    { nop                                           ;   xm.vcmcr0                                   }

    {   addi t3,sp, (STACK_VEC_TMP)*4             ;   xm.vcmci0                                    }
    { nop                                           ;   xm.vladd t3}
    {   add X_hi, X_hi, _32                     ;   xm.vstr X_hi}
    { nop                                           ;   xm.bt i, .L_main_loop                      }

    // If we had a LUT which already holds A[k], B[k] and the complex conjugate of B[k], we can do 
    // it in 23 instructions instead of 31  

    // If it seems worthwhile, could create an alternate version of this function that does it faster,
    // plus a function to initialize the needed table at start-up? It can be initialized based on the
    // existing FFT table.

    // {                                           ;   vldd table_A[0]                         }
    // {   sub i, i, 1                             ;   vldc X_lo[0]                            }
    // {                                           ;   vcmr                                    }
    // {                                           ;   vcmi                                    }
    // {                                           ;   vstr vec_tmp[0]                         }
    // {                                           ;   vldd table_B[0]                         }
    // {                                           ;   vldc X_hi[0]                            }
    // {                                           ;   vcmcr                                   }
    // {                                           ;   vcmci                                   }
    // {                                           ;   vladd vec_tmp[0]                        }
    // {                                           ;   vldd table_B_conj[0]                    }
    // {                                           ;   vldc X_lo[0]                            }
    // {   add X_lo, X_lo, _32                     ;   vstr X_lo[0]                            }
    // {                                           ;   vcmcr                                   }
    // {                                           ;   vcmci                                   }
    // {                                           ;   vstr vec_tmp[0]                         }
    // {   add table_A, table_A, _32               ;   vldc table_A[0]                         }
    // {   add table_B, table_B, _32               ;   vldd X_hi[0]                            }
    // {   add table_B_conj, table_B_conj, _32     ;   vcmcr                                   }
    // {                                           ;   vcmci                                   }
    // {                                           ;   vladd vec_tmp[0]                        }
    // {   add X_hi, X_hi, _32                     ;   vstr X_hi[0]                            }
    // {                                           ;   bt i, .L_something                      }

        xm.lddsp  s5,s6,(STACK_X0)*8
        xm.lddsp  s7,s8,(STACK_XQ)*8
    { nop                                           ;   lw t3, (STACK_INV)*4                  (sp)}
        sra s5, s5, t3
        sra s6, s6, t3

    {   add s5, s5, s6                          ;   sub s6, s5, s6                          }
        xm.stdi  s5,s6, 0(X)
    {   xm.neg s8, s8                            ;   srli i, N, 1                             }
        xm.std  s7,s8, i(X)
    

//Finally, reverse the elements again...
        sh2add X, N, X
    {   srli N, N, 1                             ; nop                                           }

    call vect_complex_s32_tail_reverse

.L_finish:
    { nop                                           ;   lw s8, 4                          (sp)}

        xm.lddsp  x18,x19,8
        xm.lddsp  x20,x21,16
        xm.lddsp  x22,x23,24
        xm.retsp (NSTACKWORDS)*4/* Multiple XAT warnings: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in retsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate", 'RETSP operand may need scaling' */

//.cc_bottom FUNCTION_NAME.function;  /* Translation error on this line: unexpected token at position 33. */ 
.set FUNCTION_NAME.nstackwords,((NSTACKWORDS) + vect_complex_s32_tail_reverse.nstackwords); /* Translation error on this line: unexpected token at position 90. */ 
.global FUNCTION_NAME.nstackwords;  /* Translation error on this line: unexpected token at position 33. */ 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores;  /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers;  /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends;  /* Translation error on this line: unexpected token at position 32. */ 

.L_function_end: 
    .size FUNCTION_NAME, .L_function_end - FUNCTION_NAME







#endif //defined(__VX4B__)
