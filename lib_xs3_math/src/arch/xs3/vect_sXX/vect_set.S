// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


#include "../asm_helper.h"


.text
.issue_mode dual
.align 4

#define NSTACKWORDS     10

#define STACK_TMP_VEC       (NSTACKWORDS-8)
#define STACK_TMP_VEC_DBL   ((STACK_TMP_VEC)/2)

#define data    r0
#define value   r1
#define length  r2


/*  
void vect_s16_set(
    int16_t data[],
    const int16_t value,
    const unsigned length);
*/
.cc_top vect_s16_set.function,vect_s16_set
vect_s16_set:
        dualentsp NSTACKWORDS
    {   shl r11, value, 16                      ;   shl r3, length, SIZEOF_LOG2_S16     }
    {   zext value, 16                          ;   zext r3, 5                          }
    {   or r11, r11, value                      ;   shr length, length, EPV_LOG2_S16    }
    {   mov value, r11                          ;   bu .L_set_bytes                     }
.cc_bottom vect_s16_set.function
.L_size_end_vect_s16_set: 
    .size vect_s16_set, .L_size_end_vect_s16_set - vect_s16_set


/*  
void vect_s32_set(
    int32_t data[],
    const int32_t value,
    const unsigned length);
*/
.cc_top vect_s32_set.function,vect_s32_set
vect_s32_set:
        dualentsp NSTACKWORDS
    {   mov r11, value                          ;   shl r3, length, SIZEOF_LOG2_S32     }
    {   zext r3, 5                              ;   shr length, length, EPV_LOG2_S32    }
    {                                           ;   bu .L_set_bytes                     }
.cc_bottom vect_s32_set.function
.L_size_end_vect_s32_set: 
    .size vect_s32_set, .L_size_end_vect_s32_set - vect_s32_set

#undef value
#undef length
#define real r1
#define imag r2
#define length r3

/*  
void vect_complex_s32_set(
    complex_s32_t data[],
    const int32_t real_part,
    const int32_t imag_part,
    const unsigned length);
*/
.cc_top vect_complex_s32_set.function,vect_complex_s32_set
vect_complex_s32_set:
        dualentsp NSTACKWORDS
    {   mov r11, imag                           ;   shl r3, length, SIZEOF_LOG2_C32     }
    {   shr r2, length, 5                       ;   zext r3, 5                          }
    {                                           ;   bu .L_set_bytes                     }
.cc_bottom vect_complex_s32_set.function
.L_size_end_vect_complex_s32_set: 
    .size vect_complex_s32_set, .L_size_end_vect_complex_s32_set - vect_complex_s32_set




#undef real 
#undef imag 
#undef length 
#define value   r1
#define length  r2

/*
    Code shared by all functions above.
*/
.type .L_set_bytes,@function
.cc_top .L_set_bytes.function,.L_set_bytes
.L_set_bytes:
        std r11, value, sp[STACK_TMP_VEC_DBL+0]
        std r11, value, sp[STACK_TMP_VEC_DBL+1]
        std r11, value, sp[STACK_TMP_VEC_DBL+2]
        std r11, value, sp[STACK_TMP_VEC_DBL+3]
    {                                           ;   ldaw r11, sp[STACK_TMP_VEC]         }
    {   mkmsk r11, r3                           ;   vldr r11[0]                         }  
    {   ldc r3, 32                              ;   bf length, .L_loop_bot              }
    {                                           ;   bu .L_loop_top                      }
.align 8
    .L_loop_top:
        {   sub length, length, 1                   ;   vstr data[0]                        }
        {   add data, data, r3                      ;   bt length, .L_loop_top              }
.L_loop_bot:
    vstrpv data[0], r11
    retsp NSTACKWORDS

.cc_bottom .L_set_bytes.function
.L_end_set_bytes: 
    .size .L_set_bytes, .L_end_set_bytes - .L_set_bytes





.globl vect_s16_set
.type vect_s16_set,@function
.set vect_s16_set.nstackwords,NSTACKWORDS;  .global vect_s16_set.nstackwords; 
.set vect_s16_set.maxcores,1;               .global vect_s16_set.maxcores; 
.set vect_s16_set.maxtimers,0;              .global vect_s16_set.maxtimers; 
.set vect_s16_set.maxchanends,0;            .global vect_s16_set.maxchanends; 

.globl vect_s32_set
.type vect_s32_set,@function
.set vect_s32_set.nstackwords,NSTACKWORDS;  .global vect_s32_set.nstackwords; 
.set vect_s32_set.maxcores,1;               .global vect_s32_set.maxcores; 
.set vect_s32_set.maxtimers,0;              .global vect_s32_set.maxtimers; 
.set vect_s32_set.maxchanends,0;            .global vect_s32_set.maxchanends; 

.globl vect_complex_s32_set
.type vect_complex_s32_set,@function
.set vect_complex_s32_set.nstackwords,NSTACKWORDS;  .global vect_complex_s32_set.nstackwords; 
.set vect_complex_s32_set.maxcores,1;               .global vect_complex_s32_set.maxcores; 
.set vect_complex_s32_set.maxtimers,0;              .global vect_complex_s32_set.maxtimers; 
.set vect_complex_s32_set.maxchanends,0;            .global vect_complex_s32_set.maxchanends; 


#endif //defined(__XS3A__)
