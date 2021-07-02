// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "xs3_math.h"
#include "xs3_vpu_scalar_ops.h"
#include "xs3_vpu_info.h"

#include "../../tst_common.h"

#include "unity.h"

static unsigned seed = 2314567;

#if DEBUG_ON || 0
#undef DEBUG_ON
#define DEBUG_ON    (1)
#endif

#define REPS        (1000)




static void test_vladd16()
{
    PRINTF("%s...\n", __func__);
    seed = 12345;

    TEST_ASSERT_EQUAL_INT16(              0, vladd16(       0,      0));
    TEST_ASSERT_EQUAL_INT16(              1, vladd16(       1,      0));
    TEST_ASSERT_EQUAL_INT16(             -1, vladd16(       0,     -1));
    TEST_ASSERT_EQUAL_INT16(              0, vladd16(       1,     -1));
    TEST_ASSERT_EQUAL_INT16(           1246, vladd16(    1234,     12));
    TEST_ASSERT_EQUAL_INT16(         -22940, vladd16(      60, -23000));
    TEST_ASSERT_EQUAL_INT16(  VPU_INT16_MAX, vladd16(  0x7F00,  0x100));
    TEST_ASSERT_EQUAL_INT16(  VPU_INT16_MIN, vladd16( -0x7F00, -0x100));


    for(int v = 0; v < REPS; v++){
        PRINTF("\trepetition %d.. (seed: 0x%08X)\n", v, seed);

        int16_t x = pseudo_rand_int16(&seed);
        int16_t y = pseudo_rand_int16(&seed);

        int32_t exp = x + y;
        exp = MIN(exp, VPU_INT16_MAX);
        exp = MAX(exp, VPU_INT16_MIN);

        int16_t res = vladd16(x, y);

        TEST_ASSERT_EQUAL_INT16(exp, res);
    }
}


static void test_vlsub16()
{
    PRINTF("%s...\n", __func__);
    seed = 12345;

    TEST_ASSERT_EQUAL_INT16(              0, vlsub16(       0,      0));
    TEST_ASSERT_EQUAL_INT16(              1, vlsub16(       1,      0));
    TEST_ASSERT_EQUAL_INT16(              1, vlsub16(       0,     -1));
    TEST_ASSERT_EQUAL_INT16(              2, vlsub16(       1,     -1));
    TEST_ASSERT_EQUAL_INT16(           1222, vlsub16(    1234,     12));
    TEST_ASSERT_EQUAL_INT16(          23060, vlsub16(      60, -23000));
    TEST_ASSERT_EQUAL_INT16(  VPU_INT16_MAX, vlsub16(  0x7F00, -0x100));
    TEST_ASSERT_EQUAL_INT16(  VPU_INT16_MIN, vlsub16( -0x7F00,  0x100));


    for(int v = 0; v < REPS; v++){
        PRINTF("\trepetition %d.. (seed: 0x%08X)\n", v, seed);

        int16_t x = pseudo_rand_int16(&seed);
        int16_t y = pseudo_rand_int16(&seed);

        int32_t exp = x - y;
        exp = MIN(exp, VPU_INT16_MAX);
        exp = MAX(exp, VPU_INT16_MIN);

        int16_t res = vlsub16(x, y);

        TEST_ASSERT_EQUAL_INT16(exp, res);
    }
}


static void test_vlashr16()
{
    PRINTF("%s...\n", __func__);
    seed = 12345;

    TEST_ASSERT_EQUAL_INT16(             0, vlashr16(     0,      0));
    TEST_ASSERT_EQUAL_INT16(             1, vlashr16(     1,      0));
    TEST_ASSERT_EQUAL_INT16(             2, vlashr16(     2,      0));
    TEST_ASSERT_EQUAL_INT16(             1, vlashr16(     2,      1));
    TEST_ASSERT_EQUAL_INT16(             0, vlashr16(     2,      2));
    TEST_ASSERT_EQUAL_INT16(             0, vlashr16(     2,      3));
    TEST_ASSERT_EQUAL_INT16(             4, vlashr16(     2,     -1));
    
    TEST_ASSERT_EQUAL_INT16(            -2, vlashr16(    -2,      0));
    TEST_ASSERT_EQUAL_INT16(            -1, vlashr16(    -2,      1));
    TEST_ASSERT_EQUAL_INT16(            -1, vlashr16(    -2,      2));
    TEST_ASSERT_EQUAL_INT16(            -1, vlashr16(    -2,      3));
    TEST_ASSERT_EQUAL_INT16(            -4, vlashr16(    -2,     -1));
    
    TEST_ASSERT_EQUAL_INT16(             5, vlashr16(    10,      1));
    TEST_ASSERT_EQUAL_INT16(             2, vlashr16(    10,      2));
    TEST_ASSERT_EQUAL_INT16(             1, vlashr16(    10,      3));
    
    TEST_ASSERT_EQUAL_INT16(        0xFFFD, vlashr16(0xFFFA,      1));
    TEST_ASSERT_EQUAL_INT16(        0xFFFE, vlashr16(0xFFFA,      2));
    TEST_ASSERT_EQUAL_INT16(        0xFFFF, vlashr16(0xFFFA,      3));
    
    TEST_ASSERT_EQUAL_INT16(            64, vlashr16(     1,     -6));
    TEST_ASSERT_EQUAL_INT16(           -64, vlashr16(    -1,     -6));

    TEST_ASSERT_EQUAL_INT16( VPU_INT16_MAX, vlashr16(     1,     -15));
    TEST_ASSERT_EQUAL_INT16( VPU_INT16_MIN, vlashr16(    -1,     -15));


    for(int v = 0; v < REPS; v++){
        PRINTF("\trepetition %d.. (seed: 0x%08X)\n", v, seed);

        int16_t x = pseudo_rand_int16(&seed);
        right_shift_t shr = pseudo_rand_int(&seed, -18, 18);


        double fexp = floor(ldexp(x, -shr));
        fexp = MIN(fexp, VPU_INT16_MAX);
        fexp = MAX(fexp, VPU_INT16_MIN);

        int16_t exp = fexp;

        int16_t res = vlashr16(x, shr);
    
        TEST_ASSERT_EQUAL_INT16(exp, res);
    }
}


static void test_vpos16()
{
    PRINTF("%s...\n", __func__);

    for(int k = 0; k < INT16_MAX; k+= 13)
        TEST_ASSERT_EQUAL_INT16( k, vpos16( (int16_t) k));
    
    for(int k = INT16_MIN; k < 0; k+=17)
        TEST_ASSERT_EQUAL_INT16( 0, vpos16( (int16_t) k));
    
}


static void test_vsign16()
{
    PRINTF("%s...\n", __func__);


    for(int k = 0; k < INT16_MAX; k+=13)
        TEST_ASSERT_EQUAL_INT16(  ((int16_t)  0x4000), vsign16( (int16_t) k)  );
    
    for(int k = INT16_MIN; k < 0; k+=17)
        TEST_ASSERT_EQUAL_INT16(  ((int16_t) -0x4000), vsign16( (int16_t) k)  );

}


static void test_vdepth1_16()
{
    PRINTF("%s...\n", __func__);

    for(int k = 0; k < INT16_MAX; k+=13)
        TEST_ASSERT_EQUAL_INT( 0, vdepth1_16( (int16_t) k));
    
    for(int k = INT16_MIN; k < 0; k+=17)
        TEST_ASSERT_EQUAL_INT( 1, vdepth1_16( (int16_t) k));

}


static void test_vdepth8_16()
{
    PRINTF("%s...\n", __func__);

    TEST_ASSERT_EQUAL_INT8(      0, vdepth8_16(          0));
    TEST_ASSERT_EQUAL_INT8(      0, vdepth8_16(       0x7F));
    TEST_ASSERT_EQUAL_INT8(      1, vdepth8_16(       0x80));
    TEST_ASSERT_EQUAL_INT8(      2, vdepth8_16(      0x200));
    TEST_ASSERT_EQUAL_INT8(   0x7F, vdepth8_16(     0x7FFF));
    TEST_ASSERT_EQUAL_INT8(      0, vdepth8_16(         -1));
    TEST_ASSERT_EQUAL_INT8(      0, vdepth8_16(      -0x80));
    TEST_ASSERT_EQUAL_INT8(     -1, vdepth8_16(      -0x81));
    TEST_ASSERT_EQUAL_INT8(     -2, vdepth8_16(    -0x0280));
    TEST_ASSERT_EQUAL_INT8(     -3, vdepth8_16(    -0x0281));
    TEST_ASSERT_EQUAL_INT8(  -0x7F, vdepth8_16(    -0x7FFF));

    for(int k = INT16_MIN; k < INT16_MAX; k += 13)
    {

        int8_t res = vdepth8_16( (int16_t) k );

        int32_t exp = round(ldexp( k, -8) + ldexp(1, -30));
        exp = MIN(exp, VPU_INT8_MAX);
        exp = MAX(exp, VPU_INT8_MIN);

        TEST_ASSERT_EQUAL_INT8( exp, res );

    }

}


static void test_vlmul16()
{
    PRINTF("%s...\n", __func__);
    seed = 34563;

    TEST_ASSERT_EQUAL_INT16(             0, vlmul16(       0,      0));
    TEST_ASSERT_EQUAL_INT16(             1, vlmul16(       1,   0x4000));
    TEST_ASSERT_EQUAL_INT16(            -1, vlmul16(       1,  -0x4000));
    TEST_ASSERT_EQUAL_INT16(           123, vlmul16(     123,   0x4000));
    TEST_ASSERT_EQUAL_INT16(          -123, vlmul16(     123,  -0x4000));
    TEST_ASSERT_EQUAL_INT16(        -12322, vlmul16(   24644,  -0x2000));
    TEST_ASSERT_EQUAL_INT16(         12322, vlmul16(   24644,   0x2000));
    TEST_ASSERT_EQUAL_INT16(         12323, vlmul16(   24645,   0x2000));
    TEST_ASSERT_EQUAL_INT16( VPU_INT16_MIN, vlmul16( -0x8000,   0x4000));
    TEST_ASSERT_EQUAL_INT16( VPU_INT16_MAX, vlmul16( -0x7FFF,  -0x7FFF));


    for(int v = 0; v < REPS; v++){
        PRINTF("\trepetition %d.. (seed: 0x%08X)\n", v, seed);

        int16_t x = pseudo_rand_int16(&seed);
        int16_t y = pseudo_rand_int16(&seed);

        // final term is because negative ties round differently on the VPU and in floating point
        double exp = round( x * ldexp(y, -14)  +  ldexp(1,-30) );

        exp = MIN(exp, VPU_INT16_MAX);
        exp = MAX(exp, VPU_INT16_MIN);

        int16_t res = vlmul16(x, y);

        TEST_ASSERT_EQUAL_INT16(exp, res);
    }
}



static void test_vlmacc16()
{
    PRINTF("%s...\n", __func__);
    seed = 778786;

    TEST_ASSERT_EQUAL_INT32(             0, vlmacc16(              0,     0,     0) ); 
    TEST_ASSERT_EQUAL_INT32(           100, vlmacc16(            100,     0,     0) ); 
    TEST_ASSERT_EQUAL_INT32(          -100, vlmacc16(           -100,     0,     0) ); 
    TEST_ASSERT_EQUAL_INT32(           100, vlmacc16(            100,     1,     0) ); 
    TEST_ASSERT_EQUAL_INT32(            94, vlmacc16(            100,     2,    -3) ); 
    TEST_ASSERT_EQUAL_INT32( VPU_INT32_MIN, vlmacc16(  VPU_INT32_MIN,    -1,     1) ); 
    TEST_ASSERT_EQUAL_INT32( VPU_INT32_MAX, vlmacc16(  VPU_INT32_MAX,     1,     1) ); 

    for(int v = 0; v < REPS; v++){
        PRINTF("\trepetition %d.. (seed: 0x%08X)\n", v, seed);

        int32_t acc = pseudo_rand_int32(&seed);
        int16_t x = pseudo_rand_int16(&seed);
        int16_t y = pseudo_rand_int16(&seed);

        int64_t s = acc;
        s += ((int32_t)x) * y;

        s = MIN(s, VPU_INT32_MAX);
        s = MAX(s, VPU_INT32_MIN);

        int32_t res = vlmacc16(acc, x, y);

        TEST_ASSERT_EQUAL_INT32(s, res);
    }
}



static void test_vlmaccr16()
{
    PRINTF("%s...\n", __func__);
    seed = 778786;


    for(int v = 0; v < REPS; v++){
        PRINTF("\trepetition %d.. (seed: 0x%08X)\n", v, seed);

        int32_t acc = pseudo_rand_int32(&seed);
        int16_t x[VPU_INT16_EPV];
        int16_t y[VPU_INT16_EPV];

        int64_t s = acc;

        for(int i = 0; i < VPU_INT16_EPV; i++){
            x[i] = pseudo_rand_int16(&seed);
            y[i] = pseudo_rand_int16(&seed);

            s += ((int64_t)x[i]) * y[i];

        }

        s = MIN(s, VPU_INT32_MAX);
        s = MAX(s, VPU_INT32_MIN);

        int32_t res = vlmaccr16(acc, x, y);

        // if(s != res){
        //     printf("rep: %d \t\tseed: 0x%08X\n", v, seed);
        //     printf("acc = %ld\n", acc);
        //     printf("x = [ ");
        //     for(int i = 0; i < 16; i++)
        //         printf("%d, ", x[i]);
        //     printf(" ]\n");
            
        //     printf("y = [ ");
        //     for(int i = 0; i < 16; i++)
        //         printf("%d, ", y[i]);
        //     printf(" ]\n");
        // }

        TEST_ASSERT_EQUAL_INT32(s, res);
    }
}



static void test_vlsat16()
{
    PRINTF("%s...\n", __func__);
    seed = 6457;

    TEST_ASSERT_EQUAL_INT16(            0, vlsat16(            0,      0));
    TEST_ASSERT_EQUAL_INT16(            1, vlsat16(            1,      0));
    TEST_ASSERT_EQUAL_INT16(            1, vlsat16(            2,      1));
    TEST_ASSERT_EQUAL_INT16(            1, vlsat16(            2,      2));
    TEST_ASSERT_EQUAL_INT16(            0, vlsat16(            2,      3));
    TEST_ASSERT_EQUAL_INT16(         0x0A, vlsat16(   0x0A7FFFFF,     24));
    TEST_ASSERT_EQUAL_INT16(         0x0B, vlsat16(   0x0A800000,     24));
    TEST_ASSERT_EQUAL_INT16(        -0x0A, vlsat16(  -0x0A800000,     24));
    TEST_ASSERT_EQUAL_INT16(        -0x0B, vlsat16(  -0x0A800001,     24));


    for(int v = 0; v < REPS; v++){
        PRINTF("\trepetition %d.. (seed: 0x%08X)\n", v, seed);

        headroom_t hr = pseudo_rand_uint(&seed, 0, 32);        
        int32_t acc = pseudo_rand_int32(&seed) >> hr;

        int16_t shr = pseudo_rand_int(&seed, 23 - hr, 25 - hr);

        if(shr < 0)
            shr = 0;


        double fexp = ldexp(acc, 0);
        
        // final term is because negative ties round differently on the VPU and in floating point
        fexp = round( fexp * ldexp(1, -shr) + ldexp(1,-30) );

        fexp = MIN(fexp, VPU_INT16_MAX);
        fexp = MAX(fexp, VPU_INT16_MIN);

        int16_t exp = fexp;

        int16_t res = vlsat16(acc, shr);

        // printf("acc = %ld\n", acc);
        // printf("shr = %d\n", shr);
        // printf("exp = %d\n", exp);
        // printf("res = %d\n", res);

        TEST_ASSERT_EQUAL_INT16(exp, res);
    }
}



static void test_vadddr16()
{
    PRINTF("%s...\n", __func__);
    seed = 778786;


    for(int v = 0; v < REPS; v++){
        PRINTF("\trepetition %d.. (seed: 0x%08X)\n", v, seed);

        vpu_int16_acc_t acc[VPU_INT16_ACC_PERIOD];

        int64_t total = 0;

        for(int i = 0; i < VPU_INT16_ACC_PERIOD; i++){
            acc[i] = i;//pseudo_rand_int16(&seed);
            total += acc[i];
        }


        vpu_int16_acc_t expected = 
            (total > VPU_INT32_MAX)? VPU_INT32_MAX
          : (total < VPU_INT32_MIN)? VPU_INT32_MIN 
          : total;


        vpu_int16_acc_t result = vadddr16(acc);

        TEST_ASSERT_EQUAL_INT32(expected, result);
    }
}





void test_xs3_vpu_scalar_ops_s16()
{
    SET_TEST_FILE();

    RUN_TEST(test_vladd16);
    RUN_TEST(test_vlsub16);
    RUN_TEST(test_vlashr16);
    RUN_TEST(test_vpos16);
    RUN_TEST(test_vsign16);
    RUN_TEST(test_vdepth1_16);
    RUN_TEST(test_vdepth8_16);
    RUN_TEST(test_vlmul16);
    RUN_TEST(test_vlmacc16);
    RUN_TEST(test_vlmaccr16);
    RUN_TEST(test_vlsat16);
    RUN_TEST(test_vadddr16);
}
