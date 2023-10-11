// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xmath/xmath.h"

#include "../tst_common.h"

#include "unity_fixture.h"


TEST_GROUP_RUNNER(vect_add) {
  RUN_TEST_CASE(vect_add, vect_s32_add_prepare);
  RUN_TEST_CASE(vect_add, vect_s16_add_basic);
  RUN_TEST_CASE(vect_add, vect_s16_add_random);
  RUN_TEST_CASE(vect_add, vect_s32_add_basic);
  RUN_TEST_CASE(vect_add, vect_s32_add_random);
}

TEST_GROUP(vect_add);
TEST_SETUP(vect_add) { fflush(stdout); }
TEST_TEAR_DOWN(vect_add) {}


static char msg_buff[200];

#define TEST_ASSERT_EQUAL_MSG(EXPECTED, ACTUAL, LINE_NUM)   do{       \
    if((EXPECTED)!=(ACTUAL)) {                                        \
      sprintf(msg_buff, "(test vector @ line %u)", (LINE_NUM));       \
      TEST_ASSERT_EQUAL_MESSAGE((EXPECTED), (ACTUAL), msg_buff);      \
    }} while(0)


static int16_t add_s16(int16_t b, int16_t c, int b_shr, int c_shr)
{
    int32_t bp = b;
    int32_t cp = c;

    bp = (b_shr >= 0)? bp >> b_shr : bp << (-b_shr);
    cp = (c_shr >= 0)? cp >> c_shr : cp << (-c_shr);

    bp = (bp >= VPU_INT16_MAX)? VPU_INT16_MAX : (bp <= VPU_INT16_MIN)? VPU_INT16_MIN : bp;
    bp = ( (b_shr > 0) && (b < 0) && (-(1<<b_shr)) < b)? -1 : bp;
    assert(b >= 0 || bp < 0);
    
    cp = (cp >= VPU_INT16_MAX)? VPU_INT16_MAX : (cp <= VPU_INT16_MIN)? VPU_INT16_MIN : cp;
    cp = ( (c_shr > 0) && (c < 0) && (-(1<<c_shr)) < c)? -1 : cp;
    assert(c >= 0 || cp < 0);

    int32_t a = bp + cp;

    a = (a >= VPU_INT16_MAX)? VPU_INT16_MAX : (a <= VPU_INT16_MIN)? VPU_INT16_MIN : a;

    return (int16_t) a;
}

static int32_t add_s32(int32_t b, int32_t c, int b_shr, int c_shr)
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

    int64_t a = bp + cp;

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


TEST(vect_add, vect_s32_add_prepare)
{
    


    typedef struct {

        struct {    headroom_t hr;  exponent_t exp;     } b;
        struct {    headroom_t hr;  exponent_t exp;     } c;
        struct {    exponent_t exp; right_shift_t b_shr; right_shift_t c_shr;     } expected;

        unsigned line;
    } test_case_t;


    test_case_t cases[] = {
        //    b{   hr,  exp },      c{   hr,  exp },   expected{   exp,  b_shr, c_shr }
        {      {    0,    0 },       {    0,    0 },           {     1,      1,    1  },       __LINE__ },
        {      {    1,    5 },       {    1,    5 },           {     5,      0,    0  },       __LINE__ },
        {      {    1,    0 },       {    0,    0 },           {     1,      1,    1  },       __LINE__ },
        {      {    0,   10 },       {    1,   10 },           {    11,      1,    1  },       __LINE__ },
        {      {   15,   -4 },       {   15,   -4 },           {   -18,    -14,  -14  },       __LINE__ },
        {      {    8,  -14 },       {    4,   -4 },           {    -7,      7,   -3  },       __LINE__ },
        {      {    8,  -14 },       {    4,   -4 },           {    -7,      7,   -3  },       __LINE__ },

    };

    const unsigned N_cases = sizeof(cases)/sizeof(test_case_t);

    const unsigned start_case = 0;

    for(unsigned int v = start_case; v < N_cases; v++){
        setExtraInfo_R(v);
        
        test_case_t* casse = &cases[v];

        exponent_t a_exp;
        right_shift_t b_shr, c_shr;

        int exp_mods[] = {0, 44, -534};

        for( int m = 0; m < sizeof(exp_mods)/sizeof(int); m++){

            //printf("\t\t\texp_delta = %d\n", exp_mods[m]);
    

            for(int sbc = 0; sbc <= 1; sbc++){

                //printf("\t\t\t\tswap b&c: %d\n", sbc);

                if(sbc){
                    vect_s32_add_prepare(&a_exp, &c_shr, &b_shr,
                                                casse->c.exp + exp_mods[m], casse->b.exp + exp_mods[m],
                                                casse->c.hr,  casse->b.hr);
                } else {
                    vect_s32_add_prepare(&a_exp, &b_shr, &c_shr,
                                                casse->b.exp + exp_mods[m], casse->c.exp + exp_mods[m],
                                                casse->b.hr,  casse->c.hr);
                }

                exponent_t expected_exp = casse->expected.exp + exp_mods[m];

                TEST_ASSERT_EQUAL_MSG(expected_exp,   a_exp, casse->line);
                TEST_ASSERT_EQUAL_MSG(casse->expected.b_shr, b_shr, casse->line);
                TEST_ASSERT_EQUAL_MSG(casse->expected.c_shr, c_shr, casse->line);
            }
        }

    }
}


TEST(vect_add, vect_s16_add_basic)
{
    

    typedef struct {
        struct {    int16_t b;  int16_t c;  } value;
        struct {    int b;      int c;      } shr;
        int16_t expected;
        unsigned line;
    } test_case_t;

    test_case_t cases[] = {
        // value{       b         c }   shr{  b   c }        exp        line num
        {       {  0x0000,   0x0000 },     {  0,  0 },    0x0000,       __LINE__},
        {       {  0x0001,   0x0000 },     {  0,  0 },    0x0001,       __LINE__},
        {       {  0x0000,   0x0001 },     {  0,  0 },    0x0001,       __LINE__},
        {       {  0x0001,   0x0001 },     {  0,  0 },    0x0002,       __LINE__},
        {       { -0x0001,   0x0001 },     {  0,  0 },    0x0000,       __LINE__},
        {       {  0x1010,   0x0101 },     {  0,  0 },    0x1111,       __LINE__},
        {       { -0x1010,  -0x0101 },     {  0,  0 },   -0x1111,       __LINE__},
        {       { -0x8000,   0x0000 },     {  0,  0 },   -0x7FFF,       __LINE__},
        {       {  0x4000,   0x4000 },     {  0,  0 },    0x7FFF,       __LINE__},
        {       {  0x7FFF,   0x7FFF },     {  0,  0 },    0x7FFF,       __LINE__},

        {       {  0x0001,   0x0001 },     {  1,  0 },    0x0001,       __LINE__},
        {       {  0x0001,   0x0001 },     {  1,  1 },    0x0000,       __LINE__},
        {       {  0x0002,   0x0002 },     {  1,  1 },    0x0002,       __LINE__},
        {       {  0x0010,   0x0020 },     {  1,  1 },    0x0018,       __LINE__},
        {       {  0x0011,   0x0021 },     {  4,  5 },    0x0002,       __LINE__},
        {       { -0x0010,  -0x0020 },     {  4,  4 },   -0x0003,       __LINE__},
        {       { -0x0011,   0x0000 },     {  1,  0 },   -0x0009,       __LINE__}, //truncation of negative rounds towards negative infinity
        {       { -0x0001,  -0x0001 },     { 10, 10 },   -0x0002,       __LINE__},

        {       {  0x0001,   0x0001 },     { -1,  0 },    0x0003,       __LINE__},
        {       {  0x0001,   0x0001 },     {  0, -1 },    0x0003,       __LINE__},
        {       {  0x0001,   0x0001 },     { -1, -1 },    0x0004,       __LINE__},
        {       {  0x0010,   0x0020 },     { -1, -1 },    0x0060,       __LINE__},
        {       {  0x0010,   0x0020 },     { -8, -8 },    0x3000,       __LINE__},
        {       {  0x0FFF,  -0x0FFF },     { -4,  0 },    0x7000,       __LINE__},
        
    };

    const unsigned N_cases = sizeof(cases)/sizeof(test_case_t);

    const unsigned start_case = 0;

    for(unsigned int v = start_case; v < N_cases; v++){
        setExtraInfo_R(v);
        
        test_case_t* casse = &cases[v];
        
        //Verify add_s16() is correct. It's used in other test cases.
        TEST_ASSERT_EQUAL_MSG(casse->expected, add_s16(casse->value.b, casse->value.c, casse->shr.b, casse->shr.c), casse->line);

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


            hr = vect_s16_add(A, B, C, len, casse->shr.b, casse->shr.c);

            for(unsigned int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(vect_s16_headroom(A, len), hr, casse->line);
            }

            memcpy(A, B, sizeof(A));
            hr = vect_s16_add(A, A, C, len, casse->shr.b, casse->shr.c);

            for(unsigned int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(vect_s16_headroom(A, len), hr, casse->line);
            }

            memcpy(A, C, sizeof(A));
            hr = vect_s16_add(A, B, A, len, casse->shr.b, casse->shr.c);

            for(unsigned int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(vect_s16_headroom(A, len), hr, casse->line);
            }

        }
    }
}


TEST(vect_add, vect_s16_add_random)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    headroom_t hr;
    int16_t A[MAX_LEN];
    int16_t B[MAX_LEN];
    int16_t C[MAX_LEN];

    for(unsigned int v = 0; v < REPS; v++){

        setExtraInfo_R(v);

        unsigned len = (pseudo_rand_uint32(&seed) % MAX_LEN) + 1;
        
        for(unsigned int i = 0; i < len; i++){
            unsigned shr = pseudo_rand_uint32(&seed) % 8;
            B[i] = pseudo_rand_int16(&seed) >> shr;
            C[i] = pseudo_rand_int16(&seed) >> shr;
        }

        int b_shr = (pseudo_rand_uint32(&seed) % 5) - 2;
        int c_shr = (pseudo_rand_uint32(&seed) % 5) - 2;
        
        const char sprintpat[] = "rep(%d)[%d of %u]: %d <-- ((%d >> %d) + (%d >> %d))     (A[i]=0x%04X; B[i]=0x%04X; C[i]=0x%04X)";

        hr = vect_s16_add(A, B, C, len, b_shr, c_shr);

        for(unsigned int i = 0; i < len; i++){
            int16_t expected = add_s16(B[i], C[i], b_shr, c_shr);
            if(expected != A[i]) sprintf(msg_buff, sprintpat,v, i, len, A[i], B[i], b_shr, C[i], c_shr, (uint16_t)A[i], (uint16_t)B[i],  (uint16_t)C[i]);
            TEST_ASSERT_EQUAL_MESSAGE(expected, A[i], msg_buff);
        }
        TEST_ASSERT_EQUAL(vect_s16_headroom(A, len), hr);
        
        memcpy(A, B, sizeof(A[0])*len);
        hr = vect_s16_add(A, A, C, len, b_shr, c_shr);

        for(unsigned int i = 0; i < len; i++){
            int16_t expected = add_s16(B[i], C[i], b_shr, c_shr);
            if(expected != A[i]) sprintf(msg_buff, sprintpat,v, i, len, A[i], B[i], b_shr, C[i], c_shr, (uint16_t)A[i],  (uint16_t)B[i],  (uint16_t)C[i]);
            TEST_ASSERT_EQUAL_MESSAGE(expected, A[i], msg_buff);
        }
        TEST_ASSERT_EQUAL(vect_s16_headroom(A, len), hr);
        
        memcpy(A, C, sizeof(A[0])*len);
        hr = vect_s16_add(A, B, A, len, b_shr, c_shr);

        for(unsigned int i = 0; i < len; i++){
            int16_t expected = add_s16(B[i], C[i], b_shr, c_shr);
            if(expected != A[i]) sprintf(msg_buff, sprintpat,v, i, len, A[i], B[i], b_shr, C[i], c_shr, (uint16_t)A[i],  (uint16_t)B[i],  (uint16_t)C[i]);
            TEST_ASSERT_EQUAL_MESSAGE(expected, A[i], msg_buff);
        }
        TEST_ASSERT_EQUAL(vect_s16_headroom(A, len), hr);
        
    }
}


TEST(vect_add, vect_s32_add_basic)
{
    

    typedef struct {
        struct {    int32_t b;  int32_t c;  } value;
        struct {    int b;      int c;      } shr;
        int32_t expected;
        unsigned line;
    } test_case_t;

    test_case_t cases[] = {
        // value{                    b                c }   shr{   b    c }            exp        line num}
        {       {           0x00000000,      0x00000000 },     {   0,   0 },    0x00000000,       __LINE__},
        {       {           0x00000001,      0x00000000 },     {   0,   0 },    0x00000001,       __LINE__},
        {       {           0x00000000,      0x00000001 },     {   0,   0 },    0x00000001,       __LINE__},
        {       {           0x00000001,      0x00000001 },     {   0,   0 },    0x00000002,       __LINE__},
        {       {          -0x00000001,      0x00000001 },     {   0,   0 },    0x00000000,       __LINE__},
        {       {           0x00001010,      0x00000101 },     {   0,   0 },    0x00001111,       __LINE__},
        {       {          -0x00001010,     -0x00000101 },     {   0,   0 },   -0x00001111,       __LINE__},
        {       { (int) (0-0x80000000),      0x00000000 },     {   0,   0 },   -0x7FFFFFFF,       __LINE__},
        {       {           0x40000000,      0x40000000 },     {   0,   0 },    0x7FFFFFFF,       __LINE__},
        {       {           0x7FFFFFFF,      0x7FFFFFFF },     {   0,   0 },    0x7FFFFFFF,       __LINE__},

        {       {           0x00000001,      0x00000001 },     {   1,   0 },    0x00000001,       __LINE__},
        {       {           0x00000001,      0x00000001 },     {   1,   1 },    0x00000000,       __LINE__},
        {       {           0x00000002,      0x00000002 },     {   1,   1 },    0x00000002,       __LINE__},
        {       {           0x00000010,      0x00000020 },     {   1,   1 },    0x00000018,       __LINE__},
        {       {           0x00000011,      0x00000021 },     {   4,   5 },    0x00000002,       __LINE__},
        {       {          -0x00000010,     -0x00000020 },     {   4,   4 },   -0x00000003,       __LINE__},
        {       {          -0x00000011,      0x00000000 },     {   1,   0 },   -0x00000009,       __LINE__}, //truncation of negative rounds towards negative infinity
        {       {          -0x00000001,     -0x00000001 },     {  10,  10 },   -0x00000002,       __LINE__},

        {       {           0x00000001,      0x00000001 },     {  -1,   0 },    0x00000003,       __LINE__},
        {       {           0x00000001,      0x00000001 },     {   0,  -1 },    0x00000003,       __LINE__},
        {       {           0x00000001,      0x00000001 },     {  -1,  -1 },    0x00000004,       __LINE__},
        {       {           0x00000010,      0x00000020 },     {  -1,  -1 },    0x00000060,       __LINE__},
        {       {           0x00000010,      0x00000020 },     {  -8,  -8 },    0x00003000,       __LINE__},
        {       {           0x00000FFF,     -0x00000FFF },     { -20, -16 },    0x7000FFFF,       __LINE__},
    };

    const unsigned N_cases = sizeof(cases)/sizeof(test_case_t);

    const unsigned start_case = 0;

    for(unsigned int v = start_case; v < N_cases; v++){
        setExtraInfo_R(v);
        
        test_case_t* casse = &cases[v];

        //Verify add_s32() is correct. It's used in other test cases.
        TEST_ASSERT_EQUAL_MSG(casse->expected, add_s32(casse->value.b, casse->value.c, casse->shr.b, casse->shr.c), casse->line);

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

            hr = vect_s32_add(A, B, C, len, casse->shr.b, casse->shr.c);

            for(unsigned int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(vect_s32_headroom(A, len), hr, casse->line);
            }

            memcpy(A, B, sizeof(A));
            hr = vect_s32_add(A, A, C, len, casse->shr.b, casse->shr.c);

            for(unsigned int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(vect_s32_headroom(A, len), hr, casse->line);
            }

            memcpy(A, C, sizeof(A));
            hr = vect_s32_add(A, B, A, len, casse->shr.b, casse->shr.c);

            for(unsigned int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(vect_s32_headroom(A, len), hr, casse->line);
            }
        }
    }
}


TEST(vect_add, vect_s32_add_random)
{
    
    unsigned seed = SEED_FROM_FUNC_NAME();


    headroom_t hr;
    int32_t A[MAX_LEN];
    int32_t B[MAX_LEN];
    int32_t C[MAX_LEN];

    for(unsigned int v = 0; v < REPS; v++){

        setExtraInfo_R(v);

        unsigned len = (pseudo_rand_uint32(&seed) % MAX_LEN) + 1;
        
        for(unsigned int i = 0; i < len; i++){
            unsigned shr = pseudo_rand_uint32(&seed) % 8;
            B[i] = pseudo_rand_int32(&seed) >> shr;
            C[i] = pseudo_rand_int32(&seed) >> shr;
        }

        int b_shr = (pseudo_rand_uint32(&seed) % 5) - 2;
        int c_shr = (pseudo_rand_uint32(&seed) % 5) - 2;
        
        const char sprintpat[] = "rep(%d)[%d of %u]: %ld <-- ((%ld >> %d) + (%ld >> %d))     (A[i]=0x%08X; B[i]=0x%08X; C[i]=0x%08X)";

        hr = vect_s32_add(A, B, C, len, b_shr, c_shr);

        for(unsigned int i = 0; i < len; i++){
            int32_t expected = add_s32(B[i], C[i], b_shr, c_shr);
            if(expected != A[i]) sprintf(msg_buff, sprintpat,v, i, len, (long unsigned int) A[i], (long unsigned int) B[i], b_shr, (long unsigned int) C[i], c_shr, (unsigned int) A[i],  (unsigned int) B[i],  (unsigned int) C[i]);
            TEST_ASSERT_EQUAL_MESSAGE(expected, A[i], msg_buff);
        }
        TEST_ASSERT_EQUAL(vect_s32_headroom(A, len), hr);
        
        memcpy(A, B, sizeof(A[0])*len);
        hr = vect_s32_add(A, A, C, len, b_shr, c_shr);

        for(unsigned int i = 0; i < len; i++){
            int32_t expected = add_s32(B[i], C[i], b_shr, c_shr);
            if(expected != A[i]) sprintf(msg_buff, sprintpat,v, i, len, (long unsigned int) A[i], (long unsigned int) B[i], b_shr, (long unsigned int) C[i], c_shr, (unsigned int) A[i],  (unsigned int) B[i],  (unsigned int) C[i]);
            TEST_ASSERT_EQUAL_MESSAGE(expected, A[i], msg_buff);
        }
        TEST_ASSERT_EQUAL(vect_s32_headroom(A, len), hr);
        
        memcpy(A, C, sizeof(A[0])*len);
        hr = vect_s32_add(A, B, A, len, b_shr, c_shr);

        for(unsigned int i = 0; i < len; i++){
            int32_t expected = add_s32(B[i], C[i], b_shr, c_shr);
            if(expected != A[i]) sprintf(msg_buff, sprintpat, v, i, len, (long unsigned int) A[i], (unsigned long int) B[i], b_shr, (unsigned long int) C[i], c_shr, (unsigned int) A[i],  (unsigned int) B[i],  (unsigned int) C[i]);
            TEST_ASSERT_EQUAL_MESSAGE(expected, A[i], msg_buff);
        }
        TEST_ASSERT_EQUAL(vect_s32_headroom(A, len), hr);
        
    }
}

