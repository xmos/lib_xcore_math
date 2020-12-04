
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "bfp_math.h"

#include "../../tst_common.h"

#include "unity.h"

#if DEBUG_ON || 0
#undef DEBUG_ON
#define DEBUG_ON    (1)
#endif


#define REPS        IF_QUICK_TEST(100, 1000)
#define MAX_LEN     40 


static unsigned seed = 666;


static char msg_buff[200];

#define TEST_ASSERT_EQUAL_MSG(EXPECTED, ACTUAL, EXTRA, LINE_NUM)   do{          \
    if((EXPECTED)!=(ACTUAL)) {                                                  \
      sprintf(msg_buff, "%s (test vector @ line %u)", (EXTRA), (LINE_NUM));     \
      TEST_ASSERT_EQUAL_MESSAGE((EXPECTED), (ACTUAL), msg_buff);                \
    }} while(0)


void test_bfp_complex_s32_to_complex_s16_case0()
{
    PRINTF("%s...\n", __func__);

    seed = 546457;

    int16_t A_real[MAX_LEN];
    int16_t A_imag[MAX_LEN];
    complex_s32_t B_data[MAX_LEN];

    bfp_complex_s16_t A;
    bfp_complex_s32_t B;

    B.data = B_data;
    A.real = A_real;
    A.imag = A_imag;

    // Make sure the rounding behavior is as expected.

    // (note: rounding negative numbers is the same as rounding the number case to unsigned)

    B.length = A.length = 3;
    B.exp = 0;
    B.data[0].re = 0x00010001; // rounds down // 14 bits of HR -- so it should shift down 2 bits.
    B.data[0].im = 0x00010002; // rounds up
    B.data[1].re = 0x00000005; // rounds down
    B.data[1].im = 0x00000006; // rounds up
    B.data[2].re = 0xFFFFFFF9; // = -7   // rounds down
    B.data[2].im = 0xFFFFFFFA; // = -6   // rounds up

    // elm           binary          plus 2 (rnd)    shifted down 2
    // 0xFFFFFFF9 -> ...1111 1001 -> ...1111 1011 -> ...1111 1110 = -2
    // 0xFFFFFFFA -> ...1111 1010 -> ...1111 1100 -> ...1111 1111 = -1

    bfp_complex_s32_headroom(&B);

    TEST_ASSERT_EQUAL(14, B.hr);

    bfp_complex_s32_to_complex_s16(&A, &B);

    TEST_ASSERT_EQUAL(2, A.exp);
    TEST_ASSERT_EQUAL(0, A.hr);

    TEST_ASSERT_EQUAL_INT16( 0x4000, A.real[0]);
    TEST_ASSERT_EQUAL_INT16( 0x4001, A.imag[0]);
    TEST_ASSERT_EQUAL_INT16( 0x0001, A.real[1]);
    TEST_ASSERT_EQUAL_INT16( 0x0002, A.imag[1]);
    TEST_ASSERT_EQUAL_INT16(-0x0002, A.real[2]);
    TEST_ASSERT_EQUAL_INT16(-0x0001, A.imag[2]);
}



void test_bfp_complex_s32_to_complex_s16_case1()
{
    PRINTF("%s...\n", __func__);

    seed = 0x70B304CF;

    int16_t A_real[MAX_LEN];
    int16_t A_imag[MAX_LEN];
    complex_s32_t B_data[MAX_LEN];

    bfp_complex_s16_t A;
    bfp_complex_s32_t B;

    struct {
        int16_t real[MAX_LEN];
        int16_t imag[MAX_LEN];
    } expected;


    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        
        bfp_complex_s32_init(&B, B_data, pseudo_rand_int(&seed, -30, 30),
                                              pseudo_rand_uint(&seed, 1, MAX_LEN+1), 0);

        bfp_complex_s16_init(&A, A_real, A_imag, INT_MAX, B.length, 0);

        B.hr = r? pseudo_rand_uint(&seed, 0, 28) : 0;

        // First 2 elements aren't random
        B.data[0].re =  0x7FFF0000 >> B.hr;
        B.data[0].im =  0x40000000 >> B.hr;
        B.data[1].re = ((int)-0x80000000) >> B.hr;
        B.data[1].im = -0x40000000 >> B.hr;

        expected.real[0] =  (B.data[0].re << B.hr) >> 16; // lower bits of expectation depends on headroom
        expected.imag[0] =  0x4000;
        expected.real[1] =  MAX(-0x7FFF, (((-0x7FFF0000) >> B.hr) << B.hr) >> 16);
        expected.imag[1] = -0x4000;

        right_shift_t exp_sh = MAX(0, B.hr-16);

        for(int i = 2; i < B.length; i++){

            expected.real[i] = (pseudo_rand_int16(&seed) >> exp_sh) << exp_sh;
            expected.imag[i] = (pseudo_rand_int16(&seed) >> exp_sh) << exp_sh;

            B.data[i].re = ((int32_t)expected.real[i]) << 16;
            B.data[i].im = ((int32_t)expected.imag[i]) << 16;

            B.data[i].re >>= B.hr;
            B.data[i].im >>= B.hr;

            if(expected.real[i] == ((int16_t)-0x8000))  expected.real[i] = (int16_t) -0x7FFF;
            if(expected.imag[i] == ((int16_t)-0x8000))  expected.imag[i] = (int16_t) -0x7FFF;
        }

        // PRINTF("\t    B.length = %u\n", B.length);
        // PRINTF("\t    B.exp = %d\n", B.exp);
        // PRINTF("\t    B.hr = %u\n", B.hr);

        // for(int i = 0; i < B.length; i++){
        //     PRINTF("\t        B.data[% 3d].re = % 15ld  (0x%08lX)\n", i, B.data[i].re, (uint32_t) B.data[i].re);
        //     PRINTF("\t        B.data[% 3d].im = % 15ld  (0x%08lX)\n", i, B.data[i].im, (uint32_t) B.data[i].im);
        // }
        
        TEST_ASSERT_EQUAL(B.hr, xs3_vect_complex_s32_headroom(B.data, B.length));

        bfp_complex_s32_to_complex_s16(&A, &B);

        TEST_ASSERT_EQUAL(B.length, A.length);
        
        // PRINTF("\t    A.length = %u\n", A.length);
        // PRINTF("\t    A.exp = %d\n", A.exp);
        // PRINTF("\t    A.hr = %u\n", A.hr);

        // for(int i = 0; i < A.length; i++){
        //     PRINTF("\t        A.real[% 3d] = % 15d  (0x%04X)\n", i, A.real[i], (uint16_t) A.real[i]);
        //     PRINTF("\t        A.imag[% 3d] = % 15d  (0x%04X)\n", i, A.imag[i], (uint16_t) A.imag[i]);
        // }

        TEST_ASSERT_EQUAL(xs3_vect_complex_s16_headroom(A.real, A.imag, A.length), A.hr);
        TEST_ASSERT_EQUAL(0, A.hr);
        
        TEST_ASSERT(ldexp(A.real[0], A.exp) == ldexp(B.data[0].re, B.exp)   );
        
        // for(int i = 0; i < A.length; i++){
        //     PRINTF("\t        expected.real[% 3d] = % 15d  (0x%04X)\n", i, expected.real[i], (uint16_t) expected.real[i]);
        //     PRINTF("\t        expected.imag[% 3d] = % 15d  (0x%04X)\n", i, expected.imag[i], (uint16_t) expected.imag[i]);
        // }

        TEST_ASSERT_EQUAL_INT16_ARRAY_MESSAGE(expected.real, A.real, A.length, "REAL");
        TEST_ASSERT_EQUAL_INT16_ARRAY_MESSAGE(expected.imag, A.imag, A.length, "IMAG");
    }
}





void test_bfp_complex_s16_to_complex_s32()
{
    PRINTF("%s...\n", __func__);

    seed = 546457;

    complex_s32_t A_data[MAX_LEN];
    complex_s32_t expected[MAX_LEN];

    struct {
        int16_t real[MAX_LEN];
        int16_t imag[MAX_LEN];
    } B_data;

    bfp_complex_s32_t A;
    bfp_complex_s16_t B;

    A.data = A_data;
    B.real = B_data.real;
    B.imag = B_data.imag;


    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);

        B.length = pseudo_rand_uint(&seed, 1, MAX_LEN+1);
        B.exp = pseudo_rand_int(&seed, -30, 30);
        B.hr = pseudo_rand_uint(&seed, 0, 14);

        A.length = B.length;

        expected[0].re = B.real[0] =  0x7FFF >> B.hr;
        expected[0].im = B.imag[0] = -0x8000 >> B.hr;
        

        for(int i = 1; i < B.length; i++){
            B.real[i] = pseudo_rand_int16(&seed) >> B.hr;
            B.imag[i] = pseudo_rand_int16(&seed) >> B.hr;
            
            expected[i].re = ((int32_t) B.real[i]);
            expected[i].im = ((int32_t) B.imag[i]);
        }

        // PRINTF("\t    B.length = %u\n", B.length);
        // PRINTF("\t    B.exp = %d\n", B.exp);
        // PRINTF("\t    B.hr = %u\n", B.hr);

        // for(int i = 0; i < B.length; i++){
        //     PRINTF("\t        B.real[% 3d] = % 7d  (0x%04X)\n", i, B.real[i], (uint16_t) B.real[i]);
        //     PRINTF("\t        B.imag[% 3d] = % 7d  (0x%04X)\n", i, B.imag[i], (uint16_t) B.imag[i]);
        // }

        TEST_ASSERT_EQUAL_MESSAGE(B.hr, xs3_vect_complex_s16_headroom(B.real, B.imag, B.length), "[Input headroom is wrong]");

        bfp_complex_s16_to_complex_s32(&A, &B);

        // PRINTF("\t    A.length = %u\n", A.length);
        // PRINTF("\t    A.exp = %d\n", A.exp);
        // PRINTF("\t    A.hr = %u\n", A.hr);

        // for(int i = 0; i < A.length; i++){
        //     PRINTF("\t        A.data[% 3d].re = % 15ld  (0x%08lX)\n", i, A.data[i].re, (uint32_t) A.data[i].re);
        //     PRINTF("\t        A.data[% 3d].im = % 15ld  (0x%08lX)\n", i, A.data[i].im, (uint32_t) A.data[i].im);
        // }

        TEST_ASSERT_EQUAL_MESSAGE(B.exp, A.exp, "[Output exponent is wrong]");
        
        TEST_ASSERT_EQUAL_MESSAGE(xs3_vect_complex_s32_headroom(A.data, A.length), A.hr, "[Output headroom is wrong]");

        TEST_ASSERT_EQUAL_INT32_ARRAY((int32_t*) &expected, (int32_t*) A.data, 2*A.length);

    }
}





void test_bfp_complex_bitdepth_convert()
{
    SET_TEST_FILE();
    RUN_TEST(test_bfp_complex_s32_to_complex_s16_case0);
    RUN_TEST(test_bfp_complex_s32_to_complex_s16_case1);
    RUN_TEST(test_bfp_complex_s16_to_complex_s32);
}