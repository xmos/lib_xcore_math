// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

#define NSTACKWORDS     (8+8+4)

#define len         x13

.text
.p2align 4


/*  
headroom_t vect_s32_min_elementwise(
    int32_t a[],
    const int32_t b[],
    const int32_t c[],
    const unsigned len,
    const right_shift_t b_shr,
    const right_shift_t c_shr);
*/
#define FUNC_NAME   vect_s32_min_elementwise
FUNC_NAME:
  { li t3, 0                              ; xm.entsp (NSTACKWORDS)*4                   }
  { slli t3, len, SIZEOF_LOG2_S32           ; xm.vsetc t3}/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in  shli t3, len, SIZEOF_LOG2_S32           \nMessage: The shift amount is not 32" */
  { xm.zexti t3, 5                             ; srli len, len, EPV_LOG2_S32              }/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in  shri len, len, EPV_LOG2_S32              \nMessage: The shift amount is not 32" */
    call vect_sXX_min_elementwise
  { li t3, 31                             ; nop                                         }
  { sub a0, t3, a0                         ; xm.retsp (NSTACKWORDS)*4                       }
.L_end_s32:

.global FUNC_NAME
.type FUNC_NAME,@function
.set FUNC_NAME.nstackwords,NSTACKWORDS;  .global FUNC_NAME.nstackwords /* Translation error on this line: unexpected token at position 38. */ 
.set FUNC_NAME.maxcores,1;               .global FUNC_NAME.maxcores /* Translation error on this line: unexpected token at position 25. */ 
.set FUNC_NAME.maxtimers,0;              .global FUNC_NAME.maxtimers /* Translation error on this line: unexpected token at position 26. */ 
.set FUNC_NAME.maxchanends,0;            .global FUNC_NAME.maxchanends /* Translation error on this line: unexpected token at position 28. */ 
.size FUNC_NAME, .L_end_s32 - FUNC_NAME
#undef FUNC_NAME
  


/*  
headroom_t vect_s16_min_elementwise(
    int16_t a[],
    const int16_t b[],
    const int16_t c[],
    const unsigned len,
    const right_shift_t b_shr,
    const right_shift_t c_shr);
*/
#define FUNC_NAME   vect_s16_min_elementwise
FUNC_NAME:
    xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
    li t3, 0x0100
  { slli t3, len, SIZEOF_LOG2_S16           ; xm.vsetc t3}/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in  shli t3, len, SIZEOF_LOG2_S16           \nMessage: The shift amount is not 32" */
  { xm.zexti t3, 5                             ; srli len, len, EPV_LOG2_S16              }/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in  shri len, len, EPV_LOG2_S16              \nMessage: The shift amount is not 32" */
    call vect_sXX_min_elementwise
  { li t3, 15                             ; nop                                         }
  { sub a0, t3, a0                         ; xm.retsp (NSTACKWORDS)*4                       }
.L_end_s16:

.global FUNC_NAME
.type FUNC_NAME,@function
.set FUNC_NAME.nstackwords,NSTACKWORDS;  .global FUNC_NAME.nstackwords /* Translation error on this line: unexpected token at position 38. */ 
.set FUNC_NAME.maxcores,1;               .global FUNC_NAME.maxcores /* Translation error on this line: unexpected token at position 25. */ 
.set FUNC_NAME.maxtimers,0;              .global FUNC_NAME.maxtimers /* Translation error on this line: unexpected token at position 26. */ 
.set FUNC_NAME.maxchanends,0;            .global FUNC_NAME.maxchanends /* Translation error on this line: unexpected token at position 28. */ 
.size FUNC_NAME, .L_end_s16 - FUNC_NAME
#undef FUNC_NAME
  


/*  
headroom_t vect_s8_min_elementwise(
    int8_t a[],
    const int8_t b[],
    const int8_t c[],
    const unsigned len,
    const right_shift_t b_shr,
    const right_shift_t c_shr);
*/
#define FUNC_NAME   vect_s8_min_elementwise
FUNC_NAME:
    xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
    li t3, 0x0200
  { mv t3, len                            ; xm.vsetc t3}
  { xm.zexti t3, 5                             ; srli len, len, EPV_LOG2_S8               }/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in  shri len, len, EPV_LOG2_S8               \nMessage: The shift amount is not 32" */
    call vect_sXX_min_elementwise
  { li t3, 7                              ; nop                                         }
  { sub a0, t3, a0                         ; xm.retsp (NSTACKWORDS)*4                       }
.L_end_s8:

.global FUNC_NAME
.type FUNC_NAME,@function
.set FUNC_NAME.nstackwords,NSTACKWORDS;  .global FUNC_NAME.nstackwords /* Translation error on this line: unexpected token at position 38. */ 
.set FUNC_NAME.maxcores,1;               .global FUNC_NAME.maxcores /* Translation error on this line: unexpected token at position 25. */ 
.set FUNC_NAME.maxtimers,0;              .global FUNC_NAME.maxtimers /* Translation error on this line: unexpected token at position 26. */ 
.set FUNC_NAME.maxchanends,0;            .global FUNC_NAME.maxchanends /* Translation error on this line: unexpected token at position 28. */ 
.size FUNC_NAME, .L_end_s8 - FUNC_NAME
#undef FUNC_NAME



#undef len

#define STACK_VEC_TMP   (NSTACKWORDS-8-4)
#define STACK_BYTEMASK  6

#define a           x10 
#define b           x11 
#define c           x12
#define len         x13
#define shr_b       x18
#define shr_c       x19
#define _32         x20
#define tmp_vec     x21
#define bytemask    len



/**
 * WARNING: This does _NOT_ use the standard ABI. It assumes x28 will contain
 *          the length of the tail in bytes.
 */

vect_sXX_min_elementwise:
    xm.stdsp  s3,s2,8
    xm.stdsp  s5,s4,16

  { li _32, 32                             ; xm.vclrdr                                  }
  mv shr_c, a5
  { xm.mkmsk t3, t3                          ; nop}
  mv shr_b, a4
  { addi tmp_vec,sp, (STACK_VEC_TMP)*4         ; nop}/* Multiple XAT warnings: 'LDAWSP outside of known frame - offset may need correction', 'LDWSP outside of known frame - offset may need correction' */
  { xm.mkmski t3, 32                           ; sw t3, (STACK_BYTEMASK)*4             (sp)}
  { nop                                         ; xm.brff len, .L_loop_bot                     }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
  { nop                                         ; xm.bu .L_loop_top                          }

  // Deal with main vector body
.p2align 4
.L_loop_top:
    // Here we need to assume shr_b and shr_c have been chosen to guarantee 1 
    // bit of headroom in each so that c[k] - b[k] can't saturate. That means 
    // this should be perfectly accurate if there's already at least 1 bit of 
    // headroom in each input.
      xm.vlashr c, shr_c
      xm.vstrpv tmp_vec, t3
      xm.vlashr b, shr_b
      xm.vstrpv a, t3 
    { addi len, len, -1                         ; xm.vlsub tmp_vec}
    { add c, c, _32                           ; xm.vpos                                    }
    { add b, b, _32                           ; xm.vlsub tmp_vec}
    { add a, a, _32                           ; xm.vstr a}
    { nop                                         ; xm.bt len, .L_loop_top                     }
.L_loop_bot:

    lw bytemask, (STACK_BYTEMASK)*4(sp)/* Multiple XAT warnings: 'LDWSP has unknown offset - this may need correction', "Falling back on assumption: the int < 64 for the integer value of the item at position 1 in the instruction's operands in ldwsp bytemask, STACK_BYTEMASK\nMessage: The offset can be encoded in sru6 immediate" */
    beqz bytemask, .L_finish
    xm.vlashr c, shr_c
    xm.vstrpv tmp_vec, bytemask
    xm.vlashr b, shr_b
    xm.vstrpv a, bytemask
    xm.vlsub tmp_vec
    xm.vpos
    mv t3, tmp_vec
    xm.vlsub tmp_vec
    xm.vstd tmp_vec
    xm.vstrpv tmp_vec, bytemask
    xm.vldr t3
    xm.vstr tmp_vec
    xm.vstrpv a, bytemask

.L_finish:
    xm.lddsp  s3,s2,8/* XAT Warning: "Not correcting LDDSP offset because it's not in the local frame range" */
    xm.lddsp  s5,s4,16/* XAT Warning: "Not correcting LDDSP offset because it's not in the local frame range" */
    xm.vgetc t3
    xm.zexti t3, 5
    mv a0, t3
    ret 


.L_end_sXX:


#endif //defined(__VX4B__)



