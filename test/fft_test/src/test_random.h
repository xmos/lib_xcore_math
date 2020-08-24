#pragma once


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


void rand_vect_s16(
    int16_t output[],
    const unsigned length,
    const unsigned headroom,
    unsigned* seed);
    
void rand_vect_s32(
    int32_t output[],
    const unsigned length,
    const unsigned headroom,
    unsigned* seed);
    
void rand_vect_complex_s16(
    complex_s16_t output[],
    const unsigned length,
    const unsigned headroom,
    unsigned* seed);
    
void rand_vect_complex_s32(
    complex_s32_t output[],
    const unsigned length,
    const unsigned headroom,
    unsigned* seed);


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

#if defined(__XC__) || defined(__CPLUSPLUS__)
}   // extern "C"
#endif