// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


#include "../asm_helper.h"

.text
.align 4;
.issue_mode dual

/*  
q8_24 q24_logistic_fast(
    const q8_24 x);
*/
#define FUNCTION_NAME   q24_logistic_fast
#define NSTACKWORDS     (6+8)

#define STACK_VEC   (NSTACKWORDS-8)

.L_log_slope: 
  .word  1015490930, 640498971, 297985800, 120120271, 46079377, 17219453, 6371555, 3717288
.L_log_offset: 
  .word 8388608, 9853420, 12529304, 14613666, 15770555, 16334225, 16588473, 16661050

.cc_top FUNCTION_NAME.function,FUNCTION_NAME
FUNCTION_NAME:
  dualentsp NSTACKWORDS
  std r4, r5, sp[0]
  std r6, r7, sp[1]

{ clz r11, r0                 ; not r1, r0                  }
{ shr r2, r1, 24              ; ldc r3, 0                   }
{ mkmsk r4, 24                ; bf r11, .L_neg_input        }
.L_pos_input:
  { shr r1, r0, 24              ;                             }
  { shr r11, r1, 3              ;                             }
  { ldap r11, .L_log_slope      ; bf r11, .L_not_big          }
  .L_big:
    { mov r0, r4                  ; bu .L_finish                }
  .L_not_big:
  { mov r2, r0                  ; ldw r3, r11[r1]             }
  { ldap r11, .L_log_offset     ;                             }
  { ldc r1, 0                   ; ldw r0, r11[r1]             }
  maccu r0, r1, r3, r2
  bu .L_finish
.L_neg_input:
  { shr r11, r2, 3              ;                             }
  { ldap r11, .L_log_slope      ; bf r11, .L_not_small        }
  .L_small:
    { ldc r0, 0                   ; bu .L_finish                }
  .L_not_small:
  { ldap r11, .L_log_offset     ; ldw r3, r11[r2]             }
  { ldc r0, 0                   ; ldw r2, r11[r2]             }
    maccu r2, r0, r3, r1
  { sub r0, r4, r2              ;                             }

.L_finish:
  ldd r4, r5, sp[0]
  ldd r6, r7, sp[1]
  retsp NSTACKWORDS

.L_func_end_unpack:
.cc_bottom FUNCTION_NAME.function

.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;     .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;                  .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;                 .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;               .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME,.L_func_end_unpack - FUNCTION_NAME

#undef NSTACKWORDS
#undef FUNCTION_NAME



#endif //defined(__XS3A__)



