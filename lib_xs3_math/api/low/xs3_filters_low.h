
#ifndef XS3_BFP_FILTERS_LOW_H_
#define XS3_BFP_FILTERS_LOW_H_

#include "xs3_math_types.h"

#ifdef __XC__
extern "C" {
#endif

#define DSP_NUM_STATES_PER_BIQUAD   (5)
#define DSP_NUM_COEFFS_PER_BIQUAD   (5)


/** Process a single sample of a 32-bit FIR filter.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Accepts a single input sample and returns a single output sample.
 * 
 * The ``filter_data`` array must have a size of at least ``num_taps - 1``.
 * Coefficients are stored in forward order (e.g. ``b0, b1, b2, ...``).
 * 
 * The filter coefficients must be given in a fixed-point format given by ``q_format``. This means 
 * that the logical coefficient values are given by  ``filter_coeffs[i] * 2^(-q_format)``.
 * 
 * The output sample value will have the same implied exponent as the input sample.
 * 
 * The output sample value saturates to the range ( -(2^32)+1, (2^32)-1 ).
 * 
 * \param input_sample      New input sample
 * \param state_data        Filter state data
 * \param filter_coeffs     Array of filter coefficients
 * \param num_taps          Number of filter taps -- length of ``filter_coeffs``
 * \param q_format          Fixed-point format of coefficients
 * 
 * \returns                 The filter output, with the same (implied) exponent as the input.
 */
int32_t xs3_filters_fir(
    const int32_t input_sample, 
    int32_t state_data[], 
    const int32_t filter_coeffs[], 
    const unsigned num_taps,
    const unsigned q_format);

/** Add a single sample to a 32-bit FIR filter's state data without processing
 * the full FIR filter.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * The ``filter_data`` array must have a size of at least ``num_taps - 1``.
 * 
 * \param input_sample  New input sample
 * \param state_data    Filter state data
 * \param num_taps      Number of filter taps.
 */
void xs3_filters_fir_add_sample(
    const int32_t input_sample, 
    int32_t state_data[], 
    const unsigned num_taps);

/** Process a single sample of an interpolating 32-bit FIR filter.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Accepts a single input sample and returns ``interp_factor`` output samples.
 * 
 * The ``filter_data`` array must have a size of at least ``num_taps - 1``.
 * The filter coefficients have an implied exponent of -31 (i.e. in a Q0.31 fixed-point format). 
 * Coefficients are stored in forward order (e.g. ``b0, b1, b2, ...``).
 * 
 * The filter coefficients must be given in a fixed-point format given by ``q_format``. This means 
 * that the logical coefficient values are given by  ``filter_coeffs[i] * 2^(-q_format)``.
 * 
 * The output sample values will have the same implied exponent as the input sample.
 * 
 * The output sample value saturates to the range ( -(2^32)+1, (2^32)-1 ).
 * 
 * \param input_sample      New input sample
 * \param state_data        Filter state data
 * \param filter_coeffs     Array of filter coefficients
 * \param num_taps          Number of filter taps -- length of ``filter_coeffs``
 * \param interp_factor     Interpolation factor
 * \param output_samples    Output samples
 * \param q_format          Fixed-point format of coefficients
 */
void xs3_filters_interpolate(
    const int32_t input_sample,
    int32_t state_data[],
    const int32_t filter_coeffs[],
    const unsigned num_taps,
    const unsigned interp_factor,
    const unsigned q_format,
    int32_t output_samples[]);


/** Process the next samples of a decimating 32-bit FIR filter.
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Accepts ``decim_factor`` input samples and returns a single output sample.
 * 
 * The decimation ratio is given by ``decim_factor``.
 * 
 * The ``filter_data`` array must have a size of at least ``num_taps - 1``.
 * The filter coefficients have an implied exponent of -31 (i.e. in a Q0.31 fixed-point format). 
 * Coefficients are stored in forward order (e.g. ``b0, b1, b2, ...``).
 * 
 * The filter coefficients must be given in a fixed-point format given by ``q_format``. This means 
 * that the logical coefficient values are given by  ``filter_coeffs[i] * 2^(-q_format)``.
 * 
 * The output sample value will have the same implied exponent as the input samples.
 * 
 * The output sample value saturates to the range ( -(2^32)+1, (2^32)-1 ).
 * 
 * \param input_samples      New input sample
 * \param state_data        Filter state data
 * \param filter_coeffs     Array of filter coefficients
 * \param num_taps          Number of filter taps -- length of ``filter_coeffs``
 * \param decim_factor      Decimation factor
 * \param q_format          Fixed-point format of coefficients
 */
int32_t xs3_filters_decimate(
    const int32_t input_samples[],
    int32_t state_data[],
    const int32_t filter_coeffs[],
    const unsigned num_taps,
    const unsigned decim_factor,
    const unsigned q_format);

/** Process one new sample through a second order IIR filter (direct form I).
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Accepts a single input sample and returns a single output sample.
 * 
 * The IIR filter algorithm executes a difference equation on current and past input values x
 *  and past output values y:
 *  ``y[n] = x[n]*b0 + x[n-1]*b1 + x[n-2]*b2 + y[n-1]*-a1 + y[n-2]*-a2``
 * 
 * The filter coefficients are stored in forward order (e.g. ``b0,b1,b2,-a1,-a2``).
 * 
 * The filter coefficients must be given in a fixed-point format given by ``q_format``. This means 
 * that the logical coefficient values are given by  ``filter_coeffs[i] * 2^(-q_format)``.
 * 
 * The output sample value will have the same implied exponent as the input samples.
 * 
 * The output sample value saturates to the range ( -(2^32)+1, (2^32)-1 ).
 *
 * \param input_sample      New input sample
 * \param state_data        Filter state data
 * \param filter_coeffs     Array of filter coefficients
 * \param q_format          Fixed-point format of coefficients
 * 
 * \returns                 Output filtered sample
 */
int32_t xs3_filters_biquad(
    const int32_t input_sample,
    int32_t state_data[DSP_NUM_STATES_PER_BIQUAD],
    const int32_t filter_coeffs[DSP_NUM_COEFFS_PER_BIQUAD],
    const unsigned q_format);


/** Process one new sample through a cascaded direct form I biquad filters
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 * 
 * Accepts a single input sample and returns a single output sample.
 *
 *  The IIR filter algorithm executes a difference equation on current and past input values x
 *  and past output values y:
 *  ``y[n] = x[n]*b0 + x[n-1]*b1 + x[n-2]*b2 + y[n-1]*-a1 + y[n-2]*-a2``
 *
 *  The filter coefficients are stored in forward order
 *  (e.g. ``section1:b0,b1,b2,-a1,-a2,sectionN:b0,b1,b2,-a1,-a2``).
 * 
 * The filter coefficients must be given in a fixed-point format given by ``q_format``. This means 
 * that the logical coefficient values are given by  ``filter_coeffs[i] * 2^(-q_format)``.
 * 
 * The output sample value will have the same implied exponent as the input samples.
 * 
 * The output sample value saturates to the range ( -(2^32)+1, (2^32)-1 ).
 * 
 * \param input_sample      New input sample
 * \param filter_coeffs     Array of filter coefficients
 * \param state_data        Filter state data
 * \param num_sections      Number of biquad filters in sequence
 * \param q_format          Fixed-point format of coefficients
 * 
 * \returns                 Output filtered sample
 */
int32_t xs3_filters_biquads(
    int32_t input_sample,
    const int32_t filter_coeffs[],
    int32_t state_data[],
    const unsigned num_sections,
    const unsigned q_format);



#ifdef __XC__
}   //extern "C"
#endif

#endif //XS3_BFP_FILTERS_LOW_H_