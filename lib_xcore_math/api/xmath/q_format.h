// Copyright 2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#pragma once

/**
 *  @defgroup qfmt_macros   Q-Format Conversion Macros
 */


#define F0(N) F ## N
#define Q0(N) Q ## N

/**
 * @brief Convert fixed-point value to double-precision float.
 * 
 * This macro is meant to allow for parameterized access to the more specific conversion macros,
 * such as `F8()`, `F24()`, `F31()` and so on. Being parameterized allows the user to specify the
 * Q-format (fractional bit count) using another macro. For example:
 * 
 * @code
 * #define X_FRAC_BITS   24
 * int32_t x = ...;
 * ...
 * // Convert x to double
 * double dbl_x = F(X_FRAC_BITS)(x);
 * @endcode
 * 
 * @ingroup qfmt_macros
 */
#define F(N) F0(N)

/**
 * @brief Convert floating-point value to fixed-point value.
 * 
 * This macro is meant to allow for parameterized access to the more specific conversion macros,
 * such as `Q8()`, `Q24()`, `Q31()` and so on. Being parameterized allows the user to specify the
 * Q-format (fractional bit count) using another macro. For example:
 * 
 * @code
 * #include <math.h>
 * ...
 * #define PI_FRAC_BITS   24
 * int32_t x = Q(PI_FRAC_BITS)(M_PI);
 * @endcode
 * 
 * @ingroup qfmt_macros
 */
#define Q(N) Q0(N)

// Convert from floating point to fixed point Q format.
// The number indicates the fractional bits or the position of the binary point

/** @brief Convert `double` value to Q1.31 fixed-point value, with rounding. 
 * @ingroup qfmt_macros
*/
#define Q31(f) (int)((signed long long)((f) * ((unsigned long long)1 << (31+20)) + (1<<19)) >> 20)

/** @brief Convert `double` value to Q2.30 fixed-point value, with rounding. 
 * @ingroup qfmt_macros
*/
#define Q30(f) (int)((signed long long)((f) * ((unsigned long long)1 << (30+20)) + (1<<19)) >> 20)

/** @brief Convert `double` value to Q3.29 fixed-point value, with rounding. 
 * @ingroup qfmt_macros
*/
#define Q29(f) (int)((signed long long)((f) * ((unsigned long long)1 << (29+20)) + (1<<19)) >> 20)

/** @brief Convert `double` value to Q4.28 fixed-point value, with rounding. 
 * @ingroup qfmt_macros
*/
#define Q28(f) (int)((signed long long)((f) * ((unsigned long long)1 << (28+20)) + (1<<19)) >> 20)

/** @brief Convert `double` value to Q5.27 fixed-point value, with rounding. 
 * @ingroup qfmt_macros
*/
#define Q27(f) (int)((signed long long)((f) * ((unsigned long long)1 << (27+20)) + (1<<19)) >> 20)

/** @brief Convert `double` value to Q6.26 fixed-point value, with rounding. 
 * @ingroup qfmt_macros
*/
#define Q26(f) (int)((signed long long)((f) * ((unsigned long long)1 << (26+20)) + (1<<19)) >> 20)

/** @brief Convert `double` value to Q7.25 fixed-point value, with rounding. 
 * @ingroup qfmt_macros
*/
#define Q25(f) (int)((signed long long)((f) * ((unsigned long long)1 << (25+20)) + (1<<19)) >> 20)

/** @brief Convert `double` value to Q8.24 fixed-point value, with rounding. 
 * @ingroup qfmt_macros
*/
#define Q24(f) (int)((signed long long)((f) * ((unsigned long long)1 << (24+20)) + (1<<19)) >> 20)

/** @brief Convert `double` value to Q9.23 fixed-point value, with rounding. 
 * @ingroup qfmt_macros
*/
#define Q23(f) (int)((signed long long)((f) * ((unsigned long long)1 << (23+20)) + (1<<19)) >> 20)

/** @brief Convert `double` value to Q10.22 fixed-point value, with rounding. 
 * @ingroup qfmt_macros
*/
#define Q22(f) (int)((signed long long)((f) * ((unsigned long long)1 << (22+20)) + (1<<19)) >> 20)

/** @brief Convert `double` value to Q11.21 fixed-point value, with rounding. 
 * @ingroup qfmt_macros
*/
#define Q21(f) (int)((signed long long)((f) * ((unsigned long long)1 << (21+20)) + (1<<19)) >> 20)

/** @brief Convert `double` value to Q12.20 fixed-point value, with rounding. 
 * @ingroup qfmt_macros
*/
#define Q20(f) (int)((signed long long)((f) * ((unsigned long long)1 << (20+20)) + (1<<19)) >> 20)

/** @brief Convert `double` value to Q13.19 fixed-point value, with rounding. 
 * @ingroup qfmt_macros
*/
#define Q19(f) (int)((signed long long)((f) * ((unsigned long long)1 << (19+20)) + (1<<19)) >> 20)

/** @brief Convert `double` value to Q14.18 fixed-point value, with rounding. 
 * @ingroup qfmt_macros
*/
#define Q18(f) (int)((signed long long)((f) * ((unsigned long long)1 << (18+20)) + (1<<19)) >> 20)

/** @brief Convert `double` value to Q15.17 fixed-point value, with rounding. 
 * @ingroup qfmt_macros
*/
#define Q17(f) (int)((signed long long)((f) * ((unsigned long long)1 << (17+20)) + (1<<19)) >> 20)

/** @brief Convert `double` value to Q16.16 fixed-point value, with rounding. 
 * @ingroup qfmt_macros
*/
#define Q16(f) (int)((signed long long)((f) * ((unsigned long long)1 << (16+20)) + (1<<19)) >> 20)

/** @brief Convert `double` value to Q17.15 fixed-point value, with rounding. 
 * @ingroup qfmt_macros
*/
#define Q15(f) (int)((signed long long)((f) * ((unsigned long long)1 << (15+20)) + (1<<19)) >> 20)

/** @brief Convert `double` value to Q18.14 fixed-point value, with rounding. 
 * @ingroup qfmt_macros
*/
#define Q14(f) (int)((signed long long)((f) * ((unsigned long long)1 << (14+20)) + (1<<19)) >> 20)

/** @brief Convert `double` value to Q19.13 fixed-point value, with rounding. 
 * @ingroup qfmt_macros
*/
#define Q13(f) (int)((signed long long)((f) * ((unsigned long long)1 << (13+20)) + (1<<19)) >> 20)

/** @brief Convert `double` value to Q20.12 fixed-point value, with rounding. 
 * @ingroup qfmt_macros
*/
#define Q12(f) (int)((signed long long)((f) * ((unsigned long long)1 << (12+20)) + (1<<19)) >> 20)

/** @brief Convert `double` value to Q21.11 fixed-point value, with rounding. 
 * @ingroup qfmt_macros
*/
#define Q11(f) (int)((signed long long)((f) * ((unsigned long long)1 << (11+20)) + (1<<19)) >> 20)

/** @brief Convert `double` value to Q22.10 fixed-point value, with rounding. 
 * @ingroup qfmt_macros
*/
#define Q10(f) (int)((signed long long)((f) * ((unsigned long long)1 << (10+20)) + (1<<19)) >> 20)

/** @brief Convert `double` value to Q23.9 fixed-point value, with rounding. 
 * @ingroup qfmt_macros
*/
#define Q9(f)  (int)((signed long long)((f) * ((unsigned long long)1 << (9+20)) + (1<<19)) >> 20)

/** @brief Convert `double` value to Q24.8 fixed-point value, with rounding. 
 * @ingroup qfmt_macros
*/
#define Q8(f)  (int)((signed long long)((f) * ((unsigned long long)1 << (8+20)) + (1<<19)) >> 20)

// Convert from fixed point to double precision floating point
// The number indicates the fractional bits or the position of the binary point

/** @brief Convert Q1.31 fixed-point value to `double` value.
 * @ingroup qfmt_macros
*/
#define F31(x) ((double)(x)/(double)(uint32_t)(1<<31)) // needs uint32_t cast because bit 31 is 1

/** @brief Convert Q2.30 fixed-point value to `double` value.
 * @ingroup qfmt_macros
*/
#define F30(x) ((double)(x)/(double)(1<<30))

/** @brief Convert Q3.29 fixed-point value to `double` value.
 * @ingroup qfmt_macros
*/
#define F29(x) ((double)(x)/(double)(1<<29))

/** @brief Convert Q4.28 fixed-point value to `double` value.
 * @ingroup qfmt_macros
*/
#define F28(x) ((double)(x)/(double)(1<<28))

/** @brief Convert Q5.27 fixed-point value to `double` value.
 * @ingroup qfmt_macros
*/
#define F27(x) ((double)(x)/(double)(1<<27))

/** @brief Convert Q6.26 fixed-point value to `double` value.
 * @ingroup qfmt_macros
*/
#define F26(x) ((double)(x)/(double)(1<<26))

/** @brief Convert Q7.25 fixed-point value to `double` value.
 * @ingroup qfmt_macros
*/
#define F25(x) ((double)(x)/(double)(1<<25))

/** @brief Convert Q8.24 fixed-point value to `double` value.
 * @ingroup qfmt_macros
*/
#define F24(x) ((double)(x)/(double)(1<<24))

/** @brief Convert Q9.23 fixed-point value to `double` value.
 * @ingroup qfmt_macros
*/
#define F23(x) ((double)(x)/(double)(1<<23))

/** @brief Convert Q10.22 fixed-point value to `double` value.
 * @ingroup qfmt_macros
*/
#define F22(x) ((double)(x)/(double)(1<<22))

/** @brief Convert Q11.21 fixed-point value to `double` value.
 * @ingroup qfmt_macros
*/
#define F21(x) ((double)(x)/(double)(1<<21))

/** @brief Convert Q12.20 fixed-point value to `double` value.
 * @ingroup qfmt_macros
*/
#define F20(x) ((double)(x)/(double)(1<<20))

/** @brief Convert Q13.19 fixed-point value to `double` value.
 * @ingroup qfmt_macros
*/
#define F19(x) ((double)(x)/(double)(1<<19))

/** @brief Convert Q14.18 fixed-point value to `double` value.
 * @ingroup qfmt_macros
*/
#define F18(x) ((double)(x)/(double)(1<<18))

/** @brief Convert Q15.17 fixed-point value to `double` value.
 * @ingroup qfmt_macros
*/
#define F17(x) ((double)(x)/(double)(1<<17))

/** @brief Convert Q16.16 fixed-point value to `double` value.
 * @ingroup qfmt_macros
*/
#define F16(x) ((double)(x)/(double)(1<<16))

// short

/** @brief Convert Q17.15 fixed-point value to `double` value.
 * @ingroup qfmt_macros
*/
#define F15(x) ((double)(x)/(double)(1<<15))

/** @brief Convert Q18.14 fixed-point value to `double` value.
 * @ingroup qfmt_macros
*/
#define F14(x) ((double)(x)/(double)(1<<14))

/** @brief Convert Q19.13 fixed-point value to `double` value.
 * @ingroup qfmt_macros
*/
#define F13(x) ((double)(x)/(double)(1<<13))

/** @brief Convert Q20.12 fixed-point value to `double` value.
 * @ingroup qfmt_macros
*/
#define F12(x) ((double)(x)/(double)(1<<12))

/** @brief Convert Q21.11 fixed-point value to `double` value.
 * @ingroup qfmt_macros
*/
#define F11(x) ((double)(x)/(double)(1<<11))

/** @brief Convert Q22.10 fixed-point value to `double` value.
 * @ingroup qfmt_macros
*/
#define F10(x) ((double)(x)/(double)(1<<10))

/** @brief Convert Q23.9 fixed-point value to `double` value.
 * @ingroup qfmt_macros
*/
#define F9(x)  ((double)(x)/(double)(1<<9))

/** @brief Convert Q24.8 fixed-point value to `double` value.
 * @ingroup qfmt_macros
*/
#define F8(x)  ((double)(x)/(double)(1<<8))
