// Copyright 2021-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

// This file exists as a compatibility work-around between vanilla Doxygen and 
// sphinx+breathe+doxygen.
// If these notes are written in an .md file, sphinx can't interpret them. If they're 
// written in an .rst file, Doxygen can't interpret them and can't build link references 
// to them.

/**
 * @page note_vector_alignment Note: Vector Alignment
 * 
 * 
 * This library makes use of the XMOS architecture's vector processing unit (VPU). All loads and
 * stores to and from the XS3 VPU have the requirement that the loaded/stored addresses must be
 * aligned to a 4-byte boundary (word-aligned).
 * 
 * In the current version of the API, this leads to the requirement that most API functions 
 * require vectors (or the data backing a BFP vector) to begin at word-aligned addresses. 
 * Vectors are *not* required, however, to have a size (in bytes) that is a multiple of 4.
 * 
 * @par Writing Alignment-safe Code
 * @parblock
 * 
 * The alignment requirement is ultimately always on the data that backs a vector. For the 
 * low-level API, that is the pointers passed to the functions themselves. For the high-level 
 * API, that is the memory to which the `data` field (or the `real` and `imag` fields in the 
 * case of `bfp_complex_s16_t`) points, specified when the BFP vector is initialized.
 * 
 * Arrays of type `int32_t` and `complex_s32_t` will normally be guaranteed to be word-aligned 
 * by the compiler. However, if the user manually specifies the beginning of an `int32_t` array, 
 * as in the following..
 * 
 * @code{.c}
 *  uint8_t byte_buffer[100];
 *  int32_t* integer_array = (int32_t*) &byte_buffer[1];
 * @endcode
 * 
 * .. the vector may not be word-aligned. It is the responsibility of the user to ensure proper
 * alignment of data.
 * 
 * For `int16_t` arrays, the compiler does not by default guarantee that the array starts on a 
 * word-aligned address. To force word-alignment on arrays of this type, use 
 * `__attribute__((aligned (4)))` in the variable definition, as in the following.
 * 
 * @code{.c}
 *     int16_t __attribute__((aligned (4))) data[100];
 * @endcode
 * 
 * Occasionally, 8-byte (double word) alignment is required. In this case, neither `int32_t` nor 
 * `int16_t` is necessarily guaranteed to align as required. Similar to the above, this can be 
 * hinted to the compiler as in the following.
 * 
 * @code{.c}
 *     int32_t __attribute__((aligned (8))) data[100];
 * @endcode
 * 
 * This library also provides the macros `WORD_ALIGNED` and `DWORD_ALIGNED` which force 4- and
 * 8-byte alignment respectively as above.
 * 
 * @endparblock
 */



/**
 * @page note_symmetric_saturation Note: Symmetrically Saturating Arithmetic
 * 
 * With ordinary integer arithmetic the block floating-point logic chooses exponents and operand
 * shifts to prevent integer overflow with worst-case input values. However, the XS3 VPU uses
 * symmetrically saturating integer arithmetic.
 *
 * Saturating arithmetic is that where partial results of the applied operation use a bit depth
 * greater than the output bit depth, and values that can't be properly expressed with the output
 * bit depth are set to the nearest expressible value. 
 *
 * For example, in ordinary C integer arithmetic, a function which multiplies two 32-bit integers
 * may internally compute the full 64-bit product and then clamp values to the range `(INT32_MIN,
 * INT32_MAX)` before returning a 32-bit result.
 *
 * Symmetrically saturating arithmetic also includes the property that the lower bound of the
 * expressible range is the negative of the upper bound of the expressible range.
 *
 * One of the major troubles with non-saturating integer arithmetic is that in a twos complement
 * encoding, there exists a non-zero integer (e.g. INT16_MIN in 16-bit twos complement arithmetic)
 * value @math{x} for which  @math{-1 \cdot x = x}. Serious arithmetic errors can result when this
 * case is not accounted for.
 *
 * One of the results of _symmetric_ saturation, on the other hand, is that there is a corner case
 * where (using the same exponent and shift logic as non-saturating arithmetic) saturation may occur
 * for a particular combination of input mantissas. The corner case is different for different
 * operations.
 *
 * When the corner case occurs, the minimum (and largest magnitude) value of the resulting vector is
 * 1 LSb greater than its ideal value (e.g. `-0x3FFF` instead of `-0x4000` for 16-bit arithmetic).
 * The error in this output element's mantissa is then 1 LSb, or 
 * @math{2^p}, where @math{p} is the exponent of the resulting BFP vector.
 *
 * Of course, the very nature of BFP arithmetic routinely involves errors of this magnitude.
 */


/**
 * @page note_spectrum_packing Note: Spectrum Packing
 * 
 * 
 * In its general form, the @math{N}-point Discrete Fourier Transform is an operation applied 
 * to a complex @math{N}-point signal @math{x[n]} to produce a complex spectrum @math{X[f]}. 
 * Any spectrum @math{X[f]} which is the result of a @math{N}-point DFT has the property that 
 * @math{X[f+N] = X[f]}. Thus, the complete representation of the @math{N}-point DFT of 
 * @math{X[n]} requires @math{N} complex elements.
 * 
 * @par Complex DFT and IDFT
 * @parblock
 * 
 * In this library, when performing a complex DFT (e.g. using fft_bfp_forward_complex()), the 
 * spectral representation that results in a straight-forward mapping:
 * 
 * `X[f]` @math{\longleftarrow X[f]} for @math{0 \le f < N}
 * 
 * where `X` is an @math{N}-element array of `complex_s32_t`, where the real part of @math{X[f]} 
 * is in `X[f].re` and the imaginary part in `X[f].im`.
 * 
 * Likewise, when performing an @math{N}-point complex inverse DFT, that is also the 
 * representation that is expected.
 * @endparblock
 * 
 * @par Real DFT and IDFT
 * @parblock
 * 
 * Oftentimes we instead wish to compute the DFT of real signals. In addition to the periodicity 
 * property (@math{X[f+N] = X[f]}), the DFT of a real signal also has a complex conjugate symmetry 
 * such that @math{X[-f] = X^*[f]}, where @math{X^*[f]} is the complex conjugate of @math{X[f]}. 
 * This symmetry makes it redundant (and thus undesirable) tostore such symmetric pairs of elements. 
 * This would allow us to get away with only explicitly storing @math{X[f} for 
 * @math{0 \le f \le N/2} in @math{(N/2)+1} complex elements.
 * 
 * Unfortunately, using such a representation has the undesirable property that the DFT of an 
 * @math{N}-point real signal cannot be computed in-place, as the representation requires more 
 * memory than we started with.

 * However, if we take the periodicity and complex conjugate symmetry properties together:
 * 
 * @f[
 * &    X[0] = X^*[0] \rightarrow Imag\{X[0]\} = 0      \\
 * &    X[-(N/2) + N] = X[N/2]                          \\
 * &    X[-N/2] = X^*[N/2] \rightarrow X[N/2] = X^*[N/2] \rightarrow Imag \{ X[N/2] \} = 0
 * @f]
 * 
 * Because both @math{X[0]} and @math{X[N/2]} are guaranteed to be real, we can recover the benefit 
 * of in-place computation in our representation by packing the real part of @math{X[N/2]} into the 
 * imaginary part of @math{X[0]}.
 * 
 * Therefore, the functions in this library that produce the spectra of real signals (such as 
 * fft_bfp_forward_mono() and fft_bfp_forward_stereo()) will pack the spectra in a slightly less 
 * straight-forward manner (as compared with the complex DFTs):
 * 
 * `X[f]` @math{\longleftarrow X[f]} for @math{1 \le f < N/2}
 * 
 * `X[0]` @math{\longleftarrow X[0] + j X[N/2]}
 * 
 * where `X` is an @math{N/2}-element array of `complex_s32_t`.
 * 
 * Likewise, this is the encoding expected when computing the @math{N}-point inverse DFT, such as by 
 * fft_bfp_inverse_mono() or fft_bfp_inverse_stereo().
 * 
 * @note One additional note, when performing a stereo DFT or inverse DFT, so as to preserve the
 * in-place computation of the result, the spectra of the two signals will be encoded into adjacent
 * blocks of memory, with the second spectrum (i.e. associated with 'channel b') occupying the
 * higher memory address.
 *
 * @endparblock
 */



/**
 * @page fft_length_support Note: Library FFT Length Support
 * 
 * When computing DFTs this library relies on one or both of a pair of look-up tables which contain 
 * portions of the Discrete Fourier Transform matrix.  Longer FFT lengths require larger look-up
 * tables.  When building using CMake, the maximum FFT length can be specified as a CMake option,
 * and these tables are auto-generated at build time.
 *
 * If not using CMake, you can manually generate these files using a python script included with the
 * library. The script is located at `lib_xcore_math/scripts/gen_fft_table.py`. If generated
 * manually, you must add the generated .c file as a source, and the directory containing
 * `xmath_fft_lut.h` must be added as an include directory when compiling the library's files.
 *
 * Note that the header file must be named `xmath_fft_lut.h` as it is included via 
 * `#include "xmath_fft_lut.h"`.
 * 
 * By default the tables contain the coefficients necessary to perform forward or inverse DFTs of up
 * to 1024 points. If larger DFTs are required, or if the maximum required DFT size is known to be
 * less than 1024 points, the `MAX_FFT_LEN_LOG2` CMake option can be modified from its default value
 * of `10`.
 * 
 * The two look-up tables correspond to the decimation-in-time and decimation-in-frequency FFT
 * algorithms, and the run-time symbols for the tables are `xmath_dit_fft_lut` and
 * `xmath_dif_fft_lut` respectively. Each table contains @math{N-4} complex 32-bit values, with a
 * size of @math{8\cdot (N-4)} bytes each.
 * 
 * To manually regenerate the tables for amaximum FFT length of @math{16384} (@math{=2^{14}}),
 * supporting only the decimation-in-time algorithm, for example, use the following:
 * 
 * @code{.c}
 *     python lib_xcore_math/script/gen_fft_table.py --dit --max_fft_log2 14
 * @endcode
 * 
 * Use the `--help` flag with `gen_fft_table.py` for a more detailed description of its syntax and 
 * parameters.
 */



/**
 * @page filter_conversion Note: Digital Filter Conversion
 * 
 * This library supports optimized implementations of 16- and 32-bit FIR filters, as well as 
 * cascaded 32-bit biquad filters.  Each of these filter implementations requires that the 
 * filter coefficients be represented in a compatible form.
 * 
 * To assist with that, several python scripts are distributed with this library which can be
 * used to convert existing floating-point filter coefficients into a code which is easily 
 * callable from within an xCore application.
 * 
 * Each script reads in floating-point filter coefficients from a file and computes a new 
 * representation for the filter with coefficients which attempt to maximize precision and are 
 * compatible with the `lib_xcore_math` filtering API.
 * 
 * Each script outputs two files which can be included in your own xCore application. The first 
 * output is a C source (`.c`) file containing the computed filter parameters and
 * several function definitions for initializing and executing the generated filter.  The second
 * output is a C header (`.h`) file which can be `#include`d into your own application to
 * give access to those functions.
 * 
 * Additionally, each script also takes a user-provided filter name as an input parameter.  The 
 * output files (as well as the function names within) include the filter name so that more than
 * one filter can be generated and executed using this mechanism.
 * 
 * As an example, take the following command to generate a 32-bit FIR filter:
 * 
 *    python lib_xcore_math/script/gen_fir_filter_s32.py MyFilter filter_coefs.txt
 * 
 * This command creates a filter named "MyFilter", with coefficients taken from a file 
 * `filter_coefs.txt`.  Two output files will be generated, `MyFilter.c` and `MyFilter.h`.
 * Including ``MyFilter.h`` provides access to 3 functions, ``MyFilter_init()``, 
 * `MyFilter_add_sample()`, and `MyFilter()` which correspond to the library functions
 * `filter_fir_s32_init()`, `filter_fir_s32_add_sample()` and `filter_fir_s32()` 
 * respectively.
 * 
 * Use the `--help` flag with the scripts for more detailed descriptions of inputs and other 
 * options.
 * 
 * |  Filter Type   | Script                                           |
 * |  -----------   | ------                                           |
 * | 32-bit FIR     | `lib_xcore_math/script/gen_fir_filter_s32.py`    |
 * | 16-bit FIR     | `lib_xcore_math/script/gen_fir_filter_s16.py`    |
 * | 32-bit Biquad  | `lib_xcore_math/script/gen_biquad_filter_s32.py` |
 * 
 */