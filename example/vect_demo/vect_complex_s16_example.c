// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include <stdio.h>
#include <stdlib.h>

#include "xmath/xmath.h"


// Prints a BFP vector both in its mantissa-exponent form and as a floating point vector. Also prints headroom.
static void print_vector_complex_s16(
    const int16_t real[], 
    const int16_t imag[],
    const char* name,
    const exponent_t exp,
    const unsigned length,
    const unsigned line)
{
  // First, the raw mantissas
  printf("%s = [", name);
  for(unsigned int k = 0; k < length; k++)
    printf("%d + j*%d, ", real[k], imag[k]);
  printf("] * 2**(%d)\t// (line %u)\n", exp, line);

  // Next, the float equivalent
  printf("%s_float = [", name);
  for(unsigned int k = 0; k < length; k++)
    printf("%0.07f + j*%0.07f, ", ldexp(real[k], exp), ldexp(imag[k], exp));
  printf("]\n");
}

#define PRINT_VECT_C16(X)     print_vector_complex_s16(X.real, X.imag, #X, X.exp, LENGTH, __LINE__)

// The length (in elements) of our BFP vectors. We're using a small length to keep the text output relatively tidy.
#define LENGTH    (4)



void vect_complex_s16_example()
{
  printf("\n\n\n\n");
  printf("================================\n");
  printf("== vect_complex_s16_example() ==\n");
  printf("================================\n");

  /*
    Here we'll look at the functin vect_complex_s16_add().

    vect_complex_s16_add() is an element-wise addition of two complex 16-bit vectors.

      headroom_t vect_complex_s16_add(
          int16_t a_real[],
          int16_t a_imag[],
          const int16_t b_real[],
          const int16_t b_imag[],
          const int16_t c_real[],
          const int16_t c_imag[],
          const unsigned length,
          const right_shift_t b_shr,
          const right_shift_t c_shr);

    The first six parameters are the real and imaginary parts of each of the input and output 
    vectors. Specifically, if A[] is (conceptually) the complex block floating-point output 
    vector, then a_real[] and a_imag[] store the real and imaginary parts respectively of A[]'s 
    mantissas. Likewise with complex input vectors B[] and C[]. So, the mantissa of an element
    B[k] is (b_real[k] + j*b_imag[k]), and if the exponent of B[] is B_exp, then the rational
    value of B[k] is  (b_real[k] + j*b_imag[k]) * 2^(B_exp).

    Each of the first six parameters must be word-aligned and point to a buffer that can hold
    at least `length` elements.

    b_shr and c_shr are arithmetic right-shifts applied to the mantissas of B[] and C[] 
    respectively prior to being added. These shifts are necessary not only to prevent
    overflow/saturation, but also because in order for adding two mantissas together to be
    a meaningful operation, they must correspond to the same exponent. For example, if we 
    two rational values (1 * 10^0) and (2 * 10^-1), each representing the number 1.0,
    if we simply add together that mantissas (1+2=3) we get a result in which no integer
    power of 10 gives a correct solution.

    b_shr and c_shr, as well as the exponent associated with the output vector A[] can be
    calculated using vect_complex_s16()'s prepare function, vect_complex_s16_prepare().

      void vect_complex_s16_prepare(
          exponent_t* a_exp,
          right_shift_t* b_shr,
          right_shift_t* c_shr,
          const exponent_t b_exp,
          const exponent_t c_exp,
          const headroom_t b_hr,
          const headroom_t c_hr);

    ASIDE: Technically vect_complex_s16_add_prepare is a macro which resolves to 
           vect_s32_add_prepare. The logic for computing the shifts and output exponent for
           vect_complex_s16_add() and vect_s32_add() is the same. The macro is used to
           provide a more uniform API for the user (i.e. the prepare function for operation 'X()'
           is 'X_prepare()').

    Here, a_exp is an output parameter which is the exponent associated with the ouput vector 
    A[]. b_shr and c_shr are output parameters which correspond to the b_shr and c_shr used
    for vect_complex_s16_add(). b_exp and b_hr are the exponent and vector headroom of 
    input vector B[]. Likewise with c_exp, c_hr and C[].

    Finally, the output of vect_complex_s16_add() is the headroom of the complex output
    vector A[]. For a 16-bit complex BFP vector, the headroom is the minimum of the headroom
    of each of the vector's complex mantissas. The headroom of a complex mantissa is the
    minimum of the headroom of its real part and its imaginary part.

  */

  // This is the data we'll use for vectors A[], B[] and C[]. Note that the real[] and imag[]
  // arrays are declared as word-aligned. This is a requirement for most vect_* functions.
  // Also note that this data structure looks very similar to bfp_complex_s16_t, which the
  // high-level BFP API uses for complex 16-bit vectors.
  struct {
    int16_t WORD_ALIGNED real[LENGTH];
    int16_t WORD_ALIGNED imag[LENGTH];
    exponent_t exp;
    headroom_t hr;
  } A, B, C;
  // To be clear about the notation used (in the comments) here,  A[], B[] and C[] refer to the
  // block floating-point vectors, and e.g. A[k] refers to the complex rational value 
  // ((A.real[k] + j*A.imag[k]) * 2^(A.exp)).


  // Setting our initial exponents to -15 means that the vector elements will span the range
  // [-1.0, 1.0), because INT16_MIN is -2^15 and INT16_MAX is (2^15)-1.
  B.exp = -15;
  C.exp = -15;

  // Randomly set the mantissas for B[] and C[]
  for(int k = 0; k < LENGTH; k++){
    B.real[k] = rand();
    B.imag[k] = rand();
    C.real[k] = rand();
    C.imag[k] = rand();
  }


  // We haven't yet initialized the headroom of vectors B[] and C[]. Because we need to know their
  // headroom to properly call vect_complex_s16_add_prepare(), we need to initialize these. 
  // vect_complex_s16_headroom() computes the headroom for a complex 16-bit mantissa vector.
  { 
    printf("\n\n\n============ Step 1 ============\n");

    B.hr = vect_complex_s16_headroom(B.real, B.imag, LENGTH);
    C.hr = vect_complex_s16_headroom(C.real, C.imag, LENGTH);

    PRINT_VECT_C16(B);
    PRINT_VECT_C16(C);

    printf("B.hr --> %u\n", B.hr);
    printf("C.hr --> %u\n", C.hr);

  }


  // Now that we've initialized everything, we can execute the add operation.
  {
    printf("\n\n\n============ Step 2 ============\n");

    // First, we prepare, which tells us the output exponent A.exp and the shift values
    // needed by vect_complex_s16_add()
    right_shift_t b_shr, c_shr;
    vect_complex_s16_add_prepare(&A.exp, &b_shr, &c_shr, B.exp, C.exp, B.hr, C.hr);

    printf("A.exp --> %d\n", A.exp);
    printf("b_shr --> %d\n", b_shr);
    printf("c_shr --> %d\n", c_shr);

    // Then we call the operator, making sure that we capture the headroom of A[], returned by
    // the function.
    A.hr = vect_complex_s16_add(A.real, A.imag, B.real, B.imag, 
                                C.real, C.imag, LENGTH, b_shr, c_shr);

    PRINT_VECT_C16(A);
    printf("A.hr --> %u\n", A.hr);

  }


  // Perhaps we need out complex 16-bit output vector A[] to be in the particular fixed-point
  // Q-format Q8.8, in which the mantissa's 8 least significant bits are fractional. We can
  // achieve this adjusting the results of the prepare step.
  { 
    printf("\n\n\n============ Step 3 ============\n");

    // First, prepare as we did before.
    right_shift_t b_shr, c_shr;
    vect_complex_s16_add_prepare(&A.exp, &b_shr, &c_shr, B.exp, C.exp, B.hr, C.hr);

    // The exponent have been chosen to maximize the precision of the result vector
    printf("A.exp --> %d (before adjustment)\n", A.exp);
    printf("b_shr --> %d (before adjustment)\n", b_shr);
    printf("c_shr --> %d (before adjustment)\n", c_shr);

    // To force an output Q-format of Q8.8, we find the adjustment necessary to change the 
    // output exponent to the negative of the number of fractional bits:
    const exponent_t required_exp = -8;
    const int adjustment = required_exp - A.exp;

    // This adjustment is used to modify the shift parameters and (for correctness) the output
    // exponent.

    //  ASIDE: If you've looked at the vect_s32_example() where we enforced an output Q-format of 
    //         Q8.24 from vect_s32_mul(), you may notice that the logic here looks different.
    //         In particular, in that example, the adjustment was split between b_shr and c_shr, 
    //         whereas here b_shr and c_shr each receive the full adjustment. This is a genuine 
    //         difference in the logic of addition versus multiplication.  To make these sorts of
    //         manual optimizations, *you must understand how the mantissas and exponents of the
    //         inputs and outputs of that particular operation relate to one another*.
    b_shr += adjustment;
    c_shr += adjustment;
    A.exp += adjustment;

    printf("A.exp --> %d (after adjustment)\n", A.exp);
    printf("b_shr --> %d (after adjustment)\n", b_shr);
    printf("c_shr --> %d (after adjustment)\n", c_shr);

    // The the adjustments complete, we call vect_complex_s16_add() as usual.
    A.hr = vect_complex_s16_add(A.real, A.imag, B.real, B.imag, 
                                    C.real, C.imag, LENGTH, b_shr, c_shr);

    PRINT_VECT_C16(A);
    printf("A.hr --> %u\n", A.hr);
  }

}