// Copyright 2021-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include "xmath/xmath.h"

#include <assert.h>
#include <stdio.h>


// Mono gradient constraint
void bfp_complex_s32_gradient_constraint_mono(
    bfp_complex_s32_t* X,
    const unsigned frame_advance)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS)
    // Length must be 2^p where p is a non-negative integer
    assert(X->length != 0);
    // for a positive power of 2, subtracting 1 should increase its headroom.
    assert(cls(X->length - 1) > cls(X->length)); 
#endif

  const unsigned FFT_N = 2 * X->length;
  const unsigned FFT_N_LOG2 = u32_ceil_log2(FFT_N);
  const unsigned FREQ_BINS = FFT_N/2;
  bfp_s32_t* x = (bfp_s32_t*)X;

  // IFFT requires 2 bits of headroom. Change exponent so we have exactly 2.
  bfp_complex_s32_use_exponent(X, X->exp - X->hr + 2 );

  // mono IFFT requires a spectrum adjustment
  fft_mono_adjust(X->data, FFT_N, 1);

  // The DIT I/FFT requires indices to be bit-reverse prior to the tranform, whereas DIF I/FFT
  // requires it after. If we use the inverse DIF and forward DIT, then both bit-reversals will
  // happen in the time domain.  This allows us to forego the index bit-reversals altogether.
  fft_dif_inverse(X->data, FREQ_BINS, &x->hr, &x->exp);

  // The time-domain indices to be zeroed out are 'frame_advance' and everything that follows.
  // Because we still have the indices out of order, we need to figure out the correct indices
  // as we go.
  // The other tricky part here is, because of the way the mono FFT uses a half-length FFT_N, where
  // odd-indexed time-domain samples end up being considered an imaginary part of a complex signal,
  // the index bit-reversal works on _pairs_ of time-domain indices. e.g.  time-domain samples
  // td[2*k] and td[2*k+1] instead of ending up at td[bitrev(2*k)] and td[bitrev(2*k+1)], will
  // end up at  ((complex_s32_t*)td)[bitrev(k)].re and ((complex_s32_t*)td)[bitrev(k)].im.

  // So really, we just need to think of td[] as a complex vector of length FFT_N/2, and we need
  // to zero out elements   td[(frame_advance/2):]  = 0

  // This takes care of  td[(FFT_N/4):(FFT_N/2)] = 0
  for(unsigned i = 1; i < FREQ_BINS; i += 2)
    X->data[i].re = X->data[i].im = 0;

  // This takes care of  td[(frame_advance/2):(FFT_N/4)] = 0
  for(unsigned i = frame_advance/2; i < FREQ_BINS/2; i++){
    const unsigned ri = n_bitrev(i, FFT_N_LOG2-1);
    X->data[ri].re = X->data[ri].im = 0;
  }

  // Now reverse the above stuff.
  bfp_complex_s32_use_exponent(X, X->exp - X->hr + 2 );
  fft_dit_forward(X->data, FREQ_BINS, &X->hr, &X->exp);
  fft_mono_adjust(X->data, FFT_N, 0);
  bfp_complex_s32_headroom(X);
}



// Stereo gradient constraint
void bfp_complex_s32_gradient_constraint_stereo(
    bfp_complex_s32_t* X1,
    bfp_complex_s32_t* X2,
    const unsigned frame_advance)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS)
    assert(X1->length != 0 && X2->length != 0);
    assert(cls(X1->length - 1) > cls(X1->length)); 
    assert(cls(X2->length - 1) > cls(X2->length)); 
#endif

  // Determine whether we can do it faster than 2 mono GCs.

  // To do it quickly requires that X1 and X2 are the same length, and that X2's buffer begins
  // immediately after X1's
  int fast_check = (X1->length == X2->length);
  fast_check = fast_check && (&X2->data[0] == &X1->data[X1->length]);

  if(!fast_check) {
    // Can't do it faster. Just do 2 monos.
    bfp_complex_s32_gradient_constraint_mono(X1, frame_advance);
    bfp_complex_s32_gradient_constraint_mono(X2, frame_advance);
    return;
  }

  // Can do it faster by doing a stereo FFT.

  const unsigned FFT_N = 2 * X1->length;
  const unsigned FFT_N_LOG2 = u32_ceil_log2(FFT_N);

  // IFFT requires 2 bits of headroom. Change exponent so we have exactly 2.
  bfp_complex_s32_use_exponent(X1, X1->exp - X1->hr + 2 );
  bfp_complex_s32_use_exponent(X2, X2->exp - X2->hr + 2 );

  // stereo IFFT requires that the spectra be merged
  fft_spectra_merge(X1->data, FFT_N);

  // The DIT I/FFT requires indices to be bit-reverse prior to the tranform, whereas DIF I/FFT
  // requires it after. If we use the inverse DIF and forward DIT, then both bit-reversals will
  // happen in the time domain.  This allows us to forego the index bit-reversals altogether.
  exponent_t exp = 0;
  headroom_t hr  = 2;
  fft_dif_inverse(X1->data, FFT_N, &hr, &exp);


  // Now we need to basically do:    X[frame_advance:] = 0  
  // on X1 and X2, except that's assuming the elements are correctly ordered. But they're still
  // in bit-reversed index order.
  // Note: frame_advance <= FFT_N/2   (always)
  // Which means we always want to zero out  X[FFT_N/2:]. Because FFT_N is guaranteed to be
  // a power of two, that means that indices (FFT_N/2):FFT_N are exactly those that have the
  // most significant bit of their (FFT_N_LOG2-bit) indices set to 1.  With bit reversed indexing,
  // these end up being all of the ODD bit-reversed indices
  for(unsigned i = 1; i < FFT_N; i+=2){
    X1->data[i].re = X1->data[i].im = 0;
  }

  // Now what's left is:   X[frame_advance:(FFT_N/2)] = 0.  If the number of elements here also
  // is a power of 2, then they can quickly be iterated over like the above.  Let's say the number
  // of elements left is K (which is a power of 2).  Then note that the most significant 
  // log2(FFT_N) - log2(K) bits of these indices are constant, while the last log2(K) vary over
  // all K possible log2(K) bit patterns.
  // Then, when bit-reversed, that means the MOST significant log2(K) bits vary over all patterns,
  // while the LEAST significant  log2(FFT_N) - log2(K) bits are constant.
  // So, the lowest index to be zeroed will just be   bit_rev(frame_advance, log2(FFT_N))
  // (because  frame_advance has the last log2(K) bits as zeros before bit-reversal), and the
  // next index to be zeroed will be  bit_rev(frame_advance + K/2). And at that point we can just
  // proceed linearly through the array.
  /*
  const unsigned start = bitrev(frame_advance, FFT_N_LOG2);
  const unsigned step = bitrev(frame_advance + ((FFT_N/2)-frame_advance)/2, FFT_N_LOG2) - start;

  for(unsigned i = start; i < FFT_N; i += step){
    X1->data[i].re = 0;
    X1->data[i].im = 0;
  }
  */
  // But I'm not sure whether we'll need to deal with cases where (FFT_N/2)-frame_advance is not
  // a power of 2, so I'll just do it the straight-forward way.
  for(unsigned i = frame_advance; i < FFT_N/2; i++){
    const unsigned ri = n_bitrev(i, FFT_N_LOG2);
    X1->data[ri].re = X1->data[ri].im = 0;
  }

  // Now reverse the above stuff.
  hr = vect_complex_s32_headroom(X1->data, FFT_N);
  if(hr != 2) {
    right_shift_t delta_p = 2 - hr;
    hr = vect_complex_s32_shr(X1->data, X1->data, FFT_N, delta_p);
    exp += delta_p;
  }

  fft_dit_forward(X1->data, FFT_N, &hr, &exp);
  hr = fft_spectra_split(X1->data, FFT_N);

  // printf("\n\n\n exp = %d\n\n\n", exp);

  X1->exp += exp;
  X2->exp += exp;
  
  bfp_complex_s32_headroom(X1);
  bfp_complex_s32_headroom(X2);
}