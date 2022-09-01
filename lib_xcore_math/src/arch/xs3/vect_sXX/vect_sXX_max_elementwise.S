// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


#include "../asm_helper.h"

#define NSTACKWORDS     (8+8)

#define len         r3

.text
.issue_mode dual
.align 16


/*  
headroom_t vect_s32_max_elementwise(
    int32_t a[],
    const int32_t b[],
    const int32_t c[],
    const unsigned len,
    const right_shift_t b_shr,
    const right_shift_t c_shr);
*/
#define FUNC_NAME   vect_s32_max_elementwise
.cc_top FUNC_NAME.function,FUNC_NAME
FUNC_NAME:
  { ldc r11, 0                              ; dualentsp NSTACKWORDS                   }
  { shl r11, len, SIZEOF_LOG2_S32           ; vsetc r11                               }
  { zext r11, 5                             ; shr len, len, EPV_LOG2_S32              }
    bl vect_sXX_max_elementwise
  { ldc r11, 31                             ;                                         }
  { sub r0, r11, r0                         ; retsp NSTACKWORDS                       }
.L_end_s32:
.cc_bottom FUNC_NAME.function

.global FUNC_NAME
.type FUNC_NAME,@function
.set FUNC_NAME.nstackwords,NSTACKWORDS;  .global FUNC_NAME.nstackwords
.set FUNC_NAME.maxcores,1;               .global FUNC_NAME.maxcores
.set FUNC_NAME.maxtimers,0;              .global FUNC_NAME.maxtimers
.set FUNC_NAME.maxchanends,0;            .global FUNC_NAME.maxchanends
.size FUNC_NAME, .L_end_s32 - FUNC_NAME
#undef FUNC_NAME
  


/*  
headroom_t vect_s16_max_elementwise(
    int16_t a[],
    const int16_t b[],
    const int16_t c[],
    const unsigned len,
    const right_shift_t b_shr,
    const right_shift_t c_shr);
*/
#define FUNC_NAME   vect_s16_max_elementwise
.cc_top FUNC_NAME.function,FUNC_NAME
FUNC_NAME:
    dualentsp NSTACKWORDS
    ldc r11, 0x0100
  { shl r11, len, SIZEOF_LOG2_S16           ; vsetc r11                               }
  { zext r11, 5                             ; shr len, len, EPV_LOG2_S16              }
    bl vect_sXX_max_elementwise
  { ldc r11, 15                             ;                                         }
  { sub r0, r11, r0                         ; retsp NSTACKWORDS                       }
.L_end_s16:
.cc_bottom FUNC_NAME.function

.global FUNC_NAME
.type FUNC_NAME,@function
.set FUNC_NAME.nstackwords,NSTACKWORDS;  .global FUNC_NAME.nstackwords
.set FUNC_NAME.maxcores,1;               .global FUNC_NAME.maxcores
.set FUNC_NAME.maxtimers,0;              .global FUNC_NAME.maxtimers
.set FUNC_NAME.maxchanends,0;            .global FUNC_NAME.maxchanends
.size FUNC_NAME, .L_end_s16 - FUNC_NAME
#undef FUNC_NAME
  


/*  
headroom_t vect_s8_max_elementwise(
    int8_t a[],
    const int8_t b[],
    const int8_t c[],
    const unsigned len,
    const right_shift_t b_shr,
    const right_shift_t c_shr);
*/
#define FUNC_NAME   vect_s8_max_elementwise
.cc_top FUNC_NAME.function,FUNC_NAME
FUNC_NAME:
    dualentsp NSTACKWORDS
    ldc r11, 0x0200
  { mov r11, len                            ; vsetc r11                               }
  { zext r11, 5                             ; shr len, len, EPV_LOG2_S8               }
    bl vect_sXX_max_elementwise
  { ldc r11, 7                              ;                                         }
  { sub r0, r11, r0                         ; retsp NSTACKWORDS                       }
.L_end_s8:
.cc_bottom FUNC_NAME.function

.global FUNC_NAME
.type FUNC_NAME,@function
.set FUNC_NAME.nstackwords,NSTACKWORDS;  .global FUNC_NAME.nstackwords
.set FUNC_NAME.maxcores,1;               .global FUNC_NAME.maxcores
.set FUNC_NAME.maxtimers,0;              .global FUNC_NAME.maxtimers
.set FUNC_NAME.maxchanends,0;            .global FUNC_NAME.maxchanends
.size FUNC_NAME, .L_end_s8 - FUNC_NAME
#undef FUNC_NAME



#undef len

#define STACK_B_SHR     (NSTACKWORDS+1)
#define STACK_C_SHR     (NSTACKWORDS+2)
#define STACK_VEC_TMP   (NSTACKWORDS-8)
#define STACK_BYTEMASK  6

#define a           r0 
#define b           r1 
#define c           r2
#define len         r3
#define shr_b       r4
#define shr_c       r5
#define _32         r6
#define tmp_vec     r7
#define bytemask    len



/**
 * WARNING: This does _NOT_ use the standard ABI. It assumes r11 will contain
 *          the length of the tail in bytes.
 */

.cc_top vect_sXX_max_elementwise.function, vect_sXX_max_elementwise
vect_sXX_max_elementwise:
    dualentsp 0
    std r4, r5, sp[1]
    std r6, r7, sp[2]

  { ldc _32, 32                             ; vclrdr                                  }
  { mkmsk r11, r11                          ; ldw shr_c, sp[STACK_C_SHR]              }
  { ldaw tmp_vec, sp[STACK_VEC_TMP]         ; ldw shr_b, sp[STACK_B_SHR]              }
  { mkmsk r11, 32                           ; stw r11, sp[STACK_BYTEMASK]             }
  {                                         ; bf len, .L_loop_bot                     }
  {                                         ; bu .L_loop_top                          }

  // Deal with main vector body
.align 16
.L_loop_top:
    // Here we need to assume shr_b and shr_c have been chosen to guarantee 1 
    // bit of headroom in each so that c[k] - b[k] can't saturate. That means 
    // this should be perfectly accurate if there's already at least 1 bit of 
    // headroom in each input.
      vlashr c[0], shr_c
      vstrpv tmp_vec[0], r11
      vlashr b[0], shr_b
      vstrpv a[0], r11 
    { sub len, len, 1                         ; vlsub tmp_vec[0]                        }
    { add c, c, _32                           ; vpos                                    }
    { add b, b, _32                           ; vladd a[0]                              }
    { add a, a, _32                           ; vstr a[0]                               }
    {                                         ; bt len, .L_loop_top                     }
.L_loop_bot:

    entsp 0
.issue_mode single
    ldw bytemask, sp[STACK_BYTEMASK]
    bf bytemask, .L_finish
    vlashr c[0], shr_c
    vstrpv tmp_vec[0], bytemask
    vlashr b[0], shr_b
    vstrpv a[0], bytemask
    vlsub tmp_vec[0]
    vpos
    mov r11, tmp_vec
    vladd a[0]
    vstd tmp_vec[0]
    vstrpv tmp_vec[0], bytemask
    vldr r11[0]
    vstr tmp_vec[0]
    vstrpv a[0], bytemask

.L_finish:
    ldd r4, r5, sp[1]
    ldd r6, r7, sp[2]
    vgetc r11
    zext r11, 5
    mov r0, r11
    retsp 0

.L_end_sXX:
.cc_bottom vect_sXX_max_elementwise.function


#endif //defined(__XS3A__)



