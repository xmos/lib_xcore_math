// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include <stdint.h>

#include "xs3_api.h"



/**
 * @page page_xs3_math_types_h  xs3_math_types.h
 * 
 * This header contains definitions for most of the library-defined types which are used
 * throughout this API.  Several other types (such as those used for filtering) are defined
 * locally if they're only used within a single header.
 * 
 * @note This header is included automatically through `xs3_math.h` or `bfp_math.h`.
 * 
 * @ingroup xs3_math_header_file
 */



/**
 * @defgroup type_scalar  lib_xs3_math Scalar Types
 * @defgroup type_bfp     lib_xs3_math Block Floating-Point Types
 * @defgroup type_misc    lib_xs3_math Misc Types
 */

/** 
 * @brief A complex number with a 64-bit real part and 64-bit imaginary part.
 * 
 * @ingroup type_scalar
 */
C_TYPE
typedef struct {
    int64_t re; ///< Real Part
    int64_t im; ///< Imaginary Part
} complex_s64_t;

/** 
 * @brief A complex number with a 32-bit real part and 32-bit imaginary part.
 * 
 * @ingroup type_scalar
 */
C_TYPE
typedef struct {
    int32_t re; ///< Real Part
    int32_t im; ///< Imaginary Part
} complex_s32_t;

/** 
 * @brief A complex number with a 16-bit real part and 16-bit imaginary part.
 * 
 * @ingroup type_scalar
 */
C_TYPE
typedef struct {
    int16_t re; ///< Real Part
    int16_t im; ///< Imaginary Part
} complex_s16_t;


/**
 * @brief An exponent.
 * 
 * Many places in this API make use of integers representing the exponent associated with some floating-point value or 
 * block floating-point vector.
 * 
 * For a floating-point value @math{x \cdot 2^p}, @math{p} is the exponent, and may usually be positive or negative.
 * 
 * @ingroup type_scalar
 */
typedef int exponent_t;

/**
 * @brief Headroom of some integer or integer array.
 * 
 * Represents the headroom of a signed or unsigned integer, complex integer or channel pair, or the headroom of the 
 * mantissa array of a block floating-point vector.
 * 
 * @ingroup type_scalar
 */
typedef unsigned headroom_t;

/**
 * @brief A rightwards arithmetic bit-shift.
 * 
 * Represents a right bit-shift to be applied to an integer. May be signed or unsigned, depending on context. If signed,
 * negative values represent leftward bit-shifts.
 * 
 * @see left_shift_t
 * 
 * @ingroup type_scalar
 */
typedef int right_shift_t;

/**
 * @brief A leftwards arithmetic bit-shift.
 * 
 * Represents a left bit-shift to be applied to an integer. May be signed or unsigned, depending on context. If signed,
 * negative values represent rightward bit-shifts.
 * 
 * @see right_shift_t
 * 
 * @ingroup type_scalar
 */
typedef int left_shift_t;


/**
 * @brief A 32-bit fixed-point scalar.
 * 
 * Represents a 32-bit fixed-point scalar with a Q-format implied by the context in which it occurs.
 * Typically this type will be used for fixed-point function parameters as a hint to the user to
 * check the documentation for the required Q-format.
 * 
 * If a function has a `fixed_s32_t` parameter ending with `_qXX` (where the `X` are digits), the
 * `XX` typically indicates the number of fractional bits. For example, a `fixed_s32_t` parameter
 * called `coef_q30` would imply 30 fractional bits and an associated exponent of -30.  This is just
 * a convention, however, and this interpretation should be verified in the function's
 * documentation.
 * 
 * @ingroup type_scalar
 */
typedef int32_t fixed_s32_t;

/**
 * @brief A 16-bit fixed-point scalar.
 * 
 * Represents a 16-bit fixed-point scalar with a Q-format implied by the context in which it occurs.
 * Typically this type will be used for fixed-point function parameters as a hint to the user to
 * check the documentation for the required Q-format.
 * 
 * If a function has a `fixed_s16_t` parameter ending with `_qXX` (where the `X` are digits), the
 * `XX` typically indicates the number of fractional bits. For example, a `fixed_s16_t` parameter
 * called `coef_q14` would imply 14 fractional bits and an associated exponent of -14.  This is just
 * a convention, however, and this interpretation should be verified in the function's
 * documentation.
 * 
 * @ingroup type_scalar
 */
typedef int16_t fixed_s16_t;



/**
 * @brief A floating-point scalar with a 32-bit mantissa.
 * 
 * Represents a (non-standard) floating-point value given by @math{ M \cdot 2^{x} }, where @math{M} is the 32-bit 
 * mantissa `mant`, and @math{x} is the exponent `exp`.
 * 
 * To convert a `float_s32_t` to a standard IEEE754 single-precision floating-point value (which may result in a loss of
 * precision):
 * 
 * @code{.c}
 *  float to_ieee_float(float_s32_t x) {
 *      return ldexpf(x.mant, x.exp);
 *  }
 * @endcode
 * 
 * @ingroup type_scalar
 */
C_TYPE
typedef struct {
    int32_t mant;       ///< 32-bit mantissa
    exponent_t exp;     ///< exponent
} float_s32_t;

/**
 * @brief A floating-point scalar with a 64-bit mantissa.
 * 
 * Represents a (non-standard) floating-point value given by @math{ M \cdot 2^{x} }, where @math{M} is the 64-bit 
 * mantissa `mant`, and @math{x} is the exponent `exp`.
 * 
 * To convert a `float_s64_t` to a standard IEEE754 double-precision floating-point value (which may result in a loss of
 * precision):
 * 
 * @code{.c}
 *  double to_ieee_float(float_s64_t x) {
 *      return ldexp(x.mant, x.exp);
 *  }
 * @endcode
 * 
 * @ingroup type_scalar
 */
C_TYPE
typedef struct {
    int64_t mant;       ///< 64-bit mantissa
    exponent_t exp;     ///< exponent
} float_s64_t;

/**
 * @brief A complex floating-point scalar with a complex 16-bit mantissa.
 * 
 * Represents a (non-standard) complex floating-point value given by @math{ A + j\cdot B \cdot 2^{x} }, where @math{A} 
 * is `mant.re`, the 16-bit real part of the mantissa, @math{B} is `mant.im`, the 16-bit imaginary part of the mantissa,
 * and @math{x} is the exponent `exp`.
 * 
 * @ingroup type_scalar
 */
C_TYPE
typedef struct {
    complex_s16_t mant; ///< complex 16-bit mantissa
    exponent_t exp;     ///< exponent
} float_complex_s16_t;

/**
 * @brief A complex floating-point scalar with a complex 32-bit mantissa.
 * 
 * Represents a (non-standard) complex floating-point value given by @math{ A + j\cdot B \cdot 2^{x} }, where @math{A} 
 * is `mant.re`, the 32-bit real part of the mantissa, @math{B} is `mant.im`, the 32-bit imaginary part of the mantissa,
 * and @math{x} is the exponent `exp`.
 * 
 * @ingroup type_scalar
 */
C_TYPE
typedef struct {
    complex_s32_t mant; ///< complex 32-bit mantissa
    exponent_t exp;     ///< exponent
} float_complex_s32_t;

/**
 * @brief A complex floating-point scalar with a complex 64-bit mantissa.
 * 
 * Represents a (non-standard) complex floating-point value given by @math{ A + j\cdot B \cdot 2^{x} }, where @math{A} 
 * is `mant.re`, the 64-bit real part of the mantissa, @math{B} is `mant.im`, the 64-bit imaginary part of the mantissa,
 * and @math{x} is the exponent `exp`.
 * 
 * @ingroup type_scalar
 */
C_TYPE
typedef struct {
    complex_s64_t mant; ///< complex 64-bit mantissa
    exponent_t exp;     ///< exponent
} float_complex_s64_t;


C_TYPE
/**
 * @brief (Opaque) Flags field for BFP vectors.
 * 
 * @warning Users should not manually modify fields of this type, as it is intended to be opaque.
 * 
 * @ingroup type_bfp
 */
typedef enum {
  /** 
   * Indicates that BFP vector's mantissa buffer(s) were allocated dynamically 
   * 
   * This flag lets the bfp_*_dealloc() functions know whether the mantissa vector must
   * be free()ed.
  */
  BFP_FLAG_DYNAMIC  =  (1 << 0),
} bfp_flags_e;


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
 * 
 * @ingroup type_bfp
 */
//! [bfp_s32_t]
C_TYPE
typedef struct {
    /** Pointer to the underlying element buffer.*/
    int32_t* data;
    /** Exponent associated with the vector. */
    exponent_t exp;
    /** Current headroom in the ``data[]`` */
    headroom_t hr;
    /** Current size of ``data[]``, expressed in elements */
    unsigned length;
    /** BFP vector flags. Users should not normally modify these manually. */
    bfp_flags_e flags;
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
 * 
 * @ingroup type_bfp
 */
//! [bfp_s16_t]
C_TYPE
typedef struct {
    /** Pointer to the underlying element buffer.*/
    int16_t* data;
    /** Exponent associated with the vector. */
    exponent_t exp;
    /** Current headroom in the ``data[]`` */
    headroom_t hr;
    /** Current size of ``data[]``, expressed in elements */
    unsigned length;
    /** BFP vector flags. Users should not normally modify these manually. */
    bfp_flags_e flags;
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
 * 
 * @ingroup type_bfp
 */
//! [bfp_complex_s32_t]
C_TYPE
typedef struct {
    /** Pointer to the underlying element buffer.*/
    complex_s32_t* data;
    /** Exponent associated with the vector. */
    exponent_t exp;
    /** Current headroom in the ``data[]`` */
    headroom_t hr;
    /** Current size of ``data[]``, expressed in elements */
    unsigned length;
    /** BFP vector flags. Users should not normally modify these manually. */
    bfp_flags_e flags;
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
 * 
 * @ingroup type_bfp
 */
//! [bfp_complex_s16_t]
C_TYPE
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
    /** BFP vector flags. Users should not normally modify these manually. */
    bfp_flags_e flags;
} bfp_complex_s16_t;
//! [bfp_complex_s16_t]


// Some standard float types required by some of the unit tests. @todo This probably belongs elsewhere


/**
 * @brief A complex number with a single-precision floating-point real part and a single-precision floating-point imaginary part.
 * 
 * @ingroup type_misc
 */
C_TYPE
typedef struct {
    float re;   ///< Real Part
    float im;   ///< Imaginary Part
} complex_float_t;


/**
 * @brief A complex number with a double-precision floating-point real part and a double-precision floating-point imaginary part.
 * 
 * @ingroup type_misc
 */
C_TYPE
typedef struct {
    double re;  ///< Real Part
    double im;  ///< Imaginary Part
} complex_double_t;


/**
 * @brief Holds a set of sixteen 32-bit accumulators in the XS3 VPU's internal format.
 * 
 * The XS3 VPU stores 32-bit accumulators with the most significant 16-bits stored in one 256-bit vector register (called vD),
 * and the least significant 16-bit stored in another 256-bit register (called vR). This struct reflects that internal format,
 * and is occasionally used to store intermediate results.
 * 
 * @note `vR` is unsigned. This reflects the fact that a signed 16-bit integer `0xSTUVWXYZ` is always exactly 
 * `0x0000WXYZ` larger than `0xSTUV0000`.  To combine the upper and lower 16-bits of an accumulator, use 
 * `(((int32_t)vD[k]) << 16) + vR[k]`.
 * 
 * @ingroup type_misc
 */
C_TYPE
typedef struct {
  int16_t vD[16];   ///< Most significant 16 bits of accumulators
  uint16_t vR[16];  ///< Least significant 16 bits of accumulators
} xs3_split_acc_s32_t;


typedef int32_t q1_31;
typedef int32_t q2_30;
typedef int32_t q4_28;
typedef int32_t q8_24;

typedef uint32_t uq0_32;
typedef uint32_t uq1_31;
typedef uint32_t uq2_30;
typedef uint32_t uq4_28;
typedef uint32_t uq8_24;
