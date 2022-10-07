// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


/*  
headroom_t vect_s32_copy(
    int32_t a[],
    const int32_t b[],
    unsigned length);
*/

#define NSTACKWORDS     (0)
#define FUNCTION_NAME   vect_s32_copy

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
  { ldc r11, 0                                ; shr tmp, len, 3                       }
  { zext tmp, 1                               ; shr len, tmp, 1                       }
  { add len, len, tmp                         ; vsetc r11                             }
  { ldc r11, 32                               ; bt tmp, .L_loop_mid                   }
  {                                           ; bu .L_loop_top                        }
  
  .align 16
  .L_loop_top:
    { add b, b, r11                             ; vldd b[0]                             }
    { add a, a, r11                             ; vstd a[0]                             }
    .L_loop_mid:
    { add b, b, r11                             ; vldd b[0]                             }
    { sub len, len, 1                           ; vstd a[0]                             }
    { add a, a, r11                             ; bt len, .L_loop_top                   }
  .L_loop_bot:

.L_finish:
  { ldc r0, 31                                ; vgetc r11                             }
  { zext r11, 5                               ;                                       }
  { sub r0, r0, r11                           ; retsp NSTACKWORDS                     }


.L_func_end:
.cc_bottom FUNCTION_NAME.function

.globl FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS;  .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;               .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;              .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;            .global FUNCTION_NAME.maxchanends


#endif //defined(__XS3A__)