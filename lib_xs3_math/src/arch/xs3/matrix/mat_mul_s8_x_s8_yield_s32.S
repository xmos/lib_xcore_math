// Copyright 2021-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


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

#define NSTACKWORDS     (10+8)

#define FUNCTION_NAME   mat_mul_s8_x_s8_yield_s32

#define STACK_N_COLS    (NSTACKWORDS+1)

#define STACK_VEC_TMP   (NSTACKWORDS-8)

#define STACK_M_ROWS      (9)
#define STACK_INPUT_VECT  (9)

#define accs            r0 
#define matrix          r1 
#define vector          r2
#define rows_left       r3
#define cols_left       r4
#define _32             r5
#define N_cols          r6
#define mat_stride_B    r7
#define mat_stride_C    r8

#define K               r10

.text
.issue_mode dual
.align 4

.cc_top FUNCTION_NAME.function,FUNCTION_NAME


FUNCTION_NAME:
    dualentsp NSTACKWORDS
    ldc r11, 0x200
    std r4, r5, sp[1]
    std r6, r7, sp[2]
    std r8, r9, sp[3]
  { ldc _32, 32                               ; stw r10, sp[1]                            }
  { ldc r10, 15                               ; vsetc r11                                 }
    add rows_left, rows_left, r10
  { shr rows_left, rows_left, 4               ; ldw N_cols, sp[STACK_N_COLS]              }
  { shl mat_stride_B, N_cols, 4               ; mov K, N_cols                             }
  { sub mat_stride_C, mat_stride_B, N_cols    ; ldc r9, 31                                }
  { add mat_stride_B, mat_stride_C, _32       ; stw vector, sp[STACK_INPUT_VECT]          }

  { zext K, 5                                 ; add r9, N_cols, r9                        }
  { sub K, _32, K                             ; shr r9, r9, 5                             }
  { zext K, 5                                 ;                                           }
  { add matrix, matrix, K                     ; sub mat_stride_C, mat_stride_C, K         }
  { shl K, r9, 5                              ; stw N_cols, sp[STACK_N_COLS]              }
  
  { add matrix, matrix, mat_stride_C          ; bu .L_output_group_top                    }

  .align 16
  .L_output_group_top:
    { add r11, accs, _32                        ; vldd accs[0]                              }
    { mov cols_left, K                          ; vldr r11[0]                               }
    { sub rows_left, rows_left, 1               ; ldw vector, sp[STACK_INPUT_VECT]          }
    
    .L_input_group_top:
      { add vector, vector, _32                   ; vldc vector[0]                            }
      { sub matrix, matrix, N_cols                ; vlmaccr matrix[0]                         }
      { sub matrix, matrix, N_cols                ; vlmaccr matrix[0]                         }
      { sub matrix, matrix, N_cols                ; vlmaccr matrix[0]                         }
      { sub matrix, matrix, N_cols                ; vlmaccr matrix[0]                         }
      { sub matrix, matrix, N_cols                ; vlmaccr matrix[0]                         }
      { sub matrix, matrix, N_cols                ; vlmaccr matrix[0]                         }
      { sub matrix, matrix, N_cols                ; vlmaccr matrix[0]                         }
      { sub matrix, matrix, N_cols                ; vlmaccr matrix[0]                         }
      { sub matrix, matrix, N_cols                ; vlmaccr matrix[0]                         }
      { sub matrix, matrix, N_cols                ; vlmaccr matrix[0]                         }
      { sub matrix, matrix, N_cols                ; vlmaccr matrix[0]                         }
      { sub matrix, matrix, N_cols                ; vlmaccr matrix[0]                         }
      { sub matrix, matrix, N_cols                ; vlmaccr matrix[0]                         }
      { sub matrix, matrix, N_cols                ; vlmaccr matrix[0]                         }
      { sub matrix, matrix, N_cols                ; vlmaccr matrix[0]                         }
      { sub cols_left, cols_left, _32             ; vlmaccr matrix[0]                         }
      { add matrix, matrix, mat_stride_B          ; bt cols_left, .L_input_group_top          }
    .L_input_group_bottom:

    { add accs, accs, _32                       ; vstd accs[0]                              }
    { add accs, accs, _32                       ; vstr accs[0]                              }
    { add matrix, matrix, mat_stride_C          ; bt rows_left, .L_output_group_top         }
  .L_output_group_bottom:
    
.L_finish:
      ldd r4, r5, sp[1]
      ldd r6, r7, sp[2]
      ldd r8, r9, sp[3]
  {                                           ; ldw r10, sp[1]                          }
  {                                           ; retsp NSTACKWORDS                       } 

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



