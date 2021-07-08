// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include "xs3_math_types.h"


/** 
 * @brief Performs a forward real Discrete Fourier Transform on a real 32-bit sequence.
 * 
 * Performs an @math{N}-point forward real DFT on the real 32-bit BFP vector `x`, where @math{N} is `x->length`. The 
 * operation is performed in-place, resulting in an @math{N/2}-element complex 32-bit BFP vector.
 * 
 * The operation performed is:
 * @f[
 *      X[f] = \sum_{n=0}^{N-1} \left( x[n]\cdot e^{-j2\pi fn/N} \right)
 *      \text{ for } 0 \le f \le N/2
 * @f]
 * where @math{x[n]} is the BFP vector initially represented by `x`, and @math{X[f]} is the DFT of @math{x[n]} 
 * represented by the returned pointer.
 * 
 * The exponent, headroom, length and data contents of `x` are all updated by this function, though `x->data` will 
 * continue to point to the same address.
 * 
 * `x->length` must be a power of 2, and must be no larger than `(1<<MAX_DIT_FFT_LOG2)`.
 * 
 * This function returns a `bfp_complex_s32_t` pointer. <b>This points to the same address as `x`.</b> This is intended
 * as a convenience for user code.
 * 
 * Upon completion, the spectrum data is encoded in `x->data` as specified for real DFTs in @ref spectrum_packing. That 
 * is, `x->data[f]` for `1 <= f < (x->length)` represent @math{X[f]} for @math{1 \le f \lt (N/2)} and `x->data[0]`
 * represents @math{X[0] + j X[N/2]}.
 * 
 * @par Example
 * \code
 *      // Initialize time domain data with samples.
 *      int32_t buffer[N] = { ... };
 *      bfp_s32_t samples;
 *      bfp_s32_init(&samples, buffer, 0, N, 1);
 *      // Perform the forward DFT
 *      {
 *          bfp_complex_s32_t* spectrum = bfp_fft_forward_mono(&samples);
 *          // `samples` should no longer be used.
 *          // Operate on frequency domain data using `spectrum`
 *          ...
 *          // Perform the inverse DFT to go back to time domain
 *          bfp_fft_inverse_mono(spectrum); // returns (bfp_s32_t*) which is the address of `samples`
 *      }
 *      // Use `samples` again to use new time domain data. 
 *      ...
 * \endcode
 * 
 * @note A single channel of real data can also be DFTed using bfp_fft_forward_stereo() (by packing the data into one of
 *       the channels of an array of `ch_pair_t`) or bfp_fft_forward_complex() (by converting the real sequence to a 
 *       complex sequence with zero imaginary part). However, bfp_fft_forward_mono() will be the most efficient way to
 *       do this, both in terms of memory and computation time. When DFTing two channels of real data, however, 
 *       bfp_fft_forward_stereo() is more efficient than using bfp_fft_forward_mono() twice.
 * 
 * @param[inout] x  The BFP vector @math{x[n]} to be DFTed.
 * 
 * @return Address of input BFP vector `x`, cast as `bfp_complex_s32_t*`.
 */
C_API
bfp_complex_s32_t* bfp_fft_forward_mono(
    bfp_s32_t* x);



/** 
 * @brief Performs an inverse real Discrete Fourier Transform on a complex 32-bit sequence.
 * 
 * Performs an @math{N}-point inverse real DFT on the real 32-bit BFP vector `x`, where @math{N} is `2*x->length`. The 
 * operation is performed in-place, resulting in an @math{N}-element real 32-bit BFP vector.
 * 
 * The operation performed is:
 * @f[
 *      x[n] = \sum_{f=0}^{N/2} \left( X[f]\cdot e^{j2\pi fn/N} \right)
 *      \text{ for } 0 \le n \lt N
 * @f]
 * where @math{X[f]} is the BFP vector initially represented by `x`, and @math{x[n]} is the IDFT of @math{X[f]} 
 * represented by the returned pointer.
 * 
 * The exponent, headroom, length and data contents of `x` are all updated by this function, though `x->data` will 
 * continue to point to the same address.
 * 
 * `x->length` must be a power of 2, and must be no larger than `(1<<(MAX_DIT_FFT_LOG2-1))`.
 * 
 * This function returns a `bfp_s32_t` pointer. <b>This points to the same address as `x`.</b> This is intended
 * as a convenience for user code.
 * 
 * When calling, the spectrum data must be encoded in `x->data` as specified for real DFTs in @ref spectrum_packing. 
 * That is, `x->data[f]` for `1 <= f < (x->length)` represent @math{X[f]} for @math{1 \le f \lt N/2}, and `x->data[0]`
 * represents @math{X[0] + j X[N/2]}.
 * 
 * @par Example
 * \code
 *      // Initialize time domain data with samples.
 *      int32_t buffer[N] = { ... };
 *      bfp_s32_t samples;
 *      bfp_s32_init(&samples, buffer, 0, N, 1);
 *      // Perform the forward DFT
 *      {
 *          bfp_complex_s32_t* spectrum = bfp_fft_forward_mono(&samples);
 *          // `samples` should no longer be used.
 *          // Operate on frequency domain data using `spectrum`
 *          ...
 *          // Perform the inverse DFT to go back to time domain
 *          bfp_fft_inverse_mono(spectrum); // returns (bfp_s32_t*) which is the address of `samples`
 *      }
 *      // Use `samples` again to use new time domain data. 
 *      ...
 * \endcode
 * 
 * @note An equivalent operation can be performed using bfp_fft_inverse_stereo() (by packing the data into the spectrum
 *       of one of the two channels) or bfp_fft_inverse_complex() (by appropriately re-encoding the spectrum). However, 
 *       bfp_fft_inverse_mono() will be the most efficient way to accomplish this, both in terms of memory and 
 *       computation time. When IDFTing the spectra of two channels of real data, however, bfp_fft_inverse_stereo() is 
 *       more efficient than using bfp_fft_inverse_mono() twice.
 * 
 * @param[inout] x  The BFP vector @math{X[f]} to be IDFTed.
 * 
 * @return Address of input BFP vector `x`, cast as `bfp_s32_t*`.
 */
C_API
bfp_s32_t* bfp_fft_inverse_mono(
    bfp_complex_s32_t* x);



/** 
 * @brief Performs a forward complex Discrete Fourier Transform on a complex 32-bit sequence.
 * 
 * Performs an @math{N}-point forward complex DFT on the complex 32-bit BFP vector `x`, where @math{N} is `x->length`. 
 * The operation is performed in-place.
 * 
 * The operation performed is:
 * @f[
 *      X[f] = \sum_{n=0}^{N-1} \left( x[n]\cdot e^{-j2\pi fn/N} \right)
 *      \text{ for } 0 \le f \lt N
 * @f]
 * where @math{x[n]} is the BFP vector initially represented by `x`, and @math{X[f]} is the DFT of @math{x[n]}, also
 * represented by `x` upon completion.
 * 
 * The exponent, headroom and data contents of `x` are updated by this function. `x->data` will continue to point to the
 * same address. 
 * 
 * `x->length` (@math{N}) must be a power of 2, and must be no larger than `(1<<MAX_DIT_FFT_LOG2)`.
 * 
 * Upon completion, the spectrum data is encoded in `x` as specified in @ref spectrum_packing. That is, `x->data[f]`
 * for `0 <= f < (x->length)` represent @math{X[f]} for @math{0 \le f \lt N}.
 * 
 * @par Example
 * \code
 *      // Initialize complex time domain data with samples.
 *      complex_s32_t buffer[N] = { ... };
 *      bfp_complex_s32_t vector;
 *      bfp_complex_s32_init(&vector, buffer, 0, N, 1);
 *      // Perform the forward DFT
 *      bfp_fft_forward_mono(&vector);
 *      // Operate on frequency domain data
 *      ...
 *      // Perform the inverse DFT to go back to time domain
 *      bfp_fft_inverse_mono(&vector);
 *      // `vector` contains (complex) time-domain data again
 *      ...
 * \endcode
 * 
 * @param[inout] x  The BFP vector @math{x[n]} to be DFTed.
 */
C_API
void bfp_fft_forward_complex(
    bfp_complex_s32_t* x);

/** 
 * @brief Performs an inverse complex Discrete Fourier Transform on a complex 32-bit sequence.
 * 
 * Performs an @math{N}-point inverse complex DFT on the complex 32-bit BFP vector `x`, where @math{N} is `x->length`. 
 * The operation is performed in-place.
 * 
 * The operation performed is:
 * @f[
 *      x[n] = \sum_{f=0}^{N-1} \left( X[f]\cdot e^{j2\pi fn/N} \right)
 *      \text{ for } 0 \le f \lt N
 * @f]
 * where @math{X[f]} is the BFP vector initially represented by `x`, and @math{x[n]} is the DFT of @math{X[f]}, also
 * represented by `x` upon completion.
 * 
 * The exponent, headroom and data contents of `x` are updated by this function. `x->data` will continue to point to the
 * same address.
 * 
 * `x->length` must be a power of 2, and must be no larger than `(1<<MAX_DIT_FFT_LOG2)`.
 * 
 * The data initially encoded in `x` are interpreted as specified in @ref spectrum_packing. That is, `x->data[f]` for 
 * `0 <= f < (x->length)` represent @math{X[f]} for @math{0 \le f \lt N}.
 * 
 * @par Example
 * \code
 *      // Initialize complex time domain data with samples.
 *      complex_s32_t buffer[N] = { ... };
 *      bfp_complex_s32_t vector;
 *      bfp_complex_s32_init(&vector, buffer, 0, N, 1);
 *      // Perform the forward DFT
 *      bfp_fft_forward_mono(&vector);
 *      // Operate on frequency domain data
 *      ...
 *      // Perform the inverse DFT to go back to time domain
 *      bfp_fft_inverse_mono(&vector);
 *      // `vector` contains (complex) time-domain data again
 *      ...
 * \endcode
 * 
 * @param[inout] x  The BFP vector @math{x[n]} to be IDFTed.
 */
C_API
void bfp_fft_inverse_complex(
    bfp_complex_s32_t* x);

/** 
 * @brief Performs a forward real Discrete Fourier Transform on a pair of real 32-bit sequences.
 * 
 * Performs an @math{N}-point forward real DFT on the real 32-bit BFP vector `x` of sample pairs, where @math{N} is 
 * `x->length`. The operation is performed in-place, resulting in a pair of @math{N/2}-element complex 32-bit BFP 
 * vectors.
 * 
 * The operation performed is:
 * @f[
 *      A[f] = \sum_{n=0}^{N-1} \left( a[n]\cdot e^{-j2\pi fn/N} \right) \text{ for } 0 \le f \le N/2 \\
 *      B[f] = \sum_{n=0}^{N-1} \left( b[n]\cdot e^{-j2\pi fn/N} \right) \text{ for } 0 \le f \le N/2
 * @f]
 * where @math{a[n]} and @math{b[n]} are the two time-domain sequences represented by BFP vector `x`, and @math{A[f]} 
 * and @math{B[f]} are the DFT of @math{a[n]} and @math{b[n]} respectively represented by complex BFP vectors `a` and 
 * `b`.
 * 
 * `x->length` (@math{N}) must be a power of 2, and must be no larger than `(1<<MAX_DIT_FFT_LOG2)`.
 * 
 * The output BFP vectors `a` and `b` need not have been initialized prior to calling this function. Their contents
 * (including the addresses to which `a->data` and `b->data` point) will be overwritten.
 * 
 * The contents of `x` are modified (and should be considered corrupted) by this function. However, the contents of 
 * `a->data` and `b->data` together occupy the region of memory originally pointed to by `x->data`.
 * 
 * The spectrum data is encoded in `a->data` and `b->data` as specified for real DFTs in @ref spectrum_packing. That 
 * is, `a->data[f]` for `1 <= f < (a->length)` represent @math{A[f]} for @math{1 \le f \lt (N/2)} and `a->data[0]`
 * represents @math{A[0] + j A[N/2]}. Likewise for the encoding of `b->data`.
 * 
 * @par Example
 * \code
 *      // Initialize time domain data with samples.
 *      ch_pair_s32_t buffer[N] = { {...}, ... };
 *      bfp_ch_pair_s32_t stereo_samples;
 *      bfp_ch_pair_s32_init(&stereo_samples, buffer, 0, N, 1);
 *      {
 *          bfp_complex_s32_t chanA, chanB;
 *          // Perform the forward DFT
 *          bfp_fft_forward_stereo(&chanA, &chanB, &stereo_samples);
 *          // `stereo_samples` should not be used in here.
 *          // Operate on frequency domain data using `chanA` and `chanB`
 *          ...
 *          // Perform the inverse DFT to go back to time domain
 *          bfp_fft_inverse_stereo(&stereo_samples, &chanA, &chanB);
 *      }
 *      // Use `stereo_samples` again to use new time domain data. 
 *      ...
 * \endcode
 * 
 * @note When performing a DFT on a pair of channels, bfp_fft_forward_stereo() is more efficient than using 
 *       bfp_fft_forward_mono() twice.
 * 
 * @warning If you intend to apply an IFFT to BFP vectors `a` and `b` you must keep track of both
 *          vectors (and keep track of which is which), because bfp_fft_inverse_stereo() requires
 *          that the mantissa buffer of vector `b` directly follows that of `a`.
 *  
 * @param[out]  a   Output spectrum for channel A.
 * @param[out]  b   Output spectrum for channel B.
 * @param[in]   x   channel-pair BFP vector to be DFTed.
 */
C_API
void bfp_fft_forward_stereo(
    bfp_complex_s32_t* a,
    bfp_complex_s32_t* b,
    bfp_ch_pair_s32_t* x);

/** 
 * @brief Performs an inverse real Discrete Fourier Transform on a pair of complex 32-bit sequences.
 * 
 * Performs a pair of @math{N}-point inverse real DFTs on the complex 32-bit BFP vectors `a` and `b`, where @math{N} is 
 * `2*a->length` (with `a->length == b->length`). The operation is performed in-place, resulting in a pair of real 
 * sequences packed into channels A and B of an @math{N}-element 32-bit BFP channel-pair vector.
 * 
 * The operation performed is:
 * @f[
 *      a[n] = \sum_{f=0}^{N/2-1} \left( A[f]\cdot e^{j2\pi fn/N} \right) \text{ for } 0 \le n \lt N \\
 *      b[n] = \sum_{f=0}^{N/2-1} \left( B[f]\cdot e^{j2\pi fn/N} \right) \text{ for } 0 \le n \lt N
 * @f]
 * where @math{A[f]} and @math{B[f]} are the frequency spectra represented by BFP vectors `a` and `b`, and @math{a[n]} 
 * and @math{b[n]} are the IDFT of @math{A[f]} and @math{B[f]} respectively represented by BFP channel-pair vector `x`.
 * 
 * `a->length` (@math{N}) must be a power of 2, and must be no larger than `(1<<(MAX_DIT_FFT_LOG2-1))`.
 * 
 * The output BFP vector `x` need not have been initialized prior to calling this function. Its contents (including the 
 * address to which `x->data` points) will be overwritten.
 * 
 * The spectrum data encoded in `a->data` and `b->data` are interpreted as specified for real DFTs in @ref 
 * spectrum_packing. That is, `a->data[f]` for `1 <= f < (a->length)` represent @math{A[f]} for @math{1 \le f \lt (N/2)} 
 * and `a->data[0]` represents @math{A[0] + j A[N/2]}. Likewise for the encoding of `b->data`.
 * 
 * @par Example
 * \code
 *      // Initialize time domain data with samples.
 *      ch_pair_s32_t buffer[N] = { {...}, ... };
 *      bfp_ch_pair_s32_t stereo_samples;
 *      bfp_ch_pair_s32_init(&stereo_samples, buffer, 0, N, 1);
 *      {
 *          bfp_complex_s32_t chanA, chanB;
 *          // Perform the forward DFT
 *          bfp_fft_forward_stereo(&chanA, &chanB, &stereo_samples);
 *          // `stereo_samples` should not be used in here.
 *          // Operate on frequency domain data using `chanA` and `chanB`
 *          ...
 *          // Perform the inverse DFT to go back to time domain
 *          bfp_fft_inverse_stereo(&stereo_samples, &chanA, &chanB);
 *      }
 *      // Use `stereo_samples` again to use new time domain data. 
 *      ...
 * \endcode
 * 
 * @note When calling this function the following conditions must hold:
 *          `b->length == a->length` and `b->data == &a->data[a->length]`
 *       This allows the operation to be performed efficiently in-place.
 * @par
 * @note When performing an IDFT on a pair of channels, bfp_fft_inverse_stereo() is more efficient than using 
 *       bfp_fft_inverse_mono() twice.
 *  
 * @param[out]  x   Output BFP channel-pair vector.
 * @param[out]  a   Spectrum for channel A.
 * @param[in]   b   Spectrum for channel B.
 */
C_API
void  bfp_fft_inverse_stereo(
    bfp_ch_pair_s32_t* x,
    const bfp_complex_s32_t* a,
    const bfp_complex_s32_t* b);

/**
 * @brief Unpack the spectrum resulting from bfp_fft_forward_mono().
 * 
 * The DFT of a real signal is periodic with period FFT_N (the FFT length) and has a complex
 * conjugate symmetry about index 0. These two properties guarantee that the imaginary part of
 * both the DC component (index 0) and the Nyquist component (index FFT_N/2) of the spectrum
 * are zero. To compute the forward FFT in-place, bfp_fft_forward_mono() packs the real part 
 * of the Nyquist rate component of the output spectrum into the imaginary part of the DC
 * component.
 * 
 * This may be undesirable when operating on the signal's complex spectrum. Use this function
 * to unpack the Nyquist component. This function will also adjust the BFP vector's length to
 * reflect this unpacking.
 * 
 * NOTE: If you intend to unpack the spectrum using this function, the buffer for the time-domain
 *       BFP vector must have length `FFT_N+2`, rather than `FFT_N` (`int32_t` elements), but
 *       these should NOT be reflected in the time-domain BFP vector's `length` field.
 * 
 * @f$
 *    Re\{x_{N/2}\} \leftarrow  Im\{x_0\}     \\
 *    Im\{x_0\} \leftarrow 0                  \\
 *    Im\{x_{N/2}\} \leftarrow 0              \\
 *    x.length \leftarrow x.length + 1
 * @f$
 * 
 * NOTE: Before bfp_fft_inverse_mono() may be applied, bfp_fft_pack_mono() must be called, as the
 *       inverse FFT expects the data to be packed.
 * 
 * @param[inout]  x  The spectrum to be unpacked
 * 
 * @see bfp_fft_forward_mono
 * @see bfp_fft_pack_mono
 */
C_API
void bfp_fft_unpack_mono(
    bfp_complex_s32_t* x);


/**
 * @brief Unpack the spectrum resulting from bfp_fft_forward_stereo().
 * 
 * The DFT of a real signal is periodic with period FFT_N (the FFT length) and has a complex
 * conjugate symmetry about index 0. These two properties guarantee that the imaginary part of
 * both the DC component (index 0) and the Nyquist component (index FFT_N/2) of the spectrum
 * are zero. To compute the forward FFT in-place, bfp_fft_forward_stereo() packs the real part 
 * of the Nyquist rate component of the output spectra into the imaginary part of the DC
 * component.
 * 
 * This may be undesirable when operating on the signal's complex spectrum. Use this function
 * to unpack the Nyquist component. This function will also adjust the BFP vectors' length to
 * reflect this unpacking.
 * 
 * NOTE: If you intend to unpack the spectrum using this function, the buffer for the time-domain
 *       BFP vector must have length `FFT_N+2`, rather than `FFT_N` (`ch_pair_s32_t` elements), but
 *       these should NOT be reflected in the time-domain BFP vector's `length` field.
 * 
 * @f$
 *    Re\{x_{N/2}\} \leftarrow  Im\{x_0\}     \\
 *    Im\{x_0\} \leftarrow 0                  \\
 *    Im\{x_{N/2}\} \leftarrow 0              \\
 *    x.length \leftarrow x.length + 1
 * @f$
 * 
 * NOTE: Before bfp_fft_inverse_stereo() may be applied, bfp_fft_pack_stereo() must be called, 
 *       as the inverse FFT expects the data to be packed.
 * 
 * @param[inout]  x  The spectrum to be unpacked
 * 
 * @see bfp_fft_forward_stereo
 * @see bfp_fft_pack_stereo
 */
C_API
void bfp_fft_unpack_stereo(
    bfp_complex_s32_t* x1,
    bfp_complex_s32_t* x2);


/**
 * @brief Pack the spectrum resulting from bfp_fft_unpack_mono().
 * 
 * This function applies the reverse process of bfp_fft_unpack_mono(), to prepare it for an
 * inverse FFT using bfp_fft_inverse_mono().
 * 
 * @param[inout]  x   The spectrum to be packed
 * 
 * @see bfp_fft_inverse_mono
 * @see bfp_fft_unpack_mono
 */
C_API
void bfp_fft_pack_mono(
    bfp_complex_s32_t* x);


/**
 * @brief Pack the spectrum resulting from bfp_fft_unpack_stereo().
 * 
 * This function applies the reverse process of bfp_fft_unpack_stereo(), to prepare it for an
 * inverse FFT using bfp_fft_inverse_stereo().
 * 
 * @param[inout]  x1   The first channel of the spectrum to be packed
 * @param[inout]  x2   The second channel of the spectrum to be packed
 * 
 * @see bfp_fft_inverse_stereo
 * @see bfp_fft_unpack_stereo
 */
C_API
void bfp_fft_pack_stereo(
    bfp_complex_s32_t* x1,
    bfp_complex_s32_t* x2);