
#include "xs3_math.h"
#include "vpu_helper.h"

static unsigned bitrev(unsigned index, size_t bit_width)
{
    unsigned res = 0;
    for(int i = 0; i < bit_width; i++, index >>= 1){
        res = ((res<<1) | (index & 0x1));
    }
    return res;
}


WEAK_FUNC
void xs3_bit_reverse_indexes(
    complex_s32_t* a,
    const unsigned length)
{
    size_t logn = ceil_log2(length);
    for(int i = 0; i < length; i++){
        
        unsigned rev = bitrev(i, logn);
        if(rev < i) continue;

        complex_s32_t tmp = a[i];
        
        a[i] = a[rev];
        a[rev] = tmp;
    }
}


WEAK_FUNC
void xs3_bit_reverse_indexes_shr(
    complex_s32_t* a,
    const unsigned length,
    const right_shift_t shr)
{
    size_t logn = ceil_log2(length);
    for(int i = 0; i < length; i++){
        
        unsigned rev = bitrev(i, logn);
        if(rev < i) continue;

        complex_s32_t tmp1 = { ASHR(32)(a[i  ].re, shr), ASHR(32)(a[i  ].im, shr) };
        complex_s32_t tmp2 = { ASHR(32)(a[rev].re, shr), ASHR(32)(a[rev].im, shr) };
        
        a[i]   = tmp2;
        a[rev] = tmp1;
    }
}


WEAK_FUNC
void xs3_merge_time_series_s32(
    complex_s32_t* a,
    const int32_t* b,
    const int32_t* c,
    const unsigned length,
    const right_shift_t b_shr,
    const right_shift_t c_shr)
{
    for(int i = 0; i < length; i++){
        a[i].re = ASHR(32)(b[i], b_shr);
        a[i].im = (c == NULL)? 0 : ASHR(32)(c[i], c_shr);
    }
}



WEAK_FUNC
void xs3_s32_to_complex_s32(
    complex_s32_t* a,
    const int32_t* b,
    const unsigned length,
    const right_shift_t b_shr)
{
    xs3_merge_time_series_s32(a, b, NULL, length, b_shr, 0);
}

void xs3_real_part_complex_s32(
    int32_t* a,
    const complex_s32_t* b,
    const unsigned length,
    const right_shift_t b_shr)
{
    for(int i = 0; i < length; i++){
        a[i] = ASHR(32)(b[i].re, b_shr);
    }
}

void xs3_imag_part_complex_s32(
    int32_t* a,
    const complex_s32_t* b,
    const unsigned length,
    const right_shift_t b_shr)
{
    for(int i = 0; i < length; i++){
        a[i] = ASHR(32)(b[i].im, b_shr);
    }
}


WEAK_FUNC
headroom_t xs3_split_fft_spectrum_s32(
    complex_s32_t* X,
    const unsigned N)
{
    const unsigned K = N/2;

    //First, reverse order of X[N/2+1:N]
    for(unsigned f = 1; f < N/4; f++){
        const complex_s32_t tmp = X[K+f];
        X[K+f] = X[N-f];
        X[N-f] = tmp;
    }

    //For spectrum of real signal, the DC and Nyquist bins must both be purely real. We will stuff
    // the Nyquist rate value into the imaginary part of the DC bin.
    complex_s32_t X0 = X[0];
    complex_s32_t XN = X[K];
    
    // If we change [X[0].re, X[0].im, X[K].re, X[K].im] to be this:
    //  [DC.re - Ny.im,  Ny.re + DC.im,   DC.re + Ny.im,  -Ny.re + DC.im  ]  
    // then we can just compute those bins in the loop below. Not very important
    // here, but makes the assembly much faster.
    // This does unfortunately introduce the requirement that the input have at least 1 bit of headroom.
    X[0].re =  X0.re - XN.im;
    X[0].im =  X0.im + XN.re;
    X[K].re =  X0.re + XN.im;
    X[K].im =  X0.im - XN.re;

    // X[0].re = X0.re;
    // X[0].im = XN.re;
    // X[K].re = X0.im;
    // X[K].im = XN.im;


    //Now split the spectrum
    for(unsigned f = 0; f < K; f++){
        //A[f] = ((X[f].re + X[-f].re) + j*(X[f].im - X[-f].im)) / 2
        //B[f] = ((X[f].im + X[-f].im) - j*(X[f].re - X[-f].re)) / 2
        //X[-f] = X[-f+N] because X[f] is periodic with period N

        complex_s32_t Xp = { X[0+f].re>>1, X[0+f].im>>1 };
        complex_s32_t Xn = { X[K+f].re>>1, X[K+f].im>>1 };

        X[0+f].re =  Xp.re + Xn.re;
        X[0+f].im =  Xp.im - Xn.im;
        X[K+f].re =  Xp.im + Xn.im;
        X[K+f].im = -Xp.re + Xn.re;

        // Really hard to add/sub real parts from imaginary parts using the VPU because those
        // aren't aligned together nicely in memory. But we can do some complex arithmetic...

        //X[f]   = Xp.re + Xn.re + j*(Xp.im - Xn.im)
        //       = conj(Xn) + Xp

        //X[K+f] = Xp.im + Xn.im + j*(-Xp.re + Xn.re)
        //       = Xp.im + Xn.im - j*Xp.re + j*Xn.re
        //       = -j*Xp.re+Xp.im + j*Xn.re + Xn.im
        //       = -j*Xp + j*Xn.re + -j*j*Xn.im
        //       = -j*Xp + j*(Xn.re - j*Xn.im)
        //       = -j*Xp + j*conj(Xn)
        //       = j*(conj(Xn) - Xp)

        // Note:  j*conj(z) = -conj(j*z)
    }

    return xs3_headroom_vect_s32((int32_t*)X, 2*N);
}

WEAK_FUNC
headroom_t xs3_merge_fft_spectra_s32(
    complex_s32_t* X,
    const unsigned N)
{
    const unsigned K = N/2;

    {
        //Pre-boggle DC and Nyquist
        complex_s32_t DC = { X[0].re>>1, X[0].im>>1 };
        complex_s32_t Ny = { X[K].re>>1, X[K].im>>1 };

        X[0].re =  DC.re + DC.im;
        X[0].im =  Ny.re - Ny.im;
        X[K].re =  Ny.re + Ny.im;
        X[K].im = -DC.re + DC.im;

    }

    for(unsigned f = 0; f < N/2; f++){
        complex_s32_t a = X[0+f];
        complex_s32_t b = X[K+f];

        X[0+f].re = a.re - b.im;
        X[0+f].im = a.im + b.re;
        X[K+f].re = b.im + a.re;
        X[K+f].im = b.re - a.im;

        //X[f]   = (a.re-b.im) + j*(a.im+b.re)
        //       = a + j*b.re - b.im
        //       = a + j*b

        //X[K+f] = (a.re+b.im) - j*(a.im+b.re)
        //       = a.re - j*a.im + j*b.re + b.im
        //       = conj(a) + j*conj(b)
        //       = conj(a) - conj(j*b)
        //       = conj(a - j*b)


    }

    for(unsigned f = 1; f < N/4; f++){
        complex_s32_t tmp = X[K+f];
        X[K+f] = X[N-f];
        X[N-f] = tmp;
    }

    return xs3_headroom_vect_s32((int32_t*)X, 2*N);
}