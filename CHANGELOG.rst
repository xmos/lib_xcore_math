lib_xs3_math change log
=======================

1.1.0 (UPCOMING)
----------------


Bugfixes
********

* Fixed bug in `bfp_fft_inverse_stereo()` where length of output BFP vector was half of correct length.

New Functions
*************
* BFP API

  * `bfp_fft_unpack_mono()` -- Used to expand the output spectrum from `bfp_fft_forward_mono()` from `FFT_N/2` elements (with the Nyquist component packed into the DC component) to `FFT_N/2 + 1` elements. This is useful as many complex operations behave undesirably on the packed representation.
  * `bfp_fft_pack_mono()` -- Opposite of `bfp_fft_unpack_mono()`. Used to repack the spectrum into a form suitable for calling `bfp_fft_inverse_mono()`.
  * `bfp_fft_unpack_stereo()` -- Same as `bfp_fft_unpack_mono()`, except used on output of `bfp_fft_forward_stereo()`.
  * `bfp_fft_pack_stereo()` -- Opposite of `bfp_fft_unpack_stereo()`. Used to repack the spectrum into a form suitable for calling `bfp_fft_inverse_stereo()`.

* Low-level API
  
  * `xs3_mat_mul_s8_x_s8_yield_s32()` -- Multiply-accumulate an 8-bit vector by an 8-bit matrix into 32-bit accumulators.
  * `xs3_mat_mul_s8_x_s16_yield_s32()` -- Multiply a 16-bit vector by an 8-bit matrix for a 32-bit result.
  * `xs3_vect_s8_is_negative()` -- Determine whether each element of an 8-bit vector is negative.
  * `xs3_vect_s16_extract_high_byte()` -- Extract the most significant byte of each element of a 16-bit vector.
  * `xs3_vect_s16_extract_low_byte()` -- Extract the least significant byte of each element of a 16-bit vector.

Miscellaneous
*************

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
