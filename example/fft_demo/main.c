// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __XS3A__
# include <xscope.h>
#endif

#include "xmath/xmath.h"

void fft_mono_example();
void fft_stereo_example();
void fft_complex_example();

#define RAND_SEED   3457345734

int main(int argc, char** argv)
{
#ifdef __XS3A__
  xscope_config_io(XSCOPE_IO_BASIC);
#endif

  // Seed the random number generator, using a constant for reproducibility
  srand(RAND_SEED);

  fft_mono_example();
  fft_stereo_example();
  fft_complex_example();

  return 0;
}


// The length (in elements) of our BFP vectors.
#define LENGTH    (128)


void fft_mono_example()
{

  printf("\n\n\n");
  printf("################\n");
  printf("### Mono FFT ###\n");
  printf("################\n\n");

  /*
    This function demonstrates how apply the forward and inverse FFT to a single channel of
    real data.  This is accomplished using the bfp_fft_forward_mono() and bfp_fft_inverse_mono()
    functions respectively.
 
    bfp_s32_t         --bfp_fft_forward_mono()--> bfp_complex_s32_t
    bfp_complex_s32_t --bfp_fft_inverse_mono()--> bfp_s32_t
  */
  
  // bfp_fft_forward_mono() requires an input BFP vector of type bfp_s32_t, whose mantissa vector
  // is backed by an int32_t array.
  // The +2 below is so spectrum unpacking can be demonstrated (more on this below).
  int32_t buffer[LENGTH+2];
  
  // We'll just fill that vector with random mantissas.
  for(int k = 0; k < LENGTH; k++){
    buffer[k] = rand() << 1;
  }
  
  // Before doing the forward FFT the input vector needs to be initialized. In many situations there
  // is no obvious natural exponent to associate with the time-domain signal (such as PCM audio
  // streams). In such a situation, it is often convenient to select an exponent that normalizes the
  // data to a known range, such as [-1.0, 1.0). For 32-bit signed data, an exponent of -31 does
  // that.
  // Note that the final parameter is instructing the init function to compute the headroom of the
  // input vector. If we instead chose to fill buffer[] with random data *after* initializing x,
  // there would be no point to computing the headroom here, as it is invalidated the moment we
  // modify x.data[].
  bfp_s32_t x;
  bfp_s32_init(&x, buffer, -31, LENGTH, 1);

  // Print out the floating point equivalent values of the input vector's elements prior to applying
  // the FFT.
  printf("x = [");
  for(unsigned int k = 0; k < x.length; k++)
    printf("%0.04f, ", ldexp(x.data[k], x.exp));
  printf("]\n\n");

  // bfp_fft_forward_mono() operates on data in-place. We'll print out the buffer address before
  // and after the transformation to convince ourselves of this.
#ifdef __xcore__
  printf("&x.data[0] --> 0x%08X\n", (unsigned) &x.data[0]);
#endif
  printf("x.length --> %u\n\n", x.length);


  // Apply the FFT.
  // This function takes a pointer to the input (time-domain, usually) BFP vector, and returns
  // a pointer *to the same address* recast to a complex BFP vector.
  bfp_complex_s32_t* X = bfp_fft_forward_mono(&x);

  /*
    There are two important things to be aware of with the mono FFT.

    1) The complex output vector X is only half the length of the input vector.
    2) The real part of the Nyquist rate component of the FFT is packed into the first element of X.

    The DFT of an N-element signal is periodic with period N. Additionally, the DFT of a real
    signal has a complex conjugate symmetry about angular frequency 0. Whereas a fully complex
    DFT requires N complex elements to fully specify the signal, the symmetry property of a
    real DFT means that only N/2 complex elements are needed.

    A consequence of the periodicity and conjugate symmetry properties taken together is that the
    components of the DFT at DC and at the Nyquist rate must have 0 imaginary part. Rather than
    require the user to supply an output vector with space allocated for N/2+1 complex elements,
    we opt to compute the DFT in-place, and pack the Nyquist rate component into the imaginary
    part of the DC component.

    If the user wishes, she can allocate space for 2 extra int32_t elements (same as 1 complex_s32_t
    element) in the time-domain buffer (but *not* identified in the length of the time-domain BFP
    vector, as that must be a power of 2), and unpack the Nyquist component after the FFT is applied
    (making sure to modify the frequency-domain BFP vectors length by adding 1). Note that
    bfp_fft_inverse_mono() however expects the data to be packed as bfp_fft_forward_mono() left it.

    Note also that the inverse mono FFT also assumes the conjugate symmetry of the frequency
    spectrum, meaning that the inverse transform is guaranteed to yield a purely real signal.
  */

#define UNPACK_SPECTRUM_MONO    1

  // Unpack the mono spectrum
  if(UNPACK_SPECTRUM_MONO) {
    bfp_fft_unpack_mono(X);
  }

  // Print out the buffer address and vector length of X, the frequency-domain signal
#ifdef __xcore__
  printf("&X->data[0] --> 0x%08X\n", (unsigned) &X->data[0]);
#endif
  printf("X->length --> %u\n\n", X->length);

  // Print out the complex BFP vector X
  printf("X = [");
  for(unsigned int k = 0; k < X->length; k++)
    printf("(%0.04f + %0.04fj), ", ldexp(X->data[k].re, X->exp), ldexp(X->data[k].im, X->exp) );
  printf("]\n\n");

  ///////////////////////////////
  // ...
  // Here you are free to do any required frequency-domain processing of the signal.
  // ...
  ///////////////////////////////

  // Re-pack the mono spectrum
  if(UNPACK_SPECTRUM_MONO) {
    bfp_fft_pack_mono(X);
  }

  // Apply the inverse FFT.
  // Like the forward mono FFT, this function returns a pointer, this time a bfp_s32_t*, which is
  // the function's input parameter recast. In this case we can ignore the returned value, as it
  // would simply point to x, which is already of the correct type and readily available. In other
  // situations (e.g. when x exists in a different scope) the returned value may be useful.
  bfp_fft_inverse_mono(X);

  // Finally, print out the inverse transformed signal, which should match the original signal to
  // within the arithmetic precision of the forward and inverse transform pair.
  printf("x = [");
  for(unsigned int k = 0; k < x.length; k++)
    printf("%0.04f, ", ldexp(x.data[k], x.exp));
  printf("]\n\n");


}


void fft_stereo_example()
{

  printf("\n\n\n");
  printf("##################\n");
  printf("### Stereo FFT ###\n");
  printf("##################\n\n");

  /*
    This function demonstrates how apply the forward and inverse FFT to a pair of channels, each
    containing a sequence of real sample data.  This is accomplished using the
    bfp_fft_forward_stereo() and bfp_fft_inverse_stereo() functions respectively.

    2 x bfp_s32_t     --bfp_fft_forward_stereo()--> 2 x bfp_complex_s32_t
    2 x bfp_complex_s32_t --bfp_fft_inverse_stereo()--> 2 x bfp_s32_t
  */
  
  // bfp_fft_forward_stereo() requires 2 input BFP vectors of type bfp_s32_t, whose  mantissa
  // vectors are backed by a int32_t arrays. The +1 is because we're going to do some unpacking of
  // the spectra after applying the FFT. See fft_mono_example() for unpacking explanation.
  // The DWORD_ALIGNED qualifier instructs the compiler to ensure the arrays start at an
  // 8-byte-aligned memory offset.
  int32_t DWORD_ALIGNED bufferA[LENGTH + 1];
  int32_t DWORD_ALIGNED bufferB[LENGTH + 1];

  // bfp_fft_forward_stereo() and bfp_fft_inverse_stereo() also require a scratch buffer with
  // element type complex_s32_t which is the same length (in elements) of the input vectors.
  complex_s32_t DWORD_ALIGNED scratch[LENGTH];
  
  // Fill in the buffers with random mantissas (left shift is to ensure some are negative).
  for(int k = 0; k < LENGTH; k++){
    bufferA[k] = rand() << 1;
    bufferB[k] = rand() << 1;
  }

  // Before doing the forward FFT the array needs to be turned into a proper BFP vector. So we
  // initialize a pair of bfp_s32_t structs.
  //
  // In many situations there is no obvious natural exponent to associate with the time-domain
  // signal (such as PCM audio streams). In such a situation, it is often convenient to select an
  // exponent that normalizes the data to a known range, such as [-1.0, 1.0). For 32-bit signed
  // data, an exponent of -31 does that.
  //
  // Note that the final parameter is instructing the init function to compute the headroom of the
  // input vector. If we instead chose to fill buffer[] with random data *after* initializing x,
  // there would be no point to computing the headroom here, as it is invalidated the moment we
  // modify a.data[] or b.data[].
  bfp_s32_t a, b;
  bfp_s32_init(&a, bufferA, -31, LENGTH, 1);
  bfp_s32_init(&b, bufferB, -31, LENGTH, 1);

  // Print out the floating point equivalent values of the input vectors' elements prior to applying
  // the FFT.
  printf("a = [");
  for(unsigned int k = 0; k < a.length; k++)
    printf("%0.04f, ", ldexp(a.data[k], a.exp));
  printf("]\n\n");

  printf("b = [");
  for(unsigned int k = 0; k < b.length; k++)
    printf("%0.04f, ", ldexp(b.data[k], b.exp));
  printf("]\n\n");

  // Apply the FFT.
  //
  // This function takes a pointer to the input BFP vectors and a scratch buffer
  bfp_fft_forward_stereo(&a, &b, scratch);

  // The two bfp_s32_t vectors containing the time-domain data have now been clobbered with the
  // frequency-domain data, and should not be directly accessed. Instead, we alias each bfp_s32_t to
  // a bfp_complex_s32_t and access the results through that.
  bfp_complex_s32_t* ChA = (bfp_complex_s32_t*) &a;
  bfp_complex_s32_t* ChB = (bfp_complex_s32_t*) &b;


  /*
    See the note above (in fft_mono_example()) about the properties of the real DFT that allow it to
    be computed in-place. Much of the same applies for the stereo DFT.

    It is another property of the DFT that the real and imaginary parts of the input domain are
    transformed in a way which allows two purely real signals to be DFTed simultaneously and their
    spectra to be fully separated afterwards. This is done by bfp_fft_forward_stereo().

    Like the mono FFT, each output spectrum (channels A and B) is represented by only N/2 complex
    elements. This again is allowed as a result of the assumption that the input signals are purely
    real. Also like the mono FFT, the real part of the Nyquist rate is packed into the imaginary
    part of the DC element for each of the spectra.

    The spectrum for each channel can be unpacked using bfp_fft_unpack_mono(), just as with
    bfp_fft_forward_mono().

    If much computation is to be done in the frequency domain this unpacking is likely to be
    relatively inexpensive and to reduce the likelihood of errors (e.g. complex multiplication of
    the packed spectrum will likely produce undesired effects).

    Before performing the inverse FFT, the signal must be re-packed with bfp_fft_pack_mono().

  */

#define UNPACK_SPECTRA_STEREO    1

  // Unpack the spectra
  if(UNPACK_SPECTRA_STEREO) {
    bfp_fft_unpack_mono(ChA);
    bfp_fft_unpack_mono(ChB);
  }


  // Print out the lengths of the channel A and channel B frequency spectra.
  printf("ChA.length --> %u\n\n", ChA->length);
  printf("ChB.length --> %u\n\n", ChB->length);

  // Print out the floating-point equivalent of the channel A and B frequency spectra.
  printf("ChA = [");
  for(unsigned int k = 0; k < ChA->length; k++)
    printf("(%0.04f + %0.04fj), ", ldexp(ChA->data[k].re, ChA->exp), ldexp(ChA->data[k].im, ChA->exp) );
  printf("]\n\n");

  printf("ChB = [");
  for(unsigned int k = 0; k < ChB->length; k++)
    printf("(%0.04f + %0.04fj), ", ldexp(ChB->data[k].re, ChB->exp), ldexp(ChB->data[k].im, ChB->exp) );
  printf("]\n\n");

  ///////////////////////////////
  // ...
  // Here you are free to do any required frequency-domain processing of the signal.
  // ...
  ///////////////////////////////

  
  // Repack the spectra
  if(UNPACK_SPECTRA_STEREO) {
    bfp_fft_pack_mono(ChA);
    bfp_fft_pack_mono(ChB);
  }

  // Apply the inverse FFT.
  // This function behaves much like the forward stereo FFT, with the input and output parameters
  // reversed.
  bfp_fft_inverse_stereo(ChA, ChB, scratch);

  // ChA and ChB should now be considered clobbered, and `a` and `b` can be used to access the
  // time-domain data.

  // Finally, print out the inverse transformed signal, which should match the original signal to
  // within the arithmetic precision of the forward and inverse transform pair.
  printf("a = [");
  for(unsigned int k = 0; k < a.length; k++)
    printf("%0.04f, ", ldexp(a.data[k], a.exp));
  printf("]\n\n");

  printf("b = [");
  for(unsigned int k = 0; k < b.length; k++)
    printf("%0.04f, ", ldexp(b.data[k], b.exp));
  printf("]\n\n");

}


void fft_complex_example()
{

  printf("\n\n\n");
  printf("###################\n");
  printf("### Complex FFT ###\n");
  printf("###################\n\n");
  
  /*
    This function demonstrates how apply the forward and inverse FFT to single channel containing
    containing a sequence of complex sample data.  This is accomplished using the
    bfp_fft_forward_complex() and bfp_fft_inverse_complex() functions respectively.

    bfp_complex_s32_t --bfp_fft_forward_complex()--> bfp_complex_s32_t
    bfp_complex_s32_t --bfp_fft_inverse_complex()--> bfp_complex_s32_t
  */

  // bfp_fft_forward_complex() requires an input BFP vector of type bfp_complex_s32_t, whose
  // mantissa vector is backed by a complex_s32_t array.
  // The complex_s32_t struct contains 2 fields, `re` for the real part and `im` for the
  // imaginary part of an element.  The buffer is then an alternating sequence of real and
  // imaginary parts for successive elements.
  complex_s32_t buffer[LENGTH];
  
  // Fill in the buffer with random mantissas (left shift is to ensure some are negative).
  for(int k = 0; k < LENGTH; k++){
    buffer[k].re = rand() << 1;
    buffer[k].im = rand() << 1;
  }

  // Before doing the forward FFT the array needs to be turned into a proper BFP vector. So we
  // initialize a bfp_complex_s32_t with bfp_complex_s32_init().
  // In many situations there is no obvious natural exponent to associate with the time-domain
  // signal. In such a situation, it is often convenient to select an exponent that normalizes
  // the data to a known range, such as [-1.0, 1.0). For 32-bit signed data, an exponent of -31
  // does that.
  // Alternatively, initializing with an exponent of 0 has the advantage that, after having
  // applied the inverse transform, the resulting exponent indicates the overall level of scaling
  // applied throughout the process.
  // Note that the final parameter is instructing the init function to compute the headroom of the
  // input vector. If we instead chose to fill buffer[] with random data *after* initializing x,
  // there would be no point to computing the headroom here, as it is invalidated the moment we
  // modify x.data[].
  bfp_complex_s32_t x;
  bfp_complex_s32_init(&x, buffer, -31, LENGTH, 1);

  // Print out the floating point equivalent values of the input vector's elements prior to applying
  // the FFT.
  printf("x = [");
  for(unsigned int k = 0; k < x.length; k++)
    printf("%0.04f + %0.04fj, ", ldexp(x.data[k].re, x.exp), ldexp(x.data[k].im, x.exp));
  printf("]\n\n");

  // bfp_fft_forward_complex() operates on data in-place. We'll print out the buffer address before
  // and after the transformation to convince ourselves of this.
#ifdef __xcore__
  printf("&x.data[0] --> 0x%08X\n", (unsigned) &x.data[0]);
#endif
  printf("x.length --> %u\n\n", x.length);

  // Apply the FFT.
  // Unlike the mono and stereo FFTs, the input and output vector types for the complex FFT are the
  // the same, so no additional structs or type casts are required.
  bfp_fft_forward_complex(&x);

  /*
    Unlike bfp_fft_forward_mono() and bfp_fft_forward_stereo(), the input signal for
    bfp_fft_forward_complex() is not purely real. As such, a full FFT_N complex elements are
    required to represent the spectrum.
    
    Additionally there is no packing of the Nyquist component's real part into the DC component's
    imaginary part.

    bfp_fft_forward_complex() can be used to perform a DFT on a single channel of purely real data
    by simply placing that signal in the real parts of a complex vector, with each element's
    imaginary part being zero. It's worth noting a few differences between doing this and just using
    bfp_fft_forward_mono().

    1) The complex input vector requires twice as much memory. For long FFTs, this may be
       appreciable.
    2) The cycles taken for bfp_fft_forward_mono() to compute an FFT_N-point DFT is approximately the
       same as the cycles taken to perform an FFT_N/2-point DFT using bfp_fft_forward_complex().
    3) The complex FFT does not result in a packed signal.
  */

  // ... howevr, we'll just create a new pointer to the spectrum to clarify when we're operating
  // in the time domain when we're in the frequency domain (`X` being frequency domain)
  bfp_complex_s32_t* X = &x;

  // Print out the address and length of the complex frequency spectrum
#ifdef __xcore__
  printf("&X->data[0] --> 0x%08X\n", (unsigned) &X->data[0]);
#endif
  printf("X->length --> %u\n\n", X->length);
  
  // Print out the floating-point equivalent of the complex frequency spectrum
  printf("X = [");
  for(unsigned int k = 0; k < X->length; k++)
    printf("(%0.04f + %0.04fj), ", ldexp(X->data[k].re, X->exp), ldexp(X->data[k].im, X->exp) );
  printf("]\n\n");

  ///////////////////////////////
  // ...
  // Here you are free to do any required frequency-domain processing of the signal.
  // ...
  ///////////////////////////////

  // Apply the inverse FFT
  bfp_fft_inverse_complex(X);

  // Finally, print out the inverse transformed signal, which should match the original signal to
  // within the arithmetic precision of the forward and inverse transform pair.
  printf("x = [");
  for(unsigned int k = 0; k < x.length; k++)
    printf("%0.04f + %0.04fj, ", ldexp(x.data[k].re, x.exp), ldexp(x.data[k].im, x.exp));
  printf("]\n\n");


}