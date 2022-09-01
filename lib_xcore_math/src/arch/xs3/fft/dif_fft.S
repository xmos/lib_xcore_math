// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


/*  
    void fft_dif_forward (
        complex_s32_t * x, 
        unsigned n, 
        headroom_t* hr, 
        exponent_t* exp);
    
    void fft_dif_inverse (
        complex_s32_t* x, 
        unsigned n, 
        headroom_t* hr, 
        exponent_t* exp);
*/


#define NSTACKWORDS (32)

#define STACK_HR        (0)
#define STACK_N         (8)
#define STACK_EXP       (9)

#define x_p 			r0 
#define n 				r1 
#define hr_p            r2
#define exp_minus_one   hr_p
#define twiddle_lut_p 	r3
#define _32             r4
#define j               r5
#define k               r6
#define a               r7
#define b               r8
#define exp_modifier    r9
#define s               r10


.text
.issue_mode  dual
.globl	fft_dif_forward
.type	fft_dif_forward,@function
.cc_top fft_dif_forward.function, fft_dif_forward

.align 4
fft_dif_forward:
	{   dualentsp NSTACKWORDS                                                               }
	{   std r4, hr_p, sp[0]                                                                 }
	{   std r5, r6, sp[1]                                                                   }
	{   std r7, r8, sp[2]                                                                   }
	{   std r9, r10, sp[3]                                                                  }
    
    {                                           ;   stw r3, sp[STACK_EXP]                   }
        ldaw r11, cp[xmath_dif_fft_lut_size]
    {   ldc r5, 32                              ;   ldw r4, r11[0]                          }
        ldaw r11, cp[xmath_dif_fft_lut]
    {   add twiddle_lut_p, r11, r4              ;   shl r4, n, 3                            }
    {   add twiddle_lut_p, twiddle_lut_p, r5    ;                                           }
    {   sub twiddle_lut_p, twiddle_lut_p, r4    ;                                           }

dif_fft_impl_start:
	{   ldc s, 31                               ;   ldw r11, hr_p[0]                        }
	{   sub s, s, r11                           ;   shr j, n, 2                             }
	{   ldaw r11, cp[fft_hr_lut]                                                            }
	{   ldc _32, 32                             ;   ldw r11, r11[s]                         }
	{   mov exp_modifier, r11                   ;   vsetc r11                               }

	{   shr s, n, 3                             ;   stw n, sp[STACK_N]                      }
	{   mov r11, x_p;                           ;   bf s, dif_fft_last_two_rounds_4_point   }

	{   mul b, n, _32                                                                       } //astew: same as `shl b, n, 5`? Unnecessary instruction?
	{   shr b, b, 3                             ;   mov a, _32                              } //astew: `shl b, n, 2`
	{   shr n, n, 4                             ;                                           } 
	{   ldaw r11, cp[fft_hr_lut]                                                            }
	{   mov s, r11                              ;   sub k, b, _32                           }

dif_fft_round_loop:
	dif_fft_outer_loop:
		{   add r11, x_p, k                         ;   mov j, a                                }
	 	{   add twiddle_lut_p, twiddle_lut_p, _32   ;   vldc twiddle_lut_p[0]                   }

		dif_fft_inner_loop:
			{   add r11, r11, b                         ;   vldr r11[0]                             }
			{   sub r11, r11, b                         ;   vladsb r11[0]                           }
			{   add r11, r11, b                         ;   vstr r11[0]                             }
			{   sub j, j, _32                           ;   vcmr                                    }
			{                                           ;   vcmi                                    }
			{                                           ;   vstr r11[0]                             }
			{   add r11, r11, b                         ;   bt j, dif_fft_inner_loop                }

		{   sub k, k, _32                           ;   bt k, dif_fft_outer_loop                }

	{   shr b, b, 1                             ;   vgetc r11                               }
	{   shl a, a, 1                             ;   zext r11, 5                             }
	{   sub k, b, _32                           ;   ldw r11, s[r11]                         }
	{   add exp_modifier, exp_modifier, r11     ;   vsetc r11                               }

	{   shr n, n, 1                             ;   bt n, dif_fft_round_loop                }
	
dif_fft_last_two_rounds:
	{   nop /*TODO make this an align */                                                    }
	{   mov r11, x_p                            ;   ldw n, sp[STACK_N]                      }
	{   shr j, n, 2                             ;                                           }	

dif_fft_last_two_rounds_loop:
	{                                           ;   vldr r11[0]                             }
	{   sub j, j, 1                             ;   vftff                                   }
	{   add r11, r11, _32                       ;   vstr r11[0]                             }

	dif_fft_last_two_rounds_4_point:
	{                                           ;   vldr r11[0]                             }
	{   sub j, j, 1                             ;   vftff                                   }
	{   add r11, r11, _32                       ;   vstr r11[0]                             }
	{                                           ;   bt j, dif_fft_last_two_rounds_loop      }

dif_fft_done:
	
	//update the hr
	{   ldc s, 31                               ;   vgetc r11                               }
	{   zext r11, 5                                                                         }
	{   sub s, s, r11	                        ;                                           }
	{   ldd r4, hr_p, sp[0]                                                                 }
	{                                           ;   stw s, hr_p[0]                          }

	//update the exponent
	{                                           ;   ldw r11, sp[STACK_EXP]                  }
	{                                           ;   ldw s, r11[0]                           }
	{   ashr exp_modifier, exp_modifier, 16                                                 }
	{   add s, s, exp_modifier                  ;                                           }
	{                                           ;   stw s, r11[0]                           }

	//restore the regs
	{   ldd r5, r6, sp[1]                                                                   }
	{   ldd r7, r8, sp[2]                                                                   }
	{   ldd r9, r10, sp[3]                                                                  }
	{   retsp NSTACKWORDS                       ;                                           }
	
	.cc_bottom fft_dif_forward.function
	.set	fft_dif_forward.nstackwords,NSTACKWORDS
	.globl	fft_dif_forward.nstackwords
	.set	fft_dif_forward.maxcores,1
	.globl	fft_dif_forward.maxcores
	.set	fft_dif_forward.maxtimers,0
	.globl	fft_dif_forward.maxtimers
	.set	fft_dif_forward.maxchanends,0
	.globl	fft_dif_forward.maxchanends
.L_fft_dif_forward:
	.size	fft_dif_forward, .L_fft_dif_forward-fft_dif_forward










	.text
    .issue_mode     dual
	.globl	        fft_dif_inverse
	.type	        fft_dif_inverse, @function
	.cc_top         fft_dif_inverse.function, fft_dif_inverse

.align 4
fft_dif_inverse:
	{   dualentsp NSTACKWORDS                                                               }
	{   std r4, hr_p, sp[0]                                                                 }
	{   std r5, r6, sp[1]                                                                   }
	{   std r7, r8, sp[2]                                                                   }
	{   std r9, r10, sp[3]                                                                  }
    
    {                                           ;   stw r3, sp[STACK_EXP]                   }
        ldaw r11, cp[xmath_dif_fft_lut_size]
    {   ldc r5, 32                              ;   ldw r4, r11[0]                          }
        ldaw r11, cp[xmath_dif_fft_lut]
    {   add twiddle_lut_p, r11, r4              ;   shl r4, n, 3                            }
    {   add twiddle_lut_p, twiddle_lut_p, r5    ;                                           }
    {   sub twiddle_lut_p, twiddle_lut_p, r4    ;                                           }

dif_ifft_impl_start:
	{   ldc s, 31                               ;   ldw r11, hr_p[0]                        }
	{   sub s, s, r11                           ;   shr j, n, 2                             }
	{   ldaw r11, cp[fft_hr_lut]                                                            }
	{   ldc _32, 32                             ;   ldw r11, r11[s]                         }
	{   mov exp_modifier, r11                   ;   vsetc r11                               }

	{   shr s, n, 3                             ;   stw n, sp[STACK_N]                      }
	{   mov r11, x_p;                           ;   bf s, dif_ifft_last_two_rounds_4_point  }

	{   mul b, n, _32                                                                       } 
	{   shr b, b, 3                             ;   mov a, _32                              }
	{   sub k, b, _32                           ;   shr n, n, 4                             }
	{   ldaw r11, cp[fft_hr_lut]                                                            }
	{   mov s, r11                              ;   ldw exp_minus_one, r11[0]               }

dif_ifft_round_loop:
	dif_ifft_outer_loop:
		{   add r11, x_p, k                         ;   mov j, a                                }
	 	{   add twiddle_lut_p, twiddle_lut_p, _32   ;   vldc twiddle_lut_p[0]                   }

		dif_ifft_inner_loop:
			{   add r11, r11, b                         ;   vldr r11[0]                             }
			{   sub r11, r11, b                         ;   vladsb r11[0]                           }
			{   add r11, r11, b                         ;   vstr r11 [0]                            }
			{   sub j, j, _32                           ;   vcmcr                                   }
			{                                           ;   vcmci                                   }
			{                                           ;   vstr r11 [0]                            }
			{   add r11, r11, b                         ;   bt j, dif_ifft_inner_loop               }

		{   sub k, k, _32                           ;   bt k, dif_ifft_outer_loop               }

	{   add exp_modifier, exp_modifier, exp_minus_one                                       }
	{   shr b, b, 1                             ;   vgetc r11                               }
	{   shl a, a, 1                             ;   zext r11, 5                             }
	{   sub k, b, _32                           ;   ldw r11, s[r11]                         }
	{   add exp_modifier, exp_modifier, r11     ;   vsetc r11                               }

	{   shr n, n, 1                             ;   bt n, dif_ifft_round_loop               }
	
dif_ifft_last_two_rounds:
	{    nop /*TODO make this an align*/                                                    }
	{   mov r11, x_p                            ;   ldw n, sp[STACK_N]                      }
	{   shr j, n, 2                             ;                                           }	

dif_ifft_last_two_rounds_loop:
	{                                           ;   vldr r11[0]                             }
	{   sub j, j, 1                             ;   vftfb                                   }
	{   add r11, r11, _32                       ;   vstr r11[0]                             }

	dif_ifft_last_two_rounds_4_point:
	{                                           ;   vldr r11[0]                             }
	{   sub j, j, 1                             ;   vftfb                                   }
	{   add r11, r11, _32                       ;   vstr r11[0]                             }
	{                                           ;   bt j, dif_ifft_last_two_rounds_loop     }

dif_ifft_done:
	//update the hr
	{   ldc s, 31                               ;   vgetc r11                               }
	{   zext r11, 5                             ;                                           }
    {   sub s, s, r11                           ;                                           }	
    {   ldd r4, hr_p, sp[0]                                                                 }
    {                                           ;   stw s, hr_p[0]                          }

	//update the exponent
	{                                           ;   ldw r11, sp[STACK_EXP]                  }
	{                                           ;   ldw s, r11[0]                           }
	{   ashr exp_modifier, exp_modifier, 16                                                 }
	{   sub exp_modifier, exp_modifier, 2                                                   }
	{   add s, s, exp_modifier                  ;                                           }
	{                                           ;   stw s, r11[0]                           }

	//restore the regs
    {   ldd r5, r6, sp[1]                                                                   }
    {   ldd r7, r8, sp[2]                                                                   }
    {   ldd r9, r10, sp[3]                                                                  }
	{                                           ;   retsp NSTACKWORDS                       }
	
	.cc_bottom fft_dif_inverse.function
	.set	fft_dif_inverse.nstackwords,NSTACKWORDS
	.globl	fft_dif_inverse.nstackwords
	.set	fft_dif_inverse.maxcores,1
	.globl	fft_dif_inverse.maxcores
	.set	fft_dif_inverse.maxtimers,0
	.globl	fft_dif_inverse.maxtimers
	.set	fft_dif_inverse.maxchanends,0
	.globl	fft_dif_inverse.maxchanends
.L_fft_dif_inverse:
	.size	fft_dif_inverse, .L_fft_dif_inverse-fft_dif_inverse




#endif //defined(__XS3A__)