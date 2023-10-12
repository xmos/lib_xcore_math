// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include "xmath/_support/fft_impl.h"

/**
 * @defgroup fft_api  XMath FFT API
 */

#ifdef __XC__
extern "C" { 
#endif



/** 
 * @brief Performs a forward real Discrete Fourier Transform on a real 32-bit sequence.
 * 
 * Performs an @math{N}-point forward real DFT on the real 32-bit BFP vector `x`, where @math{N} is
 * `x->length`. The operation is performed in-place, resulting in an @math{N/2}-element complex
 * 32-bit BFP vector.
 * 
 * @operation{
 * &    X[f] = \sum_{n=0}^{N-1} \left( x[n]\cdot e^{-j2\pi fn/N} \right)  \\
 * &         \text{ for } 0 \le f \le N/2
 * }
 * where @math{x[n]} is the BFP vector initially represented by `x`, and @math{X[f]} is the DFT of
 * @math{x[n]} represented by the returned pointer.
 * 
 * The exponent, headroom, length and data contents of `x` are all updated by this function, though
 * `x->data` will continue to point to the same address.
 *
 * `x->length` must be a power of 2, and must be no larger than `(1<<MAX_DIT_FFT_LOG2)`.
 *
 * This function returns a `bfp_complex_s32_t` pointer. <b>This points to the same address as
 * `x`.</b> This is intended as a convenience for user code.
 *
 * Upon completion, the spectrum data is encoded in `x->data` as specified for real DFTs in @ref
 * spectrum_packing. That is, `x->data[f]` for `1 <= f < (x->length)` represent @math{X[f]} for
 * @math{1 \le f < (N/2)} and `x->data[0]` represents @math{X[0] + j X[N/2]}.
 * 
 * @par Example
 * @code{.c}
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
 * @endcode
 * 
 * @param[inout] x  The BFP vector @math{x[n]} to be DFTed.
 * 
 * @return Address of input BFP vector `x`, cast as `bfp_complex_s32_t*`.
 * 
 * @ingroup fft_api
 */
C_API
bfp_complex_s32_t* bfp_fft_forward_mono(
    bfp_s32_t* x);



/** 
 * @brief Performs an inverse real Discrete Fourier Transform on a complex 32-bit sequence.
 * 
 * Performs an @math{N}-point inverse real DFT on the real 32-bit BFP vector `x`, where @math{N} is
 * `2*x->length`. The operation is performed in-place, resulting in an @math{N}-element real 32-bit
 * BFP vector.
 * 
 * @operation{
 * &    x[n] = \sum_{f=0}^{N/2} \left( X[f]\cdot e^{j2\pi fn/N} \right) \\
 * &         \text{ for } 0 \le n < N
 * }
 * where @math{X[f]} is the BFP vector initially represented by `x`, and @math{x[n]} is the IDFT of
 * @math{X[f]} represented by the returned pointer.
 *
 * The exponent, headroom, length and data contents of `x` are all updated by this function, though
 * `x->data` will continue to point to the same address.
 *
 * `x->length` must be a power of 2, and must be no larger than `(1<<(MAX_DIT_FFT_LOG2-1))`.
 *
 * This function returns a `bfp_s32_t` pointer. <b>This points to the same address as `x`.</b> This
 * is intended as a convenience for user code.
 *
 * When calling, the spectrum data must be encoded in `x->data` as specified for real DFTs in @ref
 * spectrum_packing. That is, `x->data[f]` for `1 <= f < (x->length)` represent @math{X[f]} for
 * @math{1 \le f < N/2}, and `x->data[0]` represents @math{X[0] + j X[N/2]}.
 * 
 * @par Example
 * @code
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
 * @endcode
 * 
 * @param[inout] x  The BFP vector @math{X[f]} to be IDFTed.
 * 
 * @return Address of input BFP vector `x`, cast as `bfp_s32_t*`.
 * 
 * @ingroup fft_api
 */
C_API
bfp_s32_t* bfp_fft_inverse_mono(
    bfp_complex_s32_t* x);



/** 
 * @brief Performs a forward complex Discrete Fourier Transform on a complex 32-bit sequence.
 * 
 * Performs an @math{N}-point forward complex DFT on the complex 32-bit BFP vector `x`, where
 * @math{N} is `x->length`. The operation is performed in-place.
 * 
 * @operation{
 * &    X[f] = \sum_{n=0}^{N-1} \left( x[n]\cdot e^{-j2\pi fn/N} \right)  \\
 * &         \text{ for } 0 \le f < N
 * }
 * where @math{x[n]} is the BFP vector initially represented by `x`, and @math{X[f]} is the DFT of
 * @math{x[n]}, also represented by `x` upon completion.
 *
 * The exponent, headroom and data contents of `x` are updated by this function. `x->data` will
 * continue to point to the same address. 
 *
 * `x->length` (@math{N}) must be a power of 2, and must be no larger than `(1<<MAX_DIT_FFT_LOG2)`.
 *
 * Upon completion, the spectrum data is encoded in `x` as specified in @ref spectrum_packing. That
 * is, `x->data[f]` for `0 <= f < (x->length)` represent @math{X[f]} for @math{0 \le f < N}.
 * 
 * @par Example
 * @code
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
 * @endcode
 * 
 * @param[inout] x  The BFP vector @math{x[n]} to be DFTed.
 * 
 * @ingroup fft_api
 */
C_API
void bfp_fft_forward_complex(
    bfp_complex_s32_t* x);

/** 
 * @brief Performs an inverse complex Discrete Fourier Transform on a complex 32-bit sequence.
 * 
 * Performs an @math{N}-point inverse complex DFT on the complex 32-bit BFP vector `x`, where
 * @math{N} is `x->length`. The operation is performed in-place.
 * 
 * @operation{
 * &    x[n] = \sum_{f=0}^{N-1} \left( X[f]\cdot e^{j2\pi fn/N} \right) \\
 * &         \text{ for } 0 \le f < N
 * }
 * where @math{X[f]} is the BFP vector initially represented by `x`, and @math{x[n]} is the DFT of
 * @math{X[f]}, also represented by `x` upon completion.
 *
 * The exponent, headroom and data contents of `x` are updated by this function. `x->data` will
 * continue to point to the same address.
 * 
 * `x->length` must be a power of 2, and must be no larger than `(1<<MAX_DIT_FFT_LOG2)`.
 * 
 * The data initially encoded in `x` are interpreted as specified in @ref spectrum_packing. That is,
 * `x->data[f]` for `0 <= f < (x->length)` represent @math{X[f]} for @math{0 \le f < N}.
 * 
 * @par Example
 * @code
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
 * @endcode
 * 
 * @param[inout] x  The BFP vector @math{x[n]} to be IDFTed.
 * 
 * @ingroup fft_api
 */
C_API
void bfp_fft_inverse_complex(
    bfp_complex_s32_t* x);

/** 
 * @brief Performs a forward real Discrete Fourier Transform on a pair of real 32-bit sequences.
 * 
 * @note 
 * @parblock
 * Use of this function is not currently recommended. It functions correctly, but a recent
 * change in this library's API (namely, dropping support for channel-pair vectors) means this
 * function is no more computationally efficient than calling `bfp_fft_forward_mono()` on each input
 * vector separately. Additionally, this function currently requires a scratch buffer, whereas the
 * mono FFT does not. 
 * @endparblock
 * 
 * Performs an @math{N}-point forward real DFT on the real 32-bit BFP vectors @vector{a} and
 * @vector{b}, where @math{N} is `a->length` (which must equal `b->length`). The resulting spectra,
 * @vector{A} and @vector{B}, are placed in `a` and `b`. Each spectrum is a @math{N/2}-element
 * complex 32-bit BFP vectors. To access the spectrum, the pointers `a` and `b` should be cast to
 * `bfp_complex_s32_t*` following a call to this function.
 * 
 * @operation{
 * &    A[f] = \sum_{n=0}^{N-1} \left( a[n]\cdot e^{-j2\pi fn/N} \right) \text{ for } 0 \le f \le N/2 \\
 * &    B[f] = \sum_{n=0}^{N-1} \left( b[n]\cdot e^{-j2\pi fn/N} \right) \text{ for } 0 \le f \le N/2
 * }
 * where @math{a[n]} and @math{b[n]} are the two time-domain sequences represented by input BFP
 * vectors `a` and `b`, and @math{A[f]} and @math{B[f]} are the DFT of @math{a[n]} and @math{b[n]}
 * respectively.
 * 
 * `a->length` (@math{N}) must be equal to `b->length, must be a power of 2, and must be no larger
 * than `(1<<MAX_DIT_FFT_LOG2)`.
 *
 * _The parameters `a` and `b` are used as both inputs and outputs_. To access the result of the
 * FFT, `a` and `b` should be cast to `bfp_complex_s32_t`*. The structs' metadata (e.g. `exp`, `hr`,
 * `length`) are updated by this function to reflect this change of interpretation. The `bfp_s32_t`
 * references should be considered corrupted after this call (at least until
 * bfp_fft_inverse_stereo() is called).
 *
 * The spectrum data is encoded in `a->data` and `b->data` as specified for real DFTs in @ref
 * spectrum_packing. That is, `a->data[f]` for `1 <= f < (a->length)` represent @math{A[f]} for
 * @math{1 \le f < (N/2)} and `a->data[0]` represents @math{A[0] + j A[N/2]}. Likewise for the
 * encoding of `b->data`.
 * 
 * This function requires a scratch buffer large enough to contain @math{N} `complex_s32_t`
 * elements.
 * 
 * @par Example
 * @code
 *      // Initialize time domain data with samples.
 *      int32_t bufferA[N] = { ... };
 *      int32_t bufferB[N] = { ... };
 *      complex_s32_t scratch[N]; // scratch buffer -- contents don't matter
 *      bfp_s32_t channel_A, channel_B;
 *      bfp_s32_init(&channel_A, buffer, 0, N, 1);
 *      bfp_s32_init(&channel_B, buffer, 0, N, 1);
 * 
 *      // Perform the forward DFT
 *      bfp_fft_forward_stereo(&channel_A, &channel_B, scratch);
 *      
 *      // channel_A and channel_B should now be considered clobbered as the structs are now 
 *      // effectively bfp_complex_s32_t
 *      bfp_complex_s32_t* chanA = (bfp_complex_s32_t*) &channel_A;
 *      bfp_complex_s32_t* chanB = (bfp_complex_s32_t*) &channel_B;
 * 
 *      // Operate on frequency domain data using `chanA` and `chanB`
 *      ...
 *      // Perform the inverse DFT to go back to time domain
 *      bfp_fft_inverse_stereo(&chanA, &chanB, scratch);
 *      
 *      // Use channel_A and channel_B again to use new time domain data. 
 *      ...
 * @endcode
 *  
 * @param[inout]  a         [Input] Time-domain BFP vector @vector{a}. [Output] Frequency domain
 *                          BFP vector @vector{A}
 * @param[inout]  b         [Input] Time-domain BFP vector @vector{b}. [Output] Frequency domain
 *                          BFP vector @vector{B}
 * @param         scratch   Scratch buffer of at least `a->length` `complex_s32_t` elements
 * 
 * @deprecated
 * 
 * // Suppress this from generated documentation for the time being
 * // @ingroup fft_api
 */
C_API
void bfp_fft_forward_stereo(
    bfp_s32_t* a,
    bfp_s32_t* b,
    complex_s32_t scratch[]);


/** 
 * @brief Performs an inverse real Discrete Fourier Transform on a pair of complex 32-bit sequences.
 * 
 * @note 
 * @parblock
 * Use of this function is not currently recommended. It functions correctly, but a recent
 * change in this library's API (namely, dropping support for channel-pair vectors) means this
 * function is no more computationally efficient than calling `bfp_fft_forward_mono()` on each input
 * vector separately. Additionally, this function currently requires a scratch buffer, whereas the
 * mono FFT does not. 
 * @endparblock
 * 
 * Performs an @math{N}-point inverse real DFT on the 32-bit complex BFP vectors @vector{A} and
 * @vector{B} (`A_fft` and `B_fft` respectively), where @math{N} is `A_fft->length` . The resulting
 * real signals, @vector{a} and @vector{b}, are placed in `A_fft` and `B_fft`. Each time-domain
 * result is a @math{N/2}-element real 32-bit BFP vectors. To access the spectrum, the pointers
 * `A_fft` and `B_fft` should be cast to `bfp_s32_t*` following a call to this function.
 * 
 * @operation{
 * &    a[n] = \sum_{f=0}^{N/2-1} \left( A[f]\cdot e^{j2\pi fn/N} \right) \text{ for } 0 \le n < N \\
 * &    b[n] = \sum_{f=0}^{N/2-1} \left( B[f]\cdot e^{j2\pi fn/N} \right) \text{ for } 0 \le n < N
 * }
 * where @math{A[f]} and @math{B[f]} are the frequency spectra represented by BFP vectors `A_fft`
 * and `B_fft`, and @math{a[n]} and @math{b[n]} are the IDFT of @math{A[f]} and @math{B[f]}.
 *
 * `A_fft->length` (@math{N}) must be a power of 2, and must be no larger than
 * `(1<<(MAX_DIT_FFT_LOG2-1))`.
 * 
 * _The parameters `A_fft` and `B_fft` are used as both inputs and outputs_. To access the result of
 * the IFFT, `A_fft` and `B_fft` should be cast to `bfp_s32_t`*. The structs' metadata (e.g. `exp`,
 * `hr`, `length`) are updated by this function to reflect this change of interpretation. The
 * `bfp_complex_s32_t` references should be considered corrupted after this call.
 * 
 * The spectrum data encoded in `A_fft->data` and `A_fft->data` are interpreted as specified for
 * real DFTs in @ref spectrum_packing. That is, `A_fft->data[f]` for `1 <= f < (a->length)`
 * represent @math{A[f]} for @math{1 \le f < (N/2)} and `A_fft->data[0]` represents @math{A[0] + j
 * A[N/2]}. Likewise for the encoding of `B_fft->data`.
 * 
 * This function requires a scratch buffer large enough to contain @math{2N} `complex_s32_t`
 * elements.
 * 
 * @par Example
 * @code
 *      // Initialize time domain data with samples.
 *      int32_t bufferA[N] = { ... };
 *      int32_t bufferB[N] = { ... };
 *      complex_s32_t scratch[N]; // scratch buffer -- contents don't matter
 *      bfp_s32_t channel_A, channel_B;
 *      bfp_s32_init(&channel_A, buffer, 0, N, 1);
 *      bfp_s32_init(&channel_B, buffer, 0, N, 1);
 * 
 *      // Perform the forward DFT
 *      bfp_fft_forward_stereo(&channel_A, &channel_B, scratch);
 *      
 *      // channel_A and channel_B should now be considered clobbered as the structs are now 
 *      // effectively bfp_complex_s32_t
 *      bfp_complex_s32_t* chanA = (bfp_complex_s32_t*) &channel_A;
 *      bfp_complex_s32_t* chanB = (bfp_complex_s32_t*) &channel_B;
 * 
 *      // Operate on frequency domain data using `chanA` and `chanB`
 *      ...
 *      // Perform the inverse DFT to go back to time domain
 *      bfp_fft_inverse_stereo(&chanA, &chanB, scratch);
 *      
 *      // Use channel_A and channel_B again to use new time domain data. 
 *      ...
 * @endcode
 *  
 * @param[inout]  A_fft     [Input] Freq-domain BFP vector @vector{A}. 
 *                          [Output] Time domain BFP vector @vector{b}
 * @param[inout]  B_fft     [Input] Freq-domain BFP vector @vector{b}. 
 *                          [Output] Time domain BFP vector @vector{b}
 * @param         scratch   Scratch buffer of at least `2*A_fft->length` `complex_s32_t` elements
 * 
 * @deprecated
 * 
 * // Suppress this from generated documentation for the time being
 * // @ingroup fft_api
 */
C_API
void  bfp_fft_inverse_stereo(
    bfp_complex_s32_t* A_fft,
    bfp_complex_s32_t* B_fft,
    complex_s32_t scratch[]);

/**
 * @brief Unpack the spectrum resulting from bfp_fft_forward_mono().
 * 
 * The DFT of a real signal is periodic with period FFT_N (the FFT length) and has a complex
 * conjugate symmetry about index 0. These two properties guarantee that the imaginary part of both
 * the DC component (index 0) and the Nyquist component (index FFT_N/2) of the spectrum are zero. To
 * compute the forward FFT in-place, bfp_fft_forward_mono() packs the real part of the Nyquist rate
 * component of the output spectrum into the imaginary part of the DC component.
 *
 * This may be undesirable when operating on the signal's complex spectrum. Use this function to
 * unpack the Nyquist component. This function will also adjust the BFP vector's length to reflect
 * this unpacking.
 * 
 * NOTE: If you intend to unpack the spectrum using this function, the buffer for the time-domain
 *       BFP vector must have length `FFT_N+2`, rather than `FFT_N` (`int32_t` elements), but
 *       these should NOT be reflected in the time-domain BFP vector's `length` field.
 * 
 * @operation{
 * &       Re\{x_{N/2}\}  && \leftarrow  Im\{x_0\}     \\
 * &       Im\{x_0\}      && \leftarrow 0              \\
 * &       Im\{x_{N/2}\}  && \leftarrow 0              \\
 * &       x.length       && \leftarrow x.length + 1
 * }
 * 
 * NOTE: Before bfp_fft_inverse_mono() may be applied, bfp_fft_pack_mono() must be called, as the
 *       inverse FFT expects the data to be packed.
 * 
 * @param[inout]  x  The spectrum to be unpacked
 * 
 * @see bfp_fft_forward_mono,
 *      bfp_fft_pack_mono
 * 
 * @ingroup fft_api
 */
C_API
void bfp_fft_unpack_mono(
    bfp_complex_s32_t* x);

/**
 * @brief Pack the spectrum resulting from bfp_fft_unpack_mono().
 * 
 * This function applies the reverse process of bfp_fft_unpack_mono(), to prepare it for an inverse
 * FFT using bfp_fft_inverse_mono().
 * 
 * @param[inout]  x   The spectrum to be packed
 * 
 * @see bfp_fft_inverse_mono,
 *      bfp_fft_unpack_mono
 * 
 * @ingroup fft_api
 */
C_API
void bfp_fft_pack_mono(
    bfp_complex_s32_t* x);



/**
 * @brief Compute a forward DFT using the decimation-in-time FFT algorithm.
 * 
 * This function computes the `N`-point forward DFT of a complex input signal using the
 * decimation-in-time FFT algorithm. The result is computed in-place. 
 * 
 * @operation{
 * &    X[f] = \frac{1}{2^{\alpha}} \sum_{n=0}^{N-1} \left( x[n]\cdot e^{-j2\pi fn/N} \right) \\
 * &         \text{ for } 0 \le f < N
 * }
 * 
 * `x[]` is interpreted to be a block floating-point vector with shared exponent `*exp` and with
 * `*hr` bits of headroom initially in `x[]`. During computation, this function monitors the
 * headroom of the data and compensates to avoid overflows and underflows by bit-shifting the data
 * up or down as appropriate. In the equation above, @math{\alpha}    
 * represents the (net) number of bits that the data was right-shifted by.
 * 
 * Upon completion, `*hr` is updated with the final headroom in `x[]`, and the exponent `*exp` is
 * incremented by @math{\alpha}.
 * 
 * @note In order to guarantee that saturation will not occur, `x[]` must have an _initial_ headroom
 *       of at least 2 bits.
 * 
 * @param[inout]  x     The `N`-element complex input vector to be transformed.
 * @param[in]     N     The size of the DFT to be performed.
 * @param[inout]  hr    Pointer to the initial headroom in `x[]`.
 * @param[inout]  exp   Pointer to the initial exponent associated with `x[]`.
 * 
 * @exception ET_LOAD_STORE Raised if `x` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup fft_api
 */
C_API
void fft_dit_forward (
    complex_s32_t x[], 
    const unsigned N, 
    headroom_t* hr, 
    exponent_t* exp);

/**
 * @brief Compute an inverse DFT using the decimation-in-time IFFT algorithm.
 * 
 * This function computes the `N`-point inverse DFT of a complex spectrum using the
 * decimation-in-time IFFT algorithm. The result is computed in-place. 
 * 
 * @operation{
 * &    x[n] = \frac{1}{2^{\alpha}} \sum_{f=0}^{N-1} \left( X[f]\cdot e^{j2\pi fn/N} \right) \\
 * &         \text{ for } 0 \le n < N
 * }
 * 
 * `x[]` is interpreted to be a block floating-point vector with shared exponent `*exp` and with
 * `*hr` bits of headroom initially in `x[]`. During computation, this function monitors the
 * headroom of the data and compensates to avoid overflows and underflows by bit-shifting the data
 * up or down as appropriate. In the equation above, @math{\alpha} represents the (net) number of
 * bits that the data was right-shifted by.
 * 
 * Upon completion, `*hr` is updated with the final headroom in `x[]`, and the exponent `*exp` is
 * incremented by @math{\alpha}.
 * 
 * @note In order to guarantee that saturation will not occur, `x[]` must have an _initial_ headroom
 *       of at least 2 bits.
 * 
 * @param[inout]  x     The `N`-element complex input vector to be transformed.
 * @param[in]     N     The size of the inverse DFT to be performed.
 * @param[inout]  hr    Pointer to the initial headroom in `x[]`.
 * @param[inout]  exp   Pointer to the initial exponent associated with `x[]`.
 * 
 * @exception ET_LOAD_STORE Raised if `x` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup fft_api
 */
C_API
void fft_dit_inverse (
    complex_s32_t x[], 
    const unsigned N, 
    headroom_t* hr, 
    exponent_t* exp);

/**
 * @brief Compute a forward DFT using the decimation-in-frequency FFT algorithm.
 * 
 * This function computes the `N`-point forward DFT of a complex input signal using the
 * decimation-in-frequency FFT algorithm. The result is computed in-place. 
 * 
 * @operation{
 * &    X[f] = \frac{1}{2^{\alpha}} \sum_{n=0}^{N-1} \left( x[n]\cdot e^{-j2\pi fn/N} \right) \\
 * &         \text{ for } 0 \le f < N
 * }
 * 
 * `x[]` is interpreted to be a block floating-point vector with shared exponent `*exp` and with
 * `*hr` bits of headroom initially in `x[]`. During computation, this function monitors the
 * headroom of the data and compensates to avoid overflows and underflows by bit-shifting the data
 * up or down as appropriate. In the equation above, @math{\alpha} represents the (net) number of
 * bits that the data was right-shifted by.
 * 
 * Upon completion, `*hr` is updated with the final headroom in `x[]`, and the exponent `*exp` is
 * incremented by @math{\alpha}.
 * 
 * @note In order to guarantee that saturation will not occur, `x[]` must have an _initial_ headroom
 *       of at least 2 bits.
 * 
 * @param[inout]  x     The `N`-element complex input vector to be transformed.
 * @param[in]     N     The size of the DFT to be performed.
 * @param[inout]  hr    Pointer to the initial headroom in `x[]`.
 * @param[inout]  exp   Pointer to the initial exponent associated with `x[]`.
 * 
 * @exception ET_LOAD_STORE Raised if `x` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup fft_api
 */
C_API
void fft_dif_forward (
    complex_s32_t x[], 
    const unsigned N, 
    headroom_t* hr, 
    exponent_t* exp);

/**
 * @brief Compute an inverse DFT using the decimation-in-frequency IFFT algorithm.
 * 
 * This function computes the `N`-point inverse DFT of a complex spectrum using the
 * decimation-in-frequency IFFT algorithm. The result is computed in-place. 
 * 
 * @operation{
 * &    x[n] = \frac{1}{2^{\alpha}} \sum_{f=0}^{N-1} \left( X[f]\cdot e^{j2\pi fn/N} \right) \\
 * &         \text{ for } 0 \le n < N
 * }
 * 
 * `x[]` is interpreted to be a block floating-point vector with shared exponent `*exp` and with
 * `*hr` bits of headroom initially in `x[]`. During computation, this function monitors the
 * headroom of the data and compensates to avoid overflows and underflows by bit-shifting the data
 * up or down as appropriate. In the equation above, @math{\alpha} represents the (net) number of
 * bits that the data was right-shifted by.
 * 
 * Upon completion, `*hr` is updated with the final headroom in `x[]`, and the exponent `*exp` is
 * incremented by @math{\alpha}.
 * 
 * @note In order to guarantee that saturation will not occur, `x[]` must have an _initial_ headroom
 *       of at least 2 bits.
 * 
 * @param[inout]  x     The `N`-element complex input vector to be transformed.
 * @param[in]     N     The size of the inverse DFT to be performed.
 * @param[inout]  hr    Pointer to the initial headroom in `x[]`.
 * @param[inout]  exp   Pointer to the initial exponent associated with `x[]`.
 * 
 * @exception ET_LOAD_STORE Raised if `x` is not word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup fft_api
 */
C_API
void fft_dif_inverse (
    complex_s32_t x[], 
    const unsigned N, 
    headroom_t* hr, 
    exponent_t* exp);


/**
 * @brief Perform forward FFT on a vector of IEEE754 floats.
 * 
 * This function takes real input vector @vector{x} and performs a forward FFT on the signal
 * in-place to get output vector @math{\bar{X} = FFT\{\bar{x}\}}. This implementation is accelerated
 * by converting the IEEE754 float vector into a block floating-point representation to compute the
 * FFT. The resulting BFP spectrum is then converted back to IEEE754 single-precision floats. The
 * operation is performed in-place on `x[]`.
 * 
 * See `bfp_fft_forward_mono()` for the details of the FFT.
 * 
 * Whereas the input `x[]` is an array of `fft_length` `float` elements, the output (placed in
 * `x[]`) is an array of `fft_length/2` `complex_float_t` elements, so the input should be cast
 * after calling this.
 * 
 * @code{.c}
 *  const unsigned FFT_N = 512
 *  float time_series[FFT_N] = { ... };
 *  fft_f32_forward(time_series, FFT_N);
 *  complex_float_t* freq_spectrum = (complex_float_t*) &time_series[0];
 *  const unsigned FREQ_BINS = FFT_N/2;
 *  // e.g.   freq_spectrum[FREQ_BINS-1].re
 * @endcode
 * 
 * `x[]` must begin at a double-word-aligned address.
 * 
 * 
 * @operation{ 
 * &     \bar{X}  \leftarrow FFT\{\bar{x}\}  
 * }
 * 
 * @param[inout]  x           Input vector @vector{x}
 * @param[in]     fft_length  The length of @vector{x}
 * 
 * @returns Pointer to frequency-domain spectrum (i.e. `((complex_float_t*) &x[0])`)
 * 
 * @exception ET_LOAD_STORE Raised if `x` is not double-word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_f32_api
 */
C_API
complex_float_t* fft_f32_forward(
    float x[],
    const unsigned fft_length);


/**
 * @brief Perform inverse FFT on a vector of complex_float_t.
 * 
 * This function takes complex input vector @vector{X} and performs an inverse real FFT on the
 * spectrum in-place to get output vector @math{\bar{x} = IFFT\{\bar{X}\}}. This implementation is
 * accelerated by converting the IEEE754 float vector into a block floating-point representation to
 * compute the IFFT. The resulting BFP signal is then converted back to IEEE754 single-precision
 * floats. The operation is performed in-place on `X[]`.
 * 
 * See `bfp_fft_inverse_mono()` for the details of the IFFT.
 * 
 * Input `X[]` is an array of `fft_length/2` `complex_float_t` elements. The output (placed in 
 * `X[]`) is an array of `fft_length` `float` elements.
 * 
 * @code{.c}
 *  const unsigned FFT_N = 512
 *  complex_float_t freq_spectrum[FFT_N/2] = { ... };
 *  fft_f32_inverse(freq_spectrum, FFT_N);
 *  float* time_series = (float*) &freq_spectrum[0];
 * @endcode
 * 
 * `X[]` must begin at a double-word-aligned address.
 * 
 * @param[inout]  X           Input vector @vector{X}
 * @param[in]     fft_length  The FFT length. Twice the element count of @vector{X}.
 * 
 * @returns Pointer to time-domain signal (i.e. `((float*) &X[0])`)
 * 
 * @exception ET_LOAD_STORE Raised if `X` is not double-word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup vect_f32_api
 */
C_API
float* fft_f32_inverse(
    complex_float_t X[],
    const unsigned fft_length);

    
#ifdef __XC__
} // extern "C"
#endif