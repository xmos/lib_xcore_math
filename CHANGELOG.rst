lib_xcore_math change log
=========================

2.1.3
-----

* Fixes bug (issue #147) in `s16_to_s32()`.
* Fixes bug (issue #146) in `bfp_s32_macc()` and `bfp_s32_nmacc()`.
* Fixes bug with the `vect_s32_prepare_api` not appearing in the documentation.
* Fixes bug in `bfp_s32_mean()` and `bfp_s16_mean()` when hitting a corner case scenario.
* Cleans up internal functions.
* Allows compiling and running demos and tests on Windows Native x86 platforms.
* Removes several warnings.

2.1.2
-----

* Optimization fix (issue #128) for `filter_fir_s32()`
* Documentation improvements.

2.1.1
-----

* Fixes bug (issue #116) in `vect_packed_complex_s32_macc()`.
* Fixes bug (issue #119) in `filter_fir_s32()`.
* Adds `--scale` option to the filter conversion script `gen_biquad_filter_s32.py`

  * If internal filter states (outputs from internal biquad sections) grow too large
  integer overflows may occur. Using the `--scale` option can help avoid this by
  effectively applying a gain factor to all coefficients.

* Fixes bug (mentioned in issue #119) in the `gen_fir_filter_s32.py` and
`gen_fir_filter_s16.py` filter conversion scripts where in a certain corner case
filter coefficients can overflow.

2.1.0
-----

* Adds several new operations for IEEE float vectors.
* Corrects `module_build_info` for legacy build tools.
* Fixes potential issue with include paths.

2.0.2
-----

* Updated CMake configuration to support Darwin platform.

2.0.1
-----

* Bugfix: Fixed issue with including ``xmath/xmath.h`` from XC files.
* Doc Update: Corrected instructions for configuring CMake using XS3 toolchain.


2.0.0
-----

Major, backwards compatibility-breaking update to the library.

Background
**********

This update does not add new features (compared to 1.1.0) to the library.  Instead, it is a
major refactoring of the library in a few different ways.  The overarching purpose of this
refactoring is to generalize the library so that when any future xcore ISAs are released, support
for those architectures can be included within this library in a minimally disruptive way.  The
xcore XS3 architecture currently remains the only xcore ISA officially supported by the library.

Major Changes
*************

As mentioned, the update from 1.1.0 to 2.0.0 is does not add, remove or functionally change
supported features.  Rather, the changes are primarily related to organization and the conventions
used.  However, these changes do break backwards compatibility; hence the major version increment.

The relevant major changes are:

* **Library name has changed from `lib_xs3_math` to `lib_xcore_math`**

  * This is to reflect that this library is not intended to be deprecated when future xcore ISA
    versions are released.

* **Library re-organized into multiple sub-APIs**

  * The organization of the library into the 'high-level' (BFP) and 'low-level' (non-BFP) APIs has
    grown cumbersome and no longer cleanly fits the content of the library. To this end, the
    operations made available by this library have been re-organized into several sub-APIs. These
    APIs are all versioned together and are in many cases interdependent. This new grouping is
    primarily for conceptual simplicity.

    * BFP API -- Mostly unchanged from the previous high-level API.
    * Vector API -- Corresponds to most of the functionality in the previous low-level API, less the
      operations that were moved to the new APIs.
    * Filtering API -- Operations related to supported linear filters (FIR and Biquad).
    * FFT API -- Collects the FFT-related functions (both low-level and high-level) from the
      previous API and groups them together.
    * DCT API -- Like the FFT API, collected DCT-related functions.
    * Scalar API -- A small API for implementations of scalar operations, with particular emphasis
      on the non-IEEE 754 floating-point scalars provided (but also includes some ``float``
      operations).

* **Many functions renamed**

  * Previous versions of this library used a function naming convention whereby the majority of
    low-level functions (and some types) had names prefixed with ``xs3_``, even where those
    functions were written in C (rather than XS3 assembly) and were not intimately related to the
    specifics of the XS3 hardware (i.e. where the same C implementation could plausibly be used in
    future ISA versions).
  * To that end, most functions prefixed with ``xs3_`` have had that prefix removed. This way, when
    a future ISA is released (and once support is added to this library), applications can be
    retargeted at the newer architecture without the unnecessary effort of going through the code to
    rename all the function calls.

    * e.g.  ``xs3_vect_s32_mul() --> vect_s32_mul()``
    * Note that most BFP function names remain unchanged.

  * The intention going forward is that the public API should avoid ISA version-specific naming when
    the object being named is not conceptually specific to a particular ISA (except possibly where
    optimizing different ISA versions necessitates mutually incompatible implementations).


1.1.0
-----

Major Changes
*************

* Support for channel-pair related types and operations has been dropped. These were considered to
  be too narrowly focused on making use of a single optimization (stereo FFT).

  * This is a backwards compatibility-breaking change, requiring a major version increment.

* Added various scalar arithmetic functions for `float_s32_t` type.

* Adds Discrete Cosine Transform API

* Adds various trig and exponential functions.

Bugfixes
********

* Fixed bug in `bfp_fft_inverse_stereo()` where length of output BFP vector was half of correct
  length.

New Functions
*************
* BFP API

  * FFT spectrum unpacking

    * `bfp_fft_unpack_mono()` -- Used to expand the output spectrum from `bfp_fft_forward_mono()`
      from `FFT_N/2` elements (with the Nyquist component packed into the DC component) to
      `FFT_N/2 + 1` elements. This is useful as many complex operations behave undesirably on the
      packed representation.
    * `bfp_fft_pack_mono()` -- Opposite of `bfp_fft_unpack_mono()`. Used to repack the spectrum into
      a form suitable for calling `bfp_fft_inverse_mono()`.
  
  * Dynamic BFP vector allocation
  
    * Functions for allocating and deallocating BFP vectors dynamically from the heap.
    * `bfp_sXX_alloc()`, `bfp_complex_sXX_alloc()`
    * `bfp_sXX_dealloc()`, `bfp_complex_sXX_dealloc()`

  * Multiply-accumulate functions
    
    * A handful of element-wise multiply-accumulate functions have been added for both 16-bit and
      32-bit, and both real and complex vector types. e.g...
    
    * `bfp_sXX_macc()` -- Element-wise multiply accumulate for real 16/32-bit vectors
    * `bfp_sXX_nmacc()` -- Element-wise negated multiply accumulate (i.e. multiply-subtract) for
      real vectors
    * `bfp_complex_sXX_macc()` -- Element-wise multiply accumulate for complex vectors.
    * `bfp_complex_sXX_conj_macc()` -- Element-wise conjugate multiply accumulate for complex
      vectors.
    * (and various others)
  
  * `bfp_complex_sXX_conjugate()` -- Get the complex conjugate of a vector
  * `bfp_complex_sXX_energy()` -- Compute the sum of a complex vector's elements' squared
    magnitudes.
  * `bfp_sXX_use_exponent()` / `bfp_complex_sXX_use_exponent()` -- Force BFP vector to encode
    mantissas using specified exponent (i.e. convert to specified Q-format)
  * `bfp_s32_convolve_valid()` / `bfp_complex_s32_convolve_same()` -- Filter a 32-bit signal using a
    short convolution kernel. Both "valid" and "same" padding modes are supported.
  * `xs3_vect_sXX_add_scalar()` / `xs3_vect_complex_sXX_add_scalar()` -- Functions to add scalar to
    a vector (16/32-bit real/complex)
    

* Vector API
  
  * Functions supporting mixed-depth operations

    * `xs3_mat_mul_s8_x_s8_yield_s32()` -- Multiply-accumulate an 8-bit vector by an 8-bit matrix
      into 32-bit accumulators.
    * `xs3_mat_mul_s8_x_s16_yield_s32()` -- Multiply a 16-bit vector by an 8-bit matrix for a 32-bit
      result.
    * `xs3_vect_s8_is_negative()` -- Determine whether each element of an 8-bit vector is negative.
    * `xs3_vect_s16_extract_high_byte()` -- Extract the most significant byte of each element of a
      16-bit vector.
    * `xs3_vect_s16_extract_low_byte()` -- Extract the least significant byte of each element of a
      16-bit vector.

  * Memory ops

    * `xs3_vect_s32_zip()` -- Interleave elements from two `int32_t` vectors.
    * `xs3_vect_s32_unzip()` -- De-interleave elements from a `int32_t` vector.
    * `xs3_vect_s32_copy()` -- Copy an `int32_t` vector.
    * `xs3_memcpy()` -- Quickly copy word-aligned vector to another word-aligned vector.
  * Various low-level functions used in the implementation of the high-level multiply-accumulate
    functions (e.g. `xs3_vect_s32_macc()`).
  * `xs2_vect_s32_convolve_valid()` / `xs3_vect_complex_s32_convolve_same()` -- Filter a 32-bit
    signal using a short convolution kernel. Both "valid" and "same" padding modes are supported.
  * `xs3_vect_sXX_add_scalar()` / `xs3_vect_complex_sXX_add_scalar()` -- Add a scalar to a 16- or
    32-bit real or complex vector.

  * IEEE754 single-precision float vector functions

    * `xs3_vect_f32_fft_forward()` / `xs3_vect_f32_fft_inverse()` -- Forward/Inverse FFT functions
      for vectors of floats.
    * `xs3_vect_f32_max_exponent()` -- Get maximum exponent from vector of floats.
    * `xs3_vect_f32_to_s32()` / `xs3_vect_s32_to_f32()` -- Convert between float vector and BFP
      vector.
    * `xs3_vect_f32_dot()` -- Inner product between two float vectors.

  * `xs3_vect_sXX_max_elementwise()` / `xs3_vect_sXX_min_elementwise()` -- Element-wise maximum and
    minimum between two 16-/32-bit vectors.

* DCT API

  * `dctXX_forward()` / `dctXX_inverse()` -- Forward (type-II) and inverse (type-III) `XX`-point DCT
    implementations.
  
    * Current sizes supported are `6`, `8`, `12`, `16`, `24`, `32`, `48` and `64`

  * `dct8x8_forward()` / `dct8x8_inverse()` -- Fast 2D 8-by-8 forward and inverse DCTs.


Miscellaneous
*************

* Unit tests have been refactored to make use of Unity fixtures.
* Added example apps: `vect_demo`, `bfp_demo`, `fft_demo` and `filter_demo`
* Removed configuration support for `XS3_MATH_VECTOR_TAIL_SUPPORT`
* Added `QXX()` and `FXX()` macros (e.g. `Q24()`; taken from `lib_dsp`) for converting (constants)
  between floating-point and fixed-point values.
* Added python scripts to generate code for filters

  * `lib_xs3_math/script/gen_fir_filter_s16.py`
  * `lib_xs3_math/script/gen_fir_filter_s32.py`
  * `lib_xs3_math/script/gen_biquad_filter_s32.py`

* Changed low-level API so that each function `foo()` that has an associated 'prepare' function (to
  calculate shifts or output exponents) can be prepared with `foo_prepare()`. This makes the
  low-level API more consistent.
* Separated filtering-related unit tests into a separate unit test application.
* Various improvements to CMake project files.

  * Includes automatic fetching of Unity repository during build

  

1.0.0
-----

  * Initial version
