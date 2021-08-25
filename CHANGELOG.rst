lib_xs3_math change log
=======================

2.0.0 (UPCOMING)
----------------

Major Changes
*************

* Support for channel-pair related types and operations has been dropped. These were considered to be too narrowly focused on making use of a single optimization (stereo FFT).

Bugfixes
********

* Fixed bug in `bfp_fft_inverse_stereo()` where length of output BFP vector was half of correct length.

New Functions
*************
* BFP API

  * FFT spectrum unpacking

    * `bfp_fft_unpack_mono()` -- Used to expand the output spectrum from `bfp_fft_forward_mono()` from `FFT_N/2` elements (with the Nyquist component packed into the DC component) to `FFT_N/2 + 1` elements. This is useful as many complex operations behave undesirably on the packed representation.
    * `bfp_fft_pack_mono()` -- Opposite of `bfp_fft_unpack_mono()`. Used to repack the spectrum into a form suitable for calling `bfp_fft_inverse_mono()`.
  
  * Dynamic BFP vector allocation
  
    * Functions for allocating and deallocating BFP vectors dynamically from the heap.
    * `bfp_sXX_alloc()`, `bfp_complex_sXX_alloc()`
    * `bfp_sXX_dealloc()`, `bfp_complex_sXX_dealloc()`

  * Multiply-accumulate functions
    
    * A handful of element-wise multiply-accumulate functions have been added for both 16-bit and 32-bit, and both real and complex vector types. e.g...
    
    * `bfp_sXX_macc()` -- Element-wise multiply accumulate for real 16/32-bit vectors
    * `bfp_sXX_nmacc()` -- Element-wise negated multiply accumulate (i.e. multiply-subtract) for real vectors
    * `bfp_complex_sXX_macc()` -- Element-wise multiply accumulate for complex vectors.
    * `bfp_complex_sXX_conj_macc()` -- Element-wise conjugate multiply accumulate for complex vectors.
    * (and various others)
  
  * `bfp_complex_sXX_conjugate()` -- Get the complex conjugate of a vector
  * `bfp_complex_sXX_energy()` -- Compute the sum of a complex vector's elements' squared magnitudes.
    

* Low-level API
  
  * `xs3_mat_mul_s8_x_s8_yield_s32()` -- Multiply-accumulate an 8-bit vector by an 8-bit matrix into 32-bit accumulators.
  * `xs3_mat_mul_s8_x_s16_yield_s32()` -- Multiply a 16-bit vector by an 8-bit matrix for a 32-bit result.
  * `xs3_vect_s8_is_negative()` -- Determine whether each element of an 8-bit vector is negative.
  * `xs3_vect_s16_extract_high_byte()` -- Extract the most significant byte of each element of a 16-bit vector.
  * `xs3_vect_s16_extract_low_byte()` -- Extract the least significant byte of each element of a 16-bit vector.
  * `xs3_vect_s32_zip()` -- Interleave elements from two `int32_t` vectors.
  * `xs3_vect_s32_unzip()` -- De-interleave elements from a `int32_t` vector.
  * `xs3_vect_s32_copy()` -- Copy an `int32_t` vector.
  * Various low-level functions used in the implementation of the high-level multiply-accumulate functions (e.g. `xs3_vect_s32_macc()`).

Miscellaneous
*************

* Unit tests have been refactored to make use of Unity fixtures.
* Added example apps: `vect_demo`, `bfp_demo`, `fft_demo` and `filter_demo`
* Removed configuration support for `XS3_MATH_VECTOR_TAIL_SUPPORT`
* Added python scripts to generate code for filters

  * `lib_xs3_math/script/gen_fir_filter_s16.py`
  * `lib_xs3_math/script/gen_fir_filter_s32.py`
  * `lib_xs3_math/script/gen_biquad_filter_s32.py`

* Changed low-level API so that each function `foo()` that has an associated 'prepare' function (to calculate shifts or output exponents) can be prepared with `foo_prepare()`. This makes the low-level API more consistent.
* Separated filtering-related unit tests into a separate unit test application.
* Various improvements to CMake project files.

  * Includes automatic fetching of Unity repository during build

  

1.0.0
-----

  * Initial version
