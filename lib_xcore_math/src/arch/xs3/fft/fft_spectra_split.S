// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


/*  
headroom_t fft_spectra_split(
    complex_s32_t* X,
    const unsigned N);
*/


#define FUNCTION_NAME   fft_spectra_split
#define NSTACKWORDS     (8)

#define X       r0
#define N       r1

.text
.issue_mode dual
.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.align 16
.cc_top FUNCTION_NAME.function,FUNCTION_NAME

FUNCTION_NAME:
    {   dualentsp NSTACKWORDS                                                               }
    {   std r4, r5, sp[3]                                                                   }
    {   std r6, r7, sp[2]                                                                   }
    {   std r8, r9, sp[1]                                                                   }
    {   ldc r11, 0                              ;   stw r10, sp[1]                          }
    {   shr r10, N, 3                           ;   vsetc r11                               }

  #if (XS3_CONFIG_MIN_FFT_LEN <= 4)
    {                                           ;   bt r10, .L_split_the_spectrum           }
  #endif

#if (CONFIG_MIN_FFT_LEN <= 4)
.L_fft_length_4:

    // If the FFT length is 4, just do the work here. This keeps the code below simpler.
    {   mkmsk r10, 8                            ;                                           }
    {   shl r11, r10, 16                        ;   ldw r4, X[1]                            }
    {   add r10, r10, r11                       ;   ldw r5, X[4]                            }
    {   ldc r11, 1                              ;   stw r4, X[4] /* X[2].re <- X[0].im */   }
    {   not r10, r10                            ;   stw r5, X[1] /* X[0].im <- X[2].re */   }
        vlashr X[0], r11
        vstrpv X[0], r10
        ldd r5, r4, X[1]
        ldd r7, r6, X[3]
    {   add r10, r4, r6                         ;   sub r11, r5, r7                         }
        std r11, r10, X[1]
    {   add r10, r5, r7                         ;   sub r11, r6, r4                         }
        std r11, r10, X[3]
    {                                           ;   vldd X[0]                               }
    {                                           ;   vstd X[0]                               }
    {                                           ;   bu .L_finish                            }

#endif



.L_split_the_spectrum:

    // First, reverse the tail
    {   mov r4, X                               ;   mov r5, N                               }
        ldaw X, X[N]
    {   shr N, N, 1                             ;                                           }
        bl vect_complex_s32_tail_reverse
    {   mov X, r4                               ;   mov N, r5                               }

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

#define conj_vec    r5


    {   ldaw X_hi, X_lo[N]                                                                  }
    {   ldc r11, 0x0080                                                                     }
    {   shr i, i, 2                             ;   vsetc r11                               }
        ldaw r11, cp[vpu_vec_complex_neg_j]
    {                                           ;   vldc r11[0]                             }
        ldaw r11, cp[vpu_vec_complex_conj_op]
    {                                           ;   bu .L_syzygy                            }

.align 16
.L_syzygy:
        {   sub i, i, 1                             ;   vldr r11[0]                             }
        {                                           ;   vlmul X_hi[0]                           }
        {                                           ;   vladsb X_lo[0]                          }
        {   add X_lo, X_lo, _32                     ;   vstr X_lo[0]                            }
        {                                           ;   vcmr                                    }
        {                                           ;   vcmi                                    }
        {   add X_hi, X_hi, _32                     ;   vstr X_hi[0]                            }
        {                                           ;   bt i, .L_syzygy                         }


.L_finish:
    {   ldc r0, 31                              ;   vgetc r11                               }
    {   zext r11, 5                             ;                                           }
    {   sub r0, r0, r11                         ;   ldw r10, sp[1]                          }

    {   ldd r8, r9, sp[1]                                                                   }
    {   ldd r6, r7, sp[2]                                                                   }
    {   ldd r4, r5, sp[3]                                                                   }
    {   retsp NSTACKWORDS                                                                   }

.cc_bottom FUNCTION_NAME.function; 
.set FUNCTION_NAME.nstackwords,NSTACKWORDS + vect_complex_s32_tail_reverse.nstackwords;     
                                                .global FUNCTION_NAME.nstackwords; 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores; 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers; 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends; 
.L_function_end: 
    .size FUNCTION_NAME, .L_function_end - FUNCTION_NAME







#endif //defined(__XS3A__)