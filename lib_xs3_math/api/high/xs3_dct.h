


#ifndef XS3_DCT_H_
#define XS3_DCT_H_

#include "xs3_math_types.h"

#ifdef __XC__
extern "C" {
#endif

/** Perform a forward DCT on a BFP vector of samples.
 * 
 * Data is transformed in-place.
 * 
 * ``input->length`` must be one of: 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096
 * 
 * ``input->exp`` and ``input->hr`` will be appropriately adjusted.
 * 
 * \param input   Vector of time-domain input to be transformed.
 */
void bfp_dct_forward( bfp_s32_t* input);

/** Perform an inverse DCT on a BFP vector of frequency domain samples.
 * 
 * ``input->length`` must be one of: 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096
 * 
 * ``input->exp`` and ``input->hr`` will be appropriately adjusted.
 * 
 * \param input     Vector of frequency-domain samples to be transformed.
 */
void bfp_dct_inverse( bfp_complex_s32_t* input );


#ifdef __XC__
}   //extern "C"
#endif

#endif //XS3_DCT_H_