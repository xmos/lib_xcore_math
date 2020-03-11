
#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "vpu_helper.h"



WEAK_FUNC
headroom_t xs3_cls_array_s16(
    const int16_t* v,
    const unsigned length)
{
    int16_t largest = 0;
    unsigned ldex = 0;

    for(int k = 0; k < length; k++){
        uint16_t pt =  v[k];
        uint16_t nt = -v[k];

        if(v[k] >= 0 && pt >= largest){
            largest = pt;
            ldex = k;
        } else if(v[k] < 0 && nt > largest) {
            largest = nt;
            ldex = k;
        }
    }
    
    //TODO: got to be a more efficient (platform-independent way to do this..)
    int16_t lval = v[ldex];

    for(int i = 14; i >= 0; i--){
        uint16_t mask = (1<<(i));
        int16_t lmod;
        if(lval >= 0)
            lmod = lval & ~mask;
        else
            lmod = lval | mask;

        if(lmod != lval){
            return 14 - i;
        }
    }

    return 15;
}



WEAK_FUNC
headroom_t xs3_cls_array_s32(
    const int32_t* v,
    const unsigned length)
{
    int32_t largest = 0;
    unsigned ldex = 0;

    for(int k = 0; k < length; k++){
        int32_t pt =  v[k];
        int32_t nt = -v[k];

        if(v[k] >= 0 && pt >= largest){
            largest = pt;
            ldex = k;
        } else if(v[k] < 0 && nt > largest) {
            largest = nt;
            ldex = k;
        }
    }
    
    //TODO: got to be a more efficient (platform-independent way to do this..)
    int32_t lval = v[ldex];

    for(int i = 30; i >= 0; i--){
        uint32_t mask = (1<<(i));
        int32_t lmod;
        if(lval >= 0)
            lmod = lval & ~mask;
        else
            lmod = lval | mask;
         
        if(lmod != lval){
            return 30 - i;
        }
    }

    return 31;
}

