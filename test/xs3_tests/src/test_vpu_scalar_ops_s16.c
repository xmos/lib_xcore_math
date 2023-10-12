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

TEST_GROUP_RUNNER(vpu_scalar_ops_s16) {
  RUN_TEST_CASE(vpu_scalar_ops_s16, vladd16);
  RUN_TEST_CASE(vpu_scalar_ops_s16, vlsub16);
  RUN_TEST_CASE(vpu_scalar_ops_s16, vlashr16);
  RUN_TEST_CASE(vpu_scalar_ops_s16, vpos16);
  RUN_TEST_CASE(vpu_scalar_ops_s16, vsign16);
  RUN_TEST_CASE(vpu_scalar_ops_s16, vdepth1_16);
  RUN_TEST_CASE(vpu_scalar_ops_s16, vdepth8_16);
  RUN_TEST_CASE(vpu_scalar_ops_s16, vlmul16);
  RUN_TEST_CASE(vpu_scalar_ops_s16, vlmacc16);
  RUN_TEST_CASE(vpu_scalar_ops_s16, vlmaccr16);
  RUN_TEST_CASE(vpu_scalar_ops_s16, vlsat16);
  RUN_TEST_CASE(vpu_scalar_ops_s16, vadddr16);
}

TEST_GROUP(vpu_scalar_ops_s16);
TEST_SETUP(vpu_scalar_ops_s16) { fflush(stdout); }
TEST_TEAR_DOWN(vpu_scalar_ops_s16) {}


#if SMOKE_TEST
#  define REPS       (100)
#else
#  define REPS       (1000)
#endif

const int STEP_A = (SMOKE_TEST)? 137 : 13;
const int STEP_B = (SMOKE_TEST)? 173 : 17;

TEST(vpu_scalar_ops_s16, vladd16)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    TEST_ASSERT_EQUAL_INT16(              0, vladd16(       0,      0));
    TEST_ASSERT_EQUAL_INT16(              1, vladd16(       1,      0));
    TEST_ASSERT_EQUAL_INT16(             -1, vladd16(       0,     -1));
    TEST_ASSERT_EQUAL_INT16(              0, vladd16(       1,     -1));
    TEST_ASSERT_EQUAL_INT16(           1246, vladd16(    1234,     12));
    TEST_ASSERT_EQUAL_INT16(         -22940, vladd16(      60, -23000));
    TEST_ASSERT_EQUAL_INT16(  VPU_INT16_MAX, vladd16(  0x7F00,  0x100));
    TEST_ASSERT_EQUAL_INT16(  VPU_INT16_MIN, vladd16( -0x7F00, -0x100));


    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        int16_t x = pseudo_rand_int16(&seed);
        int16_t y = pseudo_rand_int16(&seed);

        int32_t exp = x + y;
        exp = MIN(exp, VPU_INT16_MAX);
        exp = MAX(exp, VPU_INT16_MIN);

        int16_t res = vladd16(x, y);

        TEST_ASSERT_EQUAL_INT16(exp, res);
    }
}


TEST(vpu_scalar_ops_s16, vlsub16)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    TEST_ASSERT_EQUAL_INT16(              0, vlsub16(       0,      0));
    TEST_ASSERT_EQUAL_INT16(              1, vlsub16(       1,      0));
    TEST_ASSERT_EQUAL_INT16(              1, vlsub16(       0,     -1));
    TEST_ASSERT_EQUAL_INT16(              2, vlsub16(       1,     -1));
    TEST_ASSERT_EQUAL_INT16(           1222, vlsub16(    1234,     12));
    TEST_ASSERT_EQUAL_INT16(          23060, vlsub16(      60, -23000));
    TEST_ASSERT_EQUAL_INT16(  VPU_INT16_MAX, vlsub16(  0x7F00, -0x100));
    TEST_ASSERT_EQUAL_INT16(  VPU_INT16_MIN, vlsub16( -0x7F00,  0x100));


    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        int16_t x = pseudo_rand_int16(&seed);
        int16_t y = pseudo_rand_int16(&seed);

        int32_t exp = x - y;
        exp = MIN(exp, VPU_INT16_MAX);
        exp = MAX(exp, VPU_INT16_MIN);

        int16_t res = vlsub16(x, y);

        TEST_ASSERT_EQUAL_INT16(exp, res);
    }
}


TEST(vpu_scalar_ops_s16, vlashr16)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

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
    TEST_ASSERT_EQUAL_INT16(            -1, vlashr16(    -8,      3));
    TEST_ASSERT_EQUAL_INT16(            -1, vlashr16(    -8,      4)); // Note[0]
    
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
    TEST_ASSERT_EQUAL_INT16(             0, vlashr16(  9698,      38));

    // Notes:
    // [0] - This is different from vlsat16(-8, 4), which results in 0


    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        int16_t x = pseudo_rand_int16(&seed);
        right_shift_t shr = pseudo_rand_int(&seed, -18, 18);


        double fexp = floor(ldexp(x, -shr));
        fexp = MIN(fexp, VPU_INT16_MAX);
        fexp = MAX(fexp, VPU_INT16_MIN);

        int16_t exp = (int16_t) fexp;

        int16_t res = vlashr16(x, shr);
    
        TEST_ASSERT_EQUAL_INT16(exp, res);
    }
}


TEST(vpu_scalar_ops_s16, vpos16)
{
  for(int k = 0; k < INT16_MAX; k+=STEP_A)
      TEST_ASSERT_EQUAL_INT16( k, vpos16( (int16_t) k));
  
  for(int k = INT16_MIN; k < 0; k+=STEP_B)
      TEST_ASSERT_EQUAL_INT16( 0, vpos16( (int16_t) k));
}


TEST(vpu_scalar_ops_s16, vsign16)
{
  for(int k = 0; k < INT16_MAX; k+=STEP_A)
      TEST_ASSERT_EQUAL_INT16(  ((int16_t)  0x4000), vsign16( (int16_t) k)  );
  
  for(int k = INT16_MIN; k < 0; k+=STEP_B)
      TEST_ASSERT_EQUAL_INT16(  ((int16_t) -0x4000), vsign16( (int16_t) k)  );
}


TEST(vpu_scalar_ops_s16, vdepth1_16)
{
  for(int k = 0; k < INT16_MAX; k+=STEP_A)
      TEST_ASSERT_EQUAL_INT( 0, vdepth1_16( (int16_t) k));
  
  for(int k = INT16_MIN; k < 0; k+=STEP_B)
      TEST_ASSERT_EQUAL_INT( 1, vdepth1_16( (int16_t) k));
}


TEST(vpu_scalar_ops_s16, vdepth8_16)
{
    

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

    for(int k = INT16_MIN; k < INT16_MAX; k += STEP_A)
    {
        int8_t res = vdepth8_16( (int16_t) k );

        int32_t exp = lround(ldexp( k, -8) + ldexp(1, -30));
        exp = MIN(exp, VPU_INT8_MAX);
        exp = MAX(exp, VPU_INT8_MIN);

        TEST_ASSERT_EQUAL_INT8( exp, res );
    }

}


TEST(vpu_scalar_ops_s16, vlmul16)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


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


    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

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


TEST(vpu_scalar_ops_s16, vlmacc16)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    TEST_ASSERT_EQUAL_INT32(             0, vlmacc16(              0,     0,     0) );
    TEST_ASSERT_EQUAL_INT32(           100, vlmacc16(            100,     0,     0) );
    TEST_ASSERT_EQUAL_INT32(          -100, vlmacc16(           -100,     0,     0) );
    TEST_ASSERT_EQUAL_INT32(           100, vlmacc16(            100,     1,     0) );
    TEST_ASSERT_EQUAL_INT32(            94, vlmacc16(            100,     2,    -3) );
    TEST_ASSERT_EQUAL_INT32( VPU_INT32_MIN, vlmacc16(  VPU_INT32_MIN,    -1,     1) );
    TEST_ASSERT_EQUAL_INT32( VPU_INT32_MAX, vlmacc16(  VPU_INT32_MAX,     1,     1) );

    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

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


TEST(vpu_scalar_ops_s16, vlmaccr16)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

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


TEST(vpu_scalar_ops_s16, vlsat16)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    TEST_ASSERT_EQUAL_INT16(            0, vlsat16(            0,      0));
    TEST_ASSERT_EQUAL_INT16(            1, vlsat16(            1,      0));
    TEST_ASSERT_EQUAL_INT16(            1, vlsat16(            2,      1));
    TEST_ASSERT_EQUAL_INT16(            1, vlsat16(            2,      2));
    TEST_ASSERT_EQUAL_INT16(            0, vlsat16(            2,      3));
    TEST_ASSERT_EQUAL_INT16(         0x0A, vlsat16(   0x0A7FFFFF,     24));
    TEST_ASSERT_EQUAL_INT16(         0x0B, vlsat16(   0x0A800000,     24));
    TEST_ASSERT_EQUAL_INT16(        -0x0A, vlsat16(  -0x0A800000,     24));
    TEST_ASSERT_EQUAL_INT16(        -0x0B, vlsat16(  -0x0A800001,     24));
    TEST_ASSERT_EQUAL_INT16(           -1, vlsat16(           -8,      3));
    TEST_ASSERT_EQUAL_INT16(            0, vlsat16(           -8,      4)); // Note[0]

    // Notes:
    // [0] - This is different than vlashr(-8,4) which results in -1


    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

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

        int16_t exp = (int16_t) fexp;

        int16_t res = vlsat16(acc, shr);

        // printf("acc = %ld\n", acc);
        // printf("shr = %d\n", shr);
        // printf("exp = %d\n", exp);
        // printf("res = %d\n", res);

        TEST_ASSERT_EQUAL_INT16(exp, res);
    }
}


TEST(vpu_scalar_ops_s16, vadddr16)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        vpu_int16_acc_t acc[VPU_INT16_ACC_PERIOD];

        int64_t total = 0;

        for(int i = 0; i < VPU_INT16_ACC_PERIOD; i++){
            acc[i] = i;//pseudo_rand_int16(&seed);
            total += acc[i];
        }


        vpu_int16_acc_t expected =
            (total > VPU_INT32_MAX)? VPU_INT32_MAX
          : (total < VPU_INT32_MIN)? VPU_INT32_MIN
          : (vpu_int16_acc_t) total;


        vpu_int16_acc_t result = vadddr16(acc);

        TEST_ASSERT_EQUAL_INT32(expected, result);
    }
}

