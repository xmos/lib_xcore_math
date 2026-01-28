// Copyright 2021-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


/*  
void vect_s16_extract_low_byte(
    int8_t a[],
    const int16_t b[],
    const unsigned len);
*/


#include "../asm_helper.h"

#define NSTACKWORDS     (8+8+4)

#define FUNCTION_NAME   vect_s16_extract_low_byte

#define STACK_VEC_TMP   (NSTACKWORDS-8-4)

#define STACK_LEN       (6)
#define STACK_TMP       (7)

#define a           x10 
#define b           x11 
#define len         x12
#define tmpA        x13
#define tmpB        x18
#define _16         x19
#define tail        x20
#define vec_0x7FFF  x21

.text
.p2align 2


FUNCTION_NAME:
      xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
      li t3, 0x100
      xm.stdsp  s3,s2,8
      xm.stdsp  s5,s4,16
    { mv t3, len                              ; xm.vsetc t3}
    { xm.zexti t3, 4                               ; srli len, len, EPV_LOG2_S16                }/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in  shri len, len, EPV_LOG2_S16                \nMessage: The shift amount is not 32" */
    { addi tmpA,sp, (STACK_TMP)*4                  ; xm.mkmsk tail, t3                           }
    { nop                                           ; sw len, (STACK_LEN)*4                    (sp)}

// First thing, write 0x80 to all outputs.
      la t3, vpu_vec_0x80
    { li _16, 16                               ; mv tmpB, a                               }
    { nop                                           ; xm.vldr t3}
    { xm.mkmski t3, 16                             ; xm.brff len, .L_set_0x80_loop_end              }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */

    .L_set_0x80_loop_top:
      { addi len, len, -1                           ; nop                                           }
        xm.vstrpv tmpB, t3
      { add tmpB, tmpB, _16                       ; xm.bt len, .L_set_0x80_loop_top              }
    .L_set_0x80_loop_end:
    { nop                                           ; xm.brff tail, .L_0x80_no_tail                  }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
      xm.vstrpv tmpB, tail
    .L_0x80_no_tail:

// Now that that's done, actually compute outputs, only overwriting those that shouldn't be 0x80
//  (this is to avoid symmetric saturation)

      la t3, vpu_vec_0x0100
    { nop                                           ; xm.vldc t3}
      la t3, vpu_vec_0x7FFF
    { mv vec_0x7FFF, t3                       ; lw len, (STACK_LEN)*4                    (sp)}
    { addi t3,sp, (STACK_VEC_TMP)*4               ; xm.brff len, .L_loop_bot                       }/* Multiple XAT warnings: 'LDAWSP outside of known frame - offset may need correction', 'Instruction xm.brff can only branch forwards; this branch may need revising' */
    { nop                                           ; xm.bu .L_loop_top                            }

.p2align 4
.L_loop_top:
        { addi len, len, -1                           ; xm.vclrdr                                    }
        {nop                            ; xm.vlmacc0 b}
        xm.vlmacc1 b
        { add b, b, _16                             ;nop}
        { add b, b, _16                             ; xm.vstr t3}
        { xm.mkmski tmpB, 4                             ; xm.vladd vec_0x7FFF}
        { nop                                           ; xm.vdepth1                                   }
          xm.vstrpv tmpA, tmpB
        { xm.mkmski tmpB, 16                            ; lw s3,0                           ( tmpA)}
{ xm.andnot tmpB, s3                           ; nop}
{nop;xm.vldr t3}
        { li _16, 16                               ; xm.vdepth8                                   }
          xm.vstrpv a, tmpB
        { add a, a, _16                             ; xm.bt len, .L_loop_top                       }

.L_loop_bot:
    { nop                                           ; xm.brff tail, .L_finish                      }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
    { nop                                           ; xm.vclrdr                                  }
    { nop                                           ; xm.vlmacc0 b}
    xm.vlmacc1 b
    { nop                                           ; xm.vstr t3}
    { nop                                           ; xm.vladd vec_0x7FFF}
    { xm.mkmski tmpB, 4                             ; xm.vdepth1                                 }
      xm.vstrpv tmpA, tmpB                       
    { nop                                           ; lw s3,0                         ( tmpA)}
{ xm.andnot tail, s3                           ; nop}
{nop;xm.vldr t3}
    { nop                                           ; xm.vdepth8                                 }
      xm.vstrpv a, tail               
    
    
.L_finish:
        xm.lddsp  s3,s2,8
        xm.lddsp  s5,s4,16
    { nop                                           ; xm.retsp (NSTACKWORDS)*4                     } 

.L_func_end:


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS; .global FUNCTION_NAME.nstackwords /* Translation error on this line: unexpected token at position 42. */ 
.set FUNCTION_NAME.maxcores,1;              .global FUNCTION_NAME.maxcores /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;             .global FUNCTION_NAME.maxtimers /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;           .global FUNCTION_NAME.maxchanends /* Translation error on this line: unexpected token at position 32. */ 
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME



#endif //defined(__VX4B__)



