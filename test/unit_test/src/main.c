

#include <stdio.h>

#include "unity.h"
#include "test_cases.h"

int main(int argc, char** argv)
{
    int retval = 0;
    
    if(!UnityParseOptions(argc, argv)){

        retval = test_xs3_set_array();
        retval = test_xs3_cls_array();
        retval = test_xs3_shl_vect();
        retval = test_xs3_add_vect();
        retval = test_xs3_sub_vect();

        // retval = test_xs3_bfp_init();

    }
    return retval;
}
