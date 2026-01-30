// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


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


#define NSTACKWORDS (12)

#define STACK_N         (8)
#define STACK_EXP       (9)

#define x_p 			x10 
#define n 				x11 
#define hr_p            x12
#define exp_minus_one   hr_p
#define twiddle_lut_p 	x13
#define _32             x18
#define j               x19
#define k               x20
#define a               x21
#define b               x22
#define exp_modifier    x23
#define s               x24


.text
.globl	fft_dif_forward
.type	fft_dif_forward,@function

.p2align 2
fft_dif_forward:

	xm.entsp (NSTACKWORDS)*4
	xm.stdsp  x18,x12,0*8
	xm.stdsp  x19,x20,1*8
	xm.stdsp  x21,x22,2*8
	xm.stdsp  x23,x24,3*8

    { nop                               ; sw a3, (STACK_EXP)*4                (sp) }
    la t3, xmath_dif_fft_lut_size
    { li s3, 32                         ; lw s2,0                            ( t3) }
	la t3, xmath_dif_fft_lut
    { add twiddle_lut_p, t3, s2         ; xm.shli s2, n, 3                         }
    { add twiddle_lut_p, twiddle_lut_p, s3 ; nop                                   }
    { sub twiddle_lut_p, twiddle_lut_p, s2 ; nop                                   }

dif_fft_impl_start:
	{ li s, 31                             ; lw t3,0                       ( hr_p) }
	{ sub s, s, t3                         ; srli j, n, 2                          }
	
	la t3, fft_hr_lut	
	
	{ li _32, 32                           ; xm.ldw t3,s(t3)                       }
	{ mv exp_modifier, t3                  ; xm.vsetc t3                           }

	{ srli s, n, 3                         ; sw n, (STACK_N)*4                (sp) }
	{ mv t3, x_p                           ; xm.brff s, dif_fft_last_two_rounds_4_point } 

	mul b, n, _32   
	{ srli b, b, 3                         ; mv a, _32                             } //astew: `shl b, n, 2`
	{ srli n, n, 4                         ; nop                                   } 
	
	la t3, fft_hr_lut	
		
	
	{ mv s, t3                             ; sub k, b, _32                         }

dif_fft_round_loop:
	dif_fft_outer_loop:
		{ add t3, x_p, k                      ; mv j, a                               }
	 	{ add twiddle_lut_p, twiddle_lut_p, _32 ; xm.vldc twiddle_lut_p               }

		dif_fft_inner_loop:
			{ add t3, t3, b                      ; xm.vldr t3                            }
			{ sub t3, t3, b                      ; xm.vladsb t3                          }
			{ add t3, t3, b                      ; xm.vstr t3                            }
			{ sub j, j, _32                      ; xm.vcmr0                              }
			{ nop                                ; xm.vcmi0                              }
			{ nop                                ; xm.vstr t3                            }
			{ add t3, t3, b                      ; xm.bt j, dif_fft_inner_loop           }

		{ sub k, k, _32                       ; xm.bt k, dif_fft_outer_loop           }

	{ srli b, b, 1                         ; xm.vgetc t3                           }
	{ xm.shli a, a, 1                      ; xm.zexti t3, 5                        }
	{ sub k, b, _32                        ; xm.ldw t3, t3                     (s) }
	{ add exp_modifier, exp_modifier, t3   ; xm.vsetc t3                           }

	{ srli n, n, 1                         ; xm.bt n, dif_fft_round_loop           }
	
dif_fft_last_two_rounds:
	{ mv t3, x_p                           ; lw n, (STACK_N)*4                (sp) }
	{ srli j, n, 2                         ; nop                                   }	

dif_fft_last_two_rounds_loop:
	{ nop                                  ; xm.vldr t3                            }
	{ addi j, j, -1                        ; xm.vftff                              }
	{ add t3, t3, _32                      ; xm.vstr t3                            }

	dif_fft_last_two_rounds_4_point:
	{ nop                                  ; xm.vldr t3                            }
	{ addi j, j, -1                        ; xm.vftff                              }
	{ add t3, t3, _32                      ; xm.vstr t3                            }
	{ nop                                  ; xm.bt j, dif_fft_last_two_rounds_loop }

dif_fft_done:
	
	//update the hr
	{ li s, 31                             ; xm.vgetc t3                           }
	{ xm.zexti t3, 5                       ; nop                                   }
	{ sub s, s, t3                         ; nop                                   }
	xm.lddsp  x18,x12,0*8
	{ nop                                  ; sw s,0                        ( hr_p) }

	//update the exponent
	{ nop                                  ; lw t3, (STACK_EXP)*4 (sp)             }
	{ nop                                  ; lw s,0                          ( t3) }
	
	srai exp_modifier, exp_modifier, 16
	{ add s, s, exp_modifier               ; nop                                   }
	{ nop                                  ; sw s,0                          ( t3) }

	xm.lddsp  x19,x20,1*8
	xm.lddsp  x21,x22,2*8
	xm.lddsp  x23,x24,3*8

	xm.retsp (NSTACKWORDS)*4

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
	.globl	        fft_dif_inverse
	.type	        fft_dif_inverse, @function

.p2align 2
fft_dif_inverse:
	xm.entsp (NSTACKWORDS)*4
	xm.stdsp  x18,x12,0*8
	xm.stdsp  x19,x20,1*8
	xm.stdsp  x21,x22,2*8
	xm.stdsp  x23,x24,3*8
    
    { nop                                  ; sw a3, (STACK_EXP)*4             (sp) }
	la t3, xmath_dif_fft_lut_size
    { li s3, 32                            ; lw s2,0                         ( t3) }
	la t3, xmath_dif_fft_lut
    { add twiddle_lut_p, t3, s2            ; xm.shli s2, n, 3                      }
    { add twiddle_lut_p, twiddle_lut_p, s3 ; nop                                   }
    { sub twiddle_lut_p, twiddle_lut_p, s2 ; nop                                   }

dif_ifft_impl_start:
	{ li s, 31                             ; lw t3,0                       ( hr_p) }
	{ sub s, s, t3                         ; srli j, n, 2                          }
	
	la t3, fft_hr_lut	
	
	{ li _32, 32                           ; xm.ldw t3,s( t3)                      }
	{ mv exp_modifier, t3                  ; xm.vsetc t3                           }

	{ srli s, n, 3                         ; sw n, (STACK_N)*4                (sp) }
	{ mv t3, x_p                           ; xm.brff s, dif_ifft_last_two_rounds_4_point     }

	mul b, n, _32                                                                        
	{ srli b, b, 3                         ; mv a, _32                             }
	{ sub k, b, _32                        ; srli n, n, 4                          }

	la t3, fft_hr_lut	

	{ mv s, t3                             ; lw exp_minus_one,0              ( t3) }

dif_ifft_round_loop:
	dif_ifft_outer_loop:
		{ add t3, x_p, k                      ; mv j, a                               }
	 	{ add twiddle_lut_p, twiddle_lut_p, _32 ; xm.vldc twiddle_lut_p                 }

		dif_ifft_inner_loop:
			{ add t3, t3, b                      ; xm.vldr t3                            }
			{ sub t3, t3, b                      ; xm.vladsb t3                          }
			{ add t3, t3, b                      ; xm.vstr t3                            }
			{ sub j, j, _32                      ; xm.vcmcr0                             }
			{ nop                                ; xm.vcmci0                             }
			{ nop                                ; xm.vstr t3                            }
			{ add t3, t3, b                      ; xm.bt j, dif_ifft_inner_loop          }

		{ sub k, k, _32                       ; xm.bt k, dif_ifft_outer_loop          }

	{ add exp_modifier, exp_modifier, exp_minus_one ; nop                          }
	{ srli b, b, 1                         ; xm.vgetc t3                           }
	{ xm.shli a, a, 1                      ; xm.zexti t3, 5                        }
	{ sub k, b, _32                        ; xm.ldw t3, t3                     (s) }
	{ add exp_modifier, exp_modifier, t3   ; xm.vsetc t3                           }

	{ srli n, n, 1                         ; xm.bt n, dif_ifft_round_loop          }
	
dif_ifft_last_two_rounds:
	{ nop                                  ; nop /*TODO make this an align*/       }
	{ mv t3, x_p                           ; lw n, (STACK_N)*4                (sp) }
	{ srli j, n, 2                         ; nop                                   }	

dif_ifft_last_two_rounds_loop:
	{ nop                                  ; xm.vldr t3                            }
	{ addi j, j, -1                        ; xm.vftfb                              }
	{ add t3, t3, _32                      ; xm.vstr t3                            }

	dif_ifft_last_two_rounds_4_point:
	{ nop                                  ; xm.vldr t3                            }
	{ addi j, j, -1                        ; xm.vftfb                              }
	{ add t3, t3, _32                      ; xm.vstr t3                            }
	{ nop                                  ; xm.bt j, dif_ifft_last_two_rounds_loop }

dif_ifft_done:
	//update the hr
	{ li s, 31                             ; xm.vgetc t3                           }
	{ xm.zexti t3, 5                       ; nop                                   }
    { sub s, s, t3                         ; nop                                   }	
       xm.lddsp x18,x12,0*8                                                            
    { nop                                  ; sw s,0                        ( hr_p) }

	//update the exponent
	{ nop                                  ; lw t3, (STACK_EXP)*4             (sp) }
	{ nop                                  ; lw s,0                          ( t3) }
	srai exp_modifier, exp_modifier, 16
	addi exp_modifier, exp_modifier, -2                                          
	{ add s, s, exp_modifier               ; nop                                   }
	{ nop                                  ; sw s,0                          ( t3) }

	//restore the regs
	xm.lddsp  x19,x20,1*8
	xm.lddsp  x21,x22,2*8
	xm.lddsp  x23,x24,3*8
    
	 xm.retsp (NSTACKWORDS)*4

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

#endif //defined(__VX4B__)
