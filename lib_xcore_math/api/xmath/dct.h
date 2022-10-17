// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include "xmath/types.h"
#include "xmath/_support/dct_impl.h"

/**
 * @defgroup dct_api  XMath DCT API
 */

#ifdef __XC__
extern "C" { 
#endif



/**
 * @brief Scaling exponent associated with `dct6_forward()`
 * 
 * Let @vector{x} be the input to `dct6_forward()` and @vector{y} the output. If @math{x\_exp} and
 * @math{y\_exp} are the exponents associated with @vector{x} and @vector{y} respectively, then
 * the following relation holds:
 * @math{y\_exp = x\_exp + dct6\_exp}
 * 
 * @ingroup dct_api
 */
static const exponent_t dct6_exp = 4;

/**
 * @brief Scaling exponent associated with `dct8_forward()`
 * 
 * Let @vector{x} be the input to `dct6_forward()` and @vector{y} the output. If @math{x\_exp} and
 * @math{y\_exp} are the exponents associated with @vector{x} and @vector{y} respectively, then
 * the following relation holds:
 * @math{y\_exp = x\_exp + dct8_exp}
 * 
 * @ingroup dct_api
 */
static const exponent_t dct8_exp = 4;

/**
 * @brief Scaling exponent associated with `dct12_forward()`
 * 
 * Let @vector{x} be the input to `dct12_forward()` and @vector{y} the output. If @math{x\_exp} and
 * @math{y\_exp} are the exponents associated with @vector{x} and @vector{y} respectively, then
 * the following relation holds:
 * @math{y\_exp = x\_exp + dct12_exp}
 * 
 * @ingroup dct_api
 */
static const exponent_t dct12_exp = 7;

/**
 * @brief Scaling exponent associated with `dct16_forward()`
 * 
 * Let @vector{x} be the input to `dct16_forward()` and @vector{y} the output. If @math{x\_exp} and
 * @math{y\_exp} are the exponents associated with @vector{x} and @vector{y} respectively, then
 * the following relation holds:
 * @math{y\_exp = x\_exp + dct16_exp}
 * 
 * @ingroup dct_api
 */
static const exponent_t dct16_exp = 7;

/**
 * @brief Scaling exponent associated with `dct24_forward()`
 * 
 * Let @vector{x} be the input to `dct24_forward()` and @vector{y} the output. If @math{x\_exp} and
 * @math{y\_exp} are the exponents associated with @vector{x} and @vector{y} respectively, then
 * the following relation holds:
 * @math{y\_exp = x\_exp + dct24_exp}
 * 
 * @ingroup dct_api
 */
static const exponent_t dct24_exp = 10;

/**
 * @brief Scaling exponent associated with `dct32_forward()`
 * 
 * Let @vector{x} be the input to `dct32_forward()` and @vector{y} the output. If @math{x\_exp} and
 * @math{y\_exp} are the exponents associated with @vector{x} and @vector{y} respectively, then
 * the following relation holds:
 * @math{y\_exp = x\_exp + dct32_exp}
 * 
 * @ingroup dct_api
 */
static const exponent_t dct32_exp = 10;

/**
 * @brief Scaling exponent associated with `dct48_forward()`
 * 
 * Let @vector{x} be the input to `dct48_forward()` and @vector{y} the output. If @math{x\_exp} and
 * @math{y\_exp} are the exponents associated with @vector{x} and @vector{y} respectively, then
 * the following relation holds:
 * @math{y\_exp = x\_exp + dct48\_exp}
 * 
 * @ingroup dct_api
 */
static const exponent_t dct48_exp = 13;

/**
 * @brief Scaling exponent associated with `dct64_forward()`
 * 
 * Let @vector{x} be the input to `dct64_forward()` and @vector{y} the output. If @math{x\_exp} and
 * @math{y\_exp} are the exponents associated with @vector{x} and @vector{y} respectively, then
 * the following relation holds:
 * @math{y\_exp = x\_exp + dct64\_exp}
 * 
 * @ingroup dct_api
 */
static const exponent_t dct64_exp = 13;


/**
 * @brief 6-point 32-bit forward DCT.
 * 
 * This function performs a 6-point forward type-II DCT on input vector @vector{x}, and populates
 * output vector @vector{y} with the result. To avoid possible overflow or saturation, output 
 * @vector{y} is scaled down by a factor of @math{2^4} (see @ref dct6_exp).
 * 
 * This operation may be safely performed in-place if `x` and `y` point to the same vector.
 * 
 * `x` and `y` must point to 8-byte-aligned addresses.
 * 
 * @operation{
 * &   y_k \leftarrow  \frac{1}{2^4} \left( 2\sum_{n=0}^{N-1} x_n \cos\left( k\pi \frac{2n+1}{2N} \right) \right) \\
 * &   \qquad\text{for } k = 0,1,\dots,(N-1) \\
 * &   \qquad\text{with } N = 6 \\
 * }
 * 
 * @param[out]  y   Output vector @vector{y}
 * @param[in]   x   Input vector @vector{x}
 * 
 * @exception ET_LOAD_STORE Raised if `x` or `y` is not double word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup dct_api
 */
C_API
void dct6_forward(
    int32_t y[6],
    const int32_t x[6]);


/**
 * @brief 8-point 32-bit forward DCT.
 * 
 * This function performs a 8-point forward type-II DCT on input vector @vector{x}, and populates
 * output vector @vector{y} with the result. To avoid possible overflow or saturation, output 
 * @vector{y} is scaled down by a factor of @math{2^4} (see @ref dct8_exp).
 * 
 * This operation may be safely performed in-place if `x` and `y` point to the same vector.
 * 
 * `x` and `y` must point to 8-byte-aligned addresses.
 * 
 * @operation{
 * &   y_k \leftarrow  \frac{1}{2^4} \left( 2\sum_{n=0}^{N-1} x_n \cos\left( k\pi \frac{2n+1}{2N} \right) \right) \\
 * &   \qquad\text{for } k = 0,1,\dots,(N-1) \\
 * &   \qquad\text{with } N = 8 \\
 * }
 * 
 * @param[out]  y   Output vector @vector{y}
 * @param[in]   x   Input vector @vector{x}
 * 
 * @exception ET_LOAD_STORE Raised if `x` or `y` is not double word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup dct_api
 */
C_API
void dct8_forward(
    int32_t y[8],
    const int32_t x[8]);


/**
 * @brief 12-point 32-bit forward DCT.
 * 
 * This function performs a 12-point forward type-II DCT on input vector @vector{x}, and populates
 * output vector @vector{y} with the result. To avoid possible overflow or saturation, output 
 * @vector{y} is scaled down by a factor of @math{2^7} (see @ref dct12_exp).
 * 
 * This operation may be safely performed in-place if `x` and `y` point to the same vector.
 * 
 * `x` and `y` must point to 8-byte-aligned addresses.
 * 
 * @operation{
 * &   y_k \leftarrow  \frac{1}{2^7} \left( 2\sum_{n=0}^{N-1} x_n \cos\left( k\pi \frac{2n+1}{2N} \right) \right) \\
 * &   \qquad\text{for } k = 0,1,\dots,(N-1) \\
 * &   \qquad\text{with } N = 12 \\
 * }
 * 
 * @param[out]  y   Output vector @vector{y}
 * @param[in]   x   Input vector @vector{x}
 * 
 * @exception ET_LOAD_STORE Raised if `x` or `y` is not double word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup dct_api
 */
C_API
void dct12_forward(
    int32_t y[12],
    const int32_t x[12]);


/**
 * @brief 16-point 32-bit forward DCT.
 * 
 * This function performs a 16-point forward type-II DCT on input vector @vector{x}, and populates
 * output vector @vector{y} with the result. To avoid possible overflow or saturation, output 
 * @vector{y} is scaled down by a factor of @math{2^7} (see @ref dct16_exp).
 * 
 * This operation may be safely performed in-place if `x` and `y` point to the same vector.
 * 
 * `x` and `y` must point to 8-byte-aligned addresses.
 * 
 * @operation{
 * &   y_k \leftarrow  \frac{1}{2^7} \left( 2\sum_{n=0}^{N-1} x_n \cos\left( k\pi \frac{2n+1}{2N} \right) \right) \\
 * &   \qquad\text{for } k = 0,1,\dots,(N-1) \\
 * &   \qquad\text{with } N = 16 \\
 * }
 * 
 * @param[out]  y   Output vector @vector{y}
 * @param[in]   x   Input vector @vector{x}
 * 
 * @exception ET_LOAD_STORE Raised if `x` or `y` is not double word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup dct_api
 */
C_API
void dct16_forward(
    int32_t y[16],
    const int32_t x[16]);


/**
 * @brief 24-point 32-bit forward DCT.
 * 
 * This function performs a 24-point forward type-II DCT on input vector @vector{x}, and populates
 * output vector @vector{y} with the result. To avoid possible overflow or saturation, output 
 * @vector{y} is scaled down by a factor of @math{2^{10}} (see @ref dct24_exp).
 * 
 * This operation may be safely performed in-place if `x` and `y` point to the same vector.
 * 
 * `x` and `y` must point to 8-byte-aligned addresses.
 * 
 * @operation{
 * &   y_k \leftarrow  \frac{1}{2^{10}} \left( 2\sum_{n=0}^{N-1} x_n \cos\left( k\pi \frac{2n+1}{2N} \right) \right) \\
 * &   \qquad\text{for } k = 0,1,\dots,(N-1) \\
 * &   \qquad\text{with } N = 24 \\
 * }
 * 
 * @param[out]  y   Output vector @vector{y}
 * @param[in]   x   Input vector @vector{x}
 * 
 * @exception ET_LOAD_STORE Raised if `x` or `y` is not double word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup dct_api
 */
C_API
void dct24_forward(
    int32_t y[24],
    const int32_t x[24]);


/**
 * @brief 32-point 32-bit forward DCT.
 * 
 * This function performs a 32-point forward type-II DCT on input vector @vector{x}, and populates
 * output vector @vector{y} with the result. To avoid possible overflow or saturation, output 
 * @vector{y} is scaled down by a factor of @math{2^{10}} (see @ref dct32_exp).
 * 
 * This operation may be safely performed in-place if `x` and `y` point to the same vector.
 * 
 * `x` and `y` must point to 8-byte-aligned addresses.
 * 
 * @operation{
 * &   y_k \leftarrow  \frac{1}{2^{10}} \left( 2\sum_{n=0}^{N-1} x_n \cos\left( k\pi \frac{2n+1}{2N} \right) \right) \\
 * &   \qquad\text{for } k = 0,1,\dots,(N-1) \\
 * &   \qquad\text{with } N = 32 \\
 * }
 * 
 * @param[out]  y   Output vector @vector{y}
 * @param[in]   x   Input vector @vector{x}
 * 
 * @exception ET_LOAD_STORE Raised if `x` or `y` is not double word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup dct_api
 */
C_API
void dct32_forward(
    int32_t y[32],
    const int32_t x[32]);


/**
 * @brief 48-point 32-bit forward DCT.
 * 
 * This function performs a 48-point forward type-II DCT on input vector @vector{x}, and populates
 * output vector @vector{y} with the result. To avoid possible overflow or saturation, output 
 * @vector{y} is scaled down by a factor of @math{2^{13}} (see @ref dct48_exp).
 * 
 * This operation may be safely performed in-place if `x` and `y` point to the same vector.
 * 
 * `x` and `y` must point to 8-byte-aligned addresses.
 * 
 * @operation{
 * &   y_k \leftarrow  \frac{1}{2^{13}} \left( 2\sum_{n=0}^{N-1} x_n \cos\left( k\pi \frac{2n+1}{2N} \right) \right) \\
 * &   \qquad\text{for } k = 0,1,\dots,(N-1) \\
 * &   \qquad\text{with } N = 48 \\
 * }
 * 
 * @param[out]  y   Output vector @vector{y}
 * @param[in]   x   Input vector @vector{x}
 * 
 * @exception ET_LOAD_STORE Raised if `x` or `y` is not double word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup dct_api
 */
C_API
void dct48_forward(
    int32_t y[48],
    const int32_t x[48]);


/**
 * @brief 64-point 32-bit forward DCT.
 * 
 * This function performs a 64-point forward type-II DCT on input vector @vector{x}, and populates
 * output vector @vector{y} with the result. To avoid possible overflow or saturation, output 
 * @vector{y} is scaled down by a factor of @math{2^{13}} (see @ref dct64_exp).
 * 
 * This operation may be safely performed in-place if `x` and `y` point to the same vector.
 * 
 * `x` and `y` must point to 8-byte-aligned addresses.
 * 
 * @operation{
 * &   y_k \leftarrow  \frac{1}{2^{13}} \left( 2\sum_{n=0}^{N-1} x_n \cos\left( k\pi \frac{2n+1}{2N} \right) \right) \\
 * &   \qquad\text{for } k = 0,1,\dots,(N-1) \\
 * &   \qquad\text{with } N = 64 \\
 * }
 * 
 * @param[out]  y   Output vector @vector{y}
 * @param[in]   x   Input vector @vector{x}
 * 
 * @exception ET_LOAD_STORE Raised if `x` or `y` is not double word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup dct_api
 */
C_API
void dct64_forward(
    int32_t y[64],
    const int32_t x[64]);








/**
 * @brief 6-point 32-bit inverse DCT.
 * 
 * This function performs a 6-point inverse DCT (same as type-III DCT) on input vector @vector{x}, 
 * and populates output vector @vector{y} with the result.
 * 
 * This operation may be safely performed in-place if `x` and `y` point to the same vector.
 * 
 * `x` and `y` must point to 8-byte-aligned addresses.
 * 
 * @operation{
 * &   y_k \leftarrow  \frac{1}{N} \left(  \frac{x_0}{2} + \ 
 *          \sum_{n=1}^{N-1} x_n \cos\left( n\pi \frac{2k+1}{2N} \right) \right) \\
 * &   \qquad\text{for } k = 0,1,\dots,(N-1) \\
 * &   \qquad\text{with } N = 6 \\
 * }
 * 
 * @param[out]  y   Output vector @vector{y}
 * @param[in]   x   Input vector @vector{x}
 * 
 * @exception ET_LOAD_STORE Raised if `x` or `y` is not double word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup dct_api
 */
C_API
void dct6_inverse(
    int32_t y[6],
    const int32_t x[6]);


/**
 * @brief 8-point 32-bit inverse DCT.
 * 
 * This function performs a 8-point inverse DCT (same as type-III DCT) on input vector @vector{x}, 
 * and populates output vector @vector{y} with the result.
 * 
 * This operation may be safely performed in-place if `x` and `y` point to the same vector.
 * 
 * `x` and `y` must point to 8-byte-aligned addresses.
 * 
 * @operation{
 * &   y_k \leftarrow  \frac{1}{N} \left(  \frac{x_0}{2} + \ 
 *          \sum_{n=1}^{N-1} x_n \cos\left( n\pi \frac{2k+1}{2N} \right) \right) \\
 * &   \qquad\text{for } k = 0,1,\dots,(N-1) \\
 * &   \qquad\text{with } N = 8 \\
 * }
 * 
 * @param[out]  y   Output vector @vector{y}
 * @param[in]   x   Input vector @vector{x}
 * 
 * @exception ET_LOAD_STORE Raised if `x` or `y` is not double word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup dct_api
 */
C_API
void dct8_inverse(
    int32_t y[8],
    const int32_t x[8]);


/**
 * @brief 12-point 32-bit inverse DCT.
 * 
 * This function performs a 12-point inverse DCT (same as type-III DCT) on input vector @vector{x}, 
 * and populates output vector @vector{y} with the result.
 * 
 * This operation may be safely performed in-place if `x` and `y` point to the same vector.
 * 
 * `x` and `y` must point to 8-byte-aligned addresses.
 * 
 * @operation{
 * &   y_k \leftarrow  \frac{1}{N} \left(  \frac{x_0}{2} + \ 
 *          \sum_{n=1}^{N-1} x_n \cos\left( n\pi \frac{2k+1}{2N} \right) \right) \\
 * &   \qquad\text{for } k = 0,1,\dots,(N-1) \\
 * &   \qquad\text{with } N = 12 \\
 * }
 * 
 * @param[out]  y   Output vector @vector{y}
 * @param[in]   x   Input vector @vector{x}
 * 
 * @exception ET_LOAD_STORE Raised if `x` or `y` is not double word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup dct_api
 */
C_API
void dct12_inverse(
    int32_t y[12],
    const int32_t x[12]);


/**
 * @brief 16-point 32-bit inverse DCT.
 * 
 * This function performs a 16-point inverse DCT (same as type-III DCT) on input vector @vector{x}, 
 * and populates output vector @vector{y} with the result.
 * 
 * This operation may be safely performed in-place if `x` and `y` point to the same vector.
 * 
 * `x` and `y` must point to 8-byte-aligned addresses.
 * 
 * @operation{
 * &   y_k \leftarrow  \frac{1}{N} \left(  \frac{x_0}{2} + \ 
 *          \sum_{n=1}^{N-1} x_n \cos\left( n\pi \frac{2k+1}{2N} \right) \right) \\
 * &   \qquad\text{for } k = 0,1,\dots,(N-1) \\
 * &   \qquad\text{with } N = 16 \\
 * }
 * 
 * @param[out]  y   Output vector @vector{y}
 * @param[in]   x   Input vector @vector{x}
 * 
 * @exception ET_LOAD_STORE Raised if `x` or `y` is not double word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup dct_api
 */
C_API
void dct16_inverse(
    int32_t y[16],
    const int32_t x[16]);


/**
 * @brief 24-point 32-bit inverse DCT.
 * 
 * This function performs a 24-point inverse DCT (same as type-III DCT) on input vector @vector{x}, 
 * and populates output vector @vector{y} with the result.
 * 
 * This operation may be safely performed in-place if `x` and `y` point to the same vector.
 * 
 * `x` and `y` must point to 8-byte-aligned addresses.
 * 
 * @operation{
 * &   y_k \leftarrow  \frac{1}{N} \left(  \frac{x_0}{2} + \ 
 *          \sum_{n=1}^{N-1} x_n \cos\left( n\pi \frac{2k+1}{2N} \right) \right) \\
 * &   \qquad\text{for } k = 0,1,\dots,(N-1) \\
 * &   \qquad\text{with } N = 24 \\
 * }
 * 
 * @param[out]  y   Output vector @vector{y}
 * @param[in]   x   Input vector @vector{x}
 * 
 * @exception ET_LOAD_STORE Raised if `x` or `y` is not double word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup dct_api
 */
C_API
void dct24_inverse(
    int32_t y[24],
    const int32_t x[24]);


/**
 * @brief 32-point 32-bit inverse DCT.
 * 
 * This function performs a 32-point inverse DCT (same as type-III DCT) on input vector @vector{x}, 
 * and populates output vector @vector{y} with the result.
 * 
 * This operation may be safely performed in-place if `x` and `y` point to the same vector.
 * 
 * `x` and `y` must point to 8-byte-aligned addresses.
 * 
 * @operation{
 * &   y_k \leftarrow  \frac{1}{N} \left(  \frac{x_0}{2} + \ 
 *          \sum_{n=1}^{N-1} x_n \cos\left( n\pi \frac{2k+1}{2N} \right) \right) \\
 * &   \qquad\text{for } k = 0,1,\dots,(N-1) \\
 * &   \qquad\text{with } N = 32 \\
 * }
 * 
 * @param[out]  y   Output vector @vector{y}
 * @param[in]   x   Input vector @vector{x}
 * 
 * @exception ET_LOAD_STORE Raised if `x` or `y` is not double word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup dct_api
 */
C_API
void dct32_inverse(
    int32_t y[32],
    const int32_t x[32]);


/**
 * @brief 48-point 32-bit inverse DCT.
 * 
 * This function performs a 48-point inverse DCT (same as type-III DCT) on input vector @vector{x}, 
 * and populates output vector @vector{y} with the result.
 * 
 * This operation may be safely performed in-place if `x` and `y` point to the same vector.
 * 
 * `x` and `y` must point to 8-byte-aligned addresses.
 * 
 * @operation{
 * &   y_k \leftarrow  \frac{1}{N} \left(  \frac{x_0}{2} + \ 
 *          \sum_{n=1}^{N-1} x_n \cos\left( n\pi \frac{2k+1}{2N} \right) \right) \\
 * &   \qquad\text{for } k = 0,1,\dots,(N-1) \\
 * &   \qquad\text{with } N = 48 \\
 * }
 * 
 * @param[out]  y   Output vector @vector{y}
 * @param[in]   x   Input vector @vector{x}
 * 
 * @exception ET_LOAD_STORE Raised if `x` or `y` is not double word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup dct_api
 */
C_API
void dct48_inverse(
    int32_t y[48],
    const int32_t x[48]);


/**
 * @brief 64-point 32-bit inverse DCT.
 * 
 * This function performs a 64-point inverse DCT (same as type-III DCT) on input vector @vector{x}, 
 * and populates output vector @vector{y} with the result.
 * 
 * This operation may be safely performed in-place if `x` and `y` point to the same vector.
 * 
 * `x` and `y` must point to 8-byte-aligned addresses.
 * 
 * @operation{
 * &   y_k \leftarrow  \frac{1}{N} \left(  \frac{x_0}{2} + \ 
 *          \sum_{n=1}^{N-1} x_n \cos\left( n\pi \frac{2k+1}{2N} \right) \right) \\
 * &   \qquad\text{for } k = 0,1,\dots,(N-1) \\
 * &   \qquad\text{with } N = 64 \\
 * }
 * 
 * @param[out]  y   Output vector @vector{y}
 * @param[in]   x   Input vector @vector{x}
 * 
 * @exception ET_LOAD_STORE Raised if `x` or `y` is not double word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup dct_api
 */
C_API
void dct64_inverse(
    int32_t y[64],
    const int32_t x[64]);




/**
 * @brief 8-by-8 2D 8-bit forward DCT.
 * 
 * This function performs a 2-dimensional 8-by-8 type-II DCT on 8-bit input tensor @vector{x} (with
 * elements @math{x_{rc}}). Output tensor @vector{y} (with elements @math{y_{rc}}) is populated with
 * the result.
 * 
 * This 2D DCT is performed by first applying a 1D 8-point DCT across each row of @vector{x}, and 
 * then applying a 1D 8-point DCT to each column of that intermediate tensor.
 * 
 * The output is scaled by a factor of @math{2^{-\mathtt{sat}-8}}. With @math{\mathtt{sat}=0} this
 * scaling is just enough to avoid any possible saturation. If saturation is considered acceptable,
 * or known _a priori_ to not be possible, negative values for @math{\mathtt{sat}} can be used to
 * increase precision on the output.
 * 
 * This operation may be safely performed in-place if `x` and `y` point to the same vector.
 * 
 * `x` and `y` must point to 8-byte-aligned addresses.
 * 
 * @operation{
 * &  y_{rc} \leftarrow \frac{4 \sum_{m=0}^{N-1} \sum_{n=0}^{N-1} \left( \
 *        x_{mn} \cos\left( c\pi\frac{2n+1}{2N} \right) \cos\left(r\pi\frac{2m+1}{2N}  \right)\right)}{2^{\mathtt{sat}+8}}\\
 * &   \\
 * &   \qquad\text{for } r,c \in \\{0,1,\dots,(N-1)\\} \\
 * &   \qquad\text{with } N = 8 \\
 * }
 * 
 * @param[out]  y    Output vector @vector{y}
 * @param[in]   x    Input vector @vector{x}
 * @param[in]   sat  Additional output scaling exponent.
 * 
 * @exception ET_LOAD_STORE Raised if `x` or `y` is not double word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup dct_api
 */
C_API
headroom_t dct8x8_forward(
    int8_t y[8][8],
    const int8_t x[8][8],
    const right_shift_t sat);

/**
 * @brief 8-by-8 2D 8-bit inverse DCT.
 * 
 * This function performs a 2-dimensional 8-by-8 type-III (inverse) DCT on 8-bit input tensor
 * @vector{x} (with elements @math{x_{rc}}). Output tensor @vector{y} (with elements @math{y_{rc}})
 * is populated with the result.
 * 
 * This 2D DCT is performed by first applying a 1D 8-point DCT across each row of @vector{x}, and 
 * then applying a 1D 8-point DCT to each column of that intermediate tensor.
 * 
 * The output is scaled by a factor of @math{2^{-\mathtt{sat}}}. With @math{\mathtt{sat}=0} this
 * scaling is just enough to avoid any possible saturation. If saturation is considered acceptable,
 * or known _a priori_ to not be possible, negative values for @math{\mathtt{sat}} can be used to
 * increase precision on the output.
 * 
 * This operation may be safely performed in-place if `x` and `y` point to the same vector.
 * 
 * `x` and `y` must point to 8-byte-aligned addresses.
 * 
 * @operation{
 * &  y_{rc} \leftarrow \frac{ \frac{1}{N^2} \sum_{m=0}^{N-1} \sum_{n=0}^{N-1} \left( \
 *        x_{mn} \cos\left( n\pi\frac{2c+1}{2N} \right) \cos\left(m\pi\frac{2r+1}{2N}  \right)\right)}{2^{\mathtt{sat}}}\\
 * &   \\
 * &   \qquad\text{for } r,c \in \\{0,1,\dots,(N-1)\\} \\
 * &   \qquad\text{with } N = 8 \\
 * }
 * 
 * @param[out]  y    Output vector @vector{y}
 * @param[in]   x    Input vector @vector{x}
 * @param[in]   sat  Additional output scaling exponent.
 * 
 * @exception ET_LOAD_STORE Raised if `x` or `y` is not double word-aligned (See @ref note_vector_alignment)
 * 
 * @ingroup dct_api
 */
C_API
headroom_t dct8x8_inverse(
    int8_t y[8][8],
    const int8_t x[8][8],
    const right_shift_t sat);

    
#ifdef __XC__
} // extern "C"
#endif