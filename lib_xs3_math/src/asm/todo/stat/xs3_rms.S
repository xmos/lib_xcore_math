
#if defined(__XS3A__)

#include "asm_helper.h"


#define NSTACKWORDS     (6)



    // {                                           ;                                       }
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

/*  
int16_t xs3_rms_s16(
    const int16_t* b,
    const unsigned length,
    const int b_shr);
*/

#define FUNCTION_NAME xs3_rms_s16
        
ASM_PREAMBLE(FUNCTION_NAME)

.align 4

.L_mode16:
.int 0x0100

FUNCTION_NAME:
    dualentsp NSTACKWORDS
    std r4, r5, sp[1]
    std r6, r7, sp[2]


.L_done_s16:

    ldd r4, r5, sp[1]
    ldd r6, r7, sp[2]

    {   ldc r0, 15                              ;   vgetc r11                           }
    {   zext r11, 5                             ;                                       }
    {   sub r0, r0, r11                         ;   retsp NSTACKWORDS                   }


ASM_POSTAMBLE(FUNCTION_NAME, NSTACKWORDS)

#undef FUNCTION_NAME

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

/*  
int32_t xs3_rms_s32(
    const int32_t* b,
    const unsigned length,
    const int b_shr);
*/

#define FUNCTION_NAME xs3_rms_s32
    
ASM_PREAMBLE(FUNCTION_NAME)

FUNCTION_NAME:
    dualentsp NSTACKWORDS
    std r4, r5, sp[1]
    std r6, r7, sp[2]

.L_done_s32:

    ldd r4, r5, sp[1]
    ldd r6, r7, sp[2]

    {   ldc r0, 31                              ;   vgetc r11                           }
    {   zext r11, 5                             ;                                       }
    {   sub r0, r0, r11                         ;   retsp NSTACKWORDS                   }


ASM_POSTAMBLE(FUNCTION_NAME, NSTACKWORDS)

#undef FUNCTION_NAME


//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////


#endif //defined(__XS3A__)



