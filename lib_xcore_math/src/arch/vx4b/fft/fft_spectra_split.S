// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


/*  
headroom_t fft_spectra_split(
    complex_s32_t* X,
    const unsigned N);
*/


#define FUNCTION_NAME   fft_spectra_split
#define NSTACKWORDS     (32)

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
    {   srli s8, N, 3                           ;   xm.vsetc t3}

  #if (XS3_CONFIG_MIN_FFT_LEN <= 4)
    { nop                                           ;   xm.bt s8, .L_split_the_spectrum           }
  #endif

#if (CONFIG_MIN_FFT_LEN <= 4)
.L_fft_length_4:

    // If the FFT length is 4, just do the work here. This keeps the code below simpler.
    {   xm.mkmski s8, 8                            ; nop                                           }
    {   slli t3, s8, 16                        ;   lw s2,4                            ( X)}
    {   add s8, s8, t3                       ;   lw s3,16                            ( X)}
    {   li t3, 1                              ;   sw s2,16 /* X[2].re <- X[0].im */   ( X)}
    {   xm.not s8, s8                            ;   sw s3,4 /* X[0].im <- X[2].re */   ( X)}
        xm.vlashr X, t3
        xm.vstrpv X, s8
        xm.lddi  s2,s3, 8(X)
        xm.lddi  s4,s5, 24(X)
    {   add s8, s2, s4                         ;   sub t3, s3, s5                         }
        xm.stdi  s8,t3, 8(X)
    {   add s8, s3, s5                         ;   sub t3, s4, s2                         }
        xm.stdi  s8,t3, 24(X)
    { nop                                           ;   xm.vldd X}
    { nop                                           ;   xm.vstd X}
    { nop                                           ;   xm.bu .L_finish                            }

#endif



.L_split_the_spectrum:

    // First, reverse the tail
    {   mv s2, X                               ;   mv s3, N                               }
        sh2add X, N, X
    {   srli N, N, 1                             ; nop                                           }
        call vect_complex_s32_tail_reverse
    {   mv X, s2                               ;   mv N, s3                               }

#define X_lo    X
#define i       x12
#define _32     x13
#define X_hi    x18
#define DC_im   x19
#define DC_re   x20
#define Ny_im   x21
#define Ny_re   x22

    // x = [DC.re - Ny.im, Ny.re + DC.im, DC.re + Ny.im, -Ny.re + DC.im]

    // If I set [X[0].re, X[0].im, X[K].re, X[k].im] to the vector above, then I can just compute
    // the results for bins 0 and K along with everything else. Then I'm guaranteed that the number
    // of elements is a multiple of 4, which means this loop will have no tail, AND it will have
    // captured the headroom of the vector (although it will be the lesser of the lower and upper
    // halves)
    {   li _32, 32                             ;   srli i, N, 1                             }
        xm.lddi  DC_re,DC_im, 0(X)
        xm.ldd  Ny_re,Ny_im, i(X)
    {   sub s7, DC_re, Ny_im                   ;   add t3, DC_im, Ny_re                   }
        xm.stdi  s7,t3, 0(X)
    {   add s7, DC_re, Ny_im                    ;   sub t3, DC_im, Ny_re                   }
        xm.std  s7,t3, i  (X)

#undef DC_re
#undef DC_im
#undef Ny_re
#undef Ny_im

#define conj_vec    x19


    sh2add X_hi, N, X_lo
       li x28, 0x0080                                                                      /* Translation error on this line: unexpected token at position 92. */ 
    {   srli i, i, 2                             ;   xm.vsetc t3}
lui t3, %hi(vpu_vec_complex_neg_j)
        addi t3,t3, %lo(vpu_vec_complex_neg_j)
    { nop                                           ;   xm.vldc t3}
lui t3, %hi(vpu_vec_complex_conj_op)
        addi t3,t3, %lo(vpu_vec_complex_conj_op)
    { nop                                           ;   xm.bu .L_syzygy                            }

.p2align 4
.L_syzygy:
        {   addi i, i, -1                             ;   xm.vldr t3}
        { nop                                           ;   xm.vlmul0 X_hi}
        { nop                                           ;   xm.vladsb X_lo}
        {   add X_lo, X_lo, _32                     ;   xm.vstr X_lo}
        { nop                                           ;   xm.vcmr0                                   }
        { nop                                           ;   xm.vcmi0                                    }
        {   add X_hi, X_hi, _32                     ;   xm.vstr X_hi}
        { nop                                           ;   xm.bt i, .L_syzygy                         }


.L_finish:
    {   li a0, 31                              ;   xm.vgetc t3}
    {   xm.zexti t3, 5                             ; nop                                           }
    {   sub a0, a0, t3                         ;   lw s8, 4                          (sp)}

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
