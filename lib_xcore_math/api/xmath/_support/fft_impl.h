// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once



/**
 * @brief Applies the index bit-reversal required for FFTs.
 * 
 * Applying an FFT (fft_dit_forward() or fft_dif_forward()) or IFFT (fft_dit_inverse()
 * or fft_dif_inverse()) requires an operation where the elements in a complex vector are
 * rearranged such that each element ends up at a location whose index is the bit-reversal of the
 * index at which it began. This function performs that operation.
 * 
 * Only the @math{log2(length)} least significant bits of each index are considered.
 * 
 * For example, when performing a @math{32}-point FFT:
 * 
 * \code
 *      12 = 0b01100
 *      bit_reverse(k, log2(32)) -> 0b00110 = 6
 *      X[12] <- X[6]
 *      X[6]  <- X[12]
 * \endcode
 * 
 * `x` is updated in-place.
 * 
 * `length` must be a power of 2.
 * 
 * @param[in] x         The vector to have its elements reordered.
 * @param[in] length    The length of `x` (element count).
 */
C_API
void fft_index_bit_reversal(
    complex_s32_t x[],
    const unsigned length);
    

/**
 * @brief Splits the merged spectrum that results from DFTing a pair of real signals together.
 * 
 * Two real signals @math{a[n]} and @math{b[n]} can be DFTed simultaneously by computing the DFT of
 * a third, complex signal @math{x[n] = a[n] + j\cdot b[n]}. The resulting spectrum @math{X[f]}
 * contains the desired DFTs @math{A[f]} and 
 * @math{B[f]} mixed together in a separable way. This function separates @math{X[f]} into
 * @math{A[f]} and @math{B[f]}.
 *
 * Note that the DFT @math{G[f]} of a real signal @math{g[n]} is periodic with period @math{N}, and
 * with a complex conjugate symmetry such that @math{G[-f] = G^*[f]}. The @math{N}-point DFT of a
 * real signal can therefore be represented with only @math{N/2} complex elements. As such, this
 * function only writes @math{A[f]} and @math{B[f]} for @math{0 <= f < N/2}. 
 * 
 * All remaining elements can be calculated using the following properties of the DFT of a real
 * signal:
 * 
 * @math{G[-f] = G^*[f]}
 * and
 * @math{G[f] = G[f+N]}
 * 
 * where @math{G^*[f]} is the complex conjugate of @math{G[f]}.
 * 
 * Note that the above properties imply that both @math{G[0]} and @math{G[N/2]} must be purely real.
 * For simplicity (i.e. so the operation can be performed in-place), the real part of @math{A[N/2]}
 * and @math{B[N/2]} will be packed into the imaginary part of @math{A[0]} and @math{B[0]}
 * respectively.
 * 
 * This function will pack the spectra @math{A[f]} and @math{B[f]} as specified for real stereo
 * signals in @ref spectrum_packing. @math{A[f]} will begin at address `&x[0]` and @math{B[f]} will
 * begin at address `&x[length/2]`.
 * 
 * `length` is the DFT length @math{N}, as well as the length of `x` and must be a power of 2.
 * 
 * This function returns the headroom of the resulting vector `x`. 
 * 
 * @note Either of the resulting two spectra @math{A[f]} and @math{B[f]} may actually have more
 *       headroom than the value returned. Due to an optimization in this algorithm's
 *       implementation, only the minimum of the two headrooms is computed. If an accurate headroom
 *       count is required on the resulting spectra, they should be computed following this
 *       function.
 * 
 * @param [in] x        The spectrum @math{X[f]} to be split into @math{A[f]} and @math{B[f]}.
 * @param [in] length   The length of `x` in elements (i.e. FFT length @math{N}).
 * 
 * @return The headroom of the result `x`
 * 
 * @exception ET_LOAD_STORE Raised if `x` is not word-aligned (See @ref note_vector_alignment)
 */
C_API
headroom_t fft_spectra_split(
    complex_s32_t x[],
    const unsigned length);

/**
 * @brief Merges the spectra of two real signals so they can be IDFTed simultaneously.
 * 
 * This function is the inverse of fft_spectra_split().
 * 
 * If two spectra @math{A[f]} and @math{B[f]} meeting certain requirements are assumed to be the
 * spectra of two real signals @math{a[n]} and @math{b[n]} respectively, then the two spectra can be
 * combined in such a way that an inverse complex DFT will invert the spectra simultaneously,
 * resulting in a complex signal @math{x[n] = a[n] + j\cdot b[n]}. This function performs that
 * merger.
 * 
 * This function requires the spectra of @math{A[f]} and @math{B[f]} to be packed into `x` as
 * speciied for real stereo spectra in @ref spectrum_packing. Note that this is how the spectra are
 * packed by fft_spectra_split().
 * 
 * `length` is the inverse DFT length @math{N}, as well as the length of `x` and must be a power of
 * 2.
 * 
 * This function returns the headroom of the resulting vector `x`. 
 * 
 * @param [in] x        The spectra @math{A[f]} and @math{B[f]}, packed as specified.
 * @param [in] length   The length of `x` in elements (i.e. FFT length @math{N}).
 * 
 * @return The headroom of the result `x`
 * 
 * @exception ET_LOAD_STORE Raised if `x` is not word-aligned (See @ref note_vector_alignment)
 */
C_API
headroom_t fft_spectra_merge(
    complex_s32_t x[],
    const unsigned length);

/**
 * @brief Makes the adjustments required when performing a mono DFT or IDFT.
 * 
 * An @math{N/2}-point DFT can be used to compute the @math{N}-point DFT @math{X[f]} of a single
 * real signal @math{x[n]}. Similarly, an @math{(N/2)}-point inverse DFT can be used to compute the
 * real @math{N}-point inverse DFT @math{x[n]} of a spectrum @math{X[f]}. This is more efficient (in
 * terms of both memory and compute resources) than performing the operations using the full
 * @math{N}-point complex DFT or inverse DFT. 
 * 
 * This function performs a manipulation of the spectrum required in that process.
 * 
 * To perform the @math{N}-point forward DFT on such a real signal `x[n]`:
 * \code
 *      int32_t x[N] = { ... };
 *      complex_s32_t* X = (complex_s32_t*)x;
 *      exponent_t x_exp = ...;
 *      headroom_t hr = vect_s32_headroom(x, N);
 *      fft_index_bit_reversal(X, N);
 *      fft_dit_forward(X, N/2, &hr, &x_exp);
 *      fft_mono_adjust(X, N, 0);
 * \endcode
 * 
 * To perform the @math{N}-point inverse DFT on the spectrum `X[n]` of a real signal `x[n]`:
 * \code
 *      complex_s32_t X[N/2] = { ... };
 *      int32_t* x = (int32_t*)X;
 *      exponent_t X_exp = ...;
 *      headroom_t hr = vect_s32_headroom(x, N);
 *      fft_mono_adjust(X, N, 1);
 *      fft_index_bit_reversal(X, N);
 *      fft_dit_inverse(X, N/2, &hr, &X_exp);
 * \endcode
 * 
 * @note fft_dit_forward() and fft_dit_inverse() each require a certain amount of headroom
 *       to avoid overflows. See their documentation for details.
 * 
 * `x[]` is an @math{N/2}-element complex vector representing the spectrum to be adjusted.
 * 
 * `length` is size of the real DFT to be computed, _not_ the number of elements in `x[]` (or the
 * size of the complex DFT to actually be employed).
 * 
 * `inverse` should be `1` if the inverse DFT is being computed, and `0` otherwise.
 * 
 * @param[in] x         The spectrum @math{X[f]} to be modified.
 * @param[in] length    The size of the DFT to be computed. Twice the length of `x` (in elements).
 * @param[in] inverse   Flag indicating whether the inverse DFT is being computed.
 * 
 * @exception ET_LOAD_STORE Raised if `x` is not word-aligned (See @ref note_vector_alignment)
 */
C_API
void fft_mono_adjust(
    complex_s32_t x[],
    const unsigned length,
    const unsigned inverse);
