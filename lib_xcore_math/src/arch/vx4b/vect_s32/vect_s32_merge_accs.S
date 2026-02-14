// Copyright 2021-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// XMOS Public License: Version 1

#if defined(__VX4B__)


#include "../asm_helper.h"

/*  
    void vect_s32_merge_accs(
        int32_t a[],
        split_acc_s32_t b[],
        const unsigned length);
*/


#define NSTACKWORDS     (20)

#define FUNCTION_NAME   vect_s32_merge_accs

#define merged    a0
#define split     a1
#define len       a2
#define _32       a3

#define tmpR      s2
#define tmpD      s3


.text
.p2align 2


FUNCTION_NAME:
    xm.entsp (NSTACKWORDS)*4
    xm.stdsp  s3,s2,64

  { li t3, 15                           ; li _32, 32                            }
  { add len, len, t3                    ; sub merged, merged, _32               }
  { srli len, len, 4                    ; addi t3,sp, 0                         }

  .L_loop_top:

    { add split, split, _32             ; xm.vldd split                         }
    { addi t3,sp, 32                    ; xm.vstd t3                            }
    { add split, split, _32             ; xm.vldd split                         }
    { addi t3,sp, 0                     ; xm.vstd t3                            }
    { addi len, len, -1                 ; lw tmpD, 0                           (sp) }
    { add merged, merged, _32           ; lw tmpR, 32                           (sp) }
xm.unzip  tmpD, tmpR, 4
      xm.stdi  tmpR,tmpD, 0(merged)
    { nop                               ; lw tmpD, 4                           (sp) }
    { nop                               ; lw tmpR, 36                           (sp) }
xm.unzip  tmpD, tmpR, 4
      xm.stdi  tmpR,tmpD, 8(merged)
    { nop                               ; lw tmpD, 8                           (sp) }
    { nop                               ; lw tmpR, 40                          (sp) }
xm.unzip  tmpD, tmpR, 4
      xm.stdi  tmpR,tmpD, 16(merged)
    { nop                               ; lw tmpD, 12                           (sp) }
    { nop                               ; lw tmpR, 44                          (sp) }
xm.unzip  tmpD, tmpR, 4
      xm.stdi  tmpR,tmpD, 24(merged)
    { nop                               ; lw tmpD, 16                           (sp) }
    { nop                               ; lw tmpR, 48                          (sp) }
xm.unzip  tmpD, tmpR, 4
      xm.stdi  tmpR,tmpD, 32(merged)
    { nop                               ; lw tmpD, 20                           (sp) }
    { nop                               ; lw tmpR, 52                          (sp) }
xm.unzip  tmpD, tmpR, 4
      xm.stdi  tmpR,tmpD, 40(merged)
    { nop                               ; lw tmpD, 24                           (sp) }
    { nop                               ; lw tmpR, 56                          (sp) }
xm.unzip  tmpD, tmpR, 4
      xm.stdi  tmpR,tmpD, 48(merged)
    { nop                               ; lw tmpD, 28                           (sp) }
    { nop                               ; lw tmpR, 60                          (sp) }
xm.unzip  tmpD, tmpR, 4
      xm.stdi  tmpR,tmpD, 56(merged)
    { add merged, merged, _32           ; nop                                   }
    bnez  len, .L_loop_top 

  .L_finish:
      xm.lddsp  s3,s2,64
    { nop                               ; xm.retsp (NSTACKWORDS)*4              } 


.L_func_end:


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME


#endif //defined(__VX4B__)



