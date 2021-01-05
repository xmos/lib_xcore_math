
#ifndef XS3_MATH_TYPES_H_
#define XS3_MATH_TYPES_H_

#include <stdint.h>


#ifdef __XC__
extern "C" {
#endif


/** 
 * @brief A complex number with a 64-bit real part and 64-bit imaginary part.
 */
typedef struct {
    int64_t re; ///< Real Part
    int64_t im; ///< Imaginary Part
} complex_s64_t;

/** 
 * @brief A complex number with a 32-bit real part and 32-bit imaginary part.
 */
typedef struct {
    int32_t re; ///< Real Part
    int32_t im; ///< Imaginary Part
} complex_s32_t;

/** 
 * @brief A complex number with a 16-bit real part and 16-bit imaginary part.
 */
typedef struct {
    int16_t re; ///< Real Part
    int16_t im; ///< Imaginary Part
} complex_s16_t;

/**
 * @brief A complex number with a single-precision floating-point real part and a single-precision floating-point imaginary part.
 */
typedef struct {
    float re;   ///< Real Part
    float im;   ///< Imaginary Part
} complex_float_t;

/**
 * @brief A complex number with a double-precision floating-point real part and a double-precision floating-point imaginary part.
 */
typedef struct {
    double re;  ///< Real Part
    double im;  ///< Imaginary Part
} complex_double_t;

/** 
 * @brief A pair of 32-bit samples, associated with channels A and B.
 */
typedef struct {
    int32_t ch_a;   ///< Channel A
    int32_t ch_b;   ///< Channel B
} ch_pair_s32_t;

/** 
 * @brief A pair of 16-bit samples, associated with channels A and B.
 */
typedef struct {
    int16_t ch_a;   ///< Channel A
    int16_t ch_b;   ///< Channel B
} ch_pair_s16_t;

/**
 * @brief A pair of single-precision floating-point samples, associated with channels A and B.
 */
typedef struct {
    float ch_a; ///< Channel A
    float ch_b; ///< Channel B
} ch_pair_float_t;

/**
 * @brief A pair of double-precision floating-point samples, associated with channels A and B.
 */
typedef struct {
    double ch_a;    ///< Channel A
    double ch_b;    ///< Channel B
} ch_pair_double_t;


/**
 * @brief An exponent.
 * 
 * Many places in this API make use of integers representing the exponent associated with some floating-point value or 
 * block floating-point vector.
 * 
 * For a floating-point value @math{x \cdot 2^p}, @math{p} is the exponent, and may usually be positive or negative.
 */
typedef int exponent_t;

/**
 * @brief Headroom of some integer or integer array.
 * 
 * Represents the headroom of a signed or unsigned integer, complex integer or channel pair, or the headroom of the 
 * mantissa array of a block floating-point vector.
 */
typedef unsigned headroom_t;

/**
 * @brief A rightwards arithmetic bit-shift.
 * 
 * Represents a right bit-shift to be applied to an integer. May be signed or unsigned, depending on context. If signed,
 * negative values represent leftward bit-shifts.
 * 
 * @see left_shift_t
 */
typedef int right_shift_t;

/**
 * @brief A leftwards arithmetic bit-shift.
 * 
 * Represents a left bit-shift to be applied to an integer. May be signed or unsigned, depending on context. If signed,
 * negative values represent rightward bit-shifts.
 * 
 * @see right_shift_t
 */
typedef int left_shift_t;


/**
 * @brief A block floating-point vector of 32-bit elements.
 * 
 * Initialized with the ``bfp_s32_init()`` function.
 * 
 * The logical quantity represented by each element of this vector is:
 *      ``data[i]*2^(exp)``
 *      where the multiplication and exponentiation are using real (non-modular) arithmetic.
 * 
 * The BFP API keeps the ``hr`` field up-to-date with the current headroom of ``data[]`` so as to
 * minimize precision loss as elements become small.
 */
//! [bfp_s32_t]
typedef struct {
    /** Pointer to the underlying element buffer.*/
    int32_t* data;
    /** Exponent associated with the vector. */
    exponent_t exp;
    /** Current headroom in the ``data[]`` */
    headroom_t hr;
    /** Current size of ``data[]``, expressed in elements */
    unsigned length;
} bfp_s32_t;
//! [bfp_s32_t]

// astew: The tags around these structs are so that they can be copied into the documentation. Unfortunately it appears
//        to mess with the documentation in a way that I'm not sure how to fix.

/**
 * @brief A block floating-point vector of 16-bit elements.
 * 
 * Initialized with the ``bfp_s16_init()`` function.
 * 
 * The logical quantity represented by each element of this vector is:
 *      ``data[i] * 2^(exp)``
 *      where the multiplication and exponentiation are using real (non-modular) arithmetic.
 * 
 * The BFP API keeps the ``hr`` field up-to-date with the current headroom of ``data[]`` so as to
 * minimize precision loss as elements become small.
 */
//! [bfp_s16_t]
typedef struct {
    /** Pointer to the underlying element buffer.*/
    int16_t* data;
    /** Exponent associated with the vector. */
    exponent_t exp;
    /** Current headroom in the ``data[]`` */
    headroom_t hr;
    /** Current size of ``data[]``, expressed in elements */
    unsigned length;
} bfp_s16_t;
//! [bfp_s16_t]

/**
 * @brief A block floating-point vector of complex 32-bit elements.
 * 
 * Initialized with the ``bfp_complex_s32_init()`` function.
 * 
 * The logical quantity represented by each element of this vector is:
 *      ``data[k].re * 2^(exp) + i * data[k].im * 2^(exp)``
 *      where the multiplication and exponentiation are using real (non-modular) arithmetic, and
 *      i is sqrt(-1)
 * 
 * The BFP API keeps the ``hr`` field up-to-date with the current headroom of ``data[]`` so as to
 * minimize precision loss as elements become small.
 */
//! [bfp_complex_s32_t]
typedef struct {
    /** Pointer to the underlying element buffer.*/
    complex_s32_t* data;
    /** Exponent associated with the vector. */
    exponent_t exp;
    /** Current headroom in the ``data[]`` */
    headroom_t hr;
    /** Current size of ``data[]``, expressed in elements */
    unsigned length;
} bfp_complex_s32_t;
//! [bfp_complex_s32_t]

/**
 * @brief A block floating-point vector of complex 16-bit elements.
 * 
 * Initialized with the ``bfp_complex_s16_init()`` function.
 * 
 * The logical quantity represented by each element of this vector is:
 *      ``data[k].re * 2^(exp) + i * data[k].im * 2^(exp)``
 *      where the multiplication and exponentiation are using real (non-modular) arithmetic, and
 *      i is sqrt(-1)
 * 
 * The BFP API keeps the ``hr`` field up-to-date with the current headroom of ``data[]`` so as to
 * minimize precision loss as elements become small.
 */
//! [bfp_complex_s16_t]
typedef struct {
    /** Pointer to the underlying element buffer.*/
    int16_t* real;
    /** Pointer to the underlying element buffer.*/
    int16_t* imag;
    /** Exponent associated with the vector. */
    exponent_t exp;
    /** Current headroom in the ``data[]`` */
    headroom_t hr;
    /** Current size of ``data[]``, expressed in elements */
    unsigned length;
} bfp_complex_s16_t;
//! [bfp_complex_s16_t]

/**
 * @brief A block floating-point vector of 32-bit channel pairs.
 * 
 * Initialized with the ``bfp_ch_pair_s32_init()`` function.
 * 
 * The ``data[].ch_a`` elements collectively represent a sequence of samples associated with one 
 * channel, while the ``data[].ch_b`` elements collectively represent a sequence of samples associated
 * with another.
 * 
 * The logical quantity represented by each element (for channel a) of this vector is:
 *      ``data[k].ch_a * 2^(exp)``
 *      where the multiplication and exponentiation are using real (non-modular) arithmetic
 * Likewise for channel b.
 * 
 * The BFP API keeps the ``hr`` field up-to-date with the current headroom of ``data[]`` so as to
 * minimize precision loss as elements become small.
 */
//! [bfp_ch_pair_s32_t]
typedef struct {
    /** Pointer to the underlying element buffer.*/
    ch_pair_s32_t* data;
    /** Exponent associated with the vector. */
    exponent_t exp;
    /** Current headroom in the ``data[]`` */
    headroom_t hr;
    /** Current size of ``data[]``, expressed in elements */
    unsigned length;
} bfp_ch_pair_s32_t;
//! [bfp_ch_pair_s32_t]

/**
 * @brief A block floating-point vector of 16-bit channel pairs.
 * 
 * Initialized with the ``bfp_ch_pair_s16_init()`` function.
 * 
 * The ``data[].ch_a`` elements collectively represent a sequence of samples associated with one 
 * channel, while the ``data[].ch_b`` elements collectively represent a sequence of samples associated
 * with another.
 * 
 * The logical quantity represented by each element (for channel a) of this vector is:
 *      ``data[k].ch_a * 2^(exp)``
 *      where the multiplication and exponentiation are using real (non-modular) arithmetic
 * Likewise for channel b.
 * 
 * The BFP API keeps the ``hr`` field up-to-date with the current headroom of ``data[]`` so as to
 * minimize precision loss as elements become small.
 */
//! [bfp_ch_pair_s16_t]
typedef struct {
    /** Pointer to the underlying element buffer.*/
    ch_pair_s16_t* data;
    /** Exponent associated with the vector. */
    exponent_t exp;
    /** Current headroom in the ``data[]`` */
    headroom_t hr;
    /** Current size of ``data[]``, expressed in elements */
    unsigned length;
} bfp_ch_pair_s16_t;
//! [bfp_ch_pair_s16_t]


#ifdef __XC__
}   //extern "C"
#endif

#endif //XS3_MATH_TYPES_H_