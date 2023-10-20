// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include "xmath/types.h"


/**
 * @defgroup filter_api  DSP Filter API
 */


#ifdef __XC__
extern "C" { 
#endif


/**
 * @brief 32-bit Discrete-Time Finite Impulse Response (FIR) Filter
 * 
 * @todo Move most of this information out to higher-level documentation
 * 
 * @par Filter Model
 * @parblock
 * 
 * This struct represents an N-tap 32-bit discrete-time FIR Filter.
 * 
 * At each time step, the FIR filter consumes a single 32-bit input sample and produces a single
 * 32-bit output sample.
 * 
 * To process a new input sample and compute a new output sample, use filter_fir_s32(). To add a new
 * input sample to the filter without computing a new output sample, use
 * filter_fir_s32_add_sample().
 *
 * An `N`-tap FIR filter contains `N` 32-bit cofficients (pointed to by `coef`) and `N` words of
 * state data (pointed to by `state`. The state data is a vector of the `N` most recent input
 * samples. When processing a new input sample at time step `t`, `x[t]` is the new input sample,
 * `x[t-1]` is the previous input sample, and so on, up to `x[t-(N-1)]`, which is the oldest input
 * considered when computing the new output sample (see note 1 below). The coefficients form a
 * vector `b[]`, where `b[k]` is the coefficient by which the `k`th oldest input sample is
 * multiplied. There is an additional parameter `shift` which scales the output as described below.
 * Both the coefficients and `shift` are considered to be constants which do not change after
 * initialization (although nothing should break if they are changed to new valid values).
 *
 * At time step `t`, the output sample `y[t]` is computed based on the inner product (i.e. sum of
 * element-wise products) of the coefficients and state data as follows (a more detailed description
 * is below):
 * 
 *     acc = x[t-0] * b[0] + x[t-1] * b[1] + x[t-2] * b[2] + ... + x[t-(N-1)] * b[N-1]
 *     y[t] = acc >> shift
 * 
 * Importantly, all three of the operators above (addition, multiplication and the rightwards
 * bit-shift) have slightly ideosyncratic meanings. 
 *
 * The products have a built-in rounding arithmetic right-shift of 30 bits, where ties round toward
 * positive infinity. This is a hardware feature which allows for longer filters (larger `N`)
 * without sacrificing coefficient precision. These element-wise products accumulate into 8 40-bit
 * accumulators saturate the sums at symmetric 40-bit bounds (see 
 * @ref saturation). The order in which the taps are accumulated is unspecified (see note 2 below).
 *
 * After each tap has been accumulated, the 8 accumulators are then added together to get a 64-bit
 * penultimate result (with 43 useful bits). Finally, an unsigned rounding arithmetic right-shift of
 * `shift` bits is applied to the 64-bit sum, and the final result is saturated to the symmetric
 * 32-bit range (`-INT32_MAX` to `INT32_MAX` inclusive).
 *
 * Below is a more detailed description of the operations performed (not including the saturation
 * logic applied by the accumulators).
 * 
 * @f[
 * 
 * &    y[t] = sat_{32} \left(
 *               round \left( 
 *                 \left(
 *                   \sum_{k=0}^{N-1} round(x[t-k] \cdot b[k] \cdot 2^{-30}) 
 *                 \right) \cdot 2^{-shift}
 *               \right)
 *             \right)                                                          \\
 * &        \qquad\text{where } sat_{32}() \text{ saturates to } \pm(2^{31}-1)  \\
 * &        \qquad\text{  and } round() \text{ rounds to the nearest integer, with ties rounding towards } +\!\infty
 * 
 * @f]
 * @endparblock
 * 
 * @par Operations
 * @parblock
 * 
 * **Initialize**: A `filter_fir_s32_t` filter is initialized with a call to filter_fir_s32_init().
 * The caller supplies information about the filter, including the number of taps and pointers the
 * coefficients and a state buffer. It is typically recommended that the state buffer be cleared to
 * all `0`s before initializing.
 *
 * **Add Sample**: To add a new input sample without computing a new output sample, use
 * filter_fir_s32_add_sample(). This is a constant-time operation which does not depend on the
 * number of filter taps. This may be useful in some situations, for example, to quickly pre-load
 * the filter's state buffer with multiple samples, without incurring the cost of computing an
 * output with each added sample.
 * 
 * **Process Sample**: To process a new input sample and produce a new output sample, use
 * filter_fir_s32().  
 * @endparblock
 * 
 * @par Fields
 * @parblock
 * 
 * After initialization via filter_fir_s32_init(), the contents of the `filter_fir_s32_t` struct are
 * considered to be opaque, and may change between major versions. In general, user code should not
 * need to access its members.
 *
 * `num_taps` is the order of the filter, or the number of taps. It is also the (minimum) size of
 * the buffers to which `coef` and `state` point, in elements (where each element is 4 bytes). The
 * time required to process an input sample and produce an output sample is approximately linear in
 * `num_taps` (see Performance below).
 *
 * `head` is the index into `state` at which the next sample will be added.
 *
 * `shift` is the unsigned arithmetic rounding saturating right-shift applied to internal
 * accumulator to get a final output.
 *
 * `coef` is a pointer to a buffer (supplied by the user at initialization) containing the tap
 * coefficients. The coefficients are stored in forward order, with lower indices corresponding to
 * newer samples. `coef[0]`, then, corresponds to `b[0]`, `coef[1]` to `b[1]`, and so on. None of
 * the functions which operate on `filter_fir_s32_t` structs in this library will modify the
 * contents of the buffer to which `coef` points. This buffer must be at least `num_taps` words
 * long.
 *
 * `state` is a pointer to a buffer (supplied by the user at initialization) containing the state
 * data -- a history of the `num_taps` most recent input samples. `state` is used in a circular
 * fashion with `head` indicating the index at which the next sample will be inserted.
 * @endparblock
 * 
 * @par Performance
 * @parblock
 * 
 * More work remains to fully characterize the time performance of this FIR filter, but
 * asymptotically (i.e. with a large number of filter taps) processing a new input sample to produce
 * a new output sample takes approximately 3 thread cycles per 8 filter taps.
 * 
 * That assumes that both the coefficients (pointed to by `coef`) and state buffer (pointed to by
 * `state`) are stored directly in SRAM.
 * @endparblock
 * 
 * 
 * @par Coefficient Scaling
 * @parblock
 * 
 * Suppose you're starting with a floating-point FIR filter model with coefficients `B[k]` which
 * operates on a sequence of 32-bit integer input samples `x[t]` to get a result `Y[t]` where
 * 
 *     Y[t] = x[t-0] * B[0] + x[t-1] * B[1] + ... + x[t-(N-1)] * B[N-1]
 * 
 * Because of the 30-bit right-shift and the right-shift of the final accumulator by `shift` bits,
 * the coefficients `b[k]` to use with this library can be thought of as fixed-point values with 
 * `30 + shift` fractional bits.
 *
 * The floating-point coefficients `B[k]` can then be naively converted to fixed-point coefficients
 * `b[k]`
 * 
 *     shift = 0
 *     b[k] = (int32_t) round(ldexp(B[k], 30)
 * 
 * After this, any further doubling of the coefficients can be compensated for without changing the
 * overall gain by incrementing `shift`.
 *
 * To maximize precision, you'll typically want `shift` to be as large as possible while in the
 * worst case to be considered neither saturates the internal accumulator (which, for safety, should
 * generally be assumed to be 42 bits), nor saturates the final 32-bit output when `shift` is
 * applied. 
 *
 * The details of this depend on various details, such as your filter's gain and the statistics of
 * the sequence `x[t]` (e.g. any headroom `x[t]` is known _a priori_ to have).
 * 
 * @endparblock
 * 
 * @par Filter Conversion
 * @parblock
 * 
 * This library includes a python script which converts existing floating-point FIR filter 
 * coefficients into a suitable representation and generates code for easily initializing and
 * executing the filter. See @ref filter_conversion for more.
 * @endparblock
 * 
 * @par Usage Example
 * @parblock
 * 
 * \code{.c}
 *      //// A simple 256-tap averaging filter
 *      #define N       256                     // Tap count
 *      #define B_VAL   ldexp(1.0/N, 30+7)      // Value for (all) coefficients
 * 
 *      const int32_t b[TAPS] =                 // The filter coefficients
 *      { B_VAL, B_VAL, B_VAL, ..., B_VAL };
 *      const right_shift_t shift = 7;          // The (unsigned) right-shift applied to the final accumulator
 *      int32_t state_buff[TAPS] = { 0 };       // Filter state buffer, initialized to 0's
 *      filter_fir_s32_t filter;            // The filter struct
 * 
 *      #define SAMPLE_COUNT    1024
 *      int32_t x[SAMPLE_COUNT] = { ... };      // Some sequence of input samples
 *      
 *      // Initialize
 *      filter_fir_s32_init(&filter, state_buff, N, b, shift);
 * 
 *      // Just add the first 64 without processing output samples. (not necessary)
 *      for(unsigned i = 0; i < 64; i++)
 *          filter_fir_s32_add_sample(&filter, x[i]);
 *      
 *      // Process the rest, generating a sequence of filtered output samples
 *      int32_t y[SAMPLE_COUNT] = { 0 };        //Output samples (first 64 never get updated here)
 *      for(unsigned i = 64; i < SAMPLE_COUNT; i++)
 *          y[i] = filter_fir_s32(&filter, x[i]);
 * 
 *      // Do something with output sequence
 *      ...
 * \endcode
 * 
 * This example creates a simple 256-tap filter which averages the most recent 256 samples.
 *
 * Each `b[k]` is @math{2^{29}}, and the final accumulator is right-shifted 7 bits. In the worst
 * case, all input samples are @math{-2^{31}}. In that case, the final accumulator value is 
 * @math{ 256 \cdot (2^{29} \cdot -2^{31} \cdot 2^{-30}) = -2^{38} }, well below the saturation
 * limit of the accumulator. After `shift` is applied, that becomes @math{-2^{38} \cdot 2^{-7} =
 * -2^{31}}. Finally, the 32-bit symmetric saturation logic is applied, making the final output
 * value @math{-2^{31}+1}.
 * @endparblock
 * 
 * @par Notes
 * @parblock
 * 
 * 1. `state` is a circular buffer, and so the index of `x[t]` within `state` changes with each
 *    input sample. The `state` field of this struct is considered to be opaque -- its exact usage
 *    may change between versions.
 * 
 * 2. Ordinarily integer sums are associative, so the order in which elements are added added does
 *    not affect the final result. The sum that the FIR filters use, however, is saturating, with
 *    the saturation logic being applied throughout the sum. This saturation is a hard non-linearity
 *    and is _not_ associative. The details of exactly when each tap is accumulated and into which
 *    accumulator are complicated and subject to change. It is best to construct a filter such that
 *    no ordering of the taps will saturate the accumulators.
 * @endparblock
 * 
 * @see filter_fir_s32_init, 
 *      filter_fir_s32_add_sample, 
 *      filter_fir_s32
 * 
 * @ingroup filter_api
 */
C_API
typedef struct {
    /**
     * The number of taps in the FIR filter.
     * 
     * This is also the number of elements in `state` and `coef`
     */
    unsigned num_taps;

    /**
     * Index into the `state` buffer where the next new sample will be placed.
     */
    unsigned head;

    /**
     * Unsigned arithmetic rounding right-shift applied to accumulator when computing filter output.
     */
    right_shift_t shift;

    /**
     * Pointer to a buffer containing the filter coefficients.
     */
    int32_t* coef;

    /**
     * Pointer to a circular buffer containing the previous input samples.
     */
    int32_t* state;
} filter_fir_s32_t;


/**
 * @brief Initialize a 32-bit FIR filter.
 * 
 * Before filter_fir_s32() or filter_fir_s32_add_sample() can be used on a filter it must be
 * initialized with a call to this function.
 * 
 * `sample_buffer` and `coefficients` must be at least `4 * tap_count` bytes long, and aligned to a
 * 4-byte (word) boundary.
 * 
 * See `filter_fir_s32_t` for more information about 32-bit FIR filters and their operation.
 * 
 * @param[out] filter           Filter struct to be initialized
 * @param[in]  sample_buffer    Buffer used by the filter to contain state information. Must be at
 *                              least `tap_count` elements long
 * @param[in]  tap_count        Order of the FIR filter; number of filter taps
 * @param[in]  coefficients     Array containing filter coefficients.
 * @param[in]  shift            Unsigned arithmetic right-shift applied to accumulator to get filter
 *                              output sample
 * 
 * @see filter_fir_s32_t
 * 
 * @ingroup filter_api
 */
C_API
void filter_fir_s32_init(
    filter_fir_s32_t* filter,
    int32_t* sample_buffer,
    const unsigned tap_count,
    const int32_t* coefficients,
    const right_shift_t shift);

/**
 * @brief Add a new input sample to a 32-bit FIR filter without processing an output sample.
 * 
 * This function adds a new input sample to `filter`'s state without computing a new output sample.
 * This is a constant- time operation and can be used to quickly pre-load a filter with sample data.
 * 
 * See `filter_fir_s32_t` for more information about FIR filters and their operation.
 * 
 * @param[inout] filter         Filter struct to have the sample added
 * @param[in]    new_sample     Sample to be added to `filter`'s history
 * 
 * @see filter_fir_s32_t
 * 
 * @ingroup filter_api
 */
C_API
void filter_fir_s32_add_sample(
    filter_fir_s32_t* filter,
    const int32_t new_sample);

/**
 * This function implements a Finite Impulse Response (FIR) filter. 
 * 
 * The new input sample `new_sample` is added to this filter's state, and a new output sample is
 * computed and returned as specified in `filter_fir_s32_t`.
 * 
 * With a large number of filter taps, this function takes approximately 3 thread cycles per 8
 * filter taps.
 * 
 * @param[inout]    filter          Filter to be processed
 * @param[in]       new_sample      New input sample to be processed by `filter`
 * 
 * @returns     Next filtered output sample
 * 
 * @see filter_fir_s32_t
 * 
 * @ingroup filter_api
 */
C_API
int32_t filter_fir_s32(
    filter_fir_s32_t* filter,
    const int32_t new_sample);


/**
 * @brief 16-bit Discrete-Time Finite Impulse Response (FIR) Filter
 * 
 * @par Filter Model
 * @parblock
 * 
 * This struct represents an N-tap 16-bit discrete-time FIR Filter.
 * 
 * At each time step, the FIR filter consumes a single 16-bit input sample and produces a single
 * 16-bit output sample.
 * 
 * To process a new input sample and compute a new output sample, use filter_fir_s16(). To add a new
 * input sample to the filter without computing a new output sample, use
 * filter_fir_s16_add_sample().
 * 
 * An `N`-tap FIR filter contains `N` 16-bit cofficients (pointed to by `coef`) and `N` `int16_t`s
 * of state data (pointed to by `state`. The state data is a vector of the `N` most recent input
 * samples. When processing a new input sample at time step `t`, `x[t]` is the new input sample,
 * `x[t-1]` is the previous input sample, and so on, up to `x[t-(N-1)]`, which is the oldest input
 * considered when computing the new output sample (see note 1 below). The coefficients form a
 * vector `b[]`, where `b[k]` is the coefficient by which the `k`th oldest input sample is
 * multiplied. There is an additional parameter `shift` which scales the output as described below.
 * Both the coefficients and `shift` are considered to be constants which do not change after
 * initialization (although nothing should break if they are changed to new valid values).
 * 
 * At time step `t`, the output sample `y[t]` is computed based on the inner product (i.e. sum of
 * element-wise products) of the coefficients and state data as follows (a more detailed description
 * is below):
 * 
 *     acc = x[t-0] * b[0] + x[t-1] * b[1] + x[t-2] * b[2] + ... + x[t-(N-1)] * b[N-1]
 *     y[t] = acc >> shift
 * 
 * Unlike the 32-bit FIR filters (see `filter_fir_s16_t`), the products `x[t-k] * b[k]` are the raw
 * 32-bit products of the 16-bit elements. These element-wise products accumulate into a 32-bit
 * accumulator which saturates the sums at symmetric 32-bit bounds (see @ref saturation).
 *
 * After all taps have been accumulated, a rounding arithmetic right-shift of `shift` bits is
 * applied to the 64-bit sum, and the final result is saturated to the symmetric 16-bit range
 * (the open interval @math{(-2^{15}, 2^{15})}).
 *
 * Below is a more detailed description of the operations performed (not including the saturation
 * logic applied by the accumulators).
 * 
 * @f[
 * &    y[t] = sat_{16} \left(
 *               round \left( 
 *                 \left(
 *                   \sum_{k=0}^{N-1} round(x[t-k] \cdot b[k]) 
 *                 \right) \cdot 2^{-shift}
 *               \right)
 *             \right)                                                          \\
 * &        \qquad\text{where } sat_{32}() \text{ saturates to } \pm(2^{15}-1)  \\
 * &        \qquad\text{  and } round() \text{ rounds to the nearest integer, with ties rounding towards } +\!\infty
 * @f]
 * @endparblock
 * 
 * @par Operations
 * @parblock
 * 
 * **Initialize**: A `filter_fir_s16_t` filter is initialized with a call to filter_fir_s16_init().
 * The caller supplies information about the filter, including the number of taps and pointers the
 * coefficients and a state buffer. It is typically recommended that the state buffer be cleared to
 * all `0`s before initializing.
 *
 * **Add Sample**: To add a new input sample without computing a new output sample, use
 * filter_fir_s16_add_sample(). Unlike filter_fir_s32_add_sample(), this is not a constant-time
 * operation, and does depend on the number of filter taps. Nevertheless, this is faster than
 * computing output samples, and may be useful in some situations, for example, to moer quickly
 * pre-load the filter's state buffer with multiple samples, without incurring the cost of computing
 * an output with each added sample.
 *
 * **Process Sample**: To process a new input sample and produce a new output sample, use
 * filter_fir_s16().
 * @endparblock
 * 
 * @par Fields
 * @parblock
 * 
 * After initialization via filter_fir_s16_init(), the contents of the `filter_fir_s16_t` struct are
 * considered to be opaque, and may change between major versions. In general, user code should not
 * need to access its members.
 *
 * `num_taps` is the order of the filter, or the number of taps. It is also the (minimum) size of
 * the buffers to which `coef` and `state` point, in elements (where each element is 2 bytes). The
 * time required to process an input sample and produce an output sample is approximately linear in
 * `num_taps` (see Performance below). 
 *
 * `shift` is the unsigned arithmetic rounding saturating right-shift applied to internal
 * accumulator to get a final output.
 *
 * `coef` is a pointer to a buffer (supplied by the user at initialization) containing the tap
 * coefficients. The coefficients are stored in forward order, with lower indices corresponding to
 * newer samples. `coef[0]`, then, corresponds to `b[0]`, `coef[1]` to `b[1]`, and so on. None of
 * the functions which operate on `filter_fir_s16_t` structs in this library will modify the
 * contents of the buffer to which `coef` points. This buffer must be at least `num_taps` elements
 * long, and must begin at a word-aligned address.
 *
 * `state` is a pointer to a buffer (supplied by the user at initialization) containing the state
 * data -- a history of the `num_taps` most recent input samples. `state` must begin at a
 * word-aligned address.
 * @endparblock
 * 
 * 
 * @par Coefficient Scaling
 * @parblock
 * 
 * @endparblock
 * 
 * @par Filter Conversion
 * @parblock
 * 
 * This library includes a python script which converts existing floating-point FIR filter 
 * coefficients into a suitable representation and generates code for easily initializing and
 * executing the filter. See @ref filter_conversion for more.
 * @endparblock
 * 
 * @par Usage Example
 * @parblock
 * 
 * @todo
 * 
 * @endparblock
 * 
 * 
 * @see filter_fir_s16_init, 
 *      filter_fir_s16_add_sample,
 *      filter_fir_s16
 * 
 * @ingroup filter_api
 */
C_API
typedef struct {
    /**
     * The number of taps in the FIR filter.
     * 
     * This is also the number of elements in `state` and `coef`
     */
    unsigned num_taps;

    /**
     * Unsigned arithmetic rounding right-shift applied to accumulator when computing filter output.
     */
    right_shift_t shift;

    /**
     * Pointer to a buffer containing the filter coefficients. Must point to word-aligned address.
     */
    int16_t* coef;

    /**
     * Pointer to a circular buffer containing the previous input samples. Must point to
     * word-aligned address.
     */
    int16_t* state;
} filter_fir_s16_t;

/**
 * @brief Initialize a 16-bit FIR filter.
 * 
 * Before filter_fir_s16() or filter_fir_s16_add_sample() can be used on a filter it must be
 * initialized with a call to this function.
 *
 * `sample_buffer` and `coefficients` must be at least `2 * tap_count` bytes long, and aligned to a
 * 4-byte (word) boundary.
 * 
 * See `filter_fir_s16_t` for more information about 16-bit FIR filters and their operation.
 * 
 * @param[out] filter           Filter struct to be initialized
 * @param[in]  sample_buffer    Buffer used by the filter to contain state information. Must be at least `tap_count`
 *                              elements long
 * @param[in]  tap_count        Order of the FIR filter; number of filter taps
 * @param[in]  coefficients     Array containing filter coefficients
 * @param[in]  shift            Unsigned arithmetic right-shift applied to accumulator to get filter output sample
 * 
 * @see filter_fir_s16_t
 * 
 * @ingroup filter_api
 */
C_API
void filter_fir_s16_init(
    filter_fir_s16_t* filter,
    int16_t* sample_buffer,
    const unsigned tap_count,
    const int16_t* coefficients,
    const right_shift_t shift);

/**
 * @brief Add a new input sample to a 16-bit FIR filter without processing an output sample.
 * 
 * This function adds a new input sample to `filter`'s state without computing a new output sample.
 * 
 * See `filter_fir_s16_t` for more information about FIR filters and their operation.
 * 
 * @param[inout] filter         Filter struct to have the sample added
 * @param[in]    new_sample     Sample to be added to `filter`'s history
 * 
 * @see filter_fir_s16_t
 * 
 * @ingroup filter_api
 */
C_API
void filter_fir_s16_add_sample(
    filter_fir_s16_t* filter,
    const int16_t new_sample);
    
/**
 * This function implements a Finite Impulse Response (FIR) filter. 
 * 
 * The new input sample `new_sample` is added to this filter's state, and a new output sample is
 * computed and returned as specified in `filter_fir_s16_t`.
 * 
 * With a large number of filter taps, this function takes approximately 3 thread cycles per 16
 * filter taps.
 * 
 * @param[inout]    filter          Filter to be processed
 * @param[in]       new_sample      New input sample to be processed by `filter`
 * 
 * @returns     Next filtered output sample
 * 
 * @see filter_fir_s16_t
 * 
 * @ingroup filter_api
 */
C_API
int16_t filter_fir_s16(
    filter_fir_s16_t* filter,
    const int16_t new_sample);


/**
 * @brief A biquad filter block
 * 
 * Contains the coeffient and state information for a cascade of up to 8 biquad filter sections. 
 * 
 * To process a new input sample, filter_biquad_s32() can be used with a pointer to one of these
 * structs.
 * 
 * For longer cascades, an array of `filter_biquad_s32_t` structs can be used with
 * filter_biquads_s32().
 * 
 * @par Filter Conversion
 * @parblock
 * 
 * This library includes a python script which converts existing floating-point cascaed biquad
 * filter coefficients into a suitable representation and generates code for easily initializing 
 * and executing the filter. See @ref filter_conversion for more.
 * @endparblock
 * 
 * @ingroup filter_api
 */
C_API
typedef struct {
    /**
     * The number of biquad sections in this biquad filter block.
     */
    unsigned biquad_count;

    /**
     * Contains filter state inforamtion.
     * 
     * state[j][k] is the value x_k[j], i.e.  x[n-j] of the kth biquad. x[j][8] are outputs of 8th
     * biquad
     */
    int32_t state[2][9];

    /**
     * Coefficients for the biquad sections.
     * 
     * The second index corresponds to the biquad section.
     * 
     * The first index corresponds to coefficients @math{b_0}, @math{b_1}, @math{b_2}, @math{-a_1}
     * and @math{-a_2}, in that order.
     */
    int32_t coef[5][8];  // coefficients. coef[j][k] is for the kth biquad. j maps to b0,b1,b2,-a1,-a2.
} filter_biquad_s32_t;

/**
 * This function implements a 32-bit Biquad filter. 
 * 
 * The new input sample `new_sample` is added to this filter's state, and a new output sample is
 * computed and returned as specified in `filter_biquad_s32_t`.
 * 
 * This function processes a single filter block containing (up to) 8 biquad filter sections. For
 * biquad filters containing 2 or more filter blocks (more than 8 biquad filter sections), see
 * filter_biquads_s32().
 * 
 * @param[inout]    filter          Filter to be processed
 * @param[in]       new_sample      New input sample to be processed by `filter`
 * 
 * @returns     Next filtered output sample
 * 
 * @see filter_biquad_s32_t, 
 *      filter_biquads_s32
 * 
 * @ingroup filter_api
 */
C_API
int32_t filter_biquad_s32(
    filter_biquad_s32_t* filter,
    const int32_t new_sample);


/**
 * This function implements a 32-bit Biquad filter. 
 * 
 * The new input sample `new_sample` is added to this filter's state, and a new output sample is
 * computed and returned as specified in `filter_biquad_s32_t`.
 * 
 * This function processes one or more filter blocks, with each block containing up to 8 biquad
 * filter sections.
 * 
 * @param[inout]    biquads         Filter blocks to be processed
 * @param[in]       block_count     Number of filter blocks in `biquads`
 * @param[in]       new_sample      New input sample to be processed by `filter`
 * 
 * @returns     Next filtered output sample
 * 
 * @see filter_biquad_s32_t, 
 *      filter_biquad_s32
 * 
 * @ingroup filter_api
 */
C_API
int32_t filter_biquads_s32(
    filter_biquad_s32_t biquads[],
    const unsigned block_count,
    const int32_t new_sample);
    

#ifdef __XC__
} // extern "C"
#endif