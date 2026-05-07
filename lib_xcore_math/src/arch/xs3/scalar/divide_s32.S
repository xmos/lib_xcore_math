// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__XS3A__)

.text
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
    entsp 0
    ashr     r11, r1, 32
    bf       r11, .not_negative0
    ldc      r11, 0
    lsub     r3, r0, r11, r0, r11
    lsub     r3, r1, r11, r1, r3
    ashr     r11, r2, 32
    bf       r11, .not_negative01
    neg      r2, r2
    ldivu    r0, r1, r1, r0, r2
    retsp    0
.not_negative01:
    ldivu    r0, r1, r1, r0, r2
    neg      r0, r0
    retsp 0
.not_negative0:
    ashr     r11, r2, 32
    bf       r11, .not_negative00
    neg      r2, r2
    ldivu    r0, r1, r1, r0, r2
    neg      r0, r0
    retsp    0
.not_negative00:
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
    entsp 0
    ashr     r11, r1, 32
    bf       r11, .not_negative2
    ldc      r11, 0
    lsub     r3, r0, r11, r0, r11
    lsub     r3, r1, r11, r1, r3
    ldivu    r0, r1, r1, r0, r2
    neg      r0, r0
    retsp    0
.not_negative2:
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
    entsp    0
    ashr     r11, r2, 32
    bf       r11, .not_negative3
    neg      r2, r2
    ldivu    r0, r1, r1, r0, r2
    neg      r0, r0
    retsp    0
.not_negative3:
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
    entsp    0
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
