// Copyright 2021 XMOS LIMITED. This Software is subject to the terms of the 
// XMOS Public License: Version 1
#pragma once

#include "xs3_math_conf.h"
#include "xs3_math_types.h"

#ifdef __XC__
extern "C" {
#endif


unsigned getTimestamp();

void xs3_fft_index_bit_reversal_double(
    complex_double_t* a,
    const unsigned length);







/*
 * Type conversion
 */

typedef enum {

    //In trying to convert to a integer type the exponent of the float forces
    //the mantissa of the integer type to not fit in its storage type.
    CANNOT_FIT_MANTISSA=1,

    //In trying to convert to a unsinged type the float contains a negative number.
    NEGATIVE_TO_UNSIGNED=2,

    //In trying to convert to a float type the exponent of the integer is out of range.
    CANNOT_FIT_EXPONENT=4,

} conv_error_e;

double conv_s8_to_double (const int8_t x, const exponent_t x_exp, conv_error_e *error);
double conv_u8_to_double(const uint8_t x, const exponent_t x_exp, conv_error_e *error);
double conv_s16_to_double(const int16_t x, const exponent_t x_exp, conv_error_e *error);
double conv_u16_to_double(const uint16_t x, const exponent_t x_exp, conv_error_e *error);
double conv_s32_to_double (const int32_t x, const exponent_t x_exp, conv_error_e *error);
double conv_u32_to_double(const uint32_t x, const exponent_t x_exp, conv_error_e *error);
double conv_s64_to_double (const int64_t x, const exponent_t x_exp, conv_error_e *error);
double conv_u64_to_double(const uint64_t x, const exponent_t x_exp, conv_error_e *error);


int8_t conv_double_to_s8 (double d, const exponent_t d_exp, conv_error_e *error);
uint8_t conv_double_to_u8(double d, const exponent_t d_exp, conv_error_e *error);
int16_t conv_double_to_s16 (double d, const exponent_t d_exp, conv_error_e *error);
uint16_t conv_double_to_u16(double d, const exponent_t d_exp, conv_error_e *error);
int32_t conv_double_to_s32 (double d, const exponent_t d_exp, conv_error_e *error);
uint32_t conv_double_to_u32(double d, const exponent_t d_exp, conv_error_e *error);
int64_t conv_double_to_s64 (double d, const exponent_t d_exp, conv_error_e *error);
uint64_t conv_double_to_u64(double d, const exponent_t d_exp, conv_error_e *error);

complex_double_t conv_complex_s16_to_complex_double(complex_s16_t x, const exponent_t x_exp, conv_error_e *error);
complex_double_t conv_complex_s32_to_complex_double(complex_s32_t x, const exponent_t x_exp, conv_error_e *error);
ch_pair_double_t conv_ch_pair_s16_to_ch_pair_double(ch_pair_s16_t x, const exponent_t x_exp, conv_error_e *error);
ch_pair_double_t conv_ch_pair_s32_to_ch_pair_double(ch_pair_s32_t x, const exponent_t x_exp, conv_error_e *error);
complex_s16_t conv_complex_double_to_complex_s16(complex_double_t x, const exponent_t x_exp, conv_error_e *error);
complex_s32_t conv_complex_double_to_complex_s32(complex_double_t x, const exponent_t x_exp, conv_error_e *error);
ch_pair_s16_t conv_ch_pair_double_to_ch_pair_s16(ch_pair_double_t x, const exponent_t x_exp, conv_error_e *error);
ch_pair_s32_t conv_ch_pair_double_to_ch_pair_s32(ch_pair_double_t x, const exponent_t x_exp, conv_error_e *error);


void conv_vect_s16_to_double(double output[], const int16_t input[], const unsigned length, const exponent_t input_exp, conv_error_e *error);
void conv_vect_s32_to_double(double output[], const int32_t input[], const unsigned length, const exponent_t input_exp, conv_error_e *error);
void conv_vect_complex_s16_to_complex_double(complex_double_t output[], const complex_s16_t input[], const unsigned length, const exponent_t input_exp, conv_error_e *error);
void conv_vect_complex_s32_to_complex_double(complex_double_t output[], const complex_s32_t input[], const unsigned length, const exponent_t input_exp, conv_error_e *error);
void conv_vect_complex_s32_to_complex_double_v2(double out_real[], double out_imag[], const complex_s32_t input[], const unsigned length, const exponent_t input_exp, conv_error_e *error);




/*
 * Float/Fixed comparision
 */
unsigned abs_diff_ch_pair_s16(ch_pair_s16_t B, const exponent_t B_exp,ch_pair_double_t f, int channel_index, conv_error_e* error);
unsigned abs_diff_ch_pair_s32(ch_pair_s32_t B, const exponent_t B_exp,ch_pair_double_t f, int channel_index, conv_error_e* error);
unsigned abs_diff_complex_s16(complex_s16_t B, const exponent_t B_exp, complex_double_t f, conv_error_e* error);
unsigned abs_diff_complex_s32(complex_s32_t B, const exponent_t B_exp, complex_double_t f, conv_error_e* error);
unsigned abs_diff_s8(int8_t B, const exponent_t B_exp, double f, conv_error_e* error);
unsigned abs_diff_s16( int16_t B, const exponent_t B_exp, double f, conv_error_e* error);
unsigned abs_diff_s32(int32_t B, const exponent_t B_exp, double f, conv_error_e* error);
unsigned abs_diff_u8(uint8_t B, const exponent_t B_exp, double f, conv_error_e* error);
unsigned abs_diff_u16(uint16_t B, const exponent_t B_exp, double f, conv_error_e* error);
unsigned abs_diff_u32(uint32_t B, const exponent_t B_exp, double f, conv_error_e* error);


/*
 * Float/Fixed vector comparision
 */
unsigned abs_diff_vect_ch_pair_s16(ch_pair_s16_t * B, const exponent_t B_exp, ch_pair_double_t * f, unsigned length, int channel_index, conv_error_e* error);
unsigned abs_diff_vect_ch_pair_s32(ch_pair_s32_t * B, const exponent_t B_exp, ch_pair_double_t * f, unsigned length, int channel_index, conv_error_e* error);
unsigned abs_diff_vect_complex_s16(complex_s16_t * B, const exponent_t B_exp, complex_double_t * f, unsigned length, conv_error_e* error);
unsigned abs_diff_vect_complex_s32(complex_s32_t * B, const exponent_t B_exp, complex_double_t * f, unsigned length, conv_error_e* error);
unsigned abs_diff_vect_s8( int8_t * B, const exponent_t B_exp, double * f, unsigned length,conv_error_e* error);
unsigned abs_diff_vect_s16( int16_t * B, const exponent_t B_exp, double * f, unsigned length,conv_error_e* error);
unsigned abs_diff_vect_s32( int32_t * B, const exponent_t B_exp, double * f, unsigned length,conv_error_e* error);
unsigned abs_diff_vect_u8(uint8_t * B, const exponent_t B_exp, double * f, unsigned length,conv_error_e* error);
unsigned abs_diff_vect_u16(uint16_t * B, const exponent_t B_exp, double * f, unsigned length,conv_error_e* error);
unsigned abs_diff_vect_u32(uint32_t * B, const exponent_t B_exp, double * f, unsigned length,conv_error_e* error);





#define TESTING_FLOAT_NUM_DIGITS 22

#define PRINT_VAR(VAR, THING)  do{ printf("%s = ", VAR); THING; } while(0)

void print_vect_ch_pair_s16(ch_pair_s16_t * B, const exponent_t B_exp, unsigned length, int channel_index, conv_error_e* error);
void print_vect_ch_pair_s32(ch_pair_s32_t * B, const exponent_t B_exp,unsigned length, int channel_index, conv_error_e* error);
void print_vect_ch_pair_double(ch_pair_double_t * B, unsigned length,  int channel_index, conv_error_e* error);
void print_vect_complex_s16(complex_s16_t * B, const exponent_t B_exp, unsigned length, conv_error_e* error);
void print_vect_complex_s16_fft(complex_s16_t * B, const exponent_t B_exp, unsigned length, conv_error_e* error);
void print_vect_complex_s32(complex_s32_t * B, const exponent_t B_exp, unsigned length, conv_error_e* error);
void print_vect_complex_s32_fft(complex_s32_t * B, const exponent_t B_exp, unsigned length, conv_error_e* error);
void print_vect_complex_s32_raw(complex_s32_t * B, unsigned length);
void print_vect_complex_s32_fft_raw(complex_s32_t * B, unsigned length);
void print_vect_complex_double(complex_double_t * B, unsigned length, conv_error_e* error);
void print_vect_complex_double_fft(complex_double_t * B, unsigned length, conv_error_e* error);
void print_vect_s8(int8_t * B, const exponent_t B_exp, unsigned length, conv_error_e* error);
void print_vect_s16(int16_t * B, const exponent_t B_exp, unsigned length, conv_error_e* error);
void print_vect_s32(int32_t * B, const exponent_t B_exp, unsigned length, conv_error_e* error);
void print_vect_s64(int64_t * B, const exponent_t B_exp, unsigned length, conv_error_e* error);
void print_vect_u8(uint8_t * B, const exponent_t B_exp, unsigned length, conv_error_e* error);
void print_vect_u16(uint16_t * B, const exponent_t B_exp, unsigned length, conv_error_e* error);
void print_vect_u32(uint32_t * B, const exponent_t B_exp, unsigned length, conv_error_e* error);
void print_vect_u64(uint64_t * B, const exponent_t B_exp, unsigned length, conv_error_e* error);
void print_vect_double(double * B, unsigned length, conv_error_e* error);

#define PRINT_PLOT_DIFF_COMPLEX(A, B)   \
    do{ printf("[(plt.plot(np.real(qwer),label='real'),plt.plot(np.imag(qwer),label='imag')) for qwer in [\n"); \
        A; printf(" - \n"); B; printf("]]\n"); } while(0)
        

#ifdef __XC__
}
#endif