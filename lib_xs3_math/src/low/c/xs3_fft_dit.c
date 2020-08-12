
#include <stdint.h>
#include <stdio.h>

#include "xs3_math.h"
#include "vpu_helper.h"


#include "xs3_math.h"
#include "vpu_helper.h"

static complex_s32_t mul_complex_s32(complex_s32_t b, complex_s32_t c, int b_shr, int c_shr)
{
    int64_t bp_re = b.re;
    int64_t bp_im = b.im;
    int64_t cp_re = c.re;
    int64_t cp_im = c.im;

    bp_re = ASHR(32)(bp_re, b_shr);
    bp_im = ASHR(32)(bp_im, b_shr);

    cp_re = ASHR(32)(cp_re, c_shr);
    cp_im = ASHR(32)(cp_im, c_shr);

    bp_re = SAT(32)(bp_re);
    bp_im = SAT(32)(bp_im);

    cp_re = SAT(32)(cp_re);
    cp_im = SAT(32)(cp_im);

    // printf("B = %ld\t%ld\t\t%d\n", b.re, b.im, b_shr);
    // printf("C = %ld\t%ld\t\t%d\n", c.re, c.im, c_shr);

    int64_t q1 = ((bp_re * cp_re) + (1<<29)) >> 30;
    int64_t q2 = ((bp_im * cp_im) + (1<<29)) >> 30;
    int64_t q3 = ((bp_re * cp_im) + (1<<29)) >> 30;
    int64_t q4 = ((bp_im * cp_re) + (1<<29)) >> 30;

    int64_t a_re = (q1 - q2);
    int64_t a_im = (q3 + q4);

    // int64_t a_re = ((bp_re * cp_re - bp_im * cp_im) + (1<<29)) >> 30;
    // int64_t a_im = ((bp_re * cp_im + bp_im * cp_re) + (1<<29)) >> 30;

    a_re = SAT(32)(a_re);
    a_im = SAT(32)(a_im);

    complex_s32_t res = {a_re, a_im};
    return res;
}


WEAK_FUNC
void xs3_real_fft_final_pass_s32(
    complex_s32_t* X,
    const unsigned length,
    headroom_t* X_hr,
    exponent_t* X_exp,
    const complex_s32_t* W)
{
    //length is the length of the real sequence that was FFTed. Because the operation is happening
    // in-place, we only have room for length/2 complex FFT bins. But because it's a real FFT, the
    // symmetry properties mean that length/2 is all we need. We'll stuff the real part of the 
    // Nyquist rate into the imaginary part of DC.

    // G and H are the spectra of the even and odd subsequences of the original real signal
    const unsigned GH_length = length/4;
    const unsigned X_length = length/2;
    complex_s32_t* G = &X[0];
    complex_s32_t* H = &X[GH_length];

    const right_shift_t X_shr = 1;

    *X_exp += X_shr;

    {
        const complex_s32_t g0 = {ASHR(32)(G[0].re, X_shr), ASHR(32)(G[0].im, X_shr)};
        const complex_s32_t h0 = {ASHR(32)(H[0].re, X_shr), ASHR(32)(H[0].im, X_shr)};

        // The DC and Nyquist rates for X[] will go into the real and imaginary parts of X[0]. But the imaginary 
        // part of G[0] and H[0] actually store the Nyquist rates for G[] and H[], and since this update is happening
        // in-place, overwriting X[0].im with X[]'s Nyquist rate will overwrite the Nyquist rate for G[]. So, we also
        // need to compute X[GH_length] here, while we still have G[]'s Nyquist rate info.
        // X[GH_length] happens to also be the location of H[0].
        X[0].re = g0.re + h0.re;
        X[0].im = g0.re - h0.re;

        // X[GH_length] = G[GH_length] + W[GH_length] * H[GH_length] = G[0].im + W[GH_length] * H[0].im
        // W[GH_length] = W[N / 4] =  exp(-2*pi*j*(N/4)/N) = exp(-(pi/2)*j) = -j
        // X[GH_length] = G[0].im - j*H[0].im
        // (note: GH_length is Nyquist for G and H, and so is pure real for G and H, but GH_length is NOT Nyquist rate
        //        for X, so we shouldn't expect X[GH_length] to be pure real.)

        X[GH_length].re = g0.im;
        X[GH_length].im = -h0.im;
        const complex_s32_t tmp1 = {g0.im, 0};
        const complex_s32_t tmp2 = {h0.im, 0};
    }

    // If X[] is the output spectrum, then X[k] = G[k] + W[k]*H[k]. The only problem is that X[] is twice as long as
    //  G[] or H[]. There is a symmetry in G[] and H[] that we can use, though. 
    
    // G[GH_length + k] = G[GH_length - k]*     for 0 <= k < GH_length
    // H[GH_length + k] = H[GH_length - k]*     for 0 <= k < GH_length

    // We have to be careful about what memory is being written over as we go through this data so that we aren't 
    // treading over data we will need. 
    // We never need to compute X[k] for k >= X_length (X's nyquist rate and above)
    // But because   X[k] depends on G[k] and H[k], and  G[GH_length + k] = G[GH_length -k]*, both 
    //   X[GH_length - k] and X[GH_length + k] depend on the same elements G[GH_length-k] and H[GH_length-k].
    //  so they'll need to be computed at the same time.

    for(int k = 1; k < GH_length; k++){

        const unsigned m = (X_length - k);

        complex_s32_t g = {ASHR(32)(G[k].re, X_shr), ASHR(32)(G[k].im, X_shr)};
        complex_s32_t h = H[k];
        complex_s32_t tmp;

        // X[k] = G[k] + W[k]*H[k]
        tmp = mul_complex_s32(h, W[k], X_shr, 0);

        tmp.re = g.re + tmp.re;
        tmp.im = g.im + tmp.im;

        X[k] = tmp;

        // X[m] = G[m] + W[m]*H[m]
        //      = G[k]* + W[m]*(H[k]*)
        h.im = -h.im;
        tmp = mul_complex_s32(h, W[m], X_shr, 0);

        tmp.re =  g.re + tmp.re;
        tmp.im = -g.im + tmp.im;

        X[m] = tmp;
    }

    *X_hr = xs3_headroom_vect_s32((int32_t*) X, length);
}