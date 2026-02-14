// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"


/*  
  Takes a normalized angle between -0.5 and 0.5 in Q31 format  (corresponds to -pi/4 to pi/4)
  and returns tan() of that angle in Q30.

  tangent of any angle outside [-pi/4, pi/4] can be derived from a value within that range based on 
  the identities of the tangent function.

  q2_30 sbrad_tan(
      const sbrad_t theta);
*/

#define FUNCTION_NAME   sbrad_tan
#define NSTACKWORDS     (16)

#define VEC_R           (NSTACKWORDS - 12)

#define a         a0
#define r         a1
#define out_mul   a2
#define vec_r     a3
#define tmpA      s2
#define tmpB      s3
#define _30       s4

.text
.p2align 4

.L_vec_b:
.word 0x6487ED51, 0x52AEF398, 0x519AF19D, 0x517FFE6D
.word 0x517D1CB8, 0x517CCBC9, 0x517CC2D5

// The final element of vec_b[] is the tail term that includes the convergent sum
// of the geometric series [1, alpha^2, alpha^4, alpha^6, ...]. In my notes I designated
// this coefficient as  (4/3)*beta, where beta = 1.27323954.
// When you normalize angles by multiplying by 2/pi and look at the power series about 0 of
// the function tan(theta), the (constant) coefficients with each term converge asymptotically
// (as term index increases) towards the value beta.
// (4/3) is because R = alpha^2, where 0 <= alpha <= 0.5, so 0 <= R <= 0.25, and the
// convergent geometric series is in R --> [1, R, R^2, R^3, ...], which converges to
// (1/(1-R)). Given the bounds for R,    1 <= (1/(1-R)) <= (4/3).

// Specifically, the final term is  (1/(1-R))*beta*(alpha^15), but we don't want to do a division,
// so by just picking a value of R and always using that, we should significantly improve our
// absolute error (compared to not including the final convergent sum term at all). We should prefer
// larger values of R because the absolute error is greater there, but it looks like we get the
// best results when we haven't gone quite all the way to (4/3).

// I've experimentally found that the following seems to give the lowest absolute error in the test.
.word 0x6b6cb9bd // Q30( beta * (4/3)^( 0.9605835543766578 ) )
// .word 0x6ca65798 // Q30( beta * (4/3)^(1) )
.L_vec_s_hat:
  .word 1,3,5,7,9,11,13,15
.L_weights:
  .word 0x40000000, 0x40000000, 0x40000000, 0x40000000
  .word 0x40000000, 0x40000000, 0x40000000, 0x40000000
  
FUNCTION_NAME:
  xm.entsp (NSTACKWORDS)*4
  xm.stdsp  s3,s2,8
  xm.stdsp  s5,s4,0
{ li t3, 0                              ; addi vec_r,sp, (VEC_R)*4              }
{ xm.slt out_mul, a, t3                 ; xm.vsetc t3                           } // Result gets multiplied by -1 if
{ li _30, 30                            ; xm.brff out_mul, .L_hgfd              } 
{ xm.neg a, a                           ; nop                                   }
.L_hgfd:

  xm.lmul tmpA, tmpB, a, a, t3, t3
  xm.lextract r, tmpA, tmpB, _30, 32  // extract theta^2
  xm.lmul tmpA, tmpB, a, r, t3, t3 // theta * theta^2
  xm.lextract tmpA, tmpA, tmpB, _30, 32 
  xm.stdi  a,tmpA, 0   (vec_r)// theta, theta^3

#undef a  // no longer needed
#define tmpC     a0

  xm.lmul tmpA, tmpB, tmpA, r, t3, t3
  xm.lextract tmpB, tmpA, tmpB, _30, 32 // theta^5
  xm.lmul tmpA, tmpC, tmpB, r, t3, t3
  xm.lextract tmpA, tmpA, tmpC, _30, 32 // theta^7
  xm.stdi  tmpB,tmpA, 8 (vec_r)// theta^5, theta^7
 
  xm.lmul tmpA, tmpB, tmpA, r, t3, t3
  xm.lextract tmpB, tmpA, tmpB, _30, 32 // theta^9
  // stw tmpB, vec_r[4] // if we only wanted 5 terms
  xm.lmul tmpA, tmpC, tmpB, r, t3, t3
  xm.lextract tmpA, tmpA, tmpC, _30, 32 // theta^11
  xm.stdi  tmpB,tmpA, 16 (vec_r)// theta^9, theta^11

  xm.lmul tmpA, tmpB, tmpA, r, t3, t3
  xm.lextract tmpB, tmpA, tmpB, _30, 32 // theta^13
  xm.lmul tmpA, tmpC, tmpB, r, t3, t3
  xm.lextract tmpA, tmpA, tmpC, _30, 32 // theta^15
  xm.stdi  tmpB,tmpA, 24 (vec_r)// theta^13, theta^15

// Now that we've filled in vec_R[], we just need to do the VPU stuff.
// Note: All coefficients are positive and so are all elements or vec_r[],
//       and we know they can't add to more than 1.0

{ xm.ldap t3, .L_vec_b                  ; xm.vclrdr                             }
{ xm.ldap t3, .L_vec_s_hat              ; xm.vldc t3                            } // vC[] <-- P.S. coefficients
{ nop                                   ; xm.vlmacc0 vec_r                      } // inner product with power vect
xm.vlsat t3
{ mv t3, vec_r                          ; nop                                   } // ensure they're all in the same q-format
{ nop                                   ; xm.vstr t3                            } 
{ xm.ldap t3, .L_weights                ; xm.vclrdr                             }
{ nop                                   ; xm.vldc t3                            } 
{ mv t3, vec_r                          ; xm.vlmaccr0 vec_r                     } // add them together
{ nop                                   ; xm.vstr t3                            }
{ nop                                   ; lw a0,0            ( vec_r)           }
  xm.lddsp  s3,s2,8 
  xm.lddsp  s5,s4,0 
{ nop                                   ; xm.bt out_mul, .L_gpgp                }
{ nop                                   ; xm.retsp (NSTACKWORDS)*4              }
.L_gpgp:
{ xm.neg a0, a0                         ; nop                                   }
{ xm.retsp (NSTACKWORDS*4)              ; nop                                   }      

.L_func_end:

.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
.size FUNCTION_NAME,.L_func_end - FUNCTION_NAME













#endif //defined(__VX4B__)

