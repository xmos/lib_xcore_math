
#ifndef XS3_BFP_CH_PAIR_LOW_H_
#define XS3_BFP_CH_PAIR_LOW_H_

#include "xs3_math_types.h"
#include "xs3_bfp_low.h"

// XC uses 'safe' pointers by default. For the most part the 'extern "C"` block will make
// XC files being compiled treat the pointers as unsafe, but that doesn't appear to work
// for the casts in static inline functions. This fixes that behavior when compiling an
// XC file.
#ifdef __XC__
  #define UNSAFE    unsafe
#else
  #define UNSAFE    
#endif


#ifdef __XC__
extern "C" {
#endif



void xs3_vect_ch_pair_s16_set(
    ch_pair_s16_t data[],
    const int16_t ch_a,
    const int16_t ch_b,
    const unsigned length);



void xs3_vect_ch_pair_s32_set(
    ch_pair_s32_t data[],
    const int32_t ch_a,
    const int32_t ch_b,
    const unsigned length);


#ifdef __XC__

  // For some reason I can't get the static inline functions to compile when included
  // from a .xc file. There's probably some fix I don't know about. This is temporary.
  // @todo Make these work from XC.

#else    

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

#endif //XS3_BFP_CH_PAIR_LOW_H_