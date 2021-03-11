// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#ifndef XS3_FFT_H_
#define XS3_FFT_H_

// #include "xs3_fft_lut.h"

#ifdef __XC__
extern "C" {
#endif


/**
 * @brief Applies the index bit-reversal required for FFTs.
 * 
 * Applying an FFT (xs3_fft_dit_forward() or xs3_fft_dif_forward()) or IFFT (xs3_fft_dit_inverse() or xs3_fft_dif_inverse()) requires
 * an operation where the elements in a complex vector are rearranged such that each element ends up at a location
 * whose index is the bit-reversal of the index at which it began. This function performs that operation.
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
void xs3_fft_index_bit_reversal(
    complex_s32_t x[],
    const unsigned length);

/**
 * @brief Splits the merged spectrum that results from DFTing a pair of real signals together.
 * 
 * Two real signals @math{a[n]} and @math{b[n]} can be DFTed simultaneously by computing the DFT of a third, complex 
 * signal @math{x[n] = a[n] + j\cdot b[n]}. The resulting spectrum @math{X[f]} contains the desired DFTs @math{A[f]} and 
 * @math{B[f]} mixed together in a separable way. This function separates @math{X[f]} into @math{A[f]} and @math{B[f]}.
 * 
 * Note that the DFT @math{G[f]} of a real signal @math{g[n]} is periodic with period @math{N}, and with a complex 
 * conjugate symmetry such that @math{G[-f] = G^*[f]}. The @math{N}-point DFT of a real signal can therefore be 
 * represented with only @math{N/2} complex elements. As such, this function only writes @math{A[f]} and @math{B[f]} for 
 * @math{0 <= f < N/2}. 
 * 
 * All remaining elements can be calculated using the following properties of the DFT of a real signal:
 * 
 * @math{G[-k] = G^*[f]}
 * and
 * @math{G[f] = G[k+N]}
 * 
 * where @math{G^*[f]} is the complex conjugate of @math{G[f]}.
 * 
 * Note that the above properties imply that both @math{G[0]} and @math{G[N/2]} must be purely real. For simplicity 
 * (i.e. so the operation can be performed in-place), the real part of @math{A[N/2]} and @math{B[N/2]} will be packed
 * into the imaginary part of @math{A[0]} and @math{B[0]} respectively.
 * 
 * This function will pack the spectra @math{A[f]} and @math{B[f]} as specified for real stereo signals in @ref 
 * spectrum_packing. @math{A[f]} will begin at address `&x[0]` and @math{B[f]} will begin at address `&x[length/2]`.
 * 
 * `length` is the DFT length @math{N}, as well as the length of `x` and must be a power of 2.
 * 
 * This function returns the headroom of the resulting vector `x`. 
 * 
 * @note Either of the resulting two spectra @math{A[f]} and @math{B[f]} may actually have more headroom than the value
 *       returned. Due to an optimization in this algorithm's implementation, only the minimum of the two headrooms is
 *       computed. If an accurate headroom count is required on the resulting spectra, they should be computed following
 *       this function.
 * 
 * @param [in] x        The spectrum @math{X[f]} to be split into @math{A[f]} and @math{B[f]}.
 * @param [in] length   The length of `x` in elements (i.e. FFT length @math{N}).
 * 
 * @return The headroom of the result `x`
 */
headroom_t xs3_fft_spectra_split(
    complex_s32_t x[],
    const unsigned length);

/**
 * @brief Merges the spectra of two real signals so they can be IDFTed simultaneously.
 * 
 * This function is the inverse of xs3_fft_spectra_split().
 * 
 * If two spectra @math{A[f]} and @math{B[f]} meeting certain requirements are assumed to be the spectra of two real 
 * signals @math{a[n]} and @math{b[n]} respectively, then the two spectra can be combined in such a way that an inverse
 * complex DFT will invert the spectra simultaneously, resulting in a complex signal @math{x[n] = a[n] + j\cdot b[n]}.
 * This function performs that merger.
 * 
 * This function requires the spectra of @math{A[f]} and @math{B[f]} to be packed into `x` as speciied for real stereo
 * spectra in @ref spectrum_packing. Note that this is how the spectra are packed by xs3_fft_spectra_split().
 * 
 * `length` is the inverse DFT length @math{N}, as well as the length of `x` and must be a power of 2.
 * 
 * This function returns the headroom of the resulting vector `x`. 
 * 
 * @param [in] x        The spectra @math{A[f]} and @math{B[f]}, packed as specified.
 * @param [in] length   The length of `x` in elements (i.e. FFT length @math{N}).
 * 
 * @return The headroom of the result `x`
 */
headroom_t xs3_fft_spectra_merge(
    complex_s32_t x[],
    const unsigned length);

/**
 * @brief Makes the adjustments required when performing a mono DFT or IDFT.
 * 
 * An @math{N/2}-point DFT can be used to compute the @math{N}-point DFT @math{X[f]} of a single real signal 
 * @math{x[n]}. Similarly, an @math{(N/2)}-point inverse DFT can be used to compute the real @math{N}-point inverse DFT 
 * @math{x[n]} of a spectrum @math{X[f]}. This is more efficient (in terms of both memory and compute resources) than 
 * performing the operations using the full @math{N}-point complex DFT or inverse DFT. 
 * 
 * This function performs a manipulation of the spectrum required in that process.
 * 
 * To perform the @math{N}-point forward DFT on such a real signal `x[n]`:
 * \code
 *      int32_t x[N] = { ... };
 *      complex_s32_t* X = (complex_s32_t*)x;
 *      exponent_t x_exp = ...;
 *      headroom_t hr = xs3_vect_s32_headroom(x, N);
 *      xs3_fft_index_bit_reversal(X, N);
 *      xs3_fft_dit_forward(X, N/2, &hr, &x_exp);
 *      xs3_fft_mono_adjust(X, N, 0);
 * \endcode
 * 
 * To perform the @math{N}-point inverse DFT on the spectrum `X[n]` of a real signal `x[n]`:
 * \code
 *      complex_s32_t X[N/2] = { ... };
 *      int32_t* x = (int32_t*)X;
 *      exponent_t X_exp = ...;
 *      headroom_t hr = xs3_vect_s32_headroom(x, N);
 *      xs3_fft_mono_adjust(X, N, 1);
 *      xs3_fft_index_bit_reversal(X, N);
 *      xs3_fft_dit_inverse(X, N/2, &hr, &X_exp);
 * \endcode
 * 
 * @note xs3_fft_dit_forward() and xs3_fft_dit_inverse() each require a certain amount of headroom to avoid overflows. 
 * See their documentation for details.
 * 
 * `x[]` is an @math{N/2}-element complex vector representing the spectrum to be adjusted.
 * 
 * `length` is size of the real DFT to be computed, _not_ the number of elements in `x[]` (or the size of the complex
 * DFT to actually be employed).
 * 
 * `inverse` should be `1` if the inverse DFT is being computed, and `0` otherwise.
 * 
 * @param[in] x         The spectrum @math{X[f]} to be modified.
 * @param[in] length    The size of the DFT to be computed. Twice the length of `x` (in elements).
 * @param[in] inverse   Flag indicating whether the inverse DFT is being computed.
 */
void xs3_fft_mono_adjust(
    complex_s32_t x[],
    const unsigned length,
    const unsigned inverse);

/**
 * @brief Compute a forward DFT using the decimation-in-time FFT algorithm.
 * 
 * This function computes the `N`-point forward DFT of a complex input signal using the decimation-in-time FFT 
 * algorithm. The result is computed in-place. 
 * 
 * Conceptually, the operation performed is the following:
 * 
 * \f[
 *      X[f] = \frac{1}{2^{\alpha}} \sum_{n=0}^{N-1} \left( x[n]\cdot e^{-j2\pi fn/N} \right)
 *      \text{ for } 0 \le f \lt N
 * \f]
 * 
 * `x[]` is interpreted to be a block floating-point vector with shared exponent `*exp` and with `*hr` bits of headroom
 * initially in `x[]`. During computation, this function monitors the headroom of the data and compensates to avoid 
 * overflows and underflows by bit-shifting the data up or down as appropriate. In the equation above, @math{\alpha}    
 * represents the (net) number of bits that the data was right-shifted by.
 * 
 * Upon completion, `*hr` is updated with the final headroom in `x[]`, and the exponent `*exp` is incremented by 
 * @math{\alpha}.
 * 
 * @note In order to guarantee that saturation will not occur, `x[]` must have an _initial_ headroom of at least 2 
 *       bits.
 * 
 * @param[inout]  x     The `N`-element complex input vector to be transformed.
 * @param[in]     N     The size of the DFT to be performed.
 * @param[inout]  hr    Pointer to the initial headroom in `x[]`.
 * @param[inout]  exp   Pointer to the initial exponent associated with `x[]`.
 */
void xs3_fft_dit_forward (
    complex_s32_t x[], 
    const unsigned N, 
    headroom_t* hr, 
    exponent_t* exp);

/**
 * @brief Compute an inverse DFT using the decimation-in-time IFFT algorithm.
 * 
 * This function computes the `N`-point inverse DFT of a complex spectrum using the decimation-in-time IFFT 
 * algorithm. The result is computed in-place. 
 * 
 * Conceptually, the operation performed is the following:
 * 
 * \f[
 *      x[n] = \frac{1}{2^{\alpha}} \sum_{f=0}^{N-1} \left( X[f]\cdot e^{j2\pi fn/N} \right)
 *      \text{ for } 0 \le n \lt N
 * \f]
 * 
 * `x[]` is interpreted to be a block floating-point vector with shared exponent `*exp` and with `*hr` bits of headroom
 * initially in `x[]`. During computation, this function monitors the headroom of the data and compensates to avoid 
 * overflows and underflows by bit-shifting the data up or down as appropriate. In the equation above, @math{\alpha}    
 * represents the (net) number of bits that the data was right-shifted by.
 * 
 * Upon completion, `*hr` is updated with the final headroom in `x[]`, and the exponent `*exp` is incremented by 
 * @math{\alpha}.
 * 
 * @note In order to guarantee that saturation will not occur, `x[]` must have an _initial_ headroom of at least 2 
 *       bits.
 * 
 * @param[inout]  x     The `N`-element complex input vector to be transformed.
 * @param[in]     N     The size of the inverse DFT to be performed.
 * @param[inout]  hr    Pointer to the initial headroom in `x[]`.
 * @param[inout]  exp   Pointer to the initial exponent associated with `x[]`.
 */
void xs3_fft_dit_inverse (
    complex_s32_t x[], 
    const unsigned N, 
    headroom_t* hr, 
    exponent_t* exp);

/**
 * @brief Compute a forward DFT using the decimation-in-frequency FFT algorithm.
 * 
 * This function computes the `N`-point forward DFT of a complex input signal using the decimation-in-frequency FFT 
 * algorithm. The result is computed in-place. 
 * 
 * Conceptually, the operation performed is the following:
 * 
 * \f[
 *      X[f] = \frac{1}{2^{\alpha}} \sum_{n=0}^{N-1} \left( x[n]\cdot e^{-j2\pi fn/N} \right)
 *      \text{ for } 0 \le f \lt N
 * \f]
 * 
 * `x[]` is interpreted to be a block floating-point vector with shared exponent `*exp` and with `*hr` bits of headroom
 * initially in `x[]`. During computation, this function monitors the headroom of the data and compensates to avoid 
 * overflows and underflows by bit-shifting the data up or down as appropriate. In the equation above, @math{\alpha}    
 * represents the (net) number of bits that the data was right-shifted by.
 * 
 * Upon completion, `*hr` is updated with the final headroom in `x[]`, and the exponent `*exp` is incremented by 
 * @math{\alpha}.
 * 
 * @note In order to guarantee that saturation will not occur, `x[]` must have an _initial_ headroom of at least 2 
 *       bits.
 * 
 * @param[inout]  x     The `N`-element complex input vector to be transformed.
 * @param[in]     N     The size of the DFT to be performed.
 * @param[inout]  hr    Pointer to the initial headroom in `x[]`.
 * @param[inout]  exp   Pointer to the initial exponent associated with `x[]`.
 */
void xs3_fft_dif_forward (
    complex_s32_t x[], 
    const unsigned N, 
    headroom_t* hr, 
    exponent_t* exp);

/**
 * @brief Compute an inverse DFT using the decimation-in-frequency IFFT algorithm.
 * 
 * This function computes the `N`-point inverse DFT of a complex spectrum using the decimation-in-frequency IFFT 
 * algorithm. The result is computed in-place. 
 * 
 * Conceptually, the operation performed is the following:
 * 
 * \f[
 *      x[n] = \frac{1}{2^{\alpha}} \sum_{f=0}^{N-1} \left( X[f]\cdot e^{j2\pi fn/N} \right)
 *      \text{ for } 0 \le n \lt N
 * \f]
 * 
 * `x[]` is interpreted to be a block floating-point vector with shared exponent `*exp` and with `*hr` bits of headroom
 * initially in `x[]`. During computation, this function monitors the headroom of the data and compensates to avoid 
 * overflows and underflows by bit-shifting the data up or down as appropriate. In the equation above, @math{\alpha}    
 * represents the (net) number of bits that the data was right-shifted by.
 * 
 * Upon completion, `*hr` is updated with the final headroom in `x[]`, and the exponent `*exp` is incremented by 
 * @math{\alpha}.
 * 
 * @note In order to guarantee that saturation will not occur, `x[]` must have an _initial_ headroom of at least 2 
 *       bits.
 * 
 * @param[inout]  x     The `N`-element complex input vector to be transformed.
 * @param[in]     N     The size of the inverse DFT to be performed.
 * @param[inout]  hr    Pointer to the initial headroom in `x[]`.
 * @param[inout]  exp   Pointer to the initial exponent associated with `x[]`.
 */
void xs3_fft_dif_inverse (
    complex_s32_t x[], 
    const unsigned N, 
    headroom_t* hr, 
    exponent_t* exp);


#ifdef __XC__
}   //extern "C"
#endif

#endif //XS3_FFT_H_
