
Notes for lib_xs3_math                          {#notes}
======================

### Note 1: Vector Alignment ###                {#vector_alignment}

This library utilizes the XMOS XS3 architecture's vector processing unit (VPU). All loads and stores to and from the XS3 VPU have the requirement that the loaded/stored addresses must be aligned to a 4-byte boundary (word-aligned).

In the current version of the API, this leads to the requirement that most API functions require vectors (or the data backing a BFP vector) to begin at word-aligned addresses. Vectors are *not* required, however, to have a size (in bytes) that is a multiple of 4.

#### Writing Alignment-safe Code ####

The alignment requirement is ultimately always on the data that backs a vector. For the low-level API, that is the arrays passed to the functions themselves. For the high-level API, that is the memory to which the `data` field (or the `real` and `imag` fields in the case of `bfp_complex_s16_t`) points, specified when the BFP vector is initialized.

Arrays of type `int32_t` and `complex_s32_t` will normally be guaranteed to be word-aligned by the compiler. However, if the user manually specifies the beginning of an `int32_t` array, as in the following..

\code{.c}
    uint8_t byte_buffer[100];
    int32_t* integer_array = (int32_t*) &byte_buffer[1];
\endcode

.. it is the responsibility of the user to ensure proper alignment of data.

For `int16_t` arrays, the compiler does not by default guarantee that the array starts on a word-aligned address. To
force word-alignment on arrays of this type, use `__attribute__((align 4))` in the variable definition, as in the following.

\code{.c}
    int16_t __attribute__((align 4)) data[100];
\endcode

---------
### Note 2: Symmetrically Saturating Arithmetic ###     {#saturation}

With ordinary integer arithmetic the block floating-point logic chooses exponents and operand shifts to prevent integer overflow with worst-case input values. However, the XS3 VPU utilizes symetrically saturating integer arithmetic.

Saturating arithmetic is that where partial results of the applied operation use a bit depth greater than the output bit depth, and values that can't be properly expressed with the output bit depth are set to the nearest expressible value. 

For example, in ordinary C integer arithmetic, a function which multiplies two 32-bit integers may internally compute the full 64-bit product and then clamp values to the range `(INT32_MIN, INT32_MAX)` before returning a 32-bit result.

Symmetrically saturating arithmetic also includes the property that the lower bound of the expressible range is the negative of the upper bound of the expressible range.

One of the major troubles with non-saturating integer arithmetic is that in a twos complement encoding, there exists an integer (e.g. INT16_MIN in 16-bit twos complement arithmetic) value @f$x@f$ for which  @f$-1 \cdot x = x@f$. Serious arithmetic errors can result when this case is not accounted for.

One of the results of _symmetric_ saturation, on the other hand, is that there is a corner case where (using the same exponent and shift logic as non-saturating arithmetic) saturation may occur for a particular combination of input mantissas. The corner case is different for different operations.

When the corner case occurs, the minimum (and largest magnitude) value of the resulting vector is 1 LSb greater than its ideal value (e.g. `-0x3FFF` instead of `-0x4000` for 16-bit arithmetic). The error in this output element's mantissa is then 1 LSb, or @f$2^p@f$, where @f$p@f$ is the exponent of the resulting BFP vector.

Of course, the very nature of BFP arithmetic routinely involves errors of this magnitude. However, this particular case may have pitfalls that don't occur when using asymmetric BFP arithmetic.

The solution to this problem is to detect the possibility of this corner case and adjust parameters to avoid it when it is possible. To that end, this library has a compile time option, `XS3_BFP_ALLOW_SATURATION` (which defaults to `0`) to specify whether the BFP API functions should globally detect and avoid this corner case. 

If there are specific operations where a different local behavior is desired, most of the BFP operations have a function (e.g. `bfp_add_vect_prepare()` for vector addition) which can be used in combination with the low-level API to specify local behavior at run-time. The low-level API requires the user to manually specify the shift values used as its operations are carried out, and is thus agnostic to (and has no visibility of) the setting for `XS3_BFP_ALLOW_SATURATION`.

Note that the logic which regards `XS3_BFP_ALLOW_SATURATION` only checks for the _possibility_ of reaching the saturating corner case, rather than whether it will _actually_ occur. Detecting the actuality of this corner case is much more expensive.

The down side to disallowing corner-case saturation is that whenever the possibility of saturation is detected the effective bit depth of the resulting BFP vector is reduced by 1 bit. How frequently the possibility of saturation is detected depends in general both on the operation being performed and on the actual logical content of the vectors. For example, with BFP vector addition, adding vectors which are unlikely to be of the same magnitude (as determined by the value represented by their respective largest magnitude elements) detection of the saturation condition will be infrequent.

---------
### Note 3: Arithmetic Undeflow on XS3 ###              {#arithmetic_underflow}

Due to the nature of block floating-point arithmetic, underflow of data is generally possible. Underflow occurs when the logical value of a vector element is smaller (in magnitude) than one mantissa LSb, as dictated by the BFP vector's exponent.

With the XS3 VPU, the result of an underflowing value depends on whether the value was negative or not. Non-negative values underflow to `0`, whereas negative values underflow to `-1`. Users should take steps to ensure that the logic of their particular use case is not sensitive to either case.

Precisely where underflow may occur depends on the operation being performed. Refer to the operation descriptions in the low-level API (e.g. `xs3_bfp_low.h`) for details on where underflows may occur. Typically they can occur whereever a multiplication by `2` to a negative power may happen.

---------
### Note 4: Spectrum Packing ###              {#spectrum_packing}

In its general form, the @math{N}-point Discrete Fourier Transform is an operation applied to a complex @math{N}-point 
signal @math{x[n]} to produce a complex spectrum @math{X[f]}. Any spectrum @math{X[f]} which is the result of a 
@math{N}-point DFT has the property that @math{X[f+N] = X[f]}. Thus, the complete representation of the @math{N}-point 
DFT of @math{X[n]} requires @math{N} complex elements.

#### Complex DFT and IDFT ####

In this library, when performing a complex DFT (e.g. using bfp_fft_forward_complex()), the spectral representation that
results in a straight-forward mapping:

`X[f]` @math{\longleftarrow X[f]} for @math{0 \le f \lt N}

where `X` is an @math{N}-element array of `complex_s32_t`, where the real part of @math{X[f]} is in `X[f].re` and the imaginary part in `X[f].im`.

Likewise, when performing an @math{N}-point complex inverse DFT, that is also the representation that is expected.

#### Real DFT and IDFT ####

Oftentimes we instead wish to compute the DFT of real signals. In addition to the periodicity property 
(@math{X[f+N] = X[f]}), the DFT of a real signal also has a complex conjugate symmetry such that @math{X[-f] = X^*[f]},
where @math{X^*[f]} is the complex conjugate of @math{X[f]}. This symmetry makes it redundant (and thus undesirable) to
store such symmetric pairs of elements. This would allow us to get away with only explicitly storing @math{X[f} for 
@math{0 \le f \le N/2} in @math{(N/2)+1} complex elements.

Unfortunately, using such a representation has the undesirable property that the DFT of an @math{N}-point real signal
cannot be computed in-place, as the representation requires more memory than we started with.

However, if we take the periodicity and complex conjugate symmetry properties together:

\f[
    X[0] = X^*[0] \rightarrow Imag\{X[0]\} = 0 \\

    X[-(N/2) + N] = X[N/2] \\

    X[-N/2] = X^*[N/2] \rightarrow X[N/2] = X^*[N/2] \rightarrow Imag \{ X[N/2] \} = 0
\f]

Because both @math{X[0]} and @math{X[N/2]} are guaranteed to be real, we can recover the benefit of in-place computation
in our representation by packing the real part of @math{X[N/2]} into the imaginary part of @math{X[0]}.

Therefore, the functions in this library that produce the spectra of real signals (such as bfp_fft_forward_mono() and 
bfp_fft_forward_stereo()) will pack the spectra in a slightly less straight-forward manner (as compared with the complex DFTs):


`X[f]` @math{\longleftarrow X[f]} for @math{1 \le f \lt N/2}

`X[0]` @math{\longleftarrow X[0] + j X[N/2]}

where `X` is an @math{N/2}-element array of `complex_s32_t`.

Likewise, this is the encoding expected when computing the @math{N}-point inverse DFT, such as by bfp_fft_inverse_mono()
or bfp_fft_inverse_stereo().

@note One additional note, when performing a stereo DFT or inverse DFT, so as to preserve the in-place computation of 
the result, the spectra of the two signals will be encoded into adjacent blocks of memory, with the second spectrum
(i.e. associated with 'channel b') occupying the higher memory address.