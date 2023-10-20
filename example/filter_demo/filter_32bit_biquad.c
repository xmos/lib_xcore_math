// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include <stdio.h>
#include <stdlib.h>

#include "xmath/xmath.h"

// Each filter_biquad_s32_t can store (up to) 8 biquad filter sections
#define SECTION_COUNT   8
#define FILTER_TICKS    120

/**
 * Biquad filter used in this example.
 *
 * Unlike the 16- and 32-bit FIR filters, 32-bit biquad filters store their state and coefficients
 * within the filter struct itself.
 *
 */
filter_biquad_s32_t filter = {
  // Number of biquad sections in this filter block
  .biquad_count = SECTION_COUNT,
  
  // Filter state, initialized to 0
  .state = {{0}},

  // Filter coefficients
  .coef = {
    { 0x160cc0a,  0x28aa355,  0x323dae1,  0x1f12a2a,   0x32fc6d,  0x9a142d6,  0xc88bad1,  0x541f6bf },
    { 0xb4007f5,  0x4e11ec,   0x362dabe,  0x9bfb667,   0x405747,  0x6d593cd,  0xae295e3,  0xac62779 },
    { 0xb5a000d,  0xc84d2fc,  0xa20394f,   0xc9bb57,  0x454e970,  0x912f78e,   0xcf4303,  0x900fdb2 },
    {-0x574fcb8, -0x6c18c97, -0x2228d4a, -0x6ab3e49,  -0xe9532b, -0x2876ab2, -0x2d4d1e7, -0x11aaecb },
    {-0x4329901, -0x19826ba, -0x7b349e9, -0x7b0ec2e, -0x96e1eb2,  -0xbe6675, -0x92fa8be, -0x9cf08a7 }
  }
};

void filter_32bit_biquad()
{
  printf("\n\n\n\n");
  printf("=============================\n");
  printf("== 32-bit Biquad Filtering ==\n");
  printf("=============================\n");

  /**
   * In this example, we'll look at how to use the 32-bit biquad filtering functions.
   *
   * 32-bit biquad filters use the filter_biquad_s32_t type. Unlike the 16- and 32-bit FIR
   * filters, the 32-bit biquad filter type carries its own data and need not be initialized.
   * (see above)
   *
   * The coefficients of the 32-bit biquad filter are encoded as Q2.30 fixed-point values. If x[n]
   * is the input to a biquad section at time n, and y[n] is its output, then the relationship
   * between y[n] and x[n] for a single biquad filter section is:
   *
   *    y[n] = ((b0 * x[n  ]) >> 30)
   *         + ((b1 * x[n-1]) >> 30)
   *         + ((b2 * x[n-2]) >> 30)
   *         - ((a1 * y[n-1]) >> 30)
   *         - ((a2 * y[n-2]) >> 30)
   *
   *  where the right-shifts are rounding, and b0, b1, b2, -a1, and -a2 are `filter.coef[0][k]`,
   * `filter.coef[1][k]`, `filter.coef[2][k]`, `filter.coef[3][k]`, and `filter.coef[4][k]`
   *  respectively, for the kth biquad section. The output y[n] is then also the input to the
   *  (k+1)th biquad section.
   *
   * A single filter_biquad_s32_t stores coefficients and state information for up to 8 biquad
   * sections (the optimal block size for computation). If more than 8 biquad sections are required,
   * an array of filter_biquad_s32_t structs can be used, and the filter can be executed using
   * filter_biquads_s32() instead of filter_biquad_s32().
   */

  // Print out the coefficients for each biquad filter section
  for(unsigned int section = 0; section < filter.biquad_count; section++){
    printf("Biquad Section[%d]\n", section);
    printf("  -a1 = %f\n", ldexp(filter.coef[3][section], -30));
    printf("  -a2 = %f\n", ldexp(filter.coef[4][section], -30));
    printf("   b0 = %f\n", ldexp(filter.coef[0][section], -30));
    printf("   b1 = %f\n", ldexp(filter.coef[1][section], -30));
    printf("   b2 = %f\n", ldexp(filter.coef[2][section], -30));
  }


  /**
   * Process random samples.
   * Because we're only using 1 filter block here, we use filter_biquad_s32().
   */
  for(int k = 0; k < FILTER_TICKS; k++){
    int32_t input_sample = rand() << 1;
    int32_t output_sample = filter_biquad_s32(&filter, input_sample);

    printf("input[% 4d] = % 13ld;  output[% 4d] = % 13ld\n", k, (long int) input_sample, k, (long int) output_sample);
  }

}