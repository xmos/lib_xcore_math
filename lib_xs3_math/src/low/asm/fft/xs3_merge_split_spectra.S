
#if defined(__XS3A__)
#ifndef XS3_MATH_NO_ASM

/*  
headroom_t xs3_merge_fft_spectra_s32(
    complex_s32_t* X,
    const unsigned N);
*/

#include "../asm_helper.h"


#define NSTACKWORDS     (8)

#define XS3_CONFIG_MIN_FFT_LEN (4)

#define X       r0
#define N       r1

.section    .cp.rodata, "ac", @progbits
.align  4
L_neg_j_vect:
    .word 0x00000000, -0x40000000, 0x00000000, -0x40000000, 0x00000000, -0x40000000, 0x00000000, -0x40000000
L_comp_conj_vect:
    .word 0x40000000, -0x40000000, 0x40000000, -0x40000000, 0x40000000, -0x40000000, 0x40000000, -0x40000000
L_zero_vect:
    .word 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000

.text
.issue_mode dual

////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////       xs3_merge_fft_spectra_s32          ////////////////////////////

#define FUNCTION_NAME   xs3_merge_fft_spectra_s32

.text; .issue_mode dual
.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.align 4
.cc_top FUNCTION_NAME.function,FUNCTION_NAME
FUNCTION_NAME:
    {   ldc r2, 1                               ;   bu .L_common_init                       }

.cc_bottom FUNCTION_NAME.function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
CAT(.L_size_end_, FUNCTION_NAME): 
.size FUNCTION_NAME,CAT(.L_size_end_, FUNCTION_NAME) - FUNCTION_NAME

#undef FUNCTION_NAME


////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////       xs3_split_fft_spectrum_s32         ////////////////////////////

#define FUNCTION_NAME   xs3_split_fft_spectrum_s32

.text; .issue_mode dual
.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.align 4
.cc_top FUNCTION_NAME.function,FUNCTION_NAME
FUNCTION_NAME:
    {   ldc r2, 0                               ;   bu .L_common_init                       }


.cc_bottom FUNCTION_NAME.function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
CAT(.L_size_end_, FUNCTION_NAME): 
.size FUNCTION_NAME,CAT(.L_size_end_, FUNCTION_NAME) - FUNCTION_NAME

#undef FUNCTION_NAME

////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////               common init                ////////////////////////////


// .type .L_common_init,@function
.cc_top .L_common_init.function,.L_common_init
.L_common_init:

    {   ldc r11, 0                              ;   dualentsp NSTACKWORDS                   }
        std r4, r5, sp[1]
        std r6, r7, sp[2]
    {   shr r11, N, 3                           ;   vsetc r11                               }
        std r8, r9, sp[3]
        std r10, r2, sp[0]
#if (XS3_CONFIG_MIN_FFT_LEN <= 4)
    {                                           ;   bf r11, .L_fft_length_4                 }
#endif
    {                                           ;   bt r2, .L_merge_main     /*merge*/      }
    {                                           ;   bu .L_reverse_elements   /*split*/      }

.L_common_init_end:
.cc_bottom .L_common_init.function

////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////               merge main                 ////////////////////////////

.cc_top .L_merge_main.function,.L_merge_main
.L_merge_main:

#define DC_re   r2
#define DC_im   r3
#define Ny_re   r4
#define Ny_im   r5

    // Pre-boggle the DC and Nyquist bins so we can do everything on the VPU

    {   shr r8, N, 1                            ;                                           }
        ldd DC_im, DC_re, X[0]
        ldd Ny_im, Ny_re, X[r8]
        ashr DC_re, DC_re, 1
        ashr DC_im, DC_im, 1
        ashr Ny_re, Ny_re, 1
        ashr Ny_im, Ny_im, 1
    {   add r9, DC_re, DC_im                    ;   sub r11, Ny_re, Ny_im                   }
        std r11, r9, X[0]
    {   add r9, Ny_re, Ny_im                    ;   sub r11, DC_im, DC_re                   }
        std r11, r9, X[r8]

#undef DC_re
#undef DC_im
#undef Ny_re
#undef Ny_im


#define X_lo    r2
#define X_hi    r3
#define i       r4
#define _32     r5

    // Now go through and compute the outputs

    {   ldaw X_hi, X[N]                                                                     }
    {   ldc r11, 0                                                                          }
    {   shr i, N, 3                             ;   vsetc r11                               }
        ldaw r11, cp[L_neg_j_vect]
    {   mov X_lo, X                             ;   vldc r11[0]                             }
        ldaw r11, cp[L_comp_conj_vect]
    {   ldc _32, 32                             ;   bu .L_merge_syzygy                      }

.align 16
.L_merge_syzygy:
        {   sub i, i, 1                             ;   vldd X_hi[0]                        }
        {                                           ;   vcmr                                }
        {                                           ;   vcmi                                }
        {                                           ;   vladsb X_lo[0]                      }
        {   add X_lo, X_lo, _32                     ;   vstd X_lo[0]                        }
        {                                           ;   vlmul r11[0]                        }
        {   add X_hi, X_hi, _32                     ;   vstr X_hi[0]                        }
        {                                           ;   bt i, .L_merge_syzygy               }

    {                                       ;   bu .L_reverse_elements                  }
#undef X_lo
#undef X_hi
#undef i
#undef _32

.L_merge_main_end:
.cc_bottom .L_merge_main.function

////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////               split main                 ////////////////////////////

.cc_top .L_split_main.function,.L_split_main
.L_split_main:

#define X_lo    X
#define i       r2
#define _32     r3
#define X_hi    r4
#define DC_im   r5
#define DC_re   r6
#define Ny_im   r7
#define Ny_re   r8

    // x = [DC.re - Ny.im, Ny.re + DC.im, DC.re + Ny.im, -Ny.re + DC.im]

    // If I set [X[0].re, X[0].im, X[K].re, X[k].im] to the vector above, then I can just compute
    // the results for bins 0 and K along with everything else. Then I'm guaranteed that the number
    // of elements is a multiple of 4, which means this loop will have no tail, AND it will have
    // captured the headroom of the vector (although it will be the lesser of the lower and upper
    // halves)
    {   ldc _32, 32                             ;   shr i, N, 1                             }
        ldd DC_im, DC_re, X[0]
        ldd Ny_im, Ny_re, X[i]
    {   sub r9, DC_re, Ny_im                    ;   add r11, DC_im, Ny_re                   }
        std r11, r9, X[0]
    {   add r9, DC_re, Ny_im                    ;   sub r11, DC_im, Ny_re                   }
        std r11, r9, X[i]  

#undef DC_re
#undef DC_im
#undef Ny_re
#undef Ny_im

    {   ldaw X_hi, X_lo[N]                                                                  }
    {   ldc r11, 0x0080                                                                     }
    {   shr i, i, 2                             ;   vsetc r11                               }
        ldaw r11, cp[L_neg_j_vect]
    {                                           ;   vldc r11[0]                             }
        ldaw r11, cp[L_comp_conj_vect]
    {                                           ;   bu .L_split_syzygy                      }

.align 16
.L_split_syzygy:
        {   sub i, i, 1                             ;   vldr r11[0]                             }
        {                                           ;   vlmul X_hi[0]                           }
        {                                           ;   vladsb X_lo[0]                          }
        {   add X_lo, X_lo, _32                     ;   vstr X_lo[0]                            }
        {                                           ;   vcmr                                    }
        {                                           ;   vcmi                                    }
        {   add X_hi, X_hi, _32                     ;   vstr X_hi[0]                            }
        {                                           ;   bt i, .L_split_syzygy                   }

    {                                       ;   bu .L_finish                                }
#undef X_lo
#undef i
#undef _32
#undef X_hi



.L_split_main_end:
.cc_bottom .L_split_main.function


////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////                 Finish                   ////////////////////////////


.cc_top .L_finish.function,.L_finish
.L_finish:
    {   ldc r0, 31                              ;   vgetc r11                               }
    {   zext r11, 5                             ;                                           }
    {   sub r0, r0, r11                         ;   ldw r10, sp[1]                          }

        ldd r4, r5, sp[1]
        ldd r6, r7, sp[2]
        ldd r8, r9, sp[3]
        retsp NSTACKWORDS
.L_finish_end:
.cc_bottom .L_finish.function

////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////              FFT Length 4                ////////////////////////////

// Handles case where N = 4 (because the VPU can't do that easily)
#if (CONFIG_MIN_FFT_LEN <= 4)

// .type .L_fft_length_4,@function
.cc_top .L_fft_length_4.function,.L_fft_length_4
.L_fft_length_4:

    {   mkmsk r6, 8                             ;   ldw r11, sp[0]                          }
    {   shl r7, r6, 16                          ;   ldw r4, X[1]                            }
    {   add r6, r6, r7                          ;   ldw r5, X[4]                            }
    {   ldc r7, 1                               ;   stw r4, X[4] /* X[2].re <- X[0].im */   }
    {   not r6, r6                              ;   stw r5, X[1] /* X[0].im <- X[2].re */   }
    {                                           ;   bt r11, .L_fft_merge_length_4           }
    
.L_fft_split_length_4:
        vlashr X[0], r7
        vstrpv X[0], r6
        ldd r5, r4, X[1]
        ldd r7, r6, X[3]
    {   add r10, r4, r6                         ;   sub r11, r5, r7                         }
    {   add r8, r5, r7                          ;   sub r9, r6, r4                          }
    {                                           ;   bu .L_fft_length_4_last                 }

.L_fft_merge_length_4:
        ldd r5, r4, X[1]
        ldd r7, r6, X[3]
    {   sub r10, r4, r7                         ;   add r11, r5, r6                         }
    {   add r8, r4, r7                          ;   sub r9, r6, r5                          }

.L_fft_length_4_last:
        std r11, r10, X[1]
        std r9, r8, X[3]
    {                                           ;   vldd X[0]                               }
    {                                           ;   vstd X[0]                               }
    {                                           ;   bu .L_finish                            }

.L_fft_length_4_end:
.cc_bottom .L_fft_length_4.function
// .size .L_reverse_elements, .L_reverse_elements_end - .L_reverse_elements

#endif


////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////            Element Reversal              ////////////////////////////

.align 4
.cc_top .L_reverse_elements.function,.L_reverse_elements
.L_reverse_elements:


#if (XS3_CONFIG_MIN_FFT_LEN <= 8)
    // If the FFT length is 8, the VPU implementation of sequence reversal won't work because there's only
    //  two elements to be swapped. So just do that here.

    {   shr r11, N, 4                           ;                                           }
    {                                           ;   bt r11, .L_vpu_reverse                  }
        ldd r4, r5, X[5]
        ldd r6, r7, X[7]
        std r4, r5, X[7]
        std r6, r7, X[5]
        bu .L_reverse_elements_last
#endif


#define i       r2
#define X_A     r3
#define X_C     r4
#define mask_A  r5
#define mask_C  r6
#define _32     r7
#define zero    r8
#define _16     r9
#define X_hi    r10
#define X_lo    r11

.L_vpu_reverse:

        ldaw r11, cp[L_zero_vect]
    {   ldc _32, 32                             ;   mov zero, r11                           }
    {   mkmsk mask_A, 8                         ;   vclrdr                                  }
    {   ldc _16, 16                             ;   shl mask_A, mask_A, 8                   }
    {   shr i, N, 4                             ;   shl mask_C, mask_A, 16                  }
        ldaw X_lo, X[N]
        ldaw X_hi, X_lo[N]
    {   add X_lo, X_lo, 8                       ;   sub X_hi, X_hi, _32                     }
    {                                           ;   bu .L_rev_loop                          }

.align 16
.L_rev_loop:
        {   add X_A, X_hi, _16                      ;   vldc X_hi[0]                            }
        {   sub X_C, X_hi, _16                      ;   vldr X_lo[0]                            }
        {   sub i, i, 1                             ;   vlmaccr zero[0]                         }
        {                                           ;   vlmaccr zero[0]                         }
        {   sub X_hi, X_hi, _32                     ;   vstr X_hi[0]                            }
            vstrpv X_A[0], mask_A
            vstrpv X_C[0], mask_C
        {   add X_A, X_lo, _16                      ;   vstc X_lo[0]                            }
        {   sub X_C, X_lo, _16                      ;   vldr X_lo[0]                            }
        {                                           ;   vlmaccr zero[0]                         }
        {                                           ;   vlmaccr zero[0]                         }
        {   add X_lo, X_lo, _32                     ;   vstr X_lo[0]                            }
            vstrpv X_A[0], mask_A
            vstrpv X_C[0], mask_C
        {                                           ;   bt i, .L_rev_loop                       }

.L_reverse_elements_last:
    {                                           ;   ldw r11, sp[0]                          }
    {                                           ;   bt r11, .L_finish         /*merge*/     }
    {                                           ;   bu .L_split_main          /*split*/     }

#undef i
#undef X_A
#undef X_C
#undef mask_A
#undef mask_C
#undef _32
#undef zero
#undef _16
#undef X_hi
#undef X_lo
.L_reverse_elements_end:
.cc_bottom .L_reverse_elements.function
////////////////////////////////////////////////////////////////////////////////////////////




#endif //!defined(XS3_MATH_NO_ASM)
#endif //defined(__XS3A__)