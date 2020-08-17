

#include "xs3_math.h"
#include "../src/low/c/twiddle_factors.h"
#include "../low/c/vpu_helper.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

//Use decimation-in-time if true, decimation-in-frequency otherwise
#define USE_DIT 1





void bfp_real_fft_forward(
    bfp_complex_s32_t* a,
    bfp_s32_t* b)
{
    // b is initialized, a is not.
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    //astew: TODO: check that length is a power of 2.
#else
#endif

    right_shift_t b_shr = 2 - b->hr;


    //Reinterpret the real sequence as complex (which puts odd-indexed elements into the imaginary part)
    // then perform a complex N/2-point FFT on that.
    xs3_shl_vect_s32(b->data, b->data, b->length, -b_shr);
    // for(int k = 0; k < b->length; k++)
    //     b->data[k] = ASHR(32)(b->data[k], b_shr);

    a->hr  = b->hr  + b_shr;
    a->exp = b->exp + b_shr;
    a->data = (complex_s32_t*) b->data;
    a->length = b->length/2;

    xs3_bit_reverse_indexes((complex_s32_t*) b->data, b->length/2);
    xs3_fft_dit_s32(a->data, b->length/2, &a->hr, xs3_dit_fft_lut, &a->exp);

    xs3_real_fft_final_pass_s32(a->data, b->length, XS3_DIT_REAL_FFT_LUT(b->length));

}





void bfp_real_fft_inverse(
    bfp_s32_t* a,
    bfp_complex_s32_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
#else
#endif

    const unsigned N = 2*b->length;
    
    right_shift_t b_shr = 2 - b->hr;
    xs3_shl_vect_s32((int32_t*) b->data, (int32_t*) b->data, N, -b_shr);
    
    a->hr  = b->hr  + b_shr;
    a->exp = b->exp + b_shr;
    a->data = (int32_t*) b->data;
    a->length = N;

    xs3_real_ifft_first_pass_s32(b->data, N, XS3_DIT_REAL_FFT_LUT(N));

    xs3_bit_reverse_indexes(b->data, b->length);
    xs3_ifft_dit_s32(b->data, b->length, &a->hr, xs3_dit_fft_lut, &a->exp);
}












void bfp_complex_fft_forward(
    bfp_complex_s32_t* samples)
{
    //The FFT implementation unfortunately requires 2 bits of headroom to avoid saturation.
    if(samples->hr < 2){
        left_shift_t shl = samples->hr - 2;
        samples->hr = xs3_shl_vect_s32((int32_t*) samples->data,(int32_t*)  samples->data, 2*samples->length, shl);
        samples->exp -= shl;
    }

    xs3_bit_reverse_indexes(samples->data, samples->length);
    xs3_fft_dit_s32(samples->data, samples->length, &samples->hr, xs3_dit_fft_lut, &samples->exp);
}





void bfp_complex_fft_inverse(
    bfp_complex_s32_t* spectrum)
{
    //The FFT implementation unfortunately requires 2 bits of headroom to avoid saturation.
    if(spectrum->hr < 2){
        left_shift_t shl = spectrum->hr - 2;
        spectrum->hr = xs3_shl_vect_s32((int32_t*) spectrum->data,(int32_t*)  spectrum->data, 2*spectrum->length, shl);
        spectrum->exp -= shl;
    }   

    xs3_bit_reverse_indexes(spectrum->data, spectrum->length);
    xs3_ifft_dit_s32(spectrum->data, spectrum->length, &spectrum->hr, xs3_dit_fft_lut, &spectrum->exp);
}














void bfp_dual_fft_forward(
    bfp_complex_s32_t* a,
    bfp_complex_s32_t* b,
    bfp_ch_pair_s32_t* input)
{
    //The FFT implementation unfortunately requires 2 bits of headroom to avoid saturation.
    right_shift_t input_shr = 2 - input->hr;
    input->hr += input_shr;
    input->exp += input_shr;
    
    if(input_shr)
        input->hr = xs3_shl_vect_s32((int32_t*) input->data,(int32_t*)  input->data, 2*input->length, -input_shr);
    xs3_bit_reverse_indexes((complex_s32_t*) input->data, input->length);
    xs3_fft_dit_s32((complex_s32_t*) input->data, input->length, &input->hr, xs3_dit_fft_lut, &input->exp); 

    a->data = (complex_s32_t*) &input->data[0];
    b->data = (complex_s32_t*) &input->data[input->length/2];
    a->hr = xs3_split_fft_spectrum_s32(a->data, input->length);

    //a and b might actually have different headroom, but the function can only compute them together. In any case, it
    // will be the lesser of the two
    b->hr = a->hr;

    a->length = b->length = input->length / 2;
    a->exp = b->exp = input->exp;
}






void bfp_dual_fft_inverse(
    bfp_ch_pair_s32_t* a,
    const bfp_complex_s32_t* b,
    const bfp_complex_s32_t* c)
{
    //TODO: if b and c don't point to the same data as a, flag error or copy b and c to a first.

    //b and c need to be given the same exponent for this to make sense.
    right_shift_t b_shr = 2 - b->hr;
    right_shift_t c_shr = 2 - c->hr;

    exponent_t b_exp = b->exp + b_shr;
    exponent_t c_exp = c->exp + c_shr;

    if( b_exp <= c_exp ){
        b_shr += (c_exp - b_exp);
        a->exp = b->exp + b_shr;
    } else {
        c_shr += (b_exp - c_exp);
        a->exp = c->exp + c_shr;
    }

    if(b_shr)
        xs3_shl_vect_s32((int32_t*)a->data, (int32_t*) b->data, 2*b->length, -b_shr);
    if(c_shr)
        xs3_shl_vect_s32((int32_t*)&(a->data[a->length/2]), (int32_t*) c->data, 2*c->length, -c_shr);

    xs3_merge_fft_spectra_s32((complex_s32_t*) a->data, a->length);
    xs3_bit_reverse_indexes((complex_s32_t*) a->data, a->length);
    xs3_ifft_dit_s32((complex_s32_t*) a->data, a->length, &a->hr, xs3_dit_fft_lut, &a->exp);
    
}