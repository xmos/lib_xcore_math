
#if defined(__XS3A__)
#ifndef XS3_MATH_NO_ASM

/*  
    void xs3_fft_dif_s32 (
        complex_s32_t * x, 
        unsigned n, 
        headroom_t* hr, 
        const complex_s32_t* twiddle_lut, 
        exponent_t* exp);
    
    void xs3_ifft_dif_s32 (
        complex_s32_t* x, 
        unsigned n, 
        headroom_t* hr, 
        const complex_s32_t* twiddle_lut, 
        exponent_t* exp);
*/

#include "../asm_helper.h"


// In the table below the LSByte indicates the shift behavior
//  0x00 - no shift.  0x40 - left shift. 0x80 - right shift.
#define NEG1_SHL    0xffff0040
#define ZERO_SH0    0x00000000
#define POS1_SHR    0x00010080

.text
.section    .cp.rodata, "ac", @progbits
.align  4

L_hr_lut:
	.word NEG1_SHL, NEG1_SHL, NEG1_SHL, NEG1_SHL, NEG1_SHL, NEG1_SHL, NEG1_SHL, NEG1_SHL //  0 -  7 
    .word NEG1_SHL, NEG1_SHL, NEG1_SHL, NEG1_SHL, NEG1_SHL, NEG1_SHL, NEG1_SHL, NEG1_SHL //  8 - 15
	.word NEG1_SHL, NEG1_SHL, NEG1_SHL, NEG1_SHL, NEG1_SHL, NEG1_SHL, NEG1_SHL, NEG1_SHL // 16 - 23
	.word NEG1_SHL, NEG1_SHL, NEG1_SHL, NEG1_SHL, ZERO_SH0, POS1_SHR, POS1_SHR, POS1_SHR // 24 - 31


#define NSTACKWORDS (32)

#define STACK_EXP       (NSTACKWORDS+1)

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
// #define t               r11

.text
.issue_mode  dual
.globl	xs3_fft_dif_s32
.type	xs3_fft_dif_s32,@function
.cc_top xs3_fft_dif_s32.function, xs3_fft_dif_s32



.align 4
xs3_fft_dif_s32:
	{   dualentsp NSTACKWORDS                                                               }
	{   std r4, hr_p, sp[0]                                                                 }
	{   std r5, r6, sp[1]                                                                   }
	{   std r7, r8, sp[2]                                                                   }
	{   std r9, r10, sp[3]                                                                  }

dif_fft_impl_start:
	{   ldc s, 31                               ;   ldw r11, hr_p[0]                        }
	{   sub s, s, r11                           ;   shr j, n, 2                             }
	{   ldaw r11, cp[L_hr_lut]                                                              }
	{   ldc _32, 32                             ;   ldw r11, r11[s]                         }
	{   mov exp_modifier, r11                   ;   vsetc r11                               }

	{   shr s, n, 3                             ;   stw n, sp[8]                            }
	{   mov r11, x_p;                           ;   bf s, dif_fft_last_two_rounds_4_point   }

	{   mul b, n, _32                                                                       } //astew: same as `shl b, n, 5`? Unnecessary instruction?
	{   shr b, b, 3                             ;   mov a, _32                              } //astew: `shl b, n, 2`
	{   shr n, n, 4                             ;                                           } 
	{   ldaw r11, cp[L_hr_lut]                                                              }
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
	{   mov r11, x_p                            ;   ldw n, sp[8]                            }
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
	{                                           ;   ldw r11, sp[NSTACKWORDS+1]              }
	{                                           ;   ldw s, r11[0]                           }
	{   ashr exp_modifier, exp_modifier, 16                                                 }
	{   add s, s, exp_modifier                  ;                                           }
	{                                           ;   stw s, r11[0]                           }

	//restore the regs
	{   ldd r5, r6, sp[1]                                                                   }
	{   ldd r7, r8, sp[2]                                                                   }
	{   ldd r9, r10, sp[3]                                                                  }
	{   retsp NSTACKWORDS                       ;                                           }
	
	.cc_bottom xs3_fft_dif_s32.function
	.set	xs3_fft_dif_s32.nstackwords,NSTACKWORDS
	.globl	xs3_fft_dif_s32.nstackwords
	.set	xs3_fft_dif_s32.maxcores,1
	.globl	xs3_fft_dif_s32.maxcores
	.set	xs3_fft_dif_s32.maxtimers,0
	.globl	xs3_fft_dif_s32.maxtimers
	.set	xs3_fft_dif_s32.maxchanends,0
	.globl	xs3_fft_dif_s32.maxchanends
.L_xs3_fft_dif_s32:
	.size	xs3_fft_dif_s32, .L_xs3_fft_dif_s32-xs3_fft_dif_s32










	.text
    .issue_mode     dual
	.globl	        xs3_ifft_dif_s32
	.type	        xs3_ifft_dif_s32, @function
	.cc_top         xs3_ifft_dif_s32.function, xs3_ifft_dif_s32

.align 4
xs3_ifft_dif_s32:
	{   dualentsp NSTACKWORDS                                                               }
	{   std r4, hr_p, sp[0]                                                                 }
	{   std r5, r6, sp[1]                                                                   }
	{   std r7, r8, sp[2]                                                                   }
	{   std r9, r10, sp[3]                                                                  }

dif_ifft_impl_start:
	{   ldc s, 31                               ;   ldw r11, hr_p[0]                        }
	{   sub s, s, r11                           ;   shr j, n, 2                             }
	{   ldaw r11, cp[L_hr_lut]                                                              }
	{   ldc _32, 32                             ;   ldw r11, r11[s]                         }
	{   mov exp_modifier, r11                   ;   vsetc r11                               }

	{   shr s, n, 3                             ;   stw n, sp[8]                            }
	{   mov r11, x_p;                           ;   bf s, dif_ifft_last_two_rounds_4_point  }

	{   mul b, n, _32                                                                       } 
	{   shr b, b, 3                             ;   mov a, _32                              }
	{   sub k, b, _32                           ;   shr n, n, 4                             }
	{   ldaw r11, cp[L_hr_lut]                                                              }
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
	{   mov r11, x_p                            ;   ldw n, sp[8]                            }
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
	{                                           ;   ldw r11, sp[NSTACKWORDS+1]              }
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
	
	.cc_bottom xs3_ifft_dif_s32.function
	.set	xs3_ifft_dif_s32.nstackwords,NSTACKWORDS
	.globl	xs3_ifft_dif_s32.nstackwords
	.set	xs3_ifft_dif_s32.maxcores,1
	.globl	xs3_ifft_dif_s32.maxcores
	.set	xs3_ifft_dif_s32.maxtimers,0
	.globl	xs3_ifft_dif_s32.maxtimers
	.set	xs3_ifft_dif_s32.maxchanends,0
	.globl	xs3_ifft_dif_s32.maxchanends
.L_xs3_ifft_dif_s32:
	.size	xs3_ifft_dif_s32, .L_xs3_ifft_dif_s32-xs3_ifft_dif_s32



#endif //!defined(XS3_MATH_NO_ASM)
#endif //defined(__XS3A__)