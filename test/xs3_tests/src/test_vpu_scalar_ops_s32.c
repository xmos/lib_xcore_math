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

TEST_GROUP_RUNNER(vpu_scalar_ops_s32) {
  RUN_TEST_CASE(vpu_scalar_ops_s32, vladd32);
  RUN_TEST_CASE(vpu_scalar_ops_s32, vlsub32);
  RUN_TEST_CASE(vpu_scalar_ops_s32, vlashr32);
  RUN_TEST_CASE(vpu_scalar_ops_s32, vpos32);
  RUN_TEST_CASE(vpu_scalar_ops_s32, vsign32);
  RUN_TEST_CASE(vpu_scalar_ops_s32, vdepth1_32);
  RUN_TEST_CASE(vpu_scalar_ops_s32, vdepth8_32);
  RUN_TEST_CASE(vpu_scalar_ops_s32, vdepth16_32);
  RUN_TEST_CASE(vpu_scalar_ops_s32, vlmul32);
  RUN_TEST_CASE(vpu_scalar_ops_s32, vlmacc32);
  RUN_TEST_CASE(vpu_scalar_ops_s32, vlmaccr32);
  RUN_TEST_CASE(vpu_scalar_ops_s32, vlsat32);
  RUN_TEST_CASE(vpu_scalar_ops_s32, vcmr32);
  RUN_TEST_CASE(vpu_scalar_ops_s32, vcmi32);
  RUN_TEST_CASE(vpu_scalar_ops_s32, vcmcr32);
  RUN_TEST_CASE(vpu_scalar_ops_s32, vcmci32);
}

TEST_GROUP(vpu_scalar_ops_s32);
TEST_SETUP(vpu_scalar_ops_s32) { fflush(stdout); }
TEST_TEAR_DOWN(vpu_scalar_ops_s32) {}


#if SMOKE_TEST
#  define REPS       (100)
#else
#  define REPS       (1000)
#endif

const int STEP_DIV = (SMOKE_TEST)? 101 : 10001;


static int64_t mul_Q30(int32_t x, int32_t y)
{
  return ((((int64_t)x) * y) + (1LL << 29)) >> 30;
}





TEST(vpu_scalar_ops_s32, vladd32)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    TEST_ASSERT_EQUAL_INT32(              0, vladd32(       0,      0));
    TEST_ASSERT_EQUAL_INT32(              1, vladd32(       1,      0));
    TEST_ASSERT_EQUAL_INT32(             -1, vladd32(       0,     -1));
    TEST_ASSERT_EQUAL_INT32(              0, vladd32(       1,     -1));
    TEST_ASSERT_EQUAL_INT32(           1246, vladd32(    1234,     12));
    TEST_ASSERT_EQUAL_INT32(         -22940, vladd32(      60, -23000));
    TEST_ASSERT_EQUAL_INT32(  VPU_INT32_MAX, vladd32(  0x7F000000,  0x1000000));
    TEST_ASSERT_EQUAL_INT32(  VPU_INT32_MIN, vladd32( -0x7F000000, -0x1000000));


    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        int32_t x = pseudo_rand_int32(&seed);
        int32_t y = pseudo_rand_int32(&seed);

        int64_t exp = ((int64_t)x) + y;
        exp = MIN(exp, VPU_INT32_MAX);
        exp = MAX(exp, VPU_INT32_MIN);

        int32_t res = vladd32(x, y);

        if(exp != res){
            printf("x = %ld\n", (long int) x);
            printf("y = %ld\n", (long int) y);
            printf("exp = %lld\n", (long long int) exp);
            printf("res = %ld\n", (long int) res);
        }

        TEST_ASSERT_EQUAL_INT32((int32_t)exp, res);
    }
}


TEST(vpu_scalar_ops_s32, vlsub32)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    TEST_ASSERT_EQUAL_INT32(              0, vlsub32(       0,      0));
    TEST_ASSERT_EQUAL_INT32(              1, vlsub32(       1,      0));
    TEST_ASSERT_EQUAL_INT32(              1, vlsub32(       0,     -1));
    TEST_ASSERT_EQUAL_INT32(              2, vlsub32(       1,     -1));
    TEST_ASSERT_EQUAL_INT32(           1222, vlsub32(    1234,     12));
    TEST_ASSERT_EQUAL_INT32(          23060, vlsub32(      60, -23000));
    TEST_ASSERT_EQUAL_INT32(  VPU_INT32_MAX, vlsub32(  0x7F000000, -0x1000000));
    TEST_ASSERT_EQUAL_INT32(  VPU_INT32_MIN, vlsub32( -0x7F000000,  0x1000000));


    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        int32_t x = pseudo_rand_int32(&seed);
        int32_t y = pseudo_rand_int32(&seed);

        int64_t exp = ((int64_t)x) - y;
        exp = MIN(exp, VPU_INT32_MAX);
        exp = MAX(exp, VPU_INT32_MIN);

        int32_t res = vlsub32(x, y);

        TEST_ASSERT_EQUAL_INT32(exp, res);
    }
}


TEST(vpu_scalar_ops_s32, vlashr32)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    TEST_ASSERT_EQUAL_INT32(             0, vlashr32(     0,      0));
    TEST_ASSERT_EQUAL_INT32(             1, vlashr32(     1,      0));
    TEST_ASSERT_EQUAL_INT32(             2, vlashr32(     2,      0));
    TEST_ASSERT_EQUAL_INT32(             1, vlashr32(     2,      1));
    TEST_ASSERT_EQUAL_INT32(             0, vlashr32(     2,      2));
    TEST_ASSERT_EQUAL_INT32(             0, vlashr32(     2,      3));
    TEST_ASSERT_EQUAL_INT32(             4, vlashr32(     2,     -1));
    
    TEST_ASSERT_EQUAL_INT32(            -2, vlashr32(    -2,      0));
    TEST_ASSERT_EQUAL_INT32(            -1, vlashr32(    -2,      1));
    TEST_ASSERT_EQUAL_INT32(            -1, vlashr32(    -2,      2));
    TEST_ASSERT_EQUAL_INT32(            -1, vlashr32(    -2,      3));
    TEST_ASSERT_EQUAL_INT32(            -4, vlashr32(    -2,     -1));
    
    TEST_ASSERT_EQUAL_INT32(             5, vlashr32(    10,      1));
    TEST_ASSERT_EQUAL_INT32(             2, vlashr32(    10,      2));
    TEST_ASSERT_EQUAL_INT32(             1, vlashr32(    10,      3));
    
    TEST_ASSERT_EQUAL_INT32(        0xFFFFFFFD, vlashr32(0xFFFFFFFA,      1));
    TEST_ASSERT_EQUAL_INT32(        0xFFFFFFFE, vlashr32(0xFFFFFFFA,      2));
    TEST_ASSERT_EQUAL_INT32(        0xFFFFFFFF, vlashr32(0xFFFFFFFA,      3));
    
    TEST_ASSERT_EQUAL_INT32(            64, vlashr32(     1,     -6));
    TEST_ASSERT_EQUAL_INT32(           -64, vlashr32(    -1,     -6));

    TEST_ASSERT_EQUAL_INT32( VPU_INT32_MAX, vlashr32(     1,     -31));
    TEST_ASSERT_EQUAL_INT32( VPU_INT32_MIN, vlashr32(    -1,     -31));


    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        int32_t x = pseudo_rand_int32(&seed);
        right_shift_t shr = pseudo_rand_int(&seed, -18, 18);


        double fexp = floor(ldexp(x, -shr));
        fexp = MIN(fexp, VPU_INT32_MAX);
        fexp = MAX(fexp, VPU_INT32_MIN);

        int32_t exp = (int32_t) fexp;

        int32_t res = vlashr32(x, shr);
    
        TEST_ASSERT_EQUAL_INT32(exp, res);
    }
}


TEST(vpu_scalar_ops_s32, vpos32)
{
    

    for(int64_t k = 0; k < INT32_MAX; k+= (UINT32_MAX/STEP_DIV))
        TEST_ASSERT_EQUAL_INT32( k, vpos32( (int32_t) k));
    
    for(int64_t k = INT32_MIN; k < 0; k+=(UINT32_MAX/STEP_DIV))
        TEST_ASSERT_EQUAL_INT32( 0, vpos32( (int32_t) k));
    
}


TEST(vpu_scalar_ops_s32, vsign32)
{
    


    for(int64_t k = 0; k < INT32_MAX; k+=(UINT32_MAX/STEP_DIV))
        TEST_ASSERT_EQUAL_INT32(  ((int32_t)  0x40000000), vsign32( (int32_t) k)  );
    
    for(int64_t k = INT32_MIN; k < 0; k+=(UINT32_MAX/STEP_DIV))
        TEST_ASSERT_EQUAL_INT32(  ((int32_t) -0x40000000), vsign32( (int32_t) k)  );

}


TEST(vpu_scalar_ops_s32, vdepth1_32)
{
    

    for(int64_t k = 0; k < INT32_MAX; k+=(UINT32_MAX/STEP_DIV))
        TEST_ASSERT_EQUAL_INT( 0, vdepth1_32( (int32_t) k));
    
    for(int64_t k = INT32_MIN; k < 0; k+=(UINT32_MAX/STEP_DIV))
        TEST_ASSERT_EQUAL_INT( 1, vdepth1_32( (int32_t) k));

}


TEST(vpu_scalar_ops_s32, vdepth8_32)
{
    

    TEST_ASSERT_EQUAL_INT8(      0, vdepth8_32(            0));
    TEST_ASSERT_EQUAL_INT8(      0, vdepth8_32(       0x7FFFFF));
    TEST_ASSERT_EQUAL_INT8(      1, vdepth8_32(       0x800000));
    TEST_ASSERT_EQUAL_INT8(      2, vdepth8_32(      0x2000000));
    TEST_ASSERT_EQUAL_INT8(   0x7F, vdepth8_32(     0x7FFFFFFF));
    TEST_ASSERT_EQUAL_INT8(      0, vdepth8_32(             -1));
    TEST_ASSERT_EQUAL_INT8(      0, vdepth8_32(      -0x800000));
    TEST_ASSERT_EQUAL_INT8(     -1, vdepth8_32(      -0x810000));
    TEST_ASSERT_EQUAL_INT8(     -2, vdepth8_32(    -0x02800000));
    TEST_ASSERT_EQUAL_INT8(     -3, vdepth8_32(    -0x02810000));
    TEST_ASSERT_EQUAL_INT8(  -0x7F, vdepth8_32(    -0x7FFFFFFF));

    for(int64_t k = INT32_MIN; k < INT32_MAX; k += (UINT32_MAX/STEP_DIV))
    {

        int8_t res = vdepth8_32( (int32_t) k );

        int32_t exp = lround(ldexp( (double) k, -24) + ldexp(1, -40));
        exp = MIN(exp, VPU_INT8_MAX);
        exp = MAX(exp, VPU_INT8_MIN);

        TEST_ASSERT_EQUAL_INT8( exp, res );

    }

}


TEST(vpu_scalar_ops_s32, vdepth16_32)
{
    

    TEST_ASSERT_EQUAL_INT8(        0, vdepth16_32(            0));
    TEST_ASSERT_EQUAL_INT8(        0, vdepth16_32(       0x7FFF));
    TEST_ASSERT_EQUAL_INT8(        1, vdepth16_32(       0x8000));
    TEST_ASSERT_EQUAL_INT8(        2, vdepth16_32(      0x20000));
    TEST_ASSERT_EQUAL_INT8(   0x7FFF, vdepth16_32(   0x7FFFFFFF));
    TEST_ASSERT_EQUAL_INT8(        0, vdepth16_32(           -1));
    TEST_ASSERT_EQUAL_INT8(        0, vdepth16_32(      -0x8000));
    TEST_ASSERT_EQUAL_INT8(       -1, vdepth16_32(      -0x8100));
    TEST_ASSERT_EQUAL_INT8(       -2, vdepth16_32(    -0x028000));
    TEST_ASSERT_EQUAL_INT8(       -3, vdepth16_32(    -0x028100));
    TEST_ASSERT_EQUAL_INT8(  -0x7FFF, vdepth16_32(  -0x7FFFFFFF));

    for(int64_t k = INT32_MIN; k < INT32_MAX; k += (UINT32_MAX/STEP_DIV))
    {

        int8_t res = (int8_t) vdepth16_32( (int32_t) k );

        int32_t exp = lround(ldexp( (double) k, -16) + ldexp(1, -40));
        exp = MIN(exp, VPU_INT16_MAX);
        exp = MAX(exp, VPU_INT16_MIN);

        TEST_ASSERT_EQUAL_INT8( exp, res );

    }

}


TEST(vpu_scalar_ops_s32, vlmul32)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    TEST_ASSERT_EQUAL_INT32(             0, vlmul32(                       0,            0));
    TEST_ASSERT_EQUAL_INT32(             1, vlmul32(                       1,   0x40000000));
    TEST_ASSERT_EQUAL_INT32(            -1, vlmul32(                       1,  -0x40000000));
    TEST_ASSERT_EQUAL_INT32(           123, vlmul32(                     123,   0x40000000));
    TEST_ASSERT_EQUAL_INT32(          -123, vlmul32(                     123,  -0x40000000));
    TEST_ASSERT_EQUAL_INT32(        -12322, vlmul32(                   24644,  -0x20000000));
    TEST_ASSERT_EQUAL_INT32(         12322, vlmul32(                   24644,   0x20000000));
    TEST_ASSERT_EQUAL_INT32(         12323, vlmul32(                   24645,   0x20000000));
    TEST_ASSERT_EQUAL_INT32( VPU_INT32_MIN, vlmul32(    (int) (0-0x80000000),   0x40000000));
    TEST_ASSERT_EQUAL_INT32( VPU_INT32_MAX, vlmul32(             -0x7FFFFFFF,  -0x7FFFFFFF));


    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        int32_t x = pseudo_rand_int32(&seed);
        int32_t y = pseudo_rand_int32(&seed);

        // final term is because negative ties round differently on the VPU and in floating point
        double fexp = round( x * ldexp(y, -30)  +  ldexp(1,-40) );
        fexp = MIN(fexp, VPU_INT32_MAX);
        fexp = MAX(fexp, VPU_INT32_MIN);

        int32_t exp = (int32_t) fexp;

        int32_t res = vlmul32(x, y);

        TEST_ASSERT_EQUAL_INT32(exp, res);
    }
}


TEST(vpu_scalar_ops_s32, vlmacc32)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    TEST_ASSERT_EQUAL_INT32(             0, vlmacc32(           0,     0,   0 * 0x40000000) );
    TEST_ASSERT_EQUAL_INT32(           100, vlmacc32(         100,     0,   0 * 0x40000000) );
    TEST_ASSERT_EQUAL_INT32(          -100, vlmacc32(        -100,     0,   0 * 0x40000000) );
    TEST_ASSERT_EQUAL_INT32(           100, vlmacc32(         100,     1,   0 * 0x40000000) );
    TEST_ASSERT_EQUAL_INT32(            94, vlmacc32(         100,     3,  -2 * 0x40000000) );
    TEST_ASSERT_EQUAL_INT32( VPU_INT40_MIN, vlmacc32( -0x8000000000,    -1,       0x40000000) );
    TEST_ASSERT_EQUAL_INT32( VPU_INT40_MAX, vlmacc32(  0x7FFFFFFFFF,     1,       0x40000000) );

    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        vpu_int32_acc_t acc = pseudo_rand_int64(&seed) >> 24;
        int32_t x = pseudo_rand_int32(&seed);
        int32_t y = pseudo_rand_int32(&seed);

        double fexp = (double) acc;

        fexp += round( x * ldexp(y, -30) + ldexp(1,-41) );

        fexp = MIN(fexp, VPU_INT40_MAX);
        fexp = MAX(fexp, VPU_INT40_MIN);

        vpu_int32_acc_t exp = (vpu_int32_acc_t) fexp;

        vpu_int32_acc_t res = vlmacc32(acc, x, y);

        if(res != exp){
            printf("acc = %lld     (0x%010llX)\n", (long long int) acc,   (long long unsigned int) acc);
            printf("x = %ld     (0x%08lX)\n",  (long int) x,   (long unsigned int) x);
            printf("y = %ld     (0x%08lX)\n",  (long int) y,   (long unsigned int) y);
            printf("exp = %lld     (0x%010llX)\n",  (long long int) exp,   (long long unsigned int) exp);
            printf("res = %lld     (0x%010llX)\n",  (long long int) res,   (long long unsigned int) res);
        }

        TEST_ASSERT_EQUAL(exp, res);
    }
}


TEST(vpu_scalar_ops_s32, vlmaccr32)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();

    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        vpu_int32_acc_t acc = pseudo_rand_int64(&seed) >> 24;
        int32_t x[VPU_INT32_EPV];
        int32_t y[VPU_INT32_EPV];

        int64_t s = acc;

        for(int i = 0; i < VPU_INT32_EPV; i++){
            x[i] = pseudo_rand_int32(&seed);
            y[i] = pseudo_rand_int32(&seed);

            s += mul_Q30(x[i], y[i]);

        }

        s = MIN(s, VPU_INT40_MAX);
        s = MAX(s, VPU_INT40_MIN);

        vpu_int32_acc_t res = vlmaccr32(acc, x, y);

        TEST_ASSERT_EQUAL(s, res);
    }
}


TEST(vpu_scalar_ops_s32, vlsat32)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    TEST_ASSERT_EQUAL_INT32(            0, vlsat32(            0,      0));
    TEST_ASSERT_EQUAL_INT32(            1, vlsat32(            1,      0));
    TEST_ASSERT_EQUAL_INT32(            1, vlsat32(            2,      1));
    TEST_ASSERT_EQUAL_INT32(            1, vlsat32(            2,      2));
    TEST_ASSERT_EQUAL_INT32(            0, vlsat32(            2,      3));
    TEST_ASSERT_EQUAL_INT32(         0x0A, vlsat32(   0x0A7FFFFF,     24));
    TEST_ASSERT_EQUAL_INT32(         0x0B, vlsat32(   0x0A800000,     24));
    TEST_ASSERT_EQUAL_INT32(        -0x0A, vlsat32(  -0x0A800000,     24));
    TEST_ASSERT_EQUAL_INT32(        -0x0B, vlsat32(  -0x0A800001,     24));


    for(int v = 0; v < REPS/40; v++){
            setExtraInfo_RS(v, seed);

            for(headroom_t hr = 0; hr < 40; hr++){

                int32_t acc = (int32_t) ((pseudo_rand_int64(&seed) >> 24) >> hr);

                int32_t shr = pseudo_rand_int(&seed, 31 - hr, 33 - hr);

                if(shr < 0)
                    shr = 0;


                double fexp = ldexp(acc, 0);
                
                // final term is because negative ties round differently on the VPU and in floating point
                fexp = round( fexp * ldexp(1, -shr) + ldexp(1,-39) );

                fexp = MIN(fexp, VPU_INT32_MAX);
                fexp = MAX(fexp, VPU_INT32_MIN);

                int32_t exp = (int32_t) fexp;

                int32_t res = vlsat32(acc, shr);

                // printf("acc = %ld\n", acc);
                // printf("shr = %d\n", shr);
                // printf("exp = %d\n", exp);
                // printf("res = %d\n", res);

                TEST_ASSERT_EQUAL_INT32(exp, res);
        }
    }
}


TEST(vpu_scalar_ops_s32, vcmr32)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        complex_s32_t x;
        complex_s32_t y;

        x.re = pseudo_rand_int32(&seed);
        x.im = pseudo_rand_int32(&seed);
        y.re = pseudo_rand_int32(&seed);
        y.im = pseudo_rand_int32(&seed);

        int64_t a = mul_Q30(x.re, y.re);
        int64_t b = mul_Q30(x.im, y.im);

        int64_t exp64 = a - b;
        exp64 = MIN(exp64, VPU_INT32_MAX);
        exp64 = MAX(exp64, VPU_INT32_MIN);

        int32_t exp = (int32_t) exp64;

        int32_t res = vcmr32(x, y);

        TEST_ASSERT_EQUAL_INT32(exp, res);
    }
}


TEST(vpu_scalar_ops_s32, vcmi32)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        complex_s32_t x;
        complex_s32_t y;

        x.re = pseudo_rand_int32(&seed);
        x.im = pseudo_rand_int32(&seed);
        y.re = pseudo_rand_int32(&seed);
        y.im = pseudo_rand_int32(&seed);

        int64_t a = mul_Q30(x.re, y.im);
        int64_t b = mul_Q30(x.im, y.re);

        int64_t exp64 = a + b;
        exp64 = MIN(exp64, VPU_INT32_MAX);
        exp64 = MAX(exp64, VPU_INT32_MIN);

        int32_t exp = (int32_t) exp64;

        int32_t res = vcmi32(x, y);

        TEST_ASSERT_EQUAL_INT32(exp, res);
    }
}


TEST(vpu_scalar_ops_s32, vcmcr32)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        complex_s32_t x;
        complex_s32_t y;

        x.re = pseudo_rand_int32(&seed);
        x.im = pseudo_rand_int32(&seed);
        y.re = pseudo_rand_int32(&seed);
        y.im = pseudo_rand_int32(&seed);

        int64_t a = mul_Q30(x.re, y.re);
        int64_t b = mul_Q30(x.im, y.im);

        int64_t exp64 = a + b;
        exp64 = MIN(exp64, VPU_INT32_MAX);
        exp64 = MAX(exp64, VPU_INT32_MIN);

        int32_t exp = (int32_t) exp64;

        int32_t res = vcmcr32(x, y);

        TEST_ASSERT_EQUAL_INT32(exp, res);
    }
}


TEST(vpu_scalar_ops_s32, vcmci32)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    for(unsigned int v = 0; v < REPS; v++){
        setExtraInfo_RS(v, seed);

        complex_s32_t x;
        complex_s32_t y;

        x.re = pseudo_rand_int32(&seed);
        x.im = pseudo_rand_int32(&seed);
        y.re = pseudo_rand_int32(&seed);
        y.im = pseudo_rand_int32(&seed);

        int64_t a = mul_Q30(x.re, y.im);
        int64_t b = mul_Q30(x.im, y.re);

        int64_t exp64 = b - a;
        exp64 = MIN(exp64, VPU_INT32_MAX);
        exp64 = MAX(exp64, VPU_INT32_MIN);

        int32_t exp = (int32_t) exp64;

        int32_t res = vcmci32(x, y);

        TEST_ASSERT_EQUAL_INT32(exp, res);
    }
}

