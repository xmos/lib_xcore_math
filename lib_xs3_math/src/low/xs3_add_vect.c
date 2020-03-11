
#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "vpu_helper.h"



WEAK_FUNC
headroom_t xs3_add_vect_s16__(
    int16_t* a,
    const int16_t* b,
    const int16_t* c,
    const unsigned length,
    const int b_shr,
    const int c_shr)
{
    unsigned hr_mask = 0;
    for(int k = 0; k < length; k++){
        
        int16_t B = ASHR(16)(b[k], b_shr);
        int16_t C = ASHR(16)(c[k], c_shr);

        int16_t A = SAT(16)(B + C);
        
        hr_mask = HRMASK_ADD(hr_mask, A);

        a[k] = A;   
    }

    return hr_from_mask(hr_mask);
}


WEAK_FUNC
headroom_t xs3_add_vect_s32__(
    int32_t* a,
    const int32_t* b,
    const int32_t* c,
    const unsigned length,
    const int b_shr,
    const int c_shr)
{
    unsigned hr_mask = 0;
    for(int k = 0; k < length; k++){
        
        int32_t B = ASHR(32)(b[k], b_shr);
        int32_t C = ASHR(32)(c[k], c_shr);

        int32_t A = SAT(32)(B + C);
        
        hr_mask = HRMASK_ADD(hr_mask, A);

        a[k] = A;   
    }

    return hr_from_mask(hr_mask);
}