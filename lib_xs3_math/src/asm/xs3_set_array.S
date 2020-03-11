
#if defined(__XS3A__)
#ifndef XS3_MATH_NO_ASM

#include "asm_helper.h"


/*  
void xs3_set_array_s16(
    int16_t data[],
    const int16_t value,
    const unsigned length);
*/

/*  
void xs3_set_array_s32(
    int32_t data[],
    const int32_t value,
    const unsigned length);
*/

/*  
void xs3_set_array_complex_s32(
    complex_s32_t data[],
    const int32_t real_part,
    const int32_t imag_part,
    const unsigned length);
*/

#define NSTACKWORDS     10

#define STACK_TMP_VEC       (NSTACKWORDS-8)
#define STACK_TMP_VEC_DBL   ((STACK_TMP_VEC)/2)

#define data    r0
#define value   r1
#define length  r2


.text
.issue_mode dual

ASM_PREAMBLE_LCL(set_bytes);
.L_set_bytes:


/**/    std r11, value, sp[STACK_TMP_VEC_DBL+0]
        std r11, value, sp[STACK_TMP_VEC_DBL+1]
        std r11, value, sp[STACK_TMP_VEC_DBL+2]
        std r11, value, sp[STACK_TMP_VEC_DBL+3]

/**/{                                           ;   ldaw r11, sp[STACK_TMP_VEC]         }
    {   mkmsk r11, r3                           ;   vldr r11[0]                         }  
    {   ldc r3, 32                              ;   bf length, .L_loop_bot              }
    {                                           ;   bu .L_loop_top                      }

.align 16
.L_loop_top:
/**/{   sub length, length, 1                   ;   vstr data[0]                        }
    {   add data, data, r3                      ;   bt length, .L_loop_top              }

.L_loop_bot:
    vstrpv data[0], r11
    retsp NSTACKWORDS

ASM_POSTAMBLE_LCL(set_bytes);

//////////////////////////////////////////////////////////////////////////////////////////////////

ASM_PREAMBLE(xs3_set_array_s16)
.align 16
xs3_set_array_s16:
/**/    dualentsp NSTACKWORDS
    {   shl r11, value, 16                      ;   shl r3, length, SIZEOF_LOG2_S16     }
    {   or r11, r11, value                      ;   zext r3, 5                          }
    {   mov value, r11                          ;   shr length, length, EPV_LOG2_S16    }
    {                                           ;   bu .L_set_bytes                     }
ASM_POSTAMBLE(xs3_set_array_s16, NSTACKWORDS)

ASM_PREAMBLE(xs3_set_array_s32)
.align 16
xs3_set_array_s32:
/**/    dualentsp NSTACKWORDS
    {   mov r11, value                          ;   shl r3, length, SIZEOF_LOG2_S32     }
    {   zext r3, 5                              ;   shr length, length, EPV_LOG2_S32    }
    {                                           ;   bu .L_set_bytes                     }
ASM_POSTAMBLE(xs3_set_array_s32, NSTACKWORDS)

#undef value
#undef length

#define real r1
#define imag r2
#define length r3
ASM_PREAMBLE(xs3_set_array_complex_s32)
.align 16
xs3_set_array_complex_s32:
/**/    dualentsp NSTACKWORDS
    {   mov r11, imag                           ;   shl r3, length, SIZEOF_LOG2_C32     }
    {   shr r2, length, 5                       ;   zext r3, 5                          }
    {                                           ;   bu .L_set_bytes                     }
ASM_POSTAMBLE(xs3_set_array_complex_s32, NSTACKWORDS)


#endif //!defined(XS3_MATH_NO_ASM)
#endif //defined(__XS3A__)
