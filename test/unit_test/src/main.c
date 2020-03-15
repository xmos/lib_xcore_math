

#include <stdio.h>

#include "unity.h"
#include "test_cases.h"

int main(int argc, char** argv)
{
    int retval = 0;
    
    if(!UnityParseOptions(argc, argv)){

        // retval = test_xs3_set_array();
        // retval = test_xs3_cls_array();
        // retval = test_xs3_shl_vect();
        // retval = test_xs3_add_vect();
        // retval = test_xs3_sub_vect();
        // retval = test_xs3_mul_vect();
        // retval = test_xs3_scalar_mul_vect();
        // retval = test_xs3_rect_vect();
        // retval = test_xs3_abs_vect();

        retval = test_xs3_sum();
        
        // retval = test_xs3_s32_to_s16();
        // retval = test_xs3_s16_to_s32();

        // retval = test_xs3_bfp_init();

    }
    return retval;
}
