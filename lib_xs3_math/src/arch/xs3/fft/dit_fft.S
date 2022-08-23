// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


/*  
    void fft_dit_forward (
        complex_s32_t * x, 
        unsigned n, 
        headroom_t* hr, 
        exponent_t* exp);
    
    void fft_dit_inverse (
        complex_s32_t* x, 
        unsigned n, 
        headroom_t* hr, 
        exponent_t* exp);
*/

#define NSTACKWORDS (32)

#define STACK_EXP       (8)

#define x_p 			r0  //astew: Value is constant. Could be thrown on stack to free up a register.
#define n 				r1 
#define hr_p            r2  //astew: register currently only used at very beginning and end.
#define twiddle_lut_p 	r3
// #define M               r4
#define _32             r4

#define j               r5
#define k               r6

#define a               r7
#define b               r8

#define exp_modifier    r9

#define s               r10
// #define t               r11

.text
.issue_mode  dual
.globl	fft_dit_forward
.type	fft_dit_forward,@function
.cc_top fft_dit_forward.function,fft_dit_forward

.align 4
fft_dit_forward:

        dualentsp NSTACKWORDS
        std r4, hr_p, sp[0]
        std r5, r6, sp[1]
        std r7, r8, sp[2]
        std r9, r10, sp[3]
    
        ldaw r11, cp[xmath_dit_fft_lut]
    {   mov twiddle_lut_p, r11                  ;   stw r3, sp[STACK_EXP]                   }

    {   ldc exp_modifier, 0                     ;   ldw r11, hr_p[0]                        }
    {   sub r11, r11, 1                         ;   bf r11, dit_fft_impl_0_bits_hr          }
    {   sub r11, r11, 1                         ;   bf r11, dit_fft_impl_1_bits_hr          }
    {   sub r11, r11, 1                         ;   bf r11, dit_fft_impl_2_bits_hr          }
    {   sub r11, r11, 1                         ;   bf r11, dit_fft_impl_3_bits_hr          }
    {                                           ;   bu dit_fft_impl_4_bits_hr               }

#define VEC_SHR     0x80
#define VEC_SHL     0x40
#define VEC_SH0     0x00

dit_fft_impl_0_bits_hr:
    {   ldc r11, VEC_SHR                        } //VEC_SHR
    {   add exp_modifier, exp_modifier, 1       ;   bu dit_fft_impl_start                   }

dit_fft_impl_1_bits_hr:
    {   ldc r11, VEC_SHR                        } //VEC_SHR
    {   add exp_modifier, exp_modifier, 1       ;   bu dit_fft_impl_start                   }

dit_fft_impl_2_bits_hr:
    {   ldc r11, VEC_SHR                        } //VEC_SHR
    {   add exp_modifier, exp_modifier, 1       ;   bu dit_fft_impl_start                   }

dit_fft_impl_3_bits_hr:
    {   ldc r11, VEC_SH0                        } //VEC_SH0
    {   add exp_modifier, exp_modifier, 0       ;   bu dit_fft_impl_start                   }

dit_fft_impl_4_bits_hr:
    {   ldc r11, VEC_SHL                        } //VEC_SHL
    {   sub exp_modifier, exp_modifier, 1       ;   bu dit_fft_impl_start                   }


dit_fft_impl_start:
    // Iterate the dit_fft_first_two_rounds_loop loop  n/4 times (via j) because vD holds 4 complex elements
    {   shr j, n, 2                             ;   vsetc r11                               }
    // have r11 point at the beginning of the data vector
    {   mov r11, x_p                            ;   ldc _32, 32                             }


    dit_fft_first_two_rounds_loop:
        // Load 4 complex elements from the data vector (already have indexes bit-reversed)
        {                                           ;   vldd r11[0]                             }   
        // Do FFT thing and decrement loop counter
        {   sub j, j, 1                             ;   vfttf                                   }   
        // Write back to data vector, and move to point at next 4 elements
        {   add r11, r11, _32                       ;   vstd r11[0]                             }   
        // Loop if there's more. Set s to n/4
        {   shr s, n, 2                             ;   bt j, dit_fft_first_two_rounds_loop     }

    // s = (n/4)-1;  if n == 4, skip the main loop.
    {   sub s, s, 1                             ;                                           }
    {                                           ;   bf s, dit_fft_done                      }

    // b = 32       ; b will shift left in each iteration of the `dit_fft_round_loop` loop
    // a = n / 8    ; a will shift right in each iteration of the `dit_fft_round_loop` loop
    // n = n / 16   ; after this we'll do log2(n)+1 executions of `dit_fft_round_loop`
    {   mov b, _32                              ;                                           } // <-- astew: seems unnecessary. Can probably drop an indstruction here.
    {   shr n, n, 4                             ;   shr a, n, 3                             }

    dit_fft_round_loop:
        {   ldaw r11, cp[fft_hr_lut]               }

        {   mov s, r11                              ;   vgetc r11                               }
        {                                           ;   zext r11, 5                             }
        {   sub k, b, _32                           ;   ldw r11, s[r11]                         }
            ashr s, r11, 16
        {   add exp_modifier, exp_modifier, s       ;   vsetc r11                               }

        dit_fft_outer_loop:
            // j is our inner loop iterator variable
            // set s to point k bytes into the data buffer
            {   mov j, a                                ;   add s, x_p, k                           }
            {   add twiddle_lut_p, twiddle_lut_p, _32   ;   vldc twiddle_lut_p[0]                   }
            {   add r11, s, b                           ;                                           } ////this might be able to go

            dit_fft_inner_loop:
                {                                           ;   vldd r11[0]                             }
                {                                           ;   vcmr                                    }
                {                                           ;   vcmi                                    }
                {   sub j, j, 1                             ;   vladsb s[0]                             }
                {   add s, s, b                             ;   vstr s [0]                              }
                {   add s, s, b                             ;   vstd r11[0]                             }
                {   add r11, s, b                           ;   bt j, dit_fft_inner_loop                }

            {   sub k, k, _32                           ;   bt k, dit_fft_outer_loop                }

        {   shl b, b, 1                             ;   shr a, a, 1                             }
        {   shr n, n, 1                             ;   bt n, dit_fft_round_loop                }
    
dit_fft_done:
    
    //update the hr
    {   vgetc r11                               ;   ldc s, 31                               }
    {   zext r11, 5                             }
        sub s, s, r11
        ldd r4, hr_p, sp[0]
        stw s, hr_p[0]

    //update the exponent
    {                                           ;   ldw r11, sp[STACK_EXP]                  }
    {                                           ;   ldw s, r11[0]                           }
    {   add s, s, exp_modifier                  ;                                           }
    {                                           ;   stw s, r11[0]                           }

        //restore the regs
        ldd r5, r6, sp[1]
        ldd r7, r8, sp[2]
        ldd r9, r10, sp[3]
        retsp NSTACKWORDS
    
    .cc_bottom  fft_dit_forward.function
    .set	    fft_dit_forward.nstackwords,NSTACKWORDS
    .globl	    fft_dit_forward.nstackwords
    .set	    fft_dit_forward.maxcores,1
    .globl	    fft_dit_forward.maxcores
    .set	    fft_dit_forward.maxtimers,0
    .globl	    fft_dit_forward.maxtimers
    .set	    fft_dit_forward.maxchanends,0
    .globl	    fft_dit_forward.maxchanends

.Ltmp0:
    .size	fft_dit_forward, .Ltmp0-fft_dit_forward














    .text
    .issue_mode     dual
    .globl	        fft_dit_inverse
    .type	        fft_dit_inverse, @function
    .cc_top         fft_dit_inverse.function, fft_dit_inverse

.align 4
fft_dit_inverse:
        dualentsp NSTACKWORDS
        std r4, hr_p, sp[0]
        std r5, r6, sp[1]
        std r7, r8, sp[2]
        std r9, r10, sp[3]
    
        ldaw r11, cp[xmath_dit_fft_lut]
    {   mov twiddle_lut_p, r11                  ;   stw r3, sp[STACK_EXP]                   }

    {   ldc exp_modifier, 0                     ;   ldw r11, hr_p[0]                        }
    {   sub r11, r11, 1                         ;   bf r11, dit_ifft_impl_0_bits_hr         }
    {   sub r11, r11, 1                         ;   bf r11, dit_ifft_impl_1_bits_hr         }
    {   sub r11, r11, 1                         ;   bf r11, dit_ifft_impl_2_bits_hr         }
    {   sub r11, r11, 1                         ;   bf r11, dit_ifft_impl_3_bits_hr         }
    {                                           ;   bu dit_ifft_impl_4_bits_hr              }

    dit_ifft_impl_0_bits_hr:
        {   ldc r11, 0x80                           } //VEC_SHR
        {   add exp_modifier, exp_modifier, 1       ;   bu dit_ifft_impl_start                  }

    dit_ifft_impl_1_bits_hr:
        {   ldc r11, 0x80                           } //VEC_SHR
        {   add exp_modifier, exp_modifier, 1       ;   bu dit_ifft_impl_start                  }

    dit_ifft_impl_2_bits_hr:
        {   ldc r11, 0x80                           } //VEC_SHR
        {   add exp_modifier, exp_modifier, 1       ;   bu dit_ifft_impl_start                  }

    dit_ifft_impl_3_bits_hr:
        {   ldc r11, 0x00                           } //VEC_SH0
        {   add exp_modifier, exp_modifier, 0       ;   bu dit_ifft_impl_start                  }

    dit_ifft_impl_4_bits_hr:
        {   ldc r11, 0x40                           } //VEC_SHL
        {   sub exp_modifier, exp_modifier, 1       ;   bu dit_ifft_impl_start                  }

dit_ifft_impl_start:
    {   shr j, n, 2                             ;   vsetc r11                               }
    {   mov r11, x_p                            ;   ldc _32, 8*4                            }


dit_ifft_first_two_rounds_loop:
    {                                           ;   vldd r11[0]                             }
    {   sub j, j, 1                             ;   vfttb                                   }
    {   add r11, r11, _32                       ;   vstd r11[0]                             }
    {   shr s, n, 2                             ;   bt j, dit_ifft_first_two_rounds_loop    }

    {   sub s, s, 1                             ;                                           }
    {   sub exp_modifier, exp_modifier, 2       ;   bf s, dit_ifft_done                     }

    {   mov a, n                                ;   mov b, _32                              }
    {   shr a, a, 3                             ;   shr n, n, 4                             }

dit_ifft_round_loop:
    {   ldaw r11, cp[fft_hr_lut]                }

    {   mov s, r11                              ;   vgetc r11                               }
    {   zext r11, 5                             ;   sub exp_modifier, exp_modifier, 1       }
    {   sub k, b, _32                           ;   ldw r11, s[r11]                         }
        ashr s, r11, 16
    {   add exp_modifier, exp_modifier, s       ;   vsetc r11                               }

    dit_ifft_outer_loop:
        {   add s, x_p, k                           ;   mov j, a                                }
         {   add twiddle_lut_p, twiddle_lut_p, _32   ;   vldc twiddle_lut_p[0]                   }
        {   add r11, s, b                           ;                                           } ////this might be able to go

        dit_ifft_inner_loop:
            {                                           ;   vldd r11[0]                             }
            {                                           ;   vcmcr                                   }
            {                                           ;   vcmci                                   }
            {   sub j, j, 1                             ;   vladsb s[0]                             }
            {   add s, s, b                             ;   vstr s [0]                              }
            {   add s, s, b                             ;   vstd r11[0]                             }
            {   add r11, s, b                           ;   bt j, dit_ifft_inner_loop               }

        {   sub k, k, _32                           ;   bt k, dit_ifft_outer_loop               }

    {   shr a, a, 1                             ;   shl b, b, 1                             }
    {   shr n, n, 1                             ;   bt n, dit_ifft_round_loop               }
    
dit_ifft_done:
    
    //update the hr
    {   ldc s, 31                               ;   vgetc r11                               }
    {   zext r11, 5                             }
        sub s, s, r11
        ldd r4, hr_p, sp[0]
        stw s, hr_p[0]

    //update the exponent
    {                                           ;   ldw r11, sp[STACK_EXP]                  }
    {                                           ;   ldw s, r11[0]                           }
    {   add s, s, exp_modifier                  ;                                           }
    {                                           ;   stw s, r11[0]                           }

        //restore the regs
        ldd r5, r6, sp[1]
        ldd r7, r8, sp[2]
        ldd r9, r10, sp[3]
        retsp NSTACKWORDS   
    
    .cc_bottom  fft_dit_inverse.function
    .set	    fft_dit_inverse.nstackwords,NSTACKWORDS
    .globl	    fft_dit_inverse.nstackwords
    .set	    fft_dit_inverse.maxcores,1
    .globl	    fft_dit_inverse.maxcores
    .set	    fft_dit_inverse.maxtimers,0
    .globl	    fft_dit_inverse.maxtimers
    .set	    fft_dit_inverse.maxchanends,0
    .globl	    fft_dit_inverse.maxchanends
.Ltmp1:
    .size	fft_dit_inverse, .Ltmp1-fft_dit_inverse




#endif //defined(__XS3A__)