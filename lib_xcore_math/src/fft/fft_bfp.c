// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "xmath/xmath.h"
#include "xmath_fft_lut.h"


bfp_complex_s32_t* bfp_fft_forward_mono(
    bfp_s32_t* x)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS)
    // Length must be 2^p where p is a non-negative integer
    assert(x->length != 0);
    // for a positive power of 2, subtracting 1 should increase its headroom.
    assert(cls(x->length - 1) > cls(x->length));
#endif

    // The returned BFP vector is just a recasting of the input vector
    bfp_complex_s32_t* X = (bfp_complex_s32_t*) x;

    const unsigned FFT_N = x->length;

    // fft_dit_forward() requires (at least) two bits of headroom in the
    // mantissa vector
    right_shift_t x_shr = 2 - x->hr;
    vect_s32_shl(x->data, x->data, x->length, -x_shr);

    // Correct the BFP vector's parameters
    x->hr  = x->hr  + x_shr;
    x->exp = x->exp + x_shr;

    // NOTE: A real, mono FFT of length FFT_N is implemented using an FFT with
    //       length FFT_N/2 because it is more efficient in compute and memory.
    x->length = FFT_N/2;

    // The low-level FFT functions require the elements of the input vector to be jumbled
    // such that the new index of each element is a bit-reversal (ignoring endianness!) of
    // the index of its original position, considering only the least significant
    // log2(FFT_N) bits of the original index.
    // For example,
    //    (int32_t) 0x22 (34) in binary: 00000000 00000000 00000000 00100010
    //  For 256-point FFT,  0x22 =  0b00100010 -->  0b01000100 = 0x44
    //  For 512-point FFT,  0x22 = 0b000100010 --> 0b010001000 = 0x88
    fft_index_bit_reversal(X->data, X->length);

    // Do the actual FFT
    fft_dit_forward(X->data, X->length, &X->hr, &X->exp);

    // Apply the adjustment required for a mono, real FFT (because we implemented it
    // using a half-length FFT)
    fft_mono_adjust(X->data, FFT_N, 0);

    X->hr = vect_complex_s32_headroom(&X->data[0], X->length);
    return X;
}


bfp_s32_t* bfp_fft_inverse_mono(
    bfp_complex_s32_t* X)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS)
    // Length must be 2^p where p is a non-negative integer
    assert(X->length != 0);
    // for a positive power of 2, subtracting 1 should increase its headroom.
    assert(cls(X->length - 1) > cls(X->length));
#endif

    // Because the real, mono FFT only includes half a period of the spectrum,
    // the FFT length is twice the vector length
    const unsigned FFT_N = 2*X->length;

    // The returned BFP vector is just a recasting of the input vector
    bfp_s32_t* x = (bfp_s32_t*)X;
    
    // fft_dit_inverse() requires (at least) two bits of headroom in the
    // mantissa vector
    right_shift_t X_shr = 2 - X->hr;
    vect_s32_shl((int32_t*) X->data, (int32_t*) X->data, FFT_N, -X_shr);
    
    // Correct the BFP vector's parameters based on the shift done
    X->hr  = X->hr  + X_shr;
    X->exp = X->exp + X_shr;

    // NOTE: A real, mono inverse FFT of length FFT_N is implemented using an FFT with
    //       length FFT_N/2 because it is more efficient in compute and memory
    X->length = FFT_N;

    // Apply the adjustment required for a mono, real inverse FFT (because it is implemented
    // using a half-length FFT)
    fft_mono_adjust(X->data, FFT_N, 1);

    // Boggle the elements of the input spectrum as required (bit-reversed indexing) by
    // fft_dit_inverse(). (See comment above in bfp_fft_forward_mono())
    fft_index_bit_reversal(X->data, FFT_N/2);

    // Do the actual IFFT
    fft_dit_inverse(X->data, FFT_N/2, &x->hr, &x->exp);

    return x;
}


void bfp_fft_forward_complex(
    bfp_complex_s32_t* samples)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS)
    // Length must be 2^p where p is a non-negative integer
    assert(samples->length != 0);
    // for a positive power of 2, subtracting 1 should increase its headroom.
    assert(cls(samples->length - 1) > cls(samples->length));
#endif

    //The FFT implementation requires 2 bits of headroom to ensure no saturation occurs
    if(samples->hr < 2){
        left_shift_t shl = samples->hr - 2;
        samples->hr = vect_s32_shl((int32_t*) samples->data,(int32_t*)  samples->data,
                                       2*samples->length, shl);
        samples->exp -= shl;
    }

    // Boggle the elements of the input spectrum as required (bit-reversed indexing) by
    // fft_dit_forward(). (See comment above in bfp_fft_forward_mono())
    fft_index_bit_reversal(samples->data, samples->length);

    // Do the actual FFT
    fft_dit_forward(samples->data, samples->length, &samples->hr, &samples->exp);
}


void bfp_fft_inverse_complex(
    bfp_complex_s32_t* spectrum)
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS)
    // Length must be 2^p where p is a non-negative integer
    assert(spectrum->length != 0);
    // for a positive power of 2, subtracting 1 should increase its headroom.
    assert(cls(spectrum->length - 1) > cls(spectrum->length));
#endif

    //The FFT implementation requires 2 bits of headroom to ensure no saturation occurs
    if(spectrum->hr < 2){
        left_shift_t shl = spectrum->hr - 2;
        spectrum->hr = vect_s32_shl((int32_t*) spectrum->data,(int32_t*)  spectrum->data, 2*spectrum->length, shl);
        spectrum->exp -= shl;
    }

    // Boggle the elements of the input spectrum as required (bit-reversed indexing) by
    // fft_dit_forward(). (See comment above in bfp_fft_forward_mono())
    fft_index_bit_reversal(spectrum->data, spectrum->length);
    
    // Do the actual IFFT
    fft_dit_inverse(spectrum->data, spectrum->length, &spectrum->hr, &spectrum->exp);
}


void bfp_fft_forward_stereo(
    bfp_s32_t* a,
    bfp_s32_t* b,
    complex_s32_t scratch[])
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS)
    // Length must be 2^p where p is a non-negative integer
    assert(a->length != 0);
    // Length of a and b must be the same
    assert(a->length == b->length);
    // for a positive power of 2, subtracting 1 should increase its headroom.
    assert(cls(a->length - 1) > cls(a->length));
#endif

    const unsigned FFT_N = a->length;

    //The FFT implementation requires 2 bits of headroom to ensure no saturation occurs
    right_shift_t a_shr = 2 - a->hr;
    right_shift_t b_shr = 2 - b->hr;

    a->hr += a_shr;
    a->exp += a_shr;

    b->hr += b_shr;
    b->exp += b_shr;

    // The stereo FFT requires the input samples from a and b to be interleaved into a single
    // buffer. We'll zip them into scratch and apply the shifts.
    vect_s32_zip(scratch, a->data, b->data, FFT_N, a_shr, b_shr);

    // Boggle the elements of the input spectrum as required (bit-reversed indexing) by
    // fft_dit_forward(). (See comment above in bfp_fft_forward_mono())
    fft_index_bit_reversal(scratch, FFT_N);

    // The change in each signal's exponent from FFTing
    exponent_t exp_diff = 0;

    // The headroom of the FFT result
    headroom_t hr = a->hr;

    // Do the actual FFT
    fft_dit_forward(scratch, FFT_N, &hr, &exp_diff);

    // Aliased input BFP vectors
    bfp_complex_s32_t* a_fft = (bfp_complex_s32_t*) a;
    bfp_complex_s32_t* b_fft = (bfp_complex_s32_t*) b;

    // The stereo FFT assumes the input channels are purely real, and implements the DFT by packing
    // the Channel B's time-domain signal into the imaginary part of a complex input vector. The
    // resulting complex spectrum contains a half-period of each of the two channels' spectra, but
    // in a jumbled up form. This function un-jumbles them.
    hr = fft_spectra_split(scratch, FFT_N);

    // Copy the data from the scratch buffer back to the input buffers
    vect_s32_copy((int32_t*) a_fft->data, (int32_t*) &scratch[0], FFT_N);
    vect_s32_copy((int32_t*) b_fft->data, (int32_t*) &scratch[FFT_N/2], FFT_N);

    //a and b might actually have different headroom, but the fft_spectra_split() only
    // computes the headroom of the entire FFT_N-element complex spectrum, which is the same
    // as the minimum of the headroom of the two spectra. If a user needs a more accurate
    // count of each spectrum's headroom, bfp_complex_s32_headroom() should be called on
    // each output BFP vector.
    a_fft->hr = b_fft->hr = hr;

    // Correct the exponents and lengths (as only a half-period of their spectra were computed)
    a_fft->length = b_fft->length = FFT_N / 2;
    a_fft->exp += exp_diff;
    b_fft->exp += exp_diff;

}


void  bfp_fft_inverse_stereo(
    bfp_complex_s32_t* a_fft,
    bfp_complex_s32_t* b_fft,
    complex_s32_t scratch[])
{
#if (XMATH_BFP_DEBUG_CHECK_LENGTHS)
    // The two input vectors must be the same length
    assert(a_fft->length == b_fft->length);
    // Length must be 2^p where p is a non-negative integer
    assert(a_fft->length != 0);
    // for a positive power of 2, subtracting 1 should increase its headroom.
    assert(cls(a_fft->length - 1) > cls(a_fft->length));
#endif

    // a and b store only a half-period of their respective spectra, so the FFT length is twice
    // the length of a or b (which must have the same length)
    const unsigned FFT_N = 2*a_fft->length;

    // 2 bits of headroom are required by fft_dit_inverse(),
    // which means we need 3 bits here, because the headroom can be
    // reduced when merging spectra
    right_shift_t a_shr = 3 - a_fft->hr;
    right_shift_t b_shr = 3 - b_fft->hr;
    
    a_fft->exp += a_shr;
    b_fft->exp += b_shr;

    a_fft->hr += a_shr;
    b_fft->hr += b_shr;

    // Both input vectors need to be copied to the scratch buffer.
    vect_complex_s32_shr(&scratch[0], a_fft->data, FFT_N/2, a_shr);
    vect_complex_s32_shr(&scratch[FFT_N/2], b_fft->data, FFT_N/2, b_shr);

    headroom_t hr = 2;
    exponent_t exp_diff = 0;

    // Because the real, stereo IFFT is implemented using a complex FFT, the two channels'
    // spectra have to be jumbled together in a particular way prior to applying the IFFT
    fft_spectra_merge(scratch, FFT_N);

    // Undo the bit-reversed indexing
    fft_index_bit_reversal(scratch, FFT_N);
    
    // Do the actual IFFT
    fft_dit_inverse(scratch, FFT_N, &hr, &exp_diff);

    // Aliases for time-domain vectors
    bfp_s32_t* a = (bfp_s32_t*) a_fft;
    bfp_s32_t* b = (bfp_s32_t*) b_fft;

    // Copy data back from scratch buffer
    vect_s32_unzip(a->data, b->data, scratch, FFT_N);

    // Update vector metadata
    a->hr = b->hr = hr;
    a->length = b->length = FFT_N;

    a->exp += exp_diff;
    b->exp += exp_diff;
}


void bfp_fft_unpack_mono(
  bfp_complex_s32_t* x)
{
  // Move Nyquist component's real part to the correct index
  x->data[x->length].re = x->data[0].im;
  // Zero out the imaginary part of the DC and Nyquist components
  x->data[0].im = 0;
  x->data[x->length].im = 0;
  // Update length of spectrum vector
  x->length++;
}

void bfp_fft_pack_mono(
  bfp_complex_s32_t* x)
{
  // Update length of spectrum vector
  x->length--;
  // Move Nyquist component's real part to DC imaginary part
  x->data[0].im = x->data[x->length].re;
}
