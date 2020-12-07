
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xs3_math.h"

#include "../src/vect/vpu_helper.h"

#include "../../tst_common.h"

#include "unity.h"


static char msg_buff[300];
static unsigned seed = 4563456;

const extern unsigned rot_table32_rows;
const extern complex_s32_t rot_table32[30][4];

#define TEST_ASSERT_EQUAL_MSG(EXPECTED, ACTUAL, LINE_NUM)   do{       \
    if((EXPECTED)!=(ACTUAL)) {                                        \
      sprintf(msg_buff, "(test vector @ line %u)", (LINE_NUM));       \
      TEST_ASSERT_EQUAL_MESSAGE((EXPECTED), (ACTUAL), msg_buff);      \
    }} while(0)

#define TEST_ASSERT_INT32_WITHIN_MSG(THRESHOLD, EXPECTED, ACTUAL, FMT, ...)   do{       \
    if(abs((ACTUAL)-(EXPECTED)) > THRESHOLD) {                                           \
      sprintf(msg_buff, (FMT), __VA_ARGS__);                                            \
      TEST_ASSERT_INT32_WITHIN_MESSAGE((THRESHOLD), (EXPECTED), (ACTUAL), msg_buff);    \
    }} while(0)


#if DEBUG_ON || 0
#undef DEBUG_ON
#define DEBUG_ON    (1)
#endif


const extern unsigned rot_table32_rows;
const extern complex_s32_t rot_table32[30][4];

const extern unsigned rot_table16_rows;
const extern int16_t rot_table16[14][2][16];


static int16_t mag_complex_s16(complex_s16_t b, right_shift_t b_shr)
{
    int32_t bp_re = ASHR(16)(b.re, b_shr);
    int32_t bp_im = ASHR(16)(b.im, b_shr);


    int32_t sqr_mag = bp_re*bp_re + bp_im*bp_im;

    assert(sqr_mag >= 0);

    double mag = sqrt(sqr_mag);

    int16_t a = round(mag);
    
    return SAT(16)(a);
}


static int32_t mag_complex_s32(complex_s32_t b, right_shift_t b_shr)
{
    int64_t bp_re = b.re;
    int64_t bp_im = b.im;

    bp_re = ASHR(32)(bp_re, b_shr);
    bp_im = ASHR(32)(bp_im, b_shr);

    bp_re = SAT(32)(bp_re);
    bp_im = SAT(32)(bp_im);

    int64_t sqr_mag = bp_re*bp_re + bp_im*bp_im;

    double mag = sqrt(sqr_mag);

    int32_t a = round(mag);
    
    return SAT(32)(a);
}






#define REPS        1000
static void test_xs3_mag_vect_complex_calc_params()
{
    PRINTF("%s...\n", __func__);

    seed = 0x142B711E;

    for(int r = 0; r < REPS; r++){
        PRINTF("\trep % 3d..\t(seed: 0x%08X)\n", r, seed);
        
        exponent_t b_exp = pseudo_rand_int(&seed, -30, 30);
        headroom_t b_hr  = pseudo_rand_uint(&seed, 0, 31);

        complex_s32_t B = { 0, INT32_MIN >> b_hr };
        int32_t A_mag;

        for(unsigned allow_sat = 0; allow_sat <= 1; allow_sat ++){
            
            exponent_t a_exp;
            right_shift_t b_shr;

            // PRINTF("\n\t    allow_sat = %u\n", allow_sat);
            // PRINTF("\t    b_exp = %d\n", b_exp);
            // PRINTF("\t    b_hr = %d\n", b_hr);

            xs3_mag_vect_complex_calc_params(&a_exp, &b_shr, b_exp, b_hr, allow_sat);

            // PRINTF("\t    B.re = %ld    (0x%08lX)\n", B.re, (uint32_t) B.re);
            // PRINTF("\t    B.im = %ld    (0x%08lX)\n", B.im, (uint32_t) B.im);

            xs3_vect_complex_s32_mag(&A_mag, &B, 1, b_shr, (complex_s32_t*) rot_table32, rot_table32_rows);

            // PRINTF("\t    A_mag = %ld   (0x%08lX)\n", A_mag, (uint32_t) A_mag);
            // PRINTF("\t    a_exp = %d\n", a_exp);
            // PRINTF("\t    b_shr  = %d\n", b_shr);

            if(allow_sat){
                TEST_ASSERT_INT32_WITHIN(1, INT32_MAX, A_mag);
            } else {
                TEST_ASSERT_INT32_WITHIN(1, 0x40000000, A_mag);
            }
            
            TEST_ASSERT_TRUE(  ( fabs( ldexp(A_mag, a_exp) + ldexp( B.im, b_exp) ) / B.im ) <= 0.00001  );
        }
    }
}
#undef REPS







#define THRESHOLD   5
static void test_xs3_vect_complex_s16_mag_basic()
{
    PRINTF("%s...\n", __func__);

    typedef struct {
        complex_s16_t b;

        right_shift_t b_shr;

        int16_t expected;
        unsigned line;
    } test_case_t;

    unsigned max_abs_delta = 0;

    test_case_t casses[] = {
        //{ {    b.re,     b.im}, b_shr,  expected,      line num
          { {  0x0000,   0x0000},     0,    0x0000,      __LINE__},
          { {  0x0000,   0x0000},     0,    0x0000,      __LINE__},
          { {  0x4000,   0x0000},     0,    0x4000,      __LINE__},
          { { -0x4000,   0x0000},     0,    0x4000,      __LINE__},
          { {  0x0000,   0x4000},     0,    0x4000,      __LINE__},
          { {  0x0000,  -0x4000},     0,    0x4000,      __LINE__},
          { {  0x0400,   0x0000},     0,    0x0400,      __LINE__},
          { { -0x0400,   0x0000},     0,    0x0400,      __LINE__},
          { {  0x0000,   0x0400},     0,    0x0400,      __LINE__},
          { {  0x0000,  -0x0400},     0,    0x0400,      __LINE__},
          { {  0x2000,   0x2000},     0,    0x2D41,      __LINE__},
          { { -0x1000,  -0x1000},     0,    0x16A1,      __LINE__},
          
          { {  0x4000,   0x0000},     4,    0x0400,      __LINE__},
          { { -0x4000,   0x0000},     4,    0x0400,      __LINE__},
          { {  0x0000,   0x4000},     4,    0x0400,      __LINE__},
          { {  0x0000,  -0x4000},     4,    0x0400,      __LINE__},
          { {  0x0400,   0x0000},     4,    0x0040,      __LINE__},
          { { -0x0400,   0x0000},     4,    0x0040,      __LINE__},
          { {  0x0000,   0x0400},     4,    0x0040,      __LINE__},
          { {  0x0000,  -0x0400},     4,    0x0040,      __LINE__},
          { {  0x2000,   0x2000},     4,    0x02D4,      __LINE__},
          { { -0x1000,  -0x1000},     4,    0x016A,      __LINE__},
          
          { {  0x0400,   0x0000},    -4,    0x4000,      __LINE__},
          { { -0x0400,   0x0000},    -4,    0x4000,      __LINE__},
          { {  0x0000,   0x0400},    -4,    0x4000,      __LINE__},
          { {  0x0000,  -0x0400},    -4,    0x4000,      __LINE__},
          { {  0x2000,   0x2000},    -1,    0x2D41<<1,   __LINE__},
          { { -0x1000,  -0x1000},    -2,    0x2D41<<1,   __LINE__},
    };

    const unsigned N_cases = sizeof(casses)/sizeof(test_case_t);

    const unsigned start_case = 0;

    for(int v = start_case; v < N_cases; v++){
        // PRINTF("\ttest vector %d..\n", v);
        
        test_case_t* casse = &casses[v];

        unsigned lengths[] = {1, 4, 16, 32, 34 };

        for( int l = 0; l < sizeof(lengths)/sizeof(lengths[0]); l++){
            unsigned len = lengths[l];
            // printf("! %u\n", len);
            headroom_t hr;
            int16_t A[40];

            struct {
                int16_t real[40];
                int16_t imag[40];
            } B;

            for(int i = 0; i < len; i++){
                A[i] = 0xCC;
                B.real[i] = casse->b.re;
                B.imag[i] = casse->b.im;
            }

            hr = xs3_vect_complex_s16_mag(A, B.real, B.imag, len, casse->b_shr, (int16_t*) rot_table16, rot_table16_rows);

            for(int i = 0; i < len; i++){
                
                unsigned abs_delta = abs(A[i] - casse->expected);

                TEST_ASSERT_INT32_WITHIN_MSG(THRESHOLD, casse->expected, A[i], 
                    "(test vector %d @ line %u) (length: %u) (index %d): mag( (%d + i*%d) >> %d )   (delta = %d)",
                        v, casse->line, len, i, B.real[i], B.imag[i], casse->b_shr, A[i] - casse->expected );

                max_abs_delta = MAX(max_abs_delta, abs_delta);
            }
            TEST_ASSERT_EQUAL_MSG(xs3_vect_s16_headroom((int16_t*) A, len), hr, casse->line);
        }
    }

    PRINTF("\tMaximum absolute delta: %u\n", max_abs_delta);
}
#undef THRESHOLD





#define MAX_LEN     100
#define REPS        1000
#define THRESHOLD   10
static void test_xs3_vect_complex_s16_mag_random()
{
    PRINTF("%s...\n", __func__);
    unsigned seed = 0x9B54F255;

    headroom_t hr;
    
    int16_t A[MAX_LEN];

    struct {
        int16_t real[MAX_LEN];
        int16_t imag[MAX_LEN];
    } B;

    unsigned max_abs_delta = 0;

    for(int v = 0; v < REPS; v++){

        unsigned rep_seed = seed;

        // PRINTF("\trepetition % 3d..\t(seed: 0x%08X)\n", v, seed);

        unsigned len = (pseudo_rand_uint32(&seed) % MAX_LEN) + 1;
        
        headroom_t B_hr = pseudo_rand_uint32(&seed) % 4;

        for(int i = 0; i < len; i++){
            B.real[i] = pseudo_rand_int16(&seed) >> B_hr;
            B.imag[i] = pseudo_rand_int16(&seed) >> B_hr;
        }

        B_hr = MIN( xs3_vect_s16_headroom(B.real, len), xs3_vect_s16_headroom(B.imag, len) );
// printf("! %d\t\t%d\n", B_hr, B.imag[0]);
        right_shift_t b_shr = -B_hr + 1;
        
        hr = xs3_vect_complex_s16_mag(A, B.real, B.imag, len, b_shr, (int16_t*) rot_table16, rot_table16_rows);


        for(int i = 0; i < len; i++){
            complex_s16_t bbb = {B.real[i], B.imag[i]};
            int16_t expected = mag_complex_s16(bbb, b_shr);

            unsigned abs_delta = abs(A[i] - expected);

            TEST_ASSERT_INT32_WITHIN_MSG(THRESHOLD, expected, A[i], 
                "(rep %d; seed: 0x%08X) A[%i]: mag( (%d + i*%d) >> %d ) (delta = %d) (B_hr: %d)",
                       v, rep_seed, i, B.real[i], B.imag[i], b_shr, A[i] - expected, B_hr );

            max_abs_delta = MAX(max_abs_delta, abs_delta);
        }
        TEST_ASSERT_EQUAL_MSG(xs3_vect_s16_headroom((int16_t*) A, len), hr, v);
    }

    PRINTF("\tMaximum absolute delta: %u\n", max_abs_delta);
}
#undef MAX_LEN
#undef REPS
#undef THRESHOLD





#define THRESHOLD   7
static void test_xs3_vect_complex_s32_mag_basic()
{
    PRINTF("%s...\n", __func__);

    typedef struct {
        complex_s32_t b;
        right_shift_t b_shr;

        int32_t expected;
        unsigned line;
    } test_case_t;

    unsigned max_abs_delta = 0;

    test_case_t casses[] = {
        //{ {       b.re,       b.im}, b_shr,   expected,      line num
          { { 0x00000000, 0x00000000},     0, 0x00000000,      __LINE__},
          { { 0x00000000, 0x00000000},     1, 0x00000000,      __LINE__},
          { { 0x40000000, 0x00000000},     0, 0x40000000,      __LINE__},
          { {-0x40000000, 0x00000000},     0, 0x40000000,      __LINE__},
          { { 0x00000000, 0x40000000},     0, 0x40000000,      __LINE__},
          { { 0x00000000,-0x40000000},     0, 0x40000000,      __LINE__},
          { { 0x04000000, 0x00000000},     0, 0x04000000,      __LINE__},
          { {-0x04000000, 0x00000000},     0, 0x04000000,      __LINE__},
          { { 0x00000000, 0x04000000},     0, 0x04000000,      __LINE__},
          { { 0x00000000,-0x04000000},     0, 0x04000000,      __LINE__},
          { { 0x40000000, 0x00000000},     4, 0x04000000,      __LINE__},
          { {-0x40000000, 0x00000000},     4, 0x04000000,      __LINE__},
          { { 0x00000000, 0x40000000},     4, 0x04000000,      __LINE__},
          { { 0x00000000,-0x40000000},     4, 0x04000000,      __LINE__},
          { { 0x00001000, 0x00000000},     0, 0x00001000,      __LINE__},
          { { 0x00001000, 0x00000000},     4, 0x00000100,      __LINE__},
          { { 0x00001000, 0x00000000},     8, 0x00000010,      __LINE__},
          { { 0x00001000, 0x00000000},    12, 0x00000001,      __LINE__},
          { { 0x20000000, 0x20000000},     0, 0x2d413ccd,      __LINE__},
          { { 0x20000000,-0x20000000},     4, 0x02d413cd,      __LINE__},
          { {-0x40000000,-0x40000000},     0, 0x5A82799A,      __LINE__},
          { { 0x0186B400, 0x22E7EC80},     0, 0x22f07608,      __LINE__}, //worst case I've found, off by 7
        //   { { 0x20daf1a5, 0x7e8080b5},     0, 
    };

    const unsigned N_cases = sizeof(casses)/sizeof(test_case_t);

    const unsigned start_case = 0;

    for(int v = start_case; v < N_cases; v++){
        // PRINTF("\ttest vector %d..\n", v);
        
        test_case_t* casse = &casses[v];

        unsigned lengths[] = {1, 4, 16, 32, 34 };

        for( int l = 0; l < sizeof(lengths)/sizeof(lengths[0]); l++){
            unsigned len = lengths[l];
            // printf("! %u\n", len);
            headroom_t hr;
            int32_t A[40];
            complex_s32_t B[40];
            complex_s32_t C;

            for(int i = 0; i < len; i++){
                A[i] = 0xCC;
                B[i].re = casse->b.re;
                B[i].im = casse->b.im;
            }

            hr = xs3_vect_complex_s32_mag(A, B, len, casse->b_shr, (complex_s32_t*) rot_table32, rot_table32_rows);

            for(int i = 0; i < len; i++){
                
                unsigned abs_delta = abs(A[i] - casse->expected);

                TEST_ASSERT_INT32_WITHIN_MSG(THRESHOLD, casse->expected, A[i], 
                    "(test vector %d @ line %u) (length: %u) (index %d): mag(( %ld + i*%ld) >> %d)   (delta = %ld)",
                        v, casse->line, len, i, B[i].re, B[i].im, casse->b_shr, A[i] - casse->expected );

                max_abs_delta = MAX(max_abs_delta, abs_delta);
            }
            TEST_ASSERT_EQUAL_MSG(xs3_vect_s32_headroom((int32_t*) A, len), hr, casse->line);
        }
    }

    PRINTF("\tMaximum absolute delta: %u\n", max_abs_delta);
}
#undef THRESHOLD







#define MAX_LEN     100
#define REPS        1000
#define THRESHOLD   7
static void test_xs3_vect_complex_s32_mag_random()
{
    PRINTF("%s...\n", __func__);
    unsigned seed = 3463456;

    headroom_t hr;
    
    int32_t A[MAX_LEN];
    complex_s32_t B[MAX_LEN];

    unsigned max_abs_delta = 0;

    for(int v = 0; v < REPS; v++){

        unsigned rep_seed = seed;

        // PRINTF("\trepetition % 3d..\t(seed: 0x%08X)\n", v, seed);

        unsigned len = (pseudo_rand_uint32(&seed) % MAX_LEN) + 1;
        
        headroom_t B_hr = pseudo_rand_uint32(&seed) % 8;

        for(int i = 0; i < len; i++){
            //Bottom bits masked out because the expectation is calculated using double-precision floats, which
            // only have 53 mantissa bits. Masking out the bottom 8 bits of each means that their product will
            // have 16 zeros in the least significant bits, so it won't get truncated when converting to double.
            B[i].re = ( pseudo_rand_int32(&seed) & 0xFFFFFF00   ) >> (B_hr);
            B[i].im = ( pseudo_rand_int32(&seed) & 0xFFFFFF00   ) >> (B_hr);
        }
        
        B_hr = xs3_vect_s32_headroom((int32_t*) B, 2*len);

        right_shift_t b_shr = (-B_hr)+1;
        
        hr = xs3_vect_complex_s32_mag(A, B, len, b_shr, (complex_s32_t*) rot_table32, rot_table32_rows);


        for(int i = 0; i < len; i++){
            int32_t expected = mag_complex_s32(B[i], b_shr);

            unsigned abs_delta = abs(A[i] - expected);

            TEST_ASSERT_INT32_WITHIN_MSG(THRESHOLD, expected, A[i], 
                "(rep %d; seed: 0x%08X) A[%i]: mag(( %ld + i*%ld) >> %d) (delta = %ld) (B_hr: %d)",
                       v, rep_seed, i, B[i].re, B[i].im, b_shr, A[i] - expected, B_hr );

            max_abs_delta = MAX(max_abs_delta, abs_delta);
        }
        TEST_ASSERT_EQUAL_MSG(xs3_vect_s32_headroom((int32_t*) A, len), hr, v);
    }

    PRINTF("\tMaximum absolute delta: %u\n", max_abs_delta);
}
#undef MAX_LEN
#undef REPS
#undef THRESHOLD



void test_xs3_mag_vect_complex()
{
    SET_TEST_FILE();

    RUN_TEST(test_xs3_mag_vect_complex_calc_params);

    RUN_TEST(test_xs3_vect_complex_s16_mag_basic);
    RUN_TEST(test_xs3_vect_complex_s16_mag_random);

    RUN_TEST(test_xs3_vect_complex_s32_mag_basic);
    RUN_TEST(test_xs3_vect_complex_s32_mag_random);
}