
Notes for lib_xcore_math                          {#notes}
========================

## &nbsp;

## Vector Alignment ##                        {#vector_alignment}

This library makes use of the XMOS architecture's vector processing unit (VPU). In the XS3 version
of the architecture, all loads and stores stores to and from the XS3 VPU have the requirement that
the loaded/stored addresses must be aligned to a 4-byte boundary (word-aligned).

In the current version of the API, this leads to the requirement that most API functions require
vectors (or the data backing a BFP vector) to begin at word-aligned addresses. Vectors are *not*
required, however, to have a size (in bytes) that is a multiple of 4.

Some functions also make use of instructures which require data to be 8-byte-aligned.

### Writing Alignment-safe Code ###

The alignment requirement is ultimately always on the data that backs a vector. This applies to all
but the scalar API. For the BFP API, this applies to the memory to which the `data` field (or the
`real` and `imag` fields in the case of `bfp_complex_s16_t`) points, specified when the BFP vector
is initialized. A similar constraint applies when initializing filters. For the other APIs, this
will apply to the pointers that get passed into the API functions.

Arrays of type `int32_t` and `complex_s32_t` will normally be guaranteed to be word-aligned by the
compiler. However, if the user manually specifies the beginning of an `int32_t` array, as in the
following..

\code{.c}
    uint8_t byte_buffer[100];
    int32_t* integer_array = (int32_t*) &byte_buffer[1];
\endcode

.. the vector may not be word-aligned. It is the responsibility of the user to ensure proper
alignment of data.

For `int16_t` arrays, the compiler does not by default guarantee that the array starts on a
word-aligned address. To force word-alignment on arrays of this type, use 
`__attribute__((aligned (4)))` in the variable definition, as in the following.

\code{.c}
    int16_t __attribute__((aligned (4))) data[100];
\endcode

Occasionally, 8-byte (double word) alignment is required. In this case, neither `int32_t` nor
`int16_t` is necessarily guaranteed to align as required. Similar to the above, this can be hinted
to the compiler as in the following.

\code{.c}
    int32_t __attribute__((aligned (8))) data[100];
\endcode

This library also provides the macros `WORD_ALIGNED` and `DWORD_ALIGNED` which force 4- and 8-byte
alignment respectively as above.

---------
## Symmetrically Saturating Arithmetic ##     {#saturation}

With ordinary integer arithmetic the block floating-point logic chooses exponents and operand shifts
to prevent integer overflow with worst-case input values. However, the XS3 VPU uses symmetrically
saturating integer arithmetic.

Saturating arithmetic is that where partial results of the applied operation use a bit depth greater
than the output bit depth, and values that can't be properly expressed with the output bit depth are
set to the nearest expressible value. 

For example, in ordinary C integer arithmetic, a function which multiplies two 32-bit integers may
internally compute the full 64-bit product and then clamp values to the range `(INT32_MIN,
INT32_MAX)` before returning a 32-bit result.

Symmetrically saturating arithmetic also includes the property that the lower bound of the
expressible range is the negative of the upper bound of the expressible range.

One of the major troubles with non-saturating integer arithmetic is that in a twos complement
encoding, there exists a non-zero integer (e.g. INT16_MIN in 16-bit twos complement arithmetic)
value @f$x@f$ for which  @f$-1 \cdot x = x@f$. Serious arithmetic errors can result when this case
is not accounted for.

One of the results of _symmetric_ saturation, on the other hand, is that there is a corner case
where (using the same exponent and shift logic as non-saturating arithmetic) saturation may occur
for a particular combination of input mantissas. The corner case is different for different
operations.

When the corner case occurs, the minimum (and largest magnitude) value of the resulting vector is 1
LSb greater than its ideal value (e.g. `-0x3FFF` instead of `-0x4000` for 16-bit arithmetic). The
error in this output element's mantissa is then 1 LSb, or @f$2^p@f$, where @f$p@f$ is the exponent
of the resulting BFP vector.

Of course, the very nature of BFP arithmetic routinely involves errors of this magnitude.

---------
## Spectrum Packing ##              {#spectrum_packing}

In its general form, the @math{N}-point Discrete Fourier Transform is an operation applied to a
complex @math{N}-point signal @math{x[n]} to produce a complex spectrum @math{X[f]}. Any spectrum
@math{X[f]} which is the result of a @math{N}-point DFT has the property that @math{X[f+N] = X[f]}.
Thus, the complete representation of the @math{N}-point DFT of @math{X[n]} requires @math{N} complex
elements.

### Complex DFT and IDFT ###

In this library, when performing a complex DFT (e.g. using fft_bfp_forward_complex()), the spectral
representation that results in a straight-forward mapping:

`X[f]` @math{\longleftarrow X[f]} for @math{0 \le f < N}

where `X` is an @math{N}-element array of `complex_s32_t`, where the real part of @math{X[f]} is in
`X[f].re` and the imaginary part in `X[f].im`.

Likewise, when performing an @math{N}-point complex inverse DFT, that is also the representation
that is expected.

### Real DFT and IDFT ###

Oftentimes we instead wish to compute the DFT of real signals. In addition to the periodicity
property (@math{X[f+N] = X[f]}), the DFT of a real signal also has a complex conjugate symmetry such
that @math{X[-f] = X^*[f]}, where @math{X^*[f]} is the complex conjugate of @math{X[f]}. This
symmetry makes it redundant (and thus undesirable) to store such symmetric pairs of elements. This
would allow us to get away with only explicitly storing @math{X[f} for @math{0 \le f \le N/2} in
@math{(N/2)+1} complex elements.

Unfortunately, using such a representation has the undesirable property that the DFT of an
@math{N}-point real signal cannot be computed in-place, as the representation requires more memory
than we started with.

However, if we take the periodicity and complex conjugate symmetry properties together:

\f[
    X[0] = X^*[0] \rightarrow Imag\{X[0]\} = 0 \\

    X[-(N/2) + N] = X[N/2] \\

    X[-N/2] = X^*[N/2] \rightarrow X[N/2] = X^*[N/2] \rightarrow Imag \{ X[N/2] \} = 0
\f]

Because both @math{X[0]} and @math{X[N/2]} are guaranteed to be real, we can recover the benefit of
in-place computation in our representation by packing the real part of @math{X[N/2]} into the
imaginary part of @math{X[0]}.

Therefore, the functions in this library that produce the spectra of real signals (such as
fft_bfp_forward_mono() and fft_bfp_forward_stereo()) will pack the spectra in a slightly less
straight-forward manner (as compared with the complex DFTs):


`X[f]` @math{\longleftarrow X[f]} for @math{1 \le f < N/2}

`X[0]` @math{\longleftarrow X[0] + j X[N/2]}

where `X` is an @math{N/2}-element array of `complex_s32_t`.

Likewise, this is the encoding expected when computing the @math{N}-point inverse DFT, such as by
fft_bfp_inverse_mono() or fft_bfp_inverse_stereo().

@note One additional note, when performing a stereo DFT or inverse DFT, so as to preserve the
in-place computation of the result, the spectra of the two signals will be encoded into adjacent
blocks of memory, with the second spectrum (i.e. associated with 'channel b') occupying the higher
memory address.