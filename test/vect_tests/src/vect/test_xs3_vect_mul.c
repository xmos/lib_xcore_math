
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xs3_math.h"
#include "xs3_vpu_scalar_ops.h"

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


static int16_t mul_s16(int16_t b, int16_t c, int a_shr)
{
    int32_t A = ((int32_t)b)*c;
    int32_t a = A;

    a = a + (1 << (a_shr-1));
    a = a >> a_shr;
    a = (a >= VPU_INT16_MAX)? VPU_INT16_MAX : (a <= VPU_INT16_MIN)? VPU_INT16_MIN : a;

    if(A < 0 && a == 0)
        a = -1;

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




static int16_t scalar_mul_s16(int16_t b, int16_t c, right_shift_t sat)
{
    return vlsat16( ((int32_t)b) * c  , sat);
}



static int32_t scalar_mul_s32(int32_t b, int32_t alpha, int b_shr)
{
    int64_t bp = b;
    int64_t cp = alpha;

    bp = (b_shr >= 0)? bp >> b_shr : bp << (-b_shr);

    bp = (bp >= VPU_INT32_MAX)? VPU_INT32_MAX : (bp <= VPU_INT32_MIN)? VPU_INT32_MIN : bp;
    bp = ( (b_shr > 0) && (b < 0) && (-(1<<b_shr)) < b)? -1 : bp;
    assert(b >= 0 || bp < 0);
    
    cp = (cp >= VPU_INT32_MAX)? VPU_INT32_MAX : (cp <= VPU_INT32_MIN)? VPU_INT32_MIN : cp;

    int64_t a = (bp * cp + (1<<29)) >> (30);

    a = (a >= VPU_INT32_MAX)? VPU_INT32_MAX : (a <= VPU_INT32_MIN)? VPU_INT32_MIN : a;

    return (int32_t) a;
}





#define REPS        1000
#define MAX_LEN     256




static void test_xs3_vect_s16_mul_prepare()
{
    PRINTF("%s...\n", __func__);

    seed = 88845643;

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);
        
        exponent_t b_exp = ((int32_t)(pseudo_rand_uint32(&seed) % 60)) - 30;
        headroom_t b_hr  = pseudo_rand_uint32(&seed) % 16;
        exponent_t c_exp = ((int32_t)(pseudo_rand_uint32(&seed) % 60)) - 30;
        headroom_t c_hr  = pseudo_rand_uint32(&seed) % 16;

        exponent_t a_exp;
        right_shift_t a_shr;

        xs3_vect_s16_mul_prepare(&a_exp, &a_shr, b_exp, c_exp, b_hr, c_hr);

        // a_exp and a_shr should be calculated for exactly this case
        int16_t WORD_ALIGNED B = (-0x8000) >> b_hr;
        int16_t WORD_ALIGNED C = (-0x8000) >> c_hr;
        int16_t WORD_ALIGNED A;

        xs3_vect_s16_mul(&A, &B, &C, 1, a_shr);

        double q = ldexp(B, b_exp) * ldexp(C, c_exp);
        double p = ldexp(A, a_exp);

        TEST_ASSERT_EQUAL_MESSAGE(a_shr > 0? 0x4000 : B*C, A, "A is wrong");
        TEST_ASSERT_EQUAL(q, p);
    }
}





static void test_xs3_vect_s32_mul_prepare()
{
    PRINTF("%s...\n", __func__);

    seed = 0xF100319D;

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);
        
        exponent_t b_exp = ((int32_t)(pseudo_rand_uint32(&seed) % 60)) - 30;
        headroom_t b_hr  = pseudo_rand_uint32(&seed) % 31;
        exponent_t c_exp = ((int32_t)(pseudo_rand_uint32(&seed) % 60)) - 30;
        headroom_t c_hr  = pseudo_rand_uint32(&seed) % 31;

        exponent_t a_exp;
        right_shift_t b_shr, c_shr;

        xs3_vect_s32_mul_prepare(&a_exp, &b_shr, &c_shr, b_exp, c_exp, b_hr, c_hr);

        // a_exp and a_shr should be calculated for exactly this case
        int32_t WORD_ALIGNED B = -0x80000000;
        int32_t WORD_ALIGNED C = -0x80000000;
        B = B >> b_hr;
        C = C >> c_hr;
        int32_t WORD_ALIGNED A;

        xs3_vect_s32_mul(&A, &B, &C, 1, b_shr, c_shr);

        double q = ldexp(B, b_exp) * ldexp(C, c_exp);
        double p = ldexp(A, a_exp);

        TEST_ASSERT_EQUAL_MESSAGE(0x40000000, A, "A is wrong");
        TEST_ASSERT_EQUAL(q, p);
    }
}








static void test_xs3_vect_s16_scale_prepare()
{
    PRINTF("%s...\n", __func__);

    seed = 0x0E538276;

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);
        
        exponent_t b_exp = pseudo_rand_int(&seed, -30, 30);
        headroom_t b_hr  = pseudo_rand_uint(&seed, 0, 15);
        exponent_t c_exp = pseudo_rand_int(&seed, -30, 30);
        headroom_t c_hr  = pseudo_rand_uint(&seed, 0, 15);

        int16_t WORD_ALIGNED B_re = ((int16_t)-0x8000) >> b_hr;
        int16_t WORD_ALIGNED B_im = ((int16_t)-0x8000) >> b_hr;
        int16_t WORD_ALIGNED C = ((int16_t)-0x8000) >> c_hr;
        int16_t WORD_ALIGNED A_re;
        int16_t WORD_ALIGNED A_im;

        for(unsigned allow_sat = 0; allow_sat <= 1; allow_sat ++){

            exponent_t a_exp;
            right_shift_t sat, c_shr;

            xs3_vect_s16_scale_prepare(&a_exp, &sat, b_exp, c_exp, b_hr, c_hr, allow_sat);

            xs3_vect_complex_s16_real_scale(&A_re, &A_im, &B_re, &B_im, C, 1, sat);

            const int32_t p = ((int32_t)B_re) * C;
            int16_t expected = p;

            if( allow_sat ){
                if( p > VPU_INT16_MAX )
                    expected = VPU_INT16_MAX;
            } else if( sat > 0 ){
                expected = 0x4000;
            }

            TEST_ASSERT_EQUAL_HEX16(expected, A_re);
            TEST_ASSERT_EQUAL_HEX16(expected, A_im);

            // Make sure the exponent was chosen correctly

            double fp  = ldexp(B_re, b_exp) * ldexp(C, c_exp);
            double fa  = ldexp(A_re, a_exp);

            // (A should be no more than 1 LSb off)
            TEST_ASSERT( fabs(fp - fa) <= ldexp(1,a_exp) );
        }
        
    }
}







static void test_xs3_vect_s16_mul_basic()
{
    PRINTF("%s...\n", __func__);

    typedef struct {
        struct {    int16_t b;  int16_t c;  } value;
        right_shift_t a_shr;
        int16_t expected;
        unsigned line;
    } test_case_t;

    test_case_t casses[] = {
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

    const unsigned N_cases = sizeof(casses)/sizeof(test_case_t);

    const unsigned start_case = 0;

    for(int v = start_case; v < N_cases; v++){
        PRINTF("\ttest vector %d..\n", v);
        
        test_case_t* casse = &casses[v];
        
        //Verify mul_s16() is correct. It's used in other test cases.
        TEST_ASSERT_EQUAL_MSG(casse->expected, 
                    mul_s16(casse->value.b, casse->value.c, casse->a_shr), 
                    casse->line);

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

            hr = xs3_vect_s16_mul(A, B, C, len, casse->a_shr);

            for(int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(xs3_vect_s16_headroom(A, len), hr, casse->line);
            }

            memcpy(A, B, sizeof(A));
            hr = xs3_vect_s16_mul(A, A, C, len, casse->a_shr);

            for(int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(xs3_vect_s16_headroom(A, len), hr, casse->line);
            }

            memcpy(A, C, sizeof(A));
            hr = xs3_vect_s16_mul(A, B, A, len, casse->a_shr);

            for(int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(xs3_vect_s16_headroom(A, len), hr, casse->line);
            }

        }
    }
}






static void test_xs3_vect_s16_mul_random()
{
    PRINTF("%s...\n", __func__);
    seed = 456354333;

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

        int a_shr = (pseudo_rand_uint32(&seed) % 5);
        
        hr = xs3_vect_s16_mul(A, B, C, len, a_shr);

        for(int i = 0; i < len; i++){
            int16_t expected = mul_s16(B[i], C[i], a_shr);
            TEST_ASSERT_EQUAL_MESSAGE(expected, A[i], msg_buff);
        }
        TEST_ASSERT_EQUAL(xs3_vect_s16_headroom(A, len), hr);
        
        memcpy(A, B, sizeof(A[0])*len);
        hr = xs3_vect_s16_mul(A, A, C, len, a_shr);

        for(int i = 0; i < len; i++){
            int16_t expected = mul_s16(B[i], C[i], a_shr);
            TEST_ASSERT_EQUAL_MESSAGE(expected, A[i], msg_buff);
        }
        TEST_ASSERT_EQUAL(xs3_vect_s16_headroom(A, len), hr);
        
        memcpy(A, C, sizeof(A[0])*len);
        hr = xs3_vect_s16_mul(A, B, A, len, a_shr);

        for(int i = 0; i < len; i++){
            int16_t expected = mul_s16(B[i], C[i], a_shr);
            TEST_ASSERT_EQUAL_MESSAGE(expected, A[i], msg_buff);
        }
        TEST_ASSERT_EQUAL(xs3_vect_s16_headroom(A, len), hr);
        
    }
}






static void test_xs3_vect_s32_mul_basic()
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

            hr = xs3_vect_s32_mul(A, B, C, len, casse->shr.b, casse->shr.c);

            for(int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(xs3_vect_s32_headroom(A, len), hr, casse->line);
            }

            memcpy(A, B, sizeof(A));
            hr = xs3_vect_s32_mul(A, A, C, len, casse->shr.b, casse->shr.c);

            for(int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(xs3_vect_s32_headroom(A, len), hr, casse->line);
            }

            memcpy(A, C, sizeof(A));
            hr = xs3_vect_s32_mul(A, B, A, len, casse->shr.b, casse->shr.c);

            for(int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(xs3_vect_s32_headroom(A, len), hr, casse->line);
            }
        }
    }
}





static void test_xs3_vect_s32_mul_random()
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

        hr = xs3_vect_s32_mul(A, B, C, len, b_shr, c_shr);

        for(int i = 0; i < len; i++){
            int32_t expected = mul_s32(B[i], C[i], b_shr, c_shr);
            if(expected != A[i]) sprintf(msg_buff, sprintpat,v, i, len, A[i], B[i], b_shr, C[i], c_shr, (unsigned)A[i], (unsigned)B[i],  (unsigned)C[i]);
            TEST_ASSERT_EQUAL_MESSAGE(expected, A[i], msg_buff);
        }
        TEST_ASSERT_EQUAL(xs3_vect_s32_headroom(A, len), hr);
        
        memcpy(A, B, sizeof(A[0])*len);
        hr = xs3_vect_s32_mul(A, A, C, len, b_shr, c_shr);

        for(int i = 0; i < len; i++){
            int32_t expected = mul_s32(B[i], C[i], b_shr, c_shr);
            if(expected != A[i]) sprintf(msg_buff, sprintpat,v, i, len, A[i], B[i], b_shr, C[i], c_shr, (unsigned)A[i],  (unsigned)B[i],  (unsigned)C[i]);
            TEST_ASSERT_EQUAL_MESSAGE(expected, A[i], msg_buff);
        }
        TEST_ASSERT_EQUAL(xs3_vect_s32_headroom(A, len), hr);
        
        memcpy(A, C, sizeof(A[0])*len);
        hr = xs3_vect_s32_mul(A, B, A, len, b_shr, c_shr);

        for(int i = 0; i < len; i++){
            int32_t expected = mul_s32(B[i], C[i], b_shr, c_shr);
            if(expected != A[i]) sprintf(msg_buff, sprintpat,v, i, len, A[i], B[i], b_shr, C[i], c_shr, (unsigned)A[i],  (unsigned)B[i],  (unsigned)C[i]);
            TEST_ASSERT_EQUAL_MESSAGE(expected, A[i], msg_buff);
        }
        TEST_ASSERT_EQUAL(xs3_vect_s32_headroom(A, len), hr);
        
    }
}



static void test_xs3_vect_s16_scale_basic()
{
    PRINTF("%s...\n", __func__);

    typedef struct {
        struct {    int16_t b;  int16_t alpha;  } value;
        right_shift_t sat;
        int16_t expected;
        unsigned line;
    } test_case_t;

    test_case_t casses[] = {
        // value{       b     alpha }   sat        exp        line num
        {       {  0x0000,   0x0000 },    0,    0x0000,       __LINE__},
        {       {  0x0001,   0x0000 },    0,    0x0000,       __LINE__},
        {       {  0x0000,   0x0001 },    0,    0x0000,       __LINE__},
        {       {  0x4000,   0x4000 },    0,    0x7FFF,       __LINE__},
        {       {  0x4000,   0x4000 },   14,    0x4000,       __LINE__},
        {       { -0x4000,   0x4000 },   14,   -0x4000,       __LINE__},
        {       { -0x4000,  -0x4000 },   14,    0x4000,       __LINE__},
        {       {  0x4000,   0x0001 },   14,    0x0001,       __LINE__},
        {       {  0x0001,   0x4000 },   14,    0x0001,       __LINE__},
        {       {  0x4000,   0x0123 },   14,    0x0123,       __LINE__},
        {       {  0x0123,   0x4000 },   14,    0x0123,       __LINE__},
        {       {  0x0040,   0x0200 },   14,    0x0002,       __LINE__},
        {       {  0x0040,   0x0100 },   14,    0x0001,       __LINE__},
        {       {  0x0040,   0x0080 },   14,    0x0001,       __LINE__},
        {       {  0x0040,   0x0040 },   14,    0x0000,       __LINE__},
        {       {  0x7f00,   0x7f00 },   14,    0x7fff,       __LINE__},
        {       {  0x7f00,  -0x7f00 },   14,   -0x7fff,       __LINE__},
        {       { -0x8000,   0x4000 },   14,   -0x7fff,       __LINE__},
        {       {  0x4000,   0x4000 },   15,    0x2000,       __LINE__},
        {       {  0x4000,   0x2000 },   14,    0x2000,       __LINE__},
        {       {  0x4000,   0x2000 },   15,    0x1000,       __LINE__},
        {       {  0x4000,   0x0200 },   17,    0x0040,       __LINE__},
        {       {  0x0800,   0x4000 },   13,    0x1000,       __LINE__},
        {       {  0x0800,   0x4000 },   12,    0x2000,       __LINE__},
        {       {  0x0800,   0x4000 },   11,    0x4000,       __LINE__},
        {       {  0x0800,   0x4000 },   10,    0x7fff,       __LINE__},
        {       {  0x0800,  -0x4000 },   10,   -0x7fff,       __LINE__},
        {       {  0x0800,   0x2000 },   10,    0x4000,       __LINE__},

        
    };

    const unsigned N_cases = sizeof(casses)/sizeof(test_case_t);

    const unsigned start_case = 0;

    for(int v = start_case; v < N_cases; v++){
        PRINTF("\ttest vector %d..\n", v);
        
        test_case_t* casse = &casses[v];
        
        //Verify mul_s16() is correct. It's used in other test cases.
        TEST_ASSERT_EQUAL_MSG(casse->expected, scalar_mul_s16(casse->value.b, casse->value.alpha, casse->sat), casse->line);

        unsigned lengths[] = {1, 4, 16, 32, 40 };

        for( int l = 0; l < sizeof(lengths)/sizeof(lengths[0]); l++){
            unsigned len = lengths[l];

            // PRINTF("\t\tlength %u\n", len);

            headroom_t hr;
            int16_t A[40];
            int16_t B[40];

            for(int i = 0; i < len; i++){
                A[i] = 0xCC;
                B[i] = casse->value.b;
            }


            hr = xs3_vect_s16_scale(A, B, len, casse->value.alpha, casse->sat);

            for(int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(xs3_vect_s16_headroom(A, len), hr, casse->line);
            }

            memcpy(A, B, sizeof(A));
            hr = xs3_vect_s16_scale(A, A, len, casse->value.alpha, casse->sat);

            for(int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(xs3_vect_s16_headroom(A, len), hr, casse->line);
            }
        }
    }
}





static void test_xs3_vect_s16_scale_random()
{
    PRINTF("%s...\n", __func__);
    seed = 0x091C2511;

    headroom_t hr;
    int16_t A[MAX_LEN];
    int16_t B[MAX_LEN];
    int16_t alpha;

    for(int v = 0; v < REPS; v++){

        PRINTF("\trepetition %d.. (seed: 0x%08X)\n", v, seed);

        unsigned len = (pseudo_rand_uint32(&seed) % MAX_LEN) + 1;
        
        for(int i = 0; i < len; i++){
            unsigned shr = pseudo_rand_uint32(&seed) % 8;
            B[i] = pseudo_rand_int16(&seed) >> shr;
        }

        alpha = pseudo_rand_int16(&seed) >> (pseudo_rand_uint32(&seed) % 8);

        right_shift_t sat = (pseudo_rand_uint32(&seed) % 5) - 2;
        
        const char sprintpat[] = "rep(%d)[%d of %u]: %d <-- ((%d >> %d) * %d) >> 14     (A[i]=0x%04X; B[i]=0x%04X; alpha=0x%04X)";

        hr = xs3_vect_s16_scale(A, B, len, alpha, sat);

        for(int i = 0; i < len; i++){
            int16_t expected = scalar_mul_s16(B[i], alpha, sat);
            if(expected != A[i]) sprintf(msg_buff, sprintpat,v, i, len, A[i], B[i], sat, alpha, (uint16_t)A[i], (uint16_t)B[i],  (uint16_t)alpha);
            TEST_ASSERT_EQUAL_MESSAGE(expected, A[i], msg_buff);
        }
        TEST_ASSERT_EQUAL(xs3_vect_s16_headroom(A, len), hr);
        
        memcpy(A, B, sizeof(A[0])*len);
        hr = xs3_vect_s16_scale(A, A, len, alpha, sat);

        for(int i = 0; i < len; i++){
            int16_t expected = scalar_mul_s16(B[i], alpha, sat);
            if(expected != A[i]) sprintf(msg_buff, sprintpat,v, i, len, A[i], B[i], sat, alpha, (uint16_t)A[i], (uint16_t)B[i],  (uint16_t)alpha);
            TEST_ASSERT_EQUAL_MESSAGE(expected, A[i], msg_buff);
        }
        TEST_ASSERT_EQUAL(xs3_vect_s16_headroom(A, len), hr);
    }
}





static void test_xs3_vect_s32_scale_basic()
{
    PRINTF("%s...\n", __func__);

    typedef struct {
        struct {    int32_t b;  int32_t alpha;  } value;
        struct {    int b;    } shr;
        int32_t expected;
        unsigned line;
    } test_case_t;

    test_case_t casses[] = {
        // value{           b             c }   shr{   b    c }            exp        line num
        {       {  0x00000000,   0x00000000 },     {  0 },      0x00000000,       __LINE__},
        {       {  0x00010000,   0x00000000 },     {  0 },      0x00000000,       __LINE__},
        {       {  0x00000000,   0x00010000 },     {  0 },      0x00000000,       __LINE__},
        {       {  0x40000000,   0x40000000 },     {  0 },      0x40000000,       __LINE__},
        {       { -0x40000000,   0x40000000 },     {  0 },     -0x40000000,       __LINE__},
        {       { -0x40000000,  -0x40000000 },     {  0 },      0x40000000,       __LINE__},
        {       {  0x40000000,   0x00010000 },     {  0 },      0x00010000,       __LINE__},
        {       {  0x00010000,   0x40000000 },     {  0 },      0x00010000,       __LINE__},
        {       {  0x40000000,   0x01230000 },     {  0 },      0x01230000,       __LINE__},
        {       {  0x01230000,   0x40000000 },     {  0 },      0x01230000,       __LINE__},
        {       {  0x00400000,   0x02000000 },     {  0 },      0x00020000,       __LINE__},
        {       {  0x00400000,   0x01000000 },     {  0 },      0x00010000,       __LINE__},
        {       {  0x00004000,   0x00010000 },     {  0 },      0x00000001,       __LINE__},
        {       {  0x00004000,   0x00008000 },     {  0 },      0x00000001,       __LINE__},
        {       {  0x00000400,   0x00000400 },     {  0 },      0x00000000,       __LINE__},
        {       {  0x7f000000,   0x7f000000 },     {  0 },      0x7fffffff,       __LINE__},
        {       {  0x7f000000,  -0x7f000000 },     {  0 },     -0x7fffffff,       __LINE__},
        {       { -0x80000000,   0x40000000 },     {  0 },     -0x7fffffff,       __LINE__},
        {       {  0x40000000,   0x40000000 },     {  1 },      0x20000000,       __LINE__},
        {       {  0x40000000,   0x20000000 },     {  0 },      0x20000000,       __LINE__},
        {       {  0x40000000,   0x20000000 },     {  1 },      0x10000000,       __LINE__},
        {       {  0x40000000,   0x02000000 },     {  3 },      0x00400000,       __LINE__},
        {       {  0x08000000,   0x40000000 },     { -1 },      0x10000000,       __LINE__},
        {       {  0x08000000,   0x40000000 },     { -2 },      0x20000000,       __LINE__},
        {       {  0x08000000,   0x40000000 },     { -3 },      0x40000000,       __LINE__},
        {       {  0x08000000,   0x40000000 },     { -4 },      0x7fffffff,       __LINE__},
        {       {  0x08000000,  -0x40000000 },     { -4 },     -0x7fffffff,       __LINE__},
        {       {  0x08000000,   0x20000000 },     { -4 },      0x40000000,       __LINE__},
    };

    const unsigned N_cases = sizeof(casses)/sizeof(test_case_t);

    const unsigned start_case = 0;

    for(int v = start_case; v < N_cases; v++){
        PRINTF("\ttest vector %d..\n", v);
        
        test_case_t* casse = &casses[v];

        //Verify mul_s32() is correct. It's used in other test cases.
        TEST_ASSERT_EQUAL_MSG(casse->expected, scalar_mul_s32(casse->value.b, casse->value.alpha, casse->shr.b), casse->line);

        unsigned lengths[] = {1, 4, 16, 32, 40 };
        

        for( int l = 0; l < sizeof(lengths)/sizeof(lengths[0]); l++){
            unsigned len = lengths[l];

            // PRINTF("\t\tlength %u\n", len);

            headroom_t hr;
            int32_t A[40];
            int32_t B[40];

            for(int i = 0; i < len; i++){
                A[i] = 0xCC;
                B[i] = casse->value.b;
            }

            hr = xs3_vect_s32_scale(A, B, len, casse->value.alpha, casse->shr.b);

            for(int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(xs3_vect_s32_headroom(A, len), hr, casse->line);
            }

            memcpy(A, B, sizeof(A));
            hr = xs3_vect_s32_scale(A, A, len, casse->value.alpha, casse->shr.b);

            for(int i = 0; i < len; i++){
                TEST_ASSERT_EQUAL_MSG(casse->expected, A[0], casse->line);
                TEST_ASSERT_EQUAL_MSG(xs3_vect_s32_headroom(A, len), hr, casse->line);
            }
        }
    }
}





static void test_xs3_vect_s32_scale_random()
{
    PRINTF("%s...\n", __func__);
    seed = 67456;

    headroom_t hr;
    int32_t A[MAX_LEN];
    int32_t B[MAX_LEN];
    int32_t alpha;

    for(int v = 0; v < REPS; v++){

        PRINTF("\trepetition %d..\n", v);

        unsigned len = (pseudo_rand_uint32(&seed) % MAX_LEN) + 1;
        
        for(int i = 0; i < len; i++){
            unsigned shr = pseudo_rand_uint32(&seed) % 8;
            B[i] = pseudo_rand_int32(&seed) >> shr;
        }

        alpha = pseudo_rand_int32(&seed) >> (pseudo_rand_uint32(&seed) % 8);

        int b_shr = (pseudo_rand_uint32(&seed) % 5) - 2;
        
        const char sprintpat[] = "rep(%d)[%d of %u]: %ld <-- ((%ld >> %d) * %ld) >> 30     (A[i]=0x%08X; B[i]=0x%08X; alpha=0x%08X)";

        hr = xs3_vect_s32_scale(A, B, len, alpha, b_shr);

        for(int i = 0; i < len; i++){
            int32_t expected = scalar_mul_s32(B[i], alpha, b_shr);
            if(expected != A[i]) sprintf(msg_buff, sprintpat,v, i, len, A[i], B[i], b_shr, alpha, (unsigned)A[i], (unsigned)B[i],  (unsigned)alpha);
            TEST_ASSERT_EQUAL_MESSAGE(expected, A[i], msg_buff);
        }
        TEST_ASSERT_EQUAL(xs3_vect_s32_headroom(A, len), hr);
        
        memcpy(A, B, sizeof(A[0])*len);
        hr = xs3_vect_s32_scale(A, A, len, alpha, b_shr);

        for(int i = 0; i < len; i++){
            int32_t expected = scalar_mul_s32(B[i], alpha, b_shr);
            if(expected != A[i]) sprintf(msg_buff, sprintpat,v, i, len, A[i], B[i], b_shr, alpha, (unsigned)A[i], (unsigned)B[i],  (unsigned)alpha);
            TEST_ASSERT_EQUAL_MESSAGE(expected, A[i], msg_buff);
        }
        TEST_ASSERT_EQUAL(xs3_vect_s32_headroom(A, len), hr);
    }
}



void test_xs3_mul_vect()
{
    SET_TEST_FILE();

    RUN_TEST(test_xs3_vect_s16_mul_prepare);
    RUN_TEST(test_xs3_vect_s32_mul_prepare);
    RUN_TEST(test_xs3_vect_s16_scale_prepare);

    RUN_TEST(test_xs3_vect_s16_mul_basic);
    RUN_TEST(test_xs3_vect_s16_mul_random);

    RUN_TEST(test_xs3_vect_s32_mul_basic);
    RUN_TEST(test_xs3_vect_s32_mul_random);


    RUN_TEST(test_xs3_vect_s16_scale_basic);
    RUN_TEST(test_xs3_vect_s16_scale_random);

    RUN_TEST(test_xs3_vect_s32_scale_basic);
    RUN_TEST(test_xs3_vect_s32_scale_random);
}