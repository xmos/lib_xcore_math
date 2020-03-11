
#if defined(__XS3A__)

#include "asm_helper.h"

#define NSTACKWORDS     (6+8)

#define a           r0 
#define b           r1 
#define c           r2
#define len         r3
#define shr_b       r4
#define shr_c       r5
#define tmp         r6
#define tmp_vec     r7

#define bytemask    len

#define STACK_BYTEMASK  1

    // {                                           ;                                       }
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

/*  
headroom_t xs3_complex_conj_mul_vect_complex_s16(
    int16_t* a_real,
    int16_t* a_imag,
    const int16_t* b_real,
    const int16_t* b_imag,
    const int16_t* c_real,
    const int16_t* c_imag,
    const unsigned length,
    const int b_shr,
    const int c_shr);
*/

#define FUNCTION_NAME xs3_complex_conj_mul_vect_complex_s16
        
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


#endif //defined(__XS3A__)



