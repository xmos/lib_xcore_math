// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)


/*  
headroom_t vect_s16_to_vect_s32(
    int16_t a[],
    const int32_t b[],
    const unsigned length);
*/

#include "../asm_helper.h"

.text
.p2align 2

#define NSTACKWORDS     (4)

#define FUNCTION_NAME   vect_s16_to_vect_s32

#define a               x10
#define b               x11
#define len             x12
#define _16             x13
#define tail            x18
#define constsA         x28
#define constsB         x28


FUNCTION_NAME:
        xm.entsp (NSTACKWORDS)*4/* XAT Warning: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in dualentsp NSTACKWORDS\nMessage: 0th operand fits in 6 bit unsigned immediate" */
        li t3, 0x0200
    {   xm.ldap t3, .L_vlmacc_consts_A            ;   xm.vsetc t3}

        xm.stdsp  s3,s2,0
    {   slli tail, len, SIZEOF_LOG2_S32          ; nop                                           }/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shli tail, len, SIZEOF_LOG2_S32          \nMessage: The shift amount is not 32" */
    {   srli len, len, EPV_LOG2_S32              ;   xm.zexti tail, 5                            }/* XAT Warning: "Falling back on assumption: the int != 32 for the integer value of the item at position 2 in the instruction's operands in    shri len, len, EPV_LOG2_S32              \nMessage: The shift amount is not 32" */
    {   xm.mkmsk tail, tail                        ;   xm.brff len, .L_loop_bot                     }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
    { nop                                           ;   xm.bu .L_loop_top                          }

.p2align 4

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
        {   addi len, len, -1                         ;   xm.vclrdr                                  } 
        {   li _16, 16                             ;   xm.vldc b}
        {   add b, b, _16                           ;   xm.vlmacc0 t3}
        xm.vlmacc1 t3
        {   add t3, t3, _16                       ;   xm.vlmacc0 t3}
        xm.vlmacc1 t3
        {   sub t3, t3, _16                       ;   xm.vlmacc0 t3}
        xm.vlmacc1 t3
        {   add a, a, _16                           ;   xm.vstr a}
        {   add a, a, _16                           ;   xm.bt len, .L_loop_top                     }
.L_loop_bot:

    {   li _16, 16                             ;   xm.brff tail, .L_finish                      }/* XAT Warning: 'Instruction xm.brff can only branch forwards; this branch may need revising' */
    { nop                                           ;   xm.vclrdr                                  }
    { nop                                           ;   xm.vldc b}
    { nop                                           ;   xm.vlmacc0 t3}
    xm.vlmacc1 t3
    {   add t3, t3, _16                       ;   xm.vlmacc0 t3}
    xm.vlmacc1 t3
    { nop                                           ;   xm.vlmacc0 t3}
    xm.vlmacc1 t3
    xm.vstrpv a, tail

.L_finish:
        xm.lddsp  s3,s2,0
        xm.retsp (NSTACKWORDS)*4       /* Multiple XAT warnings: "Falling back on assumption: the int < 253 for the integer value of the item at position 0 in the instruction's operands in retsp NSTACKWORDS       \nMessage: 0th operand fits in 6 bit unsigned immediate", 'RETSP operand may need scaling' */

.L_func_end:


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.set FUNCTION_NAME.nstackwords,NSTACKWORDS; .global FUNCTION_NAME.nstackwords /* Translation error on this line: unexpected token at position 42. */ 
.set FUNCTION_NAME.maxcores,1;              .global FUNCTION_NAME.maxcores /* Translation error on this line: unexpected token at position 29. */ 
.set FUNCTION_NAME.maxtimers,0;             .global FUNCTION_NAME.maxtimers /* Translation error on this line: unexpected token at position 30. */ 
.set FUNCTION_NAME.maxchanends,0;           .global FUNCTION_NAME.maxchanends /* Translation error on this line: unexpected token at position 32. */ 
.size FUNCTION_NAME, .L_func_end - FUNCTION_NAME


#endif //defined(__VX4B__)
