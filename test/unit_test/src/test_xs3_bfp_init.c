
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xs3_math.h"

#include "tst_common.h"

#include "unity.h"



static void test_bfp_init_vect_s16_case0()
{

}

#define RUN(TEST) do{UnityBegin(__FILE__); RUN_TEST(TEST); retval = UnityEnd(); printf("\n\n");}while(0);

int test_xs3_bfp_init()
{
    int retval = 0;

    RUN(test_bfp_init_vect_s16_case0);

    return retval;
}