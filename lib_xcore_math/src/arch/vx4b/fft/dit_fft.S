// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)

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

#define NSTACKWORDS (12)

#define STACK_EXP       (8)

#define x_p 			x10  //astew: Value is constant. Could be thrown on stack to free up a register.
#define n 				x11 
#define hr_p            x12  //astew: register currently only used at very beginning and end.
#define twiddle_lut_p 	x13
#define _32             x18

#define j               x19
#define k               x20

#define a               x21
#define b               x22

#define exp_modifier    x23

#define s               x24
// #define t               x28

.text
.globl	fft_dit_forward
.type	fft_dit_forward,@function

.p2align 2
fft_dit_forward:

xm.entsp (NSTACKWORDS)*4
xm.stdsp  hr_p,s2,0
xm.stdsp  s4,s3,8
xm.stdsp  s6,s5,16
xm.stdsp  s8,s7,24
    
la t3, xmath_dit_fft_lut
{ mv twiddle_lut_p, t3              ; sw a3, (STACK_EXP)*4             (sp) }

{ li exp_modifier, 0                ; lw t3,0                       ( hr_p) }
{ addi t3, t3, -1                   ; xm.brff t3, dit_fft_impl_0_bits_hr    }
{ addi t3, t3, -1                   ; xm.brff t3, dit_fft_impl_1_bits_hr    }
{ addi t3, t3, -1                   ; xm.brff t3, dit_fft_impl_2_bits_hr    }
{ addi t3, t3, -1                   ; xm.brff t3, dit_fft_impl_3_bits_hr    }
{ nop                               ; xm.bu dit_fft_impl_4_bits_hr          }

#define VEC_SHR     0x80
#define VEC_SHL     0x40
#define VEC_SH0     0x00

dit_fft_impl_0_bits_hr:
li x28, VEC_SHR                         //VEC_SHR
{ addi exp_modifier, exp_modifier, 1 ; xm.bu dit_fft_impl_start              }

dit_fft_impl_1_bits_hr:
li x28, VEC_SHR                        //VEC_SHR
{ addi exp_modifier, exp_modifier, 1 ; xm.bu dit_fft_impl_start              }

dit_fft_impl_2_bits_hr:
li x28, VEC_SHR                         //VEC_SHR
{ addi exp_modifier, exp_modifier, 1 ; xm.bu dit_fft_impl_start              }

dit_fft_impl_3_bits_hr:
li x28, VEC_SH0                         //VEC_SH0
{ addi exp_modifier, exp_modifier, 0 ; xm.bu dit_fft_impl_start              }

dit_fft_impl_4_bits_hr:
li x28, VEC_SHL                         //VEC_SHL
{ addi exp_modifier, exp_modifier, -1 ; xm.bu dit_fft_impl_start              }


dit_fft_impl_start:
    // Iterate the dit_fft_first_two_rounds_loop loop  n/4 times (via j) because vD holds 4 complex elements
    { srli j, n, 2                      ; xm.vsetc t3                           }
    // have x28 point at the beginning of the data vector
    { mv t3, x_p                        ; li _32, 32                            }


    dit_fft_first_two_rounds_loop:
        // Load 4 complex elements from the data vector (already have indexes bit-reversed)
        { nop                           ; xm.vldd t3                            }   
        // Do FFT thing and decrement loop counter
        { addi j, j, -1                 ; xm.vfttf                              }   
        // Write back to data vector, and move to point at next 4 elements
        { add t3, t3, _32               ; xm.vstd t3                            }   
        // Loop if there's more. Set s to n/4
        { srli s, n, 2                  ; xm.bt j, dit_fft_first_two_rounds_loop }

    // s = (n/4)-1;  if n == 4, skip the main loop.
    { addi s, s, -1                     ; nop                                   }
    { nop                               ; xm.brff s, dit_fft_done               }

    // b = 32       ; b will shift left in each iteration of the `dit_fft_round_loop` loop
    // a = n / 8    ; a will shift right in each iteration of the `dit_fft_round_loop` loop
    // n = n / 16   ; after this we'll do log2(n)+1 executions of `dit_fft_round_loop`
    { mv b, _32                         ; nop                                   } // <-- astew: seems unnecessary. Can probably drop an indstruction here.
    { srli n, n, 4                      ; srli a, n, 3                          }

    dit_fft_round_loop:
       la x28, fft_hr_lut	
        { mv s, t3                      ; xm.vgetc t3                           }
        { nop                           ; xm.zexti t3, 5                        }
        { sub k, b, _32                 ; xm.ldw t3, t3                         (s) }
            srai s, t3, 16
        { add exp_modifier, exp_modifier, s ; xm.vsetc t3                           }

        dit_fft_outer_loop:
            // j is our inner loop iterator variable
            // set s to point k bytes into the data buffer
            { mv j, a                   ; add s, x_p, k                         }
            { add twiddle_lut_p, twiddle_lut_p, _32 ; xm.vldc twiddle_lut_p                 }
            { add t3, s, b              ; nop                                   } ////this might be able to go

            dit_fft_inner_loop:
                { nop                   ; xm.vldd t3                            }
                { nop                   ; xm.vcmr0                              }
                { nop                   ; xm.vcmi0                              }
                { addi j, j, -1         ; xm.vladsb s                           }
                { add s, s, b           ; xm.vstr s                             }
                { add s, s, b           ; xm.vstd t3                            }
                { add t3, s, b          ; xm.bt j, dit_fft_inner_loop           }

            { sub k, k, _32             ; xm.bt k, dit_fft_outer_loop           }

        { xm.shli b, b, 1               ; srli a, a, 1                          }
        { srli n, n, 1                  ; xm.bt n, dit_fft_round_loop           }
    
dit_fft_done:
    
//update the hr
{ xm.vgetc t3                       ; li s, 31                              }
xm.zexti t3, 5                             
sub s, s, t3
xm.lddsp  hr_p,s2,0
sw s,0( hr_p)

//update the exponent
{ nop                               ; lw t3, (STACK_EXP)*4            (sp) }
{ nop                               ; lw s,0                         ( t3) }
{ add s, s, exp_modifier            ; nop                                  }
{ nop                               ; sw s,0                         ( t3) }

//restore the regs
xm.lddsp  s4,s3,8
xm.lddsp  s6,s5,16
xm.lddsp  s8,s7,24
xm.retsp (NSTACKWORDS)*4
    
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
    .globl	        fft_dit_inverse
    .type	        fft_dit_inverse, @function

.p2align 2
fft_dit_inverse:
xm.entsp (NSTACKWORDS)*4
xm.stdsp  hr_p,s2,0
xm.stdsp  s4,s3,8
xm.stdsp  s6,s5,16
xm.stdsp  s8,s7,24
    
la t3, xmath_dit_fft_lut
{ mv twiddle_lut_p, t3              ; sw a3, (STACK_EXP)*4                   (sp) }

{ li exp_modifier, 0                ; lw t3,0                        ( hr_p) }
{ addi t3, t3, -1                   ; xm.brff t3, dit_ifft_impl_0_bits_hr   }
{ addi t3, t3, -1                   ; xm.brff t3, dit_ifft_impl_1_bits_hr   }
{ addi t3, t3, -1                   ; xm.brff t3, dit_ifft_impl_2_bits_hr   }
{ addi t3, t3, -1                   ; xm.brff t3, dit_ifft_impl_3_bits_hr   }
{ nop                               ; xm.bu dit_ifft_impl_4_bits_hr         }

dit_ifft_impl_0_bits_hr:
    li x28, 0x80                            //VEC_SHR
{ addi exp_modifier, exp_modifier, 1 ; xm.bu dit_ifft_impl_start             }

dit_ifft_impl_1_bits_hr:
    li x28, 0x80                            //VEC_SHR
{ addi exp_modifier, exp_modifier, 1 ; xm.bu dit_ifft_impl_start             }

dit_ifft_impl_2_bits_hr:
    li x28, 0x80                            //VEC_SHR
{ addi exp_modifier, exp_modifier, 1 ; xm.bu dit_ifft_impl_start             }

dit_ifft_impl_3_bits_hr:
    li x28, 0x00                           //VEC_SH0
{ addi exp_modifier, exp_modifier, 0 ; xm.bu dit_ifft_impl_start             }

dit_ifft_impl_4_bits_hr:
    li x28, 0x40                            //VEC_SHL
{ addi exp_modifier, exp_modifier, -1 ; xm.bu dit_ifft_impl_start             }

dit_ifft_impl_start:
{ srli j, n, 2                      ; xm.vsetc t3                           }
{ mv t3, x_p                        ; xm.ldcu _32, 8*4                      }


dit_ifft_first_two_rounds_loop:
    { nop                               ; xm.vldd t3                            }
    { addi j, j, -1                     ; xm.vfttb                              }
    { add t3, t3, _32                   ; xm.vstd t3                            }
    { srli s, n, 2                      ; xm.bt j, dit_ifft_first_two_rounds_loop }

    { addi s, s, -1                     ; nop                                   }
    { addi exp_modifier, exp_modifier, -2 ; xm.brff s, dit_ifft_done              }

    { mv a, n                           ; mv b, _32                             }
    { srli a, a, 3                      ; srli n, n, 4                          }

dit_ifft_round_loop:
    la x28, fft_hr_lut	
    { mv s, t3                          ; xm.vgetc t3                           }
    { xm.zexti t3, 5                    ; addi exp_modifier, exp_modifier, -1   }
    { sub k, b, _32                     ; xm.ldw t3, t3                         (s) }
        srai s, t3, 16
    { add exp_modifier, exp_modifier, s ; xm.vsetc t3                           }

    dit_ifft_outer_loop:
        { add s, x_p, k                 ; mv j, a                               }
         { add twiddle_lut_p, twiddle_lut_p, _32 ; xm.vldc twiddle_lut_p                 }
        { add t3, s, b                  ; nop                                   } ////this might be able to go

        dit_ifft_inner_loop:
            { nop                       ; xm.vldd t3                            }
            { nop                       ; xm.vcmcr0                             }
            { nop                       ; xm.vcmci0                             }
            { addi j, j, -1             ; xm.vladsb s                           }
            { add s, s, b               ; xm.vstr s                             }
            { add s, s, b               ; xm.vstd t3                            }
            { add t3, s, b              ; xm.bt j, dit_ifft_inner_loop          }

        { sub k, k, _32                 ; xm.bt k, dit_ifft_outer_loop          }

    { srli a, a, 1                      ; xm.shli b, b, 1                       }
    { srli n, n, 1                      ; xm.bt n, dit_ifft_round_loop          }
    
dit_ifft_done:

//update the hr
{ li s, 31                          ; xm.vgetc t3                           }
xm.zexti t3, 5                            
sub s, s, t3
xm.lddsp  hr_p,s2,0
sw s,0( hr_p)

//update the exponent
{ nop                               ; lw t3, (STACK_EXP)*4            (sp) }
{ nop                               ; lw s,0                         ( t3) }
{ add s, s, exp_modifier            ; nop                                  }
{ nop                               ; sw s,0                         ( t3) }

//restore the regs
xm.lddsp  s4,s3,8
xm.lddsp  s6,s5,16
xm.lddsp  s8,s7,24
xm.retsp (NSTACKWORDS)*4   

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




#endif //defined(__VX4B__)
