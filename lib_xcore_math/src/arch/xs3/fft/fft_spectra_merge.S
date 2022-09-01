// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


/*  
headroom_t fft_spectra_merge(
    complex_s32_t* X,
    const unsigned N);
*/



#define FUNCTION_NAME   fft_spectra_merge
#define NSTACKWORDS     (8)

#define XS3_CONFIG_MIN_FFT_LEN (4)

#define X       r0
#define N       r1


.text
.issue_mode dual
.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.align 16
.cc_top FUNCTION_NAME.function,FUNCTION_NAME

FUNCTION_NAME:
        dualentsp NSTACKWORDS
        std r4, r5, sp[1]
        std r6, r7, sp[2]
        std r8, r9, sp[3]
    {   ldc r11, 0                              ;   stw r10, sp[1]                          }
    {   shr r11, N, 3                           ;   vsetc r11                               }
#if (XS3_CONFIG_MIN_FFT_LEN <= 4)
    {                                           ;   bf r11, .L_fft_length_4                 }
    {                                           ;   bu .L_pre_boggle                        }
#endif


#if (CONFIG_MIN_FFT_LEN <= 4)
.L_fft_length_4:

    // If the FFT length is 4, just do the work here. This keeps the code below simpler.
    {                                           ;   ldw r4, X[1]                            }
    {                                           ;   ldw r5, X[4]                            }
    {                                           ;   stw r4, X[4] /* X[2].re <- X[0].im */   }
    {                                           ;   stw r5, X[1] /* X[0].im <- X[2].re */   }
        ldd r5, r4, X[1]
        ldd r7, r6, X[3]
    {   sub r10, r4, r7                         ;   add r11, r5, r6                         }
        std r11, r10, X[1]
    {   add r10, r4, r7                         ;   sub r11, r6, r5                         }
        std r11, r10, X[3]
    {                                           ;   vldd X[0]                               }
    {                                           ;   vstd X[0]                               }
    {                                           ;   vgetc r11                               }
    {   mov r4, r11                             ;   bu .L_finish                            }

#endif

.L_pre_boggle:

#define DC_re   r2
#define DC_im   r3
#define Ny_re   r4
#define Ny_im   r5

    // Pre-boggle the DC and Nyquist bins so we can do everything on the VPU
    // Wait, is it faster to just compute the results and hold onto them...?

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



#define X_lo    r2
#define X_hi    r3
#define i       r4
#define _32     r5

    // Now go through and compute the outputs

    {   ldaw X_hi, X[N]                                                                     }
    {   ldc r11, 0                                                                          }
    {   shr i, N, 3                             ;   vsetc r11                               }
        ldaw r11, cp[vpu_vec_complex_neg_j]
    {   mov X_lo, X                             ;   vldc r11[0]                             }
        ldaw r11, cp[vpu_vec_complex_conj_op]
    {   ldc _32, 32                             ;   bu .L_syzygy                            }

.align 16
.L_syzygy:
        {   sub i, i, 1                             ;   vldd X_hi[0]                        }
        {                                           ;   vcmr                                }
        {                                           ;   vcmi                                }
        {                                           ;   vladsb X_lo[0]                      }
        {   add X_lo, X_lo, _32                     ;   vstd X_lo[0]                        }
        {                                           ;   vlmul r11[0]                        }
        {   add X_hi, X_hi, _32                     ;   vstr X_hi[0]                        }
        {                                           ;   bt i, .L_syzygy                     }

    
        ldaw X, X[N]
    {   shr N, N, 1                             ;   vgetc r11                               }
    {   mov r4, r11                             ;                                           }
        bl vect_complex_s32_tail_reverse


.L_finish:


    {   ldc r0, 31                              ;                                           }
    {   zext r4, 5                              ;                                           }
    {   sub r0, r0, r4                          ;   ldw r10, sp[1]                          }

        ldd r4, r5, sp[1]
        ldd r6, r7, sp[2]
        ldd r8, r9, sp[3]
        retsp NSTACKWORDS

.cc_bottom FUNCTION_NAME.function; 
.set FUNCTION_NAME.nstackwords,NSTACKWORDS + vect_complex_s32_tail_reverse.nstackwords;     
                                                .global FUNCTION_NAME.nstackwords; 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores; 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers; 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends; 
.L_function_end: 
    .size FUNCTION_NAME, .L_function_end - FUNCTION_NAME




#endif //defined(__XS3A__)