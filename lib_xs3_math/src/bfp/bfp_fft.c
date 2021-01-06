

#include "bfp_math.h"
#include "../vect/xs3_fft_lut.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>



bfp_complex_s32_t* bfp_fft_forward_mono(
    bfp_s32_t* x)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    // Length must be 2^p where p is a non-negative integer
    assert(x->length != 0);
    // for a positive power of 2, subtracting 1 should increase its headroom.
    assert(cls(x->length - 1) > cls(x->length)); 
#endif

    bfp_complex_s32_t* X = (bfp_complex_s32_t*) x;

    const unsigned FFT_N = x->length;

    right_shift_t x_shr = 2 - x->hr;

    xs3_vect_s32_shl(x->data, x->data, x->length, -x_shr);

    x->hr  = x->hr  + x_shr;
    x->exp = x->exp + x_shr;
    x->length = FFT_N/2;

    xs3_fft_index_bit_reversal(X->data, X->length);
    xs3_fft_dit_forward(X->data, X->length, &X->hr, &X->exp);

    xs3_fft_mono_adjust(X->data, FFT_N, 0);

    return X;
}





bfp_s32_t* bfp_fft_inverse_mono(
    bfp_complex_s32_t* X)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(X->length != 0);
    assert(cls(X->length - 1) > cls(X->length)); 
#endif

    const unsigned FFT_N = 2*X->length;
    bfp_s32_t* x = (bfp_s32_t*)X;
    
    right_shift_t X_shr = 2 - X->hr;
    xs3_vect_s32_shl((int32_t*) X->data, (int32_t*) X->data, FFT_N, -X_shr);
    
    X->hr  = X->hr  + X_shr;
    X->exp = X->exp + X_shr;
    X->length = FFT_N;

    xs3_fft_mono_adjust(X->data, FFT_N, 1);

    xs3_fft_index_bit_reversal(X->data, FFT_N/2);
    xs3_fft_dit_inverse(X->data, FFT_N/2, &x->hr, &x->exp);

    return x;
}












void bfp_fft_forward_complex(
    bfp_complex_s32_t* samples)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(samples->length != 0);
    assert(cls(samples->length - 1) > cls(samples->length)); 
#endif

    //The FFT implementation unfortunately requires 2 bits of headroom to avoid saturation.
    if(samples->hr < 2){
        left_shift_t shl = samples->hr - 2;
        samples->hr = xs3_vect_s32_shl((int32_t*) samples->data,(int32_t*)  samples->data, 2*samples->length, shl);
        samples->exp -= shl;
    }

    xs3_fft_index_bit_reversal(samples->data, samples->length);
    xs3_fft_dit_forward(samples->data, samples->length, &samples->hr, &samples->exp);
}





void bfp_fft_inverse_complex(
    bfp_complex_s32_t* spectrum)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(spectrum->length != 0);
    assert(cls(spectrum->length - 1) > cls(spectrum->length)); 
#endif

    //The FFT implementation unfortunately requires 2 bits of headroom to avoid saturation.
    if(spectrum->hr < 2){
        left_shift_t shl = spectrum->hr - 2;
        spectrum->hr = xs3_vect_s32_shl((int32_t*) spectrum->data,(int32_t*)  spectrum->data, 2*spectrum->length, shl);
        spectrum->exp -= shl;
    }   

    xs3_fft_index_bit_reversal(spectrum->data, spectrum->length);
    xs3_fft_dit_inverse(spectrum->data, spectrum->length, &spectrum->hr, &spectrum->exp);
}














void bfp_fft_forward_stereo(
    bfp_complex_s32_t* a,
    bfp_complex_s32_t* b,
    bfp_ch_pair_s32_t* input)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(input->length != 0);
    assert(cls(input->length - 1) > cls(input->length)); 
#endif

    //The FFT implementation unfortunately requires 2 bits of headroom to avoid saturation.
    right_shift_t input_shr = 2 - input->hr;
    input->hr += input_shr;
    input->exp += input_shr;
    
    if(input_shr)
        input->hr = xs3_vect_s32_shl((int32_t*) input->data,(int32_t*)  input->data, 2*input->length, -input_shr);
    xs3_fft_index_bit_reversal((complex_s32_t*) input->data, input->length);
    xs3_fft_dit_forward((complex_s32_t*) input->data, input->length, &input->hr, &input->exp); 

    a->data = (complex_s32_t*) &input->data[0];
    b->data = (complex_s32_t*) &input->data[input->length/2];
    a->hr = xs3_fft_spectra_split(a->data, input->length);

    //a and b might actually have different headroom, but the function can only compute them together. In any case, it
    // will be the lesser of the two
    b->hr = a->hr;

    a->length = b->length = input->length / 2;
    a->exp = b->exp = input->exp;
}






void  bfp_fft_inverse_stereo(
    bfp_ch_pair_s32_t* x,
    const bfp_complex_s32_t* a,
    const bfp_complex_s32_t* b)
{
#if (XS3_BFP_DEBUG_CHECK_LENGTHS)
    assert(x->length != 0);
    assert(cls(x->length - 1) > cls(x->length)); 
    
    assert(a->length == b->length);
    assert(b->data == &a->data[a->length]);
#endif

    //@todo: if b and c don't point to the same data as a, flag error or copy b and c to a first.

    const unsigned FFT_N = 2*a->length;

    //b and c need to be given the same exponent for this to make sense.
    right_shift_t a_shr = 2 - a->hr;
    right_shift_t b_shr = 2 - b->hr;

    exponent_t a_exp = a->exp + a_shr;
    exponent_t b_exp = b->exp + b_shr;

    if( a_exp <= b_exp ){
        a_shr += (b_exp - a_exp);
        x->exp = a->exp + a_shr;
    } else {
        b_shr += (a_exp - b_exp);
        x->exp = b->exp + b_shr;
    }

    if(a_shr) xs3_vect_s32_shl((int32_t*)a->data, (int32_t*) a->data, FFT_N, -a_shr);
    if(b_shr) xs3_vect_s32_shl((int32_t*)b->data, (int32_t*) b->data, FFT_N, -b_shr);

    x->data = (ch_pair_s32_t*) a->data;
    x->length = a->length;
    x->hr = MIN(a->hr + a_shr, b->hr + b_shr);

    xs3_fft_spectra_merge((complex_s32_t*) x->data, FFT_N);
    xs3_fft_index_bit_reversal((complex_s32_t*) x->data, FFT_N);
    xs3_fft_dit_inverse((complex_s32_t*) x->data, FFT_N, &x->hr, &x->exp);
    
}