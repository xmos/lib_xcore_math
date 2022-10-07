// Copyright 2021-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


/*  
void vect_s16_extract_low_byte(
    int8_t a[],
    const int16_t b[],
    const unsigned len);
*/


#include "../asm_helper.h"

#define NSTACKWORDS     (8+8)

#define FUNCTION_NAME   vect_s16_extract_low_byte

#define STACK_VEC_TMP   (NSTACKWORDS-8)

#define STACK_LEN       (6)
#define STACK_TMP       (0)

#define a           r0 
#define b           r1 
#define len         r2
#define tmpA        r3
#define tmpB        r4
#define _16         r5
#define tail        r6
#define vec_0x7FFF  r7

.text
.issue_mode dual
.align 4

.cc_top FUNCTION_NAME.function,FUNCTION_NAME

FUNCTION_NAME:
      dualentsp NSTACKWORDS
      ldc r11, 0x100
      std r4, r5, sp[1]
      std r6, r7, sp[2]
    { mov r11, len                              ; vsetc r11                                 }
    { zext r11, 4                               ; shr len, len, EPV_LOG2_S16                }
    { ldaw tmpA, sp[STACK_TMP]                  ; mkmsk tail, r11                           }
    {                                           ; stw len, sp[STACK_LEN]                    }

// First thing, write 0x80 to all outputs.
      ldaw r11, cp[vpu_vec_0x80]
    { ldc _16, 16                               ; mov tmpB, a                               }
    {                                           ; vldr r11[0]                               }
    { mkmsk r11, 16                             ; bf len, .L_set_0x80_loop_end              }

    .L_set_0x80_loop_top:
      { sub len, len, 1                           ;                                           }
        vstrpv tmpB[0], r11
      { add tmpB, tmpB, _16                       ; bt len, .L_set_0x80_loop_top              }
    .L_set_0x80_loop_end:
    {                                           ; bf tail, .L_0x80_no_tail                  }
      vstrpv tmpB[0], tail
    .L_0x80_no_tail:

// Now that that's done, actually compute outputs, only overwriting those that shouldn't be 0x80
//  (this is to avoid symmetric saturation)

      ldaw r11, cp[vpu_vec_0x0100]
    {                                           ; vldc r11[0]                               }
      ldaw r11, cp[vpu_vec_0x7FFF]
    { mov vec_0x7FFF, r11                       ; ldw len, sp[STACK_LEN]                    }
    { ldaw r11, sp[STACK_VEC_TMP]               ; bf len, .L_loop_bot                       }
    {                                           ; bu .L_loop_top                            }

.align 16
.L_loop_top:
        { sub len, len, 1                           ; vclrdr                                    }
        { add b, b, _16                             ; vlmacc b[0]                               }
        { add b, b, _16                             ; vstr r11[0]                               }
        { mkmsk tmpB, 4                             ; vladd vec_0x7FFF[0]                       }
        {                                           ; vdepth1                                   }
          vstrpv tmpA[0], tmpB
        { mkmsk tmpB, 16                            ; ldw r5, tmpA[0]                           }
        { andnot tmpB, r5                           ; vldr r11[0]                               }
        { ldc _16, 16                               ; vdepth8                                   }
          vstrpv a[0], tmpB
        { add a, a, _16                             ; bt len, .L_loop_top                       }

.L_loop_bot:
    {                                           ; bf tail, .L_finish                      }
    {                                           ; vclrdr                                  }
    {                                           ; vlmacc b[0]                             }
    {                                           ; vstr r11[0]                             }
    {                                           ; vladd vec_0x7FFF[0]                     }
    { mkmsk tmpB, 4                             ; vdepth1                                 }
      vstrpv tmpA[0], tmpB                       
    {                                           ; ldw r5, tmpA[0]                         }
    { andnot tail, r5                           ; vldr r11[0]                             }
    {                                           ; vdepth8                                 }
      vstrpv a[0], tail               
    
    
.L_finish:
        ldd r4, r5, sp[1]
        ldd r6, r7, sp[2]
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



