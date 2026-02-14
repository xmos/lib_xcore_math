// Copyright 2020-2026 XMOS LIMITED.
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

#define STACK_VEC_TMP_A         (NSTACKWORDS-(8*2)-1)
#define STACK_VEC_TMP_B         (NSTACKWORDS-(8*3)-1)
#define STACK_VEC_TMP_B_CONJ    (NSTACKWORDS-(8*4)-1)
#define STACK_VEC_TMP           (NSTACKWORDS-(8*5)-1)

#define STACK_X0    (4)
#define STACK_XQ    (5)
#define STACK_X     (12)
#define STACK_N     (13)
#define STACK_W     (14)
#define STACK_INV   (15)

#define X           a0
#define N           a1
#define W           a2
#define X_lo        a3        
#define X_hi        s2  
#define _32         s3
#define i           s4
#define pos_j_vect  s5      
#define ones_vect   s6     
#define conj_vect   s7     

.text
.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
//.call FUNCTION_NAME, vect_complex_s32_tail_reverse

.p2align 4

FUNCTION_NAME:
    xm.entsp (NSTACKWORDS)*4
    xm.stdsp  s2,s3,8
    xm.stdsp  s4,s5,16
    xm.stdsp  s6,s7,24
    
    { li t3, 0                          ; sw s8, 4                         (sp) }
    { addi s2, N, -8                    ; sw a2, (STACK_INV)*4             (sp) }
    { slli s2, s2, 3                    ; xm.vsetc t3                           }

    // W <-- &xmath_dit_fft_lut[N - 8]
    // W <-- xmath_dit_fft_lut + ((N-8)<<3)
    la t3, xmath_dit_fft_lut
    { srli a3, N, 4                     ; add W, t3, s2                         }
    
    { srli N, N, 1                      ; sw X, (STACK_X)*4                (sp) }
    // exception if N < 16. Don't bother using this with really short FFTs.
    { xm.assert a3                      ; sw N, (STACK_N)*4                (sp) }

     sh2add X, N, X
    { srli N, N, 1                      ; sw W, (STACK_W)*4                (sp) }

    call vect_complex_s32_tail_reverse
    lw X, (STACK_X)*4(sp)
    lw N, (STACK_N)*4(sp)
    lw W, (STACK_W)*4(sp)


.p2align 4
.L_body:
    // the elements at indexes 0 and N/4 will come out of the loop wrong, but we can just store
    // X[0] and X[N/2] and fix them after the loop.
    { srli i, N, 1                      ; nop                                   }
    xm.lddi  s5,s6, 0(X)
    xm.ldd  s7,s8, i(X)
    xm.stdsp  s5,s6,(STACK_X0)*8
    xm.stdsp  s7,s8,(STACK_XQ)*8
    la t3, vpu_vec_complex_pos_j
    { mv pos_j_vect, t3                 ; nop                                   }
    la t3, vpu_vec_complex_ones
    { mv ones_vect, t3                  ; nop                                   }
    la t3, vpu_vec_complex_conj_op
    { mv conj_vect, t3                  ; li _32, 32                            }

        li t3, 0x0080
    { slli t3, N, 2                     ; xm.vsetc t3                           }
    { add X_hi, X, t3                   ; mv X_lo, X                            }
    { srli i, N, 3                      ; lw t3, (STACK_INV)*4             (sp) }
    { nop                               ; xm.brff t3, .L_main_loop              }
    { mv X_hi, X_lo                     ; mv X_lo, X_hi                         }

.L_main_loop://I want this loop to have 1 mod 4 alignment to eliminate all FNOPs
    { addi i, i, -1                     ; xm.vldd pos_j_vect                    }
    { sub W, W, _32                     ; xm.vldc W                             }
    { nop                               ; xm.vcmr0                              }

    { nop                               ; xm.vcmi0                              }
    { addi t3,sp, (STACK_VEC_TMP_A)*4   ; xm.vladsb ones_vect                   }
    { addi t3,sp, (STACK_VEC_TMP_B)*4   ; xm.vstd t3                            }
    { nop                               ; xm.vstr t3                            }

    { addi t3,sp, (STACK_VEC_TMP_B_CONJ)*4 ; xm.vlmul0 conj_vect                }
    { nop                               ; xm.vstr t3                            }
    { nop                               ; xm.vldc X_lo                          }
    { nop                               ; xm.vcmr0                              }

    { addi t3,sp, (STACK_VEC_TMP_B)*4   ; xm.vcmi0                              }
    { addi t3,sp, (STACK_VEC_TMP)*4     ; xm.vldd t3                            }
    { nop                               ; xm.vstr t3                            }
    { nop                               ; xm.vldc X_hi                          }

    { nop                               ; xm.vcmcr0                             }
    { nop                               ; xm.vcmci0                             }
    { nop                               ; xm.vladd t3                           }
    { addi t3,sp, (STACK_VEC_TMP_B_CONJ)*4 ; xm.vldc X_lo                       }

    { nop                               ; xm.vldd t3                            }
    { add X_lo, X_lo, _32               ; xm.vstr X_lo                          }
    { nop                               ; xm.vcmcr0                             }
    { addi t3,sp, (STACK_VEC_TMP)*4     ; xm.vcmci0                             }

    { addi t3,sp, (STACK_VEC_TMP_A)*4   ; xm.vstr t3                            }
    { nop                               ; xm.vldc t3                            }
    { nop                               ; xm.vldd X_hi                          }
    { nop                               ; xm.vcmcr0                             }

    { addi t3,sp, (STACK_VEC_TMP)*4     ; xm.vcmci0                             }
    { nop                               ; xm.vladd t3                           }
    { add X_hi, X_hi, _32               ; xm.vstr X_hi                          }
    { nop                               ; xm.bt i, .L_main_loop                 }

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
    { nop                               ; lw t3, (STACK_INV)*4                  (sp) }
    sra s5, s5, t3
    sra s6, s6, t3

    { add s5, s5, s6                    ; sub s6, s5, s6                        }
    xm.stdi  s5,s6, 0(X)
    { xm.neg s8, s8                     ; srli i, N, 1                          }
    xm.std  s7,s8, i(X)
    

//Finally, reverse the elements again...
    sh2add X, N, X
    { srli N, N, 1                      ; nop                                   }

    call vect_complex_s32_tail_reverse

.L_finish:
    { nop                               ; lw s8, 4                          (sp) }

    xm.lddsp  s2,s3,8
    xm.lddsp  s4,s5,16
    xm.lddsp  s6,s7,24
    xm.retsp (NSTACKWORDS)*4

.resource_const FUNCTION_NAME, "stack_frame_bytes", ( ((NSTACKWORDS)))*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_add   FUNCTION_NAME, "callees", vect_complex_s32_tail_reverse
.resource_list_empty FUNCTION_NAME, "parallel_callees"

.L_function_end: 
    .size FUNCTION_NAME, .L_function_end - FUNCTION_NAME


#endif //defined(__VX4B__)
