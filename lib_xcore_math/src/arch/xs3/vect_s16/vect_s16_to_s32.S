// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)


/*  
headroom_t vect_s16_to_vect_s32(
    int16_t a[],
    const int32_t b[],
    const unsigned length);
*/

#include "../asm_helper.h"

.text
.issue_mode dual
.align 4

#define NSTACKWORDS     (2)

#define FUNCTION_NAME   vect_s16_to_vect_s32

#define a               r0
#define b               r1
#define len             r2
#define _16             r3
#define tail            r4
#define constsA         r11
#define constsB         r11

.cc_top FUNCTION_NAME.function,FUNCTION_NAME

FUNCTION_NAME:
        dualentsp NSTACKWORDS
        ldc r11, 0x0200
    {   ldap r11, .L_vlmacc_consts_A            ;   vsetc r11                               }

        std r4, r5, sp[0]
    {   shl tail, len, SIZEOF_LOG2_S32          ;                                           }
    {   shr len, len, EPV_LOG2_S32              ;   zext tail, 5                            }
    {   mkmsk tail, tail                        ;   bf len, .L_loop_bot                     }
    {                                           ;   bu .L_loop_top                          }

.align 16

.L_vlmacc_consts_A:
.byte 0x7F, 0x00, 0x7F, 0x00, 0x7F, 0x00, 0x7F, 0x00, 0x7F, 0x00, 0x7F, 0x00, 0x7F, 0x00, 0x7F, 0x00
.L_vlmacc_consts_B:
.byte 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01

/*
    This function relies on some seriously horrifying deep magic. Try not to stare directly at it.

    We're converting 16-bit values to 32-bit values with the VPU in 8-bit mode. Because if we deal with a 16-bit value
    as two separate 8-bit values, we end up getting 2 adjacent accumulators, which is 32 adjacent bits in vR. It's the
    only way to expand values to a higher bit-depth inside the VPU.

    Function also relies on an understanding of the endianness of the system. A 16-bit value 0x1234 is stored as bytes 
    (in order) [0x34, 0x12]. To avoid changing the value represented, we need these two bytes to also be adjacent in 
    the output value. But, because these will end up in separate accumulators (the lower 16-bits of each being in vR 
    with the rest in vD), in order to ensure this, we need one to end up in the upper byte of the 16 bits and the other 
    to end up in the lower byte, which means our only option is to multiply by 2^8.

    Ultimately what we need in the first 4 bytes of vR (given the value above) is [0x00, 0x34, 0x12, 0x00] which when
    written to memory and interpreted as an int32 will be 0x00123400.

    So, the 0x34 just gets MACCed by (0+0+1), leaving it in the lower bits. The 0x12 gets MACCed by (0x7F + 0x7F + 0x02 
    = 0x100), pushing it into the high bits of the half word in vR.

*/

.L_loop_top: //All in 8-bit mode
        {   sub len, len, 1                         ;   vclrdr                                  } 
        {   ldc _16, 16                             ;   vldc b[0]                               }
        {   add b, b, _16                           ;   vlmacc r11[0]  /* .L_vlmacc_consts_A */ }
        {   add r11, r11, _16                       ;   vlmacc r11[0]  /* .L_vlmacc_consts_A */ }
        {   sub r11, r11, _16                       ;   vlmacc r11[0]  /* .L_vlmacc_consts_B */ }
        {   add a, a, _16                           ;   vstr a[0]                               }
        {   add a, a, _16                           ;   bt len, .L_loop_top                     }
.L_loop_bot:

    {   ldc _16, 16                             ;   bf tail, .L_finish                      }
    {                                           ;   vclrdr                                  }
    {                                           ;   vldc b[0]                               }
    {                                           ;   vlmacc r11[0]                           }
    {   add r11, r11, _16                       ;   vlmacc r11[0]                           }
    {                                           ;   vlmacc r11[0]                           }
    vstrpv a[0], tail

.L_finish:
        ldd r4, r5, sp[0]
        retsp NSTACKWORDS       

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