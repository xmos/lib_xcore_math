// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "xmath/xmath.h"
#include "xmath/xs3/vpu_scalar_ops.h"
#include "xmath/xs3/vpu_info.h"

#include "tst_common.h"

#include "unity_fixture.h"

TEST_GROUP_RUNNER(vpu_scalar_ops_s8) {
  RUN_TEST_CASE(vpu_scalar_ops_s8, vladd8);
  RUN_TEST_CASE(vpu_scalar_ops_s8, vlsub8);
  RUN_TEST_CASE(vpu_scalar_ops_s8, vlashr8);
  RUN_TEST_CASE(vpu_scalar_ops_s8, vpos8);
  RUN_TEST_CASE(vpu_scalar_ops_s8, vsign8);
  RUN_TEST_CASE(vpu_scalar_ops_s8, vdepth1_8);
  RUN_TEST_CASE(vpu_scalar_ops_s8, vlmul8);
  RUN_TEST_CASE(vpu_scalar_ops_s8, vlmacc8);
  RUN_TEST_CASE(vpu_scalar_ops_s8, vlmaccr8);
  RUN_TEST_CASE(vpu_scalar_ops_s8, vlsat8);
}

TEST_GROUP(vpu_scalar_ops_s8);
TEST_SETUP(vpu_scalar_ops_s8) { fflush(stdout); }
TEST_TEAR_DOWN(vpu_scalar_ops_s8) {}


#if SMOKE_TEST
#  define REPS       (100)
#else
#  define REPS       (1000)
#endif


TEST(vpu_scalar_ops_s8, vladd8)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    TEST_ASSERT_EQUAL_INT8(            0, vladd8(     0,      0));
    TEST_ASSERT_EQUAL_INT8(            1, vladd8(     1,      0));
    TEST_ASSERT_EQUAL_INT8(           -1, vladd8(     0,     -1));
    TEST_ASSERT_EQUAL_INT8(            0, vladd8(     1,     -1));
    TEST_ASSERT_EQUAL_INT8(           22, vladd8(    10,     12));
    TEST_ASSERT_EQUAL_INT8(           37, vladd8(    60,    -23));
    TEST_ASSERT_EQUAL_INT8( VPU_INT8_MAX, vladd8(    70,     80));
    TEST_ASSERT_EQUAL_INT8( VPU_INT8_MIN, vladd8(   -70,   -120));


    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        int8_t x = pseudo_rand_int8(&seed);
        int8_t y = pseudo_rand_int8(&seed);

        int32_t exp = x + y;
        exp = MIN(exp, VPU_INT8_MAX);
        exp = MAX(exp, VPU_INT8_MIN);

        int8_t res = vladd8(x, y);

        TEST_ASSERT_EQUAL_INT8(exp, res);
    }
}


TEST(vpu_scalar_ops_s8, vlsub8)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    TEST_ASSERT_EQUAL_INT8(            0, vlsub8(     0,      0));
    TEST_ASSERT_EQUAL_INT8(            1, vlsub8(     1,      0));
    TEST_ASSERT_EQUAL_INT8(            1, vlsub8(     0,     -1));
    TEST_ASSERT_EQUAL_INT8(            2, vlsub8(     1,     -1));
    TEST_ASSERT_EQUAL_INT8(           -2, vlsub8(    10,     12));
    TEST_ASSERT_EQUAL_INT8(           83, vlsub8(    60,    -23));
    TEST_ASSERT_EQUAL_INT8( VPU_INT8_MAX, vlsub8(    70,    -80));
    TEST_ASSERT_EQUAL_INT8( VPU_INT8_MIN, vlsub8(   -70,    120));


    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        int8_t x = pseudo_rand_int8(&seed);
        int8_t y = pseudo_rand_int8(&seed);

        int32_t exp = x - y;
        exp = MIN(exp, VPU_INT8_MAX);
        exp = MAX(exp, VPU_INT8_MIN);

        int8_t res = vlsub8(x, y);

        TEST_ASSERT_EQUAL_INT8(exp, res);
    }
}


TEST(vpu_scalar_ops_s8, vlashr8)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    TEST_ASSERT_EQUAL_INT8(            0, vlashr8(     0,      0));
    TEST_ASSERT_EQUAL_INT8(            1, vlashr8(     1,      0));
    TEST_ASSERT_EQUAL_INT8(            2, vlashr8(     2,      0));
    TEST_ASSERT_EQUAL_INT8(            1, vlashr8(     2,      1));
    TEST_ASSERT_EQUAL_INT8(            0, vlashr8(     2,      2));
    TEST_ASSERT_EQUAL_INT8(            0, vlashr8(     2,      3));
    TEST_ASSERT_EQUAL_INT8(            4, vlashr8(     2,     -1));
    
    TEST_ASSERT_EQUAL_INT8(           -2, vlashr8(    -2,      0));
    TEST_ASSERT_EQUAL_INT8(           -1, vlashr8(    -2,      1));
    TEST_ASSERT_EQUAL_INT8(           -1, vlashr8(    -2,      2));
    TEST_ASSERT_EQUAL_INT8(           -1, vlashr8(    -2,      3));
    TEST_ASSERT_EQUAL_INT8(           -4, vlashr8(    -2,     -1));
    
    TEST_ASSERT_EQUAL_INT8(            5, vlashr8(    10,      1));
    TEST_ASSERT_EQUAL_INT8(            2, vlashr8(    10,      2));
    TEST_ASSERT_EQUAL_INT8(            1, vlashr8(    10,      3));
    
    TEST_ASSERT_EQUAL_INT8(         0xFD, vlashr8(  0xFA,      1));
    TEST_ASSERT_EQUAL_INT8(         0xFE, vlashr8(  0xFA,      2));
    TEST_ASSERT_EQUAL_INT8(         0xFF, vlashr8(  0xFA,      3));
    
    TEST_ASSERT_EQUAL_INT8(           64, vlashr8(     1,     -6));
    TEST_ASSERT_EQUAL_INT8(          -64, vlashr8(    -1,     -6));
    TEST_ASSERT_EQUAL_INT8( VPU_INT8_MAX, vlashr8(     1,     -7));
    TEST_ASSERT_EQUAL_INT8( VPU_INT8_MIN, vlashr8(    -1,     -7));


    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        int8_t x = pseudo_rand_int8(&seed);
        right_shift_t shr = pseudo_rand_int(&seed, -10, 10);

        int32_t exp = (shr >= 0)? (x >> shr) : (x << (-shr));
        exp = MIN(exp, VPU_INT8_MAX);
        exp = MAX(exp, VPU_INT8_MIN);

        int8_t res = vlashr8(x, shr);
    
        TEST_ASSERT_EQUAL_INT8(exp, res);
    }
}


TEST(vpu_scalar_ops_s8, vpos8)
{
    

    for(int k = 0; k < INT8_MAX; k++)
        TEST_ASSERT_EQUAL_INT8( k, vpos8( (int8_t) k));
    
    for(int k = INT8_MIN; k < 0; k++)
        TEST_ASSERT_EQUAL_INT8( 0, vpos8( (int8_t) k));
    
}


TEST(vpu_scalar_ops_s8, vsign8)
{
    


    for(int k = 0; k < INT8_MAX; k++)
        TEST_ASSERT_EQUAL_INT8(  ((int8_t)  0x40), vsign8( (int8_t) k)  );
    
    for(int k = INT8_MIN; k < 0; k++)
        TEST_ASSERT_EQUAL_INT8(  ((int8_t) -0x40), vsign8( (int8_t) k)  );

}


TEST(vpu_scalar_ops_s8, vdepth1_8)
{
    

    for(int k = 0; k < INT8_MAX; k++)
        TEST_ASSERT_EQUAL_INT( 0, vdepth1_8( (int8_t) k));
    
    for(int k = INT8_MIN; k < 0; k++)
        TEST_ASSERT_EQUAL_INT( 1, vdepth1_8( (int8_t) k));

}


TEST(vpu_scalar_ops_s8, vlmul8)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    TEST_ASSERT_EQUAL_INT8(            0, vlmul8(     0,      0));
    TEST_ASSERT_EQUAL_INT8(            1, vlmul8(     1,   0x40));
    TEST_ASSERT_EQUAL_INT8(           -1, vlmul8(     1,  -0x40));
    TEST_ASSERT_EQUAL_INT8(          123, vlmul8(   123,   0x40));
    TEST_ASSERT_EQUAL_INT8(         -123, vlmul8(   123,  -0x40));
    TEST_ASSERT_EQUAL_INT8( VPU_INT8_MIN, vlmul8( -0x80,   0x40));
    TEST_ASSERT_EQUAL_INT8( VPU_INT8_MAX, vlmul8( -0x7F,  -0x7F));


    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        int8_t x = pseudo_rand_int8(&seed);
        int8_t y = pseudo_rand_int8(&seed);

        // final term is because negative ties round differently on the VPU and in floating point
        double exp = round( x * ldexp(y, -6)  +  ldexp(1,-10) );

        exp = MIN(exp, VPU_INT8_MAX);
        exp = MAX(exp, VPU_INT8_MIN);

        int8_t res = vlmul8(x, y);

        TEST_ASSERT_EQUAL_INT8(exp, res);
    }
}


TEST(vpu_scalar_ops_s8, vlmacc8)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    TEST_ASSERT_EQUAL_INT32(             0, vlmacc8(              0,     0,     0) );
    TEST_ASSERT_EQUAL_INT32(           100, vlmacc8(            100,     0,     0) );
    TEST_ASSERT_EQUAL_INT32(          -100, vlmacc8(           -100,     0,     0) );
    TEST_ASSERT_EQUAL_INT32(           100, vlmacc8(            100,     1,     0) );
    TEST_ASSERT_EQUAL_INT32(            94, vlmacc8(            100,     2,    -3) );
    TEST_ASSERT_EQUAL_INT32( VPU_INT32_MIN, vlmacc8(  VPU_INT32_MIN,    -1,     1) );
    TEST_ASSERT_EQUAL_INT32( VPU_INT32_MAX, vlmacc8(  VPU_INT32_MAX,     1,     1) );

    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        int32_t acc = pseudo_rand_int32(&seed);
        int8_t x = pseudo_rand_int8(&seed);
        int8_t y = pseudo_rand_int8(&seed);

        int64_t s = acc;
        s += ((int32_t)x) * y;

        s = MIN(s, VPU_INT32_MAX);
        s = MAX(s, VPU_INT32_MIN);

        int32_t res = vlmacc8(acc, x, y);

        TEST_ASSERT_EQUAL_INT32(s, res);
    }
}


TEST(vpu_scalar_ops_s8, vlmaccr8)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        int32_t acc = pseudo_rand_int32(&seed);
        int8_t x[VPU_INT8_EPV];
        int8_t y[VPU_INT8_EPV];

        int64_t s = acc;

        for(int i = 0; i < VPU_INT8_EPV; i++){
            x[i] = pseudo_rand_int8(&seed);
            y[i] = pseudo_rand_int8(&seed);

            s += ((int32_t)x[i]) * y[i];
        }

        s = MIN(s, VPU_INT32_MAX);
        s = MAX(s, VPU_INT32_MIN);

        int32_t res = vlmaccr8(acc, x, y);

        TEST_ASSERT_EQUAL_INT32(s, res);
    }
}


TEST(vpu_scalar_ops_s8, vlsat8)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    TEST_ASSERT_EQUAL_INT8(            0, vlsat8(            0,      0));
    TEST_ASSERT_EQUAL_INT8(            1, vlsat8(            1,      0));
    TEST_ASSERT_EQUAL_INT8(            1, vlsat8(            2,      1));
    TEST_ASSERT_EQUAL_INT8(            1, vlsat8(            2,      2));
    TEST_ASSERT_EQUAL_INT8(            0, vlsat8(            2,      3));
    TEST_ASSERT_EQUAL_INT8(         0x0A, vlsat8(   0x0A7FFFFF,     24));
    TEST_ASSERT_EQUAL_INT8(         0x0B, vlsat8(   0x0A800000,     24));
    TEST_ASSERT_EQUAL_INT8(        -0x0A, vlsat8(  -0x0A800000,     24));
    TEST_ASSERT_EQUAL_INT8(        -0x0B, vlsat8(  -0x0A800001,     24));


    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        headroom_t hr = pseudo_rand_uint(&seed, 0, 32);
        int32_t acc = pseudo_rand_int32(&seed) >> hr;

        int8_t shr = pseudo_rand_int(&seed, 23 - hr, 25 - hr);
        if(shr < 0) shr = 0;


        double fexp = ldexp(acc, 0);
        // final term is because negative ties round differently on the VPU and in floating point
        fexp = round( fexp * ldexp(1, -shr) + ldexp(1,-30) );
        fexp = MIN(fexp, VPU_INT8_MAX);
        fexp = MAX(fexp, VPU_INT8_MIN);

        int8_t exp = (int8_t) fexp;

        int8_t res = vlsat8(acc, shr);

        // if(exp != res){
        //     printf("acc = %ld\n", acc);
        //     printf("shr = %d\n", shr);
        //     printf("exp = %d\n", exp);
        //     printf("res = %d\n", res);
        // }

        TEST_ASSERT_EQUAL_INT8(exp, res);
    }
}

