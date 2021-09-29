// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xs3_math.h"

#include "../src/vect/vpu_helper.h"

#include "../../tst_common.h"

#include "unity_fixture.h"


TEST_GROUP_RUNNER(xs3_vect_complex_mul) {
  RUN_TEST_CASE(xs3_vect_complex_mul, xs3_vect_complex_s16_mul_prepare)
  RUN_TEST_CASE(xs3_vect_complex_mul, xs3_vect_complex_s32_mul_prepare);
  RUN_TEST_CASE(xs3_vect_complex_mul, xs3_vect_complex_s16_mul_basic);
  RUN_TEST_CASE(xs3_vect_complex_mul, xs3_vect_complex_s16_mul_random);
  RUN_TEST_CASE(xs3_vect_complex_mul, xs3_vect_complex_s32_mul_basic);
  RUN_TEST_CASE(xs3_vect_complex_mul, xs3_vect_complex_s32_mul_random);
}

TEST_GROUP(xs3_vect_complex_mul);
TEST_SETUP(xs3_vect_complex_mul) { fflush(stdout); }
TEST_TEAR_DOWN(xs3_vect_complex_mul) {}


static char msg_buff[200];

#define TEST_ASSERT_EQUAL_MSG(EXPECTED, ACTUAL, LINE_NUM)   do{       \
    if((EXPECTED)!=(ACTUAL)) {                                        \
      sprintf(msg_buff, "(test vector @ line %u)", (LINE_NUM));       \
      TEST_ASSERT_EQUAL_MESSAGE((EXPECTED), (ACTUAL), msg_buff);      \
    }} while(0)


static complex_s16_t mul_complex_s16(int16_t b_re, int16_t b_im, 
                                     int16_t c_re, int16_t c_im, 
                                     right_shift_t sat)
{
    int64_t bp_re = b_re;
    int64_t bp_im = b_im;
    int64_t cp_re = c_re;
    int64_t cp_im = c_im;

    int64_t a_re = (bp_re * cp_re - bp_im * cp_im);
    int64_t a_im = (bp_re * cp_im + bp_im * cp_re);

    a_re = SAT(16)(ROUND_SHR(a_re, sat));
    a_im = SAT(16)(ROUND_SHR(a_im, sat));

    complex_s16_t res = {a_re, a_im};
    return res;
}


static complex_s32_t mul_complex_s32(complex_s32_t b, complex_s32_t c, int b_shr, int c_shr)
{
    int64_t bp_re = b.re;
    int64_t bp_im = b.im;
    int64_t cp_re = c.re;
    int64_t cp_im = c.im;

    bp_re = ASHR(32)(bp_re, b_shr);
    bp_im = ASHR(32)(bp_im, b_shr);

    cp_re = ASHR(32)(cp_re, c_shr);
    cp_im = ASHR(32)(cp_im, c_shr);

    bp_re = SAT(32)(bp_re);
    bp_im = SAT(32)(bp_im);

    cp_re = SAT(32)(cp_re);
    cp_im = SAT(32)(cp_im);

    // printf("B = %ld\t%ld\t\t%d\n", b.re, b.im, b_shr);
    // printf("C = %ld\t%ld\t\t%d\n", c.re, c.im, c_shr);

    int64_t q1 = ((bp_re * cp_re) + (1<<29)) >> 30;
    int64_t q2 = ((bp_im * cp_im) + (1<<29)) >> 30;
    int64_t q3 = ((bp_re * cp_im) + (1<<29)) >> 30;
    int64_t q4 = ((bp_im * cp_re) + (1<<29)) >> 30;

    int64_t a_re = (q1 - q2);
    int64_t a_im = (q3 + q4);

    // int64_t a_re = ((bp_re * cp_re - bp_im * cp_im) + (1<<29)) >> 30;
    // int64_t a_im = ((bp_re * cp_im + bp_im * cp_re) + (1<<29)) >> 30;

    a_re = SAT(32)(a_re);
    a_im = SAT(32)(a_im);

    complex_s32_t res = {a_re, a_im};
    return res;
}


#define REPS        ((SMOKE_TEST)?100:1000)
TEST(xs3_vect_complex_mul, xs3_vect_complex_s16_mul_prepare)
{
    unsigned seed = SEED_FROM_FUNC_NAME();


    for(int r = 0; r < REPS; r++){
        setExtraInfo_RS(r, seed);
        
        exponent_t b_exp = pseudo_rand_int(&seed, -30, 30);
        headroom_t b_hr  = pseudo_rand_uint(&seed, 0, 14);

        exponent_t c_exp = pseudo_rand_int(&seed, -30, 30);
        headroom_t c_hr  = pseudo_rand_uint(&seed, 0, 14);

        exponent_t a_exp;
        right_shift_t a_shr;

        xs3_vect_complex_s16_mul_prepare(&a_exp, &a_shr, b_exp, c_exp, b_hr, c_hr);

        TEST_ASSERT_GREATER_OR_EQUAL_MESSAGE(0, a_shr, "[Computed sat value is negative.]");

        complex_s64_t B = { -0x8000 >> b_hr, -0x8000 >> b_hr };
        complex_s64_t C = { -0x8000 >> c_hr, -0x8000 >> c_hr };

        int64_t P_im = B.re * C.im + B.im * C.re;
        
        TEST_ASSERT_EQUAL_MESSAGE(b_exp+c_exp+a_shr, a_exp, "[Computed a_exp is wrong]");

        if(P_im <= ((int64_t)0x4000)){
            TEST_ASSERT_EQUAL_MESSAGE(0, a_shr, "[Computed sat is wrong]");
        } else if(P_im == ((int32_t)0x8000)){
            TEST_ASSERT_EQUAL_MESSAGE(0, a_shr, "[Computed sat is wrong]");
        } else {
            TEST_ASSERT( ldexp(P_im, -a_shr) == 0x8000 );
        }
    }
}
#undef REPS


#define REPS        ((SMOKE_TEST)?100:1000)


TEST(xs3_vect_complex_mul, xs3_vect_complex_s32_mul_prepare)
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

        // a_exp and a_shr should be calculated for exactly this case
        complex_s32_t WORD_ALIGNED B = {((int32_t)-0x80000000) >> b_hr, ((int32_t)-0x80000000) >> b_hr};
        complex_s32_t WORD_ALIGNED C = {((int32_t)-0x80000000) >> c_hr, ((int32_t)-0x80000000) >> c_hr};
        complex_s32_t WORD_ALIGNED A;

        xs3_vect_complex_s32_mul_prepare(&a_exp, &b_shr, &c_shr, b_exp, c_exp, b_hr, c_hr);

        xs3_vect_complex_s32_mul(&A, &B, &C, 1, b_shr, c_shr);

        TEST_ASSERT_EQUAL( INT32_MAX, A.im );
        xs3_vect_complex_s32_mul(&A, &B, &C, 1, b_shr+1, c_shr);
        TEST_ASSERT_EQUAL( 0x40000000, A.im );
    }
}
#undef REPS


TEST(xs3_vect_complex_mul, xs3_vect_complex_s16_mul_basic)
{
    typedef struct {
        struct {    complex_s16_t b;  
                    complex_s16_t c;  } value;
        right_shift_t sat;
        complex_s16_t expected;
        unsigned line;
    } test_case_t;

    test_case_t casses[] = {
        // value{ b{     re,     im},  c{     re,     im} },   sat,  exp{     re,     im},    line num
        {       {  { 0x0000, 0x0000},   { 0x0000, 0x0000} },     0,     { 0x0000, 0x0000},    __LINE__},
        {       {  { 0x0001, 0x0001},   { 0x0000, 0x0000} },     0,     { 0x0000, 0x0000},    __LINE__},
        {       {  { 0x0000, 0x0000},   { 0x0001, 0x0001} },     0,     { 0x0000, 0x0000},    __LINE__},
        {       {  { 0x0001, 0x0000},   { 0x0001, 0x0000} },     0,     { 0x0001, 0x0000},    __LINE__},
        {       {  { 0x0001, 0x0001},   { 0x0001,-0x0001} },     0,     { 0x0002, 0x0000},    __LINE__},
        {       {  {-0x0001, 0x0001},   {-0x0001, 0x0001} },     0,     { 0x0000,-0x0002},    __LINE__},
        {       {  { 0x0010, 0x0020},   { 0x0030, 0x0040} },     0,     {-0x0500, 0x0a00},    __LINE__},
        {       {  { 0x0010, 0x0020},   { 0x0030, 0x0040} },     4,     {-0x0050, 0x00a0},    __LINE__},
        {       {  {-0x4000,-0x4000},   {-0x4000,-0x4000} },     0,     { 0x0000, 0x7fff},    __LINE__},
        {       {  {-0x4000,-0x4000},   {-0x4000,-0x4000} },    15,     { 0x0000, 0x4000},    __LINE__},


    };

    const unsigned N_cases = sizeof(casses)/sizeof(test_case_t);

    const unsigned start_case = 0;

    for(int v = start_case; v < N_cases; v++){
        setExtraInfo_R(v);
        
        test_case_t* casse = &casses[v];

        //Verify mul_complex_s16() is correct. It's used in other test cases.
        complex_s16_t tmp = mul_complex_s16(casse->value.b.re, casse->value.b.im, 
                                            casse->value.c.re, casse->value.c.im, 
                                            casse->sat);
                                            
        TEST_ASSERT_EQUAL_MSG(casse->expected.re, tmp.re, casse->line);
        TEST_ASSERT_EQUAL_MSG(casse->expected.im, tmp.im, casse->line);

        unsigned lengths[] = {1, 4, 16, 32, 40 };
        

        for( int l = 0; l < sizeof(lengths)/sizeof(lengths[0]); l++){
            unsigned len = lengths[l];

            headroom_t hr;
            struct { 
                int16_t real[40]; 
                int16_t imag[40]; 
            } A, B, C;

            for(int i = 0; i < len; i++){
                A.real[i] = 0xCC;
                A.imag[i] = 0xCC;
                B.real[i] = casse->value.b.re;
                B.imag[i] = casse->value.b.im;
                C.real[i] = casse->value.c.re;
                C.imag[i] = casse->value.c.im;
            }

            hr = xs3_vect_complex_s16_mul(A.real, A.imag, 
                                                  B.real, B.imag, 
                                                  C.real, C.imag, 
                                                  len, casse->sat);
            headroom_t hrre, hrim;

            for(int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected.re, A.real[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(casse->expected.im, A.imag[0], casse->line);
            }
            hrre = xs3_vect_s16_headroom(A.real, len); hrim = xs3_vect_s16_headroom(A.imag, len);
            TEST_ASSERT_EQUAL_MSG((hrre <= hrim)? hrre : hrim, hr, casse->line);

            memcpy(&A, &B, sizeof(A));
            hr = xs3_vect_complex_s16_mul(A.real, A.imag,
                                                  A.real, A.imag,
                                                  C.real, C.imag,
                                                  len, casse->sat);

            for(int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected.re, A.real[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(casse->expected.im, A.imag[0], casse->line);
            }
            hrre = xs3_vect_s16_headroom(A.real, len); hrim = xs3_vect_s16_headroom(A.imag, len);
            TEST_ASSERT_EQUAL_MSG((hrre <= hrim)? hrre : hrim, hr, casse->line);

            /* NOTE: It is NOT safe to operate in-place with A == C */

        }
    }
}


#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (64)
#else
#  define REPS       (1000)
#  define MAX_LEN    (256)
#endif

TEST(xs3_vect_complex_mul, xs3_vect_complex_s16_mul_random)
{
    unsigned seed = SEED_FROM_FUNC_NAME();

    headroom_t hr;
    
    struct { 
        int16_t real[MAX_LEN]; 
        int16_t imag[MAX_LEN]; 
    } A, B, C;

    for(int v = 0; v < REPS; v++){

        setExtraInfo_RS(v, seed);

        unsigned len = (pseudo_rand_uint32(&seed) % MAX_LEN) + 1;
        
        for(int i = 0; i < len; i++){
            unsigned shr = pseudo_rand_uint32(&seed) % 8;
            B.real[i] = pseudo_rand_int16(&seed) >> shr;
            C.real[i] = pseudo_rand_int16(&seed) >> shr;
            B.imag[i] = pseudo_rand_int16(&seed) >> shr;
            C.imag[i] = pseudo_rand_int16(&seed) >> shr;
        }

        int sat = (pseudo_rand_uint32(&seed) % 10);
        
        hr = xs3_vect_complex_s16_mul(A.real, A.imag, 
                                              B.real, B.imag, 
                                              C.real, C.imag, 
                                              len, sat);

        headroom_t hrre, hrim;

        for(int i = 0; i < len; i++){
            complex_s16_t expected = mul_complex_s16(B.real[i], B.imag[i], C.real[i], C.imag[i], sat);
            
            TEST_ASSERT_EQUAL_MESSAGE(expected.re, A.real[i], msg_buff);
            TEST_ASSERT_EQUAL_MESSAGE(expected.im, A.imag[i], msg_buff);
        }
        hrre = xs3_vect_s16_headroom(A.real, len); hrim = xs3_vect_s16_headroom(A.imag, len);
        TEST_ASSERT_EQUAL_MSG((hrre <= hrim)? hrre : hrim, hr, v);
        
        memcpy(&A, &B, sizeof(A));
        hr = xs3_vect_complex_s16_mul(A.real, A.imag, 
                                              A.real, A.imag, 
                                              C.real, C.imag, 
                                              len, sat);

        for(int i = 0; i < len; i++){
            complex_s16_t expected = mul_complex_s16(B.real[i], B.imag[i], C.real[i], C.imag[i], sat);
            TEST_ASSERT_EQUAL_MESSAGE(expected.re, A.real[i], msg_buff);
            TEST_ASSERT_EQUAL_MESSAGE(expected.im, A.imag[i], msg_buff);
        }
        hrre = xs3_vect_s16_headroom(A.real, len); hrim = xs3_vect_s16_headroom(A.imag, len);
        TEST_ASSERT_EQUAL_MSG((hrre <= hrim)? hrre : hrim, hr, v);
        
        /* NOT safe to do A==C in place !! */

        
    }
}
#undef MAX_LEN
#undef REPS


TEST(xs3_vect_complex_mul, xs3_vect_complex_s32_mul_basic)
{
    

    typedef struct {
        struct {    complex_s32_t b;  
                    complex_s32_t c;  } value;
        struct {    int b;      int c;      } shr;
        complex_s32_t expected;
        unsigned line;
    } test_case_t;

    test_case_t casses[] = {
        //{{   val.b.re,   val.b.im}, {   val.c.re,   val.c.im} }, shr{   b,   c}, {     exp.re,     exp.im}, line num
        // { {{ 0x00008000, 0x00000000}, { 0x00010000, 0x00000000} },    {   0,   0}, { 0x00000002, 0x00000000}, __LINE__}, // 2.0
        // { {{ 0x00008000, 0x00000000}, { 0x00008000, 0x00000000} },    {   0,   0}, { 0x00000001, 0x00000000}, __LINE__}, // 1.0
        // { {{ 0x00004000, 0x00000000}, { 0x00008000, 0x00000000} },    {   0,   0}, { 0x00000001, 0x00000000}, __LINE__}, // 0.5
        // { {{ 0x00003FFF, 0x00000000}, { 0x00008000, 0x00000000} },    {   0,   0}, { 0x00000000, 0x00000000}, __LINE__}, // 0.5 - 1LSb
        
        // { {{-0x00008000, 0x00000000}, { 0x00010000, 0x00000000} },    {   0,   0}, {-0x00000002, 0x00000000}, __LINE__}, // -2.0
        // { {{-0x00008000, 0x00000000}, { 0x00008000, 0x00000000} },    {   0,   0}, {-0x00000001, 0x00000000}, __LINE__}, // -1.0
        // { {{-0x00004000, 0x00000000}, { 0x00008000, 0x00000000} },    {   0,   0}, {-0x00000000, 0x00000000}, __LINE__}, // -0.5
        // { {{-0x00004001, 0x00000000}, { 0x00008000, 0x00000000} },    {   0,   0}, {-0x00000001, 0x00000000}, __LINE__}, // -0.5 - 1LSb

        
        // { {{ 0x0008000, 0x00000000}, { 0x00010000, 0x00000000} },    {   0,   0}, {  0x00000002, 0x00000000 }, __LINE__}, 
        // { {{ 0x0008000, 0x00000000}, { 0x00008000, 0x00000000} },    {   0,   0}, {  0x00000001, 0x00000000 }, __LINE__}, 
        // { {{ 0x0008000, 0x00000000}, { 0x00004000, 0x00000000} },    {   0,   0}, {  0x00000001, 0x00000000 }, __LINE__},  //(2^15 * 2^14 - 0 * 0)  / 2^30 = 2^29 / 2^30 -> 0.5 rounds to 1
        // { {{ 0x0000000, 0x00008000}, { 0x00000000, 0x00004000} },    {   0,   0}, { -0x00000001, 0x00000000 }, __LINE__},  //(0 * 0 - 2^15 * 2^14) / 2^30 =-2^29 / 2^30 -> -0.5 rounds to 0
        // { {{ 0x0000000,-0x00008000}, { 0x00000000, 0x00004000} },    {   0,   0}, {  0x00000000, 0x00000000 }, __LINE__},  //(0 * 0 - -2^15 * 2^14) / 2^30 = 2^29 / 2^30 -> 0.5 rounds to 1


        // { {{ 0x0000000,-0x00008000}, { 0x00000000, 0x00010000} },    {   0,   0}, {  0x00000002, 0x00000000 }, __LINE__}, 
        // { {{ 0x0000000,-0x00008000}, { 0x00000000, 0x00008000} },    {   0,   0}, {  0x00000001, 0x00000000 }, __LINE__}, 
        // { {{ 0x0000000,-0x00008000}, { 0x00000000, 0x00004000} },    {   0,   0}, {  0x00000001, 0x00000000 }, __LINE__},  // 0 * 0 - -2^15 * 2^14  -> 
        
        // { {{ -42426758, -6496}, {  -59150, -993822770} },    {   0,   0}, {      -3675,   39268917}, __LINE__}, 
        // { {{ -42426758, -6496}, {  -59150, -993822771} },    {   0,   0}, {      -3675,      39628}, __LINE__}, 
        // { {{ -678828114, -103936261}, {  -30286169, -508859176} },    {   4,   9}, {      -3675,      39628}, __LINE__}, 


        { {{ 0x00000000, 0x00000000}, { 0x00000000, 0x00000000} },    {   0,   0}, { 0x00000000, 0x00000000}, __LINE__},
        { {{ 0x00000001, 0x00000001}, { 0x00000000, 0x00000000} },    {   0,   0}, { 0x00000000, 0x00000000}, __LINE__},
        { {{ 0x00000000, 0x00000000}, { 0x00000001, 0x00000001} },    { -20, -20}, { 0x00000000, 0x00000000}, __LINE__},
        { {{ 0x00000001, 0x00000000}, { 0x00000001, 0x00000000} },    { -15, -15}, { 0x00000001, 0x00000000}, __LINE__},
        { {{ 0x00000001, 0x00000000}, { 0x00000001, 0x00000000} },    { -16, -15}, { 0x00000002, 0x00000000}, __LINE__},
        { {{ 0x00000001, 0x00000000}, { 0x00000001, 0x00000000} },    { -15, -16}, { 0x00000002, 0x00000000}, __LINE__},
        { {{ 0x00000001, 0x00000000}, { 0x00000000, 0x00000001} },    { -15, -15}, { 0x00000000, 0x00000001}, __LINE__},
        { {{ 0x00000001, 0x00000001}, { 0x00000001, 0x00000000} },    { -15, -15}, { 0x00000001, 0x00000001}, __LINE__},
        { {{ 0x00000010, 0x00000020}, { 0x00000030, 0x00000040} },    { -17, -17}, {-0x00005000, 0x0000a000}, __LINE__},
        { {{ 0x40000000, 0x00000000}, { 0x40000000, 0x00000000} },    {   0,   0}, { 0x40000000, 0x00000000}, __LINE__},
        { {{ 0x00000000, 0x40000000}, { 0x00000000, 0x40000000} },    {   0,   0}, {-0x40000000, 0x00000000}, __LINE__},
    };

    const unsigned N_cases = sizeof(casses)/sizeof(test_case_t);

    const unsigned start_case = 0;

    for(int v = start_case; v < N_cases; v++){
        setExtraInfo_R(v);
        
        test_case_t* casse = &casses[v];

        //Verify mul__complexs32() is correct. It's used in other test cases.
        complex_s32_t tmp = mul_complex_s32(casse->value.b, casse->value.c, casse->shr.b, casse->shr.c);


        TEST_ASSERT_EQUAL_MSG(casse->expected.re, tmp.re, casse->line);
        TEST_ASSERT_EQUAL_MSG(casse->expected.im, tmp.im, casse->line);

        unsigned lengths[] = {1, 4, 16, 32, 40 };


        for( int l = 0; l < sizeof(lengths)/sizeof(lengths[0]); l++){
            unsigned len = lengths[l];

            headroom_t hr;
            complex_s32_t A[40];
            complex_s32_t B[40];
            complex_s32_t C[40];

            for(int i = 0; i < len; i++){
                A[i].re = 0xCC;
                A[i].im = 0xCC;
                B[i].re = casse->value.b.re;
                B[i].im = casse->value.b.im;
                C[i].re = casse->value.c.re;
                C[i].im = casse->value.c.im;
            }

            hr = xs3_vect_complex_s32_mul(A, B, C, len, casse->shr.b, casse->shr.c);

            for(int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected.re, A[0].re, casse->line);
                TEST_ASSERT_EQUAL_MSG(casse->expected.im, A[0].im, casse->line);
            }
            TEST_ASSERT_EQUAL_MSG(xs3_vect_s32_headroom((int32_t*) A, 2*len), hr, casse->line);

            memcpy(A, B, sizeof(A));
            hr = xs3_vect_complex_s32_mul(A, A, C, len, casse->shr.b, casse->shr.c);

            for(int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected.re, A[0].re, casse->line);
                TEST_ASSERT_EQUAL_MSG(casse->expected.im, A[0].im, casse->line);
            }
            TEST_ASSERT_EQUAL_MSG(xs3_vect_s32_headroom((int32_t*) A, 2*len), hr, casse->line);

            memcpy(A, C, sizeof(A));
            hr = xs3_vect_complex_s32_mul(A, B, A, len, casse->shr.b, casse->shr.c);

            for(int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected.re, A[0].re, casse->line);
                TEST_ASSERT_EQUAL_MSG(casse->expected.im, A[0].im, casse->line);
            }
            TEST_ASSERT_EQUAL_MSG(xs3_vect_s32_headroom((int32_t*) A, 2*len), hr, casse->line);
        }
    }
}


#if SMOKE_TEST
#  define REPS       (100)
#  define MAX_LEN    (64)
#else
#  define REPS       (1000)
#  define MAX_LEN    (256)
#endif

TEST(xs3_vect_complex_mul, xs3_vect_complex_s32_mul_random)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    headroom_t hr;
    
    complex_s32_t A[MAX_LEN];
    complex_s32_t B[MAX_LEN];
    complex_s32_t C[MAX_LEN];

    for(int v = 0; v < REPS; v++){

        setExtraInfo_RS(v, seed);

        unsigned len = (pseudo_rand_uint32(&seed) % MAX_LEN) + 1;
        
        for(int i = 0; i < len; i++){
            unsigned shr = pseudo_rand_uint32(&seed) % 8;
            B[i].re = pseudo_rand_int32(&seed) >> shr;
            C[i].re = pseudo_rand_int32(&seed) >> shr;
            B[i].im = pseudo_rand_int32(&seed) >> shr;
            C[i].im = pseudo_rand_int32(&seed) >> shr;
        }

        int b_shr = (pseudo_rand_uint32(&seed) % 10);
        int c_shr = (pseudo_rand_uint32(&seed) % 10);
        
        hr = xs3_vect_complex_s32_mul(A, B, C, len, b_shr, c_shr);

        // headroom_t hrre, hrim;

        for(int i = 0; i < len; i++){
            complex_s32_t expected = mul_complex_s32(B[i], C[i], b_shr, c_shr);
            TEST_ASSERT_EQUAL_MESSAGE(expected.re, A[i].re, msg_buff);
            TEST_ASSERT_EQUAL_MESSAGE(expected.im, A[i].im, msg_buff);
        }
        TEST_ASSERT_EQUAL_MSG(xs3_vect_s32_headroom((int32_t*) A, 2*len), hr, v);
        
        memcpy(&A, &B, sizeof(A));
        hr = xs3_vect_complex_s32_mul(A, A, C, len, b_shr, c_shr);

        for(int i = 0; i < len; i++){
            complex_s32_t expected = mul_complex_s32(B[i], C[i], b_shr, c_shr);
            TEST_ASSERT_EQUAL_MESSAGE(expected.re, A[i].re, msg_buff);
            TEST_ASSERT_EQUAL_MESSAGE(expected.im, A[i].im, msg_buff);
        }
        TEST_ASSERT_EQUAL_MSG(xs3_vect_s32_headroom((int32_t*) A, 2*len), hr, v);
        
        memcpy(&A, &C, sizeof(A));
        hr = xs3_vect_complex_s32_mul(A, B, A, len, b_shr, c_shr);

        for(int i = 0; i < len; i++){
            complex_s32_t expected = mul_complex_s32(B[i], C[i], b_shr, c_shr);
            TEST_ASSERT_EQUAL_MESSAGE(expected.re, A[i].re, msg_buff);
            TEST_ASSERT_EQUAL_MESSAGE(expected.im, A[i].im, msg_buff);
        }
        TEST_ASSERT_EQUAL_MSG(xs3_vect_s32_headroom((int32_t*) A, 2*len), hr, v);

        
    }
}
#undef MAX_LEN
#undef REPS

