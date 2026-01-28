// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


/*  
headroom_t fft_spectra_merge(
    complex_s32_t* X,
    const unsigned N);
*/



#define FUNCTION_NAME   fft_spectra_merge
#define NSTACKWORDS     (16)

#define XS3_CONFIG_MIN_FFT_LEN (4)

#define X       x10
#define N       x11


.text
.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.p2align 4

FUNCTION_NAME:
        xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
        xm.stdsp  s3,s2,8
        xm.stdsp  s5,s4,16
        xm.stdsp  s7,s6,24
    {   li t3, 0                              ;   sw s8, 4                          (sp)}
    {   srli t3, N, 3                           ;   xm.vsetc t3}
#if (XS3_CONFIG_MIN_FFT_LEN <= 4)
    { nop                                           ;   xm.brff t3, .L_fft_length_4                 }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
    { nop                                           ;   xm.bu .L_pre_boggle                        }
#endif


#if (CONFIG_MIN_FFT_LEN <= 4)
.L_fft_length_4:

    // If the FFT length is 4, just do the work here. This keeps the code below simpler.
    { nop                                           ;   lw s2,4                            ( X)}
    { nop                                           ;   lw s3,16                            ( X)}
    { nop                                           ;   sw s2,16 /* X[2].re <- X[0].im */   ( X)}
    { nop                                           ;   sw s3,4 /* X[0].im <- X[2].re */   ( X)}
        xm.lddi  s2,s3, 8(X)
        xm.lddi  s4,s5, 24(X)
    {   sub s8, s2, s5                         ;   add t3, s3, s4                         }
        xm.stdi  s8,t3, 8(X)
    {   add s8, s2, s5                         ;   sub t3, s4, s3                         }
        xm.stdi  s8,t3, 24(X)
    { nop                                           ;   xm.vldd X}
    { nop                                           ;   xm.vstd X}
    { nop                                           ;   xm.vgetc t3}
    {   mv s2, t3                             ;   xm.bu .L_finish2                            }
.L_finish2:


    {   li a0, 31                              ; nop                                           }
    {   xm.zexti s2, 5                              ; nop                                           }
    {   sub a0, a0, s2                          ;   lw s8, 4                          (sp)}

        xm.lddsp  s3,s2,8
        xm.lddsp  s5,s4,16
        xm.lddsp  s7,s6,24
        xm.retsp (NSTACKWORDS)*4/* Multiple XAT warnings: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in retsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate", 'RETSP operand may need scaling' */

#endif

.L_pre_boggle:

#define DC_re   x12
#define DC_im   x13
#define Ny_re   x18
#define Ny_im   x19

    // Pre-boggle the DC and Nyquist bins so we can do everything on the VPU
    // Wait, is it faster to just compute the results and hold onto them...?

    {   srli s6, N, 1                            ; nop                                           }
        xm.lddi  DC_re,DC_im, 0(X)
        xm.ldd  Ny_re,Ny_im, s6(X)
        srai DC_re, DC_re, 1
        srai DC_im, DC_im, 1
        srai Ny_re, Ny_re, 1
        srai Ny_im, Ny_im, 1
    {   xm.add s7, DC_re, DC_im                    ;   xm.sub t3, Ny_re, Ny_im                  }
        xm.stdi  s7,t3, 0(X)
    {   xm.add s7, Ny_re, Ny_im                    ;   xm.sub t3, DC_im, DC_re                  }
        xm.std  s7,t3, s6(X)



#define X_lo    x12
#define X_hi    x13
#define i       x18
#define _32     x19

    // Now go through and compute the outputs

   sh2add X_hi, N, X

       li x28, 0                                                                           /* Translation error on this line: unexpected token at position 92. */ 
    {   srli i, N, 3                             ;   xm.vsetc t3}
lui t3, %hi(vpu_vec_complex_neg_j)
        addi t3,t3, %lo(vpu_vec_complex_neg_j)
    {   mv X_lo, X                             ;   xm.vldc t3}
lui t3, %hi(vpu_vec_complex_conj_op)
        addi t3,t3, %lo(vpu_vec_complex_conj_op)
    {   li _32, 32                             ;   xm.bu .L_syzygy                            }

.p2align 4
.L_syzygy:
        {   addi i, i, -1                             ;   xm.vldd X_hi}
        { nop                                           ;   xm.vcmr0                               }
        { nop                                           ;   xm.vcmi0                                }
        { nop                                           ;   xm.vladsb X_lo}
        {   add X_lo, X_lo, _32                     ;   xm.vstd X_lo}
        { nop                                           ;   xm.vlmul0 t3}
        {   add X_hi, X_hi, _32                     ;   xm.vstr X_hi}
        { nop                                           ;   xm.bt i, .L_syzygy                     }

    
        sh2add X, N, X
    {   srli N, N, 1                             ;   xm.vgetc t3}
    {   mv s2, t3                             ; nop                                           }
        call vect_complex_s32_tail_reverse


.L_finish:


    {   li a0, 31                              ; nop                                           }
    {   xm.zexti s2, 5                              ; nop                                           }
    {   sub a0, a0, s2                          ;   lw s8, 4                          (sp)}

        xm.lddsp  s3,s2,8
        xm.lddsp  s5,s4,16
        xm.lddsp  s7,s6,24
        xm.retsp (NSTACKWORDS)*4/* Multiple XAT warnings: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in retsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate", 'RETSP operand may need scaling' */

//.cc_bottom FUNCTION_NAME.function;  /* Translation error on this line: unexpected token at position 33. */ 
.set FUNCTION_NAME.nstackwords,NSTACKWORDS + vect_complex_s32_tail_reverse.nstackwords;      /* Translation error on this line: unexpected token at position 86. */ 
                                                .global FUNCTION_NAME.nstackwords;  /* Translation error on this line: unexpected token at position 81. */ 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores;  /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers;  /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends;  /* Translation error on this line: unexpected token at position 32. */ 
.L_function_end: 
    .size FUNCTION_NAME, .L_function_end - FUNCTION_NAME




#endif //defined(__VX4B__)
