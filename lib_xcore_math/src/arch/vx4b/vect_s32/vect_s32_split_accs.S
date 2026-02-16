// Copyright 2021-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// XMOS Public License: Version 1

#if defined(__VX4B__)


#include "../asm_helper.h"

/*  
    void vect_s32_split_accs(
        split_acc_s32_t a[],
        const int32_t b[],
        const unsigned length);
*/


#define NSTACKWORDS     (20)

#define FUNCTION_NAME   vect_s32_split_accs

#define split     a0
#define merged    a1
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
  { add len, len, t3                    ; nop                                   }
  { srli len, len, 4                    ; addi t3,sp, 0                         }

  .L_loop_top:

      xm.lddi  tmpD,tmpR, 0(merged)
xm.zip tmpR, tmpD, 4
    { nop                               ; sw tmpR, 0                           (sp) }
    { nop                               ; sw tmpD, 32                           (sp) }
    
      xm.lddi  tmpD,tmpR, 8(merged)
xm.zip tmpR, tmpD, 4
    { nop                               ; sw tmpR, 4                           (sp) }
    { nop                               ; sw tmpD, 36                           (sp) }
    
      xm.lddi  tmpD,tmpR, 16(merged)
xm.zip tmpR, tmpD, 4
    { nop                               ; sw tmpR, 8                           (sp) }
    { nop                               ; sw tmpD, 40                          (sp) }
    
      xm.lddi  tmpD,tmpR, 24(merged)
xm.zip tmpR, tmpD, 4
    { add merged, merged, _32           ; sw tmpR, 12                           (sp) }
    { nop                               ; sw tmpD, 44                          (sp) }
    
      xm.lddi  tmpD,tmpR, 0(merged)
xm.zip tmpR, tmpD, 4
    { nop                               ; sw tmpR, 16                           (sp) }
    { nop                               ; sw tmpD, 48                          (sp) }
    
      xm.lddi  tmpD,tmpR, 8(merged)
xm.zip tmpR, tmpD, 4
    { nop                               ; sw tmpR, 20                           (sp) }
    { nop                               ; sw tmpD, 52                          (sp) }
    
      xm.lddi  tmpD,tmpR, 16(merged)
xm.zip tmpR, tmpD, 4
    { nop                               ; sw tmpR, 24                           (sp) }
    { nop                               ; sw tmpD, 56                          (sp) }
    
      xm.lddi  tmpD,tmpR, 24(merged)
xm.zip tmpR, tmpD, 4
    { nop                               ; sw tmpR, 28                           (sp) }
    { addi len, len, -1                 ; sw tmpD, 60                          (sp) }


    { addi t3,sp, 32                    ; xm.vldd t3                            }
    { add split, split, _32             ; xm.vstd split                         }
    { addi t3,sp, 0                     ; xm.vldd t3                            }
    { add split, split, _32             ; xm.vstd split                         }

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



