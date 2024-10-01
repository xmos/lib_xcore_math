// Copyright 2020-2022 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include "floating_fft.h"

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


unsigned flt_bitrev(
    const unsigned index,
    const size_t bit_width)
{
    unsigned inp = index;
    unsigned res = 0;

    for(unsigned int i = 0; i < bit_width; i++, inp >>= 1){
        res = ((res<<1) | (inp & 0x1));
    }

    return res;
}

// {double, double} dsp_poly_interpolate(double left, double peak, double right, double center_bin){
//     double bin_offset = (left - right) / (2. * (2. * peak - left -right));
//     double new_peak = peak - 0.25 * (left - right) * bin_offset;

//     return {center_bin + bin_offset, new_peak};
// }
