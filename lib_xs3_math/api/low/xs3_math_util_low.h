

#ifndef XS3_MATH_UTIL_LOW_H_
#define XS3_MATH_UTIL_LOW_H_

#include "xs3_math_types.h"

#include <assert.h>

#define ASSERT(COND)    assert(COND)

#ifdef __XC__
extern "C" {
#endif

static inline headroom_t CLS(
    const int32_t a)
{
#ifdef __XS3A__
    unsigned res;
    asm( "cls %0, %1" : "=r"(res) : "r"(a) );
    return res;
#else
    ASSERT(0);  //TODO
#endif //__XS3A__
}


static inline headroom_t CLS_S16(
    const int16_t a)
{
    return CLS(a) - 16;
}

static inline headroom_t CLS_C16(
    const complex_s16_t a)
{
    const headroom_t re_hr = CLS(a.re) - 16;
    const headroom_t im_hr = CLS(a.im) - 16;

    return (re_hr <= im_hr)? re_hr : im_hr;
}


static inline headroom_t CLS_S32(
    const int32_t a)
{
    return CLS(a);
}


static inline headroom_t CLS_C32(
    const complex_s32_t a)
{
    const headroom_t re_hr = CLS(a.re);
    const headroom_t im_hr = CLS(a.im);

    return (re_hr <= im_hr)? re_hr : im_hr;
}


static inline void unpack_float_s16(int16_t* mant, exponent_t* exp, const float x)
{
#ifdef __XS3A__
    ASSERT(0);  //TODO
#else
    ASSERT(0);  //TODO
#endif //__XS3A__
}


static inline void unpack_float_s32(int32_t* mant, exponent_t* exp, const float x)
{
#ifdef __XS3A__
    ASSERT(0);  //TODO
#else
    ASSERT(0);  //TODO
#endif //__XS3A__
}

static inline float pack_float_s16(int16_t mant, exponent_t exp)
{
#ifdef __XS3A__
    ASSERT(0);  //TODO
#else
    ASSERT(0);  //TODO
#endif //__XS3A__
}


static inline float pack_float_s32(int32_t mant, exponent_t exp)
{
#ifdef __XS3A__
    ASSERT(0);  //TODO
#else
    ASSERT(0);  //TODO
#endif //__XS3A__
}


static inline int ashr(int x, int shr)
{
#ifdef __XS3A__
    ASSERT(0);  //TODO
#else
    ASSERT(0);  //TODO
#endif //__XS3A__
}

#ifdef __XC__
} // extern "C" 
#endif


#endif //XS3_MATH_UTIL_LOW_H_