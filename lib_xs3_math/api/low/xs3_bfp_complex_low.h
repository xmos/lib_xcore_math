
#ifndef XS3_BFP_COMPLEX_LOW_H_
#define XS3_BFP_COMPLEX_LOW_H_

#include "xs3_math_types.h"

#ifdef __XC__
extern "C" {
#endif


/**
 * @brief Multiply a complex 16-bit vector by a real 16-bit vector.
 * 
 * Multiply the complex vector @vector{b} element-wise with the real vector @vector{c} and divide the product by 
 * @math{2^{-sat}}, placing the result in the complex vector @vector{a}. 
 * 
 * The 16-bit elements of @vector{b} and @vector{c} are multiplied for a 32-bit product. The 32-bit product is divided
 * by @math{2^{sat}} and finally saturated to the symmetric 16-bit range (see TODO) for the final result. The division
 * rounds away from zero.
 * 
 * The parameters `a_real[]` and `a_imag[]` together represent the complex output vector @vector{a}, with the former 
 * representing the real part of its elements and the latter representing the imaginary parts.
 * 
 * Similarly, `b_real[]` and `b_imag[]` together represent the complex input vector @vector{b}.
 * 
 * The parameter `c_real[]` represents the real input vector @vector{c}.
 * 
 * `sat` is the right-shift applied to the products of @vector{b} and @vector{c}, as @math{2^{-sat}}. `sat` can only
 * be used to apply a right shift of the products, so `sat` must be non-negative.
 * 
 * `length` is the number of elements in vectors @vector{a}, @vector{b} and @vector{c}.
 * 
 * This function returns the headroom of the output vector @vector{a}. The headroom of a complex 16-bit vector is the 
 * minimum of the headroom between its real and imaginary parts (i.e. `a_real[]` and `a_imag[]`).
 * 
 * @low_op{16, @f$ Re\\{a_k\\} \leftarrow Re\\{b_k\\} \cdot c_k \cdot 2^{-sat} \\
 *                 Im\\{a_k\\} \leftarrow Im\\{b_k\\} \cdot c_k \cdot 2^{-sat} \\
 *                      \qquad\text{ for }k\in 0\ ...\ (length-1)@f$ }
 * 
 * @par Maximizing Precision:
 *      TODO - Guidelines for keeping maximal precision with this function.
 * 
 * @par Safe In-Place Computation:
 *      The output of this function can be safely computed in-place if the output pointers `a_real[]` and `a_imag[]` are
 *      equal to the input pointers `b_real[]`, `b_imag[]` or `c_real[]`. Using in-place operations may reduce peak 
 *      memory requirements.
 * 
 * @par Word Alignment Required:
 *      Parameters `a_real[]`, `a_imag[]`, `b_real[]`, `b_imag[]`, and `c_real[]` must begin at a word-aligned (4-byte)
 *      address. For more about alignment requirements, see \ref vector_alignment.
 * 
 * @param[out] a_real   Real part of output vector @vector{a}.
 * @param[out] a_imag   Imaginary part of output vector @vector{a}.
 * @param[in]  b_real   Real part of input vector @vector{b}.
 * @param[in]  b_imag   Imaginary part of input vector @vector{b}.
 * @param[in]  c_real   Input vector @vector{c}.
 * @param[in]  length   Number of elements in vectors @vector{a}, @vector{b}, and @vector{c}.
 * @param[in]  sat      Right-shift applied to products.
 * 
 * @return The headroom of the output vector @vector{a} is returned.
 */
headroom_t xs3_mul_vect_complex_s16(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const int16_t c_real[],
    const unsigned length,
    const right_shift_t sat);


/**
 * @brief Multiply a complex 32-bit vector by a real 32-bit vector.
 * 
 * Multiply the complex vector @vector{b} element-wise with the real vector @vector{c}, placing the final result in the 
 * complex vector @vector{a}. This function saturates the output elements to the symmetric 32-bit range. To avoid 
 * saturation, 3 scaling factors are applied during the operation. 
 * 
 * Prior to computing their product, the elements of @vector{b} and @vector{c} are respectively scaled by the factors 
 * @math{2^{-b\_shr}} and @math{2^{-c\_shr}}. This can be used to scale the elements up or down (without rounding). Note
 * that the results of this scaling are also saturated, and so in general neither input vector should be scaled up by
 * more than its headroom.
 * 
 * Finally, a scale factor of @math{2^{-30}} is applied to the 64-bit products of elements. Rounding is applied with
 * this scaling.
 * 
 * The parameters `a[]`, `b[]` and `c[]` represent the vectors @vector{a}, @vector{b} and @vector{c} respectively.
 * 
 * The parameter `length` is the number of elements in each of the vectors.
 * 
 * The parameters `b_shr` and `c_shr` define the scaling factors applied to elements of @vector{b} and @vector{c} 
 * respectively.
 * 
 * This function returns the headroom of the output vector @vector{a}.
 * 
 * @low_op{32, @f$ Re\\{a_k\\} \leftarrow \left( \frac{Re\\{b_k\\}}{2^{b\_shr}}\right) \left(\frac{c_k}{2^{c\_shr}}\right) \cdot 2^{-30} \\
 *                 Im\\{a_k\\} \leftarrow \left( \frac{Im\\{b_k\\}}{2^{b\_shr}}\right) \left(\frac{c_k}{2^{c\_shr}}\right) \cdot 2^{-30} \\
 *              \qquad\text{ for }k\in 0\ ...\ (length-1)@f$ }
 * 
 * @par Maximizing Precision:
 *      TODO - Guidelines for keeping maximal precision with this function.
 * 
 * @par Safe In-Place Computation:
 *      The output of this function can be safely computed in-place if the output `a[]` is equal to the input pointer 
 *      `b[]`. Using in-place operations may reduce peak memory requirements.
 * 
 * @par Word Alignment Required:
 *      Parameters `a[]`, `b[]`, and `c[]` must begin at a word-aligned (4-byte) address. For more about alignment 
 *      requirements, see \ref vector_alignment.
 * 
 * @param[out] a        Complex output vector @vector{a}.
 * @param[in]  b        Complex input vector @vector{b}.
 * @param[in]  c        Real input vector @vector{c}.
 * @param[in]  length   Number of elements in vectors @vector{a}, @vector{b}, and @vector{c}.
 * @param[in]  b_shr    Right-shift applied to elements of @vector{b}.
 * @param[in]  c_shr    Right-shift applied to elements of @vector{c}.
 * 
 * @return The headroom of the output vector @vector{a} is returned.
 */
headroom_t xs3_mul_vect_complex_s32(
    complex_s32_t a[],
    const complex_s32_t b[],
    const int32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);



/**
 * @brief Compute complex product of two complex 16-bit vectors.
 * 
 * Multiply the vector @vector{b} element-wise with the vector @vector{c} and divide the product by 
 * @math{2^{-sat}}, placing the result in the complex vector @vector{a}.
 * 
 * The 16-bit elements of @vector{b} and @vector{c} are multiplied to produce 32-bit values for the real and imaginary
 * part of the result. Those 32-bit values are divided by @math{2^{sat}} and finally saturated to the symmetric 16-bit 
 * range (see TODO) for the final result. The division rounds away from zero.
 * 
 * The parameters `a_real[]` and `a_imag[]` together represent the complex output vector @vector{a}, with the former 
 * representing the real part of its elements and the latter representing the imaginary parts.
 * 
 * Similarly, `b_real[]` and `b_imag[]` together represent the complex input vector @vector{b}, and the parameters
 * `c_real[]` and `c_imag[]` together represent the complex input vector @vector{c}.
 * 
 * `sat` is the right-shift applied to the products of @vector{b} and @vector{c}, as @math{2^{-sat}}. `sat` can only
 * be used to apply a right shift of the products, so `sat` must be non-negative.
 * 
 * `length` is the number of elements in vectors @vector{a}, @vector{b} and @vector{c}.
 * 
 * This function returns the headroom of the output vector @vector{a}. The headroom of a complex 16-bit vector is the 
 * minimum of the headroom between its real and imaginary parts (i.e. `a_real[]` and `a_imag[]`).
 * 
 * @low_op{16, @f$ Re\\{a_k\\} \leftarrow \left(Re\\{b_k\\}\cdot Re\\{c_k\\}-Im\\{b_k\\}\cdot Im\\{c_k\\}\right) 
 *          \cdot 2^{-sat} \\
 *          Im\\{a_k\\} \leftarrow \left(Re\\{b_k\\}\cdot Im\\{c_k\\}+Im\\{b_k\\}\cdot Re\\{c_k\\}\right)
 *          \cdot 2^{-sat} \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) @f$}
 * 
 * @par Maximizing Precision:
 *      TODO - Guidelines for keeping maximal precision with this function.
 * 
 * @par Safe In-Place Computation:
 *      The output of this function can be safely computed in-place if the output pointers `a_real[]` and `a_imag[]` are
 *      equal to the input pointers `b_real[]`, `b_imag[]`, `c_real[]` or `c_imag`. Using in-place operations may 
 *      reduce peak memory requirements.
 * 
 * @par Word Alignment Required:
 *      Parameters `a_real[]`, `a_imag[]`, `b_real[]`, `b_imag[]`, `c_real[]` and `c_imag[]` must begin at a 
 *      word-aligned (4-byte) address. For more about alignment requirements, see \ref vector_alignment.
 * 
 * @param[out] a_real   Real part of output vector @vector{a}.
 * @param[out] a_imag   Imaginary part of output vector @vector{a}.
 * @param[in]  b_real   Real part of input vector @vector{b}.
 * @param[in]  b_imag   Imaginary part of input vector @vector{b}.
 * @param[in]  c_real   Real part of input vector @vector{c}.
 * @param[in]  c_real   Imaginary part of input vector @vector{c}.
 * @param[in]  length   Number of elements in vectors @vector{a}, @vector{b}, and @vector{c}.
 * @param[in]  sat      Right-shift applied to products.
 * 
 * @return The headroom of the output vector @vector{a} is returned.
 */
headroom_t xs3_complex_mul_vect_complex_s16(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const int16_t c_real[],
    const int16_t c_imag[],
    const unsigned length,
    const right_shift_t sat);


/**
 * @brief Compute the complex product of two complex 32-bit vectors.
 * 
 * Multiply the vector @vector{b} element-wise with the vector @vector{c}, placing the final result in the 
 * vector @vector{a}. This function saturates the output elements to the symmetric 32-bit range. To avoid 
 * saturation, 3 scaling factors are applied during the operation. 
 * 
 * Prior to computing their product, the elements of @vector{b} and @vector{c} are respectively scaled by the factors 
 * @math{2^{-b\_shr}} and @math{2^{-c\_shr}}. This can be used to scale the elements up or down (without rounding). Note
 * that the results of this scaling are also saturated, and so in general neither input vector should be scaled up by
 * more than its headroom.
 * 
 * Finally, a scale factor of @math{2^{-30}} is applied to the 64-bit products of elements. Rounding is applied with
 * this scaling.
 * 
 * The parameters `a[]`, `b[]` and `c[]` represent the vectors @vector{a}, @vector{b} and @vector{c} respectively.
 * 
 * The parameter `length` is the number of elements in each of the vectors.
 * 
 * The parameters `b_shr` and `c_shr` define the scaling factors applied to elements of @vector{b} and @vector{c} 
 * respectively.
 * 
 * This function returns the headroom of the output vector @vector{a}.
 * 
 * @low_op{32, @f$ Re\\{a_k\\} \leftarrow \left( \frac{Re\\{b_k\\}}{2^{b\_shr}} \cdot \frac{Re\\{c_k\\}}{2^{c\_shr}} 
 *              - \frac{Im\\{b_k\\}}{2^{b\_shr}} \cdot \frac{Im\\{c_k\\}}{2^{c\_shr}} \right) \cdot 2^{-30} \\
 *          Im\\{a_k\\} \leftarrow \left( \frac{Re\\{b_k\\}}{2^{b\_shr}} \cdot \frac{Im\\{c_k\\}}{2^{c\_shr}} 
 *              + \frac{Im\\{b_k\\}}{2^{b\_shr}} \cdot \frac{Re\\{c_k\\}}{2^{c\_shr}} \right) \cdot 2^{-30} \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) @f$}
 * 
 * @par Maximizing Precision:
 *      TODO - Guidelines for keeping maximal precision with this function.
 * 
 * @par Safe In-Place Computation:
 *      The output of this function can be safely computed in-place if the output `a[]` is equal to the input pointer 
 *      `b[]`. Using in-place operations may reduce peak memory requirements.
 * 
 * @par Word Alignment Required:
 *      Parameters `a[]`, `b[]`, and `c[]` must begin at a word-aligned (4-byte) address. For more about alignment 
 *      requirements, see \ref vector_alignment.
 * 
 * @param[out] a        Complex output vector @vector{a}.
 * @param[in]  b        Complex input vector @vector{b}.
 * @param[in]  c        Complex input vector @vector{c}.
 * @param[in]  length   Number of elements in vectors @vector{a}, @vector{b}, and @vector{c}.
 * @param[in]  b_shr    Right-shift applied to elements of @vector{b}.
 * @param[in]  c_shr    Right-shift applied to elements of @vector{c}.
 * 
 * @return The headroom of the output vector @vector{a} is returned.
 */
headroom_t xs3_complex_mul_vect_complex_s32(
    complex_s32_t a[],
    const complex_s32_t b[],
    const complex_s32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Compute the complex product of a complex 16-bit vector and the complex conjugate of another.
 * 
 * Multiply the vector @vector{b} element-wise with the complex conjugate of vector @vector{c} and divide the product by 
 * @math{2^{-sat}}, placing the result in the complex vector @vector{a}.
 * 
 * The 16-bit elements of @vector{b} and the complex conjugate of @vector{c} are multiplied to produce 32-bit values for 
 * the real and imaginary part of the result. Those 32-bit values are divided by @math{2^{sat}} and finally saturated to 
 * the symmetric 16-bit range (see TODO) for the final result. The division rounds away from zero.
 * 
 * The parameters `a_real[]` and `a_imag[]` together represent the complex output vector @vector{a}, with the former 
 * representing the real part of its elements and the latter representing the imaginary parts.
 * 
 * Similarly, `b_real[]` and `b_imag[]` together represent the complex input vector @vector{b}, and the parameters
 * `c_real[]` and `c_imag[]` together represent the complex input vector @vector{c}.
 * 
 * `sat` is the right-shift applied to the products of @vector{b} and @vector{c}, as @math{2^{-sat}}. `sat` can only
 * be used to apply a right shift of the products, so `sat` must be non-negative.
 * 
 * `length` is the number of elements in vectors @vector{a}, @vector{b} and @vector{c}.
 * 
 * This function returns the headroom of the output vector @vector{a}. The headroom of a complex 16-bit vector is the 
 * minimum of the headroom between its real and imaginary parts (i.e. `a_real[]` and `a_imag[]`).
 * 
 * @low_op{16, @f$ Re\\{a_k\\} \leftarrow \left(Re\\{b_k\\}\cdot Re\\{c_k\\} + Im\\{b_k\\}\cdot Im\\{c_k\\}\right) 
 *          \cdot 2^{-sat} \\
 *          Im\\{a_k\\} \leftarrow \left(Im\\{b_k\\}\cdot Re\\{c_k\\} - Re\\{b_k\\}\cdot Im\\{c_k\\}\right)
 *          \cdot 2^{-sat} \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) @f$}
 * 
 * @par Maximizing Precision:
 *      TODO - Guidelines for keeping maximal precision with this function.
 * 
 * @par Safe In-Place Computation:
 *      The output of this function can be safely computed in-place if the output pointers `a_real[]` and `a_imag[]` are
 *      equal to the input pointers `b_real[]`, `b_imag[]`, `c_real[]` or `c_imag`. Using in-place operations may 
 *      reduce peak memory requirements.
 * 
 * @par Word Alignment Required:
 *      Parameters `a_real[]`, `a_imag[]`, `b_real[]`, `b_imag[]`, `c_real[]` and `c_imag[]` must begin at a 
 *      word-aligned (4-byte) address. For more about alignment requirements, see \ref vector_alignment.
 * 
 * @param[out] a_real   Real part of output vector @vector{a}.
 * @param[out] a_imag   Imaginary part of output vector @vector{a}.
 * @param[in]  b_real   Real part of input vector @vector{b}.
 * @param[in]  b_imag   Imaginary part of input vector @vector{b}.
 * @param[in]  c_real   Real part of input vector @vector{c}.
 * @param[in]  c_real   Imaginary part of input vector @vector{c}.
 * @param[in]  length   Number of elements in vectors @vector{a}, @vector{b}, and @vector{c}.
 * @param[in]  sat      Right-shift applied to products.
 * 
 * @return The headroom of the output vector @vector{a} is returned.
 */
headroom_t xs3_complex_conj_mul_vect_complex_s16(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const int16_t c_real[],
    const int16_t c_imag[],
    const unsigned length,
    const right_shift_t sat);


/**
 * @brief Compute the complex product of a complex 32-bit vector with the complex conjugate of another.
 * 
 * Multiply the vector @vector{b} element-wise with the complex conjugate of vector @vector{c}, placing the final result 
 * in the vector @vector{a}. This function saturates the output elements to the symmetric 32-bit range. To avoid 
 * saturation, 3 scaling factors are applied during the operation. 
 * 
 * Prior to computing their product, the elements of @vector{b} and @vector{c^*} are respectively scaled by the factors 
 * @math{2^{-b\_shr}} and @math{2^{-c\_shr}}. This can be used to scale the elements up or down (without rounding). Note
 * that the results of this scaling are also saturated, and so in general neither input vector should be scaled up by
 * more than its headroom.
 * 
 * Finally, a scale factor of @math{2^{-30}} is applied to the 64-bit products of elements. Rounding is applied with
 * this scaling.
 * 
 * The parameters `a[]`, `b[]` and `c[]` represent the vectors @vector{a}, @vector{b} and @vector{c} respectively.
 * 
 * The parameter `length` is the number of elements in each of the vectors.
 * 
 * The parameters `b_shr` and `c_shr` define the scaling factors applied to elements of @vector{b} and @vector{c} 
 * respectively.
 * 
 * This function returns the headroom of the output vector @vector{a}.
 * 
 * @low_op{32, @f$ Re\\{a_k\\} \leftarrow \left( \frac{Re\\{b_k\\}}{2^{b\_shr}} \cdot \frac{Re\\{c_k\\}}{2^{c\_shr}} 
 *              + \frac{Im\\{b_k\\}}{2^{b\_shr}} \cdot \frac{Im\\{c_k\\}}{2^{c\_shr}} \right) \cdot 2^{-30} \\
 *          Im\\{a_k\\} \leftarrow \left( \frac{Im\\{b_k\\}}{2^{b\_shr}} \cdot \frac{Re\\{c_k\\}}{2^{c\_shr}} 
 *              - \frac{Re\\{b_k\\}}{2^{b\_shr}} \cdot \frac{Im\\{c_k\\}}{2^{c\_shr}} \right) \cdot 2^{-30} \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) @f$}
 * 
 * @par Maximizing Precision:
 *      TODO - Guidelines for keeping maximal precision with this function.
 * 
 * @par Safe In-Place Computation:
 *      The output of this function can be safely computed in-place if the output `a[]` is equal to the input pointer 
 *      `b[]`. Using in-place operations may reduce peak memory requirements.
 * 
 * @par Word Alignment Required:
 *      Parameters `a[]`, `b[]`, and `c[]` must begin at a word-aligned (4-byte) address. For more about alignment 
 *      requirements, see \ref vector_alignment.
 * 
 * @param[out] a        Complex output vector @vector{a}.
 * @param[in]  b        Complex input vector @vector{b}.
 * @param[in]  c        Complex input vector @vector{c}.
 * @param[in]  length   Number of elements in vectors @vector{a}, @vector{b}, and @vector{c}.
 * @param[in]  b_shr    Right-shift applied to elements of @vector{b}.
 * @param[in]  c_shr    Right-shift applied to elements of @vector{c}.
 * 
 * @return The headroom of the output vector @vector{a} is returned.
 */
headroom_t xs3_complex_conj_mul_vect_complex_s32(
    complex_s32_t a[],
    const complex_s32_t b[],
    const complex_s32_t c[],
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr);


/**
 * @brief Compute complex product of a complex 16-bit vector with a complex 16-bit scalar.
 * 
 * Multiply the vector @vector{b} with the scalar @math{c} and divide the product by 
 * @math{2^{-sat}}, placing the result in the complex vector @vector{a}.
 * 
 * Each 16-bit element of @vector{b} is multiplied by @math{c} to produce a 32-bit value for the real and imaginary
 * part of the result. Those 32-bit values are divided by @math{2^{sat}} and finally saturated to the symmetric 16-bit 
 * range (see TODO) for the final result. The division rounds away from zero.
 * 
 * The parameters `a_real[]` and `a_imag[]` together represent the complex output vector @vector{a}, with the former 
 * representing the real part of its elements and the latter representing the imaginary parts.
 * 
 * Similarly, `b_real[]` and `b_imag[]` together represent the complex input vector @vector{b}. 
 * 
 * The parameters `c_real` and `c_imag` together represent the complex input scalar @math{c}.
 * 
 * `sat` is the right-shift applied to the products of @vector{b} and @math{c}, as @math{2^{-sat}}. `sat` can only
 * be used to apply a right shift of the products, so `sat` must be non-negative.
 * 
 * `length` is the number of elements in vectors @vector{a} and @vector{b}.
 * 
 * This function returns the headroom of the output vector @vector{a}. The headroom of a complex 16-bit vector is the 
 * minimum of the headroom between its real and imaginary parts (i.e. `a_real[]` and `a_imag[]`).
 * 
 * @low_op{16, @f$ Re\\{a_k\\} \leftarrow \left(Re\\{b_k\\}\cdot Re\\{c\\}-Im\\{b_k\\}\cdot Im\\{c\\}\right) 
 *          \cdot 2^{-sat} \\
 *          Im\\{a_k\\} \leftarrow \left(Re\\{b_k\\}\cdot Im\\{c\\}+Im\\{b_k\\}\cdot Re\\{c\\}\right)
 *          \cdot 2^{-sat} \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) @f$}
 * 
 * @par Maximizing Precision:
 *      TODO - Guidelines for keeping maximal precision with this function.
 * 
 * @par Safe In-Place Computation:
 *      The output of this function can be safely computed in-place if the output pointers `a_real[]` and `a_imag[]` are
 *      equal to the input pointers `b_real[]` or `b_imag[]`. Using in-place operations may reduce peak memory 
 *      requirements.
 * 
 * @par Word Alignment Required:
 *      Parameters `a_real[]`, `a_imag[]`, `b_real[]`, and `b_imag[]` must begin at a word-aligned (4-byte) address. 
 *      For more about alignment requirements, see \ref vector_alignment.
 * 
 * @param[out] a_real   Real part of output vector @vector{a}.
 * @param[out] a_imag   Imaginary part of output vector @vector{a}.
 * @param[in]  b_real   Real part of input vector @vector{b}.
 * @param[in]  b_imag   Imaginary part of input vector @vector{b}.
 * @param[in]  c_real   Real part of input scalar @math{c}.
 * @param[in]  c_real   Imaginary part of input scalar @math{c}.
 * @param[in]  length   Number of elements in vectors @vector{a} and @vector{b}.
 * @param[in]  sat      Right-shift applied to products.
 * 
 * @return The headroom of the output vector @vector{a} is returned.
 */
headroom_t xs3_complex_scal_mul_vect_complex_s16(
    int16_t a_real[],
    int16_t a_imag[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const int16_t c_real,
    const int16_t c_imag,
    const unsigned length,
    const right_shift_t sat);


/**
 * @brief Compute complex product of a complex 32-bit vector with a complex 32-bit scalar.
 * 
 * Multiply the vector @vector{b} with the scalar @math{c}, placing the final result in the 
 * vector @vector{a}. This function saturates the output elements to the symmetric 32-bit range. To avoid 
 * saturation, 2 scaling factors are applied during the operation. 
 * 
 * Prior to computing their product, the elements of @vector{b} are scaled by the factor @math{2^{-b\_shr}}. This can 
 * be used to scale the elements up or down (without rounding). Note that the results of this scaling are also 
 * saturated, and so in general @vector{b} should not be scaled up by more than its headroom.
 * 
 * Finally, a scale factor of @math{2^{-30}} is applied to the 64-bit products of elements. Rounding is applied with
 * this scaling.
 * 
 * The parameters `a[]` and `b[]` represent the vectors @vector{a} and @vector{b} respectively.
 * 
 * The parameters `c_real` and `c_imag` represent the real and imaginary parts of the complex scalar @math{c}.
 * 
 * The parameter `length` is the number of elements in each of the vectors.
 * 
 * The parameter `b_shr` defines the scaling factor applied to elements of @vector{b}.
 * 
 * This function returns the headroom of the output vector @vector{a}.
 * 
 * @low_op{32, @f$ Re\\{a_k\\} \leftarrow \left( \frac{Re\\{b_k\\}}{2^{b\_shr}} \cdot Re\\{c\\} 
 *              - \frac{Im\\{b_k\\}}{2^{b\_shr}} \cdot Im\\{c\\} \right) \cdot 2^{-30} \\
 *          Im\\{a_k\\} \leftarrow \left( \frac{Re\\{b_k\\}}{2^{b\_shr}} \cdot Im\\{c\\} 
 *              + \frac{Im\\{b_k\\}}{2^{b\_shr}} \cdot Re\\{c\\} \right) \cdot 2^{-30} \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) @f$}
 * 
 * @par Maximizing Precision:
 *      TODO - Guidelines for keeping maximal precision with this function.
 * 
 * @par Safe In-Place Computation:
 *      The output of this function can be safely computed in-place if the output `a[]` is equal to the input pointer 
 *      `b[]`. Using in-place operations may reduce peak memory requirements.
 * 
 * @par Word Alignment Required:
 *      Parameters `a[]` and `b[]` must begin at a word-aligned (4-byte) address. For more about alignment 
 *      requirements, see \ref vector_alignment.
 * 
 * @param[out] a        Complex output vector @vector{a}.
 * @param[in]  b        Complex input vector @vector{b}.
 * @param[in]  c_real   Real part of @math{c}
 * @param[in]  c_imag   Imaginary part of @math{c}
 * @param[in]  length   Number of elements in vectors @vector{a} and @vector{b}.
 * @param[in]  b_shr    Right-shift applied to elements of @vector{b}.
 * 
 * @return The headroom of the output vector @vector{a} is returned.
 */
headroom_t xs3_complex_scal_mul_vect_complex_s32(
    complex_s32_t a[],
    const complex_s32_t b[],
    const int32_t c_real,
    const int32_t c_imag,
    const unsigned length,
    const right_shift_t b_shr);


/**
 * @brief Convert a complex 16-bit vector into a complex 32-bit vector.
 * 
 * Each element of the input vector @vector{b} is assigned to the corresponding element of output vector @vector{b}. No
 * scaling is applied by this operation.
 * 
 * The parameter `a[]` represents the 32-bit output vector @vector{a}.
 * 
 * The parameters `b_real[]` and `b_imag[]` together represent the complex input vector @vector{b}.
 * 
 * The parameter `length` is the number of elements in each of the vectors.
 * 
 * @low_op{16, @f$ Re\\{a_k\\} \leftarrow Re\\{b_k\\} \\
 *                 Im\\{a_k\\} \leftarrow Im\\{b_k\\} \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)@f$}
 * 
 * @note The headroom of the output vector @vector{a} is 16 more than that of the input vector @vector{b}.
 * 
 * @param[out] a        Complex output vector @vector{a}.
 * @param[in]  b_real   Real part of complex input vector @vector{b}.
 * @param[in]  b_imag   Imaginary part of complex input vector @vector{b}.
 * @param[in]  length   Number of elements in vectors @vector{a} and @vector{b}
 */
void xs3_complex_s16_to_complex_s32(
    complex_s32_t a[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const unsigned length);

/** 
 * @brief Convert a complex 32-bit vector into a complex 16-bit vector.
 * 
 * Each element of the input vector @vector{b} scaled by @math{2^{-b\_shr}} abd assigned to the corresponding element 
 * of output vector @vector{b}.
 * 
 * The parameters `a_real[]` and `a_imag[]` together represent the complex 16-bit output vector @vector{b}.
 * 
 * The parameter `b[]` represents the complex 32-bit input vector @vector{a}.
 * 
 * The parameter `length` is the number of elements in each of the vectors.
 * 
 * @low_op{32, @f$ Re\\{a_k\\} \leftarrow \frac{Re\\{b_k\\}}{2^{b\_shr}} \\
 *                 Im\\{a_k\\} \leftarrow \frac{Im\\{b_k\\}}{2^{b\_shr}} \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1) @f$}
 * 
 * @par Maximizing Precision:
 *      To maintain the greatest possible precision, the value provided for `b_shr` should be in the range
 *      @math{-hr \le b\_shr \le 16 - hr} where @math{hr} is the headroom of vector @vector{b}.
 * 
 * @param[out] a_real   Real part of complex output vector @vector{b}.
 * @param[out] a_imag   Imaginary part of complex output vector @vector{b}.
 * @param[in]  b        Complex input vector @vector{a}.
 * @param[in]  length   Number of elements in vectors @vector{a} and @vector{b}
 * @param[in]  b_shr    Right-shift applied to elements of @vector{b}.
 * 
 */
void xs3_complex_s32_to_complex_s16(
    int16_t a_real[],
    int16_t a_imag[],
    const complex_s32_t b[],
    const unsigned length,
    const right_shift_t b_shr);



/**
 * @brief Compute the squared magnitudes of elements of a complex 16-bit vector.
 * 
 * The parameter `a[]` represents the vector of squared magnitudes @vector{a}.
 * 
 * The parameters `b_real[]` and `b_imag[]` together represent the complex input vector @vector{b}.
 * 
 * The parameter `length` is the number of elements in each of the vectors.
 * 
 * The parameter `sat` is the right-shift applied to each result to avoid saturation. `sat` can only be used to apply a 
 * right-shift of the products, so `sat` must be non-negative.
 * 
 * @low_op{16, @f$ a_k \leftarrow ((Re\\{b_k\\})^2 + (Im\\{b_k\\})^2)\cdot 2^{-sat} \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)@f$}
 * 
 * @par Maximizing Precision:
 *      TODO - Guidelines for keeping maximal precision with this function.
 * 
 * @param[out] a        The output vector @vector{a}.
 * @param[in]  b_real   The real part of input vector @vector{b}.
 * @param[in]  b_imag   The imaginary part of input vector @vector{b}.
 * @param[in]  length   The number of elements in vector @vector{b}.
 * @param[in]  sat      The right-shift applied to the results.
 */
headroom_t xs3_squared_mag_vect_complex_s16(
    int16_t a[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const unsigned length,
    const right_shift_t sat);

/**
 * @brief Computes the squared magnitudes of elements of a complex 32-bit vector.
 * 
 * Computes the squared magnitudes of each element of the input vector @vector{b}. The results are output as vector
 * @vector{a}. Results are saturated to symmetric 32-bit bounds. To avoid saturation and underflow, 2 scaling factors
 * are applied during this operation.
 * 
 * The first scaling is given by `b_shr` (scales by @math{2^{b\_shr}}) and occurs on the real and imaginary parts of 
 * each element of @vector{b} prior to squaring them. The second scale is by the constant factor @math{2^{-30}} and 
 * happens after the real and imaginary parts are summed.
 * 
 * The parameter `a[]` represents the vector of squared magnitudes @vector{a}.
 * 
 * The parameter `b` represents the complex input vector @vector{b}.
 * 
 * The parameter `length` is the number of elements in each of the vectors.
 * 
 * The parameter `b_shr` is the right-shift applied to each element of @vector{b} to avoid saturation and underflow.
 * 
 * @low_op{16, @f$ 
 *                  v_k = b_k \cdot 2^{-b\_shr} \\
 *                  a_k \leftarrow ((Re\\{v_k\\})^2 + (Im\\{v_k\\})^2)\cdot 2^{-30} \\
 *                      \qquad\text{ for }k\in 0\ ...\ (length-1)
 *             @f$}
 * 
 * @par Maximizing Precision:
 *      TODO - Guidelines for keeping maximal precision with this function.
 * 
 * @param[out] a        The output vector @vector{a}.
 * @param[in]  b_real   The real part of input vector @vector{b}.
 * @param[in]  b_imag   The imaginary part of input vector @vector{b}.
 * @param[in]  length   The number of elements in vector @vector{b}.
 * @param[in]  sat      The right-shift applied to the results.
 */
headroom_t xs3_squared_mag_vect_complex_s32(
    int32_t a[],
    const complex_s32_t b[],
    const unsigned length,
    const right_shift_t b_shr);


/**
 * Returns headroom
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 */
headroom_t xs3_mag_vect_complex_s16(
    int16_t a[],
    const int16_t b_real[],
    const int16_t b_imag[],
    const unsigned length,
    const right_shift_t b_shr,
    const int16_t* rot_table,
    const unsigned table_rows);

/**
 * Returns headroom
 * 
 * <BLOCKQUOTE><CODE style="color:red;">
 *  NOT YET IMPLEMENTED / NOT TESTED.
 * 
 *  See \ref api_status.
 * </CODE></BLOCKQUOTE>
 * 
 */
headroom_t xs3_mag_vect_complex_s32(
    int32_t a[],
    const complex_s32_t b[],
    const unsigned length,
    const right_shift_t b_shr,
    const complex_s32_t* rot_table,
    const unsigned table_rows);

    
/**
 * @brief Compute the sum of elements of a complex 16-bit vector.
 * 
 * This function computes the 32-bit complex sum of the elements of vector @vector{b}. The result is returned 
 * as a `complex_s32_t`.
 * 
 * @low_op{16, @f$ \sum_{k=0}^{length-1} \left( Re\\{b_k\\} + j\cdot Im\\{b_k\\} \right) 
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)   @f$}
 * 
 * @param[in] b_real    The real part of input vector @vector{b}.
 * @param[in] b_imag    The imaginary part of input vector @vector{b}.
 * @param[in] length    The number of elements in vector @vector{b}.
 * 
 * @return The complex 32-bit sum.
 */
complex_s32_t xs3_sum_complex_s16(
    const int16_t b_real[],
    const int16_t b_imag[],
    const unsigned length);


/**
 * @brief Compute the sum of elements of a complex 32-bit vector.
 * 
 * This function computes the 40-bit complex sum of elements of vector @vector{b}. Prior to being added, each element 
 * is scaled down by a factor @math{2^{b\_shr}}. This function does not support scaling up the elements of @vector{b}, 
 * so `b_shr` must be non-negative. The sum saturates at symmetric 40-bit boundaries. The result is output through
 * @math{a}, with the real and imaginary parts sign-extended to 64 bits.
 * 
 * @low_op{32, @f$ a \leftarrow \sum_{k=0}^{length-1} \left( \frac{Re\\{b_k\\}}{2^{b\_shr}} 
 *                                                  + j\cdot \frac{Im\\{b_k\\}}{2^{b\_shr}} \right) 
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)   @f$}
 * 
 * @par Maximizing Precision:
 *      TODO - Guidelines for keeping maximal precision with this function.
 * 
 * @param[out] a        The result @math{a}.
 * @param[in]  b        The complex input vector @vector{b}.
 * @param[in]  length   The number of elements in vector @vector{b}.
 * @param[in]  b_shr    The right-shift applied to elements of @vector{b}.
 */
void xs3_sum_complex_s32(
    complex_s64_t* a,
    const complex_s32_t b[],
    const unsigned length,
    const right_shift_t b_shr);




/**
 * @brief Reverses the order of the tail of a complex 32-bit vector.
 * 
 * Reverses the tail of the complex 32-bit vector @vect{X}. In other words, the first element @math{X[0]} remains where 
 * it is, and the remaining @math{N-1} elements are rearranged to have their order reversed.
 * 
 * @note This function is used when performing a mono FFT.
 * @par
 * @note Saturation logic may be applied to the real or imaginary parts of elements (@ref saturation).
 * 
 * @param[in]   x   The vector to have its tail reversed.
 * @param[in]   N   The length of `x`.
 */
void xs3_tail_reverse_complex_s32(
    complex_s32_t x[],
    const unsigned N);



/**
 * @brief Get headroom of complex 16-bit vector.
 * 
 */
static headroom_t xs3_headroom_vect_complex_s16(
    const int16_t a_real[],
    const int16_t a_imag[],
    const unsigned length)
{
    headroom_t hr_re = xs3_headroom_vect_s16(a_real, length);
    headroom_t hr_im = xs3_headroom_vect_s16(a_imag, length);
    return MIN(hr_re, hr_im);
}



/**
 * @brief Get headroom of complex 32-bit vector.
 * 
 */
static headroom_t xs3_headroom_vect_complex_s32(
    const complex_s32_t a[], 
    const unsigned length)
{
    return xs3_headroom_vect_s32((int32_t*)a, 2*length);
}

/**
 * @brief Adds one complex 32-bit vector to another.
 * 
 */
static headroom_t xs3_add_vect_complex_s32(
    complex_s32_t a[], 
    const complex_s32_t b[],
    const complex_s32_t c[],
    const unsigned length, 
    const right_shift_t b_shr,
    const right_shift_t c_shr)
{
    return xs3_add_vect_s32((int32_t*)a, (int32_t*)b, (int32_t*)c, 2*length, b_shr, c_shr);
}
    
    
/**
 * @brief Subtracts one complex 32-bit vector from another.
 * 
 */
static headroom_t xs3_sub_vect_complex_s32(
    complex_s32_t a[], 
    const complex_s32_t b[],
    const complex_s32_t c[],
    const unsigned length, 
    const right_shift_t b_shr,
    const right_shift_t c_shr)
{
    return xs3_sub_vect_s32((int32_t*)a, (int32_t*)b, (int32_t*)c, 2*length, b_shr, c_shr);
}


#ifdef __XC__
}   //extern "C"
#endif

#endif //XS3_BFP_COMPLEX_LOW_H_