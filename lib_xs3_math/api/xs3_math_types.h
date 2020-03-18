
#ifndef XS3_MATH_TYPES_H_
#define XS3_MATH_TYPES_H_

#include <stdint.h>


#ifdef __XC__
extern "C" {
#endif

/** 
 * Represents a complex number with a 32-bit real part and 32-bit imaginary part.
 */
typedef struct {
    int32_t re;
    int32_t im;
} complex_s32_t;

/** 
 * Represents a complex number with a 16-bit real part and 16-bit imaginary part.
 */
typedef struct {
    int16_t re;
    int16_t im;
} complex_s16_t;

/**
 * Represents a complex floating-point number.
 */
typedef struct {
    float re;
    float im;
} complex_float_t;

/** 
 * Represents a pair of 32-bit samples, each associated with a different channel.
 */
typedef struct {
    int32_t ch_a;
    int32_t ch_b;
} ch_pair_s32_t;

/** 
 * Represents a pair of 16-bit samples, each associated with a different channel.
 */
typedef struct {
    int16_t ch_a;
    int16_t ch_b;
} ch_pair_s16_t;


/**
 * Represents an exponent for a block floating-point vector
 */
typedef int exponent_t;

/**
 * Represents the headroom in a signed or unsigned integer, or in
 * a block floating-point vector
 */
typedef unsigned headroom_t;

/**
 * Represents a number of bits to arithmetically right-shift an integer.
 */
typedef int right_shift_t;

/**
 * Represents a number of bits to arithmetically left-shift an integer.
 */
typedef int left_shift_t;


/**
 * Represents a block floating-point vector of 32-bit elements.
 * 
 * Initialized with the ``bfp_init_vect_s32()`` function.
 * 
 * The logical quantity represented by each element of this vector is:
 *      ``data[i]*2^(exp)``
 *      where the multiplication and exponentiation are using real (non-modular) arithmetic.
 * 
 * The BFP API keeps the ``hr`` field up-to-date with the current headroom of ``data[]`` so as to
 * minimize precision loss as elements become small.
 */
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

/**
 * Represents a block floating-point vector of 16-bit elements.
 * 
 * Initialized with the ``bfp_init_vect_s16()`` function.
 * 
 * The logical quantity represented by each element of this vector is:
 *      ``data[i] * 2^(exp)``
 *      where the multiplication and exponentiation are using real (non-modular) arithmetic.
 * 
 * The BFP API keeps the ``hr`` field up-to-date with the current headroom of ``data[]`` so as to
 * minimize precision loss as elements become small.
 */
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


// /**
//  * Represents a block floating-point vector of 8-bit elements.
//  * 
//  * Initialized with the ``bfp_init_vect_s8()`` function.
//  * 
//  * The logical quantity represented by each element of this vector is:
//  *      ``data[i] * 2^(exp)``
//  *      where the multiplication and exponentiation are using real (non-modular) arithmetic.
//  * 
//  * The BFP API keeps the ``hr`` field up-to-date with the current headroom of ``data[]`` so as to
//  * minimize precision loss as elements become small.
//  */
// typedef struct {
//     int8_t* data;
//     /** Exponent associated with the vector. */
//     int exp;
//     /** Current headroom in the ``data[]`` */
//     unsigned hr;
//     /** Current size of ``data[]``, expressed in elements */
//     unsigned length;
// } bfp_s8_t;




/**
 * Represents a block floating-point vector of complex 32-bit elements.
 * 
 * Initialized with the ``bfp_init_vect_complex_s32()`` function.
 * 
 * The logical quantity represented by each element of this vector is:
 *      ``data[k].re * 2^(exp) + i * data[k].im * 2^(exp)``
 *      where the multiplication and exponentiation are using real (non-modular) arithmetic, and
 *      i is sqrt(-1)
 * 
 * The BFP API keeps the ``hr`` field up-to-date with the current headroom of ``data[]`` so as to
 * minimize precision loss as elements become small.
 */
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

/**
 * Represents a block floating-point vector of complex 16-bit elements.
 * 
 * Initialized with the ``bfp_init_vect_complex_s16()`` function.
 * 
 * The logical quantity represented by each element of this vector is:
 *      ``data[k].re * 2^(exp) + i * data[k].im * 2^(exp)``
 *      where the multiplication and exponentiation are using real (non-modular) arithmetic, and
 *      i is sqrt(-1)
 * 
 * The BFP API keeps the ``hr`` field up-to-date with the current headroom of ``data[]`` so as to
 * minimize precision loss as elements become small.
 */
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

/**
 * Represents a block floating-point vector of 32-bit channel pairs.
 * 
 * Initialized with the ``bfp_init_vect_ch_pair_s32()`` function.
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

/**
 * Represents a block floating-point vector of 16-bit channel pairs.
 * 
 * Initialized with the ``bfp_init_vect_ch_pair_s16()`` function.
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


#ifdef __XC__
}   //extern "C"
#endif

#endif //XS3_MATH_TYPES_H_