// Copyright 2020-2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>

#include "xmath/xmath.h"


/*
 * This can definitely be optimized a decent bit if done in assembly.
 * 
 * Comparison to q24_logistics_fast():
 *                              Time           Max Observed Error
 *    q24_logistics              10t                 36
 *    q24_logistics_fast          t                110280
 * 
 */ 
q8_24 q24_logistic(
    const q8_24 x)
{

  unsigned is_neg = x < 0;
  q8_24 pos_x = is_neg? ~x : x;
  q8_24 result = 0;

  if(pos_x <= Q24(1.2)){
    // Compute using the power series expansion of the logistics function

    // (note: the 0 is to mask out an 8th element from the dot product)
    const q2_30 logi_ps_coef[9] = {
      Q30(0.5), Q30(0.25), Q30(-0.020833333333333332), Q30(0.0020833333333333333),
      Q30(-0.00021081349206349207), Q30(2.1356922398589065e-05), 
      Q30(-2.1638758617925286e-06), 0 };

    // initializing 8 elements even though only using 7 because of the VPU len
    q8_24 odd_powers[8] = {0};

    result = logi_ps_coef[0] >> 6;

    s32_odd_powers(odd_powers, pos_x, 7, 24);
    result += chunk_s32_dot(&odd_powers[0], &logi_ps_coef[1]);

  } else {
    // Compute using the power series expansion of 1/(1-y), where 
    //  y = exp(-x)

    float_s32_t xx = {-pos_x, -24};
    float_s32_t yy = float_s32_exp(xx);

    // printf("xx = %f  (%ld << %d)\n", ldexp(xx.mant, xx.exp), xx.mant, xx.exp);
    // printf("yy = %f  (%ld << %d)\n", ldexp(yy.mant, yy.exp), yy.mant, yy.exp);

    //convert to Q24
    right_shift_t shr = -24 - yy.exp;

    q8_24 y = (shr >= 0)? (yy.mant >> shr) 
                        : (yy.mant << (-shr));

    // The power series expansion for 1/(1-y) is just the geometric 
    // series summed. But we're actually trying to figure out 1/(1+y), so
    // we take the negative of y.
    y = -y;
    q8_24 y_pow = Q24(1.0);

    result = Q24(1.0);
    
    for(int k = 0; k < 10; k++){
      y_pow = (((int64_t)y_pow) * y) >> 24;
      result += y_pow;
    }
  }

  if(is_neg){
    // logistics(-x) = 1.0 - logistics(x)
    result = Q24(1.0) - result;
  }
  
  return result;
}
