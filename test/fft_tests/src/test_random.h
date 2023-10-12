// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once


EXTERN_C int16_t  pseudo_rand_int16(unsigned *r);
EXTERN_C uint16_t pseudo_rand_uint16(unsigned *r);
EXTERN_C int32_t  pseudo_rand_int32(unsigned *r);
EXTERN_C uint32_t pseudo_rand_uint32(unsigned *r);
EXTERN_C int64_t  pseudo_rand_int64(unsigned *r);
EXTERN_C uint64_t pseudo_rand_uint64(unsigned *r);

EXTERN_C void pseudo_rand_bytes(unsigned *r, char* buffer, unsigned size);


EXTERN_C
void rand_vect_s16(
    int16_t output[],
    const unsigned length,
    const unsigned headroom,
    unsigned* seed);
    

EXTERN_C
void rand_vect_s32(
    int32_t output[],
    const unsigned length,
    const unsigned headroom,
    unsigned* seed);
    

EXTERN_C
void rand_vect_complex_s16(
    complex_s16_t output[],
    const unsigned length,
    const unsigned headroom,
    unsigned* seed);
    

EXTERN_C
void rand_vect_complex_s32(
    complex_s32_t output[],
    const unsigned length,
    const unsigned headroom,
    unsigned* seed);


EXTERN_C
void test_random_bfp_s16(
    bfp_s16_t* B, 
    unsigned max_len, 
    unsigned* seed,
    bfp_s16_t* A,
    unsigned int length);


EXTERN_C
void test_random_bfp_s32(
    bfp_s32_t* B, 
    unsigned max_len, 
    unsigned* seed,
    bfp_s32_t* A,
    unsigned int length);

EXTERN_C
void test_random_bfp_complex_s16(
    bfp_complex_s16_t* B, 
    unsigned max_len, 
    unsigned* seed,
    bfp_complex_s16_t* A,
    unsigned int length);

EXTERN_C
void test_random_bfp_complex_s32(
    bfp_complex_s32_t* B, 
    unsigned max_len, 
    unsigned* seed,
    bfp_complex_s32_t* A,
    unsigned int length);