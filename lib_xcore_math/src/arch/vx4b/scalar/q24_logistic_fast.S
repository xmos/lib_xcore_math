// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"

.text
.align 4; /* Translation error on this line: unexpected token at position 8. */ 

/*  
q8_24 q24_logistic_fast(
    const q8_24 x);
*/
#define FUNCTION_NAME   q24_logistic_fast
#define NSTACKWORDS     (8+8)

#define STACK_VEC   (NSTACKWORDS-8)

.L_log_slope: 
  .word  1015490930, 640498971, 297985800, 120120271, 46079377, 17219453, 6371555, 3717288
.L_log_offset: 
  .word 8388608, 9853420, 12529304, 14613666, 15770555, 16334225, 16588473, 16661050

FUNCTION_NAME:
  xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
  xm.stdsp  s3,s2,0
  xm.stdsp  s5,s4,8

{ xm.clz t3, a0                 ; xm.not a1, a0                  }
{ srli a2, a1, 24              ; li a3, 0                   }
{ xm.mkmski s2, 24                ; xm.brff t3, .L_neg_input        }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
.L_pos_input:
  { srli a1, a0, 24              ; nop                             }
  { srli t3, a1, 3              ; nop                             }
  { xm.ldap t3, .L_log_slope      ; xm.brff t3, .L_not_big          }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
  .L_big:
    { mv a0, s2                  ; xm.bu .L_finish                }
  .L_not_big:
  { mv a2, a0                  ; xm.ldw a3, a1             (t3)}
  { xm.ldap t3, .L_log_offset     ; nop                             }
  { li a1, 0                   ; xm.ldw a0, a1             (t3)}
  xm.maccu a0, a1, a3, a2
  tail .L_finish
.L_neg_input:
  { srli t3, a2, 3              ; nop                             }
  { xm.ldap t3, .L_log_slope      ; xm.brff t3, .L_not_small        }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
  .L_small:
    { li a0, 0                   ; xm.bu .L_finish                }
  .L_not_small:
  { xm.ldap t3, .L_log_offset     ; xm.ldw a3, a2             (t3)}
  { li a0, 0                   ; xm.ldw a2, a2             (t3)}
    xm.maccu a2, a0, a3, a1
  { sub a0, s2, a2              ; nop                             }

.L_finish:
  xm.lddsp  s3,s2,0
  xm.lddsp  s5,s4,8
  xm.retsp (NSTACKWORDS)*4/* Multiple XAT warnings: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in retsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate", 'RETSP operand may need scaling' */

.L_func_end_unpack:

.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords /* Translation error on this line: unexpected token at position 42. */ 
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends /* Translation error on this line: unexpected token at position 32. */ 
.size FUNCTION_NAME,.L_func_end_unpack - FUNCTION_NAME

#undef NSTACKWORDS
#undef FUNCTION_NAME



#endif //defined(__VX4B__)



