// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


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
#define NSTACKWORDS     (14)

#define VEC_R           (NSTACKWORDS - 8)

#define a         r0
#define r         r1
#define out_mul   r2
#define vec_r     r3
#define tmpA      r4
#define tmpB      r5
#define _30       r6

.text
.issue_mode dual
.align 16

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
  
.cc_top FUNCTION_NAME.function,FUNCTION_NAME
FUNCTION_NAME:
  dualentsp NSTACKWORDS
  std r4, r5, sp[1]
  std r6, r7, sp[2]
{ ldc r11, 0                  ; ldaw vec_r, sp[VEC_R]       }
{ lss out_mul, a, r11         ; vsetc r11                   } // Result gets multiplied by -1 if
{ ldc _30, 30                 ; bf out_mul, .L_hgfd         } // the input was negative
{ neg a, a                    ;                             }
.L_hgfd:

  lmul tmpA, tmpB, a, a, r11, r11
  lextract r, tmpA, tmpB, _30, 32  // extract theta^2
  lmul tmpA, tmpB, a, r, r11, r11 // theta * theta^2
  lextract tmpA, tmpA, tmpB, _30, 32 
  std tmpA, a, vec_r[0]   // theta, theta^3

#undef a  // no longer needed
#define tmpC     r0

  lmul tmpA, tmpB, tmpA, r, r11, r11
  lextract tmpB, tmpA, tmpB, _30, 32 // theta^5
  lmul tmpA, tmpC, tmpB, r, r11, r11
  lextract tmpA, tmpA, tmpC, _30, 32 // theta^7
  std tmpA, tmpB, vec_r[1] // theta^5, theta^7
 
  lmul tmpA, tmpB, tmpA, r, r11, r11
  lextract tmpB, tmpA, tmpB, _30, 32 // theta^9
  // stw tmpB, vec_r[4] // if we only wanted 5 terms
  lmul tmpA, tmpC, tmpB, r, r11, r11
  lextract tmpA, tmpA, tmpC, _30, 32 // theta^11
  std tmpA, tmpB, vec_r[2] // theta^9, theta^11

  lmul tmpA, tmpB, tmpA, r, r11, r11
  lextract tmpB, tmpA, tmpB, _30, 32 // theta^13
  lmul tmpA, tmpC, tmpB, r, r11, r11
  lextract tmpA, tmpA, tmpC, _30, 32 // theta^15
  std tmpA, tmpB, vec_r[3] // theta^13, theta^15

// Now that we've filled in vec_R[], we just need to do the VPU stuff.
// Note: All coefficients are positive and so are all elements or vec_r[],
//       and we know they can't add to more than 1.0

{ ldap r11, .L_vec_b          ; vclrdr                      }
{ ldap r11, .L_vec_s_hat      ; vldc r11[0]                 } // vC[] <-- P.S. coefficients
{                             ; vlmacc vec_r[0]             } // inner product with power vect
{ mov r11, vec_r              ; vlsat r11[0]                } // ensure they're all in the same q-format
{                             ; vstr r11[0]                 } 
{ ldap r11, .L_weights        ; vclrdr                      }
{                             ; vldc r11[0]                 } 
{ mov r11, vec_r              ; vlmaccr vec_r[0]            } // add them together
{                             ; vstr r11[0]                 }
{                             ; ldw r0, vec_r[0]            }
  ldd r4, r5, sp[1]
  ldd r6, r7, sp[2]
{                             ; bt out_mul, .L_gpgp         }
{                             ; retsp NSTACKWORDS           }
.L_gpgp:
{ neg r0, r0                  ; retsp NSTACKWORDS           }

.L_func_end:
.cc_bottom FUNCTION_NAME.function

.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME,.L_func_end - FUNCTION_NAME













#endif //defined(__XS3A__)

