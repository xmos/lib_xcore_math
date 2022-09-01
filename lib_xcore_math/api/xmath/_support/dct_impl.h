// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

/*
 * Matrix for the 6-point DCT forward transform
 */
extern int32_t dct6_matrix[6][8];

/*
 * Matrix for the 8-point DCT transform
 */
extern int32_t dct8_matrix[9][8];

/*
 * Matrix for the 6-point DCT inverse transform
 */
extern int32_t idct6_matrix[7][8];

/*
 * Matrix for the 8-point DCT inverse transform
 */
extern int32_t idct8_matrix[9][8];

/*
 * Look-up table used when recursively computing the 12-point DCT using the 6-point DCT
 */
extern int32_t dct12_lut[6];

/*
 * Look-up table used when recursively computing the 16-point DCT using the 8-point DCT
 */
extern int32_t dct16_lut[8];

/*
 * Look-up table used when recursively computing the 24-point DCT using the 12-point DCT
 */
extern int32_t dct24_lut[12];

/*
 * Look-up table used when recursively computing the 32-point DCT using the 16-point DCT
 */
extern int32_t dct32_lut[16];

/*
 * Look-up table used when recursively computing the 48-point DCT using the 24-point DCT
 */
extern int32_t dct48_lut[24];

/*
 * Look-up table used when recursively computing the 64-point DCT using the 32-point DCT
 */
extern int32_t dct64_lut[32];

/*
 * Rearrange the contents of x[] so that each element appears at the index which is the bit-reversal
 * of its original index (limited to the log2(length) least significant bits). length must be a 
 * power of 2.
 * 
 * For example, if length is 32, then x[0] will stay where it is, and x[3] will move to x[24], 
 * because 00011 (3) goes to 11000 (24)
 */
C_API
void dct_index_bit_reversal(
    int32_t x[],
    const unsigned length);

/*
 * Reverse the order of elements in b[] and place them in a[]. Works in-place.
 */
C_API
void vect_s32_flip(
    int32_t a[],
    const int32_t b[],
    const unsigned length);

/*
 * Compute the element-wise sums and differences of head[] and tail[].
 * The sums are placed in sums[]. The differences are multiplied element-wise by dct_lut[] (as 
 * Q2.14 values) and placed in diffs[].
 */
C_API
headroom_t dct_adsb_s32(
    int32_t sums[],
    int32_t diffs[],
    const int32_t head[],
    const int32_t tail[],
    const unsigned chunks,
    const int32_t dct_lut[]);
    
/*
 * Apply the deconvolution required for the recursive DCT to the contents of D[], and interleave
 * the contents of B[] with that into res[]. i.e. if the deconvolved D[] is called D'[], then
 * res will get..
 * res[0] = B[0]
 * res[1] = D'[0]
 * res[2] = B[1]
 * res[3] = D'[1]
 * etc
 */
C_API
void dct_deconvolve_s32(
    int32_t res[],
    const int32_t B[],
    const int32_t D[],
    const unsigned res_length);

/*
 * Apply the convolution required for the recursive inverse DCT to x[] and place in y[]
 */
C_API
void idct_convolve(
    int32_t y[],
    const int32_t x[],
    const unsigned chunks);

/*
 * Scale the contents of x[] by Q2.14 vector idct_lut[] and apply a right shift of shr bits to the
 * result.
 * A 'chunk' is 8 elements.
 */
C_API
void idct_scale(
    int32_t x[],
    const int32_t idct_lut[],
    const unsigned chunks,
    const right_shift_t shr);

/*
 * Apply the add/sub procedure required for the recursive IDCT. 
 */
C_API
void idct_adsb(
    int32_t sums[],
    int32_t diffs[],
    const int32_t s[],
    const int32_t t_tilde[],
    const unsigned chunks);

////////// 2D DCT Stuff for 8-bit

/*
 * 8-point DCT transformation matrix used by dct8x8_forward(). Contents are 16-bit
 * Coefficients are in the normal order, and the second half of each row is 0 to make the
 * procedure significantly faster.
 */
extern int16_t dct8_matrix_16bit[8][16];

/*
 * 8-point DCT transformation matrix used by dct8x8_inverse(). Contents are 16-bit
 * Coefficients are in the normal order, and the second half of each row is 0 to make the
 * procedure significantly faster.
 */
extern int16_t idct8_matrix_16bit[8][16];


/*
 * First step in the 8-by-8 DCT.  It expands x[][] to 16 bits, does an 8-point DCT of each row, and
 * then populates y[][] with the *transpose* of that. i.e. columns of y[][] are the 8-point DCTs
 * of the rows of x[][].
 * 
 * Used for forward or inverse DCT, depending on whether dct8_matrix_16bit or idct8_matrix_16bit
 * is used.
 */
C_API
headroom_t dct8x8_stageA(
    int16_t y[8][8],
    const int8_t x[8][8],
    const int16_t matrix[8][16]);
    

/*
 * Second step in the 8-by-8 DCT.  It expands x[][] to 16 bits, does an 8-point DCT of each row, and
 * then populates y[][] with the *transpose* of that. i.e. columns of y[][] are the 8-point DCTs
 * of the rows of x[][].
 * 
 * Used for forward or inverse DCT, depending on whether dct8_matrix_16bit or idct8_matrix_16bit
 * is used.
 */
C_API
headroom_t dct8x8_stageB(
    int8_t y[8][8],
    const int16_t x[8][8],
    const int16_t matrix[8][16],
    const right_shift_t sat);
