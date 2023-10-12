// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include <stdio.h>
#include <stdlib.h>

#include "xmath/xmath.h"


#define TAP_COUNT     (35)

#define FILTER_TICKS  (50)


void filter_32bit_fir()
{
  printf("\n\n\n\n");
  printf("==========================\n");
  printf("== 32-bit FIR Filtering ==\n");
  printf("==========================\n");

  /**
   * In this example we'll look at how to use the 32-bit FIR filtering functions.
   *
   * The applied filter logic is approximately:
   *
   *    output[n] = sum_k{ (input[n-k] * b[k]) >> 30 } >> shift
   *    where
   *      > output[n] is the filter output at time n
   *      > input[n-k] is the filter input k time steps before time n
   *      > b[k] is the user-supplied filter coefficient associated with time lag k
   *          (i.e. larger k is applied to older samples)
   *      > shift is a user-specified power-of-two scale factor
   *      > sum_k{} is a sum over variable k with   0 < k < N_taps, and
   *      > N_taps is the number of filter taps.
   *
   * The multiplication inside the sum is a full 64-bit multiplication with a built-in 30-bit
   * rounding right-shift (as shown).  The maximum value of any such product is then
   *   ((-2^31)*(-2^31)*(2^(-30))) = 2^(62-30) = 2^(32).  These (effectively 33-bit) products
   * are accumulated into a set of 8 internal 40-bit saturating accumulators, accumulator p
   * containing the partial sum of the filter indices p mod 8.  The 8 accumulators containing the
   * 40-bit partial sums are then added together for an (at-most) 43-bit final sum. Finally, a
   * rounding, saturating right-shift of `shift` bits is applied to the 43-bit sum to get the
   * final result which is output.  The final shift saturates to the symmetric 32-bit bounds
   * of INT32_MAX and -INT32_MAX.  (Note that each of the 40-bit accumulators saturates to
   * symmetric 40-bit bounds.)
   *
   * Below we'll declare and initialize a 32-bit FIR filter with TAP_COUNT random coefficients.
   * We'll then iterate for TAP_COUNT steps, adding samples to the filter without actually computing
   * the filter output. Finally, we'll iterate another FILTER_TICKS steps, adding random samples to
   * the filter and calculating the outputs.
   */


  /**
   * Declare the required filter struct, filter_fir_s32_t.
   *
   * If your application requires multiple 32-bit FIR filters, one of these should be allocated
   * for each such filter.
   */
  filter_fir_s32_t filter;

  /**
   * Allocate buffers for the filter coefficients and the filter state.
   *
   * To initialize the filter, the init function needs to be provided with the filter coefficients.
   * In practice, if filter coefficients are known in advance, these can be declared as read-only
   * memory. They will not be modified by the filter API.
   *
   * The filter API uses a circular buffer to mangage the filter's state for the user. To do this,
   * it requires a user-supplied buffer in which to place the historical sample data.
   *
   * Both coefficient and sample buffers must be larger enough to contain TAP_COUNT int32_t
   * elements.
   *
   * Additionally, both buffers must begin at a 4-byte- (word-) aligned address. This is the default
   * behavior for int32_t arrays, but use of the WORD_ALIGNED macro is shown here for completeness.
   * This alignment should be manually specified if the buffer was originally declared as a char
   * array, for example.
   */
  int32_t WORD_ALIGNED coefficients[TAP_COUNT] = {0};
  int32_t WORD_ALIGNED sample_buffer[TAP_COUNT] = {0};

  /**
   * How to choose the final shift factor for a filter is beyond the scope of this example, but
   * here a final scale of 2 bits seems to work correctly. Note that this shift is unsigned
   * (i.e. negative values are treated as large right-shifts; they will not left-shift).
   */
  right_shift_t filter_shr = 2;

  // Choose some random coefficients
  for(int lag = 0; lag < TAP_COUNT; lag++){
    coefficients[lag] = rand() << 1;
  }

  /**
   * Initialize filter with filter_fir_s32_init().
   *
   * The parameters here are straight-forward, given the description of filter behavior
   * above.
   */
  filter_fir_s32_init(&filter, sample_buffer, TAP_COUNT, coefficients, filter_shr);

  // Print out the filter coefficients
  printf("Filter Coefficients:\n");
  
  printf("b = [");
  for(int k = 0; k < TAP_COUNT; k++){
    printf("% 11ld, ", (long int) filter.coef[k]);
    if(k % 8 == 7) printf("\n     ");
  }
  printf("]\n");

  printf("filter_shr = %d\n\n", filter.shift);

  
  // Print the equivalent floating-point FIR filter coefficients
  printf("b_float = [");
  for(int k = 0; k < TAP_COUNT; k++){
    printf("%f, ", ldexp(filter.coef[k], -30-filter.shift));
    if(k % 6 == 5) printf("\n           ");
  }
  printf("]\n\n");

  /**
   * Using filter_s32_add_sample(), supply the filter with random input samples without actually
   * computing any filtered output.
   *
   * This may be useful, for example, to quickly fill up a filters state with inputs, without
   * incurring the cost to compute the filtered output for each sample.
   */
  for(int k = -TAP_COUNT; k < 0; k++) {
    int32_t input_sample = rand() << 1;
    filter_fir_s32_add_sample(&filter, input_sample);

    printf("input[% 4d] = % 12ld;  output[% 4d] =      (N/A)\n", k, (long int) input_sample, k);
  }
  printf("\n");

  /**
   * Using filter_fir_s32(), supply the filter with new random input samples, computing the filtered
   * output value each time.
   */
  for(int k = 0; k < FILTER_TICKS; k++){
    int32_t input_sample = rand() << 1;
    int32_t output_sample = filter_fir_s32(&filter, input_sample);

    printf("input[% 4d] = % 12ld;  output[% 4d] = % 12ld\n", k, (long int) input_sample, k, (long int) output_sample);
  }

}