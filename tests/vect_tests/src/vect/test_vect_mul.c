// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xmath/xmath.h"
#include "xmath/xs3/vpu_scalar_ops.h"

#include "../tst_common.h"
#include "unity_fixture.h"

TEST_GROUP(vect_mul);
TEST_SETUP(vect_mul) { fflush(stdout); }
TEST_TEAR_DOWN(vect_mul) {}

TEST_GROUP_RUNNER(vect_mul) {
  RUN_TEST_CASE(vect_mul, vect_s16_mul_prepare);
  RUN_TEST_CASE(vect_mul, vect_s32_mul_prepare);
  RUN_TEST_CASE(vect_mul, vect_s16_mul_basic);
  RUN_TEST_CASE(vect_mul, vect_s16_mul_random);
  RUN_TEST_CASE(vect_mul, vect_s32_mul_basic);
  RUN_TEST_CASE(vect_mul, vect_s32_mul_random);
}



static char msg_buff[200];

#define TEST_ASSERT_EQUAL_MSG(EXPECTED, ACTUAL, LINE_NUM)   do{       \
    if((EXPECTED)!=(ACTUAL)) {                                        \
      sprintf(msg_buff, "(test vector @ line %u)", (LINE_NUM));       \
      TEST_ASSERT_EQUAL_MESSAGE((EXPECTED), (ACTUAL), msg_buff);      \
    }} while(0)


/**
 * This is a VLMACC-based multiply, which means the right-shift
 * is applied using VLSAT rather than VLASHR. (VLASHR doesn't apply
 * the rounding logic used by VLSAT)
 */
static int16_t mul_s16(int16_t b, int16_t c, int a_shr)
{
    int32_t A = ((int32_t)b)*c;
    int32_t a = A;

    if(a_shr != 0)
      a = a + (1 << (a_shr-1));
      
    a = a >> a_shr;
    a = (a >= VPU_INT16_MAX)? VPU_INT16_MAX : (a <= VPU_INT16_MIN)? VPU_INT16_MIN : a;

    return (int16_t) a;
}

static int32_t mul_s32(int32_t b, int32_t c, int b_shr, int c_shr)
{
    int64_t bp = b;
    int64_t cp = c;

    bp = (b_shr >= 0)? bp >> b_shr : bp << (-b_shr);
    cp = (c_shr >= 0)? cp >> c_shr : cp << (-c_shr);

    bp = (bp >= VPU_INT32_MAX)? VPU_INT32_MAX : (bp <= VPU_INT32_MIN)? VPU_INT32_MIN : bp;
    bp = ( (b_shr > 0) && (b < 0) && (-(1<<b_shr)) < b)? -1 : bp;
    assert(b >= 0 || bp < 0);
    
    cp = (cp >= VPU_INT32_MAX)? VPU_INT32_MAX : (cp <= VPU_INT32_MIN)? VPU_INT32_MIN : cp;
    cp = ( (c_shr > 0) && (c < 0) && (-(1<<c_shr)) < c)? -1 : cp;
    assert(c >= 0 || cp < 0);

    int64_t a = (bp * cp + (1<<29)) >> (30);

    a = (a >= VPU_INT32_MAX)? VPU_INT32_MAX : (a <= VPU_INT32_MIN)? VPU_INT32_MIN : a;

    return (int32_t) a;
}



#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (64)
#else
#  define REPS       (1000)
#  define MAX_LEN    (256)
#endif


TEST(vect_mul, vect_s16_mul_prepare)
{
    unsigned seed = SEED_FROM_FUNC_NAME();


    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);
        
        exponent_t b_exp = ((int32_t)(pseudo_rand_uint32(&seed) % 60)) - 30;
        headroom_t b_hr  = pseudo_rand_uint32(&seed) % 16;
        exponent_t c_exp = ((int32_t)(pseudo_rand_uint32(&seed) % 60)) - 30;
        headroom_t c_hr  = pseudo_rand_uint32(&seed) % 16;

        exponent_t a_exp;
        right_shift_t a_shr;

        vect_s16_mul_prepare(&a_exp, &a_shr, b_exp, c_exp, b_hr, c_hr);

        // a_exp and a_shr should be calculated for exactly this case
        int16_t WORD_ALIGNED B = (-0x8000) >> b_hr;
        int16_t WORD_ALIGNED C = (-0x8000) >> c_hr;
        int16_t WORD_ALIGNED A;

        vect_s16_mul(&A, &B, &C, 1, a_shr);

        double q = ldexp(B, b_exp) * ldexp(C, c_exp);
        double p = ldexp(A, a_exp);

        TEST_ASSERT_EQUAL_MESSAGE(a_shr > 0? 0x4000 : B*C, A, "A is wrong");
        TEST_ASSERT_EQUAL(q, p);
    }
}


TEST(vect_mul, vect_s32_mul_prepare)
{
    unsigned seed = SEED_FROM_FUNC_NAME();


    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);
        
        exponent_t b_exp = ((int32_t)(pseudo_rand_uint32(&seed) % 60)) - 30;
        headroom_t b_hr  = pseudo_rand_uint32(&seed) % 31;
        exponent_t c_exp = ((int32_t)(pseudo_rand_uint32(&seed) % 60)) - 30;
        headroom_t c_hr  = pseudo_rand_uint32(&seed) % 31;

        exponent_t a_exp;
        right_shift_t b_shr, c_shr;

        vect_s32_mul_prepare(&a_exp, &b_shr, &c_shr, b_exp, c_exp, b_hr, c_hr);

        // a_exp and a_shr should be calculated for exactly this case
        int32_t WORD_ALIGNED B = (int) (0-0x80000000);
        int32_t WORD_ALIGNED C = (int) (0-0x80000000);
        B = B >> b_hr;
        C = C >> c_hr;
        int32_t WORD_ALIGNED A;

        vect_s32_mul(&A, &B, &C, 1, b_shr, c_shr);

        double q = ldexp(B, b_exp) * ldexp(C, c_exp);
        double p = ldexp(A, a_exp);

        TEST_ASSERT_EQUAL_MESSAGE(0x40000000, A, "A is wrong");
        TEST_ASSERT_EQUAL(q, p);
    }
}


TEST(vect_mul, vect_s16_mul_basic)
{
    typedef struct {
        struct {    int16_t b;  int16_t c;  } value;
        right_shift_t a_shr;
        int16_t expected;
        unsigned line;
    } test_case_t;

    test_case_t cases[] = {
        // value{       b         c }    a_shr        exp        line num
        {       {  0x0000,   0x0000 },     0,      0x0000,       __LINE__},
        {       {  0x0001,   0x0000 },     0,      0x0000,       __LINE__},
        {       {  0x0000,   0x0001 },     0,      0x0000,       __LINE__},
        {       {  0x0001,   0x0001 },     0,      0x0001,       __LINE__},
        {       {  0x0002,   0x0001 },     0,      0x0002,       __LINE__},
        {       {  0x0001,   0x0002 },     0,      0x0002,       __LINE__},
        {       {  0x0010,   0x0010 },     0,      0x0100,       __LINE__},
        {       { -0x0001,   0x0001 },     0,     -0x0001,       __LINE__},
        {       {  0x0100,   0x0100 },     0,      0x7FFF,       __LINE__},
        {       {  0x0100,   0x0100 },     2,      0x4000,       __LINE__},
    };

    const unsigned N_cases = sizeof(cases)/sizeof(test_case_t);

    const unsigned start_case = 0;

    for(unsigned int v = start_case; v < N_cases; v++){
        setExtraInfo_R(v);
        
        test_case_t* casse = &cases[v];
        
        //Verify mul_s16() is correct. It's used in other test cases.
        TEST_ASSERT_EQUAL_MSG(casse->expected,
                    mul_s16(casse->value.b, casse->value.c, casse->a_shr),
                    casse->line);

        unsigned lengths[] = {1, 4, 16, 32, 40 };

        for( int l = 0; l < sizeof(lengths)/sizeof(lengths[0]); l++){
            unsigned len = lengths[l];

            headroom_t hr;
            int16_t A[40];
            int16_t B[40];
            int16_t C[40];

            for(unsigned int i = 0; i < len; i++){
                A[i] = 0xCC;
                B[i] = casse->value.b;
                C[i] = casse->value.c;
            }

            hr = vect_s16_mul(A, B, C, len, casse->a_shr);

            for(unsigned int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(vect_s16_headroom(A, len), hr, casse->line);
            }

            memcpy(A, B, sizeof(A));
            hr = vect_s16_mul(A, A, C, len, casse->a_shr);

            for(unsigned int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(vect_s16_headroom(A, len), hr, casse->line);
            }

            memcpy(A, C, sizeof(A));
            hr = vect_s16_mul(A, B, A, len, casse->a_shr);

            for(unsigned int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(vect_s16_headroom(A, len), hr, casse->line);
            }

        }
    }
}


TEST(vect_mul, vect_s16_mul_random)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    headroom_t hr;
    int16_t A[MAX_LEN];
    int16_t B[MAX_LEN];
    int16_t C[MAX_LEN];

    int16_t expected[MAX_LEN];

    const char debug_fmt[] = "Expected: %d <-- (%d * %d) >> %d \nActual: %d\n";

    for(unsigned int v = 0; v < REPS; v++){
        unsigned old_seed = seed;
        unsigned len = (pseudo_rand_uint32(&seed) % MAX_LEN) + 1;
        
        setExtraInfo_RSL(v, old_seed, len);
        
        for(unsigned int i = 0; i < len; i++){
            unsigned shr = pseudo_rand_uint32(&seed) % 8;
            B[i] = pseudo_rand_int16(&seed) >> shr;
            C[i] = pseudo_rand_int16(&seed) >> shr;
        }

        int a_shr = (pseudo_rand_uint32(&seed) % 5);

        // Determine expected outputs
        for(unsigned int i = 0; i < len; i++)
          expected[i] = mul_s16(B[i], C[i], a_shr);
        
        
        // A <-- B * C
        hr = vect_s16_mul(A, B, C, len, a_shr);

        XTEST_ASSERT_VECT_S16_EQUAL(expected, A, len,
            debug_fmt, expected[i], B[i], C[i], a_shr, A[i] );
        TEST_ASSERT_EQUAL(vect_s16_headroom(A, len), hr);
        
        // A <-- B
        // A <-- A * C
        memcpy(A, B, sizeof(A[0])*len);
        hr = vect_s16_mul(A, A, C, len, a_shr);

        XTEST_ASSERT_VECT_S16_EQUAL(expected, A, len,
            debug_fmt, expected[i], B[i], C[i], a_shr, A[i] );
        TEST_ASSERT_EQUAL(vect_s16_headroom(A, len), hr);
        
        // A <-- C
        // A <-- B * A
        memcpy(A, C, sizeof(A[0])*len);
        hr = vect_s16_mul(A, B, A, len, a_shr);

        XTEST_ASSERT_VECT_S16_EQUAL(expected, A, len,
            debug_fmt, expected[i], B[i], C[i], a_shr, A[i] );
        TEST_ASSERT_EQUAL(vect_s16_headroom(A, len), hr);
        
    }
}


TEST(vect_mul, vect_s32_mul_basic)
{
    typedef struct {
        struct {    int32_t b;  int32_t c;  } value;
        struct {    int b;      int c;      } shr;
        int32_t expected;
        unsigned line;
    } test_case_t;

    test_case_t cases[] = {
        // value{                b               c }   shr{  b   c }              exp        line num
        {       {       0x00000000,     0x00000000 },     {  0,  0 },      0x00000000,       __LINE__},
        {       {       0x00010000,     0x00000000 },     {  0,  0 },      0x00000000,       __LINE__},
        {       {       0x00000000,     0x00010000 },     {  0,  0 },      0x00000000,       __LINE__},
        {       {       0x40000000,     0x40000000 },     {  0,  0 },      0x40000000,       __LINE__},
        {       {      -0x40000000,     0x40000000 },     {  0,  0 },     -0x40000000,       __LINE__},
        {       {      -0x40000000,    -0x40000000 },     {  0,  0 },      0x40000000,       __LINE__},
        {       {       0x40000000,     0x00010000 },     {  0,  0 },      0x00010000,       __LINE__},
        {       {       0x00010000,     0x40000000 },     {  0,  0 },      0x00010000,       __LINE__},
        {       {       0x40000000,     0x01230000 },     {  0,  0 },      0x01230000,       __LINE__},
        {       {       0x01230000,     0x40000000 },     {  0,  0 },      0x01230000,       __LINE__},
        {       {       0x00400000,     0x02000000 },     {  0,  0 },      0x00020000,       __LINE__},
        {       {       0x00400000,     0x01000000 },     {  0,  0 },      0x00010000,       __LINE__},
        {       {       0x00004000,     0x00010000 },     {  0,  0 },      0x00000001,       __LINE__},
        {       {       0x00004000,     0x00008000 },     {  0,  0 },      0x00000001,       __LINE__},
        {       {       0x00000400,     0x00000400 },     {  0,  0 },      0x00000000,       __LINE__},
        {       {       0x7f000000,     0x7f000000 },     {  0,  0 },      0x7fffffff,       __LINE__},
        {       {       0x7f000000,    -0x7f000000 },     {  0,  0 },     -0x7fffffff,       __LINE__},
        {       { (int) (0-0x80000000),     0x40000000 },     {  0,  0 },     -0x7fffffff,       __LINE__},
        {       {       0x40000000,     0x40000000 },     {  1,  0 },      0x20000000,       __LINE__},
        {       {       0x40000000,     0x40000000 },     {  0,  1 },      0x20000000,       __LINE__},
        {       {       0x40000000,     0x40000000 },     {  1,  1 },      0x10000000,       __LINE__},
        {       {       0x40000000,     0x40000000 },     {  3,  5 },      0x00400000,       __LINE__},
        {       {       0x40000000,     0x08000000 },     {  0, -1 },      0x10000000,       __LINE__},
        {       {       0x40000000,     0x08000000 },     {  0, -2 },      0x20000000,       __LINE__},
        {       {       0x40000000,     0x08000000 },     {  0, -3 },      0x40000000,       __LINE__},
        {       {       0x40000000,     0x08000000 },     {  0, -4 },      0x7fffffff,       __LINE__},
        {       {      -0x40000000,     0x08000000 },     {  0, -4 },     -0x7fffffff,       __LINE__},
        {       {       0x40000000,     0x08000000 },     {  1, -4 },      0x40000000,       __LINE__},
    };

    const unsigned N_cases = sizeof(cases)/sizeof(test_case_t);

    const unsigned start_case = 0;

    for(unsigned int v = start_case; v < N_cases; v++){
        setExtraInfo_R(v);
        
        test_case_t* casse = &cases[v];

        //Verify mul_s32() is correct. It's used in other test cases.
        TEST_ASSERT_EQUAL_MSG(casse->expected, mul_s32(casse->value.b, casse->value.c, casse->shr.b, casse->shr.c), casse->line);

        unsigned lengths[] = {1, 4, 16, 32, 40 };
        

        for( int l = 0; l < sizeof(lengths)/sizeof(lengths[0]); l++){
            unsigned len = lengths[l];

            headroom_t hr;
            int32_t A[40];
            int32_t B[40];
            int32_t C[40];

            for(unsigned int i = 0; i < len; i++){
                A[i] = 0xCC;
                B[i] = casse->value.b;
                C[i] = casse->value.c;
            }

            hr = vect_s32_mul(A, B, C, len, casse->shr.b, casse->shr.c);

            for(unsigned int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(vect_s32_headroom(A, len), hr, casse->line);
            }

            memcpy(A, B, sizeof(A));
            hr = vect_s32_mul(A, A, C, len, casse->shr.b, casse->shr.c);

            for(unsigned int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(vect_s32_headroom(A, len), hr, casse->line);
            }

            memcpy(A, C, sizeof(A));
            hr = vect_s32_mul(A, B, A, len, casse->shr.b, casse->shr.c);

            for(unsigned int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(vect_s32_headroom(A, len), hr, casse->line);
            }
        }
    }
}


TEST(vect_mul, vect_s32_mul_random)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    headroom_t hr;
    int32_t A[MAX_LEN];
    int32_t B[MAX_LEN];
    int32_t C[MAX_LEN];

    int32_t expected[MAX_LEN];

    const char debug_fmt[] = "Expected: %ld <-- (%ld >> %d) * (%ld >> %d) \nActual: %ld\n";

    for(unsigned int v = 0; v < REPS; v++){
        unsigned old_seed = seed;
        unsigned len = (pseudo_rand_uint32(&seed) % MAX_LEN) + 1;

        setExtraInfo_RSL(v, old_seed, len);
        
        for(unsigned int i = 0; i < len; i++){
            unsigned shr = pseudo_rand_uint32(&seed) % 8;
            B[i] = pseudo_rand_int32(&seed) >> shr;
            C[i] = pseudo_rand_int32(&seed) >> shr;
        }

        int b_shr = (pseudo_rand_uint32(&seed) % 5) - 2;
        int c_shr = (pseudo_rand_uint32(&seed) % 5) - 2;
        

        // Determine expected outputs
        for(unsigned int i = 0; i < len; i++)
          expected[i] = mul_s32(B[i], C[i], b_shr, c_shr);
        

        // A <-- B * C
        hr = vect_s32_mul(A, B, C, len, b_shr, c_shr);

        XTEST_ASSERT_VECT_S32_EQUAL(expected, A, len,
            debug_fmt, (long int) expected[i], (long int) B[i], b_shr, (long int) C[i], c_shr, (long int) A[i] );
        TEST_ASSERT_EQUAL(vect_s32_headroom(A, len), hr);
        
        // A <-- B
        // A <-- A * C
        memcpy(A, B, sizeof(A[0])*len);
        hr = vect_s32_mul(A, A, C, len, b_shr, c_shr);

        XTEST_ASSERT_VECT_S32_EQUAL(expected, A, len,
            debug_fmt, (long int) expected[i], (long int) B[i], b_shr, (long int) C[i], c_shr, (long int) A[i] );
        TEST_ASSERT_EQUAL(vect_s32_headroom(A, len), hr);
        
        // A <-- C
        // A <-- B * A
        memcpy(A, C, sizeof(A[0])*len);
        hr = vect_s32_mul(A, B, A, len, b_shr, c_shr);

        XTEST_ASSERT_VECT_S32_EQUAL(expected, A, len,
            debug_fmt, (long int) expected[i], (long int) B[i], b_shr, (long int) C[i], c_shr, (long int) A[i] );
        TEST_ASSERT_EQUAL(vect_s32_headroom(A, len), hr);
        
    }
}

