// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include <stdio.h>
#include <stdlib.h>

#ifdef __XS3A__
# include <xscope.h>
#endif

#include "xmath/xmath.h"

void bfp_s32_example();

#define RAND_SEED   0xDECAFFED

int main(int argc, char** argv)
{
#ifdef __XS3A__
  xscope_config_io(XSCOPE_IO_BASIC);
#endif

  // Seed the random number generator, using a constant for reproducibility
  srand(RAND_SEED);

  // This example app only demonstrates the real 32-bit BFP functions, but the other three main categories of
  // arithmetic BFP functions, real 16-bit, complex 32-bit and complex 16-bit, all work in a very similar way.
  
  // The slight caveat is that in a complex 32-bit BFP vector the real and imaginary parts of a given element are
  // stored in adjacent memory locations, in a complex 16-bit BFP vector the real and imaginary parts are stored in
  // separate buffers.
  bfp_s32_example();

  return 0;
}


// Prints a BFP vector both in its mantissa-exponent form and as a floating point vector. Also prints headroom.
static void print_vector(
    const bfp_s32_t* vect,
    const char* name,
    const unsigned line_num)
{
  printf("=== From line #%u ===\n", line_num);

  // First, the raw mantissas
  printf("%s = [", name);
  for(unsigned int k = 0; k < vect->length; k++)
    printf("%ld, ", (long int)vect->data[k]);
  printf("] * 2**(%d)\n", vect->exp);

  // Next, the float equivalent
  printf("%s_float = [", name);
  for(unsigned int k = 0; k < vect->length; k++)
    printf("%0.07f, ", ldexp(vect->data[k], vect->exp));
  printf("]\n");

  // And the headroom
  printf("%s headroom: %u\n\n", name, vect->hr);
}

#define PRINT_VECT(X)     print_vector(&X, #X, __LINE__)
#define PRINT_SCALAR(X)   printf("%s = %0.07f\n\n", #X, ldexp((double) X.mant, X.exp))


// The length (in elements) of our BFP vectors. We're using a small length to keep the text output relatively tidy.
#define LENGTH    (4)



void bfp_s32_example()
{

  printf("###################################\n");
  printf("### 32-bit block floating-point ###\n");
  printf("###################################\n\n");
  
  // Let's use 3 BFP vectors, A, B and C for this example. For real 32-bit BFP vectors, we use the type bfp_s32_t.
  bfp_s32_t A, B, C;

  // The bfp_s32_t type does not allocate its own buffer for the mantissa vector (nor does the initialization function).
  // Instead, it contains a pointer that points to the backing buffer.
  int32_t buffer_A[LENGTH];
  int32_t buffer_B[LENGTH];
  int32_t buffer_C[LENGTH];

  // With the space allocated, we can initialize our BFP vectors.
  // bfp_s32_init() takes:
  //    - A pointer to the bfp_s32_t to be initialized
  //    - The address of the buffer which backs the vector
  //    - The initial exponent of the vector
  //    - The length of the vector, in elements
  //    - a boolean indicating whether or not the headroom should be immediately calculated.
  printf("Initializing BFP vectors A, B and C\n");
  bfp_s32_init(&A, buffer_A, -31, LENGTH, 0);
  bfp_s32_init(&B, buffer_B, -31, LENGTH, 0);
  bfp_s32_init(&C, buffer_C, -31, LENGTH, 0);
  
  // Above we specified an initial exponent of -31. For a signed 32-bit value, which can hold integers between
  // (and including) -(2^31) and (2^31)-1, an exponent of -31 means the representable range is -1.0 to 1.0,
  // including the former, but not the latter.

  // Above we also instructed bfp_s32_init() NOT to compute the headroom. This is because for B and C we're about
  // to randomly set the mantissas, invalidating whatever headroom was calculated. A, on the other hand, will first
  // be used as an output vector, so its exponent, headroom and mantissa data will be clobbered.

  // This is because we're going to fill them with random data next, so their current values are irrelevant.

  // Fill vector's B and C with random data.
  for(int k = 0; k < LENGTH; k++){
    B.data[k] = rand();
    C.data[k] = rand();
  }

  // Last thing before we print out the initial values of vectors B and C will be to force a recalculation of their
  // headroom with this random data. A call to bfp_s32_headroom() accomplishes this.
  printf("\nUpdating headroom of vectors B and C.\n");
  bfp_s32_headroom(&B);
  bfp_s32_headroom(&C);

  // Now print out the initial values for B and C.
  {
    printf("Initial values for BFP vectors B and C:\n");
    PRINT_VECT(B);
    PRINT_VECT(C);
    printf("\n\n");
  }


  // Let's start by applying a right-shift to the values of B using bfp_s32_shl() (with a negative shift). This isn't
  // necessary for the calls that follow, we can at least watch what happens to the mantissas, exponent and headroom.
  {
    printf("A[k] <-- B[k] >> 10\n");
    bfp_s32_shl(&A, &B, -10);
    PRINT_VECT(A);
    // Applying a -10 bit left-shift effectively divides the vector by a factor of 1024. It's worth pointing out that
    // because we're doing this in block floating-point, it would have been (nearly) equivalent to simply reduce the
    // exponent of B by 10.
    printf("\n\n");
  }
  

  // Now let's try adding together vectors B and C element-wise.
  {
    printf("A[k] <-- B[k] + C[k]\n");
    bfp_s32_add(&A, &B, &C);
    PRINT_VECT(A);

    // Note that the above call to bfp_s32_add() takes 3 arguments. The first is the BFP vector into which the result
    // will be placed, the second and third are the two input vectors. All three bfp_s32_t's must have already been
    // initialized (otherwise they won't point to a valid data buffer), although the mantissas, exponent and headroom
    // of the output vector will be clobbered.
    // Most BFP functions in this library are very similar, with the first (non-const) bfp_s32_t* being the output vector
    // and the subsequent bfp_s32_t pointers being the input vectors.
    printf("\n\n");
  }

  
  // In the previous step, we updated A with the sum of vector's B and C. But, if we have no need to keep the contents
  // of B around (e.g. it's needed for a subsequent stage), we may be able to forego the memory cost of a third
  // vector (A) just for the output by updating B in-place.
  {
    printf("B[k] <-- B[k] + C[k]\n");
    bfp_s32_add(&B, &B, &C);
    PRINT_VECT(B);

    // Except where explicitly stated otherwise, the element-wise BFP functions in this library (which take a
    // pointer to an output vector) can safely update one or the other of the input vectors in-place.
    printf("\n\n");
  }
  
  
  // Element-wise multiplication works just like addition:
  {
    printf("A[k] <-- B[k] * C[k]\n");
    bfp_s32_mul(&A, &B, &C);
    PRINT_VECT(A);
    printf("\n\n");
  }
  


  
  // There are several BFP functions that take only a single input vector:

  // Rectify a vector so that all elements are non-negative
  {
    printf("B[k] <-- max{ A[k], 0 }\n");
    bfp_s32_rect(&B, &A);
    PRINT_VECT(B);
    printf("\n\n");
  }


  // Take the square root of each element of a vector
  {
    printf("A[k] <-- sqrt(B[k])\n");
    bfp_s32_sqrt(&A, &B);
    PRINT_VECT(A);
    printf("\n\n");
  }


  // There are others that take a vector and a scalar as inputs, like bfp_s32_scale()
  {
    float_s32_t alpha = {
      .mant = 0xBADDA7E5, //mantissa
      .exp = -23          //exponent
    };
    printf("C[k] <-- C[k] * %0.07f\n", ldexp(alpha.mant, alpha.exp));
    bfp_s32_scale(&C, &C, alpha);
    PRINT_VECT(C);
    printf("\n\n");
  }


  // And still others that produce a scalar, rather than a vector

  // Inner product of vectors B and C
  {
    printf("alpha <-- sum{ B[k] * C[k] }  (inner product)\n");
    float_s64_t alpha = bfp_s32_dot(&B, &C);
    PRINT_SCALAR(alpha);
    printf("\n\n");
  }
  

  // Maximum element of a BFP vector
  {
    printf("alpha <-- max{ B[0], B[1], ... B[LENGTH-1] }\n");
    float_s32_t alpha = bfp_s32_max(&B);
    PRINT_SCALAR(alpha);
    printf("\n\n");
  }


  // Mean value of a BFP vector
  {
    printf("alpha <-- mean{ A[0], A[1], ..., A[LENGTH-1] }\n");
    float_s32_t alpha = bfp_s32_mean(&A);
    PRINT_SCALAR(alpha);
    printf("\n\n");
  }

  
}