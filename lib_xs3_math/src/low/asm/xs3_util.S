
#if defined(__XS3A__)
#ifndef XS3_MATH_NO_ASM

#include "asm_helper.h"

.text

.issue_mode dual
/*  
void xs3_unpack_float(
    int32_t* mantissa,
    exponent_t* exp,
    float input);
*/
#define FUNCTION_NAME   xs3_unpack_float
#define NSTACKWORDS     (0)

ASM_PREAMBLE_V2(FUNCTION_NAME);
FUNCTION_NAME:
    dualentsp NSTACKWORDS
    fsexp r3, r11, r2
    fmant r2, r2
    ldaw r11, r11[-6]
{   add r11, r11, 1             ;   bf r3, .L_xuf_lblA          }
{   neg r2, r2                  ;                               }
    .L_xuf_lblA:
{                               ;   stw r2, r0[0]               }
{                               ;   stw r11, r1[0]              }
    retsp NSTACKWORDS
ASM_POSTAMBLE(FUNCTION_NAME, NSTACKWORDS);
#undef NSTACKWORDS
#undef FUNCTION_NAME

.issue_mode dual



#endif //!defined(XS3_MATH_NO_ASM)
#endif //defined(__XS3A__)



