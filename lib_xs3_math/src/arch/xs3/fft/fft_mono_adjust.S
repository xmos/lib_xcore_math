// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


/*  
void fft_mono_adjust(
    complex_s32_t* X,
    const unsigned N,
    const unsigned inverse);
*/


#define FUNCTION_NAME   fft_mono_adjust

#define NSTACKVECTS     (4)
#define NSTACKWORDS     (16 + 8*(NSTACKVECTS))

#define STACK_VEC_TMP_A         (NSTACKWORDS-(8*1))
#define STACK_VEC_TMP_B         (NSTACKWORDS-(8*2))
#define STACK_VEC_TMP_B_CONJ    (NSTACKWORDS-(8*3))
#define STACK_VEC_TMP           (NSTACKWORDS-(8*4))

#define STACK_X0    (4)
#define STACK_XQ    (5)
#define STACK_X     (12)
#define STACK_N     (13)
#define STACK_W     (14)
#define STACK_INV   (15)

#define X           r0
#define N           r1
#define W           r2
#define X_lo        r3        
#define X_hi        r4  
#define _32         r5
#define i           r6
#define pos_j_vect  r7      
#define ones_vect   r8     
#define conj_vect   r9     

.text
.issue_mode dual
.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.call FUNCTION_NAME, vect_complex_s32_tail_reverse

.align 16
.cc_top FUNCTION_NAME.function,FUNCTION_NAME

FUNCTION_NAME:

    {   dualentsp NSTACKWORDS                                                               }
    {   std r4, r5, sp[3]                                                                   }
    {   std r6, r7, sp[2]                                                                   }
    {   std r8, r9, sp[1]                                                                   }
    {   ldc r11, 0                              ;   stw r10, sp[1]                          }
    {   sub r4, N, 8                            ;   stw r2, sp[STACK_INV]                   }
    {   shl r4, r4, 3                           ;   vsetc r11                               }

    // W <-- &xmath_dit_fft_lut[N - 8]
    // W <-- xmath_dit_fft_lut + ((N-8)<<3)
        ldaw r11, cp[xmath_dit_fft_lut]
    {   shr r3, N, 4                            ;   add W, r11, r4                          }
    
    {   shr N, N, 1                             ;   stw X, sp[STACK_X]                      }
    // exception if N < 16. Don't bother using this with really short FFTs.
    {   ecallf r3                               ;   stw N, sp[STACK_N]                      }

        ldaw X, X[N]
    {   shr N, N, 1                             ;   stw W, sp[STACK_W]                      }

    bl vect_complex_s32_tail_reverse
    ldw X, sp[STACK_X]
    ldw N, sp[STACK_N]
    ldw W, sp[STACK_W]


.align 16
.L_body:
    // the elements at indexes 0 and N/4 will come out of the loop wrong, but we can just store
    // X[0] and X[N/2] and fix them after the loop.
    {   shr i, N, 1                             ;                                           }
        ldd r8, r7, X[0]
        ldd r10, r9, X[i]
        std r8, r7, sp[STACK_X0]
        std r10, r9, sp[STACK_XQ]
        ldaw r11, cp[vpu_vec_complex_pos_j]
    {   mov pos_j_vect, r11                     ;                                           }
        ldaw r11, cp[vpu_vec_complex_ones]
    {   mov ones_vect, r11                      ;                                           }
        ldaw r11, cp[vpu_vec_complex_conj_op]
    {   mov conj_vect, r11                      ;   ldc _32, 32                             }

        ldc r11, 0x0080
    {   shl r11, N, 2                           ;   vsetc r11                               }
    {   add X_hi, X, r11                        ;   mov X_lo, X                             }
    {   shr i, N, 3                             ;   ldw r11, sp[STACK_INV]                  }
    {                                           ;   bf r11, .L_main_loop                    }
    {   mov X_hi, X_lo                          ;   mov X_lo, X_hi                          }

.L_main_loop://I want this loop to have 1 mod 4 alignment to eliminate all FNOPs
    {   sub i, i, 1                             ;   vldd pos_j_vect[0]                      }
    {   sub W, W, _32                           ;   vldc W[0]                               }
    {                                           ;   vcmr                                    }

    {                                           ;   vcmi                                    }
    {   ldaw r11, sp[STACK_VEC_TMP_A]           ;   vladsb ones_vect[0]                     }
    {   ldaw r11, sp[STACK_VEC_TMP_B]           ;   vstd r11[0]                             }
    {                                           ;   vstr r11[0]                             }

    {   ldaw r11, sp[STACK_VEC_TMP_B_CONJ]      ;   vlmul conj_vect[0]                      }
    {                                           ;   vstr r11[0]                             }
    {                                           ;   vldc X_lo[0]                            }
    {                                           ;   vcmr                                    }

    {   ldaw r11, sp[STACK_VEC_TMP_B]           ;   vcmi                                    }
    {   ldaw r11, sp[STACK_VEC_TMP]             ;   vldd r11[0]                             }
    {                                           ;   vstr r11[0]                             }
    {                                           ;   vldc X_hi[0]                            }

    {                                           ;   vcmcr                                   }
    {                                           ;   vcmci                                   }
    {                                           ;   vladd r11[0]                            }
    {   ldaw r11, sp[STACK_VEC_TMP_B_CONJ]      ;   vldc X_lo[0]                            }

    {                                           ;   vldd r11[0]                             }
    {   add X_lo, X_lo, _32                     ;   vstr X_lo[0]                            }
    {                                           ;   vcmcr                                   }
    {   ldaw r11, sp[STACK_VEC_TMP]             ;   vcmci                                   }

    {   ldaw r11, sp[STACK_VEC_TMP_A]           ;   vstr r11[0]                             }
    {                                           ;   vldc r11[0]                             }
    {                                           ;   vldd X_hi[0]                            }
    {                                           ;   vcmcr                                   }

    {   ldaw r11, sp[STACK_VEC_TMP]             ;   vcmci                                   }
    {                                           ;   vladd r11[0]                            }
    {   add X_hi, X_hi, _32                     ;   vstr X_hi[0]                            }
    {                                           ;   bt i, .L_main_loop                      }

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

        ldd r8, r7, sp[STACK_X0]
        ldd r10, r9, sp[STACK_XQ]
    {                                           ;   ldw r11, sp[STACK_INV]                  }
        ashr r7, r7, r11
        ashr r8, r8, r11

    {   add r7, r7, r8                          ;   sub r8, r7, r8                          }
        std r8, r7, X[0]
    {   neg r10, r10                            ;   shr i, N, 1                             }
        std r10, r9, X[i]
    

//Finally, reverse the elements again...
        ldaw X, X[N]
    {   shr N, N, 1                             ;                                           }

    bl vect_complex_s32_tail_reverse

.L_finish:
    {                                           ;   ldw r10, sp[1]                          }

        ldd r8, r9, sp[1]
        ldd r6, r7, sp[2]
        ldd r4, r5, sp[3]
        retsp NSTACKWORDS

.cc_bottom FUNCTION_NAME.function; 
.set FUNCTION_NAME.nstackwords,((NSTACKWORDS) + vect_complex_s32_tail_reverse.nstackwords);
.global FUNCTION_NAME.nstackwords; 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores; 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers; 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends; 

.L_function_end: 
    .size FUNCTION_NAME, .L_function_end - FUNCTION_NAME







#endif //defined(__XS3A__)