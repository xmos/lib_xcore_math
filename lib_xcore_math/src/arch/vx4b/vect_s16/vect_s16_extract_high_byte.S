// Copyright 2021-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


/*  
void vect_s16_extract_high_byte(
    int8_t a[],
    const int16_t b[],
    const unsigned len);
*/


#include "../asm_helper.h"

#define NSTACKWORDS     (12+8+4)

#define FUNCTION_NAME   vect_s16_extract_high_byte

#define STACK_VEC_TMP   (NSTACKWORDS-8-4)

#define STACK_LEN       (8)
#define STACK_TMP       (0)

#define a           a0 
#define b           a1 
#define len         a2
#define eight       a3
#define vec_tmp     x18
#define tmp         x19
#define _16         s4
#define tail        s5
#define vec_0x007F  x22

.text
.p2align 2


FUNCTION_NAME:
      xm.entsp (NSTACKWORDS)*4
      li t3, 0x100
      xm.stdsp  s3,s2,8
      xm.stdsp  s5,s4,16
      xm.stdsp  s7,s6,24
    { xm.mkmski s8, 4                   ; sw s8, 4                            (sp) }
    { mv t3, len                        ; xm.vsetc t3                           }
    { xm.zexti t3, 4                    ; srli len, len, EPV_LOG2_S16           }
    { addi vec_tmp,sp, (STACK_VEC_TMP)*4 ; xm.mkmsk tail, t3                     }
    { addi tmp,sp, (STACK_TMP)*4        ; sw len, (STACK_LEN)*4                    (sp) }

// First thing, write 0x80 to all outputs.
la t3, vpu_vec_0x80
    { li _16, 16                        ; mv s6, a                              }
    { li eight, 8                       ; xm.vldr t3                            }
    { xm.mkmski t3, 16                  ; xm.brff len, .L_set_0x80_loop_end     }

    .L_set_0x80_loop_top:
      { addi len, len, -1               ; nop                                   }
        xm.vstrpv s6, t3
      { add s6, s6, _16                 ; xm.bt len, .L_set_0x80_loop_top       }
    .L_set_0x80_loop_end:
    { nop                               ; xm.brff tail, .L_0x80_no_tail         }
      xm.vstrpv s6, tail
    .L_0x80_no_tail:

// Now that that's done, actually compute outputs, only overwriting those that shouldn't be 0x80
//  (this is to avoid symmetric saturation)

la t3, vpu_vec_0x007F
    { nop                               ; lw len, (STACK_LEN)*4                    (sp) }
    { mv vec_0x007F, t3                 ; xm.brff len, .L_loop_bot              }
    { nop                               ; xm.bu .L_loop_top                     }

.p2align 4
.L_loop_top:
          xm.vlashr b, eight
{ xm.neg eight, eight                   ; nop                                   }
{ nop                                   ; xm.vstr vec_tmp                       }
        { add b, b, _16                 ; xm.vladd vec_0x007F                   }
        { xm.mkmski t3, 16              ; xm.vdepth1                            }
          xm.vstrpv tmp, s8
        { addi len, len, -1             ; lw s7,0                            ( tmp) }
        { xm.andnot t3, s7              ; add b, b, _16                         }
          xm.vlashr vec_tmp, eight
{ xm.neg eight, eight                   ; nop                                   }
{ nop                                   ; xm.vdepth8                            }
          xm.vstrpv a, t3
        { add a, a, _16                 ; xm.bt len, .L_loop_top                }

.L_loop_bot:
    { nop                               ; xm.brff tail, .L_finish               }
      xm.vlashr b, eight
{ xm.neg eight, eight                   ; nop                                   }
{ nop                                   ; xm.vstr vec_tmp                       }
    { nop                               ; xm.vladd vec_0x007F                   }
    { nop                               ; xm.vdepth1                            }
      xm.vstrpv tmp, s8                       
    { nop                               ; lw s7,0                          ( tmp) }
    { xm.andnot tail, s7                ; nop                                   }
      xm.vlashr vec_tmp, eight                    
    { nop                               ; xm.vdepth8                            }
      xm.vstrpv a, tail               
    
    
.L_finish:
        xm.lddsp  s3,s2,8
        xm.lddsp  s5,s4,16
        xm.lddsp  s7,s6,24
    { nop                               ; lw s8, 4                        (sp)  }
    { nop                               ; xm.retsp (NSTACKWORDS)*4              } 

.L_func_end:


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS; .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;              .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;             .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;           .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME



#endif //defined(__VX4B__)



