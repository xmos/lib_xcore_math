// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include <stdlib.h>
#include <stdint.h>

#include "xs3_math_types.h"


/** 
 * @brief Initialize a 16-bit BFP vector.
 * 
 * This function initializes each of the fields of BFP vector `a`.
 * 
 * `data` points to the memory buffer used to store elements of the vector, so it must be at least `length * 2` bytes 
 * long, and must begin at a word-aligned address.
 * 
 * `exp` is the exponent assigned to the BFP vector. The logical value associated with the `k`th element of the vector 
 * after initialization is @math{ data_k \cdot 2^{exp} }.
 * 
 * If `calc_hr` is false, `a->hr` is initialized to 0. Otherwise, the headroom of the the BFP vector is calculated and 
 * used to initialize `a->hr`.
 * 
 * @param[out] a        BFP vector to initialize
 * @param[in] data      `int16_t` buffer used to back `a`
 * @param[in] exp       Exponent of BFP vector
 * @param[in] length    Number of elements in the BFP vector
 * @param[in] calc_hr   Boolean indicating whether the HR of the BFP vector should be calculated
 */
C_API
void bfp_s16_init(
    bfp_s16_t* a, 
    int16_t* data, 
    const exponent_t exp, 
    const unsigned length,
    const unsigned calc_hr);

/** 
 * @brief Initialize a 32-bit BFP vector.
 * 
 * This function initializes each of the fields of BFP vector `a`.
 * 
 * `data` points to the memory buffer used to store elements of the vector, so it must be at least `length * 4` bytes 
 * long, and must begin at a word-aligned address.
 * 
 * `exp` is the exponent assigned to the BFP vector. The logical value associated with the `k`th element of the vector 
 * after initialization is @math{ data_k \cdot 2^{exp} }.
 * 
 * If `calc_hr` is false, `a->hr` is initialized to 0. Otherwise, the headroom of the the BFP vector is calculated and 
 * used to initialize `a->hr`.
 * 
 * @param[out] a         BFP vector to initialize
 * @param[in]  data      `int32_t` buffer used to back `a`
 * @param[in]  exp       Exponent of BFP vector
 * @param[in]  length    Number of elements in the BFP vector
 * @param[in]  calc_hr   Boolean indicating whether the HR of the BFP vector should be calculated
 */
C_API
void bfp_s32_init(
    bfp_s32_t* a, 
    int32_t* data, 
    const exponent_t exp, 
    const unsigned length,
    const unsigned calc_hr);

/** 
 * @brief Initialize a complex 16-bit BFP vector.
 * 
 * This function initializes each of the fields of BFP vector `a`.
 * 
 * Unlike complex 32-bit BFP vectors (`bfp_complex_s16_t`), for the sake of various optimizations the real and imaginary
 * parts of elements' mantissas are stored in separate memory buffers.
 * 
 * `real_data` points to the memory buffer used to store the real part of each mantissa. It must be at least 
 * `length * 2` bytes long, and must begin at a word-aligned address.
 * 
 * `imag_data` points to the memory buffer used to store the imaginary part of each mantissa. It must be at least 
 * `length * 2` bytes long, and must begin at a word-aligned address.
 * 
 * `exp` is the exponent assigned to the BFP vector. The logical value associated with the `k`th element of the vector 
 * after initialization is @math{ data_k \cdot 2^{exp} }.
 * 
 * If `calc_hr` is false, `a->hr` is initialized to 0. Otherwise, the headroom of the the BFP vector is calculated and 
 * used to initialize `a->hr`.
 * 
 * @param[out] a         BFP vector to initialize
 * @param[in]  real_data `int16_t` buffer used to back the real part of `a`
 * @param[in]  imag_data `int16_t` buffer used to back the imaginary part of `a`
 * @param[in]  exp       Exponent of BFP vector
 * @param[in]  length    Number of elements in BFP vector
 * @param[in] calc_hr   Boolean indicating whether the HR of the BFP vector should be calculated
 */
C_API
void bfp_complex_s16_init(
    bfp_complex_s16_t* a, 
    int16_t* real_data,
    int16_t* imag_data, 
    const exponent_t exp, 
    const unsigned length,
    const unsigned calc_hr);

/** 
 * @brief Initialize a 32-bit complex BFP vector.
 * 
 * This function initializes each of the fields of `a`.
 * 
 * Unlike `bfp_complex_s16_t`, complex 32-bit BFP vectors use a single buffer to store the real and imaginary parts of each 
 * mantissa, such that the imaginary part of element `k` follows the real part of element `k` in memory. `data` points 
 * to the memory buffer used to store elements of the vector, and must be at least `length * 8` bytes long.
 * 
 * `exp` is the exponent assigned to the BFP vector. The logical value associated with the `k`th complex
 * element of the vector after initialization will be @f$ \left(data_{2k} + i\cdot data_{2k+1} \right)\cdot2^{exp} @f$.
 * 
 * If `calc_hr` is false, `a->hr` is initialized to 0. Otherwise, the headroom of the the BFP vector 
 * is calculated and used to initialize `a->hr`.
 * 
 * @param[out] a         BFP vector struct to initialize
 * @param[in]  data      `complex_s32_t` buffer used to back `a`
 * @param[in]  exp       Exponent of BFP vector
 * @param[in]  length    Number of elements in BFP vector
 * @param[in]  calc_hr   Boolean indicating whether the HR of the BFP vector should be calculated
 */
C_API
void bfp_complex_s32_init(
    bfp_complex_s32_t* a, 
    complex_s32_t* data, 
    const exponent_t exp, 
    const unsigned length,
    const unsigned calc_hr);

/** 
 * @brief Initialize a 16-bit channel pair BFP vector.
 * 
 * This function initializes each of the fields of `a`.
 * 
 * 16-bit channel-pair BFP vectors use a single buffer to store the channel A and channel B values of each mantissa, 
 * such that channel B of element `k` follows channel A of element `k` in memory. `data` points to the memory buffer 
 * used to store elements of the vector, must be at least `length * 4` bytes long, and must begin at a word-aligned
 * address.
 * 
 * `exp` is the exponent assigned to the BFP vector. The logical value associated with the `k`th complex
 * element of the vector after initialization will be @f$ \left(data_{2k} + i\cdot data_{2k+1} \right)\cdot2^{exp} @f$.
 * 
 * If `calc_hr` is false, `a->hr` is initialized to 0. Otherwise, the headroom of the the BFP vector 
 * is calculated and used to initialize `a->hr`.
 * 
 * @param[out] a         BFP vector struct to initialize
 * @param[in]  data      `ch_pair_s16_t` buffer used to back `a`
 * @param[in]  exp       Exponent of BFP vector
 * @param[in]  length    Number of elements in BFP vector
 * @param[in]  calc_hr   Boolean indicating whether the HR of the BFP vector should be calculated
 */
C_API
void bfp_ch_pair_s16_init(
    bfp_ch_pair_s16_t* a, 
    ch_pair_s16_t* data, 
    const exponent_t exp, 
    const unsigned length,
    const unsigned calc_hr);


/** 
 * @brief Initialize a 32-bit channel pair BFP vector.
 * 
 * This function initializes each of the fields of `a`.
 * 
 * 32-bit channel-pair BFP vectors use a single buffer to store the channel A and channel B values of each mantissa, 
 * such that channel B of element `k` follows channel A of element `k` in memory. `data` points to the memory buffer 
 * used to store elements of the vector, must be at least `length * 8` bytes long, and must begin at a word-aligned
 * address.
 * 
 * `exp` is the exponent assigned to the BFP vector. The logical value associated with the `k`th complex
 * element of the vector after initialization will be @f$ \left(data_{2k} + i\cdot data_{2k+1} \right)\cdot2^{exp} @f$.
 * 
 * If `calc_hr` is false, `a->hr` is initialized to 0. Otherwise, the headroom of the the BFP vector 
 * is calculated and used to initialize `a->hr`.
 * 
 * @param[out] a         BFP vector struct to initialize
 * @param[in]  data      `ch_pair_s32_t` buffer used to back `a`
 * @param[in]  exp       Exponent of BFP vector
 * @param[in]  length    Number of elements in BFP vector
 * @param[in]  calc_hr   Boolean indicating whether the HR of the BFP vector should be calculated
 */
C_API
void bfp_ch_pair_s32_init(
    bfp_ch_pair_s32_t* a, 
    ch_pair_s32_t* data, 
    const exponent_t exp, 
    const unsigned length,
    const unsigned calc_hr);


/**
 * @brief Set all elements of a 16-bit BFP vector to a specified value.
 * 
 * The exponent of `a` is set to `exp`, and each element's mantissa is set to `b`.
 * 
 * After performing this operation, all elements will represent the same value @math{b \cdot 2^{exp}}.
 * 
 * `a` must have been initialized (see bfp_s16_init()).
 * 
 * @param[out] a         BFP vector to update
 * @param[in]  b         New value each mantissa is set to
 * @param[in]  exp       New exponent for the BFP vector
 */
C_API
void bfp_s16_set(
    bfp_s16_t* a,
    const int16_t b,
    const exponent_t exp);


/**
 * @brief Set all elements of a 32-bit BFP vector to a specified value.
 * 
 * The exponent of `a` is set to `exp`, and each element's mantissa is set to `b`.
 * 
 * After performing this operation, all elements will represent the same value @math{b \cdot 2^{exp}}.
 * 
 * `a` must have been initialized (see bfp_s32_init()).
 * 
 * @param[out] a         BFP vector to update
 * @param[in]  b         New value each mantissa is set to
 * @param[in]  exp       New exponent for the BFP vector
 */
C_API
void bfp_s32_set(
    bfp_s32_t* a,
    const int32_t b,
    const exponent_t exp);


/**
 * @brief Set all elements of a complex 16-bit BFP vector to a specified value.
 * 
 * The exponent of `a` is set to `exp`, and each element's mantissa is set to `b`.
 * 
 * After performing this operation, all elements will represent the same value @math{b \cdot 2^{exp}}.
 * 
 * `a` must have been initialized (see bfp_complex_s16_init()).
 * 
 * @param[out] a         BFP vector to update
 * @param[in]  b         New value each complex mantissa is set to
 * @param[in]  exp       New exponent for the BFP vector
 */
C_API
void bfp_complex_s16_set(
    bfp_complex_s16_t* a,
    const complex_s16_t b,
    const exponent_t exp);


/**
 * @brief Set all elements of a complex 32-bit BFP vector to a specified value.
 * 
 * The exponent of `a` is set to `exp`, and each element's mantissa is set to `b`.
 * 
 * After performing this operation, all elements will represent the same value @math{b \cdot 2^{exp}}.
 * 
 * `a` must have been initialized (see bfp_complex_s32_init()).
 * 
 * @param[out] a         BFP vector to update
 * @param[in]  b         New value each complex mantissa is set to
 * @param[in]  exp       New exponent for the BFP vector
 */
C_API
void bfp_complex_s32_set(
    bfp_complex_s32_t* a,
    const complex_s32_t b,
    const exponent_t exp);


/**
 * @brief Set all elements of a 16-bit channel-pair BFP vector to a specified value.
 * 
 * The exponent of `a` is set to `exp`. Each element's `ch_a` is set to `b.ch_a`, and each element's `ch_b` is set to
 * `b.ch_b`.
 * 
 * After performing this operation, all elements will represent the same value @math{b \cdot 2^{exp}}.
 * 
 * `a` must have been initialized (see bfp_ch_pair_s16_init()).
 * 
 * @param[out] a         BFP vector to update
 * @param[in]  b         New value each channel-pair mantissa is set to
 * @param[in]  exp       New exponent for the BFP vector
 */
C_API
void bfp_ch_pair_s16_set(
    bfp_ch_pair_s16_t* a,
    const ch_pair_s16_t b,
    const exponent_t exp);


/**
 * @brief Set all elements of a 32-bit channel-pair BFP vector to a specified value.
 * 
 * The exponent of `a` is set to `exp`. Each element's `ch_a` is set to `b.ch_a`, and each element's `ch_b` is set to
 * `b.ch_b`.
 * 
 * After performing this operation, all elements will represent the same value @math{b \cdot 2^{exp}}.
 * 
 * `a` must have been initialized (see bfp_ch_pair_s32_init()).
 * 
 * @param[out] a         BFP vector to update
 * @param[in]  b         New value each channel-pair mantissa is set to
 * @param[in]  exp       New exponent for the BFP vector
 */
C_API
void bfp_ch_pair_s32_set(
    bfp_ch_pair_s32_t* a,
    const ch_pair_s32_t b,
    const exponent_t exp);


/**
 * @brief Dynamically allocate a 32-bit BFP vector from the heap.
 * 
 * If allocation was unsuccessful, the `data` field of the returned vector will be NULL, and
 * the `length` field will be zero.  Otherwise, `data` will point to the allocated memory and
 * the `length` field will be the user-specified length. The `length` argument must not be 
 * zero.
 * 
 * Neither the BFP exponent, headroom, nor the elements of the allocated mantissa vector are 
 * set by this function. To set the BFP vector elements to a known value, use bfp_s32_set() 
 * on the retuned BFP vector.
 * 
 * BFP vectors allocated using this function must be deallocated using bfp_s32_dealloc() to
 * avoid a memory leak.
 * 
 * To initialize a BFP vector using static memory allocation, use bfp_s32_init() instead.
 * 
 * @note This function always allocates an extra 2 elements so that `bfp_fft_unpack_mono()` can
 *       safely be used, but these two elements will NOT be reflected in the returned vector
 *       length.
 * @par
 * @note Dynamic allocation of BFP vectors relies on allocation from the heap, and offers no
 *       guarantees about the execution time.  Use of this function in any time-critical 
 *       section of code is highly discouraged.
 * 
 * @param[in] length  The length of the BFP vector to be allocated (in elements)
 * 
 * @returns 32-bit BFP vector
 * 
 * @see bfp_s32_dealloc
 * @see bfp_s32_init
 */
C_API
bfp_s32_t bfp_s32_alloc(
    unsigned length);

    
/**
 * @brief Dynamically allocate a 16-bit BFP vector from the heap.
 * 
 * If allocation was unsuccessful, the `data` field of the returned vector will be NULL, and
 * the `length` field will be zero.  Otherwise, `data` will point to the allocated memory and
 * the `length` field will be the user-specified length. The `length` argument must not be 
 * zero.
 * 
 * Neither the BFP exponent, headroom, nor the elements of the allocated mantissa vector are 
 * set by this function. To set the BFP vector elements to a known value, use bfp_s16_set() 
 * on the retuned BFP vector.
 * 
 * BFP vectors allocated using this function must be deallocated using bfp_s16_dealloc() to
 * avoid a memory leak.
 * 
 * To initialize a BFP vector using static memory allocation, use bfp_s16_init() instead.
 * 
 * @note Dynamic allocation of BFP vectors relies on allocation from the heap, and offers no
 *       guarantees about the execution time.  Use of this function in any time-critical 
 *       section of code is highly discouraged.
 * 
 * @param[in] length  The length of the BFP vector to be allocated (in elements)
 * 
 * @returns 16-bit BFP vector
 * 
 * @see bfp_s16_dealloc
 * @see bfp_s16_init
 */
C_API
bfp_s16_t bfp_s16_alloc(
    const unsigned length);


/**
 * @brief Dynamically allocate a complex 32-bit BFP vector from the heap.
 * 
 * If allocation was unsuccessful, the `data` field of the returned vector will be NULL, and
 * the `length` field will be zero.  Otherwise, `data` will point to the allocated memory and
 * the `length` field will be the user-specified length. The `length` argument must not be 
 * zero.
 * 
 * Neither the BFP exponent, headroom, nor the elements of the allocated mantissa vector are 
 * set by this function. To set the BFP vector elements to a known value, use 
 * bfp_complex_s32_set() on the retuned BFP vector.
 * 
 * BFP vectors allocated using this function must be deallocated using 
 * bfp_complex_s32_dealloc() to avoid a memory leak.
 * 
 * To initialize a BFP vector using static memory allocation, use bfp_complex_s32_init() 
 * instead.
 * 
 * @note Dynamic allocation of BFP vectors relies on allocation from the heap, and offers no
 *       guarantees about the execution time.  Use of this function in any time-critical 
 *       section of code is highly discouraged.
 * 
 * @param[in] length  The length of the BFP vector to be allocated (in elements)
 * 
 * @returns Complex 32-bit BFP vector
 * 
 * @see bfp_complex_s32_dealloc
 * @see bfp_complex_s32_init
 */
C_API
bfp_complex_s32_t bfp_complex_s32_alloc(
    const unsigned length);

    
/**
 * @brief Dynamically allocate a complex 16-bit BFP vector from the heap.
 * 
 * If allocation was unsuccessful, the `real` and `imag` fields of the returned vector will 
 * be NULL, and the `length` field will be zero.  Otherwise, `real` and `imag` will point to 
 * the allocated memory and the `length` field will be the user-specified length. The `length` 
 * argument must not be zero.
 * 
 * This function allocates a single block of memory for both the real and imaginary parts of
 * the BFP vector.  Because all BFP functions require the mantissa buffers to begin at a word-
 * aligned address, if `length` is odd, this function will allocate an extra `int16_t` element
 * for the buffer.
 * 
 * Neither the BFP exponent, headroom, nor the elements of the allocated mantissa vector are 
 * set by this function. To set the BFP vector elements to a known value, use 
 * bfp_complex_s16_set() on the retuned BFP vector.
 * 
 * BFP vectors allocated using this function must be deallocated using 
 * bfp_complex_s16_dealloc() to avoid a memory leak.
 * 
 * To initialize a BFP vector using static memory allocation, use bfp_complex_s16_init() 
 * instead.
 * 
 * @note Dynamic allocation of BFP vectors relies on allocation from the heap, and offers no
 *       guarantees about the execution time.  Use of this function in any time-critical 
 *       section of code is highly discouraged.
 * 
 * @param[in] length  The length of the BFP vector to be allocated (in elements)
 * 
 * @returns Complex 16-bit BFP vector
 * 
 * @see bfp_complex_s16_dealloc
 * @see bfp_complex_s16_init
 */
C_API
bfp_complex_s16_t bfp_complex_s16_alloc(
    const unsigned length);


/**
 * @brief Dynamically allocate a 32-bit BFP channel-pair vector from the heap.
 * 
 * If allocation was unsuccessful, the `data` field of the returned vector will be NULL, and
 * the `length` field will be zero.  Otherwise, `data` will point to the allocated memory and
 * the `length` field will be the user-specified length. The `length` argument must not be 
 * zero.
 * 
 * Neither the BFP exponent, headroom, nor the elements of the allocated mantissa vector are 
 * set by this function. To set the BFP vector elements to a known value, use 
 * bfp_ch_pair_s32_set() on the retuned BFP vector.
 * 
 * BFP vectors allocated using this function must be deallocated using 
 * bfp_ch_pair_s32_dealloc() to avoid a memory leak.
 * 
 * To initialize a BFP vector using static memory allocation, use bfp_ch_pair_s32_init()
 * instead.
 * 
 * @note This function always allocates an extra 2 `ch_pair_s32_t` elements so that 
 *       `bfp_fft_unpack_stereo()` can safely be used, but these two elements will NOT be 
 *       reflected in the returned vector's length.
 * @par
 * @note Dynamic allocation of BFP vectors relies on allocation from the heap, and offers no
 *       guarantees about the execution time.  Use of this function in any time-critical 
 *       section of code is highly discouraged.
 * 
 * @param[in] length  The length of the BFP vector to be allocated (in elements)
 * 
 * @returns 32-bit BFP channel-pair vector
 * 
 * @see bfp_ch_pair_s32_dealloc
 * @see bfp_ch_pair_s32_init
 */
C_API
bfp_ch_pair_s32_t bfp_ch_pair_s32_alloc(
    const unsigned length);


/**
 * @brief Dynamically allocate a 16-bit BFP channel-pair vector from the heap.
 * 
 * If allocation was unsuccessful, the `data` field of the returned vector will be NULL, and
 * the `length` field will be zero.  Otherwise, `data` will point to the allocated memory and
 * the `length` field will be the user-specified length. The `length` argument must not be 
 * zero.
 * 
 * Neither the BFP exponent, headroom, nor the elements of the allocated mantissa vector are 
 * set by this function. To set the BFP vector elements to a known value, use 
 * bfp_ch_pair_s16_set() on the retuned BFP vector.
 * 
 * BFP vectors allocated using this function must be deallocated using 
 * bfp_ch_pair_s16_dealloc() to avoid a memory leak.
 * 
 * To initialize a BFP vector using static memory allocation, use bfp_ch_pair_s16_init()
 * instead.
 * 
 * @note Dynamic allocation of BFP vectors relies on allocation from the heap, and offers no
 *       guarantees about the execution time.  Use of this function in any time-critical 
 *       section of code is highly discouraged.
 * 
 * @param[in] length  The length of the BFP vector to be allocated (in elements)
 * 
 * @returns 16-bit BFP channel-pair vector
 * 
 * @see bfp_ch_pair_s16_dealloc
 * @see bfp_ch_pair_s16_init
 */
C_API
bfp_ch_pair_s16_t bfp_ch_pair_s16_alloc(
    const unsigned length);


/**
 * @brief Deallocate a 32-bit BFP vector allocated by bfp_s32_alloc().
 * 
 * Use this function to free the heap memory allocated by bfp_s32_alloc().
 * 
 * BFP vectors whose mantissa buffer was (successfully) dynamically allocated have a flag 
 * set which indicates as much.  This function can safely be called on any bfp_s32_t which
 * has not had its `flags` or `data` manually manipulated, including:
 * * bfp_s32_t resulting from a successful call to bfp_s32_alloc()
 * * bfp_s32_t resulting from an unsuccessful call to bfp_s32_alloc()
 * * bfp_s32_t initialized with a call to bfp_s32_init()
 * 
 * In the latter two cases, this function does nothing. In the former, the `data`, `length`
 * and `flags` fields of `vector` are cleared to zero.
 * 
 * @param[in] vector  BFP vector to be deallocated.
 * 
 * @see bfp_s32_alloc
 */
C_API
void bfp_s32_dealloc(
    bfp_s32_t* vector);


/**
 * @brief Deallocate a 16-bit BFP vector allocated by bfp_s16_alloc().
 * 
 * Use this function to free the heap memory allocated by bfp_s16_alloc().
 * 
 * BFP vectors whose mantissa buffer was (successfully) dynamically allocated have a flag 
 * set which indicates as much.  This function can safely be called on any bfp_s16_t which
 * has not had its `flags` or `data` manually manipulated, including:
 * * bfp_s16_t resulting from a successful call to bfp_s16_alloc()
 * * bfp_s16_t resulting from an unsuccessful call to bfp_s16_alloc()
 * * bfp_s16_t initialized with a call to bfp_s16_init()
 * 
 * In the latter two cases, this function does nothing. In the former, the `data`, `length`
 * and `flags` fields of `vector` are cleared to zero.
 * 
 * @param[in] vector  BFP vector to be deallocated.
 * 
 * @see bfp_s16_alloc
 */
C_API
void bfp_s16_dealloc(
    bfp_s16_t* vector);


/**
 * @brief Deallocate a complex 32-bit BFP vector allocated by bfp_complex_s32_alloc().
 * 
 * Use this function to free the heap memory allocated by bfp_complex_s32_alloc().
 * 
 * BFP vectors whose mantissa buffer was (successfully) dynamically allocated have a flag 
 * set which indicates as much.  This function can safely be called on any bfp_complex_s32_t 
 * which has not had its `flags` or `data` manually manipulated, including:
 * * bfp_complex_s32_t resulting from a successful call to bfp_complex_s32_alloc()
 * * bfp_complex_s32_t resulting from an unsuccessful call to bfp_complex_s32_alloc()
 * * bfp_complex_s32_t initialized with a call to bfp_complex_s32_init()
 * 
 * In the latter two cases, this function does nothing. In the former, the `data`, `length`
 * and `flags` fields of `vector` are cleared to zero.
 * 
 * @param[in] vector  BFP vector to be deallocated.
 * 
 * @see bfp_complex_s32_alloc
 */
C_API
void bfp_complex_s32_dealloc(
    bfp_complex_s32_t* vector);


/**
 * @brief Deallocate a complex 16-bit BFP vector allocated by bfp_complex_s16_alloc().
 * 
 * Use this function to free the heap memory allocated by bfp_complex_s16_alloc().
 * 
 * BFP vectors whose mantissa buffer was (successfully) dynamically allocated have a flag 
 * set which indicates as much.  This function can safely be called on any bfp_complex_s16_t 
 * which has not had its `flags` or `real` manually manipulated, including:
 * * bfp_complex_s16_t resulting from a successful call to bfp_complex_s16_alloc()
 * * bfp_complex_s16_t resulting from an unsuccessful call to bfp_complex_s16_alloc()
 * * bfp_complex_s16_t initialized with a call to bfp_complex_s16_init()
 * 
 * In the latter two cases, this function does nothing. In the former, the `real`, `imag`, 
 * `length` and `flags` fields of `vector` are cleared to zero.
 * 
 * @param[in] vector  BFP vector to be deallocated.
 * 
 * @see bfp_complex_s16_alloc
 */
C_API
void bfp_complex_s16_dealloc(
    bfp_complex_s16_t* vector);


/**
 * @brief Deallocate a 32-bit channel-pair BFP vector allocated by bfp_ch_pair_s32_alloc().
 * 
 * Use this function to free the heap memory allocated by bfp_ch_pair_s32_alloc().
 * 
 * BFP vectors whose mantissa buffer was (successfully) dynamically allocated have a flag 
 * set which indicates as much.  This function can safely be called on any bfp_ch_pair_s32_t 
 * which has not had its `flags` or `data` manually manipulated, including:
 * * bfp_ch_pair_s32_t resulting from a successful call to bfp_ch_pair_s32_alloc()
 * * bfp_ch_pair_s32_t resulting from an unsuccessful call to bfp_ch_pair_s32_alloc()
 * * bfp_ch_pair_s32_t initialized with a call to bfp_ch_pair_s32_init()
 * 
 * In the latter two cases, this function does nothing. In the former, the `data`, `length`
 * and `flags` fields of `vector` are cleared to zero.
 * 
 * @param[in] vector  BFP vector to be deallocated.
 * 
 * @see bfp_ch_pair_s32_alloc
 */
C_API
void bfp_ch_pair_s32_dealloc(
    bfp_ch_pair_s32_t* vector);


/**
 * @brief Deallocate a 16-bit channel-pair BFP vector allocated by bfp_ch_pair_s16_alloc().
 * 
 * Use this function to free the heap memory allocated by bfp_ch_pair_s16_alloc().
 * 
 * BFP vectors whose mantissa buffer was (successfully) dynamically allocated have a flag 
 * set which indicates as much.  This function can safely be called on any bfp_ch_pair_s16_t 
 * which has not had its `flags` or `data` manually manipulated, including:
 * * bfp_ch_pair_s16_t resulting from a successful call to bfp_ch_pair_s16_alloc()
 * * bfp_ch_pair_s16_t resulting from an unsuccessful call to bfp_ch_pair_s16_alloc()
 * * bfp_ch_pair_s16_t initialized with a call to bfp_ch_pair_s16_init()
 * 
 * In the latter two cases, this function does nothing. In the former, the `data`, `length`
 * and `flags` fields of `vector` are cleared to zero.
 * 
 * @param[in] vector  BFP vector to be deallocated.
 * 
 * @see bfp_ch_pair_s16_alloc
 */
C_API
void bfp_ch_pair_s16_dealloc(
    bfp_ch_pair_s16_t* vector);