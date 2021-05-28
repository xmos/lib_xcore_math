// Copyright 2020-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include "tst_common.h"

// #include <xs1.h>
// #include <xclib.h>
#include <stdio.h>
#include <assert.h>

#include "bfp_math.h"
#include "pseudo_rand.h"


void rand_vect_s16(
    int16_t output[],
    const unsigned length,
    const unsigned headroom,
    unsigned* seed)
{
    for(int i = 0; i < length; i++)
        output[i] = pseudo_rand_int16(seed) >> headroom;
}

void rand_vect_s32(
    int32_t output[],
    const unsigned length,
    const unsigned headroom,
    unsigned* seed)
{
    for(int i = 0; i < length; i++)
        output[i] = pseudo_rand_int32(seed) >> headroom;
}

void rand_vect_complex_s16(
    complex_s16_t output[],
    const unsigned length,
    const unsigned headroom,
    unsigned* seed)
{
    for(int i = 0; i < length; i++){
        output[i].re = pseudo_rand_int16(seed) >> headroom;
        output[i].im = pseudo_rand_int16(seed) >> headroom;
    }
}

void rand_vect_complex_s32(
    complex_s32_t output[],
    const unsigned length,
    const unsigned headroom,
    unsigned* seed)
{
    for(int i = 0; i < length; i++){
        output[i].re = pseudo_rand_int32(seed) >> headroom;
        output[i].im = pseudo_rand_int32(seed) >> headroom;
    }
}


void print_warns(
    int start_case, 
    unsigned test_c, 
    unsigned test_asm)
{
    if(start_case != -1 && start_case != 0){
        printf("\t\t\t\t\t\t************************************************\n");
        printf("\t\t\t\t\t\t***** WARNING: Test not started on case 0 ******\n");
        printf("\t\t\t\t\t\t************************************************\n");
    }
    if(!test_c){
        printf("\t\t\t\t\t\t************************************************\n");
        printf("\t\t\t\t\t\t***** WARNING: Not testing C implementation ****\n");
        printf("\t\t\t\t\t\t************************************************\n");
    }
    if(!test_asm){
        printf("\t\t\t\t\t\t************************************************\n");
        printf("\t\t\t\t\t\t***** WARNING: Not testing ASM implementation **\n");
        printf("\t\t\t\t\t\t************************************************\n");
    }
}