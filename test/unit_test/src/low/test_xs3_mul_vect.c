
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xs3_math.h"

#include "../tst_common.h"

#include "unity.h"


static unsigned seed = 2314567;
static char msg_buff[200];

#define TEST_ASSERT_EQUAL_MSG(EXPECTED, ACTUAL, LINE_NUM)   do{       \
    if((EXPECTED)!=(ACTUAL)) {                                        \
      sprintf(msg_buff, "(test vector @ line %u)", (LINE_NUM));       \
      TEST_ASSERT_EQUAL_MESSAGE((EXPECTED), (ACTUAL), msg_buff);      \
    }} while(0)


#if DEBUG_ON || 0
#undef DEBUG_ON
#define DEBUG_ON    (1)
#endif


static int16_t mul_s16(int16_t b, int16_t c, int b_shr, int c_shr)
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

    int32_t a = (bp * cp + (1<<13)) >> (14);

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



static void test_xs3_mul_vect_s16_basic()
{
    PRINTF("%s...\n", __func__);

    typedef struct {
        struct {    int16_t b;  int16_t c;  } value;
        struct {    int b;      int c;      } shr;
        int16_t expected;
        unsigned line;
    } test_case_t;

    test_case_t casses[] = {
        // value{       b         c }   shr{  b   c }        exp        line num
        {       {  0x0000,   0x0000 },     {  0,  0 },    0x0000,       __LINE__},
        {       {  0x0001,   0x0000 },     {  0,  0 },    0x0000,       __LINE__},
        {       {  0x0000,   0x0001 },     {  0,  0 },    0x0000,       __LINE__},
        {       {  0x4000,   0x4000 },     {  0,  0 },    0x4000,       __LINE__},
        {       { -0x4000,   0x4000 },     {  0,  0 },   -0x4000,       __LINE__},
        {       { -0x4000,  -0x4000 },     {  0,  0 },    0x4000,       __LINE__},
        {       {  0x4000,   0x0001 },     {  0,  0 },    0x0001,       __LINE__},
        {       {  0x0001,   0x4000 },     {  0,  0 },    0x0001,       __LINE__},
        {       {  0x4000,   0x0123 },     {  0,  0 },    0x0123,       __LINE__},
        {       {  0x0123,   0x4000 },     {  0,  0 },    0x0123,       __LINE__},
        {       {  0x0040,   0x0200 },     {  0,  0 },    0x0002,       __LINE__},
        {       {  0x0040,   0x0100 },     {  0,  0 },    0x0001,       __LINE__},
        {       {  0x0040,   0x0080 },     {  0,  0 },    0x0001,       __LINE__},
        {       {  0x0040,   0x0040 },     {  0,  0 },    0x0000,       __LINE__},
        {       {  0x7f00,   0x7f00 },     {  0,  0 },    0x7fff,       __LINE__},
        {       {  0x7f00,  -0x7f00 },     {  0,  0 },   -0x7fff,       __LINE__},
        {       { -0x8000,   0x4000 },     {  0,  0 },   -0x7fff,       __LINE__},
        
        {       {  0x4000,   0x4000 },     {  1,  0 },    0x2000,       __LINE__},
        {       {  0x4000,   0x4000 },     {  0,  1 },    0x2000,       __LINE__},
        {       {  0x4000,   0x4000 },     {  1,  1 },    0x1000,       __LINE__},
        {       {  0x4000,   0x4000 },     {  3,  5 },    0x0040,       __LINE__},

        {       {  0x4000,   0x0800 },     {  0, -1 },    0x1000,       __LINE__},
        {       {  0x4000,   0x0800 },     {  0, -2 },    0x2000,       __LINE__},
        {       {  0x4000,   0x0800 },     {  0, -3 },    0x4000,       __LINE__},
        {       {  0x4000,   0x0800 },     {  0, -4 },    0x7fff,       __LINE__},
        {       { -0x4000,   0x0800 },     {  0, -4 },   -0x7fff,       __LINE__},
        {       {  0x4000,   0x0800 },     {  1, -4 },    0x4000,       __LINE__},

        
    };

    const unsigned N_cases = sizeof(casses)/sizeof(test_case_t);

    const unsigned start_case = 0;

    for(int v = start_case; v < N_cases; v++){
        PRINTF("\ttest vector %d..\n", v);
        
        test_case_t* casse = &casses[v];
        
        //Verify mul_s16() is correct. It's used in other test cases.
        TEST_ASSERT_EQUAL_MSG(casse->expected, mul_s16(casse->value.b, casse->value.c, casse->shr.b, casse->shr.c), casse->line);

        unsigned lengths[] = {1, 4, 16, 32, 40 };

        for( int l = 0; l < sizeof(lengths)/sizeof(lengths[0]); l++){
            unsigned len = lengths[l];

            // PRINTF("\t\tlength %u\n", len);

            headroom_t hr;
            int16_t A[40];
            int16_t B[40];
            int16_t C[40];

            for(int i = 0; i < len; i++){
                A[i] = 0xCC;
                B[i] = casse->value.b;
                C[i] = casse->value.c;
            }


            hr = xs3_mul_vect_s16(A, B, C, len, casse->shr.b, casse->shr.c);

            for(int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(xs3_headroom_vect_s16(A, len), hr, casse->line);
            }

            memcpy(A, B, sizeof(A));
            hr = xs3_mul_vect_s16(A, A, C, len, casse->shr.b, casse->shr.c);

            for(int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(xs3_headroom_vect_s16(A, len), hr, casse->line);
            }

            memcpy(A, C, sizeof(A));
            hr = xs3_mul_vect_s16(A, B, A, len, casse->shr.b, casse->shr.c);

            for(int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(xs3_headroom_vect_s16(A, len), hr, casse->line);
            }

        }
    }
}


#define MAX_LEN     100
#define REPS        IF_QUICK_TEST(10, 100)
static void test_xs3_mul_vect_s16_random()
{
    PRINTF("%s...\n", __func__);
    seed = 998754;

    headroom_t hr;
    int16_t A[MAX_LEN];
    int16_t B[MAX_LEN];
    int16_t C[MAX_LEN];

    for(int v = 0; v < REPS; v++){

        PRINTF("\trepetition %d..\n", v);

        unsigned len = (pseudo_rand_uint32(&seed) % MAX_LEN) + 1;
        
        for(int i = 0; i < len; i++){
            unsigned shr = pseudo_rand_uint32(&seed) % 8;
            B[i] = pseudo_rand_int16(&seed) >> shr;
            C[i] = pseudo_rand_int16(&seed) >> shr;
        }

        int b_shr = (pseudo_rand_uint32(&seed) % 5) - 2;
        int c_shr = (pseudo_rand_uint32(&seed) % 5) - 2;
        
        const char sprintpat[] = "rep(%d)[%d of %u]: %d <-- ((%d >> %d) * (%d >> %d)) >> 14     (A[i]=0x%04X; B[i]=0x%04X; C[i]=0x%04X)";

        hr = xs3_mul_vect_s16(A, B, C, len, b_shr, c_shr);

        for(int i = 0; i < len; i++){
            int16_t expected = mul_s16(B[i], C[i], b_shr, c_shr);
            if(expected != A[i]) sprintf(msg_buff, sprintpat,v, i, len, A[i], B[i], b_shr, C[i], c_shr, (uint16_t)A[i], (uint16_t)B[i],  (uint16_t)C[i]);
            TEST_ASSERT_EQUAL_MESSAGE(expected, A[i], msg_buff);
        }
        TEST_ASSERT_EQUAL(xs3_headroom_vect_s16(A, len), hr);
        
        memcpy(A, B, sizeof(A[0])*len);
        hr = xs3_mul_vect_s16(A, A, C, len, b_shr, c_shr);

        for(int i = 0; i < len; i++){
            int16_t expected = mul_s16(B[i], C[i], b_shr, c_shr);
            if(expected != A[i]) sprintf(msg_buff, sprintpat,v, i, len, A[i], B[i], b_shr, C[i], c_shr, (uint16_t)A[i],  (uint16_t)B[i],  (uint16_t)C[i]);
            TEST_ASSERT_EQUAL_MESSAGE(expected, A[i], msg_buff);
        }
        TEST_ASSERT_EQUAL(xs3_headroom_vect_s16(A, len), hr);
        
        memcpy(A, C, sizeof(A[0])*len);
        hr = xs3_mul_vect_s16(A, B, A, len, b_shr, c_shr);

        for(int i = 0; i < len; i++){
            int16_t expected = mul_s16(B[i], C[i], b_shr, c_shr);
            if(expected != A[i]) sprintf(msg_buff, sprintpat,v, i, len, A[i], B[i], b_shr, C[i], c_shr, (uint16_t)A[i],  (uint16_t)B[i],  (uint16_t)C[i]);
            TEST_ASSERT_EQUAL_MESSAGE(expected, A[i], msg_buff);
        }
        TEST_ASSERT_EQUAL(xs3_headroom_vect_s16(A, len), hr);
        
    }
}
#undef MAX_LEN
#undef REPS


static void test_xs3_mul_vect_s32_basic()
{
    PRINTF("%s...\n", __func__);

    typedef struct {
        struct {    int32_t b;  int32_t c;  } value;
        struct {    int b;      int c;      } shr;
        int32_t expected;
        unsigned line;
    } test_case_t;

    test_case_t casses[] = {
        // value{           b             c }   shr{   b    c }            exp        line num
        {       {  0x00000000,   0x00000000 },     {  0,  0 },      0x00000000,       __LINE__},
        {       {  0x00010000,   0x00000000 },     {  0,  0 },      0x00000000,       __LINE__},
        {       {  0x00000000,   0x00010000 },     {  0,  0 },      0x00000000,       __LINE__},
        {       {  0x40000000,   0x40000000 },     {  0,  0 },      0x40000000,       __LINE__},
        {       { -0x40000000,   0x40000000 },     {  0,  0 },     -0x40000000,       __LINE__},
        {       { -0x40000000,  -0x40000000 },     {  0,  0 },      0x40000000,       __LINE__},
        {       {  0x40000000,   0x00010000 },     {  0,  0 },      0x00010000,       __LINE__},
        {       {  0x00010000,   0x40000000 },     {  0,  0 },      0x00010000,       __LINE__},
        {       {  0x40000000,   0x01230000 },     {  0,  0 },      0x01230000,       __LINE__},
        {       {  0x01230000,   0x40000000 },     {  0,  0 },      0x01230000,       __LINE__},
        {       {  0x00400000,   0x02000000 },     {  0,  0 },      0x00020000,       __LINE__},
        {       {  0x00400000,   0x01000000 },     {  0,  0 },      0x00010000,       __LINE__},
        {       {  0x00004000,   0x00010000 },     {  0,  0 },      0x00000001,       __LINE__},
        {       {  0x00004000,   0x00008000 },     {  0,  0 },      0x00000001,       __LINE__},
        {       {  0x00000400,   0x00000400 },     {  0,  0 },      0x00000000,       __LINE__},
        {       {  0x7f000000,   0x7f000000 },     {  0,  0 },      0x7fffffff,       __LINE__},
        {       {  0x7f000000,  -0x7f000000 },     {  0,  0 },     -0x7fffffff,       __LINE__},
        {       { -0x80000000,   0x40000000 },     {  0,  0 },     -0x7fffffff,       __LINE__},
        {       {  0x40000000,   0x40000000 },     {  1,  0 },      0x20000000,       __LINE__},
        {       {  0x40000000,   0x40000000 },     {  0,  1 },      0x20000000,       __LINE__},
        {       {  0x40000000,   0x40000000 },     {  1,  1 },      0x10000000,       __LINE__},
        {       {  0x40000000,   0x40000000 },     {  3,  5 },      0x00400000,       __LINE__},
        {       {  0x40000000,   0x08000000 },     {  0, -1 },      0x10000000,       __LINE__},
        {       {  0x40000000,   0x08000000 },     {  0, -2 },      0x20000000,       __LINE__},
        {       {  0x40000000,   0x08000000 },     {  0, -3 },      0x40000000,       __LINE__},
        {       {  0x40000000,   0x08000000 },     {  0, -4 },      0x7fffffff,       __LINE__},
        {       { -0x40000000,   0x08000000 },     {  0, -4 },     -0x7fffffff,       __LINE__},
        {       {  0x40000000,   0x08000000 },     {  1, -4 },      0x40000000,       __LINE__},
    };

    const unsigned N_cases = sizeof(casses)/sizeof(test_case_t);

    const unsigned start_case = 0;

    for(int v = start_case; v < N_cases; v++){
        PRINTF("\ttest vector %d..\n", v);
        
        test_case_t* casse = &casses[v];

        //Verify mul_s32() is correct. It's used in other test cases.
        TEST_ASSERT_EQUAL_MSG(casse->expected, mul_s32(casse->value.b, casse->value.c, casse->shr.b, casse->shr.c), casse->line);

        unsigned lengths[] = {1, 4, 16, 32, 40 };
        

        for( int l = 0; l < sizeof(lengths)/sizeof(lengths[0]); l++){
            unsigned len = lengths[l];

            // PRINTF("\t\tlength %u\n", len);

            headroom_t hr;
            int32_t A[40];
            int32_t B[40];
            int32_t C[40];

            for(int i = 0; i < len; i++){
                A[i] = 0xCC;
                B[i] = casse->value.b;
                C[i] = casse->value.c;
            }

            hr = xs3_mul_vect_s32(A, B, C, len, casse->shr.b, casse->shr.c);

            for(int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(xs3_headroom_vect_s32(A, len), hr, casse->line);
            }

            memcpy(A, B, sizeof(A));
            hr = xs3_mul_vect_s32(A, A, C, len, casse->shr.b, casse->shr.c);

            for(int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(xs3_headroom_vect_s32(A, len), hr, casse->line);
            }

            memcpy(A, C, sizeof(A));
            hr = xs3_mul_vect_s32(A, B, A, len, casse->shr.b, casse->shr.c);

            for(int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(xs3_headroom_vect_s32(A, len), hr, casse->line);
            }
        }
    }
}





#define MAX_LEN     100
#define REPS        IF_QUICK_TEST(10, 100)
static void test_xs3_mul_vect_s32_random()
{
    PRINTF("%s...\n", __func__);
    seed = 67456;

    headroom_t hr;
    int32_t A[MAX_LEN];
    int32_t B[MAX_LEN];
    int32_t C[MAX_LEN];

    for(int v = 0; v < REPS; v++){

        PRINTF("\trepetition %d..\n", v);

        unsigned len = (pseudo_rand_uint32(&seed) % MAX_LEN) + 1;
        
        for(int i = 0; i < len; i++){
            unsigned shr = pseudo_rand_uint32(&seed) % 8;
            B[i] = pseudo_rand_int32(&seed) >> shr;
            C[i] = pseudo_rand_int32(&seed) >> shr;
        }

        int b_shr = (pseudo_rand_uint32(&seed) % 5) - 2;
        int c_shr = (pseudo_rand_uint32(&seed) % 5) - 2;
        
        const char sprintpat[] = "rep(%d)[%d of %u]: %ld <-- ((%ld >> %d) * (%ld >> %d)) >> 30     (A[i]=0x%08X; B[i]=0x%08X; C[i]=0x%08X)";

        hr = xs3_mul_vect_s32(A, B, C, len, b_shr, c_shr);

        for(int i = 0; i < len; i++){
            int32_t expected = mul_s32(B[i], C[i], b_shr, c_shr);
            if(expected != A[i]) sprintf(msg_buff, sprintpat,v, i, len, A[i], B[i], b_shr, C[i], c_shr, (unsigned)A[i], (unsigned)B[i],  (unsigned)C[i]);
            TEST_ASSERT_EQUAL_MESSAGE(expected, A[i], msg_buff);
        }
        TEST_ASSERT_EQUAL(xs3_headroom_vect_s32(A, len), hr);
        
        memcpy(A, B, sizeof(A[0])*len);
        hr = xs3_mul_vect_s32(A, A, C, len, b_shr, c_shr);

        for(int i = 0; i < len; i++){
            int32_t expected = mul_s32(B[i], C[i], b_shr, c_shr);
            if(expected != A[i]) sprintf(msg_buff, sprintpat,v, i, len, A[i], B[i], b_shr, C[i], c_shr, (unsigned)A[i],  (unsigned)B[i],  (unsigned)C[i]);
            TEST_ASSERT_EQUAL_MESSAGE(expected, A[i], msg_buff);
        }
        TEST_ASSERT_EQUAL(xs3_headroom_vect_s32(A, len), hr);
        
        memcpy(A, C, sizeof(A[0])*len);
        hr = xs3_mul_vect_s32(A, B, A, len, b_shr, c_shr);

        for(int i = 0; i < len; i++){
            int32_t expected = mul_s32(B[i], C[i], b_shr, c_shr);
            if(expected != A[i]) sprintf(msg_buff, sprintpat,v, i, len, A[i], B[i], b_shr, C[i], c_shr, (unsigned)A[i],  (unsigned)B[i],  (unsigned)C[i]);
            TEST_ASSERT_EQUAL_MESSAGE(expected, A[i], msg_buff);
        }
        TEST_ASSERT_EQUAL(xs3_headroom_vect_s32(A, len), hr);
        
    }
}
#undef MAX_LEN
#undef REPS


int test_xs3_mul_vect()
{
    int retval = 0;
    
    UnityBegin(__FILE__);

    RUN_TEST(test_xs3_mul_vect_s16_basic);
    RUN_TEST(test_xs3_mul_vect_s16_random);

    RUN_TEST(test_xs3_mul_vect_s32_basic);
    RUN_TEST(test_xs3_mul_vect_s32_random);

    retval = UnityEnd();
    

    return retval;
}