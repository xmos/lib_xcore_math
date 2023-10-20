// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include <stdint.h>

#include "xmath/api.h"

#ifdef __XC__
extern "C" {
#endif

// Disable warning messages C4820:
// warning C4820: '<unnamed-tag>': '4' bytes padding added after data member XYZ
#ifdef _WIN32
#pragma warning( disable : 4820)
#endif

/**
 * @defgroup type_scalar_int      xmath Scalar Types (Integer)
 * @defgroup type_scalar_float    xmath Scalar Types (Floating-point)
 * @defgroup type_scalar_fixed    xmath Scalar Types (Fixed-point)
 * @defgroup type_bfp             xmath Block Floating-Point Types
 * @defgroup type_misc            xmath Misc Types
 */

/**
 * @brief A complex number with a 64-bit real part and 64-bit imaginary part.
 *
 * @ingroup type_scalar_int
 */
C_TYPE
typedef struct {
    int64_t re; ///< Real Part
    int64_t im; ///< Imaginary Part
} complex_s64_t;

/**
 * @brief A complex number with a 32-bit real part and 32-bit imaginary part.
 *
 * @ingroup type_scalar_int
 */
C_TYPE
typedef struct {
    int32_t re; ///< Real Part
    int32_t im; ///< Imaginary Part
} complex_s32_t;

/**
 * @brief A complex number with a 16-bit real part and 16-bit imaginary part.
 *
 * @ingroup type_scalar_int
 */
C_TYPE
typedef struct {
    int16_t re; ///< Real Part
    int16_t im; ///< Imaginary Part
} complex_s16_t;


/**
 * @brief An exponent.
 *
 * Many places in this API make use of integers representing the exponent associated with some
 * floating-point value or block floating-point vector.
 *
 * For a floating-point value @math{x \cdot 2^p}, @math{p} is the exponent, and may usually be
 * positive or negative.
 *
 * @ingroup type_scalar_int
 */
typedef int exponent_t;

/**
 * @brief Headroom of some integer or integer array.
 *
 * Represents the headroom of a signed or unsigned integer, complex integer or channel pair, or the
 * headroom of the mantissa array of a block floating-point vector.
 *
 * @ingroup type_scalar_int
 */
typedef unsigned headroom_t;

/**
 * @brief A rightwards arithmetic bit-shift.
 *
 * Represents a right bit-shift to be applied to an integer. May be signed or unsigned, depending on
 * context. If signed, negative values represent leftward bit-shifts.
 *
 * @see left_shift_t
 *
 * @ingroup type_scalar_int
 */
typedef int right_shift_t;

/**
 * @brief A leftwards arithmetic bit-shift.
 *
 * Represents a left bit-shift to be applied to an integer. May be signed or unsigned, depending on
 * context. If signed, negative values represent rightward bit-shifts.
 *
 * @see right_shift_t
 *
 * @ingroup type_scalar_int
 */
typedef int left_shift_t;



/**
 * @brief A floating-point scalar with a 32-bit mantissa.
 *
 * Represents a (non-standard) floating-point value given by @math{ M \cdot 2^{x} }, where @math{M}
 * is the 32-bit mantissa `mant`, and @math{x} is the exponent `exp`.
 *
 * To convert a `float_s32_t` to a standard IEEE754 single-precision floating-point value (which may
 * result in a loss of precision):
 *
 * @code{.c}
 *  float to_ieee_float(float_s32_t x) {
 *      return ldexpf(x.mant, x.exp);
 *  }
 * @endcode
 *
 * @ingroup type_scalar_float
 */
C_TYPE
typedef struct {
    int32_t mant;       ///< 32-bit mantissa
    exponent_t exp;     ///< exponent
} float_s32_t;

/**
 * @brief A floating-point scalar with a 64-bit mantissa.
 *
 * Represents a (non-standard) floating-point value given by @math{ M \cdot 2^{x} }, where @math{M}
 * is the 64-bit mantissa `mant`, and @math{x} is the exponent `exp`.
 *
 * To convert a `float_s64_t` to a standard IEEE754 double-precision floating-point value (which may
 * result in a loss of precision):
 *
 * @code{.c}
 *  double to_ieee_float(float_s64_t x) {
 *      return ldexp(x.mant, x.exp);
 *  }
 * @endcode
 *
 * @ingroup type_scalar_float
 */
C_TYPE
typedef struct {
    int64_t mant;       ///< 64-bit mantissa
    exponent_t exp;     ///< exponent
} float_s64_t;

/**
 * @brief A complex floating-point scalar with a complex 16-bit mantissa.
 *
 * Represents a (non-standard) complex floating-point value given by
 * @math{ A + j\cdot B \cdot 2^{x}}, where @math{A} is `mant.re`, the 16-bit real part of the
 * mantissa, @math{B} is `mant.im`, the 16-bit imaginary part of the mantissa, and @math{x} is the
 * exponent `exp`.
 *
 * @ingroup type_scalar_float
 */
C_TYPE
typedef struct {
    complex_s16_t mant; ///< complex 16-bit mantissa
    exponent_t exp;     ///< exponent
} float_complex_s16_t;

/**
 * @brief A complex floating-point scalar with a complex 32-bit mantissa.
 *
 * Represents a (non-standard) complex floating-point value given by @math{ A + j\cdot B \cdot 2^{x}
 * }, where @math{A} is `mant.re`, the 32-bit real part of the mantissa, @math{B} is `mant.im`, the
 * 32-bit imaginary part of the mantissa, and @math{x} is the exponent `exp`.
 *
 * @ingroup type_scalar_float
 */
C_TYPE
typedef struct {
    complex_s32_t mant; ///< complex 32-bit mantissa
    exponent_t exp;     ///< exponent
} float_complex_s32_t;

/**
 * @brief A complex floating-point scalar with a complex 64-bit mantissa.
 *
 * Represents a (non-standard) complex floating-point value given by
 * @math{ A + j\cdot B \cdot 2^{x}}, where @math{A} is `mant.re`, the 64-bit real part of the
 * mantissa, @math{B} is `mant.im`, the 64-bit imaginary part of the mantissa, and @math{x} is the
 * exponent `exp`.
 *
 * @ingroup type_scalar_float
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

// astew: The tags around these structs are so that they can be copied into the documentation.
//        Unfortunately it appears to mess with the documentation in a way that I'm not sure how to
//        fix.

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


/**
 * @brief A complex number with a single-precision floating-point real part and a single-precision
 * floating-point imaginary part.
 *
 * @ingroup type_scalar_float
 */
C_TYPE
typedef struct {
    float re;   ///< Real Part
    float im;   ///< Imaginary Part
} complex_float_t;


/**
 * @brief A complex number with a double-precision floating-point real part and a double-precision
 * floating-point imaginary part.
 *
 * @ingroup type_scalar_float
 */
C_TYPE
typedef struct {
    double re;  ///< Real Part
    double im;  ///< Imaginary Part
} complex_double_t;


/**
 * @brief Holds a set of sixteen 32-bit accumulators in the XS3 VPU's internal format.
 *
 * The XS3 VPU stores 32-bit accumulators with the most significant 16-bits stored in one 256-bit
 * vector register (called vD), and the least significant 16-bit stored in another 256-bit register
 * (called vR). This struct reflects that internal format, and is occasionally used to store
 * intermediate results.
 *
 * @note `vR` is unsigned. This reflects the fact that a signed 16-bit integer `0xSTUVWXYZ` is
 *        always exactly `0x0000WXYZ` larger than `0xSTUV0000`.  To combine the upper and lower
 *        16-bits of an accumulator, use `(((int32_t)vD[k]) << 16) + vR[k]`.
 *
 * @ingroup type_misc
 */
C_TYPE
typedef struct {
  int16_t vD[16];   ///< Most significant 16 bits of accumulators
  uint16_t vR[16];  ///< Least significant 16 bits of accumulators
} split_acc_s32_t;


/**
 * @brief Q1.31 (Signed) Fixed-point value.
 *
 * Represents a signed, 32-bit, real, fixed-point value with 31 fractional bits (i.e. an implicit
 * exponent of @math{-31}).
 *
 * Capable of representing values in the range @math{\left[-1.0, 1.0\right)}
 *
 * @ingroup type_scalar_fixed
 */
typedef int32_t q1_31;

/**
 * @brief Q2.30 (Signed) Fixed-point value.
 *
 * Represents a signed, 32-bit, real, fixed-point value with 30 fractional bits (i.e. an implicit
 * exponent of @math{-30}).
 *
 * Capable of representing values in the range @math{\left[-2.0, 2.0\right)}
 *
 * @ingroup type_scalar_fixed
 */
typedef int32_t q2_30;

/**
 * @brief Q4.28 (Signed) Fixed-point value.
 *
 * Represents a signed, 32-bit, real, fixed-point value with 28 fractional bits (i.e. an implicit
 * exponent of @math{-28}).
 *
 * Capable of representing values in the range @math{\left[-8.0, 8.0\right)}
 *
 * @ingroup type_scalar_fixed
 */
typedef int32_t q4_28;

/**
 * @brief Q8.24 (Signed) Fixed-point value.
 *
 * Represents a signed, 32-bit, real, fixed-point value with 24 fractional bits (i.e. an implicit
 * exponent of @math{-24}).
 *
 * Capable of representing values in the range @math{\left[-128.0, 128.0\right)}
 *
 * @ingroup type_scalar_fixed
 */
typedef int32_t q8_24;


/**
 * @brief UQ0.32 (Unsigned) Fixed-point value.
 *
 * Represents an unsigned, 32-bit, real, fixed-point value with 32 fractional bits (i.e. an implicit
 * exponent of @math{-32}).
 *
 * Capable of representing values in the range @math{\left[0, 1.0\right)}
 *
 * @ingroup type_scalar_fixed
 */
typedef uint32_t uq0_32;


/**
 * @brief UQ1.31 (Unsigned) Fixed-point value.
 *
 * Represents an unsigned, 32-bit, real, fixed-point value with 31 fractional bits (i.e. an implicit
 * exponent of @math{-31}).
 *
 * Capable of representing values in the range @math{\left[0, 2.0\right)}
 *
 * @ingroup type_scalar_fixed
 */
typedef uint32_t uq1_31;


/**
 * @brief UQ2.30 (Unsigned) Fixed-point value.
 *
 * Represents an unsigned, 32-bit, real, fixed-point value with 30 fractional bits (i.e. an implicit
 * exponent of @math{-30}).
 *
 * Capable of representing values in the range @math{\left[0, 4.0\right)}
 *
 * @ingroup type_scalar_fixed
 */
typedef uint32_t uq2_30;


/**
 * @brief UQ4.28 (Unsigned) Fixed-point value.
 *
 * Represents an unsigned, 32-bit, real, fixed-point value with 28 fractional bits (i.e. an implicit
 * exponent of @math{-28}).
 *
 * Capable of representing values in the range @math{\left[0, 16.0\right)}
 *
 * @ingroup type_scalar_fixed
 */
typedef uint32_t uq4_28;


/**
 * @brief UQ8.24 (Unsigned) Fixed-point value.
 *
 * Represents an unsigned, 32-bit, real, fixed-point value with 24 fractional bits (i.e. an implicit
 * exponent of @math{-24}).
 *
 * Capable of representing values in the range @math{\left[0, 256.0\right)}
 *
 * @ingroup type_scalar_fixed
 */
typedef uint32_t uq8_24;


/**
 * @brief Specialized angular unit used by this library.
 *
 * 'sbrad' is a kind of modified
 * [binary radian](https://en.wikipedia.org/wiki/Binary_angular_measurement) (hence 'brad') which
 * takes into account the symmetries of @math{sin(\theta)}.
 *
 * Use @ref radians_to_sbrads() to convert from radians to `sbrad_t`.
 *
 * @ingroup type_scalar_fixed
 */
typedef q1_31 sbrad_t;

/**
 * @brief Angle measurement in radians using a Q8.24 representation.
 *
 * @ingroup type_scalar_fixed
 */
typedef q8_24 radian_q24_t;


#ifdef __XC__
}
#endif