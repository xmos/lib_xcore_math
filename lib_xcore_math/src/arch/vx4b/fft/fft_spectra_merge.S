// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


/*  
headroom_t fft_spectra_merge(
    complex_s32_t* X,
    const unsigned N);
*/



#define FUNCTION_NAME   fft_spectra_merge
#define NSTACKWORDS     (8)

#define VX4_CONFIG_MIN_FFT_LEN (4)

#define X       a0
#define N       a1


.text
.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.p2align 4

FUNCTION_NAME:
        xm.entsp (NSTACKWORDS)*4
        xm.stdsp  s3,s2,8
        xm.stdsp  s5,s4,16
        xm.stdsp  s7,s6,0
    { li t3, 0                          ; sw s8, 24                          (sp) }
    { srli t3, N, 3                     ; xm.vsetc t3                           }
#if (VX4_CONFIG_MIN_FFT_LEN <= 4)
    { nop                               ; xm.brff t3, .L_fft_length_4           }
    { nop                               ; xm.bu .L_pre_boggle                   }
#endif


#if (CONFIG_MIN_FFT_LEN <= 4)
.L_fft_length_4:

    // If the FFT length is 4, just do the work here. This keeps the code below simpler.
    { nop                               ; lw s2,4                            ( X) }
    { nop                               ; lw s3,16                            ( X) }
    { nop                               ; sw s2,16 /* X[2].re <- X[0].im */   ( X) }
    { nop                               ; sw s3,4 /* X[0].im <- X[2].re */   ( X) }
        xm.lddi  s2,s3, 8(X)
        xm.lddi  s4,s5, 24(X)
    { sub s8, s2, s5                    ; add t3, s3, s4                        }
        xm.stdi  s8,t3, 8(X)
    { add s8, s2, s5                    ; sub t3, s4, s3                        }
        xm.stdi  s8,t3, 24(X)
    { nop                               ; xm.vldd X                             }
    { nop                               ; xm.vstd X                             }
    { nop                               ; xm.vgetc t3                           }
    { mv s2, t3                         ; xm.bu .L_finish2                      }
.L_finish2:


    { li a0, 31                         ; nop                                   }
    { xm.zexti s2, 5                    ; nop                                   }
    { sub a0, a0, s2                    ; lw s8, 24                          (sp) }

        xm.lddsp  s3,s2,8
        xm.lddsp  s5,s4,16
        xm.lddsp  s7,s6,0
        xm.retsp (NSTACKWORDS)*4

#endif

.L_pre_boggle:

#define DC_re   a2
#define DC_im   a3
#define Ny_re   s2
#define Ny_im   s3

    // Pre-boggle the DC and Nyquist bins so we can do everything on the VPU
    // Wait, is it faster to just compute the results and hold onto them...?

    { srli s6, N, 1                     ; nop                                   }
        xm.lddi  DC_re,DC_im, 0(X)
        xm.ldd  Ny_re,Ny_im, s6(X)
        srai DC_re, DC_re, 1
        srai DC_im, DC_im, 1
        srai Ny_re, Ny_re, 1
        srai Ny_im, Ny_im, 1
    { xm.add s7, DC_re, DC_im           ; xm.sub t3, Ny_re, Ny_im               }
        xm.stdi  s7,t3, 0(X)
    { xm.add s7, Ny_re, Ny_im           ; xm.sub t3, DC_im, DC_re               }
        xm.std  s7,t3, s6(X)



#define X_lo    a2
#define X_hi    a3
#define i       s2
#define _32     s3

    // Now go through and compute the outputs

   sh2add X_hi, N, X

       li t3, 0                                                                          
    { srli i, N, 3                      ; xm.vsetc t3                           }
la t3, vpu_vec_complex_neg_j
    { mv X_lo, X                        ; xm.vldc t3                            }
la t3, vpu_vec_complex_conj_op
    { li _32, 32                        ; xm.bu .L_syzygy                       }

.p2align 4
.L_syzygy:
        { addi i, i, -1                 ; xm.vldd X_hi                          }
        { nop                           ; xm.vcmr0                              }
        { nop                           ; xm.vcmi0                              }
        { nop                           ; xm.vladsb X_lo                        }
        { add X_lo, X_lo, _32           ; xm.vstd X_lo                          }
        { nop                           ; xm.vlmul0 t3                          }
        { add X_hi, X_hi, _32           ; xm.vstr X_hi                          }
        { nop                           ; xm.bt i, .L_syzygy                    }

    
        sh2add X, N, X
    { srli N, N, 1                      ; xm.vgetc t3                           }
    { mv s2, t3                         ; nop                                   }
        call vect_complex_s32_tail_reverse


.L_finish:


    { li a0, 31                         ; nop                                   }
    { xm.zexti s2, 5                    ; nop                                   }
    { sub a0, a0, s2                    ; lw s8, 24                          (sp) }

        xm.lddsp  s3,s2,8
        xm.lddsp  s5,s4,16
        xm.lddsp  s7,s6,0
        xm.retsp (NSTACKWORDS)*4

//.cc_bottom FUNCTION_NAME.function; 
.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
                                                .global FUNCTION_NAME.nstackwords; 
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
.L_function_end: 
    .size FUNCTION_NAME, .L_function_end - FUNCTION_NAME




#endif //defined(__VX4B__)
