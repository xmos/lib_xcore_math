// Copyright 2020-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#if defined(__VX4B__)

.text

/*
For the long divisions, we only have xm.ldivu instruction, which only opearates on unsigned numbers.
Hence, to handle negative values, we need to negate all the numbers that are negative
and negate the result if needed (hence all the .not_negative jumps).

The result doean't fit into 32 bit word it would return -1.
To avoid that we compare
high part of the numerator with the denominator (hence all the .valid jumps).
If we want a signed result,
we'd have to use xm.lextract as we'are interested in num[63:31] to do a comparison.
 */

/*
int32_t s32_divide_s64_s32(
    const int64_t x,
    const int32_t y);
*/

#define FUNCTION_NAME   s32_divide_s64_s32
#define NSTACKWORDS     (0)

.p2align 2
FUNCTION_NAME:
    bge      x11, x0, .not_negative0
    add      x28, x0, 0
    xm.lsub  x10, x13, x28, x10, x28
    xm.lsub  x11, x13, x28, x11, x13
    bge      x12, x0, .not_negative01
    neg      x12, x12
    li x28, 31
    xm.lextract x13, x11, x10, x28, 32
    blt x13, x12, .valid11
    { xm.mkmsk x10, x28 ; xm.retsp 0 }
.valid11:
    xm.ldivu x10, x11, x11, x10, x12
    xm.retsp 0
.not_negative01:
    li x28, 31
    xm.lextract x13, x11, x10, x28, 32
    blt x13, x12, .valid10
    lui x10, 0x80000
    xm.retsp 0
.valid10:
    xm.ldivu x10, x11, x11, x10, x12
    neg      x10, x10
    xm.retsp 0
.not_negative0:
    li x28, 31
    bge      x12, x0, .not_negative00
    neg      x12, x12
    xm.lextract x13, x11, x10, x28, 32
    blt x13, x12, .valid01
    lui x10, 0x80000
    xm.retsp 0
.valid01:
    xm.ldivu x10, x11, x11, x10, x12
    neg      x10, x10
    xm.retsp 0
.not_negative00:
    // x28 = 31
    xm.lextract x13, x11, x10, x28, 32
    blt x13, x12, .valid00
    { xm.mkmsk x10, x28 ; xm.retsp 0 }
.valid00:
    xm.ldivu x10, x11, x11, x10, x12
    xm.retsp 0


.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
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

.p2align 2
FUNCTION_NAME:
    bge      x11, x0, .not_negative2
    add      x28, x0, 0
    xm.lsub  x10, x13, x28, x10, x28
    xm.lsub  x11, x13, x28, x11, x13
    li x28, 31
    xm.lextract x13, x11, x10, x28, 32
    blt x13, x12, .valid3
    lui x10, 0x80000
    xm.retsp 0
.valid3:
    xm.ldivu x10, x11, x11, x10, x12
    neg      x10, x10
    xm.retsp 0
.not_negative2:
    li x28, 31
    xm.lextract x13, x11, x10, x28, 32
    blt x13, x12, .valid2
    { xm.mkmsk x10, x28 ; xm.retsp 0 }
.valid2:
    xm.ldivu x10, x11, x11, x10, x12
    xm.retsp 0

.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
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

.p2align 2
FUNCTION_NAME:
    li x28, 31
    bge      x12, x0, .not_negative3
    neg      x12, x12
    xm.lextract x13, x11, x10, x28, 32
    blt x13, x12, .valid5
    lui x10, 0x80000
    xm.retsp 0
.valid5:
    xm.ldivu x10, x11, x11, x10, x12
    neg      x10, x10
    xm.retsp 0
.not_negative3:
    // x28 = 31
    xm.lextract x13, x11, x10, x28, 32
    blt x13, x12, .valid4
    { xm.mkmsk x10, x28 ; xm.retsp 0 }
.valid4:
    xm.ldivu x10, x11, x11, x10, x12
    xm.retsp 0

.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
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

.p2align 2
FUNCTION_NAME:
    // will return -1 if saturates
    xm.ldivu x10, x11, x11, x10, x12
    xm.retsp 0

.global FUNCTION_NAME
.type FUNCTION_NAME,@function
.resource_const FUNCTION_NAME, "stack_frame_bytes", (NSTACKWORDS)*4
.resource_list_empty FUNCTION_NAME, "tail_callees"
.resource_list_empty FUNCTION_NAME, "callees"
.resource_list_empty FUNCTION_NAME, "parallel_callees"
.size FUNCTION_NAME,. - FUNCTION_NAME

//======================================================================================


#endif //defined(__VX4B__)
