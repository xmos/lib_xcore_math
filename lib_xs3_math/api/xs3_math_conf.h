
#ifndef XS3_MATH_CONF_H_
#define XS3_MATH_CONF_H_

#ifdef __xs3_math_user_conf_h_exists__
#include "xs3_math_user_conf.h"
#endif


/**
 * Iff true, vectorized BFP functions which take multiple vectors as input will check to 
 * ensure that each BFP vector has the same length.
 */
#ifndef XS3_BFP_DEBUG_CHECK_LENGTHS
#define XS3_BFP_DEBUG_CHECK_LENGTHS (0)
#endif


/**
 * Iff true, certain BFP operations may result in values that would have been INT32_MIN
 * saturating to INT32_MIN+1 (for 32-bit BFP operations), or INT16_MIN values saturating
 * to INT16_MIN+1 (for 16-bit operations).
 * 
 * This saturation can be prevented by forcing that there always be 1 bit of headroom in
 * a result (save for the case in which the result contains INT*_MIN), and thus comes at
 * the cost of one fewer possible bit of precision in results.
 */
#ifndef XS3_BFP_ALLOW_SATURATION
#define XS3_BFP_ALLOW_SATURATION (1)
#endif



#endif //XS3_MATH_CONF_H_