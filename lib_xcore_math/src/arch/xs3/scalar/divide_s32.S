// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)

.text
.issue_mode dual

/*
For the long divisions, we only have ldivu instruction, which only opearates on unsigned numbers.
Hence, to handle negative values, we need to negate all the numbers that are negative
and negate the result if needed (hence all the .not_negative jumps).

To avoid arithmetic exception when the result won't fit into 32 bit word, we compare
high part of the numerator with the denominator (hence all the .valid jumps).
If we want a signed result,
we'd have to use lextract as we'are interested in num[63:31] to do a comparison.
 */

/*
int32_t s32_divide_s64_s32(
    const int64_t x,
    const int32_t y);
*/

#define FUNCTION_NAME   s32_divide_s64_s32
#define NSTACKWORDS     (0)

.cc_top FUNCTION_NAME.function,FUNCTION_NAME
.align 4
FUNCTION_NAME:
    dualentsp 0
    ashr     r11, r1, 32
    {   ldc      r11, 0     ;   bf       r11, .not_negative0}
    lsub     r3, r0, r11, r0, r11
    lsub     r3, r1, r11, r1, r3
    ashr     r11, r2, 32
    {ldc         r11, 31    ;   bf       r11, .not_negative01}
    neg      r2, r2
    lextract r3, r1, r0, r11, 32
    lss r3, r3, r2
    bt r3, .valid11
    {   mkmsk    r0, r11    ;   retsp    0          }
.valid11:
    ldivu    r0, r1, r1, r0, r2
    retsp    0
.not_negative01:
    // r11 = 31
    lextract r3, r1, r0, r11, 32
    lss r3, r3, r2
    {   ldc      r3, 1      ;   bt r3, .valid10     }
    {   shl      r0, r3, r11;   retsp    0          }
.valid10:
    ldivu    r0, r1, r1, r0, r2
    {   neg      r0, r0     ;   retsp    0          }
.not_negative0:
    ashr     r11, r2, 32
    {   ldc      r11, 31    ;   bf       r11, .not_negative00}
    neg      r2, r2
    lextract r3, r1, r0, r11, 32
    lss r3, r3, r2
    {   ldc      r3, 1      ;   bt r3, .valid01     }
    {   shl      r0, r3, r11;   retsp    0          }
.valid01:
    ldivu    r0, r1, r1, r0, r2
    {   neg      r0, r0     ;   retsp    0          }
.not_negative00:
    // r11 = 31
    lextract r3, r1, r0, r11, 32
    lss r3, r3, r2
    bt r3, .valid00
    {   mkmsk    r0, r11    ;   retsp    0          }
.valid00:
    ldivu    r0, r1, r1, r0, r2
    retsp    0
.cc_bottom FUNCTION_NAME.function


.globl FUNCTION_NAME
.type  FUNCTION_NAME,@function
.set   FUNCTION_NAME.nstackwords, 0
.globl FUNCTION_NAME.nstackwords
.set   FUNCTION_NAME.maxchanends, 0
.globl FUNCTION_NAME.maxchanends
.set   FUNCTION_NAME.maxtimers, 0
.globl FUNCTION_NAME.maxtimers
.set   FUNCTION_NAME.maxcores, 0
.globl FUNCTION_NAME.maxcores
.size FUNCTION_NAME,. - FUNCTION_NAME

//======================================================================================
/*
int32_t s32_divide_s64_u32(
    const int64_t x,
    const uint32_t y);

    @todo: this should be a static inline
*/
#undef FUNCTION_NAME
#define FUNCTION_NAME   s32_divide_s64_u32
#define NSTACKWORDS     (0)

.cc_top FUNCTION_NAME.function,FUNCTION_NAME
.align 4
FUNCTION_NAME:
    dualentsp 0
    ashr     r11, r1, 32
    {   ldc      r11, 0     ;   bf       r11, .not_negative2}
    lsub     r3, r0, r11, r0, r11
    lsub     r3, r1, r11, r1, r3
    ldc r11, 31
    lextract r3, r1, r0, r11, 32
    lss r3, r3, r2
    {   ldc      r3, 1      ;   bt r3, .valid3      }
    {   shl      r0, r3, r11;   retsp    0          }
.valid3:
    ldivu    r0, r1, r1, r0, r2
    {   neg      r0, r0     ;   retsp    0          }
.not_negative2:
    ldc r11, 31
    lextract r3, r1, r0, r11, 32
    lss r3, r3, r2
    bt r3, .valid2
    {   mkmsk    r0, r11    ;   retsp    0          }
.valid2:
    ldivu    r0, r1, r1, r0, r2
    retsp    0
.cc_bottom FUNCTION_NAME.function

.globl FUNCTION_NAME
.type  FUNCTION_NAME,@function
.set   FUNCTION_NAME.nstackwords, 0
.globl FUNCTION_NAME.nstackwords
.set   FUNCTION_NAME.maxchanends, 0
.globl FUNCTION_NAME.maxchanends
.set   FUNCTION_NAME.maxtimers, 0
.globl FUNCTION_NAME.maxtimers
.set   FUNCTION_NAME.maxcores, 0
.globl FUNCTION_NAME.maxcores
.size FUNCTION_NAME,. - FUNCTION_NAME

//======================================================================================
/*
int32_t s32_divide_u64_s32(
    const uint64_t x,
    const int32_t y);
*/

#undef FUNCTION_NAME
#define FUNCTION_NAME   s32_divide_u64_s32
#define NSTACKWORDS     (0)

.cc_top FUNCTION_NAME.function,FUNCTION_NAME
.align 4
FUNCTION_NAME:
    dualentsp    0
    ashr     r11, r2, 32
    {   ldc      r11, 31    ;   bf       r11, .not_negative3}
    neg      r2, r2
    lextract r3, r1, r0, r11, 32
    lss r3, r3, r2
    {   ldc      r3, 1      ;   bt       r3, .valid5}
    {   shl      r0, r3, r11;   retsp    0          }
.valid5:
    ldivu    r0, r1, r1, r0, r2
    {   neg      r0, r0     ;   retsp    0          }
.not_negative3:
    // r11 = 31
    lextract r3, r1, r0, r11, 32
    lss r3, r3, r2
    bt r3, .valid4
    {   mkmsk    r0, r11    ;   retsp    0          }
.valid4:
    ldivu    r0, r1, r1, r0, r2
    retsp    0
.cc_bottom FUNCTION_NAME.function

.globl FUNCTION_NAME
.type  FUNCTION_NAME,@function
.set   FUNCTION_NAME.nstackwords, 0
.globl FUNCTION_NAME.nstackwords
.set   FUNCTION_NAME.maxchanends, 0
.globl FUNCTION_NAME.maxchanends
.set   FUNCTION_NAME.maxtimers, 0
.globl FUNCTION_NAME.maxtimers
.set   FUNCTION_NAME.maxcores, 0
.globl FUNCTION_NAME.maxcores
.size FUNCTION_NAME,. - FUNCTION_NAME

//======================================================================================
/*
int32_t u32_divide_u64_u32(
    const uint64_t x,
    const uint32_t y);

    @todo: this should be a static inline
*/

#undef FUNCTION_NAME
#define FUNCTION_NAME   u32_divide_u64_u32
#define NSTACKWORDS     (0)

.cc_top FUNCTION_NAME.function,FUNCTION_NAME
.align 4
FUNCTION_NAME:
    {   lss      r11, r1, r2;   dualentsp 0         }
    bt       r11, .valid6
    {   mkmsk    r0, 32     ;   retsp    0          }
.valid6:
    ldivu    r0, r1, r1, r0, r2
    retsp    0
.cc_bottom FUNCTION_NAME.function

.globl FUNCTION_NAME
.type  FUNCTION_NAME,@function
.set   FUNCTION_NAME.nstackwords, 0
.globl FUNCTION_NAME.nstackwords
.set   FUNCTION_NAME.maxchanends, 0
.globl FUNCTION_NAME.maxchanends
.set   FUNCTION_NAME.maxtimers, 0
.globl FUNCTION_NAME.maxtimers
.set   FUNCTION_NAME.maxcores, 0
.globl FUNCTION_NAME.maxcores
.size FUNCTION_NAME,. - FUNCTION_NAME

//======================================================================================


#endif //defined(__XS3A__)
