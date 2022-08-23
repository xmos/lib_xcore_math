// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


/*  
void xs3_memcpy(
    void* dst,
    const void* src,
    unsigned bytes);
*/

#define NSTACKWORDS     (0)
#define FUNCTION_NAME   xs3_memcpy

#define a           r0
#define b           r1
#define len         r2
#define tmp         r3

.text
.issue_mode dual
.align 4

.cc_top FUNCTION_NAME.function,FUNCTION_NAME

FUNCTION_NAME:
    dualentsp NSTACKWORDS
  { shr tmp, len, 5               ; zext len, 5                   }
  { ldc r11, 32                   ; bt tmp, .L_loop_bot           } 
  .L_loop_top:
    { add b, b, r11                 ; vldd b[0]                     }
    { sub tmp, tmp, 1               ; vstd a[0]                     }
    { add a, a, r11                 ; bt tmp, .L_loop_top           }
  .L_loop_bot:
  { mkmsk len, len                ; bf len, .L_finish             }
  { mov r11, b                    ;                               }
  {                               ; vldr r11[0]                   }
    vstrpv a[0], len
.L_finish:
    retsp NSTACKWORDS


.L_func_end:
.cc_bottom FUNCTION_NAME.function

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;  .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;               .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;              .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;            .global FUNCTION_NAME.maxchanends


#endif //defined(__XS3A__)