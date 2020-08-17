
#ifndef XS3_FFT_LOW_H_
#define XS3_FFT_LOW_H_

#ifdef __XC__
extern "C" {
#endif


/**
 * 
 */
void xs3_bit_reverse_indexes(
    complex_s32_t* a,
    const unsigned length);

/**
 * @brief Split the merged spectra resulting from DFTing a pair of real signals together.
 * 
 * Two real signals `a[n]` and `b[n]` can be DFTed simultaneously by computing the DFT of a third, complex signal `x[n]`
 * in which `x[n] = a[n] + j*b[n]`. The resulting spectrum `X[f]` contains the desired DFTs `A[f]` and `B[f]` mixed 
 * together. This function separates `X[f]` back into `A[f]` and `B[f]`, placing the resulting complex spectra into
 * `A` and `B`.
 * 
 * Note that the DFT `G[f]` of a real signal `g[n]` is periodic with period `N` (the DFT length), and with a complex 
 * conjugate symmetry such that `G[-f] = G[f]*`. The `N`-point DFT of a real signal can therefore be represented with 
 * only `N/2` complex elements. As such, this function only writes `A[f]` and `B[f]` for `0 <= f < N/2`. All remaining
 * elements `A[f]` and `B[f]` can be calculated using following properties:
 * 
 * `G[-k] = (G[f])*`
 * and
 * `G[f] = G[k+N]`
 * 
 * where the `( )*` operator represents the complex conjugate.
 * 
 * The parameter `N` is the FFT length (and the length of `X`). The arrays `A` and `B` must have room for `N/2` complex 
 * elements each.
 * 
 * Often it will be desirable to apply this operator on `X[f]` in-place, which is done by passing `A = &X[0]` and 
 * `B = &X[N/2]`.
 * 
 * The resulting @math{\bar{A}[f]} is the spectrum of @math{Real\left{\bar{x}[n]\right}} and @math{\bar{B}[f]} is the 
 * spectrum of @math{Imag\left{\bar{x}[n]\right}}.
 * 
 * @param [out] A   The output spectrum @math{\bar{A}[f]}.
 * @param [out] B   The output spectrum @math{\bar{B}[f]}.
 * @param [in]  X   The input spectrum to be split @math{\bar{X}[f]}.
 * @param [in]  N   The size of the spectrum @vector{X[f]} in elements (i.e. FFT length).
 */
headroom_t xs3_split_fft_spectrum_s32(
    complex_s32_t* X,
    const unsigned N);

/**
 * 
 */
headroom_t xs3_merge_fft_spectra_s32(
    complex_s32_t* X,
    const unsigned N);
    
void xs3_s32_to_complex_s32(
    complex_s32_t* a,
    const int32_t* b,
    const unsigned length,
    const right_shift_t b_shr);
    
void xs3_real_part_complex_s32(
    int32_t* a,
    const complex_s32_t* b,
    const unsigned length,
    const right_shift_t b_shr);
    
void xs3_imag_part_complex_s32(
    int32_t* a,
    const complex_s32_t* b,
    const unsigned length,
    const right_shift_t b_shr);

void xs3_merge_time_series_s32(
    complex_s32_t* a,
    const int32_t* b,
    const int32_t* c,
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);

void xs3_spectrum_tail_reverse(
    complex_s32_t* X,
    const unsigned N);

void xs3_real_fft_final_pass_s32(
    complex_s32_t* X,
    const unsigned N,
    const complex_s32_t* W);

void xs3_real_ifft_first_pass_s32(
    complex_s32_t* X,
    const unsigned N,
    const complex_s32_t* W);

/**
 * @brief Compute a forward FFT (decimation-in-time).
 * 
 */
void xs3_fft_dit_s32 (
    complex_s32_t * x, 
    unsigned n, 
    headroom_t* hr, 
    const complex_s32_t* twiddle_lut, 
    exponent_t* exp);

/**
 * @brief Compute an inverse FFT (decimation-in-time).
 * 
 */
void xs3_ifft_dit_s32 (
    complex_s32_t* x, 
    unsigned n, 
    headroom_t* hr, 
    const complex_s32_t* twiddle_lut, 
    exponent_t* exp);

/**
 * @brief Compute a forward FFT (decimation-in-frequency).
 * 
 */
void xs3_fft_dif_s32 (
    complex_s32_t * x, 
    unsigned n, 
    headroom_t* hr, 
    const complex_s32_t* twiddle_lut, 
    exponent_t* exp);

/**
 * @brief Compute an inverse FFT (decimation-in-frequency).
 * 
 */
void xs3_ifft_dif_s32 (
    complex_s32_t* x, 
    unsigned n, 
    headroom_t* hr, 
    const complex_s32_t* twiddle_lut, 
    exponent_t* exp);


#ifdef __XC__
}   //extern "C"
#endif

#endif //XS3_FFT_LOW_H_