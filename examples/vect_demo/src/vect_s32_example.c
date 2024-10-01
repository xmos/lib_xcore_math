// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include <stdio.h>
#include <stdlib.h>

#include "xmath/xmath.h"


// Prints a BFP vector both in its mantissa-exponent form and as a floating point vector. Also
// prints headroom.
static void print_vector_s32(
    const int32_t vect[],
    const char* name,
    const exponent_t exp,
    const unsigned length,
    const unsigned line)
{
  // First, the raw mantissas
  printf("%s = [", name);
  for(unsigned int k = 0; k < length; k++)
    printf("%ld, ", (long int) vect[k]);
  printf("] * 2**(%d)\t// (line %u)\n", exp, line);

  // Next, the float equivalent
  printf("%s_float = [", name);
  for(unsigned int k = 0; k < length; k++)
    printf("%0.07f, ", ldexp(vect[k], exp));
  printf("]\n");
}

#define PRINT_VECT_S32(X)     print_vector_s32(X.data, #X, X.exp, LENGTH, __LINE__)

// The length (in elements) of our BFP vectors. We're using a small length to keep the text output
// relatively tidy.
#define LENGTH    (4)



void vect_s32_example()
{

  printf("\n\n\n\n");
  printf("========================\n");
  printf("== vect_s32_example() ==\n");
  printf("========================\n");

  /*
    Here we'll look at the function vect_s32_mul()

    vect_s32_mul() is an element-wise real multiply for vectors of 32-bit elements. Its prototype
    is:

      headroom_t vect_s32_mul(
        int32_t a[],
        const int32_t b[],
        const int32_t c[],
        const unsigned length,
        const right_shift_t b_shr,
        const right_shift_t c_shr);

    The first three parameters are the input and output vectors. More specifically, b[] and c[] are
    input vectors and a[] is an output. All three vectors should be at least length elements long.
    The vectors must also begin at word-aligned addresses (this happens automaticaly when operating
    on an int32_t array, but may need to be explicitly specified if, for example, we're operating on
    a char array which is being recast as an int32_t array).

    b_shr and c_shr are arithmetic right-shifts applied to elements of b[] and c[] (respectively)
    prior to multiplication. These are the levers used to simultaneously prevent
    overflows/saturation in the result and to minimize the headroom of the result (which has the
    effect of maximizing the precision of the result).

    What values to use for b_shr and c_shr? The vect_* functions which take shift parameters as
    arguments each have an associated helper function whose name is the same as that of the operator
    function with "_prepare" at the end. So, for vect_s32_mul() the corresponding prepare function
    is vect_s32_mul_prepare().

      void vect_s32_mul_prepare(
        exponent_t* a_exp,
        right_shift_t* b_shr,
        right_shift_t* c_shr,
        const exponent_t b_exp,
        const exponent_t c_exp,
        const headroom_t b_hr,
        const headroom_t c_hr);

    Most of the prepare functions look similar to this. Here a_exp, b_shr and c_shr are the
    parameters to be output by this function. b_shr and c_shr are the shift parameters to be passed
    on to vect_s32_mul() and a_exp is the exponent that should be associated with the product vector
    a[]. The inputs b_exp and c_exp are the exponents associated with input vectors b[] and c[]
    respectively.

    Exponents come into play because the prepare functions assume that a block floating-point-like
    behavior is desired. In general, the product of two 32-bit integers will not fit into a 32-bit
    value, and so the output exponent a_exp is calculated to ensure the product does not hit the
    data type's saturation bounds while preserving the most possible precision.

    b_hr and c_hr are the headroom of vectors b[] and c[]. The headroom of a vector is the number of
    bits that its elements may be left-shifted without losing information (i.e. saturating).
    Equivalently, for vectors with signed elements, it is the minimum number of _redundant_ leading
    sign bits among the individual elements. This is important because it tells us about the range
    of values actually held in a vector of integers.

    Returning to vect_s32_mul(), note that its return value is also of type headroom_t. The XS3 VPU
    has hardware features that allow it to track the headroom of data being processed. The returned
    value is the headroom of the output vector a[] (which may be needed for subsequent processing
    steps).

  */

  // Let's start by creating 3 structs representing BFP vectors
  struct {
    int32_t WORD_ALIGNED data[LENGTH];
    exponent_t exp;
    headroom_t hr;
  } A, B, C;

  // Note that this struct is similar to the bfp_s32_t struct, except
  //    1) it contains no length because here we're using a static length of LENGTH
  //    2) instead of a pointer to a mantissa buffer, the buffer is part of the struct.

  // Also notice that WORD_ALIGNED was used to guarantee word-alignment of data[] (even though it
  // should not actually be necessary here because we're using int32_t).


  // Let's fill B[] and C[] with random values
  {
    printf("\n\n\n============ Step 1 ============\n");

    // Random mantissas
    for(int k = 0; k < LENGTH; k++){
      B.data[k] = rand();
      C.data[k] = rand();
    }
    
    //use an exponent of -31 on each, just to keep the values tractable
    B.exp = -31;
    C.exp = -31;

    PRINT_VECT_S32(B);
    PRINT_VECT_S32(C);

  }
  
  // We don't yet know the headroom of B[] and C[], so we'll find that next
  {
    printf("\n\n\n============ Step 2 ============\n");

    B.hr = vect_s32_headroom(B.data, LENGTH);
    C.hr = vect_s32_headroom(C.data, LENGTH);
    printf("B.hr --> %u\n", B.hr);
    printf("C.hr --> %u\n", C.hr);
  }

  // Now we can multiply the vectors
  {
    printf("\n\n\n============ Step 3 ============\n");

    right_shift_t b_shr;
    right_shift_t c_shr;
    vect_s32_mul_prepare(&A.exp, &b_shr, &c_shr, B.exp, C.exp, B.hr, C.hr);

    printf("A.exp --> %d  (output exponent)\n", A.exp);
    printf("b_shr --> %d\n", b_shr);
    printf("c_shr --> %d\n", c_shr);

    A.hr = vect_s32_mul(A.data, B.data, C.data, LENGTH, b_shr, c_shr);

    PRINT_VECT_S32(A);
    printf("A.hr --> %u\n", A.hr);
  }


  // The code above is exactly how bfp_s32_mul() implements this behavior.

  /*
    What if this multiplication is the last step in our BFP logic, and immediately afterwards the
    result is to be input to a function which requires the data to be in a specific Q-format? In
    this case, if we used the logic above (or bfp_s32_mul()) we may then have to add an additional
    step to shift the mantissas around to achieve the correct exponent. Let's suppose the data must
    be in a Q24 format:

      ... //continued from above
      const exponent_t required_exponent = -24;
      right_shift_t q_shr = required_exponent - A.exp;
      vect_s32_shr(A.data, A.data, LENGTH, q_shr);
      A.hr += q_shr;
      A.exp += q_shr;

    Alternatively, you can save CPU cycles and skip that step entirely by modifying the shift
    parameters b_shr and c_shr.

  */
  {
    printf("\n\n\n============ Step 4 ============\n");

    right_shift_t b_shr;
    right_shift_t c_shr;
    vect_s32_mul_prepare(&A.exp, &b_shr, &c_shr, B.exp, C.exp, B.hr, C.hr);

    // This is the required exponent
    const exponent_t required_exponent = -24;
    // This is the number of bits the mantissas must be right-shifted to achieve the required
    // exponents
    const int adjustment = required_exponent - A.exp;
    
    /*
     Here it becomes important that you understand the particular operation under consideration.
     This is element-wise multiplication, such that
 
       A[k] * 2^(A.exp) = B[k] * 2^(B.exp) * C[k] * 2^(C.exp)
                        = B[k] * C[k] * 2^(B.exp + C.exp)
 
     The temptation at this point might be to add 'adjustment' to both b_shr and c_shr. But because
     multiplication of exponentials (with a common base) results in adding together their exponents,
     in order to achieve the desired output exponent, we instead need to add a _total_ of
     'adjustment' to the _sum_ b_shr + c_shr. It can all be added to one or to the other, or it can
     be split between them, which is what we'll do here.
    */
    b_shr += (adjustment / 2);
    c_shr += (adjustment - (adjustment / 2));
    // And just for completeness we'll make sure A[]'s exponent is also adjusted
    A.exp += adjustment;

    // The remaining steps are identical to the pure block floating-point case above.
    A.hr = vect_s32_mul(A.data, B.data, C.data, LENGTH, b_shr, c_shr);

    PRINT_VECT_S32(A);
    // Note that the printed floating point output values are (nearly) the same as above, indicating
    // we've done this correctly.
    printf("A.hr --> %u\n", A.hr);

    // A final note on manually specifying an output Q-format: When doing this, there is no built-in
    // protection which prevents the user from corrupting the data through saturation or underflows.
    // That protection can only come from a clear understanding of the assumptions being made about
    // the input vectors (e.g. the range of possible exponents and mantissa values), and an
    // understanding of the operator being invoked.
  }



  /*
    Finally, in some situations we may have a priori information that the BFP operators are unable
    to take into account.

    For example, sometimes we may have a vector of unsigned integers to multiply by another 32-bit
    vector, but the XS3 VPU does not support any unsigned multiplications. Does that mean we cannot
    do such an operation? Hardly!
  */
  {
    printf("\n\n\n============ Step 5 ============\n");

    // Let's suppose that we want to multiply B[] by C[], but where C[] is a vector of unsigned
    // integers.

    // The key fact that allows this to work is that *as long as the unsigned input vector has at
    // least 1 bit of (unsigned) headroom, it can be safely treated as a signed vector*.

        // ASIDE: "signed" vs "unsigned" headroom. Signed headroom (which is almost always what we
        //        mean by "headroom" in lib_xcore_math) is the number of _redundant_ leading sign
        //        bits of the vector element with the fewest sign bits. The number of redundant
        //        leading sign bits is one fewer than the number of sign bits. "unsigned" headroom,
        //        on the other hand, is the number of leading _zeros_ of the vector element with the
        //        fewest number of leading zeros. Unsigned integers do not have sign bits, of
        //        course. The signed headroom of an unsigned vector is then one less than the
        //        unsigned headroom of that vector. So, this becomes a problem only when the
        //        unsigned vector has 0 bits of unsigned headroom (i.e. _negative_ signed headroom).

    // Ideally the (unsigned) headroom of C[] would already be known or is calculated (avoiding the
    // need for any adjustments if there's already 1 bit of (unsigned) headroom) but a simple
    // strategy that usually works is to just manually add 1 bit of headroom to the unsigned vector
    // as follows
    for(int k = 0; k < LENGTH; k++)
      C.data[k] = ((uint32_t)C.data[k]) >> 1;
  
    C.exp += 1;
    PRINT_VECT_S32( C );

    // Now that we've guaranteed one bit of headroom, we can apply the usual logic
    right_shift_t b_shr;
    right_shift_t c_shr;
    vect_s32_mul_prepare(&A.exp, &b_shr, &c_shr, B.exp, C.exp, B.hr, C.hr);

    printf("A.exp --> %d\n", A.exp);
    printf("b_shr --> %d\n", b_shr);
    printf("c_shr --> %d\n", c_shr);

    A.hr = vect_s32_mul(A.data, B.data, C.data, LENGTH, b_shr, c_shr);
    PRINT_VECT_S32(A);
    printf("A.hr --> %u\n", A.hr);

    // As a final note on this topic, because the XS3 VPU only uses signed logic, even if it outputs
    // a vector with all non-negative mantissas a sign bit must necessarily be present, and so this
    // trick should, in general, never be needed with vectors that result from lib_xcore_math
    // operators.
  }
  
}