
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "xs3_math.h"
#include "xs3_vpu_scalar_ops.h"
#include "xs3_vpu.h"

#include "../../tst_common.h"

#include "unity.h"

static unsigned seed = 2314567;

#if DEBUG_ON || 0
#undef DEBUG_ON
#define DEBUG_ON    (1)
#endif

#define REPS        (1000)




static void test_vladd32()
{
    PRINTF("%s...\n", __func__);
    seed = 12345;

    TEST_ASSERT_EQUAL_INT32(              0, vladd32(       0,      0));
    TEST_ASSERT_EQUAL_INT32(              1, vladd32(       1,      0));
    TEST_ASSERT_EQUAL_INT32(             -1, vladd32(       0,     -1));
    TEST_ASSERT_EQUAL_INT32(              0, vladd32(       1,     -1));
    TEST_ASSERT_EQUAL_INT32(           1246, vladd32(    1234,     12));
    TEST_ASSERT_EQUAL_INT32(         -22940, vladd32(      60, -23000));
    TEST_ASSERT_EQUAL_INT32(  VPU_INT32_MAX, vladd32(  0x7F000000,  0x1000000));
    TEST_ASSERT_EQUAL_INT32(  VPU_INT32_MIN, vladd32( -0x7F000000, -0x1000000));


    for(int v = 0; v < REPS; v++){
        PRINTF("\trepetition %d.. (seed: 0x%08X)\n", v, seed);

        int32_t x = pseudo_rand_int32(&seed);
        int32_t y = pseudo_rand_int32(&seed);

        int64_t exp = ((int64_t)x) + y;
        exp = MIN(exp, VPU_INT32_MAX);
        exp = MAX(exp, VPU_INT32_MIN);

        int32_t res = vladd32(x, y);

        if(exp != res){
            PRINTF("x = %ld\n", x);
            PRINTF("y = %ld\n", y);
            PRINTF("exp = %lld\n", exp);
            PRINTF("res = %ld\n", res);
        }

        TEST_ASSERT_EQUAL_INT32((int32_t)exp, res);
    }
}


static void test_vlsub32()
{
    PRINTF("%s...\n", __func__);
    seed = 12345;

    TEST_ASSERT_EQUAL_INT32(              0, vlsub32(       0,      0));
    TEST_ASSERT_EQUAL_INT32(              1, vlsub32(       1,      0));
    TEST_ASSERT_EQUAL_INT32(              1, vlsub32(       0,     -1));
    TEST_ASSERT_EQUAL_INT32(              2, vlsub32(       1,     -1));
    TEST_ASSERT_EQUAL_INT32(           1222, vlsub32(    1234,     12));
    TEST_ASSERT_EQUAL_INT32(          23060, vlsub32(      60, -23000));
    TEST_ASSERT_EQUAL_INT32(  VPU_INT32_MAX, vlsub32(  0x7F000000, -0x1000000));
    TEST_ASSERT_EQUAL_INT32(  VPU_INT32_MIN, vlsub32( -0x7F000000,  0x1000000));


    for(int v = 0; v < REPS; v++){
        PRINTF("\trepetition %d.. (seed: 0x%08X)\n", v, seed);

        int32_t x = pseudo_rand_int32(&seed);
        int32_t y = pseudo_rand_int32(&seed);

        int64_t exp = ((int64_t)x) - y;
        exp = MIN(exp, VPU_INT32_MAX);
        exp = MAX(exp, VPU_INT32_MIN);

        int32_t res = vlsub32(x, y);

        TEST_ASSERT_EQUAL_INT32(exp, res);
    }
}


static void test_vlashr32()
{
    PRINTF("%s...\n", __func__);
    seed = 12345;

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


    for(int v = 0; v < REPS; v++){
        PRINTF("\trepetition %d.. (seed: 0x%08X)\n", v, seed);

        int32_t x = pseudo_rand_int32(&seed);
        right_shift_t shr = pseudo_rand_int(&seed, -18, 18);


        double fexp = floor(ldexp(x, -shr));
        fexp = MIN(fexp, VPU_INT32_MAX);
        fexp = MAX(fexp, VPU_INT32_MIN);

        int32_t exp = fexp;

        int32_t res = vlashr32(x, shr);
    
        TEST_ASSERT_EQUAL_INT32(exp, res);
    }
}


static void test_vpos32()
{
    PRINTF("%s...\n", __func__);

    for(int64_t k = 0; k < INT32_MAX; k+= (UINT32_MAX/10001))
        TEST_ASSERT_EQUAL_INT32( k, vpos32( (int32_t) k));
    
    for(int64_t k = INT32_MIN; k < 0; k+=(UINT32_MAX/10001))
        TEST_ASSERT_EQUAL_INT32( 0, vpos32( (int32_t) k));
    
}


static void test_vsign32()
{
    PRINTF("%s...\n", __func__);


    for(int64_t k = 0; k < INT32_MAX; k+=(UINT32_MAX/10001))
        TEST_ASSERT_EQUAL_INT32(  ((int32_t)  0x40000000), vsign32( (int32_t) k)  );
    
    for(int64_t k = INT32_MIN; k < 0; k+=(UINT32_MAX/10001))
        TEST_ASSERT_EQUAL_INT32(  ((int32_t) -0x40000000), vsign32( (int32_t) k)  );

}


static void test_vdepth1_32()
{
    PRINTF("%s...\n", __func__);

    for(int64_t k = 0; k < INT32_MAX; k+=(UINT32_MAX/10001))
        TEST_ASSERT_EQUAL_INT( 0, vdepth1_32( (int32_t) k));
    
    for(int64_t k = INT32_MIN; k < 0; k+=(UINT32_MAX/10001))
        TEST_ASSERT_EQUAL_INT( 1, vdepth1_32( (int32_t) k));

}


static void test_vdepth8_32()
{
    PRINTF("%s...\n", __func__);

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

    for(int64_t k = INT32_MIN; k < INT32_MAX; k += (UINT32_MAX/10001))
    {

        int8_t res = vdepth8_32( (int32_t) k );

        int32_t exp = round(ldexp( k, -24) + ldexp(1, -40));
        exp = MIN(exp, VPU_INT8_MAX);
        exp = MAX(exp, VPU_INT8_MIN);

        TEST_ASSERT_EQUAL_INT8( exp, res );

    }

}


static void test_vdepth16_32()
{
    PRINTF("%s...\n", __func__);

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

    for(int64_t k = INT32_MIN; k < INT32_MAX; k += (UINT32_MAX/10001))
    {

        int8_t res = vdepth16_32( (int32_t) k );

        int32_t exp = round(ldexp( k, -16) + ldexp(1, -40));
        exp = MIN(exp, VPU_INT16_MAX);
        exp = MAX(exp, VPU_INT16_MIN);

        TEST_ASSERT_EQUAL_INT8( exp, res );

    }

}


static void test_vlmul32()
{
    PRINTF("%s...\n", __func__);
    seed = 34563;

    TEST_ASSERT_EQUAL_INT32(             0, vlmul32(           0,            0));
    TEST_ASSERT_EQUAL_INT32(             1, vlmul32(           1,   0x40000000));
    TEST_ASSERT_EQUAL_INT32(            -1, vlmul32(           1,  -0x40000000));
    TEST_ASSERT_EQUAL_INT32(           123, vlmul32(         123,   0x40000000));
    TEST_ASSERT_EQUAL_INT32(          -123, vlmul32(         123,  -0x40000000));
    TEST_ASSERT_EQUAL_INT32(        -12322, vlmul32(       24644,  -0x20000000));
    TEST_ASSERT_EQUAL_INT32(         12322, vlmul32(       24644,   0x20000000));
    TEST_ASSERT_EQUAL_INT32(         12323, vlmul32(       24645,   0x20000000));
    TEST_ASSERT_EQUAL_INT32( VPU_INT32_MIN, vlmul32( -0x80000000,   0x40000000));
    TEST_ASSERT_EQUAL_INT32( VPU_INT32_MAX, vlmul32( -0x7FFFFFFF,  -0x7FFFFFFF));


    for(int v = 0; v < REPS; v++){
        PRINTF("\trepetition %d.. (seed: 0x%08X)\n", v, seed);

        int32_t x = pseudo_rand_int32(&seed);
        int32_t y = pseudo_rand_int32(&seed);

        // final term is because negative ties round differently on the VPU and in floating point
        double fexp = round( x * ldexp(y, -30)  +  ldexp(1,-40) );
        fexp = MIN(fexp, VPU_INT32_MAX);
        fexp = MAX(fexp, VPU_INT32_MIN);

        int32_t exp = fexp;

        int32_t res = vlmul32(x, y);

        TEST_ASSERT_EQUAL_INT32(exp, res);
    }
}



static void test_vlmacc32()
{
    PRINTF("%s...\n", __func__);
    seed = 778786;

    TEST_ASSERT_EQUAL_INT32(             0, vlmacc32(           0,     0,   0 * 0x40000000) ); 
    TEST_ASSERT_EQUAL_INT32(           100, vlmacc32(         100,     0,   0 * 0x40000000) ); 
    TEST_ASSERT_EQUAL_INT32(          -100, vlmacc32(        -100,     0,   0 * 0x40000000) ); 
    TEST_ASSERT_EQUAL_INT32(           100, vlmacc32(         100,     1,   0 * 0x40000000) ); 
    TEST_ASSERT_EQUAL_INT32(            94, vlmacc32(         100,     3,  -2 * 0x40000000) ); 
    TEST_ASSERT_EQUAL_INT32( VPU_INT40_MIN, vlmacc32( -0x8000000000,    -1,       0x40000000) ); 
    TEST_ASSERT_EQUAL_INT32( VPU_INT40_MAX, vlmacc32(  0x7FFFFFFFFF,     1,       0x40000000) ); 

    for(int v = 0; v < REPS; v++){
        PRINTF("\trepetition %d.. (seed: 0x%08X)\n", v, seed);

        vpu_int32_acc_t acc = pseudo_rand_int64(&seed) >> 24;
        int32_t x = pseudo_rand_int32(&seed);
        int32_t y = pseudo_rand_int32(&seed);

        double fexp = acc;

        fexp += round( x * ldexp(y, -30) + ldexp(1,-41) );

        fexp = MIN(fexp, VPU_INT40_MAX);
        fexp = MAX(fexp, VPU_INT40_MIN);

        vpu_int32_acc_t exp = fexp;

        vpu_int32_acc_t res = vlmacc32(acc, x, y);

        if(res != exp){
            printf("acc = %lld     (0x%010llX)\n", acc,   (uint64_t) acc);
            printf("x = %ld     (0x%08lX)\n", x,   (uint32_t) x);
            printf("y = %ld     (0x%08lX)\n", y,   (uint32_t) y);
            printf("exp = %lld     (0x%010llX)\n", exp,   (uint64_t) exp);
            printf("res = %lld     (0x%010llX)\n", res,   (uint64_t) res);
        }

        TEST_ASSERT_EQUAL(exp, res);
    }
}



static void test_vlmaccr32()
{
    PRINTF("%s...\n", __func__);
    seed = 778786;


    for(int v = 0; v < REPS; v++){
        PRINTF("\trepetition %d.. (seed: 0x%08X)\n", v, seed);

        vpu_int32_acc_t acc = pseudo_rand_int64(&seed) >> 24;
        int32_t x[VPU_INT32_EPV];
        int32_t y[VPU_INT32_EPV];

        double s = acc;

        for(int i = 0; i < VPU_INT32_EPV; i++){
            x[i] = pseudo_rand_int32(&seed);
            y[i] = pseudo_rand_int32(&seed);

            
            s += round( x[i] * ldexp(y[i], -30) + ldexp(1,-41) );

        }

        s = MIN(s, VPU_INT40_MAX);
        s = MAX(s, VPU_INT40_MIN);

        vpu_int32_acc_t exp = s;

        vpu_int32_acc_t res = vlmaccr32(acc, x, y);

        // if(s != res){
        //     printf("rep: %d \t\tseed: 0x%08X\n", v, seed);
        //     printf("acc = %ld\n", acc);
        //     printf("x = [ ");
        //     for(int i = 0; i < 32; i++)
        //         printf("%d, ", x[i]);
        //     printf(" ]\n");
            
        //     printf("y = [ ");
        //     for(int i = 0; i < 32; i++)
        //         printf("%d, ", y[i]);
        //     printf(" ]\n");
        // }

        TEST_ASSERT_EQUAL(s, res);
    }
}



static void test_vlsat32()
{
    PRINTF("%s...\n", __func__);
    seed = 6457;

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
            PRINTF("\trepetition %d.. (seed: 0x%08X)\n", v, seed);

            for(headroom_t hr = 0; hr < 40; hr++){

                int32_t acc = (pseudo_rand_int64(&seed) >> 24) >> hr;

                int32_t shr = pseudo_rand_int(&seed, 31 - hr, 33 - hr);

                if(shr < 0)
                    shr = 0;


                double fexp = ldexp(acc, 0);
                
                // final term is because negative ties round differently on the VPU and in floating point
                fexp = round( fexp * ldexp(1, -shr) + ldexp(1,-39) );

                fexp = MIN(fexp, VPU_INT32_MAX);
                fexp = MAX(fexp, VPU_INT32_MIN);

                int32_t exp = fexp;

                int32_t res = vlsat32(acc, shr);

                // printf("acc = %ld\n", acc);
                // printf("shr = %d\n", shr);
                // printf("exp = %d\n", exp);
                // printf("res = %d\n", res);

                TEST_ASSERT_EQUAL_INT32(exp, res);
        }
    }
}


static void test_vcmr32()
{
    PRINTF("%s...\n", __func__);
    seed = 34563;


    for(int v = 0; v < REPS; v++){
        PRINTF("\trepetition %d.. (seed: 0x%08X)\n", v, seed);

        complex_s32_t x;
        complex_s32_t y;

        x.re = pseudo_rand_int32(&seed);
        x.im = pseudo_rand_int32(&seed);
        y.re = pseudo_rand_int32(&seed);
        y.im = pseudo_rand_int32(&seed);

        double a = round( x.re * ldexp(y.re, -30) + ldexp(1,-40) );
        double b = round( x.im * ldexp(y.im, -30) + ldexp(1,-40) );

        double fexp = a - b;
        fexp = MIN(fexp, VPU_INT32_MAX);
        fexp = MAX(fexp, VPU_INT32_MIN);

        int32_t exp = fexp;

        int32_t res = vcmr32(x, y);

        TEST_ASSERT_EQUAL_INT32(exp, res);
    }
}


static void test_vcmi32()
{
    PRINTF("%s...\n", __func__);
    seed = 34563;


    for(int v = 0; v < REPS; v++){
        PRINTF("\trepetition %d.. (seed: 0x%08X)\n", v, seed);

        complex_s32_t x;
        complex_s32_t y;

        x.re = pseudo_rand_int32(&seed);
        x.im = pseudo_rand_int32(&seed);
        y.re = pseudo_rand_int32(&seed);
        y.im = pseudo_rand_int32(&seed);

        double a = round( x.re * ldexp(y.im, -30) + ldexp(1,-40) );
        double b = round( x.im * ldexp(y.re, -30) + ldexp(1,-40) );

        double fexp = a + b;
        fexp = MIN(fexp, VPU_INT32_MAX);
        fexp = MAX(fexp, VPU_INT32_MIN);

        int32_t exp = fexp;

        int32_t res = vcmi32(x, y);

        TEST_ASSERT_EQUAL_INT32(exp, res);
    }
}


static void test_vcmcr32()
{
    PRINTF("%s...\n", __func__);
    seed = 34563;


    for(int v = 0; v < REPS; v++){
        PRINTF("\trepetition %d.. (seed: 0x%08X)\n", v, seed);

        complex_s32_t x;
        complex_s32_t y;

        x.re = pseudo_rand_int32(&seed);
        x.im = pseudo_rand_int32(&seed);
        y.re = pseudo_rand_int32(&seed);
        y.im = pseudo_rand_int32(&seed);

        double a = round( x.re * ldexp(y.re, -30) + ldexp(1,-40) );
        double b = round( x.im * ldexp(y.im, -30) + ldexp(1,-40) );

        double fexp = a + b;
        fexp = MIN(fexp, VPU_INT32_MAX);
        fexp = MAX(fexp, VPU_INT32_MIN);

        int32_t exp = fexp;

        int32_t res = vcmcr32(x, y);

        TEST_ASSERT_EQUAL_INT32(exp, res);
    }
}


static void test_vcmci32()
{
    PRINTF("%s...\n", __func__);
    seed = 34563;


    for(int v = 0; v < REPS; v++){
        PRINTF("\trepetition %d.. (seed: 0x%08X)\n", v, seed);

        complex_s32_t x;
        complex_s32_t y;

        x.re = pseudo_rand_int32(&seed);
        x.im = pseudo_rand_int32(&seed);
        y.re = pseudo_rand_int32(&seed);
        y.im = pseudo_rand_int32(&seed);

        double a = round( x.re * ldexp(y.im, -30) + ldexp(1,-40) );
        double b = round( x.im * ldexp(y.re, -30) + ldexp(1,-40) );

        double fexp = b - a;
        fexp = MIN(fexp, VPU_INT32_MAX);
        fexp = MAX(fexp, VPU_INT32_MIN);

        int32_t exp = fexp;

        int32_t res = vcmci32(x, y);

        TEST_ASSERT_EQUAL_INT32(exp, res);
    }
}





void test_xs3_vpu_scalar_ops_s32()
{
    SET_TEST_FILE();

    RUN_TEST(test_vladd32);
    RUN_TEST(test_vlsub32);
    RUN_TEST(test_vlashr32);
    RUN_TEST(test_vpos32);
    RUN_TEST(test_vsign32);
    RUN_TEST(test_vdepth1_32);
    RUN_TEST(test_vdepth8_32);
    RUN_TEST(test_vdepth16_32);
    RUN_TEST(test_vlmul32);
    RUN_TEST(test_vlmacc32);
    RUN_TEST(test_vlmaccr32);
    RUN_TEST(test_vlsat32);
    RUN_TEST(test_vcmr32);
    RUN_TEST(test_vcmi32);
    RUN_TEST(test_vcmcr32);
    RUN_TEST(test_vcmci32);
}