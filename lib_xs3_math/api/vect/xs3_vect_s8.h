// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include "xs3_math_conf.h"
#include "xs3_math_types.h"
#include "xs3_util.h"

#ifdef __XC__
extern "C" {
#endif


/**
 * @brief Determine whether each element of a signed 8-bit input vector are negative.
 * 
 * Each element @math{a_k} of 8-bit output vector @vector{a} is set to 1 if the corresponding element @math{b_k}
 * of 8-bit input vector @vector{b} is negative, and is set to 0 otherwise.
 * 
 * `a[]` represents the 8-bit output vector @vector{a}, with the element `a[k]` representing @math{a_k}.
 * 
 * `b[]` represents the 8-bit input vector @vector{b}, with the element `b[k]` representing @math{b_k}.
 * 
 * `length` is the number of elements in `a[]` and `b[]`.
 * 
 * @operation{
 *      a_k \leftarrow \begin{cases}
 *          1 & b_k \lt 0              \\ 
 *          0 & otherwise\end{cases}   \\
 *          \qquad\text{ for }k\in 0\ ...\ (length-1)
 *  }
 * 
 * @param[out]  a       Output vector @vector{a}
 * @param[in]   b       Input vector @vector{b}
 * @param[in]   length  Number of elements in @vector{a} and @vector{b}
 * 
 * @ingroup xs3_vect8_func
 */
C_API
void xs3_vect_s8_is_negative(
    int8_t a[],
    const int8_t b[],
    const unsigned length);




#ifdef __XC__
}   //extern "C"
#endif

