

#ifndef TST_COMMON_H_
#define TST_COMMON_H_

#include <stdint.h>

#include "xs3_math.h"

#define SET_TEST_FILE()     Unity.TestFile = __FILE__

#ifdef __xcore__
#define WORD_ALIGNED __attribute__((aligned (4)))
#else
#define WORD_ALIGNED
#endif

#ifndef DEBUG_ON
#define DEBUG_ON    0
#endif

#ifndef QUICK_TEST
#define QUICK_TEST  0
#endif

#define IF_QUICK_TEST(X, Y)  ((QUICK_TEST)? X : Y)

#define PRINTF(...)     do{if (DEBUG_ON) {printf(__VA_ARGS__);}} while(0)

int16_t  pseudo_rand_int16(unsigned *r);
uint16_t pseudo_rand_uint16(unsigned *r);
int32_t  pseudo_rand_int32(unsigned *r);
uint32_t pseudo_rand_uint32(unsigned *r);
int64_t  pseudo_rand_int64(unsigned *r);
uint64_t pseudo_rand_uint64(unsigned *r);


void pseudo_rand_bytes(unsigned *r, char* buffer, unsigned size);


#if defined(__XC__) || defined(__CPLUSPLUS__)
extern "C" {
#endif 

void xs3_fft_index_bit_reversal_double(
    complex_double_t* a,
    const unsigned length);


void test_random_bfp_s16(
    bfp_s16_t* B, 
    unsigned max_len, 
    unsigned* seed,
    bfp_s16_t* A,
    int length);

void test_random_bfp_s32(
    bfp_s32_t* B, 
    unsigned max_len, 
    unsigned* seed,
    bfp_s32_t* A,
    int length);
void test_random_bfp_complex_s16(
    bfp_complex_s16_t* B, 
    unsigned max_len, 
    unsigned* seed,
    bfp_complex_s16_t* A,
    int length);
void test_random_bfp_complex_s32(
    bfp_complex_s32_t* B, 
    unsigned max_len, 
    unsigned* seed,
    bfp_complex_s32_t* A,
    int length);
void test_random_bfp_ch_pair_s16(
    bfp_ch_pair_s16_t* B, 
    unsigned max_len, 
    unsigned* seed,
    bfp_ch_pair_s16_t* A,
    int length);
void test_random_bfp_ch_pair_s32(
    bfp_ch_pair_s32_t* B, 
    unsigned max_len, 
    unsigned* seed,
    bfp_ch_pair_s32_t* A,
    int length);

void test_double_from_s16(
    double* d_out,
    bfp_s16_t* d_in);
void test_double_from_s32(
    double* d_out,
    bfp_s32_t* d_in);
void test_double_from_complex_s16(
    double* d_out_real,
    double* d_out_imag,
    bfp_complex_s16_t* d_in);
void test_double_from_complex_s32(
    double* d_out_real,
    double* d_out_imag,
    bfp_complex_s32_t* d_in);


void test_s16_from_double(
    int16_t* d_out,
    double* d_in,
    unsigned length,
    exponent_t use_exp);

void test_s32_from_double(
    int32_t* d_out,
    double* d_in,
    unsigned length,
    exponent_t use_exp);

void test_complex_s16_from_double(
    int16_t* d_out_real,
    int16_t* d_out_imag,
    double* d_in_real,
    double* d_in_imag,
    unsigned length,
    exponent_t use_exp);

void test_complex_s32_from_double(
    complex_s32_t* d_out,
    double* d_in_real,
    double* d_in_imag,
    unsigned length,
    exponent_t use_exp);

#if defined(__XC__) || defined(__CPLUSPLUS__)
}   // extern "C"
#endif

#endif //TST_COMMON_H_