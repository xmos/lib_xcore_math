
#if defined(__XS3A__)
#ifndef XS3_MATH_NO_ASM

#include "asm_helper.h"


.text
.issue_mode dual

#define NSTACKWORDS     10

#define STACK_TMP_VEC       (NSTACKWORDS-8)
#define STACK_TMP_VEC_DBL   ((STACK_TMP_VEC)/2)

#define data    r0
#define value   r1
#define length  r2


/*  
void xs3_set_vect_s16(
    int16_t data[],
    const int16_t value,
    const unsigned length);
*/
.cc_top xs3_set_vect_s16.function,xs3_set_vect_s16
xs3_set_vect_s16:
        dualentsp NSTACKWORDS
    {   shl r11, value, 16                      ;   shl r3, length, SIZEOF_LOG2_S16     }
    {   zext value, 16                          ;   zext r3, 5                          }
    {   or r11, r11, value                      ;   shr length, length, EPV_LOG2_S16    }
    {   mov value, r11                          ;   bu .L_set_bytes                     }
.cc_bottom xs3_set_vect_s16.function
.L_size_end_xs3_set_vect_s16: 
    .size xs3_set_vect_s16, .L_size_end_xs3_set_vect_s16 - xs3_set_vect_s16


/*  
void xs3_set_vect_s32(
    int32_t data[],
    const int32_t value,
    const unsigned length);
*/
.cc_top xs3_set_vect_s32.function,xs3_set_vect_s32
xs3_set_vect_s32:
        dualentsp NSTACKWORDS
    {   mov r11, value                          ;   shl r3, length, SIZEOF_LOG2_S32     }
    {   zext r3, 5                              ;   shr length, length, EPV_LOG2_S32    }
    {                                           ;   bu .L_set_bytes                     }
.cc_bottom xs3_set_vect_s32.function
.L_size_end_xs3_set_vect_s32: 
    .size xs3_set_vect_s32, .L_size_end_xs3_set_vect_s32 - xs3_set_vect_s32

#undef value
#undef length
#define real r1
#define imag r2
#define length r3

/*  
void xs3_set_vect_complex_s32(
    complex_s32_t data[],
    const int32_t real_part,
    const int32_t imag_part,
    const unsigned length);
*/
.cc_top xs3_set_vect_complex_s32.function,xs3_set_vect_complex_s32
xs3_set_vect_complex_s32:
        dualentsp NSTACKWORDS
    {   mov r11, imag                           ;   shl r3, length, SIZEOF_LOG2_C32     }
    {   shr r2, length, 5                       ;   zext r3, 5                          }
    {                                           ;   bu .L_set_bytes                     }
.cc_bottom xs3_set_vect_complex_s32.function
.L_size_end_xs3_set_vect_complex_s32: 
    .size xs3_set_vect_complex_s32, .L_size_end_xs3_set_vect_complex_s32 - xs3_set_vect_complex_s32




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





.globl xs3_set_vect_s16
.type xs3_set_vect_s16,@function
.set xs3_set_vect_s16.nstackwords,NSTACKWORDS;  .global xs3_set_vect_s16.nstackwords; 
.set xs3_set_vect_s16.maxcores,1;               .global xs3_set_vect_s16.maxcores; 
.set xs3_set_vect_s16.maxtimers,0;              .global xs3_set_vect_s16.maxtimers; 
.set xs3_set_vect_s16.maxchanends,0;            .global xs3_set_vect_s16.maxchanends; 

.globl xs3_set_vect_s32
.type xs3_set_vect_s32,@function
.set xs3_set_vect_s32.nstackwords,NSTACKWORDS;  .global xs3_set_vect_s32.nstackwords; 
.set xs3_set_vect_s32.maxcores,1;               .global xs3_set_vect_s32.maxcores; 
.set xs3_set_vect_s32.maxtimers,0;              .global xs3_set_vect_s32.maxtimers; 
.set xs3_set_vect_s32.maxchanends,0;            .global xs3_set_vect_s32.maxchanends; 

.globl xs3_set_vect_complex_s32
.type xs3_set_vect_complex_s32,@function
.set xs3_set_vect_complex_s32.nstackwords,NSTACKWORDS;  .global xs3_set_vect_complex_s32.nstackwords; 
.set xs3_set_vect_complex_s32.maxcores,1;               .global xs3_set_vect_complex_s32.maxcores; 
.set xs3_set_vect_complex_s32.maxtimers,0;              .global xs3_set_vect_complex_s32.maxtimers; 
.set xs3_set_vect_complex_s32.maxchanends,0;            .global xs3_set_vect_complex_s32.maxchanends; 

#endif //!defined(XS3_MATH_NO_ASM)
#endif //defined(__XS3A__)
