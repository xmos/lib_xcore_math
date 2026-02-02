// Copyright 2021-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


/*  

  M_rows MUST be a multiple of 16, and N_cols MUST be a multiple of 32

void mat_mul_s8_x_s8_yield_s32 (
    split_acc_s32_t accumulators[],
    const int8_t matrix[],
    const int8_t input_vect[],
    const unsigned M_rows,
    const unsigned N_cols);
*/


#include "../asm_helper.h"

#define NSTACKWORDS     (12+8+4)

#define FUNCTION_NAME   mat_mul_s8_x_s8_yield_s32

#define STACK_VEC_TMP   (NSTACKWORDS-8-4)

#define STACK_M_ROWS      (9)
#define STACK_INPUT_VECT  (9)

#define accs            a0 
#define matrix          a1 
#define vector          a2
#define rows_left       a3
#define cols_left       x18
#define _32             x19
#define N_cols          s4
#define mat_stride_B    s5
#define mat_stride_C    x22

#define K               s8

.text
.p2align 2



FUNCTION_NAME:
    xm.entsp (NSTACKWORDS)*4
    li t3, 0x200
    xm.stdsp  s3,s2,8
    xm.stdsp  s5,s4,16
    xm.stdsp  s7,s6,24
  { li _32, 32                          ; sw s8, 4                            (sp) }
  { li s8, 15                           ; xm.vsetc t3                           }
    add rows_left, rows_left, s8
    mv N_cols, a4
  { srli rows_left, rows_left, 4        ; nop                                   }
  { slli mat_stride_B, N_cols, 4        ; mv K, N_cols                          }
  { sub mat_stride_C, mat_stride_B, N_cols ; li s7, 31                             }
  { add mat_stride_B, mat_stride_C, _32 ; sw vector, (STACK_INPUT_VECT)*4          (sp) }

  { xm.zexti K, 5                       ; add s7, N_cols, s7                    }
  { sub K, _32, K                       ; srli s7, s7, 5                        }
  { xm.zexti K, 5                       ; nop                                   }
  { add matrix, matrix, K               ; sub mat_stride_C, mat_stride_C, K     }
  mv a4, N_cols
  { slli K, s7, 5                       ; nop                                   }
  
  { add matrix, matrix, mat_stride_C    ; xm.bu .L_output_group_top             }

  .p2align 4
  .L_output_group_top:
    { add t3, accs, _32                 ; xm.vldd accs                          }
    { mv cols_left, K                   ; xm.vldr t3                            }
    { addi rows_left, rows_left, -1     ; lw vector, (STACK_INPUT_VECT)*4          (sp) }
    
    .L_input_group_top:
      { add vector, vector, _32         ; xm.vldc vector                        }
      { sub matrix, matrix, N_cols      ; xm.vlmaccr0 matrix                    }
      { sub matrix, matrix, N_cols      ; xm.vlmaccr0 matrix                    }
      { sub matrix, matrix, N_cols      ; xm.vlmaccr0 matrix                    }
      { sub matrix, matrix, N_cols      ; xm.vlmaccr0 matrix                    }
      { sub matrix, matrix, N_cols      ; xm.vlmaccr0 matrix                    }
      { sub matrix, matrix, N_cols      ; xm.vlmaccr0 matrix                    }
      { sub matrix, matrix, N_cols      ; xm.vlmaccr0 matrix                    }
      { sub matrix, matrix, N_cols      ; xm.vlmaccr0 matrix                    }
      { sub matrix, matrix, N_cols      ; xm.vlmaccr0 matrix                    }
      { sub matrix, matrix, N_cols      ; xm.vlmaccr0 matrix                    }
      { sub matrix, matrix, N_cols      ; xm.vlmaccr0 matrix                    }
      { sub matrix, matrix, N_cols      ; xm.vlmaccr0 matrix                    }
      { sub matrix, matrix, N_cols      ; xm.vlmaccr0 matrix                    }
      { sub matrix, matrix, N_cols      ; xm.vlmaccr0 matrix                    }
      { sub matrix, matrix, N_cols      ; xm.vlmaccr0 matrix                    }
      { sub cols_left, cols_left, _32   ; xm.vlmaccr0 matrix                    }
      { add matrix, matrix, mat_stride_B ; xm.bt cols_left, .L_input_group_top   }
    .L_input_group_bottom:

    { add accs, accs, _32               ; xm.vstd accs                          }
    { add accs, accs, _32               ; xm.vstr accs                          }
    { add matrix, matrix, mat_stride_C  ; xm.bt rows_left, .L_output_group_top  }
  .L_output_group_bottom:
    
.L_finish:
      xm.lddsp  s3,s2,8
      xm.lddsp  s5,s4,16
      xm.lddsp  s7,s6,24
  { nop                                 ; lw s8, 4                          (sp) }
  { nop                                 ; xm.retsp (NSTACKWORDS)*4              } 

.L_func_end:


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS; .global FUNCTION_NAME.nstackwords
.set FUNCTION_NAME.maxcores,1;              .global FUNCTION_NAME.maxcores
.set FUNCTION_NAME.maxtimers,0;             .global FUNCTION_NAME.maxtimers
.set FUNCTION_NAME.maxchanends,0;           .global FUNCTION_NAME.maxchanends
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME



#endif //defined(__VX4B__)



