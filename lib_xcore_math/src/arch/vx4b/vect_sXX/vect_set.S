// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


#include "../asm_helper.h"


.text
.p2align 2

#define NSTACKWORDS     32

#define STACK_TMP_VEC       (NSTACKWORDS-16)
#define STACK_TMP_VEC_DBL   ((STACK_TMP_VEC)/2)

#define data    x10
#define value   x11
#define length  x12


/*  
void vect_s16_set(
    int16_t data[],
    const int16_t value,
    const unsigned length);
*/
vect_s16_set:
        xm.entsp (NSTACKWORDS)*4
    {   slli t3, value, 16                    ;   slli a3, length, SIZEOF_LOG2_S16     }
    {   xm.zexti value, 16                    ;   xm.zexti a3, 5                       }
    {   or t3, t3, value                      ;   srli length, length, EPV_LOG2_S16    }
    {   mv value, t3                          ;   xm.bu .L_set_bytes                   }
.L_size_end_vect_s16_set: 
    .size vect_s16_set, .L_size_end_vect_s16_set - vect_s16_set


/*  
void vect_s32_set(
    int32_t data[],
    const int32_t value,
    const unsigned length);
*/
vect_s32_set:
        xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
    {   mv t3, value                          ;   slli a3, length, SIZEOF_LOG2_S32     }/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shli a3, length, SIZEOF_LOG2_S32     \nMessage: The shift amount is not 32" */
    {   xm.zexti a3, 5                              ;   srli length, length, EPV_LOG2_S32    }/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shri length, length, EPV_LOG2_S32    \nMessage: The shift amount is not 32" */
    { nop                                           ;   xm.bu .L_set_bytes                     }
.L_size_end_vect_s32_set: 
    .size vect_s32_set, .L_size_end_vect_s32_set - vect_s32_set

#undef value
#undef length
#define real x11
#define imag x12
#define length x13

/*  
void vect_complex_s32_set(
    complex_s32_t data[],
    const int32_t real_part,
    const int32_t imag_part,
    const unsigned length);
*/
vect_complex_s32_set:
        xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
    {   mv t3, imag                           ;   slli a3, length, SIZEOF_LOG2_C32     }/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shli a3, length, SIZEOF_LOG2_C32     \nMessage: The shift amount is not 32" */
    {   srli a2, length, 5                       ;   xm.zexti a3, 5                          }
    { nop                                           ;   xm.bu .L_set_bytes                     }
.L_size_end_vect_complex_s32_set: 
    .size vect_complex_s32_set, .L_size_end_vect_complex_s32_set - vect_complex_s32_set




#undef real 
#undef imag 
#undef length 
#define value   x11
#define length  x12

/*
    Code shared by all functions above.
*/
.type .L_set_bytes,@function
.L_set_bytes:
        xm.stdsp  value,t3,(STACK_TMP_VEC_DBL+0)*8
        xm.stdsp  value,t3,(STACK_TMP_VEC_DBL+1)*8
        xm.stdsp  value,t3,(STACK_TMP_VEC_DBL+2)*8
        xm.stdsp  value,t3,(STACK_TMP_VEC_DBL+3)*8
    { nop                                           ;   addi t3,sp, (STACK_TMP_VEC)*4         }
    {   xm.mkmsk t3, a3                           ;   xm.vldr t3}  
    {   li a3, 32                              ;   xm.brff length, .L_loop_bot              }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
    { nop                                           ;   xm.bu .L_loop_top                      }
.p2align 3
    .L_loop_top:
        {   addi length, length, -1                   ;   xm.vstr data}
        {   add data, data, a3                      ;   xm.bt length, .L_loop_top              }
.L_loop_bot:
    xm.vstrpv data, t3
    xm.retsp (NSTACKWORDS)*4/* Multiple XAT warnings: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in retsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate", 'RETSP operand may need scaling' */

.L_end_set_bytes: 
    .size .L_set_bytes, .L_end_set_bytes - .L_set_bytes





.globl vect_s16_set
.type vect_s16_set,@function
.set vect_s16_set.nstackwords,NSTACKWORDS;  .global vect_s16_set.nstackwords;  /* Translation error on this line: unexpected token at position 41. */ 
.set vect_s16_set.maxcores,1;               .global vect_s16_set.maxcores;  /* Translation error on this line: unexpected token at position 28. */ 
.set vect_s16_set.maxtimers,0;              .global vect_s16_set.maxtimers;  /* Translation error on this line: unexpected token at position 29. */ 
.set vect_s16_set.maxchanends,0;            .global vect_s16_set.maxchanends;  /* Translation error on this line: unexpected token at position 31. */ 

.globl vect_s32_set
.type vect_s32_set,@function
.set vect_s32_set.nstackwords,NSTACKWORDS;  .global vect_s32_set.nstackwords;  /* Translation error on this line: unexpected token at position 41. */ 
.set vect_s32_set.maxcores,1;               .global vect_s32_set.maxcores;  /* Translation error on this line: unexpected token at position 28. */ 
.set vect_s32_set.maxtimers,0;              .global vect_s32_set.maxtimers;  /* Translation error on this line: unexpected token at position 29. */ 
.set vect_s32_set.maxchanends,0;            .global vect_s32_set.maxchanends;  /* Translation error on this line: unexpected token at position 31. */ 

.globl vect_complex_s32_set
.type vect_complex_s32_set,@function
.set vect_complex_s32_set.nstackwords,NSTACKWORDS;  .global vect_complex_s32_set.nstackwords;  /* Translation error on this line: unexpected token at position 49. */ 
.set vect_complex_s32_set.maxcores,1;               .global vect_complex_s32_set.maxcores;  /* Translation error on this line: unexpected token at position 36. */ 
.set vect_complex_s32_set.maxtimers,0;              .global vect_complex_s32_set.maxtimers;  /* Translation error on this line: unexpected token at position 37. */ 
.set vect_complex_s32_set.maxchanends,0;            .global vect_complex_s32_set.maxchanends;  /* Translation error on this line: unexpected token at position 39. */ 


#endif //defined(__VX4B__)
