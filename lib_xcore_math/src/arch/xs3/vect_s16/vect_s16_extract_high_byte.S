// Copyright 2021-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


/*  
void vect_s16_extract_high_byte(
    int8_t a[],
    const int16_t b[],
    const unsigned len);
*/


#include "../asm_helper.h"

#define NSTACKWORDS     (10+8)

#define FUNCTION_NAME   vect_s16_extract_high_byte

#define STACK_VEC_TMP   (NSTACKWORDS-8)

#define STACK_LEN       (8)
#define STACK_TMP       (0)

#define a           r0 
#define b           r1 
#define len         r2
#define eight       r3
#define vec_tmp     r4
#define tmp         r5
#define _16         r6
#define tail        r7
#define vec_0x007F  r8

.text
.issue_mode dual
.align 4

.cc_top FUNCTION_NAME.function,FUNCTION_NAME

FUNCTION_NAME:
      dualentsp NSTACKWORDS
      ldc r11, 0x100
      std r4, r5, sp[1]
      std r6, r7, sp[2]
      std r8, r9, sp[3]
    { mkmsk r10, 4                              ; stw r10, sp[1]                            }
    { mov r11, len                              ; vsetc r11                                 }
    { zext r11, 4                               ; shr len, len, EPV_LOG2_S16                }
    { ldaw vec_tmp, sp[STACK_VEC_TMP]           ; mkmsk tail, r11                           }
    { ldaw tmp, sp[STACK_TMP]                   ; stw len, sp[STACK_LEN]                    }

// First thing, write 0x80 to all outputs.
      ldaw r11, cp[vpu_vec_0x80]
    { ldc _16, 16                               ; mov r8, a                                 }
    { ldc eight, 8                              ; vldr r11[0]                               }
    { mkmsk r11, 16                             ; bf len, .L_set_0x80_loop_end              }

    .L_set_0x80_loop_top:
      { sub len, len, 1                         ;                                           }
        vstrpv r8[0], r11
      { add r8, r8, _16                         ; bt len, .L_set_0x80_loop_top              }
    .L_set_0x80_loop_end:
    {                                         ; bf tail, .L_0x80_no_tail                    }
      vstrpv r8[0], tail
    .L_0x80_no_tail:

// Now that that's done, actually compute outputs, only overwriting those that shouldn't be 0x80
//  (this is to avoid symmetric saturation)

      ldaw r11, cp[vpu_vec_0x007F]
    {                                           ; ldw len, sp[STACK_LEN]                    }
    { mov vec_0x007F, r11                       ; bf len, .L_loop_bot                       }
    {                                           ; bu .L_loop_top                            }

.align 16
.L_loop_top:
          vlashr b[0], eight
        { neg eight, eight                          ; vstr vec_tmp[0]                           }
        { add b, b, _16                             ; vladd vec_0x007F[0]                       }
        { mkmsk r11, 16                             ; vdepth1                                   }
          vstrpv tmp[0], r10
        { sub len, len, 1                           ; ldw r9, tmp[0]                            }
        { andnot r11, r9                            ; add b, b, _16                             }
          vlashr vec_tmp[0], eight
        { neg eight, eight                          ; vdepth8                                   }
          vstrpv a[0], r11
        { add a, a, _16                             ; bt len, .L_loop_top                       }

.L_loop_bot:
    {                                           ; bf tail, .L_finish                      }
      vlashr b[0], eight
    { neg eight, eight                          ; vstr vec_tmp[0]                         }
    {                                           ; vladd vec_0x007F[0]                     }
    {                                           ; vdepth1                                 }
      vstrpv tmp[0], r10                       
    {                                           ; ldw r9, tmp[0]                          }
    { andnot tail, r9                           ;                                         }
      vlashr vec_tmp[0], eight                    
    {                                           ; vdepth8                                 }
      vstrpv a[0], tail               
    
    
.L_finish:
        ldd r4, r5, sp[1]
        ldd r6, r7, sp[2]
        ldd r8, r9, sp[3]
    {                                           ; ldw r10, sp[1]                        }
    {                                           ; retsp NSTACKWORDS                     } 

.L_func_end:
.cc_bottom FUNCTION_NAME.function


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS; .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;              .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;             .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;           .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME



#endif //defined(__XS3A__)



