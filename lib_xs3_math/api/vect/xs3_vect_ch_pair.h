
#ifndef XS3_VECT_CH_PAIR_H_
#define XS3_VECT_CH_PAIR_H_

#include "xs3_math_types.h"
#include "xs3_vect.h"
#include "xs3_vect_complex.h"



#ifdef __XC__
extern "C" {
#endif




#ifdef __XC__

  // For some reason I can't get the static inline functions to compile when included
  // from a .xc file. There's probably some fix I don't know about. This is temporary.
  // @todo Make these work from XC.

#else    

static inline void xs3_vect_ch_pair_s16_set(
    ch_pair_s16_t data[],
    const int16_t ch_a,
    const int16_t ch_b,
    const unsigned length)
{
    union {
        int32_t s32;
        ch_pair_s16_t cp16;
    } tmp;

    tmp.cp16.ch_a = ch_a;
    tmp.cp16.ch_b = ch_b;
    
    xs3_vect_s32_set((int32_t*) data, tmp.s32, length);
}

static inline void xs3_vect_ch_pair_s32_set(
    ch_pair_s32_t data[],
    const int32_t ch_a,
    const int32_t ch_b,
    const unsigned length)
{
    xs3_vect_complex_s32_set((complex_s32_t*) data, ch_a, ch_b, length);
}

static inline headroom_t xs3_vect_ch_pair_s16_headroom(
    const ch_pair_s16_t a[],
    const unsigned length)
{
    return xs3_vect_s16_headroom((int16_t *) a, 2*length);
}


static inline headroom_t xs3_vect_ch_pair_s32_headroom(
    const ch_pair_s32_t a[],
    const unsigned length)
{
    return xs3_vect_s32_headroom((int32_t*) a, 2*length);
}


static inline headroom_t xs3_vect_ch_pair_s16_shr(
    ch_pair_s16_t a[],
    const ch_pair_s16_t b[],
    const unsigned length,
    const right_shift_t shr)
{
    return xs3_vect_s16_shr((int16_t*) a, (int16_t*) b, 2*length, shr);
}

static inline headroom_t xs3_vect_ch_pair_s32_shr(
    ch_pair_s32_t a[],
    const ch_pair_s32_t b[],
    const unsigned length,
    const right_shift_t shr)
{
    return xs3_vect_s32_shr((int32_t*) a, (int32_t*) b, 2*length, shr);
}


static inline headroom_t xs3_vect_ch_pair_s16_shl(
    ch_pair_s16_t a[],
    const ch_pair_s16_t b[],
    const unsigned length,
    const left_shift_t shl)
{
    return xs3_vect_ch_pair_s16_shr(a, b, length, -shl);
}

static inline headroom_t xs3_vect_ch_pair_s32_shl(
    ch_pair_s32_t a[],
    const ch_pair_s32_t b[],
    const unsigned length,
    const left_shift_t shl)
{
    return xs3_vect_ch_pair_s32_shr(a, b, length, -shl);
}

#endif //__XC__




#ifdef __XC__
}   //extern "C"
#endif

#endif //XS3_VECT_CH_PAIR_H_